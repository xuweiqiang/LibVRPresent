// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIVX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIVX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __DIVXINTERFACE_H
#define __DIVXINTERFACE_H

#ifdef DIVX_EXPORTS
#define DIVX_API __declspec(dllexport)
#else
#define DIVX_API __declspec(dllimport)
#endif

#define DIVX_CODE_LENGTH	11
#define UUID_LENGTH			8

typedef struct _DivX_Data {
	unsigned char	*drmContext;
	unsigned int	drmContextLength;
	BYTE			*pSTRD;

	// Protections results
	BYTE	ict;
	BYTE	cgmsaSignal;
	BYTE	acptbSignal;
	BYTE	digitalProtectionSignal;
} DivX_Data_t;


typedef HRESULT (* fnInitPlayBack_t) (BYTE *pSTRD);
typedef HRESULT (* fnDecryptVideo_t) (BYTE *frame, DWORD frameSize, BYTE *drmFrameInfo);
typedef HRESULT (* fnDecryptAudio_t) (BYTE *frame, DWORD frameSize);

typedef HRESULT (* fnPreparePlayback_t)(BYTE *pRentalMessageFlag, BYTE *pUseLimit, BYTE *pUseCount);
typedef HRESULT (* fnCommitPlayback_t) (void);
typedef HRESULT (* fnFinalizePlayback_t) (void);
typedef HRESULT (* fnGetRegistrationCode_t) (wchar_t *szRegistrationCode);
typedef HRESULT (* fnGetDeregistrationCode_t) (wchar_t *szDeregistrationCode);
typedef HRESULT (* fnSetRandomSample_t) (void);
typedef HRESULT (* fnInitDrmMemory_t) (void);
typedef BOOL (* fnIsDeviceActivated_t) (void);

typedef int (* fnEncryptMessage_t) (int nIndex, BOOL bFinal, PBYTE pbMessage, DWORD *pdwMessageLength, DWORD dwBufLen);
typedef int (* fnDecryptMessage_t) (int nIndex, BOOL bFinal, PBYTE pbMessage, DWORD *pdwMessageLength);
typedef int (* fnGetRandomBytes_t) (PBYTE pbBuffer, const DWORD dwBufferSize);

extern "C"
{
	// These are for the demux.
	DIVX_API HRESULT InitDivXPlayback(BYTE* pSTRD);
	DIVX_API HRESULT DecryptDivXVideo(BYTE *frame, DWORD frameSize, BYTE *drmFrameInfo);
	DIVX_API HRESULT DecryptDivXAudio(BYTE *frame, DWORD frameSize);

	// These are for the player.
	DIVX_API int InitDivX(void);
	DIVX_API int DeinitDivX(void);
	DIVX_API HRESULT PrepareDivXPlayback(BYTE *pRentalMessageFlag, BYTE *pUseLimit, BYTE *pUseCount);
	DIVX_API HRESULT CommitDivXPlayback();
	DIVX_API HRESULT FinalizeDivXPlayback();
	DIVX_API HRESULT GetDivXRegistrationCode(wchar_t *szRegistrationCode);
	DIVX_API HRESULT GetDivXDeregistrationCode(wchar_t *szDeregistrationCode);
	DIVX_API HRESULT SetRandomSample();
	DIVX_API BOOL IsDeviceActivated();
	// This should only be run once ever per device.
	DIVX_API HRESULT InitDrmMemory();

	DIVX_API int EncryptMessage(int nIndex, BOOL bFinal, PBYTE pbMessage, DWORD *pdwMessageLength, DWORD dwBufLen);
	DIVX_API int DecryptMessage(int nIndex, BOOL bFinal, PBYTE pbMessage, DWORD *pdwMessageLength);
	DIVX_API int GetRandomBytes(PBYTE pbBuffer, const DWORD dwBufferSize);
}




//	All the COM-defined FACILITY_ITF codes have a code value in the range of 0x0000-0x01FF. 
//	While it is legal to use any codes in FACILITY_ITF, it is recommended that only code 
//	values in the range of 0x0200-0xFFFF be used. This recommendation is made as a means of 
//	reducing confusion with any COM-defined errors.
#define AU_ERROR_CODE(x) MAKE_HRESULT(1, FACILITY_ITF, (x) + 0x200)
#define AU_SUCCESS_CODE(x) MAKE_HRESULT(0, FACILITY_ITF, (x) + 0x200)


// DivX Interface HRESULTS
#define DIVX_DRM_E_NOT_INITIALIZED		AU_ERROR_CODE(1)	//	DivX DRM not initialized.
#define DIVX_DRM_E_NOT_AUTHORIZED		AU_ERROR_CODE(2)	//	Device not Authorized to play this file.
#define DIVX_DRM_E_NOT_REGISTERED		AU_ERROR_CODE(3)	//	This device isn't registered with DivX.
#define DIVX_DRM_E_RENTAL_EXPIRED		AU_ERROR_CODE(4)	//	The media file has been viewed the max number of times.
#define DIVX_DRM_E_DECRYPTION_FAILED	AU_ERROR_CODE(5)	//	
#define DIVX_DRM_E_GET_REG_CODE_FAILED	AU_ERROR_CODE(6)	//	

#endif __DIVXINTERFACE_H