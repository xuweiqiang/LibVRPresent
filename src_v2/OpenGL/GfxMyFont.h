#include "stdafx.h"

/*****************************
  # # ##  欢迎光临 zwqxin的3D图形编程记事簿
       http://www.zwqxin.com  
  # # ##  MyFont类 modified  by zwqxin 2009.8

http://www.zwqxin.com/archives/opengl/opengl-font-setting-showing.html
*////////////////////////////

class MyFont
{
   public:

	MyFont();
	~MyFont();

	void SetFontColor(float r,float g,float b, float a);

  	/////////一般的英语字体打印
	void BuildGLFont(int fontHeight);
    void PrintGLText(GLint x, GLint y, const char *string, ...);

	//////////从字体集纹理中取出的字符
	void BuildTextureFont(GLuint fonttex, int fontHeight, int screenWidth, int screenHeight);
	void PrintTextureText(GLint x, GLint y, char *string, int TextureSet);

    ///////////GDI, 位图字体 可设中文字体
	void BuildGDIFont(LPCTSTR lpszFacename, int fontWeights, int fontHeight);
	void PrintGDIText(GLint x, GLint y, CString str);

    void KillFont();

private: 
	void PrintfChtext(int x, int y, LPCTSTR lpszText);

	bool NormalFont;
	bool TextureFont;
	bool GDIFont;

	GLfloat mColor[4];

	GLuint TextureFontFont;
	int ScreenWidth;
	int ScreenHeight;

  	GLuint	NormalFontBase;
	GLuint	TextureFontBase;

	HFONT	hGDIFont;
 }; 