// Video Renderer Interface 
//
// IVrend.h

#ifndef __IVREND_H__
#define __IVREND_H__


#ifdef __cplusplus
  // {A08EA375-C7DA-4ef7-A5C7-0A31FD923683}
  DEFINE_GUID(IID_IVrend, 
  0xa08ea375, 0xc7da, 0x4ef7, 0xa5, 0xc7, 0xa, 0x31, 0xfd, 0x92, 0x36, 0x83);

  interface IVrend : public IUnknown
  {
    public:
      STDMETHOD(SetRotationMode)(ULONG nMode) = 0;
      STDMETHOD(GetRotationMode)(ULONG *nMode) = 0;

      STDMETHOD(SetDisplayStatsMode)(ULONG nMode) = 0;
      STDMETHOD(GetDisplayStatsMode)(ULONG *nMode) = 0;

      STDMETHOD(SetDeinterlaceMode)(ULONG nMode) = 0;
      STDMETHOD(GetDeinterlaceMode)(ULONG *nMode) = 0;

      STDMETHOD(SetGamma)(ULONG nVal) = 0;
      STDMETHOD(GetGamma)(ULONG *nVal) = 0;

      STDMETHOD(SetAvSyncMode)(ULONG nVal) = 0;
      STDMETHOD(GetAvSyncMode)(ULONG *nVal) = 0;

      STDMETHOD(GetDisplayGeometry)(RECT *pRect) = 0;
      STDMETHOD(GetDisplayBpp)(ULONG *nBpp) = 0;

      STDMETHOD(SetDeblockMode)(ULONG nMode) = 0;
      STDMETHOD(GetDeblockMode)(ULONG *nMode) = 0;
  };
#endif

#endif //__IVREND_H__

