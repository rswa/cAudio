// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cAudioManager.h"
#include "cAudio.h"
#include "cAudioSource.h"
#include "cAudioPlatform.h"
#include "cAudioSleep.h"
#include "cUtils.h"
#include "cThread.h"
#include "cLogger.h"
#include "cPluginManager.h"
#include "cThread.h"
#include "cMemorySource.h"
#include "cRawAudioDecoderFactory.h"
#include <string.h>
#include <algorithm>
#include "cOpenALDeviceContext.h"

namespace cAudio
{
	cAudioManager::~cAudioManager() 
	{ 			
		shutDown();
	}

    bool cAudioManager::initialize(const char* deviceName, int outputFrequency, int eaxEffectSlots)
    {
		cAudioMutexBasicLock lock(Mutex);

		if(Initialized)
			return false;

		AudioContext = CAUDIO_NEW cOpenALDeviceContext(this);

		if(!AudioContext->initialize(deviceName, outputFrequency, eaxEffectSlots))
			return false;

#ifdef CAUDIO_USE_INTERNAL_THREAD
		if (!AudioThread)
		{
			AudioThread = new cAudioThread(this);
		}
		AudioThread->start();
#endif
		Initialized = true;
		return true;
    }

	void cAudioManager::shutDown()
	{
		if(Initialized)
		{
			if (AudioThread) // First wait for our update thread to finish up
			{
				AudioThread->join();
				delete AudioThread;
				AudioThread = NULL;
			} 

			cAudioMutexBasicLock lock(Mutex);

			releaseAllSources();
			
			if (AudioContext)
			{
				AudioContext->shutDown();
				CAUDIO_DELETE AudioContext;
				AudioContext = NULL;
			}

			Initialized = false;

			getLogger()->logInfo("AudioManager", "Manager successfully shutdown.");
		}
	}

	void cAudioManager::update()
	{
		cAudioMutexBasicLock lock(Mutex);
		size_t count = audioSources.size();
		for(size_t i=0; i<count; i++)
		{
			IAudioSource* source = audioSources[i];
			if (source->isValid())
			{
				source->update();
			}
		}

		if (!managedAudioSources.empty())
		{
			count = managedAudioSources.size();
			for(size_t i=0; i<count; i++)
			{
				IAudioSource* source = managedAudioSources[i];
				if (source->isValid())
				{
					source->update();
					if (source->isStopped())
					{
						managedAudioSourcesDelBuffer.push_back(source);
					}
				}
			}

			count = managedAudioSourcesDelBuffer.size();
			for(size_t i=0; i<count; i++)
			{
				IAudioSource* source = managedAudioSourcesDelBuffer[i];
				cAudioVector<IAudioSource*>::Type::iterator it = std::find(managedAudioSources.begin(), managedAudioSources.end(), source);
				if (it != managedAudioSources.end())
				{
					managedAudioSources.erase(it);
					CAUDIO_DELETE source;
				}
			}
			managedAudioSourcesDelBuffer.clear();
		}		
	}

	void cAudioManager::run()
	{
		if(!audioSources.empty()) 
			update();
		
		cAudioSleep(1);
	}

	bool cAudioManager::isUpdateThreadRunning() 
	{
		return (AudioThread != NULL && AudioThread->isRunning());
	}

#if CAUDIO_EFX_ENABLED == 1
	IAudioEffects* cAudioManager::getEffects()
	{
		if (AudioContext)
		{
			return AudioContext->getEffects();
		}
		return NULL;
	}
#endif

	IAudioSource* cAudioManager::play2D(const char* filename, bool playLooped, bool startPaused)
	{
		cAudioMutexBasicLock lock(Mutex);
		IAudioSource* pAudioSrc = create("", filename, true);

		if (!playLooped && !startPaused)
		{
			cAudioVector<IAudioSource*>::Type::iterator it = std::find(audioSources.begin(), audioSources.end(), pAudioSrc);
			if (it != audioSources.end())
			{
				audioSources.erase(it);
			}
			managedAudioSources.push_back(pAudioSrc);

			pAudioSrc->play2d();
			return NULL;			 
		}

		if (playLooped && !startPaused)
		{
			pAudioSrc->play2d(true);
		}

		return pAudioSrc;
	}

	IAudioSource* cAudioManager::play3D(const char* filename, cVector3 position, bool playLooped, bool startPaused)
	{
		cAudioMutexBasicLock lock(Mutex);
		IAudioSource* pAudioSrc = create("", filename, true);

		if (!playLooped && !startPaused)
		{
			cAudioVector<IAudioSource*>::Type::iterator it = std::find(audioSources.begin(), audioSources.end(), pAudioSrc);
			if (it != audioSources.end())
			{
				audioSources.erase(it);
			}
			managedAudioSources.push_back(pAudioSrc);

			pAudioSrc->play3d(position);
			return NULL;			 
		}

		if (playLooped && !startPaused)
		{
			pAudioSrc->play3d(position, 1.0, true);
		}

		return pAudioSrc;
	}

	void cAudioManager::setMasterVolume(float vol)
	{
		cAudioMutexBasicLock lock(Mutex);
		MasterVolume = vol;
		size_t count = audioSources.size();
		for(size_t i=0; i<count; i++)
		{
			audioSources[i]->setVolume(audioSources[i]->getVolume());
		}
	}

	float cAudioManager::getMasterVolume() const
	{
		return MasterVolume;
	}

	void cAudioManager::stopAllSounds() 
	{
		cAudioMutexBasicLock lock(Mutex);
		size_t count = audioSources.size();
		for(size_t i=0; i<count; i++)
		{
			if(audioSources[i]->isPlaying())
				audioSources[i]->stop();
		}
	}

	IAudioSource* cAudioManager::createAudioSource(IAudioDecoder* decoder, const cAudioString& audioName, const cAudioString& dataSource)
	{
		if(decoder && decoder->isValid())
		{
#if CAUDIO_EFX_ENABLED == 1
			IAudioSource* audio = CAUDIO_NEW cAudioSource(decoder, AudioContext, ((cAudioEffects*)getEffects())->getEFXInterface());
#else
			IAudioSource* audio = CAUDIO_NEW cAudioSource(decoder, AudioContext);
#endif
			decoder->drop();

			if(audio && audio->isValid())
			{
				if(!audioName.empty())
					audioIndex[audioName] = audio;

				audioSources.push_back(audio);		
				getLogger()->logInfo("AudioManager", "Audio Source (%s) created from Data Source %s.", audioName.c_str(), dataSource.c_str());
				return audio;
			}

			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Error creating audio source.", audioName.c_str());
			audio->drop();
			return NULL;
		}
		getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data could not be decoded by (.%s) decoder.", audioName.c_str(), decoder->getType().c_str());
		decoder->drop();
		return NULL;
	}

    IAudioSource* cAudioManager::create(const char* name, const char* filename, bool stream)
    {
		if(!Initialized) return NULL;

		cAudioMutexBasicLock lock(Mutex);
		cAudioString audioName = safeCStr(name);
		cAudioString path = safeCStr(filename);
		cAudioString ext = getExt(path);
		IAudioDecoderFactory* factory = getAudioDecoderFactory(ext.c_str());

		if(!factory) {
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): No decoder could be found for (.%s).", audioName.c_str(), ext.c_str());
			return NULL;
		}

		for(size_t i=0; i<dataSourcePriorityList.size(); ++i)
		{
			const cAudioString dataSourceName = dataSourcePriorityList[i].second;
			IDataSourceFactory* dataFactory = datasourcemap[dataSourceName];
			if(dataFactory)
			{
				IDataSource* source = dataFactory->CreateDataSource(filename, stream);
				if(source && source->isValid())
				{
					IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
					source->drop();

					IAudioSource* audio = createAudioSource(decoder, audioName, dataSourceName);
					if(audio != NULL)
						return audio;

					if(source)
						source->drop();

					return NULL;
				}
			}
		}		
		return NULL;
    }

    IAudioSource* cAudioManager::createFromMemory(const char* name, const char* data, size_t length, const char* extension)
    {
		if(!Initialized) return NULL;

		cAudioMutexBasicLock lock(Mutex);
		cAudioString audioName = safeCStr(name);
		cAudioString ext = safeCStr(extension);
		IAudioDecoderFactory* factory = getAudioDecoderFactory(ext.c_str());

		if(!factory) {
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Codec (.%s) is not supported.", audioName.c_str(), ext.c_str());
			return NULL;
		}

		cMemorySource* source = CAUDIO_NEW cMemorySource(data, length, true);
		if(source && source->isValid())
		{
			IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
			source->drop();

			IAudioSource* audio = createAudioSource(decoder, audioName, "cMemorySource");
			if(audio != NULL)
				return audio;

			if(source)
				source->drop();
		}
		return NULL;
    }

	IAudioSource* cAudioManager::createFromAudioBuffer(const char* name, AudioCaptureBuffer* pBiffer, unsigned int frequency, AudioFormats format)
	{
		return createFromRaw(name, pBiffer->getWriteBuffer(), pBiffer->getLength(), frequency, format);
	}

	IAudioSource* cAudioManager::createFromRaw(const char* name, const char* data, size_t length, unsigned int frequency, AudioFormats format)
	{
		if(!Initialized) return NULL;

		cAudioMutexBasicLock lock(Mutex);
		cAudioString audioName = safeCStr(name);
		IAudioDecoderFactory* factory = getAudioDecoderFactory("raw");

		if(!factory) {
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Codec (.raw) is not supported.", audioName.c_str());
			return NULL;
		}

		cMemorySource* source = CAUDIO_NEW cMemorySource(data, length, true);
		if(source && source->isValid())
		{
			IAudioDecoder* decoder = ((cRawAudioDecoderFactory*)factory)->CreateAudioDecoder(source, frequency, format);
			source->drop();

			IAudioSource* audio = createAudioSource(decoder, audioName, "cMemorySource");
			if(audio != NULL)
				return audio;

			if(source)
				source->drop();
		}
		return NULL;
	}

    bool cAudioManager::registerAudioDecoder(IAudioDecoderFactory* factory, const char* extension)
    {
		cAudioMutexBasicLock lock(Mutex);
		cAudioString ext = safeCStr(extension);
        decodermap[ext] = factory;
		getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s registered.", ext.c_str());
		return true;
    }

	void cAudioManager::unRegisterAudioDecoder(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString ext = safeCStr(extension);
		decodermapIterator it = decodermap.find(ext);
		if(it != decodermap.end())
		{
			decodermap.erase(it);
			getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s unregistered.", ext.c_str());
		}
	}

	bool cAudioManager::isAudioDecoderRegistered(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString ext = safeCStr(extension);
		decodermapIterator it = decodermap.find(ext);
		return (it != decodermap.end());
	}

	IAudioDecoderFactory* cAudioManager::getAudioDecoderFactory(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString ext = safeCStr(extension);
		decodermapIterator it = decodermap.find(ext);
		if(it != decodermap.end())
		{
			return it->second;
		}
		return NULL;
	}

	void cAudioManager::unRegisterAllAudioDecoders()
	{
		cAudioMutexBasicLock lock(Mutex);
		decodermap.clear();
	}

	bool compareDataSourcePriorities(std::pair<int, cAudioString> left, std::pair<int, cAudioString> right)
	{
		return (left.first > right.first);
	}

	bool cAudioManager::registerDataSource(IDataSourceFactory* factory, const char* name, int priority)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString safeName = safeCStr(name);
        datasourcemap[safeName] = factory;
		dataSourcePriorityList.push_back(std::pair<int, cAudioString>(priority, safeName));
		std::sort(dataSourcePriorityList.begin(), dataSourcePriorityList.end(), compareDataSourcePriorities);

		getLogger()->logInfo("AudioManager", "Data Source named %s registered (Priority %i).", safeName.c_str(), priority);
		return true;
	}

	void cAudioManager::unRegisterDataSource(const char* name)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString safeName = safeCStr(name);
		datasourcemapIterator it = datasourcemap.find(safeName);
		if(it != datasourcemap.end())
		{
			datasourcemap.erase(it);
			getLogger()->logInfo("AudioManager", "Data Source named %s unregistered.", safeName.c_str());
		}

		for(size_t i=0; i<dataSourcePriorityList.size(); ++i)
		{
			if(dataSourcePriorityList[i].second == safeName)
			{
				dataSourcePriorityList.erase(dataSourcePriorityList.begin()+i);
				break;
			}
		}

		std::sort(dataSourcePriorityList.begin(), dataSourcePriorityList.end(), compareDataSourcePriorities);
	}

	bool cAudioManager::isDataSourceRegistered(const char* name)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString safeName = safeCStr(name);
		datasourcemapIterator it = datasourcemap.find(safeName);
		return (it != datasourcemap.end());
	}

	IDataSourceFactory* cAudioManager::getDataSourceFactory(const char* name)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioString safeName = safeCStr(name);
		datasourcemapIterator it = datasourcemap.find(safeName);
		if(it != datasourcemap.end())
		{
			return it->second;
		}
		return NULL;
	}

	void cAudioManager::unRegisterAllDataSources()
	{
		cAudioMutexBasicLock lock(Mutex);
		datasourcemap.clear();
		dataSourcePriorityList.clear();
	}

	void cAudioManager::registerEventHandler(IManagerEventHandler* handler)
	{
		if(handler)
		{
			eventHandlerList.push_back(handler);
		}
	}

	void cAudioManager::unRegisterEventHandler(IManagerEventHandler* handler)
	{
		if(handler)
		{
			eventHandlerList.remove(handler);
		}
	}

	void cAudioManager::unRegisterAllEventHandlers()
	{
		eventHandlerList.clear();
	}

	void cAudioManager::signalEvent(Events sevent)
	{
		cAudioMutexBasicLock lock(Mutex);
		cAudioList<IManagerEventHandler*>::Type::iterator it = eventHandlerList.begin();

		if(it != eventHandlerList.end())
		{
			switch(sevent)
			{
				case ON_INIT: 
					
					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onInit();
					}

					break;
				
				case ON_UPDATE:

					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onUpdate();
					}

					break;

				case ON_RELEASE:

					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onRelease();
					}

					break;

				case ON_SOURCECREATE:

					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onSourceCreate();
					}

					break;

				case ON_DECODERREGISTER:

					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onDecoderRegister();
					}

					break;

				case ON_DATASOURCEREGISTER:

					for(it; it != eventHandlerList.end(); it++)
					{
						(*it)->onDataSourceRegister();
					}

					break;
			}
		}
	}

    IAudioSource* cAudioManager::getSoundByName(const char* name)
    {
		cAudioMutexBasicLock lock(Mutex);
		cAudioString audioName = safeCStr(name);
        audioIndexIterator i = audioIndex.find(audioName);
        if (i == audioIndex.end())
		{
			return NULL;
		}
        return i->second;
    }

    void cAudioManager::releaseAllSources()
    {
		cAudioMutexBasicLock lock(Mutex);
		cAudioVector<IAudioSource*>::Type::iterator audioSourcesIter;

        cAudioVector<IAudioSource*>::Type deleteSources = audioSources;
        for(audioSourcesIter = deleteSources.begin(); audioSourcesIter != deleteSources.end(); ++audioSourcesIter) 
        {
            if((*audioSourcesIter))
                (*audioSourcesIter)->drop();
        }
		audioSources.clear();
		audioIndex.clear();

		cAudioVector<IAudioSource*>::Type::iterator managedAudioIter;
		for(managedAudioIter = managedAudioSources.begin(); managedAudioIter != managedAudioSources.end(); managedAudioIter++)
		{
			if ((*managedAudioIter))
			{
				CAUDIO_DELETE (*managedAudioIter);
			}
		}
		managedAudioSources.clear();

		cAudioVector<IAudioSource*>::Type::iterator managedAudioSourcesDelIter;
		for(managedAudioSourcesDelIter = managedAudioSourcesDelBuffer.begin(); managedAudioSourcesDelIter != managedAudioSourcesDelBuffer.end(); managedAudioSourcesDelIter++)
		{
			if ((*managedAudioSourcesDelIter))
			{
				CAUDIO_DELETE (*managedAudioSourcesDelIter);
			}
		}
		managedAudioSourcesDelBuffer.clear();
    }

	void cAudioManager::release(IAudioSource* source)
	{
		if(source)
		{		
			cAudioMutexBasicLock lock(Mutex);
			audioIndexIterator it = audioIndex.begin();
			for ( it=audioIndex.begin(); it != audioIndex.end(); it++ )
			{
				if( it->second == source )
				{
					audioIndex.erase(it);
					break;
				}
			}
			for(unsigned int i=0; i<audioSources.size(); ++i)
			{
				if(source == audioSources[i])
				{					
					audioSources.erase(audioSources.begin()+i);		

					if (source->getReferenceCount() <= 1)
						CAUDIO_DELETE source;
					else
						source->drop();

					break;
				}
			}
		}
	}
};
