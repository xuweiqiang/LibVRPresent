// LibVideoMixer.h : main header file for the LibVideoMixer DLL
//

#pragma once
#include "../../inc/McCom.h"
#include "VideomixerInterface.h"
#include "../../inc/GfxPresent.h"
#include "../../inc/GfxContext.h"
#include "../../3rdparty/imglib/ci_codec_type.h"
#include "../../3rdparty/imglib/ci_imageproc.h"
#include "../LibGfxPresent/AlphaBlending.h"
#include "../../inc/GfxPresentPrepare.h"
typedef struct
{
	DWORD	input_fourCC;	
	DWORD	dwWidth;
	DWORD	dwHeight;
	VM_SURFACE_LAYER_ID surface_id;
	DWORD	dwTranColor;
	void* pBuf;
	BOOL bEnabled;
	RECT rDst;
	FLOAT fAlpha;
}VIDEOMIXER_HANDLE;

class CLibVideoMixerApp : 
	public IMcVideoPresenterMixer,
	public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CLibVideoMixerApp();
	virtual ~CLibVideoMixerApp();
	STDMETHODIMP Open(struct VM_OpenOptions* pOptions, DWORD dwOptionSize);
	STDMETHODIMP Close();
	STDMETHODIMP Reset();
	STDMETHODIMP Get(VM_SURFACE_LAYER_ID id, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(VM_SURFACE_LAYER_ID id, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP CreateSurface(VM_SURFACE_LAYER_ID id, struct VM_SurfaceOpenOptions* pOptions);
	STDMETHODIMP DestroySurface(VM_SURFACE_LAYER_ID id);
	STDMETHODIMP Prepare(VM_SURFACE_LAYER_ID id, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP PresentBuffer(VM_SURFACE_LAYER_ID id, DWORD dwFlags);
	STDMETHODIMP SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk);

// Overrides
private:
	STDMETHODIMP ColorConvertion_Resize(void* pInput,void* pOutput,DWORD input_width, DWORD input_height, DWORD input_format,
		DWORD output_width, DWORD output_height, DWORD output_format, RECT* pDst, DWORD dwColor);
	STDMETHODIMP TransparentOverwrite(void* pBackground,void* pSrc,DWORD dwSrcTransColor,RECT* pDst, FLOAT alpha);
	
	STDMETHODIMP GetFourCCLength(DWORD fourCC, DWORD *plength);
	void* m_pMainBuf;
	BOOL m_bOpen;

	struct VM_OpenOptions m_openOptions;
	int m_bytes_per_pixcel; 
	static const DWORD const_dwFourCC[4];
	CAlphaBlending* m_pAlphaBlend;
	void* m_pCCResizeBuf;
	VIDEOMIXER_HANDLE m_hSurface[MAX_SURFACE_NUM];
	CRITICAL_SECTION m_csPresent; 
	IMcVideoPresenter	*m_pVRPresent;

	DWORD m_mainSurf_FourCC;
	GfxPrepareDataV2 m_PrepareData;

};

