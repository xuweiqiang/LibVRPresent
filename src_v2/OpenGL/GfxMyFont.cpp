// DesertedCity.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GfxMyFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MyFont::MyFont() : NormalFontBase(0), TextureFontBase(0), TextureFontFont(0),
 NormalFont(false), TextureFont(false), GDIFont(false)
{
  mColor[0] = mColor[1] = mColor[2] = mColor[3] = 1.0f;
}

MyFont::~MyFont()
{
  KillFont();
}


void MyFont::SetFontColor(float r,float g,float b, float a)
{
	mColor[0] = r;
	mColor[1] = g;
	mColor[2] = b;
	mColor[3] = a;
}


 /////////一般的英语字体打印
void MyFont::BuildGLFont(int fontHeight)
{
   HDC   hDC =::GetDC(HWND_DESKTOP);    //////就是这里搞晕了半晚
   
   int tFontHeight = -1 * fontHeight;

   NormalFontBase = glGenLists(96);						    // Storage For 96 Characters
   HFONT font = CreateFont( -tFontHeight,					// Height Of Font
							  0,							// Width Of Font
						 	  0,							// Angle Of Escapement
							  0,							// Orientation Angle
							  FW_BOLD,						// Font Weight
						  	  TRUE,							// Italic
							  FALSE,						// Underline
							  FALSE,						// Strikeout
							  ANSI_CHARSET,					// Character Set Identifier
							  OUT_TT_PRECIS,				// Output Precision
							  CLIP_DEFAULT_PRECIS,			// Clipping Precision
							  ANTIALIASED_QUALITY,			// Output Quality
							  FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
							  "Georgia");					// Font Name

	HFONT oldfont = (HFONT)SelectObject(hDC, font);         // Selects The Font We Want
	
	wglUseFontBitmaps(hDC, 32, 96, NormalFontBase);			// Builds 96 Characters Starting At Character 32
	
	SelectObject(hDC, oldfont);							// Selects The Font We Want to return to
	DeleteObject(font);									// Delete The Font
    
	SetBkMode(hDC,TRANSPARENT);   

	NormalFont = true; 
}


void MyFont::PrintGLText(GLint x, GLint y, const char *string, ...)	// Custom GL "Print" Routine
{
	char  text[256];								    // Holds Our String
	va_list	pArguments;										// Pointer To List Of Arguments

	if (string == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(pArguments, string);						// Parses The String For Variables
	vsprintf(text, string, pArguments);				    // And Converts Symbols To Actual Numbers
	va_end(pArguments);									// Results Are Stored In Text

                                                         // Pushes The Display List Bits
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT |  GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);	
	glDisable(GL_LIGHTING); 
	glDisable(GL_TEXTURE_2D); 
	glColor4f(mColor[0], mColor[1], mColor[2], mColor[3]);

	glWindowPos2i(x, y);
						
	glListBase(NormalFontBase - 32);					// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text

	glPopAttrib();	
}


//////////从字体集纹理中取出的字符
void MyFont::BuildTextureFont(GLuint fonttex, int fontHeight, int screenWidth, int screenHeight)
{
	TextureFontFont = fonttex;

	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	glEnable(GL_TEXTURE_2D);

	TextureFontBase = glGenLists(256);					// Creating 256 Display Lists

	glBindTexture(GL_TEXTURE_2D, TextureFontFont);	    // Select Our Font Texture

	for(int loop=0; loop<256; loop++)					// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(TextureFontBase + loop, GL_COMPILE);	// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(fontHeight, 0);				// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(fontHeight,fontHeight);		// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0, fontHeight);				// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(fontHeight,0,0);				// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built

    glDisable(GL_TEXTURE_2D);

	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;

	TextureFont = true;

}


void MyFont::PrintTextureText(GLint x, GLint y, char *string, int TextureSet)
{
	if (TextureSet > 1)TextureSet = 1;
	if (TextureSet < 0)TextureSet = 0;

	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT|  GL_LIST_BIT);
	glDisable(GL_LIGHTING); 

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureFontFont);		// Select Our Font Texture

	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glColor4f(mColor[0], mColor[1], mColor[2], mColor[3]);  

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,ScreenWidth,0,ScreenHeight,-1,1);			// Set Up An Ortho Screen

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	
	glLoadIdentity();									// Reset The Modelview Matrix
	glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
	
	glListBase(TextureFontBase-32 + (128 * TextureSet));// Choose The Font Set (0 or 1)
	glCallLists(strlen(string),GL_UNSIGNED_BYTE,string);// Write The Text To The Screen
	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDisable(GL_TEXTURE_2D);

	glPopAttrib();
}


///////////GDI, 位图字体 可设中文字体
void MyFont::BuildGDIFont(LPCTSTR lpszFacename, int fontWeights, int fontHeight)
{
	// Font Weights //
	//#define FW_DONTCARE         0
	//#define FW_THIN             100
	//#define FW_EXTRALIGHT       200
	//#define FW_LIGHT            300
	//#define FW_NORMAL           400
	//#define FW_MEDIUM           500
	//#define FW_SEMIBOLD         600
	//#define FW_BOLD             700
	//#define FW_EXTRABOLD        800
	//#define FW_HEAVY            900

	  int tfontHeight = -1 * fontHeight;

      hGDIFont = CreateFont(tfontHeight, 0, 0, 0, fontWeights, 0, 0, 0, GB2312_CHARSET,
		                0, 0, 0, FF_MODERN, lpszFacename);
	  GDIFont = true;
}

void MyFont::PrintfChtext(int x, int y, LPCTSTR lpszText)
{
	  CBitmap bitmap;
	  BITMAP bm;
	  SIZE size;
	  
	  HDC MDC = ::CreateCompatibleDC(NULL);
	  SelectObject(MDC, hGDIFont);
	  
	  ::GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);

	  bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);

	  HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,bitmap);

	  SetBkColor  (MDC, RGB(0,     0,   0));
	  SetTextColor(MDC, RGB(255, 255, 255));

	  TextOut(MDC, 0, 0, lpszText, strlen(lpszText));

	  bitmap.GetBitmap(&bm);
	  size.cx = (bm.bmWidth + 31) & (~31);

	  int bufsize = size.cy * size.cx;

	  struct {  
		      BITMAPINFOHEADER bih;
			  RGBQUAD col[2];
 			 }bic; 

	  BITMAPINFO *binf = (BITMAPINFO *)&bic; 
	  binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);
	  binf->bmiHeader.biWidth    = bm.bmWidth;
	  binf->bmiHeader.biHeight   = bm.bmHeight;
	  binf->bmiHeader.biPlanes   = 1;   
	  binf->bmiHeader.biBitCount = 1;
	  binf->bmiHeader.biCompression = BI_RGB;
	  binf->bmiHeader.biSizeImage   = bufsize; 

	  UCHAR* Bits = new UCHAR[bufsize];	
	  ::GetDIBits(MDC,bitmap, 0, bm.bmHeight, Bits, binf, DIB_RGB_COLORS); 
                                      
	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	  //glRasterPos2i(x, y); 
	  glWindowPos2i(x, y);
	  glBitmap(size.cx, size.cy, 0, 0, 0, 0, Bits); 

	  delete Bits;    
	  SelectObject(MDC, oldBmp);  
	  ::DeleteDC(MDC);
}

void MyFont::PrintGDIText(GLint x, GLint y, CString str)
{
    glLoadIdentity();
	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);

	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_LIGHTING); 

	glColor4f(mColor[0], mColor[1], mColor[2], mColor[3]);  

	PrintfChtext (x, y, str); 
	
	glPopAttrib();
}


void MyFont::KillFont()
{
   if(NormalFont)
   {
	   glDeleteLists(NormalFontBase, 96); // Delete All 96 Characters
	   NormalFont = false;
   }

   if(TextureFont)
   {
	   glDeleteLists(TextureFontBase, 256); // Delete All 256 Characters
       TextureFont = false;
   }

   if(GDIFont)
   {
       GDIFont = false;
   }

}



