/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.7
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace cAudio {

using System;
using System.Runtime.InteropServices;

public class IAudioManager : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal IAudioManager(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(IAudioManager obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~IAudioManager() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          cAudioCSharpWrapperPINVOKE.delete_IAudioManager(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public virtual bool initialize(string deviceName, int outputFrequency, int eaxEffectSlots) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_initialize__SWIG_0(swigCPtr, deviceName, outputFrequency, eaxEffectSlots);
    return ret;
  }

  public virtual bool initialize(string deviceName, int outputFrequency) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_initialize__SWIG_1(swigCPtr, deviceName, outputFrequency);
    return ret;
  }

  public virtual bool initialize(string deviceName) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_initialize__SWIG_2(swigCPtr, deviceName);
    return ret;
  }

  public virtual bool initialize() {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_initialize__SWIG_3(swigCPtr);
    return ret;
  }

  public virtual void shutDown() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_shutDown(swigCPtr);
  }

  public virtual void update() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_update(swigCPtr);
  }

  public virtual bool isUpdateThreadRunning() {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_isUpdateThreadRunning(swigCPtr);
    return ret;
  }

  public virtual IAudioSource getSoundByName(string name) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_getSoundByName(swigCPtr, name);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual void releaseAllSources() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_releaseAllSources(swigCPtr);
  }

  public virtual void release(IAudioSource source) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_release(swigCPtr, IAudioSource.getCPtr(source));
  }

  public virtual IAudioSource play2D(string filename, bool playLooped, bool startPaused) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play2D__SWIG_0(swigCPtr, filename, playLooped, startPaused);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource play2D(string filename, bool playLooped) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play2D__SWIG_1(swigCPtr, filename, playLooped);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource play2D(string filename) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play2D__SWIG_2(swigCPtr, filename);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource play3D(string filename, cVector3 position, bool playLooped, bool startPaused) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play3D__SWIG_0(swigCPtr, filename, cVector3.getCPtr(position), playLooped, startPaused);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    if (cAudioCSharpWrapperPINVOKE.SWIGPendingException.Pending) throw cAudioCSharpWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual IAudioSource play3D(string filename, cVector3 position, bool playLooped) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play3D__SWIG_1(swigCPtr, filename, cVector3.getCPtr(position), playLooped);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    if (cAudioCSharpWrapperPINVOKE.SWIGPendingException.Pending) throw cAudioCSharpWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual IAudioSource play3D(string filename, cVector3 position) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_play3D__SWIG_2(swigCPtr, filename, cVector3.getCPtr(position));
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    if (cAudioCSharpWrapperPINVOKE.SWIGPendingException.Pending) throw cAudioCSharpWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual void setMasterVolume(float vol) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_setMasterVolume(swigCPtr, vol);
  }

  public virtual float getMasterVolume() {
    float ret = cAudioCSharpWrapperPINVOKE.IAudioManager_getMasterVolume(swigCPtr);
    return ret;
  }

  public virtual void stopAllSounds() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_stopAllSounds(swigCPtr);
  }

  public virtual IAudioSource create(string name, string filename, bool stream) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_create__SWIG_0(swigCPtr, name, filename, stream);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource create(string name, string filename) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_create__SWIG_1(swigCPtr, name, filename);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource createFromMemory(string name, string data, uint length, string extension) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_createFromMemory(swigCPtr, name, data, length, extension);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource createFromRaw(string name, string data, uint length, uint frequency, AudioFormats format) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_createFromRaw(swigCPtr, name, data, length, frequency, (int)format);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual IAudioSource createFromAudioBuffer(string name, AudioCaptureBuffer pBiffer, uint frequency, AudioFormats format) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_createFromAudioBuffer(swigCPtr, name, AudioCaptureBuffer.getCPtr(pBiffer), frequency, (int)format);
    IAudioSource ret = (cPtr == IntPtr.Zero) ? null : new IAudioSource(cPtr, false);
    return ret;
  }

  public virtual bool registerAudioDecoder(IAudioDecoderFactory factory, string extension) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_registerAudioDecoder(swigCPtr, IAudioDecoderFactory.getCPtr(factory), extension);
    return ret;
  }

  public virtual void unRegisterAudioDecoder(string extension) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterAudioDecoder(swigCPtr, extension);
  }

  public virtual bool isAudioDecoderRegistered(string extension) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_isAudioDecoderRegistered(swigCPtr, extension);
    return ret;
  }

  public virtual IAudioDecoderFactory getAudioDecoderFactory(string extension) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_getAudioDecoderFactory(swigCPtr, extension);
    IAudioDecoderFactory ret = (cPtr == IntPtr.Zero) ? null : new IAudioDecoderFactory(cPtr, false);
    return ret;
  }

  public virtual void unRegisterAllAudioDecoders() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterAllAudioDecoders(swigCPtr);
  }

  public virtual bool registerDataSource(IDataSourceFactory factory, string name, int priority) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_registerDataSource(swigCPtr, IDataSourceFactory.getCPtr(factory), name, priority);
    return ret;
  }

  public virtual void unRegisterDataSource(string name) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterDataSource(swigCPtr, name);
  }

  public virtual bool isDataSourceRegistered(string name) {
    bool ret = cAudioCSharpWrapperPINVOKE.IAudioManager_isDataSourceRegistered(swigCPtr, name);
    return ret;
  }

  public virtual IDataSourceFactory getDataSourceFactory(string name) {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_getDataSourceFactory(swigCPtr, name);
    IDataSourceFactory ret = (cPtr == IntPtr.Zero) ? null : new IDataSourceFactory(cPtr, false);
    return ret;
  }

  public virtual void unRegisterAllDataSources() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterAllDataSources(swigCPtr);
  }

  public virtual void registerEventHandler(IManagerEventHandler handler) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_registerEventHandler(swigCPtr, IManagerEventHandler.getCPtr(handler));
  }

  public virtual void unRegisterEventHandler(IManagerEventHandler handler) {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterEventHandler(swigCPtr, IManagerEventHandler.getCPtr(handler));
  }

  public virtual void unRegisterAllEventHandlers() {
    cAudioCSharpWrapperPINVOKE.IAudioManager_unRegisterAllEventHandlers(swigCPtr);
  }

  public virtual IListener getListener() {
    IntPtr cPtr = cAudioCSharpWrapperPINVOKE.IAudioManager_getListener(swigCPtr);
    IListener ret = (cPtr == IntPtr.Zero) ? null : new IListener(cPtr, false);
    return ret;
  }

}

}
