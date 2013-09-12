#pragma once


// CDispDlg dialog
#include <rfb/rfbclient.h>
#include <VR/GfxPresent.h>


class CDispDlg : public CDialog
{
	DECLARE_DYNAMIC(CDispDlg)

public:
	CDispDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDispDlg();

	void SetVncServer(const char * serverAddr);

	static rfbBool FrameBufferResizeCb(rfbClient *client)
	{
		CDispDlg * dlg = (CDispDlg *)rfbClientGetClientData(client, (void *)tagMain);
		return dlg->OnRfbResize(client);
	}

	static void FrameBufferUpdateCb(rfbClient *client, int x, int y, int w, int h)
	{
		CDispDlg * dlg = (CDispDlg *)rfbClientGetClientData(client, (void *)tagMain);
		return dlg->OnRfbUpdate(client, x, y, w, h);
	}

// Dialog Data
	enum { IDD = IDD_DISP };

protected:
	bool Connect();
	rfbBool OnRfbResize(rfbClient *client);
	void OnRfbUpdate(rfbClient *client, int x, int y, int w, int h);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	rfbClient * m_rfbClient;
	CString m_serverAddr;
	static const int tagMain = 1;

	IMcVideoPresenterAdv* m_pPresenterAdv ;
	IMcVideoPresenterStream* m_pStream1;
	VP_CANVASINFO_PARAMS m_canvasInfo;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
