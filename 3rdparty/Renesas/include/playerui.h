//********************************************************************
//Renesas Player User Interface
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//User Interface
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _PLAYER_UI_H
#define _PLAYER_UI_H

//**************************************
//Behavior
//**************************************


//**************************************
//Screen Type
//**************************************
enum UI_SCN_TYPE{
	UI_SCN_CONTROLPANEL = 0,
	UI_SCN_LOGO_STARTUP,
	UI_SCN_LOGO_AAC,
};

//**************************************
//Public Function
//**************************************
bool UI_OpenFileWindow(HWND hwnd,WCHAR *filename);
void UI_MessageBox(HWND hWnd , LPCTSTR lpText, LPCTSTR lpCaption);
void UI_ShowLogo(HWND hWnd , HDC hdc , HINSTANCE hinst , UI_SCN_TYPE type);
void UI_ShowPlayString(HWND hWnd, LPCTSTR lpString, LPRECT lpRect, int x , int y);

#endif //_PLAYER_UI_H