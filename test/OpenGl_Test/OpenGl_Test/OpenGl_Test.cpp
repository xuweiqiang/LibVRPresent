// OpenGl_Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OpenGl_Test.h"

#include	<stdio.h>							// 标准输入/输出库的头文件
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
GLuint		mtexture[1] = {0};							// 存储一个纹理
static FILE* m_file = NULL;
LPBYTE m_pbuf = NULL;
LPBYTE m_pSwapbuf = NULL;
GLfloat mLightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// 环境光参数
GLfloat mLightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // 漫射光参数
GLfloat mLightPosition[]= { 0.0f, 0.0f, -3.0f, 1.0f };				 // 光源位置
GLuint	mfilter = 0;									// 滤波类型
float mx = 0.0f;
float my = 0.0f;
float mz = -8.0f;
bool mAlpha = FALSE;
bool mLp = FALSE;
bool mFov = FALSE;
GLUquadricObj *mquadratic;					// 二次几何体

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
			if (MessageBox(NULL,_T("Faild to switch to fullscreen mode\nUse Window mode？"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
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

	glLightfv(GL_LIGHT1, GL_AMBIENT, mLightAmbient);				// 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mLightDiffuse);				// 设置漫射光
	glLightfv(GL_LIGHT1, GL_POSITION,mLightPosition);			// 设置光源位置
	glEnable(GL_LIGHT1);							// 启用一号光源

	glColor4f(1.0f,1.0f,1.0f,0.8f);			// 全亮度， 50% Alpha 混合
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// 基于源象素alpha通道值的半透明混合函数
//	glDisable(GL_BLEND);		// 关闭混合
//	glEnable(GL_DEPTH_TEST);	// 打开深度测试

	glPolygonMode( GL_BACK, GL_LINE);			// 后表面完全填充
	glPolygonMode( GL_FRONT,GL_FILL );			// 前表面使用线条绘制

	glEnable(GL_COLOR_MATERIAL);				// 使用颜色材质

	GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		// 雾的颜色设为白色

	glFogi(GL_FOG_MODE, GL_EXP2);		// 设置雾气的模式
	glFogfv(GL_FOG_COLOR, fogColor);			// 设置雾的颜色
	glFogf(GL_FOG_DENSITY, 0.15f);			// 设置雾的密度
	glHint(GL_FOG_HINT, GL_DONT_CARE);			// 设置系统如何计算雾气
	glFogf(GL_FOG_START, 1.0f);				// 雾气的开始位置
	glFogf(GL_FOG_END, 0.0f);				// 雾气的结束位置


	mquadratic=gluNewQuadric();				// 创建二次几何体
	gluQuadricNormals(mquadratic, GLU_SMOOTH);		// 使用平滑法线
	gluQuadricTexture(mquadratic, GL_TRUE);		// 使用纹理

	return TRUE;
}

int KillGLWindow()
{
	gluDeleteQuadric(mquadratic);				// 删除二次几何体
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
		glGenTextures(1, &mtexture[0]);					// 创建纹理
	// 使用来自位图数据生成 的典型纹理
	glBindTexture(GL_TEXTURE_2D, mtexture[0]);


	if(mfilter == 0)
	{
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
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

	static GLfloat		rtri = 0;						// 用于三角形的角度
	static GLfloat		rquad = 0;						// 用于四边形的角度

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();

	glTranslatef(mx,my,mz);				// 右移1.5单位,并移入屏幕 6.0
	glRotatef(rquad,1.0f,1.0f,1.0f);			//  绕X轴旋转四边形

	glBindTexture(GL_TEXTURE_2D, mtexture[0]);				// 选择纹理

	int iCol = 0;
	static GLfloat boxcol[6][3]=				// 盒子的颜色数组
	{
		// 亮:红，橙，黄，绿，蓝
		{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
	};

	glBegin(GL_QUADS);
		// 前侧面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 0.0f, 1.0f);					// 法线指向观察者
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		// 后侧面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 0.0f,-1.0f);					// 法线背向观察者
		glTexCoord2f(2.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(2.0f, 2.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 2.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		// 顶面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f, 1.0f, 0.0f);					// 法线向上
		glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.5f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.5f, 0.5f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		// 底面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 0.0f,-1.0f, 0.0f);					// 法线朝下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		// 右侧面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f( 1.0f, 0.0f, 0.0f);					// 法线朝右
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		// 左侧面
		glColor3fv(boxcol[iCol++]);	
		glNormal3f(-1.0f, 0.0f, 0.0f);					// 法线朝左
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();


		glColor3fv(boxcol[0]);	
		glRotatef(-1.0f*rquad,1.0f,1.0f,1.0f);			//  绕X轴旋转四边形
		glRotatef(rquad,1.0f,0.0f,0.0f);			//  绕X轴旋转四边形
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


	//if(MessageBox(NULL,_T("Do you want to run on fullscreen？"), _T("Set DisplayMode"),MB_YESNO|MB_ICONQUESTION)==IDNO)
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


				if (mkeys['A'])				// PageUp按下了?
				{
					mx-=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys['A'] = FALSE;
				}
				if (mkeys['D'])				// PageUp按下了?
				{
					mx+=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys['D'] = FALSE;
				}

				if (mkeys['W'])				// PageUp按下了?
				{
					my+=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys['W'] = FALSE;
				}
				if (mkeys['S'])				// PageUp按下了?
				{
					my-=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys['S'] = FALSE;
				}

				if (mkeys[VK_PRIOR])				// PageUp按下了?
				{
					mz-=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys[VK_PRIOR] = FALSE;
				}
				if (mkeys[VK_NEXT])				// PageUp按下了?
				{
					mz+=0.4f;				// 若按下，将木箱移向屏幕内部
					mkeys[VK_NEXT] = FALSE;
				}

				if (mkeys['L'])				// PageUp按下了?
				{
					if(mLp)
					{
						glDisable(GL_LIGHTING);		// 禁用光源
						mLp = FALSE;
					}
					else
					{
						glEnable(GL_LIGHTING);		// 启用光源
						mLp = TRUE;
					}
					mkeys['L'] = FALSE;
				}

				if (mkeys['B'])				// PageUp按下了?
				{					
					if(mAlpha)
					{
						glDisable(GL_BLEND);		// 关闭混合
						glEnable(GL_DEPTH_TEST);	// 打开深度测试
						mAlpha = FALSE; 
					}
					else
					{
						glEnable(GL_BLEND);		// 打开混合
						glDisable(GL_DEPTH_TEST);	// 关闭深度测试
						mAlpha = TRUE; 
					}
					mkeys['B'] = FALSE;
				}

				if (mkeys['F'])				// PageUp按下了?
				{					
					if(mFov)
					{
						glClearColor(0.0f,0.0f,0.0f,1.0f);			// 设置背景的颜色为雾气的颜色
						glDisable(GL_FOG);		// 关闭混合
						mFov= FALSE; 
					}
					else
					{
						glClearColor(0.5f,0.5f,0.5f,1.0f);			// 设置背景的颜色为雾气的颜色
						glEnable(GL_FOG);					// 使用雾气
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
