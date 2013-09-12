// ***************************************************************
//  RendererSDK   version:  1.2    date: 09/05/2007
//  -------------------------------------------------------------
//  Copyright (C) 2007 SiRF, Inc.  All Rights Reserved
// ***************************************************************
//	Basic types and Interfaces
// ***************************************************************
#ifndef RENDER_SDK_H_
#define RENDER_SDK_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef PVOID		VR_HANDLE;
	typedef HWND		VR_HWND;

	typedef enum 
	{
		TITAN_HW_RENDERER,//default is TITAN_HW_RENDERER		
		TITAN_DSP_RENDERER,
		TITAN_USER_MANAGE_BUFFER,
		TITAN_BITMAP_ORDER
	} VR_CONTROL_TYPE;	//use to CVR_EscapeCtrl 

	typedef struct
	{
		unsigned char **	ppY;
		unsigned char **	ppU;
		unsigned char **	ppV;
	} VR_USER_DEF_YUV, PVR_USER_DEF_YUV;   //Use this struct to control yuv pointer

	typedef struct
	{
		int			left;
		int			top;
		int			right; 
		int			bottom;
	} VR_SRCRECT, VR_RECT;
	
	typedef struct
	{
		float		left;		//0.0-1.0, 1.0 represent the maximum value
		float		top;		//0.0-1.0, 1.0 represent the maximum value
		float		right;		//0.0-1.0, 1.0 represent the maximum value
		float		bottom;		//0.0-1.0, 1.0 represent the maximum value

	} VR_DSTRECT;		//Destination rect
	
	typedef struct
	{
		long			struct_siz;
		unsigned char*	pFrameBuf;		//Virtual address
		unsigned char*	pPhyFrameBuf;	//Physical address 
		int				format_id;		//Now only support YUV420
		int				width;						
		int				height;					
	
		union
		{
			unsigned long	RBitMask;
			unsigned long	YBitMask;	
		};
		union
		{
			unsigned long	GBitMask;
			unsigned long	UBitMask;	
		};
		union
		{
			unsigned long	BBitMask;
			unsigned long	VBitMask;	
		};
		union
		{
			int		hoz_stride;
			int		Y_stride;		//For YV12 only
		};

		int			UV_stride;
		long		reserved[8];
	} VR_FRAME_SURFACE, *PVR_FRAME_SURFACE, **PPVR_FRAME_SURFACE;		//Descriptor of surface

	typedef enum 
	{
		VR_SCR_NORMAL,		//No rotation
		VR_SCR_ROT_CK90,	//Clockwise rotation 90degree
		VR_SCR_ROT_CCK90,   //Counter clockwise rotation 90degree
		VR_SCR_MIRROR,		//Display the video in mirrored mode
		VR_SCR_FLIP			//Rotate 180degree
	} VR_SCR_MODE;		//Rotation mode, Now only can rotate with DSP renderer

	typedef enum 
	{
		VR_DISP_NORMAL,		//Preserve the aspect ratio of original video, also known as Letterbox mode
		VR_DISP_FULL,		//Scale the video to full window regardless the original aspect ratio
		VR_DISP_JUST		//Now not support. Almost the same as full mode, but the scaling ratio is less on center region of window than the left/right borders, that is, nonlinearly scaled horizontally to fill the screen, it's also known as Panorama mode.
	} VR_DISP_MODE;		//Display mode
	
	typedef struct
	{
		VR_SCR_MODE		smode;		//Rotation mode
		VR_DISP_MODE	dmode;		//Display mode
	} VR_DISP_MODE_DESC;	//Display mode

	typedef struct
	{
        int			iRegNum;			//Specifies the region index for which you want to set information.
		VR_RECT		dest;				//The destination rectangle for OSD
        float		alpha;				//Specifies the alpha blending value; must be a value from 0.0 to 1.0 (inclusive).
        long		src_color_key;		//Specifies the source color key(XRGB format, little-endian)
        int			src_width;			//Specifies the width of source bitmap 
        int			src_height;			//Specifies the height of source bitmap
		int			bImgChanged;		// 1 to indicate the content of image to which pBitmap pointered is changed, 0 otherwise
		unsigned char* pBitmap;			// Pointer to the raw bitmap data, the data must be in 32bit XRGB little-endian format, that is, B1G1R1XB2G2R2X¡­ in byte order.
	} VR_OSD_INFO, *PVR_OSD_INFO;		//OSD information

	typedef enum  
	{
			VR_OK,
			VR_ERR_UNKNOWN,
			VR_ERR_INVALID_PARAM,
			VR_ERR_DEVICE_FAILURE,
			VR_ERR_OUT_OF_MEM,
			VR_ERR_NOT_IMPL,
			VR_ERR_INVALID_DST_RECT,
			VR_ERR_INVALID_FORMAT,
			VR_ERR_SURFACE_LOCKED
	} VR_RESULT;		//Return code
	


//This function is called to create video render handler.
//Return: VR_OK if successful otherwise failure
VR_RESULT	CVR_CreateVideoRender(
								  VR_HANDLE *pHandle,	//[out] Pointer to a variable which will hold the handle.
								  VR_HWND hWnd			//[in] The window handle of video playback. It MUST be the top level window of the playback application.
								  );

//This function is delete video render handler.
//Return: VR_OK if successful else failed.
VR_RESULT	CVR_DeleteVideoRender(
								  VR_HANDLE handle		//[in] The handle of video renderer.
								  );

//This function is called to set source video info.
//Remark: Call this function before surface creation, that is, surface creation routine depends on this function to provide information regarding the resolution 
VR_RESULT	CVR_SetVideoFormat(
							   VR_HANDLE handle,		//[in] The handle of video render.
							   int format_id,			//[in] Predefined format id. Now this param is ignored(we support YUV420 only), you can pass only value. 
							   int width,				//[in] Source video width in pixels. With DSP(HW) render, it must be a multiple of 4(2).
							   int height,				//[in] Source video height. With DSP render, it must be a multiple of 4.
							   VR_SRCRECT* pSrcRect,	//[in/out] Pointer to the source rectangle structure, If it's NULL, the source rectangle covers the full source image. If the specified rectangle is not acceptable for boundary alignment restriction, the aligned value will be returned in this parameter. If it's invalid the function will return VR_ERR_INVALID_PARAM
							   int* LumPitch,			//[in/out] Pointer to a variable to hold Y pitch. If the platform can not support the proposed Y pitch, the acceptable Y pitch will be returned by this parameter. Normal value is the same as width. Only valid for YV12 format, for planar format, it's simply the horizontal source pitch.
							   int* ChromPitch			//[in/out] U/V pitch. Normal value is the half of width. Only valid for YV12 format
							   );

//Set display mode for the target screen.
//Remark: If this function is not been called, the default mode is VR_SCR_NORMAL.
VR_RESULT CVR_SetDisplayMode(
							 VR_HANDLE handle,			//[in] The handle of video render.
							 VR_DISP_MODE_DESC mode		//[in] Screen display mode.
							 );

//This function is called to set destination video rectangle.
//Remark: The default value is to cover the full screen region.
VR_RESULT CVR_SetDstRect(
						 VR_HANDLE handle,				//[in] The handle of video renderer.
						 VR_DSTRECT  rect				//[in] Specifies destination rectangle.
						 );

//Render a frame of video.
//Return: VR_OK if successful, otherwise failure.
VR_RESULT	CVR_Render(
					   VR_HANDLE handle,				//[in] The handle of video renderer
					   VR_FRAME_SURFACE* pSurf,			//[in] Pointer to surface to be rendered to screen.
					   bool bWait						//[in] Wait until render complete or not.
					   );

//This function is called to wait until render video finished.
//Return: VR_OK if successful else failed.
//Remark: Only necessary if you set bWait equals TRUE in CVR_Render call
VR_RESULT	CVR_WaitComplete(
							 VR_HANDLE handle			//[in] The handle of video renderer
							 );

//Create a surface(frame buffer) for rendering.
//Return: VR_OK if successful else failed.
//Remark: Information will be filled in surface structure once the operation succeed, do NOT change its contents.
VR_RESULT CVR_CreateSurface(
							VR_HANDLE handle,			//[in] The handle of video renderer
							VR_FRAME_SURFACE** ppSurf,	//[in/out] Pointer to a variable to hold the pointer of created surface
							int bPhysical				//[in] 1 if physical continuous memory is required for surface frame buffer.
							);

//Destroy a surface created by CVR_CreateSurface.
//Return: VR_OK if successful else failed.
VR_RESULT CVR_DestroySurface (
							  VR_HANDLE handle,			//[in] The handle of video renderer
							  VR_FRAME_SURFACE* pSurf	//[out] Pointer to the surface to destroy
							  );

//Lock the surface, then caller can access the internal frame buffer directly.
//Return: VR_OK if successful else failed.
//Remark: Surface shall be unlocked before rendering
VR_RESULT CVR_LockSurface(
						  VR_HANDLE handle,				//[in] The handle of video renderer
						  VR_FRAME_SURFACE* pSurf		//[out] Pointer to the surface you want to lock
						  );

//Unlock the specified surface.
//Return: VR_OK if successful else failed.
//Remark: Unlock should always pair with CVR_LockSurface.
VR_RESULT CVR_UnlockSurface(
							VR_HANDLE handle,			//[in] The handle of video renderer
							VR_FRAME_SURFACE* pSurf		//[out] Pointer to the surface you want to unlock
							);

//DO NOT USE this function
VR_RESULT CVR_SetOSDRegionNum(
							  VR_HANDLE handle,		
							  int num			
							  );

//DO NOT USE this function
VR_RESULT CVR_SetOSDInfo(
						 VR_HANDLE handle,		
						 int region_num,	
						 PVR_OSD_INFO inf	
						 );


// Return: VR_OK if successful else failed.
// Remark: On Titan, we can control the underlying scaling method being used and the coefficient for the scaler through this function.
VR_RESULT CVR_EscapeCtrl(
						 VR_HANDLE handle,				//[in] The handle of video renderer
						 long ctrl_id,					//[in] The Renderer you want to switch. See RENDERER_NAME
						 unsigned char* pInBuf,			//[in] Now just pass NULL. Pointer to a buffer to hold parameter for specified escape code.
						 int dat_siz,					//[in] Now just pass 0. Buffer size.
						 unsigned char* pOutBuf			//[out] Now just pass NULL. Pointer to a output buffer to hold returned data.
						 );

//Get the internal error code for the last operation, which is platform dependent and is provided as a manner to help the malfunction diagnosis. 
// Return: The error code.
// Remark: It will be DirectDraw or Direct3D Mobile error code on WinCE platform.
unsigned long CVR_GetLastError(
							   VR_HANDLE handle			//[in] The handle of video renderer
							   );
							
#ifdef __cplusplus
}
#endif



#endif // RENDER_SDK_H_



