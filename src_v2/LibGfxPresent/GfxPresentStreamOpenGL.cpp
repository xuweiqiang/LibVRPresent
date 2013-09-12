//===================================== ========================================
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
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include "../../inc/GfxBuildDef.h"

#ifdef VM_OPENGL

#include "GfxPresentStreamOpenGL.h"
#include "../../inc/dp.h"

//#define USE_CALLLIST
#define OGL_STREAM_DEFAULT_BPP 4
CVRPresenterStreamOpenGL::CVRPresenterStreamOpenGL(VP_OPEN_STREAM_PARAMS *pOpenParams):
m_bEnable(false),
m_Alpha(MAX_ALPHA_VAL),
m_scanMode(VP_SCAN_PROGRESSIVE),
m_zoom_mode(VP_ZOOM_MODE_STRETCH),
m_bOpen(false),
mTextureId(0),
m_pbuff(0),
m_pVideoWindow(NULL),
m_pCallback(NULL),
m_mouseProcessed(false),
m_pVideoContext(0)
{
	if(!pOpenParams || !pOpenParams ->dwWidth || !pOpenParams->dwHeight)		return;
	
	memset(&m_rProject, 0,sizeof(m_rProject));
	memset(&m_canvasParam, 0,sizeof(m_canvasParam));	
	memset(&m_rMouse, 0,sizeof(m_rMouse));	
	m_zOrder = -1.0f;	
	m_needUpdate = false;
	m_callList = 0;
	m_bInitCallList = false;
	m_field = eTOP_FIELD;
	m_pTopBuff = 0;
	m_pBottomBuff = 0;
	m_textureId = -1;
	memset(&m_animation_parms, 0,sizeof(m_animation_parms));
	memset(&m_ani_rotaion_params, 0,sizeof(m_ani_rotaion_params));
	m_ani_rotaion_params_tmp = 0.0f;


	memcpy(&m_open_param, pOpenParams, sizeof(m_open_param));
	SetRect(&m_rectSrc, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);
	SetRect(&m_rectDst, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);

	m_alignedWidth  = GetAlign(m_open_param.dwWidth);
	m_alignedHeight = GetAlign(m_open_param.dwHeight);
	if(!m_alignedWidth  || !m_alignedHeight )	return;


	m_pVideoWindow = CreateVideoWindow();
	if(!m_pVideoWindow)	return;
	
	return;
}

CVRPresenterStreamOpenGL::~CVRPresenterStreamOpenGL()
{
	Close();
	if(m_pVideoWindow)
	{
		m_pVideoWindow->Release();
		m_pVideoWindow  = NULL;
	}
	return;
}

STDMETHODIMP CVRPresenterStreamOpenGL::Open()
{
	HRESULT hr = S_OK;
	if(m_bOpen || !m_pVideoWindow || !m_open_param.dwWidth || !m_open_param.dwHeight)	return E_UNEXPECTED;

	if(m_open_param.dwBPP == 0)	m_open_param.dwBPP = OGL_STREAM_DEFAULT_BPP;
	if(m_open_param.dwBPP != 3 && m_open_param.dwBPP != 4  )	return E_INVALIDARG;

	m_bEnable = false;
	m_Alpha = MAX_ALPHA_VAL;
	m_scanMode = VP_SCAN_PROGRESSIVE;
	m_zoom_mode = VP_ZOOM_MODE_STRETCH;
	mTextureId = 0;
	m_pCallback = NULL;
	m_mouseProcessed = false;
	m_zOrder = -1.0f;	
	memset(&m_animation_parms, 0,sizeof(m_animation_parms));
	memset(&m_ani_rotaion_params, 0,sizeof(m_ani_rotaion_params));
	m_pVideoContext = 0;

	m_pbuff	= (LPBYTE)malloc(m_alignedWidth*m_alignedHeight*m_open_param.dwBPP);
	m_pTopBuff = (LPBYTE)malloc(m_alignedWidth*m_alignedHeight*m_open_param.dwBPP);
	m_pBottomBuff = (LPBYTE)malloc(m_alignedWidth*m_alignedHeight*m_open_param.dwBPP);
	if(!m_pbuff || !m_pTopBuff || !m_pBottomBuff )
		return E_UNEXPECTED;

	VE_Options tVEOpen;
	ZeroMemory(&tVEOpen, sizeof(tVEOpen));

	tVEOpen.mode = (VE_MODE)m_zoom_mode;
	tVEOpen.rSrcClip.right = tVEOpen.rDst.right = tVEOpen.rSrc.right = m_open_param.dwWidth;
	tVEOpen.rSrcClip.bottom = tVEOpen.rDst.bottom =  tVEOpen.rSrc.bottom = m_open_param.dwHeight;
	tVEOpen.aspect.dwAspectX = m_open_param.dwAspectX;
	tVEOpen.aspect.dwAspectY = m_open_param.dwAspectY;

	hr = m_pVideoWindow->Open(&tVEOpen,sizeof(tVEOpen));
	if(FAILED(hr))	return hr;

	m_bOpen = true;
	return S_OK;	
}

STDMETHODIMP CVRPresenterStreamOpenGL::Close()
{
	if(!m_bOpen)	return E_FAIL;

	if(m_pTopBuff)
	{
		free(m_pTopBuff);
		m_pTopBuff = NULL;
	}

	if(m_pBottomBuff)
	{
		free(m_pBottomBuff);
		m_pBottomBuff = NULL;
	}
	/*
	if(m_callList)
	{
		glDeleteLists(m_callList, 1);
		m_callList = 0;

	}
	if(mTextureId )
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}*/
	if(m_pbuff)
	{
		delete m_pbuff;
		m_pbuff = NULL;
	}

	if(m_pVideoWindow)
	{
		m_pVideoWindow->Close();
	}

	m_bOpen = false;
	return S_OK;
}
	
STDMETHODIMP CVRPresenterStreamOpenGL::Get(IN REFGUID guidPropSet, IN DWORD dwPropID,
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
{
	if(!m_bOpen)	return E_UNEXPECTED;

	if(pPropData==0)
		return E_POINTER;
	
	switch(dwPropID)
	{
		case MC_PROPID_VP_RECTSRC:
			{
				*reinterpret_cast<RECT *>(pPropData) = m_rectSrc;
				if(pcbReturned)
					*pcbReturned = sizeof(RECT);
				return S_OK;
			}
		case MC_PROPID_VP_RECTDST:
			{
				*reinterpret_cast<RECT *>(pPropData) = m_rectDst;
				if(pcbReturned)
					*pcbReturned = sizeof(m_rectDst);
				return S_OK;
			}
		case MC_PROPID_VP_ENABLE:
			{
				*reinterpret_cast<bool *>(pPropData) = m_bEnable;
				if(pcbReturned)
					*pcbReturned = sizeof(m_bEnable);
				return S_OK;
			}
		case MC_PROPID_VP_ALPHA:
			{
				*reinterpret_cast<DWORD *>(pPropData) = m_Alpha;
				if(pcbReturned)
					*pcbReturned = sizeof(DWORD);
				return S_OK;
			}
		case MC_PROPID_VP_SCAN_MODE:
			{
				*reinterpret_cast<VP_SCAN_MODE *>(pPropData) = m_scanMode;
				if(pcbReturned)
					*pcbReturned = sizeof(m_scanMode);
				return S_OK;
			}
		case MC_PROPID_VP_ZORDER_ID:
			{
				*reinterpret_cast<float*>(pPropData) = m_zOrder ;
				return S_OK;
			}
		case MC_PROPID_VP_CANVAS_INFO:
			{
				if(pPropData==0)
					return E_POINTER;
				*reinterpret_cast<VP_CANVASINFO_PARAMS*>(pPropData) = m_canvasParam;
				return S_OK;
			}
		default:
			break;
	}
	return E_NOTIMPL;
}

STDMETHODIMP CVRPresenterStreamOpenGL::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, 
	IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
{
	HRESULT hr = S_OK;
	if(!m_bOpen && (dwPropID != MC_PROPID_VP_CANVAS_INFO && 
		dwPropID != MC_PROPID_VP_ZORDER_ID ))	return E_UNEXPECTED;
	
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTSRC:
		{
			m_rectSrc = *reinterpret_cast<RECT *>(pPropData);
		}
		break;
	case MC_PROPID_VP_RECTDST:
		{
			m_rectDst = *reinterpret_cast<RECT *>(pPropData);
		}
		break;
	case MC_PROPID_VP_ENABLE:
		{
			m_bEnable = *(reinterpret_cast<bool*>(pPropData));
		}
		break;
	case MC_PROPID_VP_ALPHA:
		{
			m_Alpha = *(reinterpret_cast<DWORD*>(pPropData)); 
		}
		break;
	case MC_PROPID_VP_DEINTERLACE_MODE:
	case MC_PROPID_VP_SCAN_MODE:
		{
			m_scanMode= *(reinterpret_cast<VP_SCAN_MODE*>(pPropData));
		}
		break;
	case MC_PROPID_VP_ZOOM:
		{
			m_zoom_mode = *reinterpret_cast<VP_ZOOM_MODE*>(pPropData);			
			hr = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,&m_zoom_mode , sizeof(m_zoom_mode ));
		}
		break;
	case MC_PROPID_VP_ZORDER_ID:
		{
			m_zOrder = *reinterpret_cast<float*>(pPropData);
			if(m_zOrder < -100.0f)
				m_zOrder = -100.0f;
			if(m_zOrder > 0.0f)
				m_zOrder = 0.0f;
		}
		break;
	case MC_PROPID_VP_CANVAS_INFO:
		{
			m_canvasParam = *reinterpret_cast<VP_CANVASINFO_PARAMS*>(pPropData);
		}
		break;
	case MC_PROPID_VP_MOUSE_CLICK:
		{
			m_rMouse =*reinterpret_cast<VP_RECT*>(pPropData); 
			m_mouseProcessed = true;
		}
		break;
	case MC_PROPID_VP_CALLBACK:
		{
			m_pCallback =*reinterpret_cast<fpVPCallback*>(pPropData); 
		}
		break;
	case MC_PROPID_VP_VM_CONTEXT:
		{
			m_pVideoContext = (ICivmVideoContext*)pPropData;
		}
		break;
	default:
		return E_FAIL;
	}
	return hr;
}

STDMETHODIMP CVRPresenterStreamOpenGL::LockSurface(LPBYTE *ppData, DWORD *pdwStride)
{
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamOpenGL::UnlockSurface()
{
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamOpenGL::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)	return S_OK;
	if(dwFlags & OGL_FLIP_FLAG)
		return Flip();
	else
		return Preparebuffer(ppbPlanes, pdwStrides,dwFlags);
}

STDMETHODIMP CVRPresenterStreamOpenGL::DispMsg(char* pStr)
{
#if 1
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);
	//glTranslatef(0.0f,0.0f,1.0f);
	glRasterPos2f(0.0f,0.0f);

	for(int i=0;pStr[i] != '\0';i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,pStr[i]);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
#endif
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamOpenGL::Update(GLfloat x, GLfloat y, GLfloat z, GLfloat  sizeX, GLfloat  sizeY)
{
	static DWORD startDisp = GetTickCount() + 5000;
	static char buf[256];

	if(m_mouseProcessed)
	{
		glPushMatrix();

		double		m_modelMatrix[16];
		double		m_projMatrix[16];
		int			m_viewport[4];
		double		m_winx;
		double		m_winy;
		double		m_winz; 

		glGetIntegerv(GL_VIEWPORT, m_viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, m_projMatrix);
		gluProject(x,y,z,m_modelMatrix, 
			m_projMatrix,m_viewport,
			&m_winx,&m_winy,&m_winz);

		m_rProject.left = (long)m_winx;
		m_rProject.bottom  = (long)(m_viewport[3] - m_winy);
		
		gluProject(x+1.0f*sizeX,y+1.0f*sizeY,z,m_modelMatrix, 
			m_projMatrix,m_viewport,
			&m_winx,&m_winy,&m_winz);

		m_rProject.right = (long)m_winx;
		m_rProject.top = (long)(m_viewport[3] - m_winy);
		
		DP("Mouse %d,%d world :(%d,%d,%d,%d) \n",m_rMouse.x, m_rMouse.y, m_rProject.left, m_rProject.right, m_rProject.top, m_rProject.bottom);
		if(m_rMouse.x>= m_rProject.left && m_rMouse.x<= m_rProject.right)				
			if(m_rMouse.y>= m_rProject.top && m_rMouse.y<= m_rProject.bottom)
			{
				if(m_pCallback)
				{
					VP_CALLBACK_MOUSECLICK_PARAM param;
					param.rDest = m_rProject;
					param.rMouse = m_rMouse;
					m_pCallback(MC_VP_CALLBACK_MOUSECLICK,(void*)&param);
				}
				if(m_pVideoContext)
				{
					HWND wnd;
					HRESULT hr= m_pVideoContext->Get(MC_PROPID_VM_HWND,&wnd,0 );
					if(SUCCEEDED(hr))
					{
						VM_MOUSECLICK_PARAMS param = {0};
						param.mouse_x = m_rMouse.x;
						param.mouse_y = m_rMouse.y;
						param.rDst = m_rProject;
						PostMessage(wnd, MC_PROPID_VM_MSG_MOUSECLICK,(WPARAM)&param,sizeof(param));
					}
				}
				startDisp = GetTickCount();
				sprintf(buf,"(%d,%d) in(%d, %d,%d,%d) is pressed",m_rMouse.x, m_rMouse.y,m_rProject.left, 
					m_rProject.top, m_rProject.right, m_rProject.bottom);
				DispMsg(buf);
			}
		glPopMatrix();

		m_mouseProcessed = false;
	}
	else
	{
		if(GetTickCount() - startDisp < 1000)
			DispMsg(buf);
	}

	return S_OK;
}

STDMETHODIMP CVRPresenterStreamOpenGL::Preparebuffer(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	DWORD srcWidth[3];
	DWORD srcHeight[3];
	srcWidth[0] = m_open_param.dwWidth;
	srcWidth[1] = srcWidth[2] = m_open_param.dwWidth>>1;

	srcHeight[0] = m_open_param.dwHeight;
	srcHeight[1] = srcHeight[2] = m_open_param.dwHeight>>1;

	RECT rSrc;
	SetRect(&rSrc, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);
	RECT rDst;
	SetRect(&rDst, 0,0, m_open_param.dwWidth, m_open_param.dwHeight);

	DWORD srcStrides[3] = {pdwStrides[0],pdwStrides[1],pdwStrides[2] };
	DWORD dstStride= m_open_param.dwWidth;

	if(m_scanMode != VP_SCAN_PROGRESSIVE)
	{
		rSrc.top /=2;
		rSrc.bottom/=2;
		srcStrides[0] *= 2;
		srcStrides[1] *= 2;
		srcStrides[2] *= 2;

		if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
		{
			if(m_open_param.dwBPP == 4)
			{
				m_PrepareData.PrepareBufferYV12ToARGB(m_pTopBuff, m_alignedWidth, ppbPlanes, srcStrides, &rSrc, &rDst, dwFlags, 0);

				ppbPlanes[0] += pdwStrides[0];
				ppbPlanes[1] += pdwStrides[1];
				ppbPlanes[2] += pdwStrides[2];
				m_PrepareData.PrepareBufferYV12ToARGB(m_pBottomBuff, m_alignedWidth, ppbPlanes, srcStrides, &rSrc, &rDst, dwFlags, 0);
			}
		}
		else if(m_open_param.dwFourCC == MAKE_FOURCC('A','R','G','B'))
		{
			if(m_open_param.dwBPP == 4)
			{
				for(unsigned int i=0;i< rSrc.bottom;i++)
				{
					memcpy(m_pTopBuff + dstStride*m_open_param.dwBPP*i, ppbPlanes[0] + srcStrides[0]*4*i,m_open_param.dwWidth*4);
				}
				
				ppbPlanes[0] += pdwStrides[0];
				for(unsigned int i=0;i< rSrc.bottom;i++)
				{
					memcpy(m_pBottomBuff + dstStride*m_open_param.dwBPP*i, ppbPlanes[0] + srcStrides[0]*4*i,m_open_param.dwWidth*4);
				}
			}

		}
	}
	else
	{
		if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
		{
			if(m_open_param.dwBPP == 4)
				m_PrepareData.PrepareBufferYV12ToARGB(m_pbuff, m_alignedWidth, ppbPlanes, srcStrides, &rSrc, &rDst, dwFlags, 0);
		}
		else if(m_open_param.dwFourCC == MAKE_FOURCC('A','R','G','B'))
		{
			if(m_open_param.dwBPP == 4)
			{
				for(unsigned int i=0;i< rSrc.bottom;i++)
				{
					memcpy(m_pbuff + dstStride*m_open_param.dwBPP*i, ppbPlanes[0] + srcStrides[0]*4*i,m_open_param.dwWidth*4);
				}

			}
		}

	}
	m_needUpdate = true;
	return S_OK;
}
STDMETHODIMP CVRPresenterStreamOpenGL::Flip()
{
	HRESULT hr = S_OK;


	if(!m_bEnable)
		return S_OK;

	if(!m_bOpen)	return E_FAIL;

#ifdef USE_CALLLIST
	if(!m_bInitCallList)
	{
		m_bInitCallList = true;
		m_callList = glGenLists(1);


		M3DVector3f vNormal;
		M3DVector3f vCorners[6] = { { 1.0f, 1.0f, 0.0f }, 
		{ 0.0f, 1.0, 0.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 1.0f, 0.0f, -1.0f }}; 


		glNewList(m_callList, GL_COMPILE); 
		glBegin(GL_QUADS);		
		m3dFindNormal(vNormal, vCorners[0], vCorners[1], vCorners[2]);		
		glNormal3fv(vNormal);


		if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
		{
			glTexCoord2f((GLfloat)m_open_param.dwWidth/m_alignedWidth, (GLfloat)m_open_param.dwHeight/m_alignedHeight);		
			glVertex3fv(vCorners[3]);		
			glTexCoord2f(0.0f, (GLfloat)m_open_param.dwHeight/m_alignedHeight);		
			glVertex3fv(vCorners[2]);		
			glTexCoord2f(0.0f, 0.0f);		
			glVertex3fv(vCorners[1]);		
			glTexCoord2f((GLfloat)m_open_param.dwWidth/m_alignedWidth, 0.0f);		
			glVertex3fv(vCorners[0]);		
		}
		else if(m_open_param.dwFourCC == MAKE_FOURCC('A','R','G','B'))
		{
			glTexCoord2f((GLfloat)m_open_param.dwWidth/m_alignedWidth, (GLfloat)m_open_param.dwHeight/m_alignedHeight);		
			glVertex3fv(vCorners[1]);		
			glTexCoord2f(0.0f, (GLfloat)m_open_param.dwHeight/m_alignedHeight);		
			glVertex3fv(vCorners[0]);		
			glTexCoord2f(0.0f, 0.0f);		
			glVertex3fv(vCorners[3]);		
			glTexCoord2f((GLfloat)m_open_param.dwWidth/m_alignedWidth, 0.0f);		
			glVertex3fv(vCorners[2]);		
		}
		glEnd();
		glEndList(); 

	}
#endif

	glEnable(GL_BLEND);
	//Use constant alpha value
	glBlendFunc(GL_CONSTANT_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
	glBlendColor(m_open_param.fAlpha,m_open_param.fAlpha,m_open_param.fAlpha,m_open_param.fAlpha);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	GLfloat posX=0;
	GLfloat posY=0; 
	GLfloat sizeX=0;
	GLfloat sizeY=0; 

	if(m_pVideoContext)
	{
		RECT r2D = {0,0,0,0};
		hr = m_pVideoContext->GetPos2D(&r2D);
		if(SUCCEEDED(hr))
			m_rectDst = r2D;
	}
	

////////////////////////
	if(m_animation_parms.type == eVR_ANIMATION_ROTATE)
	{
		glRotatef(m_ani_rotaion_params_tmp, m_ani_rotaion_params.xFactor,m_ani_rotaion_params.yFactor,m_ani_rotaion_params.zFactor);
		m_ani_rotaion_params_tmp += m_ani_rotaion_params.angle_step;
		if(m_ani_rotaion_params_tmp > m_ani_rotaion_params.stop_angle)
		{
			if(m_animation_parms.times == 1)
				StopAnimation();		
			else if(m_animation_parms.times != 0xff)
			{
				m_animation_parms.times--;
				m_ani_rotaion_params_tmp = m_ani_rotaion_params.start_angle;
			}
		}
	}

///////////////////////

	RECT rSrc;
	SetRect(&rSrc,0,0,m_open_param.dwWidth, m_open_param.dwHeight);
	if(m_pVideoWindow)
	{
		m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&m_rectDst, sizeof(m_rectDst));

		hr= m_pVideoWindow->Get(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,&rSrc, sizeof(rSrc),NULL);
		if(FAILED(hr))
			return hr;
		RECT rDst;
		hr = m_pVideoWindow->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&rDst, sizeof(rDst),NULL);
		if(FAILED(hr))
			return hr;

		posX = (GLfloat)(rDst.left + rDst.right)/2 ;
		posY = (GLfloat)(rDst.bottom + rDst.top)/2;
		posX = (GLfloat)(posX - m_canvasParam.dwWidth/2)/(GLfloat)(m_canvasParam.dwWidth) ;
		posY = (GLfloat)(m_canvasParam.dwHeight/2 - posY)/(GLfloat)(m_canvasParam.dwHeight);

		sizeX = (GLfloat)(rDst.right - rDst.left) /m_canvasParam.dwWidth;
		sizeY = (GLfloat)(rDst.bottom - rDst.top )/m_canvasParam.dwHeight;

	}

	glTranslatef(posX, posY, m_zOrder);
	glScalef(sizeX,sizeY,1.0f);
	
	Update(posX, posY, m_zOrder,sizeX ,sizeY);

	if(m_needUpdate)
	{
		if(m_scanMode == VP_SCAN_PROGRESSIVE)
		{
			m_needUpdate =false;
			m_textureId = m_textureMgr.LoadTextureFromMemory(m_pbuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
		}
		else if (m_scanMode == VP_SCAN_INTERLACED_TOPFIELED_FIRST)
		{
/*
			if(m_field == eTOP_FIELD)
			{
				m_textureId = m_textureMgr.LoadTextureFromMemory(m_pTopBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
				m_field = eBOTTOM_FIELD;
			}
			else if(m_field == eBOTTOM_FIELD)

			{
				m_textureId = m_textureMgr.LoadTextureFromMemory(m_pBottomBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
				m_field = eTOP_FIELD;
				m_needUpdate =false;
			}
*/
				m_textureId = m_textureMgr.LoadTextureFromMemory(m_pTopBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
				m_needUpdate =false;
		}
		else if (m_scanMode == VP_SCAN_INTERLACED_BOTTOMFIELED_FIRST)
		{
/*			if(m_field == eBOTTOM_FIELD)
			{
				m_textureId = m_textureMgr.LoadTextureFromMemory(m_pBottomBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
				m_field = eTOP_FIELD;
			}
			else if(m_field == eTOP_FIELD)
			{
				m_textureId = m_textureMgr.LoadTextureFromMemory(m_pTopBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
				m_field = eBOTTOM_FIELD;
				m_needUpdate =false;
			}
*/
			m_textureId = m_textureMgr.LoadTextureFromMemory(m_pBottomBuff, m_alignedWidth, m_alignedHeight,m_open_param.dwBPP,  m_textureId);
			m_needUpdate =false;
		}
	
	}

	glBindTexture (GL_TEXTURE_2D, m_textureId);

#ifdef USE_CALLLIST
	glCallList(m_callList);
#else	
	M3DVector3f vNormal = {0};
	M3DVector3f vCorners[6] = { { 0.5f, 0.5f, 0.0f }, 
							{ -0.5f, 0.5f, 0.0f },
							{ -0.5f, -0.5f, 0.0f },
							{ 0.5f, -0.5f, 0.0f }};   	


	glBegin(GL_QUADS);		
	m3dFindNormal(vNormal, vCorners[0], vCorners[1], vCorners[2]);		
	glNormal3fv(vNormal);

	GLfloat fX_offset = (GLfloat)rSrc.left/(GLfloat)m_alignedWidth;
	GLfloat fY_offset = (GLfloat)rSrc.top/(GLfloat)m_alignedHeight;
	
//	if(m_open_param.dwFourCC == MAKE_FOURCC('Y','V','1','2'))
	{
		glTexCoord2f((GLfloat)(rSrc.right - rSrc.left)/m_alignedWidth + fX_offset, 
			(GLfloat)(rSrc.bottom - rSrc.top)/m_alignedHeight+ fY_offset);		
		glVertex3fv(vCorners[3]);		
		glTexCoord2f(fX_offset, (GLfloat)(rSrc.bottom - rSrc.top)/m_alignedHeight+ fY_offset);		
		glVertex3fv(vCorners[2]);		
		glTexCoord2f(fX_offset, fY_offset);		
		glVertex3fv(vCorners[1]);		
		glTexCoord2f((GLfloat)(rSrc.right - rSrc.left)/m_alignedWidth + fX_offset, fY_offset);		
		glVertex3fv(vCorners[0]);		
	}
/*
	else if(m_open_param.dwFourCC == MAKE_FOURCC('A','R','G','B'))
	{
		glTexCoord2f((GLfloat)(rSrc.right - rSrc.left)/m_alignedWidth + fX_offset, 
			(GLfloat)(rSrc.bottom - rSrc.top)/m_alignedHeight + fY_offset);		
		glVertex3fv(vCorners[1]);		
		glTexCoord2f(fX_offset, (GLfloat)(rSrc.bottom - rSrc.top)/m_alignedHeight + fY_offset);		
		glVertex3fv(vCorners[0]);		
		glTexCoord2f(fX_offset, fY_offset);		
		glVertex3fv(vCorners[3]);		
		glTexCoord2f((GLfloat)(rSrc.right - rSrc.left)/m_alignedWidth + fX_offset, fY_offset);		
		glVertex3fv(vCorners[2]);		
	}
*/
	glEnd();
#endif

	return S_OK;	
}

int CVRPresenterStreamOpenGL::GetAlign(int input)
{
	int output = 0x1;
	while(input)
	{
		output <<= 1;
		input /=2;
	}
	return output;
}

STDMETHODIMP CVRPresenterStreamOpenGL::StartAnimation(VR_ANIMATION_PARAMS* pParms)
{
	if(!pParms)	return E_INVALIDARG;
	memcpy(&m_animation_parms, pParms, sizeof(VR_ANIMATION_PARAMS));

	memset(&m_ani_rotaion_params, 0,sizeof(m_ani_rotaion_params));

	if(m_animation_parms.type == eVR_ANIMATION_ROTATE)
	{
		if(!pParms->pParms)	return E_FAIL;
		memcpy(&m_ani_rotaion_params, pParms->pParms,sizeof(m_ani_rotaion_params));
		m_ani_rotaion_params_tmp = m_ani_rotaion_params.start_angle;
	}
	return S_OK;
}

STDMETHODIMP CVRPresenterStreamOpenGL::StopAnimation()
{
	memset(&m_animation_parms, 0,sizeof(m_animation_parms));
	return S_OK;
}

#endif
