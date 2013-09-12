// OpenGl_Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OpenGl_Test.h"

#include	<stdio.h>							// ��׼����/������ͷ�ļ�
#include <gl\gl.h> 
#include <gl\glu.h> 
#pragma comment(lib, "openGL32.lib")
#pragma comment(lib, "GLu32.lib")

#define MAX_LOADSTRING 100
#define SRC_WIDTH 320
#define SRC_HEIGHT 240

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

HWND mhWnd;
HDC mhDC = NULL;
HGLRC mhRC;
bool mloop = TRUE;
bool mfullscreen = TRUE;
bool	mkeys[256];								
bool	mactive=TRUE;							
GLuint		mtexture[1] = {0};							// �洢һ������
static FILE* m_file = NULL;
LPBYTE m_pbuf = NULL;
LPBYTE m_pSwapbuf = NULL;
GLfloat mLightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// ���������
GLfloat mLightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // ��������
GLfloat mLightPosition[]= { 0.0f, 0.0f, -3.0f, 1.0f };				 // ��Դλ��
GLuint	mfilter = 0;									// �˲�����
float mx = 0.0f;
float my = 0.0f;
float mz = -8.0f;
bool mAlpha = FALSE;
bool mLp = FALSE;
bool mFov = FALSE;
GLUquadricObj *mquadratic;					// ���μ�����

void ReSizeGLScene(GLsizei width, GLsizei height);
int KillGLWindow();
int OnPaintOpenGL();
int LoadGLTextures(int srcWidth , int srcHeight);

int CreateGLWindow(HINSTANCE hInstance,int width, int height,int bits ,bool fullscreenflag)
{
	mfullscreen  = fullscreenflag;

	WNDCLASS	wc;							
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		
	wc.lpfnWndProc		= (WNDPROC) WndProc;				
	wc.cbClsExtra		= 0;						
	wc.cbWndExtra		= 0;						
	wc.hInstance		= hInstance;//GetModuleHandle(NULL);					
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;						
	wc.lpszMenuName		= NULL;						
	wc.lpszClassName	= _T("OpenG");

	if (!RegisterClass(&wc))						
	{
		MessageBox(NULL,_T("RegisterClass failed"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if (mfullscreen)								
	{
		DEVMODE dmScreenSettings;						
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,_T("Faild to switch to fullscreen mode\nUse Window mode��"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				mfullscreen=FALSE;				
			}
			else
			{
				MessageBox(NULL,_T("To be closed"),_T("Error"),MB_OK|MB_ICONSTOP);
				return FALSE;					
			}
		}
	}

	DWORD		dwExStyle;						
	DWORD		dwStyle;						
	if (mfullscreen)								
	{
		dwExStyle=WS_EX_APPWINDOW;					
		dwStyle=WS_POPUP;						
		ShowCursor(FALSE);						
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;					
	}

	RECT WindowRect;							
	WindowRect.left=(long)0;						
	WindowRect.right=(long)width;						
	WindowRect.top=(long)0;							
	WindowRect.bottom=(long)height;					
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	if (!(mhWnd=CreateWindowEx(	dwExStyle,				
					_T("OpenG"),				
					NULL,					
					WS_CLIPSIBLINGS |			
					WS_CLIPCHILDREN |			
					dwStyle,				
					0, 0,					
					WindowRect.right-WindowRect.left,	
					WindowRect.bottom-WindowRect.top,	
					NULL,					
					NULL,					
					hInstance,				
					NULL)))							
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Failed to create window"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	return TRUE;
}

int InitOpenGL(HWND hWnd,int bits, int width, int height)
{
	static	PIXELFORMATDESCRIPTOR pfd=					
	{
		sizeof(PIXELFORMATDESCRIPTOR),					
		1,								
		PFD_DRAW_TO_WINDOW |			
		PFD_SUPPORT_OPENGL |			
		PFD_DOUBLEBUFFER,				
		PFD_TYPE_RGBA,					
		bits,							
		0, 0, 0, 0, 0, 0,				
		0,								
		0,								
		0,								
		0, 0, 0, 0,						
		16,								
		0,								
		0,								
		PFD_MAIN_PLANE,					
		0,								
		0, 0, 0							
	};

	if (!(mhDC=GetDC(hWnd)))							
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Failed to get device context"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	int PixelFormat = 0;
	if (!(PixelFormat=ChoosePixelFormat(mhDC,&pfd)))				
	{
		KillGLWindow();							
		MessageBox(NULL,_T("No compatible pixcel format is found "),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!SetPixelFormat(mhDC,PixelFormat,&pfd))				
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Set pixelformat failed "),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	PIXELFORMATDESCRIPTOR pxcel;
	memset(&pxcel, 0 ,sizeof(pxcel));
	DescribePixelFormat(mhDC, PixelFormat, sizeof(pxcel), &pxcel);

/*****************************************************************************************************************************/
	if (!(mhRC=wglCreateContext(mhDC)))					
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Failed to create rendering context"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}
	if(!wglMakeCurrent(mhDC,mhRC))						
	{
		KillGLWindow();							
		MessageBox(NULL,_T("Failed to bind rendering context to device context"),_T("Error"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}
	ShowWindow(mhWnd,SW_SHOW);					
	SetForegroundWindow(mhWnd);					
	SetFocus(hWnd);								
	ReSizeGLScene(width, height);				

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		
	glClearDepth(1.0f);							
	glEnable(GL_DEPTH_TEST);					
	glDepthFunc(GL_LEQUAL);						
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			

	glLightfv(GL_LIGHT1, GL_AMBIENT, mLightAmbient);				// ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mLightDiffuse);				// ���������
	glLightfv(GL_LIGHT1, GL_POSITION,mLightPosition);			// ���ù�Դλ��
	glEnable(GL_LIGHT1);							// ����һ�Ź�Դ

	glColor4f(1.0f,1.0f,1.0f,0.8f);			// ȫ���ȣ� 50% Alpha ���
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
//	glDisable(GL_BLEND);		// �رջ��
//	glEnable(GL_DEPTH_TEST);	// ����Ȳ���

	glPolygonMode( GL_BACK, GL_LINE);			// �������ȫ���
	glPolygonMode( GL_FRONT,GL_FILL );			// ǰ����ʹ����������

	glEnable(GL_COLOR_MATERIAL);				// ʹ����ɫ����

	GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		// �����ɫ��Ϊ��ɫ

	glFogi(GL_FOG_MODE, GL_EXP2);		// ����������ģʽ
	glFogfv(GL_FOG_COLOR, fogColor);			// ���������ɫ
	glFogf(GL_FOG_DENSITY, 0.15f);			// ��������ܶ�
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// ����ϵͳ��μ�������
	glFogf(GL_FOG_START, 1.0f);				// �����Ŀ�ʼλ��
	glFogf(GL_FOG_END, 0.0f);				// �����Ľ���λ��


	mquadratic=gluNewQuadric();				// �������μ�����
	gluQuadricNormals(mquadratic, GLU_SMOOTH);		// ʹ��ƽ������
	gluQuadricTexture(mquadratic, GL_TRUE);		// ʹ������

	return TRUE;
}

int KillGLWindow()
{
	gluDeleteQuadric(mquadratic);				// ɾ�����μ�����
	if (mfullscreen)								
	{
		ChangeDisplaySettings(NULL,0);				
		ShowCursor(TRUE);						
	}

	if (mhRC)								
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,_T("Failed to release binding"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(mhRC))					
		{
			MessageBox(NULL,_T("Failed to release rendering context"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		}
		mhRC=NULL;							
	}

	if (mhDC && !ReleaseDC(mhWnd,mhDC))					
	{
		MessageBox(NULL,_T("Failed to release device context"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		mhDC=NULL;							
	}

	if (mhWnd && !DestroyWindow(mhWnd))					
	{
		MessageBox(NULL,_T("Failed to destryo window"),_T("Error"),MB_OK | MB_ICONINFORMATION);
		mhWnd=NULL;							
	}

	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	if(m_pbuf)
	{
		free(m_pbuf);
		m_pbuf = NULL;
	}	
	if(m_pSwapbuf)
	{
		free(m_pSwapbuf);
		m_pSwapbuf = NULL;
	}	

	return TRUE;
}

void ReSizeGLScene(GLsizei width, GLsizei height)				
{
	if (height==0)								
		height=1;							

	glViewport(0, 0, width, height);					

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();							

	
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();
}

int LoadGLTextures(int srcWidth , int srcHeight)
{
	static int m_FileLength = 0;
	int FrameSize = srcWidth*srcHeight*3;

	if((m_file == NULL)&&(m_pbuf == NULL))
	{
		m_file = fopen(".\\888_qvga.dat","rb");
		if(m_file == NULL)
			return FALSE;
		fseek(m_file,0,SEEK_END);
		m_FileLength = ftell(m_file);
		fseek(m_file, 0 , SEEK_SET);
		m_pbuf = (LPBYTE)malloc(FrameSize);
		m_pSwapbuf = (LPBYTE)malloc(FrameSize);
	}

	if(!(((m_FileLength - ftell(m_file)) >= FrameSize)&&(!feof(m_file))))
		fseek(m_file, 0 , SEEK_SET);

	fread(m_pbuf ,sizeof(BYTE),FrameSize,m_file);

	for(int i=0;i<srcHeight;i++)
	memcpy(m_pSwapbuf+FrameSize - (i+1)*srcWidth*3, m_pbuf+i*srcWidth*3, srcWidth*3);

	if(mtexture[0] == 0)
		glGenTextures(1, &mtexture[0]);					// ��������
	// ʹ������λͼ�������� �ĵ�������
	glBindTexture(GL_TEXTURE_2D, mtexture[0]);


	if(mfilter == 0)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
		glTexImage2D(GL_TEXTURE_2D, 0, 1, srcWidth, srcHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pSwapbuf);
	}
	else if(mfilter == 1)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, srcWidth, srcHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pSwapbuf);
	}
	else if(mfilter == 2)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, srcWidth, srcHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_pSwapbuf); 
	}
	else
		glDisable(GL_TEXTURE_2D);


	return TRUE;

}

int OnPaintOpenGL()
{
	LoadGLTextures(SRC_WIDTH ,SRC_HEIGHT);

	static GLfloat		rtri = 0;						// ���������εĽǶ�
	static GLfloat		rquad = 0;						// �����ı��εĽǶ�

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();

	glTranslatef(mx,my,mz);				// ����1.5��λ,��������Ļ 6.0
	glRotatef(rquad,1.0f,1.0f,1.0f);			//  ��X����ת�ı���

	glBindTexture(GL_TEXTURE_2D, mtexture[0]);				// ѡ������

	int iCol = 0;
	static GLfloat boxcol[6][3]=				// ���ӵ���ɫ����
	{
		// ��:�죬�ȣ��ƣ��̣���
		{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
	};

	glBegin(GL_QUADS);
		// ǰ����
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 0.0f, 1.0f);					// ����ָ��۲���
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		// �����
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 0.0f,-1.0f);					// ���߱���۲���
		glTexCoord2f(2.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(2.0f, 2.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 2.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		// ����
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 1.0f, 0.0f);					// ��������
		glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.5f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.5f, 0.5f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		// ����
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f,-1.0f, 0.0f);					// ���߳���
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		// �Ҳ���
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 1.0f, 0.0f, 0.0f);					// ���߳���
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		// �����
		glColor3fv(boxcol[iCol++]);	
		glNormal3f(-1.0f, 0.0f, 0.0f);					// ���߳���
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();


		glColor3fv(boxcol[0]);	
		glRotatef(-1.0f*rquad,1.0f,1.0f,1.0f);			//  ��X����ת�ı���
		glRotatef(rquad,1.0f,0.0f,0.0f);			//  ��X����ת�ı���
		glTranslatef(3.0f,0.0f,-1.5f);			
		gluCylinder(mquadratic,0.5f,0.5f,3.0f,128,128);	


	rtri+=1.0f;
	rquad += 0.5f;

	SwapBuffers(mhDC);			

	return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	//if(MessageBox(NULL,_T("Do you want to run on fullscreen��"), _T("Set DisplayMode"),MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		mfullscreen=FALSE;						
	}

	if (!CreateGLWindow(hInstance,640,480,16,mfullscreen))
	{
		return 0;							
	}

	if(!InitOpenGL(mhWnd,24,640,480))
	{
		return 0;							
	}

 	// TODO: Place code here.
	MSG msg;
	int ret = 0;
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGL_TEST));

	// Main message loop:
	while (mloop)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
			if (mactive)
			{
				if (mkeys[VK_ESCAPE])				
				{
					mloop=FALSE;				
					mkeys[VK_ESCAPE] = FALSE; 				
				}


				if(mkeys[VK_F2])
				{
					if(mfilter >= 3)
						mfilter=0;
					else
						mfilter++;
					mkeys[VK_F2] = FALSE;
				}


				if (mkeys['A'])				// PageUp������?
				{
					mx-=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys['A'] = FALSE;
				}
				if (mkeys['D'])				// PageUp������?
				{
					mx+=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys['D'] = FALSE;
				}

				if (mkeys['W'])				// PageUp������?
				{
					my+=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys['W'] = FALSE;
				}
				if (mkeys['S'])				// PageUp������?
				{
					my-=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys['S'] = FALSE;
				}

				if (mkeys[VK_PRIOR])				// PageUp������?
				{
					mz-=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys[VK_PRIOR] = FALSE;
				}
				if (mkeys[VK_NEXT])				// PageUp������?
				{
					mz+=0.4f;				// �����£���ľ��������Ļ�ڲ�
					mkeys[VK_NEXT] = FALSE;
				}

				if (mkeys['L'])				// PageUp������?
				{
					if(mLp)
					{
						glDisable(GL_LIGHTING);		// ���ù�Դ
						mLp = FALSE;
					}
					else
					{
						glEnable(GL_LIGHTING);		// ���ù�Դ
						mLp = TRUE;
					}
					mkeys['L'] = FALSE;
				}

				if (mkeys['B'])				// PageUp������?
				{					
					if(mAlpha)
					{
						glDisable(GL_BLEND);		// �رջ��
						glEnable(GL_DEPTH_TEST);	// ����Ȳ���
						mAlpha = FALSE; 
					}
					else
					{
						glEnable(GL_BLEND);		// �򿪻��
						glDisable(GL_DEPTH_TEST);	// �ر���Ȳ���
						mAlpha = TRUE; 
					}
					mkeys['B'] = FALSE;
				}

				if (mkeys['F'])				// PageUp������?
				{					
					if(mFov)
					{
						glClearColor(0.0f,0.0f,0.0f,1.0f);			// ���ñ�������ɫΪ��������ɫ
						glDisable(GL_FOG);		// �رջ��
						mFov= FALSE; 
					}
					else
					{
						glClearColor(0.5f,0.5f,0.5f,1.0f);			// ���ñ�������ɫΪ��������ɫ
						glEnable(GL_FOG);					// ʹ������
						mFov= TRUE; 
					}
					mkeys['F'] = FALSE;
				}
				

				OnPaintOpenGL();
			}
	}

	return (int) msg.wParam;
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
     case WM_SIZE:        // Change the window size.
		ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );
		break;
	case WM_DESTROY:
		KillGLWindow();
		PostQuitMessage(0);
		mloop =FALSE;
		break;
	//case WM_KEYDOWN:						
	//	mkeys[wParam] = TRUE;					
	//	break;
	case WM_KEYUP:							
		mkeys[wParam] = TRUE;				
		break;
	case WM_ACTIVATE:						
		if (!HIWORD(wParam))				
		{
			mactive=TRUE;					
		}
		else
		{
			mactive=FALSE;					
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
