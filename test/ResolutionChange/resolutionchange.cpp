// ResolutionChange.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ResolutionChange.h"

#define MAX_LOADSTRING 100
#define TIMER_FREQUENCY 	10000
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static sRESOLUTION_LIST* g_devList = NULL;
static sBACKUP_RES sbackup[10];
static DWORD sbackup_count = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int ChangeResolution(int devNum, LPDEVMODE pmode)
{
	sRESOLUTION_LIST* p_resTmp = g_devList;
	BOOL bfound = FALSE;
	if(pmode != NULL)
	{
		while(p_resTmp != NULL)
		{
			if(p_resTmp->devNum == devNum)
			{
				bfound = TRUE;
				break;
			}
			p_resTmp = p_resTmp->next_dev;

		}
		//Change resolution
		if(bfound)
			ChangeDisplaySettingsEx(p_resTmp->sDev.DeviceName, pmode,NULL,CDS_RESET|CDS_UPDATEREGISTRY,NULL);
	}

	return 0;
}

/********************************************
	Save mode to display settings link table of Resolution List
*********************************************/
int SaveToLoop(sMod_LOOP*& phead_loop, LPDEVMODE pmode)	
{
	if(phead_loop == NULL)
	{
		phead_loop = new sMod_LOOP;
		ZeroMemory(phead_loop, sizeof(sMod_LOOP));
		memcpy(&phead_loop->dispset,pmode, sizeof(phead_loop->dispset));
		return 0;
	}

	sMod_LOOP* plast_mode = phead_loop;
	while(plast_mode->next_mode != NULL)
	{
		plast_mode = plast_mode->next_mode;
	}

	sMod_LOOP* ptmp = new sMod_LOOP;
	ZeroMemory(ptmp, sizeof(sMod_LOOP));
	memcpy(&ptmp->dispset,pmode, sizeof(ptmp->dispset));
	pmode->dmPelsWidth =pmode->dmPelsWidth;
	pmode->dmPelsHeight = pmode->dmPelsHeight;
	plast_mode->next_mode = ptmp;
	return 0;
}


/********************************************
	Save mode to Resolution List
*********************************************/
int SaveToList(DWORD devNum, LPDEVMODE pmode , PDISPLAY_DEVICE  pDev)
{
	/*Initiliase resolution list if it's not available*/
	if(g_devList==NULL)
	{
		g_devList = new sRESOLUTION_LIST;
		ZeroMemory(g_devList, sizeof(sRESOLUTION_LIST));
		g_devList->devNum = devNum;
		SaveToLoop(g_devList->pDispSet_loophead, pmode);
		memcpy(&g_devList->sDev, pDev, sizeof(g_devList->sDev));

	}
	else
	{
		// Searching for same deviceNum
		BOOL bfound_dev = FALSE;
		sRESOLUTION_LIST* res_tmp =g_devList;
		while(1)
		{
			if(res_tmp->devNum == devNum)
			{
				bfound_dev = TRUE;
				break;
			}
			if(res_tmp->next_dev !=NULL)
				res_tmp = res_tmp->next_dev;
			else
				break;	//Not found
		}

		if(bfound_dev)	//Finding same device
		{
			SaveToLoop(res_tmp->pDispSet_loophead, pmode);
		}
		else
		{
			//Create a new one
			sRESOLUTION_LIST* res_tmp_new = new sRESOLUTION_LIST;
			ZeroMemory(res_tmp_new, sizeof(sRESOLUTION_LIST));
			res_tmp_new->devNum = devNum;
			SaveToLoop(res_tmp_new->pDispSet_loophead, pmode);	
			memcpy(&res_tmp_new->sDev, pDev, sizeof(res_tmp_new->sDev));
			res_tmp->next_dev = res_tmp_new;
		}
	}
	return 0;
}

/*************************************************
	Enumerate all the display devices and all the settings. Save them 
to resolution list.
*************************************************/
int InitAll()
{
	DISPLAY_DEVICE sDev;
	ZeroMemory(&sDev, sizeof(sDev));
	sDev.cb = sizeof(DISPLAY_DEVICE);

	ZeroMemory(&sbackup, sizeof(sbackup));
	int iDev_num=0;
	BOOL ret = TRUE;

	do
	{
		ret = EnumDisplayDevices(NULL,iDev_num,&sDev,0);
		if(ret)
		{
			sbackup[sbackup_count].devNum = iDev_num; 
			sbackup[sbackup_count].dispset.dmSize = sizeof(DEVMODE); 
			EnumDisplaySettings(sDev.DeviceName,ENUM_CURRENT_SETTINGS,&sbackup[sbackup_count].dispset);
			sbackup_count++;
			DWORD iModeNum =0;

			while(ret)
			{
				DEVMODE sMode;
				ZeroMemory(&sMode, sizeof(sMode));
				sMode.dmSize = sizeof(DEVMODE);
				
				ret = EnumDisplaySettings(sDev.DeviceName,iModeNum,&sMode);
				if(ret)
				{
					SaveToList(iDev_num,&sMode,&sDev);
				}
				iModeNum++;
			}
			ret = TRUE;
		}
		iDev_num++;
	}while(ret);
	
	if(g_devList == NULL) //No supported resolution is found.
		return -1;
	return 0;
}

/*************************************************
	Unit all
*************************************************/
int Uninit()
{
	/*Revert to the resolution before*/
	for(DWORD i=0;i<sbackup_count;i++)
	{
		sRESOLUTION_LIST* p_resTmp = g_devList;
		while(p_resTmp != NULL)
		{
			if(p_resTmp->devNum == sbackup[i].devNum)
				break;
			p_resTmp= p_resTmp->next_dev;
		}
		if(p_resTmp != NULL)//Change resolution
			ChangeDisplaySettingsEx(p_resTmp->sDev.DeviceName, &sbackup[i].dispset,NULL,CDS_RESET|CDS_UPDATEREGISTRY,NULL);
	}
	sbackup_count = 0;
	//Release resolution List
	sRESOLUTION_LIST* pList = g_devList;
	sRESOLUTION_LIST* pList_next = pList;

	sMod_LOOP* pLoop;
	sMod_LOOP* pLoop_next;
	while(pList != NULL)
	{
		pList_next = pList->next_dev;

		//Destroy all modes in a device
		{
			pLoop = pList->pDispSet_loophead;
			pLoop_next = pLoop->next_mode;
			while(pLoop!=NULL)
			{
				pLoop_next = pLoop->next_mode;
				delete pLoop;
				pLoop = pLoop_next;
			}
		}

		delete pList;
		pList = pList_next;
	}

	g_devList = NULL;
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RESOLUTIONCHANGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	if(InitAll() != 0)
		return -1;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESOLUTIONCHANGE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Uninit();
	return (int) msg.wParam;
}


static int GetResolution(int devNum, eRES_CHANGE_TYPE type, LPDEVMODE pmode)
{
	BOOL bfound = FALSE;
	sRESOLUTION_LIST* p_resTmp = g_devList;

	//Find device		
	if(pmode != NULL)
	{
		while(p_resTmp != NULL)
		{
			if(p_resTmp->devNum == devNum)
			{
				bfound = TRUE;
				break;
			}
			p_resTmp = p_resTmp->next_dev;

		}
	}

	if(bfound == FALSE)
		return -1;


	int icount =1; 
	switch(type)
	{
		case STANDARD:
			icount = 1;
			break;
		case RANDOM:
			icount = rand();
			break;
		default:
			break;
	}

	int i = 0;
	static sMod_LOOP* pcurrenct_mode = p_resTmp->pDispSet_loophead;

	while(1)
	{
		if(pcurrenct_mode->next_mode != NULL)
			pcurrenct_mode = pcurrenct_mode->next_mode;
		else
			pcurrenct_mode = p_resTmp->pDispSet_loophead;
		if(++i>= icount)
			break;
	}

	*pmode = pcurrenct_mode->dispset;
	return 0;

}


/*************************************************
	A timer is created while initialization. This's callback function of timer.
*************************************************/
static int DoTimer() 
{
	DEVMODE mode;
	int deviceNum = 0;
// Get device Num
	sRESOLUTION_LIST* p_resTmp = g_devList;
		
	while(p_resTmp != NULL)
	{
		deviceNum++;
		p_resTmp = p_resTmp->next_dev;
	}

	for(int i=0;i<deviceNum;i++)
	{
		if(GetResolution(i,RANDOM,&mode)  == 0)
			ChangeResolution(i, &mode);
	}

	
	return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESOLUTIONCHANGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RESOLUTIONCHANGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

	if(SetTimer(hWnd,ID_TIMER,TIMER_FREQUENCY,NULL) == 0)
		return FALSE;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMER);	
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		DoTimer();
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
