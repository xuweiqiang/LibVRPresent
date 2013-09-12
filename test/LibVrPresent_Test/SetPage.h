#pragma once
#include "afxcmn.h"
#include "afxwin.h"
//#include "GfxContext.h"
#include "GfxPresent.h"

#define TEST_V2_SURF

typedef enum
{
	eBRIGHTNESS,
	eCONTRAST,
	eHUE,
	eSATURATION,
	eGAMMA,
	eCOLORENABLE,
	eSHARPNESS,
}CPARA_TYPE;


// CSetPage dialog

class CSetPage : public CDialog
{
	DECLARE_DYNAMIC(CSetPage)

public:
	CSetPage(CWnd* pParent = NULL);   // standard constructor
#ifdef TEST_V2_SURF
	void SetPresent(IMcVideoPresenterAdv* pPresent );
#else
	void SetPresent(IMcVideoPresenter* pPresent );
#endif
	virtual BOOL OnInitDialog(void);
	virtual ~CSetPage();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);

	CSliderCtrl m_SliderLight;
	CStatic m_LightValue;
	CSliderCtrl m_SliderContrast;
	CStatic m_ContrastValue;
	CSliderCtrl m_SliderSaturation;
	CStatic m_SaturationValue;
	CSliderCtrl m_SliderAcutance;
	CStatic m_AcutanceValue;
	CStatic m_sLight;
	CStatic m_sContrast;
	CStatic m_sSaturation;
	CStatic m_sAcutance;

	#ifdef TEST_V2_SURF
	IMcVideoPresenterAdv* m_pPresent;
	#else
	IMcVideoPresenter* m_pPresent;
	#endif
	void Get(CPARA_TYPE eType, int& iValue);
	void Set(CPARA_TYPE eType,const int iValue);
	BOOL GetCapability(CPARA_TYPE eType);

};
