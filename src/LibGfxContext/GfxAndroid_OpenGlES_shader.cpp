//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2010  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include "../../inc/GfxBuildDef.h"

#ifdef GFX_ANDROID_OPENGL_ES_SHADER

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <android/log.h>

static int frames = 0;
#define PI (3.141592)
#define PI_OVER_180 (PI/180.0)


float gw = 800;
float gh = 480;


static float vw = 720;
static float vh = 480;

GLuint g_shaderProgramMvp = 0;

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
    "}\n";

static GLuint fs = 0;      //GL fragment shader id
static GLuint vs = 0;      //GL vertex shader id

GLuint texture;
static const char* VertexVertexText = 
	"precision highp float;\n"
	"\n"
	"attribute vec4 vertexPos;\n"
	"\n"
	"attribute vec4 texCoord;\n"
	"\n"
	"uniform mat4 mvp;\n"
	"\n"
	"varying vec4 texC;\n"
	"\n"
	"void main()\n"
	"{\n"
	"   gl_Position = mvp*vertexPos;\n"
	"   \n"
	"   texC = texCoord;\n"
	"}\n"
	"";

static const char* FragmentFragmentText = 
	"precision highp float;\n"
	"\n"
	"uniform sampler2D TexImages0;\n"
	"\n"
	"varying vec4 texC;\n"
	"\n"
	"void main()\n"
	"{\n"
	"   gl_FragColor = texture2D(TexImages0, texC.xy*1.0);\n"
	"\n"
	"}\n"
	"";


static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}


class CGfxAndroidOGLES_SHADER : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidOGLES_SHADER();
	virtual ~CGfxAndroidOGLES_SHADER();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags);
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags);
	STDMETHODIMP SetSrcRect(const RECT *pRect);
	STDMETHODIMP SetDstRect(const RECT *pRect);
	STDMETHODIMP SetScnRect(const RECT *pRect);
	STDMETHODIMP GetScnRect(RECT *pRect);

	STDMETHODIMP SetWindowHandle(HWND hwnd);
	STDMETHODIMP GetWindowHandle(HWND& hwnd);
	STDMETHODIMP SetAlpha(DWORD dwAlpha);
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey);
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey);
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP GetDDCaps(LONG* pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetWindowHandle(Window& hwnd)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
       m_screenParams= *((VR_ANDROID_ISUR_PARAM *)pSurf);
       gw = m_screenParams.scnWidth;
       gh = m_screenParams.scnHeight;
       
        DP("setscreen %d,%d \n",m_screenParams.scnWidth, m_screenParams.scnHeight);
		return S_OK;
	}
	
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP UIFlip(LPVOID pHandle);
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  


protected:

	bool setupGraphics(int w, int h);
	int InitGL(GLuint &prog);
	GLuint loadShader(GLenum shaderType, const char* pSource);
	 void  _loadTextures(GLubyte* image, int tex_name);
	 void  loadTextures();


	STDMETHODIMP Update();
	STDMETHODIMP CalculateGLStride(DWORD input, DWORD* pOut);

	bool m_bOpen;
	unsigned char *m_pBuff;
	unsigned char *m_pRGBBuff;
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD m_dwFourCC;
	bool m_bFirstTime;
	GLuint m_textueId;
	bool m_prepareFlag;
    VR_ANDROID_ISUR_PARAM m_screenParams;
	CRITICAL_SECTION m_csDisplay; 

};


CGfxAndroidOGLES_SHADER::CGfxAndroidOGLES_SHADER():
m_bOpen(false),
m_pBuff(0),
m_pRGBBuff(0),
m_dwWidth(0),
m_dwHeight(0),
m_dwFourCC(0),
m_bFirstTime(true),
m_prepareFlag(false)
{
	memset(&m_rectSrc, 0,sizeof(m_rectSrc));
	memset(&m_rectDst, 0,sizeof(m_rectDst));
	InitializeCriticalSection(&m_csDisplay);
}

CGfxAndroidOGLES_SHADER::~CGfxAndroidOGLES_SHADER()
{
	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CGfxAndroidOGLES_SHADER::CalculateGLStride(DWORD input, DWORD* pOut)
{
	if(!pOut ||input>1024 )	return E_FAIL;
	if(input >512)	*pOut = 512<<1;
	else if(input >512>>1) 	*pOut = 512;
	else if(input >512>>2) 	*pOut = 512>>1;
	else if(input >512>>3) 	*pOut = 512>>2;
	else if(input >512>>4) 	*pOut = 512>>3;
	else 
		*pOut = 512>>4;
	
	return S_OK;
}
static long tickAll = 0;

STDMETHODIMP CGfxAndroidOGLES_SHADER::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	tickAll =  _getTime();
	frames = 0;
	int i = 0;
	if(m_bOpen || pdwFourCC == NULL)	return E_FAIL;


	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	//CalculateGLStride(m_dwWidth, &m_glWidth);	
	//CalculateGLStride(m_dwHeight, &m_glHeight);


	DWORD type = 0;
	for(i=0;pdwFourCC[i];i++)
	{
		if(pdwFourCC[i] == MAKEFOURCC('R','8','8','8'))
		{
			m_pBuff = (unsigned char * )malloc(m_dwWidth  *m_dwHeight *3);
			memset(m_pBuff, 0,m_dwWidth *m_dwHeight*3);
			break;
		}

	}
	
	if(!pdwFourCC[i])
		return E_FAIL;


	__android_log_print(ANDROID_LOG_ERROR, "VRP", "Open (%d,%d)", dwWidth, dwHeight);

	if(!m_pBuff )	return E_FAIL;

	m_dwFourCC = MAKEFOURCC('R','8','8','8');

	
	m_bOpen = true;
	m_bFirstTime = 1;
	DP("shader: Open successed \n");
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::Close()
{
	if(!m_bOpen)	return E_FAIL;
	DP("shader: Close \n");

	if(m_pBuff)
	{
		free(m_pBuff);
		m_pBuff = NULL;
	}
	
	m_bOpen = false;
	return S_OK;
}
static bool bLock = false;


long g_tick = 0;

STDMETHODIMP CGfxAndroidOGLES_SHADER::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
//	DP("shader: Lock\n");

	EnterCriticalSection(&m_csDisplay);
	    if(ppBuf)
		    *ppBuf = m_pBuff;
	    if(pStride)
		    *pStride = m_dwWidth*3;
	    if(pPixCount)
		    *pPixCount = 12;

	m_prepareFlag = true;
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGLES_SHADER::Unlock(LONG num)
{
//	DP("shader: UnLock\n");

  for(int i =0;i<m_dwWidth*m_dwHeight;i++)
  {
    unsigned char r = m_pBuff[i*3];
    unsigned char b = m_pBuff[(i*3+2)];
    m_pBuff[i*3] = b;
    m_pBuff[(i*3+2)] = r;
  }
  
	LeaveCriticalSection(&m_csDisplay);
	//if(frames < 90)
	//__android_log_print(ANDROID_LOG_ERROR, "VRP", "CC:%ld", _getTime() - g_tick);
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGLES_SHADER::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;
}

void matIdentity(float m[16])
{
	for(int i=0; i<16; i++)
	{
		m[i] = (i%5==0)? 1.0f : 0.0f;  //The first and every fifth element after that is 1.0 other are 0.0
	}
}

bool matFrustum(float* m,float f32Left, float f32Right,float f32Bottom, float f32Top, float f32ZNear, float f32ZFar)
{
	float diff = f32Right - f32Left;
	if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;
	
	diff = f32Top - f32Bottom;
	if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;
		
	diff = f32ZFar - f32ZNear;
	if(diff > -FLT_EPSILON && diff < FLT_EPSILON) //chk for divide by zero......
		return false;	   
		
       m[0] = float(2.0*f32ZNear/(f32Right-f32Left));
       m[1] = m[2] = m[3] = 0;

       m[4] = 0;
       m[5] = float(2.0*f32ZNear/(f32Top-f32Bottom));
       m[6] = m[7] = 0;

       m[8] = (f32Right + f32Left) / (f32Right - f32Left);
       m[9] = (f32Top + f32Bottom) / (f32Top - f32Bottom);
       m[10] = -( (f32ZNear + f32ZFar) / (f32ZFar - f32ZNear)  );
       m[11] = -1;
       
       m[12] = m[13] =0;
       m[14] = -( (2*f32ZNear*f32ZFar) / (f32ZFar-f32ZNear));
       m[15] = 0;
	   return true;
}


//like gluperspective matrix		 
bool matPerspective(float* m ,float fieldOfViewDegree, float aspectRatio, float zNear, float zFar)
{
   if(fieldOfViewDegree <= 0.0f || fieldOfViewDegree >=180.0f)
      //fieldOfViewDegree = 45.0f;   //assign FOV to 45 deg if value passed is not in proper range 
	   return false;

   float FOVrad = float(PI_OVER_180*fieldOfViewDegree*0.5f);//angle divided by 2 !!!

   float f32top = float( zNear*tan(FOVrad) );
   float f32Right = aspectRatio*f32top;
   return matFrustum(m,-f32Right,f32Right,-f32top,f32top,zNear,zFar);

}  

void matMult(float* m3, const float* m1, const float* m2) 
{
        
       m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
       m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
       m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
       m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
       
       m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
       m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
       m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
       m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
       
       m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
       m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
       m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
       m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
       
       m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
       m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
       m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
       m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

}

//rotate about arbit axis
bool matRotate(float* m, float DEGAngle, float x, float y, float z)
{
	    
	   float Sq = (float) sqrt(x*x + y*y + z*z);
	   float inv;
	   if(Sq > -FLT_EPSILON && Sq < FLT_EPSILON) //chk for divide by zero......
		   return false;
	   inv = 1.0f/Sq;	
	   x = x * inv;   
	   y = y * inv;
	   z = z * inv;
	   float radian = (float)PI_OVER_180 * DEGAngle;
       float f32c = (float)cos(radian); //TODO!! Optimize me
       float f32s = (float)sin(radian);
       float f32OneMinC = 1 - f32c;
		
		float RotMat[16];
       RotMat[0] = f32c + f32OneMinC * x * x;
       RotMat[1] = (f32OneMinC * x * y) + (z * f32s);
       RotMat[2] = (f32OneMinC * x * z) - (y * f32s);
	   RotMat[3] = 0.0;
       RotMat[4] = (f32OneMinC * x * y) - (z * f32s);
       RotMat[5] = f32c + f32OneMinC * y * y;
       RotMat[6] = (f32OneMinC * y * z) + (x * f32s);
	   RotMat[7] = 0.0;
       RotMat[8] = (f32OneMinC * x * z) + (y * f32s);
       RotMat[9] = (f32OneMinC * y * z) - (x * f32s);
       RotMat[10] = f32c + f32OneMinC * z * z;
	   RotMat[11] = RotMat[12] =RotMat[13] = RotMat[14] = 0.0;RotMat[15] =1.0f;
	   float t[16];
	   matMult( &t[0] ,  m , &RotMat[0]);
	   for(int i = 0; i < 16; ++i)
		m[i] = t[i];
		
	  return true;
}

void matTranslate(float* m, const float x, const float y, const float z  )
{
       m[12] = m[0] * x + m[4] *y + m[8]* z + m[12];
       m[13] = m[1] * x + m[5] *y + m[9]* z + m[13];
       m[14] = m[2] * x + m[6] *y + m[10]* z + m[14];
	   m[15] = m[3] * x + m[7] *y + m[11]* z + m[15];
}  


STDMETHODIMP CGfxAndroidOGLES_SHADER::UIFlip(LPVOID pHandle)
{
//	DP("shader: UIFlip\n");

	if(frames == 0)
	{
	  DP("setupGraphics %f,%f \n",gw,gh);
		bool ret = setupGraphics(gw,gh);
		if(ret != true)	return E_FAIL;
	}

	frames ++;

	//vw = m_dwWidth;
	//vh = m_dwHeight;

	vw = gw;
	vh = gh;
	
	// the front-side rectangle
	static GLfloat vVerticesRect[] = {
		-vw,  vh, 0,
		-vw, -vh, 0,
		vw, -vh, 0,
		vw, vh, 0,
	};



	static float rot = 0;
	//rot += 5;

	float aspect = gw/gh;

	float mat_p[16];
	float mat_t[16];
	float mvp[16];

	matIdentity(mat_p);
	matIdentity(mat_t);
	matIdentity(mvp);

	matPerspective(mat_p, 90,  aspect, 1, 1001);

  if(gw == 800 && gh == 480)
	matTranslate(mat_t, 0, 0, -480.0f);
  else
	matTranslate(mat_t, 0, 0, -800.0f);

	matRotate(mat_t, 180, 1.0f, 0.0f, 0.0f);
	matMult((float*)mvp, (float*)mat_p, (float*)mat_t);

	glClearColor(0,1,0,1);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT/*|GL_DEPTH_BUFFER_BIT*/);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	static const GLfloat cube_texcoord[] =
	{
		0.0, 1.0,
		0.0, 0.0, 
		1.0, 0.0,
		1.0, 1.0,
	};

	glUseProgram(g_shaderProgramMvp);
	int loc2 = glGetUniformLocation(g_shaderProgramMvp, "mvp");

	// set the transformation
	glUniformMatrix4fv(loc2, 1, GL_FALSE, (float*)mvp);

	// draw the rotating object
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVerticesRect);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, cube_texcoord);
	glBindTexture(GL_TEXTURE_2D, texture);

EnterCriticalSection(&m_csDisplay);

	glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, m_dwWidth, m_dwHeight, 
		GL_RGB, GL_UNSIGNED_BYTE, (const void*)m_pBuff); 
/*
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,m_dwWidth, m_dwHeight, 
		0, GL_RGB, GL_UNSIGNED_BYTE, 0); 
	glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, m_dwWidth, m_dwHeight, 
		GL_RGB, GL_UNSIGNED_BYTE, (const void*)m_pBuff); 
		*/
LeaveCriticalSection(&m_csDisplay);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//	glFlush();
	
	//DP("shader: UIFlip Finish\n");
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::Show(BOOL bShow)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetDstRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetScnRect(const RECT *pRect)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetScnRect(RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidOGLES_SHADER::SetWindowHandle(HWND hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetWindowHandle(HWND& hwnd)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = 0;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0;
	return S_OK;

}
STDMETHODIMP CGfxAndroidOGLES_SHADER::IsOpen()
{
	return S_OK;
}
STDMETHODIMP CGfxAndroidOGLES_SHADER::Update()
{
	return S_OK;
}

GLuint CGfxAndroidOGLES_SHADER::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

 void CGfxAndroidOGLES_SHADER::_loadTextures(GLubyte* image, int tex_name)
{
	glBindTexture(GL_TEXTURE_2D, tex_name);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_dwWidth, m_dwHeight, 
		0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)image); 
}

 void CGfxAndroidOGLES_SHADER::loadTextures()
{
	glGenTextures(1, &texture);
	_loadTextures(m_pBuff ,texture);
}

int CGfxAndroidOGLES_SHADER::InitGL(GLuint &prog)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	m_pBuff = (unsigned char*)malloc(m_dwWidth*m_dwHeight*3);
	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, VertexVertexText);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, FragmentFragmentText);
	// Create the program object
	programObject = glCreateProgram();
	if(programObject == 0)
		return 0;
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	// Bind vPosition to attribute 0   
	glBindAttribLocation(programObject, 0, "vertexPos");
	//glBindAttribLocation(programObject, 1, "mycolor");
	glBindAttribLocation(programObject, 1, "texCoord");
	// Link the program
	glLinkProgram(programObject);
	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if(!linked) 
	{
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if(infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			// esLogMessage("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}
		glDeleteProgram(programObject);
		__android_log_print(ANDROID_LOG_ERROR, "SanAngeles", "LINK FAILED");
		return false;
	}
	__android_log_print(ANDROID_LOG_ERROR, "SanAngeles", "LINK SUCCESS");

	// Store the program object
	prog = programObject;
	vs = vertexShader;
	fs = fragmentShader;

	glClearColor(0.0f, 0.0f, 100.0f, 1.0f);

	// texture:
	glEnable(GL_TEXTURE_2D); 
	loadTextures();

	return true;
}

bool CGfxAndroidOGLES_SHADER::setupGraphics(int w, int h) {
	int ret = 0;
	DP("shader: setupGraphics \n");
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

     glViewport(0, 0, w, h);
    checkGlError("glViewport");

	gw = w;
	gh = h;
	DP("shader: start initGL\n");
	ret = InitGL(g_shaderProgramMvp);
	DP("shader: end initGL:%d\n",ret);
	
    return ret;
}
IMcGfxOverlay *CreateOverlayAndroidOGL_ES_SHADER()
{
	return new CGfxAndroidOGLES_SHADER();
}
#endif
