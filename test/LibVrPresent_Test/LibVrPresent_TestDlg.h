// LibVrPresent_TestDlg.h : header file
//
#pragma once
#include "Resource.h"
#include "DisplayDlg.h"
#include "DllSelect.h"
#include "SetPage.h"
#include "GfxPresent.h"
#include "afxwin.h"

#ifndef SHx
#include "LibCpuIdle.h"
#pragma comment(lib, "LibCpuIdle.lib")
#endif

#define TEST_V2_SURF

#if ((_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
#define SET_PAGE
#endif
#ifdef ST_DEMO
#define CREATE_WINDOW_BY_APP
#define USE_NEW_INTERFACE
#endif

#ifndef _WIN32_WCE
#define CREATE_WINDOW_BY_APP
#endif
//#define DLL_SEL
//#define OSD_TEST

typedef  IMcVideoPresenter*  (*fpCreateVideoPresenter)();

typedef struct PresentBufferInfo
{
	DWORD FrameNumber;
	BOOL Vaild;
	PresentBufferInfo* Next;
}PresentBufferInfo_T;



typedef enum VideoStatus
{
	ePlay,
	ePause,
	eStop,
}eStatus,*peStatus;


// CLibVrPresent_TestDlg dialog
class CLibVrPresent_TestDlg : public CDialog
{

// Construction
public:
    	CLibVrPresent_TestDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_LIBVRPRESENT_TEST_DIALOG };
public:
    BOOL IsValidDll(TCHAR *dllFile);
    void MoveDest(CPoint point);
    void ReSize(CSize size);
    void FullScrVideo();
    void CloseVideo();
    afx_msg void OnBnClickedButtonPause();
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMove(int x,int y);

    VP_ZOOM_MODE m_eVideoMode;
    afx_msg void OnBnClickedRadioLetterBox();
    afx_msg void OnBnClickedRadioCropping();
    afx_msg void OnBnClickedRadioStretch();
    void SetDisplayMode(VP_ZOOM_MODE mode);
    IMcVideoPresenter* m_pPresent;
#ifdef OSD_TEST
    IMcVideoPresenter* m_pPresent_OSD;
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    // Screen Width of origianl source file
    afx_msg void OnBnClickedButtonOpen();
    afx_msg void OnBnClickedCheckBbEnable();
    // Implementation
    HICON m_hIcon;
    // Generated message map functions
    virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
    afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
    DECLARE_MESSAGE_MAP()

private:
    HRESULT LoadLib(HMODULE& h_dll);
#ifndef TEST_V2_SURF
	int SetDestRect(IMcVideoPresenter* pPresent);
#else
	int SetDestRect(IMcVideoPresenterAdv* pPresent);
#endif
    int SetClipRect(IMcVideoPresenter* pPresent );

    static DWORD WINAPI BBPresentProc(LPVOID lpParameter);
    void InitPresentObject(DWORD Number);
    PresentBufferInfo_T* GetPresentObject();
    void PutPresentObject(PresentBufferInfo_T* Number);
    PresentBufferInfo_T* GetFreePresentObject();
    void PutFreePresentObject(PresentBufferInfo_T* Number);
    BOOL WaitPresentObjectEmpty(DWORD ms);
    void DeinitPresentObject(void);
    void SetPlayWnd();
    void EnableSettings(bool able);
    void OpenVideo();
    void OnTimer(UINT_PTR nIDEvent);
    void ClosePresenterAndFreeLib(IMcVideoPresenter*& pPresent, HMODULE& h_dll);
    HRESULT Init_CreatePresenter(HMODULE& h_dll);
    HRESULT Init_CreatePresenterProbe(HMODULE& h_dll, eRUNTYPE surface_mode );
    int SetDestColorKey(IMcVideoPresenter* pPresent);

    RECT Srcrect;
    RECT Cliprect;
    RECT Dstrect;
    DWORD m_vSourceWidth;
    DWORD m_vSourceHeight;
    DWORD m_vClipStartX;
    DWORD m_vClipStartY;
    DWORD m_vClipWidth;
    DWORD m_vClipHeight;
    DWORD m_vDestStartX;
    DWORD m_vDestStartY;
    DWORD m_vDestWidth;
    DWORD m_vDestHeight;
    DWORD m_vBackBufferNumber;
    INT   m_vBackBufferEnable;
	BOOL m_bCKey;
	DWORD m_dwCKey;
    PresentBufferInfo_T* m_PresentObjectBuffer;
    PresentBufferInfo_T* m_FreePresentObject;//Head of Free-Objectglist
    PresentBufferInfo_T* m_PresentObject;//Head of Present-Object-list
    HANDLE m_hBBPresentSema;  //–≈∫≈¡ø
    HANDLE m_hBBPresentThread;
    //HANDLE m_hPlayCtrlThread;
    BOOL m_fStopPresentThread;
    CRITICAL_SECTION m_csObjects; 
    DWORD m_dwBuffers;

    VP_VIDEOBUFFER_INFO vbi;
    eRUNTYPE etype;
    eStatus status;
#ifdef CREATE_WINDOW_BY_APP
   CDisplayDlg *ddlg;
#endif 
#ifdef DLL_SEL
   CDllSelect *dllSelect;
#endif
   CSetPage *setPage;
    CButton m_PausePlay;
    CButton m_Stop;
    CEdit m_SrcW;
    CEdit m_SrcH;
    CEdit m_DstX;
    CEdit m_DstY;
    CEdit m_DstW;
    CEdit m_DstH;
    FILE* m_file;
    DWORD m_FileLength;
    BOOL m_bReplay; // Loop Replay Mode
    BOOL m_isFullScr; //
    RECT m_BeforeFullScrRect;
    CTabCtrl m_Tab;

	LONG m_lRotationMode;
	HWND m_wnd;
	DWORD m_dwStrides[3];
    LPBYTE m_pYbuf;
    LPBYTE m_pUbuf;
    LPBYTE m_pVbuf;
	BOOL bOsd1Enabled;
	BOOL bOsd2Enabled;
	static RECT m_rect_last;
#ifdef _WIN32_WCE
#ifndef SHx
	CCPUIDLE m_idle;
#endif
#endif
public:
	afx_msg void OnBnClickedButtonRotate();
	afx_msg void OnBnClickedButtonOsd1();
	afx_msg void OnBnClickedButtonOsd2();

	IMcVideoPresenterAdv* m_pPresentAdv;
    IMcVideoPresenterStream* m_pMainStream;
    IMcVideoPresenterStream* m_pSubStream1;
    IMcVideoPresenterStream* m_pSubStream2;
    IMcVideoPresenterStream* m_pSubStream3;
    IMcVideoPresenterStream* m_pSubStream4;
    IMcVideoPresenterStream* m_pSubStream5;
    IMcVideoPresenterStream* m_pSubStream6;
	IMcVideoPresenterAnimation*	m_pAnimation;
	HRESULT TestV2(HMODULE& h_dll);
};
