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
#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/GfxPresentPrepare.h"
#include "../../inc/GfxVideoWindow.h"

#include "../../src_v2/OpenGL/gltools.h"   // GLTools
#include "../../src_v2/OpenGL/math3d.h"    // 3D Math Library
#include "../../src_v2/OpenGL/glframe.h"   // Frame Class

//#include "../../inc/VideoMixer.h"
#include "../OpenGL/TextureManager.h"


class CVRPresenterStreamOpenGL : 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
enum
{
	eTOP_FIELD,
	eBOTTOM_FIELD
};
public:
	CVRPresenterStreamOpenGL( VP_OPEN_STREAM_PARAMS *pOpenParams);
	virtual ~CVRPresenterStreamOpenGL ();

	//IUNKNOWN
	DECLARE_IUNKNOWN

	STDMETHODIMP Open();
	STDMETHODIMP Close();
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHODIMP LockSurface(LPBYTE *ppData, DWORD *pdwStride);
	STDMETHODIMP UnlockSurface();
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP Update(GLfloat  x, GLfloat y, GLfloat z,GLfloat  sizeX, GLfloat  sizeY);
	STDMETHODIMP StartAnimation(VR_ANIMATION_PARAMS* pParms);
	STDMETHODIMP StopAnimation();

private:
	STDMETHODIMP Flip();
	STDMETHODIMP Preparebuffer(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
	STDMETHODIMP DispMsg(char* pStr);
	int GetAlign(int input);

	VP_OPEN_STREAM_PARAMS m_open_param;
	bool m_bEnable;
	RECT m_rectSrc;
	RECT m_rectDst;
	DWORD m_Alpha;
	VP_ZOOM_MODE m_zoom_mode;
	GfxPrepareDataV2 m_PrepareData;
	VP_SCAN_MODE m_scanMode;
	bool m_bOpen;

	GLuint	mTextureId;
	LPBYTE	m_pbuff;
	LPBYTE	m_pTopBuff;
	LPBYTE	m_pBottomBuff;
	LONG m_alignedWidth;
	LONG m_alignedHeight;
	float m_zOrder;
	RECT m_rProject;
	//VP_CANVASINFO_PARAMS m_mapParam;
	VP_CANVASINFO_PARAMS m_canvasParam;

	IMcVideoWindow * m_pVideoWindow;
	VP_RECT m_rMouse;
	fpVPCallback m_pCallback;
	bool m_mouseProcessed;

	ICivmVideoContext* m_pVideoContext;

	CTextureManager	m_textureMgr;
	int m_textureId;

	bool m_needUpdate;
	int m_callList;
	bool m_bInitCallList;
	int m_field;
	VR_ANIMATION_PARAMS m_animation_parms;
	VR_ANIMATION_ROTATE_PARAMS m_ani_rotaion_params;
	float m_ani_rotaion_params_tmp;
};


