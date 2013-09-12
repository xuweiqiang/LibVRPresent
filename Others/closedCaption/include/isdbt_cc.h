#ifndef ISDBT_CC_H_
#define ISDBT_CC_H_

#include <windows.h>

enum OUT_FORMAT 
{
	YV12,
	RGB888,
	XRGB,
	RGB565
};

enum ISDBTCC_PROPID
{
	ISDBTCC_DEFAULT_LANGUAGE_CODE, /*default language code to be used*/	
	ISDBTCC_LANGUAGE_NUM, /*available languages from streams */
	ISDBTCC_LANGUAGE_SELECT, /*current selection of language by index*/
	ISDBTCC_LANGUAGE_CODES, /*language codes from stream, can be more than 1*/

	//the following 3 is set by ISDBTCC_DEFAULT_COLOR
	ISDBTCC_BACKGROUND_COLOR, /*background color of the output buffer*/
	ISDBTCC_FONT_BACKGROUND_COLOR, /*background color of the drawing font*/
	ISDBTCC_FONT_COLOR, /*font color*/
	
	ISDBTCC_BACKGROUND_COLOR_CUSTOMIZED,
	ISDBTCC_FONT_BACKGROUND_COLOR_CUSTOMIZED,
	ISDBTCC_FONT_COLOR_CUSTOMIZED,

};

enum ISDBTCC_DEFAULT_COLOR 
{
	CC_BLACK,
	CC_RED,
	CC_GREEN,
	CC_YELLOW,
	CC_BLUE,
	CC_MAGENTA,
	CC_CYAN,
	CC_WHITE
};

/*
for dwFlags in ISDBTCC_OpenOptions
*/
enum ISDBTCC_OPEN_FLAGS
{
	ISDBTCC_FLAG_NONE = 0,
	ISDBTCC_FLAG_OUTPUT_UPSIDEDOWN = 1<<0, /*For some devices which fill the memory from bottom*/
};

typedef void *ISDBTCC_HANDLE;


struct ISDBTCC_OpenOptions
{	
	OUT_FORMAT OutputFormat;
	DWORD dwOutputBufferNumber; 
	DWORD dwWidth;			/*output width*/
	DWORD dwHeight;			/*output height*/
	DWORD dwFontWidth;		
	DWORD dwFontHeight;	
	DWORD dwMaxLines;
	DWORD dwCharacterPerLine;
	DWORD dwDefaultLanguageCode;
	DWORD dwFlags;
	DWORD dwReserve[16];
};

struct ISDBTCC_DecodeOptions
{
	DWORD dwTimeStamp;	
	DWORD dwReserve[16];
};

struct ISDBTCC_Output
{
	OUT_FORMAT OutputFormat;
	DWORD dwTimeStamp;
	DWORD dwBitsPerPixel;
	DWORD dwWidth[3];
	DWORD dwHeight[3];
	DWORD dwStride[3];
	PBYTE pBuffer[3];	
	DWORD dwReserve[16];
};

////////////////////////////////////////////////////////////////////////////////
//Interface
HRESULT ISDBTCC_Decode(ISDBTCC_HANDLE handle, ISDBTCC_DecodeOptions* option, PBYTE pData, DWORD* pLen);
HRESULT ISDBTCC_Create(ISDBTCC_HANDLE *handle);
HRESULT ISDBTCC_Release(ISDBTCC_HANDLE handle);
HRESULT ISDBTCC_Open(ISDBTCC_HANDLE handle, ISDBTCC_OpenOptions *option);
HRESULT ISDBTCC_Close(ISDBTCC_HANDLE handle);
HRESULT ISDBTCC_Reset(ISDBTCC_HANDLE handle);
HRESULT ISDBTCC_GetOutput(ISDBTCC_HANDLE handle, ISDBTCC_Output* pOutput);
HRESULT ISDBTCC_Set(ISDBTCC_HANDLE handle, DWORD dwPropID, LPVOID pPropData, DWORD cbPropData);
HRESULT ISDBTCC_Get(ISDBTCC_HANDLE handle, DWORD dwPropID, LPVOID pPropData, DWORD cbPropData);


/*
functions for Character Coding - use in EPG and Closed Caption
*/
//Japan
typedef enum {
	KANJI,
	ALPHANUMERIC,
	HIRAGANA,
	KATAKANA,
	MOSAICA,
	MOSAICB,
	MOSAICC,
	MOSAICD,
	P_ALPHANUMERIC,
	P_HIRAGANA,
	P_KATAGANA,
	JISX0201KATAGANA,
	JISCHANGEKNAJI1,
	JISCHANGEKNAJI2,
	ADDSIGN,
	DRCS_FONT //user defined, get from stream
} GSetName;

int JIStoSJIS(unsigned char* pShiftJis, int iSjisLen, unsigned char* pJis, int iJisLen, GSetName gl, GSetName gr, void *param);
int JIStoUTF16(unsigned short* pOut, int outlen, unsigned char* pInput, int inlen, GSetName gl, GSetName gr, void *param);

//Brazil
typedef enum {	
	ALPHANUMERIC_LATIN,
	LATIN_EXTENSION,
	DRCS_FONT_LATIN, //user defined, get from stream
	SPECIAL_CHARACTERS_SET
} GSetName_Latin;

int PORtoUTF16(unsigned short* unicode, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param);


/*
function for Closed Caption
*/
//Japan
int CharacterDecode_JIS(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName gl, GSetName gr, void *param);

//Brazil
int CharacterDecode_Latin(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param);



#endif //ISDBT_CC_H_