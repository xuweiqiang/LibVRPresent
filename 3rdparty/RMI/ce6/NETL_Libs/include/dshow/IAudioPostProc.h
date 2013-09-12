// Audio Post Processor Interface
//
// IAudioPostProc.h

#ifndef __IAUDIOPOSTPROC_H__
#define __IAUDIOPOSTPROC_H__

#ifdef __cplusplus

  // {DE2B48DD-E80A-4370-AB5A-16B4E8CC2F12}
  DEFINE_GUID(IID_IAudioCore,
  0xde2b48dd, 0xe80a, 0x4370, 0xab, 0x5a, 0x16, 0xb4, 0xe8, 0xcc, 0x2f, 0x12);

  #define AUDIO_CORE_OFF    0x00
  #define AUDIO_CORE_ON     0x01

  typedef struct audio_test_struct_
  {
    LONGLONG      bytes_length;
    LONGLONG      ms_duration;
    unsigned int  crc;
    unsigned int  sampling_rate;
    unsigned int  channels;
  } audio_test_struct;

  interface IAudioCore : public IUnknown
  {
    public:
      //EQ Controller Settings (num_bands is 3 or 5) (db +-12), set one band at a time
      STDMETHOD(SetEqLevel)(int num_bands, int band, int db) = 0;
      STDMETHOD(GetEqLevel)(int *num_bands, int band, int *db) = 0;

      STDMETHOD(SetEqCenterFrequency)(int band, int freq) = 0;
      STDMETHOD(GetEqCenterFrequency)(int band, int *freq) = 0;

      STDMETHOD(SetEqBandwidthPercent)(int band, int freq) = 0;
      STDMETHOD(GetEqBandwidthPercent)(int band, int *freq) = 0;

      STDMETHOD(GetSpectrumAnalyzerResults)(int freq_band, int *level) = 0;

      //AudioCoreEffects Settings bOn(on/off)
      //NOTE: needs to be on for settings below to take effect.
      STDMETHOD(SetAudioCoreEffects)(unsigned int bOn) = 0;
      STDMETHOD(GetAudioCoreEffects)(unsigned int *bOn) = 0;

      //Sound Level Controller Settings bOn(on/off) nDepth(1-3)
      STDMETHOD(SetSoundLevelController)(unsigned int bOn, unsigned int nDepth) = 0;
      STDMETHOD(GetSoundLevelController)(unsigned int *bOn, unsigned int *nDepth) = 0;

      //Volume Settings bOn(on/off)
      STDMETHOD(SetVolume)(unsigned int bOn) = 0;
      STDMETHOD(GetVolume)(unsigned int *bOn) = 0;

      //Volume Gain Settings nInternal(0-255) nExternal(0-255)
      STDMETHOD(SetVolumeGain)(unsigned int nInternal, unsigned int nExternal) = 0;
      STDMETHOD(GetVolumeGain)(unsigned int *nInternal, unsigned int *nExternal) = 0;

      //Volume Balance Settings nBalace(-127 to +127)
      STDMETHOD(SetVolumeBalance)(int nBalance) = 0;
      STDMETHOD(GetVolumeBalance)(int *nBalance) = 0;

      //Volume Mute Settings bMute(on/off)
      STDMETHOD(SetVolumeMute)(unsigned int bMute) = 0;
      STDMETHOD(GetVolumeMute)(unsigned int *bMute) = 0;

      //Audio Encode (on/off)
      STDMETHOD(SetAudioEncode)(unsigned int bEnable) = 0;
      STDMETHOD(GetAudioEncode)(unsigned int *bEnable) = 0;

      //Audio Encode (file name)
      STDMETHOD(SetAudioEncodeName)(TCHAR *name) = 0;
      STDMETHOD(GetAudioEncodeName)(TCHAR *name) = 0;

      //Applies the Effect Settings
      // This function is required after changes are made to any of the above effects to activate them.
      STDMETHOD(ApplyEffects)(void) = 0;

      //Audio Test
      STDMETHOD(StartAudioTest)(unsigned int Start) = 0;
      STDMETHOD(StopAudioTest)(audio_test_struct *ResultInfo) = 0;
};
#endif

#endif //__IAUDIOPOSTPROC_H__

