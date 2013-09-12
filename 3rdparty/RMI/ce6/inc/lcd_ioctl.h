/*****************************************************************************
Copyright 2003-2009 RMI Corporation. All rights reserved.

Any transfer or redistribution of the source code, with or without modification,
IS PROHIBITED, unless prior written consent was obtained. Any transfer or
redistribution of the binary code for use on the RMI Alchemy Family,
with or without modification, is permitted, provided that the following
condition is met:

Redistributions in binary form must reproduce the above copyright notice,

this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution:

THIS SOFTWARE IS PROVIDED BY RMI Corporation 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RMI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef __LCD_IOCTL_H__
#define __LCD_IOCTL_H__


/*
 *  Defines the # of available planes/overlay/window and assigns
 *  a plane to each major component.
 *   ** These are recommended values only.
 *   ** Display.cpp uses DESKTOP and OVERLAY(ddraw)
 *   ** 3d egl/platform_win32.c uses OGL
 *   ** MAE Backend(ITE) will use MAE_PLANE
 */
#define NUM_PLANES		4

#define DESKTOP_PLANE	0
#define MAE_PLANE		1
#define OGL_PLANE       2
#define OVERLAY_PLANE	3


// IOCTL flags
#define LCD_UPDATE_IN_VBLANK 1


#define IOCTLCode(f,m) \
  CTL_CODE ( FILE_DEVICE_UNKNOWN, (f), (m), FILE_ANY_ACCESS )

#define SIZE_RAM_ARRAY 256

/*
 *	Data definitions and IOCTLs for new OVERLAY functionality within LCD driver
 */
typedef struct {
	ULONG ndx;
	ULONG winctrl0;						// X,Y Alpha
	ULONG winctrl1;						// Priority, pipe, format, CCO, Pixel ordering, size
	ULONG winctrl2;					    // Colorkey mode, Double Buffer, RAM, Linewidth, Scale
	ULONG bufctrl;
	ULONG flags;
	ULONG reserved0;
	ULONG reserved1;
} OVERLAY_IOCTL,*POVERLAY_IOCTL;
#define OVERLAY_CONFIG_GET				(1<<0)
// Allows callers to override registry settings in display.reg
#define OVERLAY_OVERRIDE_REGISTRY		(1<<1)


typedef struct {
	ULONG ndx;
	ULONG flags;
	VOID * phys;
} OVERLAY_UPDATE_IOCTL,*POVERLAY_UPDATE_IOCTL;
#define OVERLAY_UPDATE_VBLANKWAIT		1
#define OVERLAY_UPDATE_OVERRIDE			2

typedef struct {
	ULONG colorkey;
	ULONG colorkeymsk;
} LCD_COLORKEY_IOCTL,*PLCD_COLORKEY_IOCTL;

typedef struct {
	ULONG colorkey;
	ULONG colorkeymsk;
} LCD_GAMMA_IOCTL,*PLCD_GAMMA_IOCTL;

#define	OVERLAY_MAGIC(n)				(n|10000)
#define LCD_SCREEN_GET					IOCTLCode(OVERLAY_MAGIC(0), METHOD_BUFFERED)
#define LCD_OVERLAY_ENABLE 				IOCTLCode(OVERLAY_MAGIC(1), METHOD_BUFFERED)
#define LCD_OVERLAY_DISABLE   			IOCTLCode(OVERLAY_MAGIC(2), METHOD_BUFFERED)
#define LCD_OVERLAY_CREATE 				IOCTLCode(OVERLAY_MAGIC(3), METHOD_BUFFERED)
#define LCD_OVERLAY_DESTROY				IOCTLCode(OVERLAY_MAGIC(4), METHOD_BUFFERED)
#define LCD_OVERLAY_UPDATE				IOCTLCode(OVERLAY_MAGIC(5), METHOD_BUFFERED)
#define LCD_COLORKEY_SET				IOCTLCode(OVERLAY_MAGIC(6), METHOD_BUFFERED)
#define LCD_COLORKEY_GET				IOCTLCode(OVERLAY_MAGIC(7), METHOD_BUFFERED)
#define LCD_BACKGROUND_SET				IOCTLCode(OVERLAY_MAGIC(8), METHOD_BUFFERED)
#define LCD_BACKGROUND_GET				IOCTLCode(OVERLAY_MAGIC(9), METHOD_BUFFERED)
#define LCD_GAMMA_SET					IOCTLCode(OVERLAY_MAGIC(10), METHOD_BUFFERED)
#define LCD_GAMMA_GET					IOCTLCode(OVERLAY_MAGIC(11), METHOD_BUFFERED)
#define LCD_VBLANK_WAIT					IOCTLCode(OVERLAY_MAGIC(12), METHOD_BUFFERED)
#define LCD_OVERLAY_CONFIG 				IOCTLCode(OVERLAY_MAGIC(13), METHOD_BUFFERED)
#define LCD_OVERLAY_SET_NEXT_BUFFER		IOCTLCode(OVERLAY_MAGIC(14), METHOD_BUFFERED)



/*
 *	Data definitions and IOCTLs for PIXEL functionality within LCD driver
 */
typedef struct {
	ULONG ndx;
	ULONG x,y;							// X,Y
	ULONG w,h;							// width, height
	ULONG flags;
	ULONG reserved0;
} PIXEL_IOCTL,*PPIXEL_IOCTL;

#define	PIXEL_MAGIC(n)					(n|20000)
#define PIXEL_READ						IOCTLCode(PIXEL_MAGIC(0), METHOD_BUFFERED)
#define PIXEL_WRITE						IOCTLCode(PIXEL_MAGIC(1), METHOD_BUFFERED)


#define lcdReadPixels( OVLY, X, Y, W, H, DST ) {   \
		PIXEL_IOCTL _pix;	 		\
		HDC _hLCD = GetDC(NULL);	\
		_pix.ndx = OVLY;			\
		_pix.x = X;					\
		_pix.y = Y;					\
		_pix.w = W;					\
		_pix.h = H;					\
		ExtEscape(_hLCD, PIXEL_READ, sizeof(PIXEL_IOCTL), (LPCSTR)&_pix, W*H*4, (ULONG*)DST); \
	}

#endif // __LCD_IOCTL_H__