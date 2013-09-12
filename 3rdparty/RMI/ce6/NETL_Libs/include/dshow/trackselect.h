// Alchemy Filter GUIDS and MediaTypes
// trackselect.h
// Copyright (c) 2007 - 2008  RMI Corp.
/////////////////////////////////////////////////////////
#ifndef __TRACK_SELECT_H__
#define __TRACK_SELECT_H__

// interface to retrieve audio and sub-picture track selection
// {6B8FEFF3-098E-4DB4-A254-3E8D9B71AEC6}
DEFINE_GUID(IID_ITrackSelect,
0x6b8feff3, 0x098e, 0x4db4, 0xa2, 0x54, 0x3e, 0x8d, 0x9b, 0x71, 0xae, 0xc6);

interface ITrackSelect : public IUnknown
{
  /*!
    Sets the active audio track and mutes all others.

    @param trackId (INP) track ID of audio track to enable  
    @return S_OK if successful or error code
  */
  STDMETHOD(SetAudioTrack)(int trackId) = 0;

  /*!
    Gets the active audio track,

    @param trackId (OUT) track ID of current audio track  
    @return S_OK if successful or error code
  */
  STDMETHOD(GetAudioTrack)(int *trackId) = 0;

  /*!
    Gets the number of audio tracks of current title.

    @param tracks (OUT) number of tracks. 
    @return S_OK if successful or error code
  */
  STDMETHOD (GetNumAudioTracks) (int *tracks) =0;

  /*!
    Gets the track name.

    @param track (INP) number of track (1 based). 
    @return S_OK if successful or error code
  */  
  STDMETHOD (GetAudioTrackName) (int trackNum, wchar_t *name, int bufferSize) =0;

  /*!
    Sets the default audio lang.

    @param trackId (INP) language code "en","fr",ect. 
    @return S_OK if successful or error code
  */
  STDMETHOD (SetDefaultAudioLanguage) (TCHAR *lang) = 0;

  /*!
    Gets the default audio lang.

    @param trackId (OUT) language code "en","fr",ect. 
    @return S_OK if successful or error code
  */
  STDMETHOD (GetDefaultAudioLanguage) (TCHAR *lang) = 0;

  /*!
    Mute all audio tracks   

    @return S_OK if successful or error code
  */
  STDMETHOD (SetAudioOff) () = 0;

  /*!
    Unmute audio tracks.  

    @return S_OK if successful or error code
  */
  STDMETHOD (SetAudioOn) () = 0;

  /*!
    Sets the active subtitle track and disables all others.

    @param trackId (INP) track ID of subtitle track to enable 
    @return S_OK if successful or error code
  */  
  STDMETHOD (SetSubtitleTrack) (int trackId) = 0;

  /*!
    Gets the active subtitle track.

    @param track (OUT) track number of active subtitle track.
    @return S_OK if successful or error code
  */  
  STDMETHOD (GetSubtitleTrack) (int *track) = 0;

  /*!
    Gets the number of subtitle tracks of current title.

    @param tracks (OUT) number of tracks. 
    @return S_OK if successful or error code
  */  
  STDMETHOD (GetNumSubtitleTracks) (int *tracks) = 0;

  /*!
    See if  subtitles are currently supplied to output pin or not.

    @param tracks (OUT) TRUE if subtitles are on, False otherwise.  
    @return S_OK if successful or error code
  */
  STDMETHOD (GetSubtitlesStatus) (bool *bSubtitleOn)=0;

  /*!
    Gets the track name.

    @param track (INP) number of track (1 based). 
    @return S_OK if successful or error code
  */  
  STDMETHOD (GetSubtitleTrackName) (int trackNum, wchar_t *name, int bufferSize) = 0;

  /*!
    Sets the default subtitle lang.

    @param trackId (INP) language code "en","fr",ect. 
    @return S_OK if successful or error code
  */
  STDMETHOD (SetDefaultSubtitleLanguage) (TCHAR *lang) = 0;

  /*!
    Gets the default subtitle lang.

    @param trackId (OUT) language code "en","fr",ect. 
    @return S_OK if successful or error code
  */
  STDMETHOD (GetDefaultSubtitleLanguage) (TCHAR *lang) = 0;

  /*!
    Enables all subtitle tracks   

    @return S_OK if successful or error code
  */
  STDMETHOD (SetSubtitlesOn) () = 0;

  /*!
    Disable all subtitle tracks   

    @return S_OK if successful or error code
  */
  STDMETHOD (SetSubtitlesOff) () = 0;

  // will provide the total number of programs found within the Transport Stream
  // note: some files contain programs without any audio or video
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  STDMETHOD(GetNumPrograms)(UINT *pNumPrograms) = 0;

  // select a program index for playback from 0 to number of programs-1
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  // returns VFW_E_NOT_FOUND if the no audio/video was found for selected program stream
  STDMETHOD(SetProgram)(UINT iProgramIndex) = 0;

  // will provide the program index of stream currently selected
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  STDMETHOD(GetProgram)(UINT *pProgramIndex) = 0;

  // provides the program number associated with the index passed in 
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  STDMETHOD(GetProgramNumber)(UINT iProgramIndex, UINT* pProgramNum, UINT* pProgramPid) = 0;

  // provides the Audio information assocaited with the index passed in
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  // returns VFW_E_NOT_FOUND if the no audio was found for selected program stream
  STDMETHOD(GetAudioInfo)(UINT iProgramIndex, UINT* pAudioPid, UINT* pFormatTag, UINT* pSampleRate, UINT* pChannels) = 0;

  // provides the Video information assocaited with the index passed in
  // returns S_OK on success
  // returns VFW_E_UNSUPPORTED_STREAM if stream is not a transport stream
  // returns VFW_E_NOT_FOUND if the no video was found for selected program stream
  STDMETHOD(GetVideoInfo)(UINT iProgramIndex, UINT* pVideoPid, UINT* pWidth, UINT* pHeight) = 0;
};
#endif