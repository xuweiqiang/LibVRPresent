#ifndef _FONTDRAW_H_
#define _FONTDRAW_H_

enum FONT_TYPE {
	FONT_NONE=			0,
	ALPHA_FONT16=		1,
	JIS_FONT16=			2,
	JIS_FONT24=			3,
	LATINEXT_FONT16=	4
};

#include "draw.h"

class FontDraw {

public:

	FontDraw();
	~FontDraw();

	int Open(DWORD width, DWORD height, DWORD num_of_buffer);
	int Close();
	int Clear();
	
	void SetUpsideDown(BOOL value){
		m_pDraw->SetUpsideDown(value);
	}
	void SetFontBackgroundColor(int color[4]);
	void SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR default_color);
	void SetFontColor(int color[4]);
	void SetFontColor(ISDBTCC_DEFAULT_COLOR default_color);
	void SetBackgroundColor(int color[4]);
	void SetBackgroundColor(ISDBTCC_DEFAULT_COLOR default_color);
	void SetFont(DWORD language_code, DWORD font_width, DWORD font_height);
	void SetOutputFormat(OUT_FORMAT format);
	DWORD  GetFontType();
	void SetPosition(int x, int y);
	void SetMaxPos(int lines, int fonts_perline);

	void GetFrameBuffer(unsigned char *buffer[3]);			

	int DMGL_StartDraw(int start_x, int start_y, unsigned int *data, int length, PBYTE buffer[3]);
	/*
	return time in msec if there is TIME code
	*/
	int DMGL_CheckTimeCode(unsigned int *data, int length, int *used_length);


	int DMGL_CheckFLCCode(int *input, int *output, int length);

private:

	int DMGL_Next();

	FontDrawImpl *m_pDraw;

	unsigned char *m_FontData;
	DWORD m_dwLanguageCode;
	OUT_FORMAT m_OutType;
	int m_FontMaxIndex;	
	int m_FontWidth;
	int m_FontHeight;
};

#endif //_FONTDRAW_H_
