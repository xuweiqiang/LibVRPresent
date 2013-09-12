// SetPage.cpp : implementation file
//

#include "stdafx.h"
#include "LibVrPresent_Test.h"
#include "SetPage.h"
#include "ddraw.h"

// CSetPage dialog

IMPLEMENT_DYNAMIC(CSetPage, CDialog)

CSetPage::CSetPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPage::IDD, pParent)
{
	m_pPresent = NULL;
}


#ifdef TEST_V2_SURF
void CSetPage::SetPresent(IMcVideoPresenterAdv* pPresent )
#else
void CSetPage::SetPresent(IMcVideoPresenter* pPresent )
#endif
{
	m_pPresent = pPresent;
	OnInitDialog();
}

CSetPage::~CSetPage()
{
}

void CSetPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LIGHT, m_SliderLight);
	DDX_Control(pDX, IDC_STATIC_LIGHTVALUE, m_LightValue);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_SliderContrast);
	DDX_Control(pDX, IDC_STATIC_CONTRASTVALUE, m_ContrastValue);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_SliderSaturation);
	DDX_Control(pDX, IDC_STATIC_SATURATIONVALUE, m_SaturationValue);
	DDX_Control(pDX, IDC_SLIDER_ACUTANCE, m_SliderAcutance);
	DDX_Control(pDX, IDC_STATIC_ACUTANCEVALUE, m_AcutanceValue);
	DDX_Control(pDX, IDC_SLIGHT, m_sLight);
	DDX_Control(pDX, IDC_SCONTRAST, m_sContrast);
	DDX_Control(pDX, IDC_SSATURATION, m_sSaturation);
	DDX_Control(pDX, IDC_SACUTANCE, m_sAcutance);
}

BEGIN_MESSAGE_MAP(CSetPage, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHT, &CSetPage::OnNMCustomdrawSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CONTRAST, &CSetPage::OnNMCustomdrawSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SATURATION, &CSetPage::OnNMCustomdrawSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACUTANCE, &CSetPage::OnNMCustomdrawSlider)
END_MESSAGE_MAP()

// CSetPage message handlers

BOOL CSetPage::OnInitDialog()
{
	int iVal = 0;
	CDialog::OnInitDialog();

	Get(eBRIGHTNESS,iVal);
	m_SliderLight.SetRange(MIN_COLORCONTROL_VAL, MAX_COLORCONTROL_VAL, TRUE);
	m_SliderLight.SetPos(iVal);
	m_SliderLight.SetTicFreq(10);
	
	Get(eCONTRAST,iVal);
	m_SliderContrast.SetRange(MIN_COLORCONTROL_VAL, MAX_COLORCONTROL_VAL, TRUE);
	m_SliderContrast.SetPos(iVal);
	m_SliderContrast.SetTicFreq(10);

	Get(eSATURATION,iVal);
	m_SliderSaturation.SetRange(MIN_COLORCONTROL_VAL, MAX_COLORCONTROL_VAL, TRUE);
	m_SliderSaturation.SetPos(iVal);
	m_SliderSaturation.SetTicFreq(10);

	Get(eSHARPNESS,iVal);
	m_SliderAcutance.SetRange(MIN_COLORCONTROL_VAL, MAX_COLORCONTROL_VAL, TRUE);
	m_SliderAcutance.SetPos(iVal);
	m_SliderAcutance.SetTicFreq(10);

	return 0;
}

void CSetPage::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	TCHAR strName[10];

	CSliderCtrl* pSlider;
	CStatic* pTxt;
	CStatic* pVal;
	CPARA_TYPE ePara;

	switch(pNMHDR->idFrom)
	{
		case IDC_SLIDER_LIGHT:
			pSlider = &m_SliderLight;
			pTxt = &m_sLight;
			pVal = &m_LightValue;
			ePara= eBRIGHTNESS;
			break;
		case IDC_SLIDER_CONTRAST:
			pSlider = &m_SliderContrast;
			pTxt = &m_sContrast;
			pVal = &m_ContrastValue;
			ePara= eCONTRAST;
			break;
		case IDC_SLIDER_SATURATION:
			pSlider = &m_SliderSaturation;
			pTxt = &m_sSaturation;
			pVal = &m_SaturationValue;
			ePara= eSATURATION;
			break;
		case IDC_SLIDER_ACUTANCE:
			pSlider = &m_SliderAcutance;
			pTxt = &m_sAcutance;
			pVal = &m_AcutanceValue;
			ePara= eSHARPNESS;
			break;
		default:
			return;
	}

	int iPos = pSlider->GetPos();
	
	Set(ePara, iPos);
	_stprintf(strName, _T("%d%%"), iPos);
	pVal->SetWindowText(strName);

	BOOL bEnable;
	if(GetCapability(ePara))
		bEnable = TRUE;
	else
		bEnable = FALSE;

	pTxt->EnableWindow(bEnable);
	pSlider->EnableWindow(bEnable);
	pVal->EnableWindow(bEnable);

	*pResult = 0;
	return;
}

BOOL CSetPage::GetCapability(CPARA_TYPE eType)
{
	DWORD dwId;
	VR_FILTERCONTROL sColor;
	if(m_pPresent == NULL)
		return FALSE;
	if(FAILED(m_pPresent->Get(IID_IMcVideoPresenter, MC_PROPID_VP_COLORSYS_CAP,NULL,NULL,(LPVOID)&sColor,NULL,NULL)))
		return FALSE;

	switch(eType)
	{
		case eBRIGHTNESS:
			dwId = DDCOLOR_BRIGHTNESS; 
			break;
		case eCONTRAST:
			dwId = DDCOLOR_CONTRAST; 
			break;
		case eHUE:
			dwId = DDCOLOR_HUE; 
			break;
		case eSATURATION:
			dwId = DDCOLOR_SATURATION ; 
			break;
		case eGAMMA:
			dwId = DDCOLOR_GAMMA ; 
			break;
		case eCOLORENABLE:
			dwId = DDCOLOR_COLORENABLE; 
			break;
		case eSHARPNESS:
			dwId = DDCOLOR_SHARPNESS ; 
			break;
		default:
			return FALSE;
	}

	return (sColor.dwFlags&dwId?TRUE:FALSE);
}

void CSetPage::Get(CPARA_TYPE eType, int& iValue)
{
	DWORD dwId;
	if(m_pPresent == NULL)
		return;

	switch(eType)
	{
		case eBRIGHTNESS:
			dwId = MC_PROPID_VP_BRIGHTNESS; 
			break;
		case eCONTRAST:
			dwId = MC_PROPID_VP_CONTRAST; 
			break;
		case eHUE:
			dwId = MC_PROPID_VP_HUE; 
			break;
		case eSATURATION:
			dwId = MC_PROPID_VP_SATURATION; 
			break;
		case eGAMMA:
			dwId = MC_PROPID_VP_GAMMA; 
			break;
		case eCOLORENABLE:
			dwId = MC_PROPID_VP_COLORENABLE; 
			break;
		case eSHARPNESS:
			dwId = MC_PROPID_VP_SHARPNESS; 
			break;
		default:
			return;
	}
	m_pPresent->Get( IID_IMcVideoPresenter, dwId,NULL,NULL,(LPVOID)&iValue,NULL,NULL);
	return;

}

void CSetPage::Set(CPARA_TYPE eType,const int iValue)
{
	DWORD dwId;
	if(m_pPresent == NULL)
		return;

	switch(eType)
	{
		case eBRIGHTNESS:
			dwId = MC_PROPID_VP_BRIGHTNESS; 
			break;
		case eCONTRAST:
			dwId = MC_PROPID_VP_CONTRAST; 
			break;
		case eHUE:
			dwId = MC_PROPID_VP_HUE; 
			break;
		case eSATURATION:
			dwId = MC_PROPID_VP_SATURATION; 
			break;
		case eGAMMA:
			dwId = MC_PROPID_VP_GAMMA; 
			break;
		case eCOLORENABLE:
			dwId = MC_PROPID_VP_COLORENABLE; 
			break;
		case eSHARPNESS:
			dwId = MC_PROPID_VP_SHARPNESS; 
			break;
		default:
			return;
	}
	m_pPresent->Set(IID_IMcVideoPresenter, dwId,NULL,NULL,(LPVOID)&iValue,NULL);
	return;

}

