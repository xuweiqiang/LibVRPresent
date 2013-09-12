// GfxD3D_IDTPixelFormat.h: interface for the GfxD3D_IDTPixelFormat class.

// Code by Karel Donk
// Contact me at karel@miraesoft.com for more information, or visit my homepage at
// http://www.miraesoft.com/karel/ from more codesamples.
// May be modified and used freely

#pragma once

// Function return values
#define DT_OK						1 // Function was successful
#define DT_ERROR					0 // Function was not successful
#define DT_NOT_INITIALIZED			2 // Object has not yet been initialized
#define DT_COULD_NOT_OPEN_FILE		3 // File could not be found or opened
#define DT_FILE_READ_ERROR			4 // Could not read from file
#define DT_FILE_WRITE_ERROR		5 // Could not write to file
#define DT_COMPRESSION_ERROR		6 // Compression error
#define DT_DECOMPRESSION_ERROR		7 // Decompresison error
#define DT_INVALID_FORMAT			8 // Invalid data format

#include <d3d9.h>
#include <D3d9types.h>
//#include <D3dx9math.h>


//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


// DT color information
typedef struct _DT_COLOR {
	int Red;
	int Green;
	int Blue;
	int Alpha;
} DT_COLOR;

// Surface pixelformat information structure
typedef struct _DT_PIXELFMT {
	D3DFORMAT Format;
	DWORD RShift;
	DWORD GShift;
	DWORD BShift;
	DWORD AShift;
	DWORD ABMask;
	DWORD RBMask;
	DWORD GBMask;
	DWORD BBMask;

	// Maximum color component values for the pixelformat
	// Some surfaces have a max of 255, others 63, others 15 etc.
	DWORD AMaxVal;
	DWORD RMaxVal;
	DWORD GMaxVal;
	DWORD BMaxVal;
	int BitsPerPixel;
} DT_PIXELFMT;

class GfxD3D_IDTPixelFormat  
{
public:
	DWORD GetAMaxVal(D3DFORMAT format);
	DWORD GetRMaxVal(D3DFORMAT format);
	DWORD GetGMaxVal(D3DFORMAT format);
	DWORD GetBMaxVal(D3DFORMAT format);
	DWORD GetShiftValue(DWORD dmask);
	DWORD GetABitMask(D3DFORMAT format);
	DWORD GetRBitMask(D3DFORMAT format);
	DWORD GetGBitMask(D3DFORMAT format);
	DWORD GetBBitMask(D3DFORMAT format);
	HRESULT GetDTPixelFormat(D3DFORMAT format, DT_PIXELFMT * dtpixelfmt);
	DWORD GetBitDept(D3DFORMAT format);
	DWORD GetRBitLoc(D3DFORMAT format);
	DWORD GetGBitLoc(D3DFORMAT format);
	DWORD GetBBitLoc(D3DFORMAT format);
	DWORD GetABitLoc(D3DFORMAT format);
	GfxD3D_IDTPixelFormat();
	virtual ~GfxD3D_IDTPixelFormat();

};
