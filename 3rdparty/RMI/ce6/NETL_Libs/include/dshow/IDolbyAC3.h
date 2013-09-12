// Dolby AC3 Interface 
//
// IDolbyAC3.h

#ifndef __IDOLBYAC3_H__
#define __IDOLBYAC3_H__

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
  // {ABA40CEA-4C40-4a88-A083-7B6DC136514D}
  DEFINE_GUID(IID_IDolbyAC3, 
  0xaba40cea, 0x4c40, 0x4a88, 0xa0, 0x83, 0x7b, 0x6d, 0xc1, 0x36, 0x51, 0x4d);

  interface IDolbyAC3 : public IUnknown
  {
    public:
      STDMETHOD(SetKaraokeMode)(unsigned int nMode) = 0;
      STDMETHOD(GetKaraokeMode)(unsigned int *nMode) = 0;

      STDMETHOD(SetCompressionMode)(unsigned int nMode) = 0;  
      STDMETHOD(GetCompressionMode)(unsigned int *nMode) = 0; 

      STDMETHOD(SetLFEPresent)(unsigned int nLFE) = 0;   
      STDMETHOD(GetLFEPresent)(unsigned int *nLFE) = 0;   
      
      STDMETHOD(SetOutputChannelConfig)(unsigned int nConfig) = 0;      
      STDMETHOD(GetOutputChannelConfig)(unsigned int *nConfig) = 0;     

      STDMETHOD(SetChannels)(unsigned int nChannels) = 0;       
      STDMETHOD(GetChannels)(unsigned int *nChannels) = 0;       

      STDMETHOD(SetScaleFactor)(unsigned int nScaleFactor) = 0;         
      STDMETHOD(GetScaleFactor)(unsigned int *nScaleFactor) = 0;         

      STDMETHOD(SetStereoMode)(unsigned int nScaleFactor) = 0;         
      STDMETHOD(GetStereoMode)(unsigned int *nScaleFactor) = 0;         

      STDMETHOD(SetDualMonoMode)(unsigned int nScaleFactor) = 0;         
      STDMETHOD(GetDualMonoMode)(unsigned int *nScaleFactor) = 0;         

      STDMETHOD(SetDynamicRangeLo)(unsigned int nDynamicRangeLo) = 0;           
      STDMETHOD(GetDynamicRangeLo)(unsigned int *nDynamicRangeLo) = 0;           

      STDMETHOD(SetDynamicRangeHi)(unsigned int nDynamicRangeHi) = 0;             
      STDMETHOD(GetDynamicRangeHi)(unsigned int *nDynamicRangeHi) = 0;             
      
      STDMETHOD(SetChannelOrder)(unsigned int *nChannelOrder, int size)           = 0;               
      STDMETHOD(GetChannelOrder)(unsigned int *nChannelOrder, unsigned int *nSize) = 0;              

      STDMETHOD(GetAheaderinfo)(aheaderinfo_t *pAheader, unsigned int *nSize) = 0;              

  };
#endif

#define FLAG_AC3_INT_ONE               0x20000000 /* Do not change this! It must agree with INT_ONE in ac3d_com.h */

#endif //__IDOLBYAC3_H__

