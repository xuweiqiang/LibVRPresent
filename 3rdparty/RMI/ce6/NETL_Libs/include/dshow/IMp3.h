// MP3 Decoder Interface 
//
// IMp3.h

#ifndef __IMP3_H__
#define __IMP3_H__

#ifndef _AHEADERINFO_
#define _AHEADERINFO_
// for reporting the header info from the audio decoder
typedef struct aheaderinfo_s
{
  int  iSamplesPerSec;    /* samples per second per channel */
  int  iBitsPerSample;    /* the audio renderer expands or truncates to 16 bits */
  int  iChannels_decoded; /* channels present in the audio stream */   
  int  iChannels_downmix; /* output channels in downmix from decoder */   
  int  iBitrate;          /* bps, bits per second */
  int  iFramesize;        
  int  iDuration;         /* milliseconds */
  int  iLayer;    
  char pcDecoderFullName[256]; /* e.g: Ogg Vorbis, MPEG 2 layer 3, AC3 MP4, etc. */
} aheaderinfo_t;
#endif

#ifdef __cplusplus
  // {BBA40CEB-4D40-4a88-A083-7B6DC136514F}
  DEFINE_GUID(IID_IMp3, 
  0xbba40ceb, 0x4d40, 0x4a88, 0xa0, 0x83, 0x7b, 0x6d, 0xc1, 0x36, 0x51, 0x4f);

  interface IMp3 : public IUnknown
  {
    public:
      STDMETHOD(GetAheaderinfo)(aheaderinfo_t *pAheader, unsigned int *nSize) = 0;              
      STDMETHOD(GetAvgbitrate)(int *avgbitrate) = 0;
  };
#endif

#endif //__IMP3_H__

