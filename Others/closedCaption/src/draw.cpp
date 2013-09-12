#include <stdlib.h>
#include <string.h>
#include "draw.h"

#define SAFEFREE(x)           if (x != NULL) {free(x); x = NULL;}

FontDrawImpl::FontDrawImpl()
{
	m_OutBuffer[0] = m_OutBuffer[1] = m_OutBuffer[2] = m_OutBuffer[3] = 0;
	m_XOffset = NULL;
	m_Width = m_Height = 0;
	m_posX = m_posY = 0;
	m_maxLines = m_setLines = 0;
	m_maxFontsPerLine = m_setFontsPerLine = 0;
	m_posX = m_posY = 0;
	m_FontWidth = m_FontHeight = 0;
	m_bUpsideDown = 0;
	SetBackgroundColor(CC_BLACK);
	SetFontBackgroundColor(CC_BLACK);
	SetFontColor(CC_WHITE);
}

FontDrawImpl::~FontDrawImpl()
{
}

int FontDrawImpl::Open(DWORD width, DWORD height, DWORD num_of_buffer)
{
	m_Width = width;
	m_Height = height;
	if (m_XOffset != NULL)
	{
		free(m_XOffset);
		m_XOffset = 0;
	}
	if (m_Height > 0)
		m_XOffset = (int*)malloc(m_Height*sizeof(int));
	if (m_XOffset != NULL)
		memset(m_XOffset, 0, m_Height*sizeof(int));
	return 0;
}

int FontDrawImpl::Close()
{
	if (m_XOffset != NULL)
	{
		free(m_XOffset);
		m_XOffset = 0;
	}
	return 0;
}

void FontDrawImpl::SetFontColor(int color[4])
{	
	m_Color[0] = color[0];
	m_Color[1] = color[1];
	m_Color[2] = color[2];
	m_Color[3] = color[3];
}

void FontDrawImpl::SetFontColor(ISDBTCC_DEFAULT_COLOR color)
{	
}

void FontDrawImpl::SetFontBackgroundColor(int color[4])
{	
	m_FontBackgroundColor[0] = color[0];
	m_FontBackgroundColor[1] = color[1];
	m_FontBackgroundColor[2] = color[2];
	m_FontBackgroundColor[3] = color[3];
}

void FontDrawImpl::SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{
}

void FontDrawImpl::SetBackgroundColor(int color[4])
{	
	m_BackgroundColor[0] = color[0];
	m_BackgroundColor[1] = color[1];
	m_BackgroundColor[2] = color[2];
	m_BackgroundColor[3] = color[3];
}

void FontDrawImpl::SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{
}

int FontDrawImpl::SetPosition(int x, int y)
{
	int  currLine = m_posY/m_FontHeight;
	int  dstLine = y/m_FontHeight;
	int  i;
	if (currLine > dstLine)
	{
		for (i = currLine; i > dstLine; i--)
			m_XOffset[i] = 0;
	}
	m_XOffset[dstLine] = x;

	m_posX = x;
	m_posY = y;

	return 0;
}

int FontDrawImpl::SetMaxPos(int lines, int fonts_perline)
{
	if (lines < 0 || fonts_perline < 0)
		return -1;

	m_setLines = lines;
	m_setFontsPerLine = fonts_perline;
	if (m_Width > 0 && m_Height > 0 &&
		m_FontWidth > 0 && m_FontHeight > 0)
	{
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
	}

	return 0;
}

int FontDrawImpl::NewLine()
{
	CenterThisLine();

	if(m_posX!=0)
		m_posY += m_FontHeight;

	m_posX = 0;

	if (m_posY >= m_maxLines*m_FontHeight)
	{
		MoveLines(1, 0, m_maxLines-1);
		ClearLines(m_maxLines-1, 1);
		m_posY = (m_maxLines-1)*m_FontHeight;
	}

	return 0;	
}

void FontDrawImpl::CenterThisLine()
{
}
void FontDrawImpl::AlignToBottom()
{
	int curr_line = m_posY/m_FontHeight;
	int offset_bottom = m_maxLines-curr_line-1;
	if (offset_bottom > 0)
	{
		MoveLines(0, offset_bottom, curr_line+1);
		ClearLines(0, offset_bottom);
		m_posY += offset_bottom*m_FontHeight;
	}
}
void FontDrawImpl::MoveLines(DWORD src_line, DWORD dst_line, DWORD cnt)
{
}
void FontDrawImpl::ClearLines(DWORD start_line, DWORD cnt)
{
}

int FontDrawImpl::DrawClear()
{
	return 0;
}


int FontDrawImpl::DrawFont(unsigned char* font_bitmap, int width, int height)
{
	return 0;
}

int FontDrawImpl::DrawUserData(unsigned char *addr, int w, int h)
{
	return 0;
}

int FontDrawImpl::DrawSpace()
{
	return 0;
}

int FontDrawImpl::DrawDel()
{
	return 0;
}


void FontDrawImpl::GetOutputBuffer(unsigned char *buffer[3])
{
	buffer[0] = m_OutBuffer[0];
	buffer[1] = m_OutBuffer[1];
	buffer[2] = m_OutBuffer[2];	
}


/**************************************************************************
Font Draw for YUV420
**************************************************************************/
class DrawImpl_YV12 : public FontDrawImpl
{
public:

	DrawImpl_YV12();
	virtual int Open(DWORD width, DWORD height, DWORD num_of_buffer);
	virtual int Close();
	virtual void CenterThisLine();
	virtual void MoveLines(DWORD src_line, DWORD dst_line, DWORD cnt);
	virtual void ClearLines(DWORD start_line, DWORD cnt);

protected:
	virtual void SetFontColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color);

	virtual int DrawClear();
	virtual int DrawFont(unsigned char* font_bitmap, int width, int height);
	virtual int DrawUserData(unsigned char *addr, int w, int h);
	virtual int DrawSpace();
	virtual int DrawDel();

private:
	static const unsigned char m_black[3];
	static const unsigned char m_red[3];
	static const unsigned char m_green[3];
	static const unsigned char m_yellow[3];
	static const unsigned char m_blue[3];
	static const unsigned char m_magenta[3];
	static const unsigned char m_cyan[3];
	static const unsigned char m_white[3];	
};

/*
//color bar stream
const unsigned char m_black[3]  = {16, 128, 128};
const unsigned char m_red[3]  = {51, 108, 211};
const unsigned char m_green[3]  = {133, 63, 52};
const unsigned char m_yellow[3] = {168, 44, 136};
const unsigned char m_blue[3] = {28, 212, 120};
const unsigned char m_magenta[3] = {67, 167, 174};
const unsigned char m_cyan[3] = {145, 147, 44};
const unsigned char m_white[3]  = {235, 128, 128};
*/

//CCIR
/*
const unsigned char m_black[3]	= { 16, 128, 128};
const unsigned char m_red[3]    = { 81,  90, 240};
const unsigned char m_green[3]  = {145,  54,  34};
const unsigned char m_yellow[3] = {210,  16, 146};
const unsigned char m_blue[3]   = { 41, 240, 110};
const unsigned char m_magenta[3]= {106, 202, 222};
const unsigned char m_cyan[3]   = {170, 166,  16};
const unsigned char m_white[3]  = {235, 128, 128};
*/

//75% color bar

//SDTV
/*
const unsigned char m_black[3]	= { 16, 128, 128};
const unsigned char m_red[3]    = { 65, 100, 212};
const unsigned char m_green[3]  = {112,  72,  58};
const unsigned char m_yellow[3] = {162,  44, 142};
const unsigned char m_blue[3]   = { 35, 212, 114};
const unsigned char m_magenta[3]= { 84, 184, 198};
const unsigned char m_cyan[3]   = {131, 156,  44};
const unsigned char m_white[3]  = {180, 128, 128};
*/


//HDTV
/*
const unsigned char m_black[3]	= { 16, 128, 128};
const unsigned char m_red[3]    = { 51, 109, 212};
const unsigned char m_green[3]  = {133,  63,  52};
const unsigned char m_yellow[3] = {168,  44, 136};
const unsigned char m_blue[3]   = { 28, 212, 120};
const unsigned char m_magenta[3]= { 63, 193, 204};
const unsigned char m_cyan[3]   = {145, 147,  44};
const unsigned char m_white[3]  = {180, 128, 128};
*/

const unsigned char DrawImpl_YV12::m_black[3]	= { 16, 128, 128};
const unsigned char DrawImpl_YV12::m_red[3]    = { 65, 100, 212};
const unsigned char DrawImpl_YV12::m_green[3]  = {112,  72,  58};
const unsigned char DrawImpl_YV12::m_yellow[3] = {162,  44, 142};
const unsigned char DrawImpl_YV12::m_blue[3]   = { 35, 212, 114};
const unsigned char DrawImpl_YV12::m_magenta[3]= { 84, 184, 198};
const unsigned char DrawImpl_YV12::m_cyan[3]   = {131, 156,  44};
const unsigned char DrawImpl_YV12::m_white[3]  = {180, 128, 128};	

DrawImpl_YV12::DrawImpl_YV12() : FontDrawImpl()
{	
	
}

int DrawImpl_YV12::Open(DWORD width, DWORD height, DWORD num_of_buffer)
{	
	SetFontColor(CC_WHITE);
	FontDrawImpl::Open(width, height, num_of_buffer);
	return 0;
}

int DrawImpl_YV12::Close()
{
	m_OutBuffer[1] = m_OutBuffer[2] = m_OutBuffer[3] = NULL;

	FontDrawImpl::Close();
	return 0;
}

void DrawImpl_YV12::SetFontColor(ISDBTCC_DEFAULT_COLOR color)
{	
	switch(color)
	{
	case CC_BLACK:		
		m_Color[0] = m_black[0];
		m_Color[1] = m_black[1];
		m_Color[2] = m_black[2];
		break;
	case CC_RED:
		m_Color[0] = m_red[0];
		m_Color[1] = m_red[1];
		m_Color[2] = m_red[2];
		break;
	case CC_GREEN:
		m_Color[0] = m_green[0];
		m_Color[1] = m_green[1];
		m_Color[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_Color[0] = m_yellow[0];
		m_Color[1] = m_yellow[1];
		m_Color[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_Color[0] = m_blue[0];
		m_Color[1] = m_blue[1];
		m_Color[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_Color[0] = m_magenta[0];
		m_Color[1] = m_magenta[1];
		m_Color[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_Color[0] = m_cyan[0];
		m_Color[1] = m_cyan[1];
		m_Color[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_Color[0] = m_white[0];
		m_Color[1] = m_white[1];
		m_Color[2] = m_white[2];
		break;
	}
}

void DrawImpl_YV12::SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{
	switch(color)
	{
	case CC_BLACK:		
		m_FontBackgroundColor[0] = m_black[0];
		m_FontBackgroundColor[1] = m_black[1];
		m_FontBackgroundColor[2] = m_black[2];
		break;
	case CC_RED:
		m_FontBackgroundColor[0] = m_red[0];
		m_FontBackgroundColor[1] = m_red[1];
		m_FontBackgroundColor[2] = m_red[2];
		break;
	case CC_GREEN:
		m_FontBackgroundColor[0] = m_green[0];
		m_FontBackgroundColor[1] = m_green[1];
		m_FontBackgroundColor[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_FontBackgroundColor[0] = m_yellow[0];
		m_FontBackgroundColor[1] = m_yellow[1];
		m_FontBackgroundColor[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_FontBackgroundColor[0] = m_blue[0];
		m_FontBackgroundColor[1] = m_blue[1];
		m_FontBackgroundColor[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_FontBackgroundColor[0] = m_magenta[0];
		m_FontBackgroundColor[1] = m_magenta[1];
		m_FontBackgroundColor[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_FontBackgroundColor[0] = m_cyan[0];
		m_FontBackgroundColor[1] = m_cyan[1];
		m_FontBackgroundColor[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_FontBackgroundColor[0] = m_white[0];
		m_FontBackgroundColor[1] = m_white[1];
		m_FontBackgroundColor[2] = m_white[2];
		break;
	}
}

void DrawImpl_YV12::SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{	
	switch(color)
	{
	case CC_BLACK:		
		m_BackgroundColor[0] = m_black[0];
		m_BackgroundColor[1] = m_black[1];
		m_BackgroundColor[2] = m_black[2];
		break;
	case CC_RED:
		m_BackgroundColor[0] = m_red[0];
		m_BackgroundColor[1] = m_red[1];
		m_BackgroundColor[2] = m_red[2];
		break;
	case CC_GREEN:
		m_BackgroundColor[0] = m_green[0];
		m_BackgroundColor[1] = m_green[1];
		m_BackgroundColor[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_BackgroundColor[0] = m_yellow[0];
		m_BackgroundColor[1] = m_yellow[1];
		m_BackgroundColor[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_BackgroundColor[0] = m_blue[0];
		m_BackgroundColor[1] = m_blue[1];
		m_BackgroundColor[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_BackgroundColor[0] = m_magenta[0];
		m_BackgroundColor[1] = m_magenta[1];
		m_BackgroundColor[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_BackgroundColor[0] = m_cyan[0];
		m_BackgroundColor[1] = m_cyan[1];
		m_BackgroundColor[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_BackgroundColor[0] = m_white[0];
		m_BackgroundColor[1] = m_white[1];
		m_BackgroundColor[2] = m_white[2];
		break;
	}
}

int DrawImpl_YV12::DrawClear()
{
	if(m_OutBuffer[0])
		memset(m_OutBuffer[0], m_BackgroundColor[0], m_Width*m_Height);
	if(m_OutBuffer[1])
		memset(m_OutBuffer[1], m_BackgroundColor[1], (m_Width>>1)*(m_Height>>1));
	if(m_OutBuffer[2])
		memset(m_OutBuffer[2], m_BackgroundColor[2], (m_Width>>1)*(m_Height>>1));
	if(m_XOffset)
		memset(m_XOffset, 0, m_Height*sizeof(int));
	return 0;
}

/*
1. font_width must be multiple of 8 -- 8, 16, 24...
2. draw in place
*/
int DrawImpl_YV12::DrawFont(unsigned char* font_bitmap, int width, int height)
{
	unsigned char *font_addr;
	unsigned char *font_start;	
	int i, j, k;		

	unsigned char *dst;	
	
	font_start = font_bitmap;
	
	{
		//Y	
		if(m_OutBuffer[0])
		{
			font_addr = font_start;
			dst = m_OutBuffer[0]+m_Width*m_posY+m_posX;

			for( i=0; i<m_FontHeight; i++ ) {			
				for( j=0;j<(m_FontWidth>>3);j++) {
					int byte = font_addr[j];
					for( k=0;k<8;k++) {
						dst[k+(j<<3)] = ((byte>>(7-k))&1) ? m_Color[0] : m_FontBackgroundColor[0];
					}
				}
				dst		   += m_Width;
				font_addr  += (m_FontWidth>>3);
			}
		}

		/*
		for better font quality, it is better to use RGB directly
		*/
		//U
		if(m_OutBuffer[1])
		{
			font_addr = font_start;
			dst = m_OutBuffer[1]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
			for( i=0; i<m_FontHeight; i+=2 ) {			
				for( j=0;j<(m_FontWidth>>3);j++) {
					int byte = font_addr[j]|font_addr[j+(m_FontWidth>>3)];
					byte |= (byte<<1);
					for( k=0;k<4;k++) {
						dst[k+(j<<2)] = ((byte>>(7-(k<<1)))&1) ? m_Color[1] : m_FontBackgroundColor[1];
					}
				}
				dst		   += (m_Width>>1);
				font_addr  += (m_FontWidth>>2);
			}
		}

		//V
		if(m_OutBuffer[2])
		{
			font_addr = font_start;
			dst = m_OutBuffer[2]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
			for( i=0; i<m_FontHeight; i+=2 ) {			
				for( j=0;j<(m_FontWidth>>3);j++) {
					int byte = font_addr[j]|font_addr[j+(m_FontWidth>>3)];
					byte |= (byte<<1);
					for( k=0;k<4;k++) {
						dst[k+(j<<2)] = ((byte>>(7-(k<<1)))&1) ? m_Color[2] : m_FontBackgroundColor[2];
					}
				}
				dst		   += (m_Width>>1);
				font_addr  += (m_FontWidth>>2);
			}
		}

		return NextPos();
	}
	return -1;
}

int DrawImpl_YV12::DrawUserData(unsigned char *addr, int w, int h)
{
	unsigned char *font_addr;
	unsigned char *dst;
	int i, j, k;	

	int width = w;
	int height = h;
	if(width<m_FontWidth)
		width=m_FontWidth;
	if(height>m_FontHeight)
		height = m_FontHeight;

	if(addr)
	{
		//Y	
		if(m_OutBuffer[0])
		{
			font_addr = addr;
			dst = m_OutBuffer[0]+m_Width*m_posY+m_posX;

			for( i=0; i<height; i++ ) {			
				for( j=0;j<(width>>3);j++) {
					int byte = font_addr[j];
					for( k=0;k<8;k++) {
						dst[k+(j<<3)] = ((byte>>(7-k))&1) ? m_Color[0] : m_FontBackgroundColor[0];
					}
				}
				dst		   += m_Width;
				font_addr  += (w>>3);
			}
		}

		/*
		for better font quality, it is better to use RGB directly
		*/
		//U
		if(m_OutBuffer[1])
		{
			font_addr = addr;
			dst = m_OutBuffer[1]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
			for( i=0; i<height; i+=2 ) {			
				for( j=0;j<(width>>3);j++) {
					int byte = font_addr[j]|font_addr[j+(width>>3)];
					byte |= (byte<<1);
					for( k=0;k<4;k++) {
						dst[k+(j<<2)] = ((byte>>(7-(k<<1)))&1) ? m_Color[1] : m_FontBackgroundColor[1];
					}
				}
				dst		   += (m_Width>>1);
				font_addr  += (w>>2);
			}
		}

		//V
		if(m_OutBuffer[2])
		{
			font_addr = addr;
			dst = m_OutBuffer[2]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
			for( i=0; i<height; i+=2 ) {			
				for( j=0;j<(width>>3);j++) {
					int byte = font_addr[j]|font_addr[j+(width>>3)];
					byte |= (byte<<1);
					for( k=0;k<4;k++) {
						dst[k+(j<<2)] = ((byte>>(7-(k<<1)))&1) ? m_Color[2] : m_FontBackgroundColor[2];
					}
				}
				dst		   += (m_Width>>1);
				font_addr  += (w>>2);
			}
		}

		return NextPos();
	}
	return -1;	
}

int DrawImpl_YV12::DrawSpace()
{
	int i;
	unsigned char *dst;

	if(m_OutBuffer[0])
	{
		dst = m_OutBuffer[0]+m_Width*m_posY+m_posX;
		for( i=0; i<m_FontHeight; i++ ) {
			memset(dst, m_FontBackgroundColor[0], m_FontWidth);		
			dst += m_Width;	
		}	
	}

	if(m_OutBuffer[1])
	{
		dst = m_OutBuffer[1]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
		for( i=0; i<(m_FontHeight>>1); i++ ) {
			memset(dst, m_FontBackgroundColor[1], (m_FontWidth>>1));		
			dst += (m_Width>>1);	
		}
	}

	if(m_OutBuffer[2])
	{
		dst = m_OutBuffer[2]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
		for( i=0; i<(m_FontHeight>>1); i++ ) {
			memset(dst, m_FontBackgroundColor[2], (m_FontWidth>>1));		
			dst += (m_Width>>1);	
		}
	}

	return NextPos();
}

int DrawImpl_YV12::DrawDel()
{
	int i;
	unsigned char *dst;

	dst = m_OutBuffer[0]+m_Width*m_posY+m_posX;

	for( i=0; i<m_FontHeight; i++ ) {
		memset(dst, m_Color[0], m_FontWidth);		
		dst += m_Width;	
	}	

	if(m_OutBuffer[1])
	{
		dst = m_OutBuffer[1]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
		for( i=0; i<(m_FontHeight>>1); i++ ) {
			memset(dst, m_Color[1], (m_FontWidth>>1));		
			dst += (m_Width>>1);	
		}
	}

	if(m_OutBuffer[2])
	{
		dst = m_OutBuffer[2]+(m_Width>>1)*(m_posY>>1)+(m_posX>>1);
		for( i=0; i<(m_FontHeight>>1); i++ ) {
			memset(dst, m_Color[2], (m_FontWidth>>1));		
			dst += (m_Width>>1);	
		}
	}

	return NextPos();
}

void DrawImpl_YV12::CenterThisLine()
{
	int  Xoffset = 0;
	int  XoffsetOrg = m_XOffset[m_posY/m_FontHeight];
	if (m_posX-XoffsetOrg < m_Width)
	{
		Xoffset = (m_Width-(m_posX-XoffsetOrg))/2;
		Xoffset &= ~1;
	}

	if (Xoffset > 0 && Xoffset != XoffsetOrg)
	{
		unsigned char* dst;
		int  i, j;
		int  forwardCopy, offsetDelta;
		if (Xoffset < XoffsetOrg)
		{
			forwardCopy = 1;
			offsetDelta = XoffsetOrg-Xoffset;
		}
		else
		{
			forwardCopy = 0;
			offsetDelta = Xoffset-XoffsetOrg;
		}

		// Y
		for (j = 0; j < m_FontHeight; j++)
		{
			dst = m_OutBuffer[0]+m_Width*(m_posY+j);
			if (forwardCopy)
			{
				for (i = XoffsetOrg; i < m_posX; i++)
					dst[i-offsetDelta] = dst[i];
				memset(dst+m_posX-offsetDelta, m_BackgroundColor[0], offsetDelta);
			}
			else
			{
				for (i = m_posX; i >= XoffsetOrg; i--)
					dst[i+offsetDelta] = dst[i];
				memset(dst+XoffsetOrg, m_BackgroundColor[0], offsetDelta);
			}
		}
		// U
		for (j = 0; j < (m_FontHeight>>1); j++)
		{
			dst = m_OutBuffer[1]+(m_Width>>1)*((m_posY>>1)+j);
			if (forwardCopy)
			{
				for (i = (XoffsetOrg>>1); i < (m_posX>>1); i++)
					dst[i-(offsetDelta>>1)] = dst[i];
				memset(dst+((m_posX-offsetDelta)>>1), m_BackgroundColor[1], (offsetDelta)>>1);
			}
			else
			{
				for (i = (m_posX>>1); i >= (XoffsetOrg>>1); i--)
					dst[i+(offsetDelta>>1)] = dst[i];
				memset(dst+(XoffsetOrg>>1), m_BackgroundColor[1], (offsetDelta>>1));
			}
		}
		// V
		for (j = 0; j < (m_FontHeight>>1); j++)
		{
			dst = m_OutBuffer[2]+(m_Width>>1)*((m_posY>>1)+j);
			if (forwardCopy)
			{
				for (i = (XoffsetOrg>>1); i < (m_posX>>1); i++)
					dst[i-(offsetDelta>>1)] = dst[i];
				memset(dst+((m_posX-offsetDelta)>>1), m_BackgroundColor[2], (offsetDelta)>>1);
			}
			else
			{
				for (i = (m_posX>>1); i >= (XoffsetOrg>>1); i--)
					dst[i+(offsetDelta>>1)] = dst[i];
				memset(dst+(XoffsetOrg>>1), m_BackgroundColor[2], (offsetDelta>>1));
			}
		}
		m_posX += Xoffset-XoffsetOrg;
		m_XOffset[m_posY/m_FontHeight] = Xoffset;
	}
}

void DrawImpl_YV12::MoveLines(DWORD src_line, DWORD dst_line, DWORD cnt)
{
	if (m_Width <= 0 || m_Height <= 0 || m_maxFontsPerLine <= 0 || m_maxLines <= 0 ||
		m_OutBuffer[0] == NULL || m_OutBuffer[1] == NULL || m_OutBuffer[2] == NULL ||
		cnt <= 0 || src_line == dst_line ||
		 src_line >= m_maxLines || src_line+cnt-1 >= m_maxLines ||
		 dst_line >= m_maxLines || dst_line+cnt-1 >= m_maxLines)
		return;

	unsigned char  *pSrc, *pDst;
	unsigned int  nLineSize = m_Width*m_FontHeight;
	int  i;
	if (src_line > dst_line)
	{
		// Y
		pSrc = m_OutBuffer[0]+src_line*nLineSize;
		pDst = m_OutBuffer[0]+dst_line*nLineSize;
		for (i = 0; i < cnt; i++)
		{
			memcpy(pDst, pSrc, nLineSize);
			pSrc += nLineSize;
			pDst += nLineSize;
			m_XOffset[dst_line+i] = m_XOffset[src_line+i];
		}
		// U
		pSrc = m_OutBuffer[1]+src_line*(nLineSize>>2);
		pDst = m_OutBuffer[1]+dst_line*(nLineSize>>2);
		for (i = 0; i < cnt; i++)
		{
			memcpy(pDst, pSrc, nLineSize>>2);
			pSrc += nLineSize>>2;
			pDst += nLineSize>>2;
		}
		// U
		pSrc = m_OutBuffer[2]+src_line*(nLineSize>>2);
		pDst = m_OutBuffer[2]+dst_line*(nLineSize>>2);
		for (i = 0; i < cnt; i++)
		{
			memcpy(pDst, pSrc, nLineSize>>2);
			pSrc += nLineSize>>2;
			pDst += nLineSize>>2;
		}
	}
	else
	{
		// Y
		pSrc = m_OutBuffer[0]+(src_line+cnt-1)*nLineSize;
		pDst = m_OutBuffer[0]+(dst_line+cnt-1)*nLineSize;
		for (i = cnt-1; i >= 0; i--)
		{
			memcpy(pDst, pSrc, nLineSize);
			pSrc -= nLineSize;
			pDst -= nLineSize;
			m_XOffset[dst_line+i] = m_XOffset[src_line+i];
		}
		// U
		pSrc = m_OutBuffer[1]+(src_line+cnt-1)*(nLineSize>>2);
		pDst = m_OutBuffer[1]+(dst_line+cnt-1)*(nLineSize>>2);
		for (i = cnt-1; i >= 0; i--)
		{
			memcpy(pDst, pSrc, nLineSize>>2);
			pSrc -= nLineSize>>2;
			pDst -= nLineSize>>2;
		}
		// U
		pSrc = m_OutBuffer[2]+(src_line+cnt-1)*(nLineSize>>2);
		pDst = m_OutBuffer[2]+(dst_line+cnt-1)*(nLineSize>>2);
		for (i = cnt-1; i >= 0; i--)
		{
			memcpy(pDst, pSrc, nLineSize>>2);
			pSrc -= nLineSize>>2;
			pDst -= nLineSize>>2;
		}
	}
}

void DrawImpl_YV12::ClearLines(DWORD start_line, DWORD cnt)
{
	if (m_Width <= 0 || m_Height <= 0 || m_maxFontsPerLine <= 0 || m_maxLines <= 0 ||
		m_OutBuffer[0] == NULL || m_OutBuffer[1] == NULL || m_OutBuffer[2] == NULL ||
		cnt <= 0 ||  start_line >= m_maxLines || start_line+cnt-1 >= m_maxLines)
		return;

	unsigned char  *pBuf;
	unsigned int  nLineSize = m_Width*m_FontHeight;
	int  i;
	for (i = 0; i < cnt; i++)
		m_XOffset[start_line+i] = 0;
	// Y
	pBuf = m_OutBuffer[0]+start_line*nLineSize;
	memset(pBuf, m_BackgroundColor[0], nLineSize*cnt);
	// U
	pBuf = m_OutBuffer[1]+start_line*(nLineSize>>2);
	memset(pBuf, m_BackgroundColor[1], (nLineSize>>2)*cnt);
	// V
	pBuf = m_OutBuffer[2]+start_line*(nLineSize>>2);
	memset(pBuf, m_BackgroundColor[2], (nLineSize>>2)*cnt);
}

/**************************************************************************
Font Draw for XRGB
**************************************************************************/
class DrawImpl_XRGB : public FontDrawImpl
{
public:
	DrawImpl_XRGB();

	virtual int Open(DWORD width, DWORD height, DWORD num_of_buffer);
	virtual int Close();
	virtual void AlignToBottom() {}

protected:
	virtual void SetFontColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color);
	virtual void SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color);

	virtual int DrawClear();
	virtual int DrawFont(unsigned char* font_bitmap, int width, int height);
	virtual int DrawUserData(unsigned char *addr, int w, int h);
	virtual int DrawSpace();
	virtual int DrawDel();

	static const unsigned char m_black[3];
	static const unsigned char m_red[3];
	static const unsigned char m_green[3];
	static const unsigned char m_yellow[3];
	static const unsigned char m_blue[3];
	static const unsigned char m_magenta[3];
	static const unsigned char m_cyan[3];
	static const unsigned char m_white[3];

	void inline ColorXRGB(unsigned char *dst, int color[3])
	{
		dst[0] = color[2];
		dst[1] = color[1];
		dst[2] = color[0];
		dst[3] = 0;
	}

	//RGB
/*
const unsigned char m_black[3]	= { 0, 0, 0};
const unsigned char m_red[3]    = { 255, 0, 0};
const unsigned char m_green[3]  = {0,  255,  0};
const unsigned char m_yellow[3] = {255,  255, 0};
const unsigned char m_blue[3]   = { 0, 0, 255};
const unsigned char m_magenta[3]= { 255, 0, 255};
const unsigned char m_cyan[3]   = {0, 255,  255};
const unsigned char m_white[3]  = {255, 255, 255};
*/
};

const unsigned char DrawImpl_XRGB::m_black[3]	= { 0, 0, 0};
const unsigned char DrawImpl_XRGB::m_red[3]    = { 255, 0, 0};
const unsigned char DrawImpl_XRGB::m_green[3]  = {0,  255,  0};
const unsigned char DrawImpl_XRGB::m_yellow[3] = {255,  255, 0};
const unsigned char DrawImpl_XRGB::m_blue[3]   = { 0, 0, 255};
const unsigned char DrawImpl_XRGB::m_magenta[3]= { 255, 0, 255};
const unsigned char DrawImpl_XRGB::m_cyan[3]   = {0, 255,  255};
const unsigned char DrawImpl_XRGB::m_white[3]  = {255, 255, 255};

DrawImpl_XRGB::DrawImpl_XRGB() : FontDrawImpl()
{	
	
}

int DrawImpl_XRGB::Open(DWORD width, DWORD height, DWORD num_of_buffer)
{
	SetFontColor(CC_WHITE);
	FontDrawImpl::Open(width, height, num_of_buffer);
	return 0;
}

int DrawImpl_XRGB::Close()
{	
	m_OutBuffer[1] = m_OutBuffer[2] = m_OutBuffer[3] = NULL;

	FontDrawImpl::Close();
	return 0;
}

void DrawImpl_XRGB::SetFontColor(ISDBTCC_DEFAULT_COLOR color)
{	
	switch(color)
	{
	case CC_BLACK:		
		m_Color[0] = m_black[0];
		m_Color[1] = m_black[1];
		m_Color[2] = m_black[2];
		break;
	case CC_RED:
		m_Color[0] = m_red[0];
		m_Color[1] = m_red[1];
		m_Color[2] = m_red[2];
		break;
	case CC_GREEN:
		m_Color[0] = m_green[0];
		m_Color[1] = m_green[1];
		m_Color[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_Color[0] = m_yellow[0];
		m_Color[1] = m_yellow[1];
		m_Color[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_Color[0] = m_blue[0];
		m_Color[1] = m_blue[1];
		m_Color[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_Color[0] = m_magenta[0];
		m_Color[1] = m_magenta[1];
		m_Color[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_Color[0] = m_cyan[0];
		m_Color[1] = m_cyan[1];
		m_Color[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_Color[0] = m_white[0];
		m_Color[1] = m_white[1];
		m_Color[2] = m_white[2];
		break;
	}
}

void DrawImpl_XRGB::SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{
	switch(color)
	{
	case CC_BLACK:		
		m_FontBackgroundColor[0] = m_black[0];
		m_FontBackgroundColor[1] = m_black[1];
		m_FontBackgroundColor[2] = m_black[2];
		break;
	case CC_RED:
		m_FontBackgroundColor[0] = m_red[0];
		m_FontBackgroundColor[1] = m_red[1];
		m_FontBackgroundColor[2] = m_red[2];
		break;
	case CC_GREEN:
		m_FontBackgroundColor[0] = m_green[0];
		m_FontBackgroundColor[1] = m_green[1];
		m_FontBackgroundColor[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_FontBackgroundColor[0] = m_yellow[0];
		m_FontBackgroundColor[1] = m_yellow[1];
		m_FontBackgroundColor[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_FontBackgroundColor[0] = m_blue[0];
		m_FontBackgroundColor[1] = m_blue[1];
		m_FontBackgroundColor[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_FontBackgroundColor[0] = m_magenta[0];
		m_FontBackgroundColor[1] = m_magenta[1];
		m_FontBackgroundColor[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_FontBackgroundColor[0] = m_cyan[0];
		m_FontBackgroundColor[1] = m_cyan[1];
		m_FontBackgroundColor[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_FontBackgroundColor[0] = m_white[0];
		m_FontBackgroundColor[1] = m_white[1];
		m_FontBackgroundColor[2] = m_white[2];
		break;
	}

	m_FontBackgroundColor[3] = 0;
}

void DrawImpl_XRGB::SetBackgroundColor(ISDBTCC_DEFAULT_COLOR color)
{	
	switch(color)
	{
	case CC_BLACK:		
		m_BackgroundColor[0] = m_black[0];
		m_BackgroundColor[1] = m_black[1];
		m_BackgroundColor[2] = m_black[2];
		break;
	case CC_RED:
		m_BackgroundColor[0] = m_red[0];
		m_BackgroundColor[1] = m_red[1];
		m_BackgroundColor[2] = m_red[2];
		break;
	case CC_GREEN:
		m_BackgroundColor[0] = m_green[0];
		m_BackgroundColor[1] = m_green[1];
		m_BackgroundColor[2] = m_green[2];
		break;
	case CC_YELLOW:
		m_BackgroundColor[0] = m_yellow[0];
		m_BackgroundColor[1] = m_yellow[1];
		m_BackgroundColor[2] = m_yellow[2];
		break;
	case CC_BLUE:
		m_BackgroundColor[0] = m_blue[0];
		m_BackgroundColor[1] = m_blue[1];
		m_BackgroundColor[2] = m_blue[2];
		break;
	case CC_MAGENTA:
		m_BackgroundColor[0] = m_magenta[0];
		m_BackgroundColor[1] = m_magenta[1];
		m_BackgroundColor[2] = m_magenta[2];
		break;
	case CC_CYAN:
		m_BackgroundColor[0] = m_cyan[0];
		m_BackgroundColor[1] = m_cyan[1];
		m_BackgroundColor[2] = m_cyan[2];
		break;
	case CC_WHITE:
		m_BackgroundColor[0] = m_white[0];
		m_BackgroundColor[1] = m_white[1];
		m_BackgroundColor[2] = m_white[2];
		break;
	}

	m_BackgroundColor[3] = 0;
}

int DrawImpl_XRGB::DrawClear()
{
	if(m_OutBuffer[0])
		memset(m_OutBuffer[0], m_BackgroundColor[0], m_Width*m_Height*4);
	if(m_XOffset)
		memset(m_XOffset, 0, m_Height*sizeof(int));
	return 0;
}

/*
1. font_width must be multiple of 8 -- 8, 16, 24...
2. draw in place
*/
int DrawImpl_XRGB::DrawFont(unsigned char* font_bitmap, int width, int height)
{
	unsigned char *font_addr;
	unsigned char *font_start;	
	int i, j, k;
	unsigned char *dst;	
	
	font_start = font_bitmap;

	int stride;
	unsigned char *output_start;
	if(m_bUpsideDown)
	{
		stride = -m_Width;
		output_start = m_OutBuffer[0]+(m_Height-1)*m_Width*4;
	}
	else
	{
		stride = m_Width;
		output_start = m_OutBuffer[0];
	}
	
	{		
		if(m_OutBuffer[0])
		{
			font_addr = font_start;
			dst = output_start+stride*m_posY*4+m_posX*4;

			for( i=0; i<m_FontHeight; i++ ) {			
				for( j=0;j<(m_FontWidth>>3);j++) {
					int byte = font_addr[j];
					for( k=0;k<8;k++) {
						//dst[k+(j<<3)] = ((byte>>(7-k))&1) ? m_Color[0] : dst[k+(j<<3)];
						if((byte>>(7-k))&1)
						{
							ColorXRGB(&dst[(k+(j<<3))<<2], m_Color);							
						}
					}
				}
				dst		   += stride*4;
				font_addr  += (m_FontWidth>>3);
			}
		}		

		return NextPos();
	}
	return -1;
}

int DrawImpl_XRGB::DrawUserData(unsigned char *addr, int w, int h)
{
	unsigned char *font_addr;
	unsigned char *dst;
	int i, j, k;	

	int width = w;
	int height = h;
	if(width<m_FontWidth)
		width=m_FontWidth;
	if(height>m_FontHeight)
		height = m_FontHeight;

	int stride;
	unsigned char *output_start;
	if(m_bUpsideDown)
	{
		stride = -m_Width;
		output_start = m_OutBuffer[0]+(m_Height-1)*m_Width*4;
	}
	else
	{
		stride = m_Width;
		output_start = m_OutBuffer[0];
	}

	if(addr)
	{
		//Y	
		if(m_OutBuffer[0])
		{
			font_addr = addr;
			dst = output_start+stride*m_posY*4+m_posX*4;

			for( i=0; i<m_FontHeight; i++ ) {			
				for( j=0;j<(m_FontWidth>>3);j++) {
					int byte = font_addr[j];
					for( k=0;k<8;k++) {
						//dst[k+(j<<3)] = ((byte>>(7-k))&1) ? m_Color[0] : dst[k+(j<<3)];
						if((byte>>(7-k))&1)
						{
							ColorXRGB(&dst[(k+(j<<3))<<2], m_Color);							
						}
					}
				}
				dst		   += stride*4;
				font_addr  += (m_FontWidth>>3);
			}
		}

		return NextPos();
	}
	return -1;	
}

int DrawImpl_XRGB::DrawSpace()
{
	int i, j;
	unsigned char *dst;

	int stride;
	unsigned char *output_start;
	if(m_bUpsideDown)
	{
		stride = -m_Width;
		output_start = m_OutBuffer[0]+(m_Height-1)*m_Width*4;
	}
	else
	{
		stride = m_Width;
		output_start = m_OutBuffer[0];
	}

	if(m_OutBuffer[0])
	{
		dst = output_start+stride*m_posY*4+m_posX*4;
		for( j=0; j<m_FontHeight; j++ ) {
			for( i=0; i<m_FontWidth; i+=4 ) {
				ColorXRGB(dst+i, m_BackgroundColor);					
			}
			dst += stride*4;
		}	
	}	

	return NextPos();
}

int DrawImpl_XRGB::DrawDel()
{
	int i, j;
	unsigned char *dst;

	int stride;
	unsigned char *output_start;
	if(m_bUpsideDown)
	{
		stride = -m_Width;
		output_start = m_OutBuffer[0]+(m_Height-1)*m_Width*4;
	}
	else
	{
		stride = m_Width;
		output_start = m_OutBuffer[0];
	}	

	if(m_OutBuffer[0])
	{
		dst = output_start+stride*m_posY*4+m_posX*4;
		for( j=0; j<m_FontHeight; j++ ) {
			for( i=0; i<m_FontWidth; i+=4 ) {
				ColorXRGB(dst+i, m_Color);				
			}
			dst += stride*4;
		}	
	}	

	return NextPos();
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
FontDrawImpl * CreateFontDraw(OUT_FORMAT type)
{
	if(type==YV12)
		return reinterpret_cast<FontDrawImpl *>(new DrawImpl_YV12());
	if(type==XRGB)
		return reinterpret_cast<FontDrawImpl *>(new DrawImpl_XRGB());

	return 0;
}