#ifndef _SUB_DECODER_H_
#define _SUB_DECODER_H_

#ifndef EXPORT
#define EXPORT extern "C" _declspec(dllexport)
#endif

typedef enum
{
	EMC_PROPID_UNKNOWN = 0x0000,
}EMC_PROPID_SUB;

typedef enum
{
	CI_SUB_COLOR_SPACE_YCBCRT_SDTV  = 1,
	CI_SUB_COLOR_SPACE_YCBCRT_HDTV,
	CI_SUB_COLOR_SPACE_RGBA8888,
}CI_SUB_COLOR_SPACE;

#ifndef EMC_PROPID_OFFSET
#define EMC_PROPID_OFFSET(groupid)	(groupid<<8)

enum EMC_PROPID_SUBTITLE
{
	EMC_PROPID_SUBTITLE_TYPE = EMC_PROPID_OFFSET(0),
	EMC_PROPID_SUBTITLE_SETDRAWCB,
	EMC_PROPID_SUBTITLE_DISP_ON,
	EMC_PROPID_SUBTITLE_DISP_OFF,
	EMC_PROPID_SUBTITLE_NOSYNC,
	EMC_PROPID_SUBTITLE_BACKGROUND_COLOR,
	EMC_PROPID_SUBTITLE_SWITCH_LANGUAGE,
};
#endif

typedef struct CI_SUB_Rect_ 
{
	long	lLeft;
	long	lTop;
	long	lRight;
	long	lBottom;
}CI_SUB_Rect;

typedef struct CI_SUB_Open_Option_ 
{
	CI_SUB_Rect		SrcRect;
	CI_SUB_COLOR_SPACE ColorType;
	unsigned short	usCompositionPageID;
	unsigned short	usAncillaryPageID;
}CI_SUB_Open_Option;

typedef struct 
{
	unsigned char* pRegionBuffer;
	CI_SUB_Rect    DispRect;
	bool           bShow;
	unsigned long  PTS;
	unsigned long  TotalRegions;
	unsigned long  RegionIndex;
}CI_SUB_Draw_Info;

typedef void (*PFN_DisplayDrawCB)(CI_SUB_Draw_Info *pDrawInfo, void *pContext);

DECLARE_INTERFACE_(ISUBDecoder, IUnknown)
{
    STDMETHOD(Open)(DWORD dwOption) PURE;
    STDMETHOD(Close)() PURE;
    STDMETHOD(Reset)() PURE;
    STDMETHOD(Get)(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned) PURE;
    STDMETHOD(Set)(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData) PURE;

	STDMETHOD(SendPES)(unsigned char *pBuffer, DWORD dwLength) PURE;
	STDMETHOD(SendES)(unsigned char *pBuffer, DWORD& dwLength, DWORD PTS) PURE;
	STDMETHOD(SendPCR)(UINT64 u64TimeStamp) PURE;
};

EXPORT ISUBDecoder* CreateSUBDecoder();
#endif