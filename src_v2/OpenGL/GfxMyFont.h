#include "stdafx.h"

/*****************************
  # # ##  ��ӭ���� zwqxin��3Dͼ�α�̼��²�
       http://www.zwqxin.com  
  # # ##  MyFont�� modified  by zwqxin 2009.8

http://www.zwqxin.com/archives/opengl/opengl-font-setting-showing.html
*////////////////////////////

class MyFont
{
   public:

	MyFont();
	~MyFont();

	void SetFontColor(float r,float g,float b, float a);

  	/////////һ���Ӣ�������ӡ
	void BuildGLFont(int fontHeight);
    void PrintGLText(GLint x, GLint y, const char *string, ...);

	//////////�����弯������ȡ�����ַ�
	void BuildTextureFont(GLuint fonttex, int fontHeight, int screenWidth, int screenHeight);
	void PrintTextureText(GLint x, GLint y, char *string, int TextureSet);

    ///////////GDI, λͼ���� ������������
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