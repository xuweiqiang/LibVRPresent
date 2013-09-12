#ifndef DRAW_H_
#define DRAW_H_

#include "isdbt_cc.h"

class FontDrawImpl {

public:
	FontDrawImpl();
	virtual ~FontDrawImpl();

	virtual int Open(DWORD width, DWORD height, DWORD num_of_buffer);
	virtual int Close();
	virtual void CenterThisLine();
	virtual void AlignToBottom();
	virtual void MoveLines(DWORD src_line, DWORD dst_line, DWORD cnt);
	virtual void ClearLines(DWORD start_line, DWORD cnt);

	void SetOutputBuffer(unsigned char *buffer[3]){
		m_OutBuffer[0] = buffer[0];
		m_OutBuffer[1] = buffer[1];
		m_OutBuffer[2] = buffer[2];		
	}

	int NewLine();
	int SetPosition(int x, int y);
	int SetMaxPos(int lines, int fonts_perline);

	int SetFontSize(int width, int height){
		if (width <= 0 || height <= 0)
			return -1;

		m_FontWidth = width;
		m_FontHeight = height;

		int  maxFontsPerLine = m_Width/m_FontWidth;
		int  maxLines = m_Height/m_FontHeight;
		if (m_setFontsPerLine == 0)
			m_maxFontsPerLine = maxFontsPerLine;
		else
			m_maxFontsPerLine = maxFontsPerLine < m_setFontsPerLine ? maxFontsPerLine : m_setFontsPerLine;
		if (m_setLines == 0)
			m_maxLines = maxLines;
		else
			m_maxLines = maxLines < m_setLines ? maxLines : m_setLines;

		return 0;
	}

	int NextPos()
	{
		m_posX+=m_FontWidth;
		if(m_posX>=(m_maxFontsPerLine*m_FontWidth))
		{
			return NewLine();
		}

		return 0;		
	}

	int NextPos(int width, int height)
	{
		m_posX+=width;
		if(m_posX>=(m_maxFontsPerLine*m_FontWidth))
		{
			return NewLine();
		}

		return 0;
	}

	void SetUpsideDown(BOOL value){
		m_bUpsideDown = value;
	}

	virtual void GetOutputBuffer(unsigned char *buffer[3]);
	virtual void SetFontColor(int color[4]);
	virtual void SetFontColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetFontBackgroundColor(int color[4]);
	virtual void SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetBackgroundColor(int color[4]);
	virtual void SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color);
	
	virtual int DrawClear();	
	virtual int DrawFont(unsigned char* font_bitmap, int width, int height);
	virtual int DrawUserData(unsigned char *addr, int w, int h);
	virtual int DrawSpace();
	virtual int DrawDel();	//fill the same color in the font

protected:
	unsigned char *m_OutBuffer[4];
	int* m_XOffset;

	int m_Width;
	int m_Height;

	int m_Color[4];
	int m_BackgroundColor[4];
	int m_FontBackgroundColor[4];

	int m_posX, m_posY;
	int m_maxLines;
	int m_maxFontsPerLine;
	int m_setLines;
	int m_setFontsPerLine;

	int m_FontWidth;
	int m_FontHeight;

	BOOL m_bUpsideDown;
};

FontDrawImpl * CreateFontDraw(OUT_FORMAT type);

#endif //DRAW_H_//
