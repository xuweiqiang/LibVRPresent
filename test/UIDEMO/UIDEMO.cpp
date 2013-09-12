// Pyramid.cpp
// Demonstrates Simple Texture Mapping
// OpenGL SuperBible
// Richard S. Wright Jr.
#include "shared/gltools.h"   // GLTools
#include "shared/math3d.h"    // 3D Math Library
#include "shared/glframe.h"   // Frame Class
#include <stdlib.h>
#include "stdio.h"
#include "glext.h"	
#include <gl\glaux.h>							// Header File For The GLaux Library
#include "stdio.h"


GLuint	textureBackground = 0;
AUX_RGBImageRec *TextureImageBackGround = NULL;

GLuint	textureTOP[5] = {0};
AUX_RGBImageRec *TextureImageTOP[5] = {0};

GLuint	texturebottom[2] = {0};
AUX_RGBImageRec *TextureImageBottom[2] = {0};

GLuint nFontList;
static bool bPressed = true;

GLuint	base;				// Base Display List For The Font Set


AUX_RGBImageRec *LoadBMP(char *Filename)					// Loads A Bitmap Image
{
	FILE *File=NULL;							// File Handle
	if (!Filename)								// Make Sure A Filename Was Given
	{
		return NULL;							// If Not Return NULL
	}

	File=fopen(Filename,"r");						// Check To See If The File Exists
	if (File)								// Does The File Exist?
	{
		fclose(File);							// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;								// If Load Failed Return NULL
}


// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int w, int h)
{
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	GLfloat fAspect = (GLfloat)w/(GLfloat)h;

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(w<=h)
		glOrtho(-2.0,2.0, -2.0/fAspect, 2.0/fAspect, -5.0, 10.0);
	else
		glOrtho(-2.0*fAspect, 2.0*fAspect, -2,2, -5.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	char fileName[256];

	//Load background
	glGenTextures(1, &textureBackground);
	sprintf(fileName,".\\testvector4.bmp");
	TextureImageBackGround =  LoadBMP(fileName);
	glBindTexture(GL_TEXTURE_2D, textureBackground);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImageBackGround->sizeX, TextureImageBackGround->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE, TextureImageBackGround->data);

	//Load top part
	glGenTextures(5, &textureTOP[0]);
	TextureImageTOP[0] = LoadBMP(".\\rightTopIcon1.bmp");
	TextureImageTOP[1] = LoadBMP(".\\rightTopIcon2.bmp");
	TextureImageTOP[2] = LoadBMP(".\\rightTopIcon3.bmp");
	TextureImageTOP[3] = LoadBMP(".\\rightTopIcon4.bmp");
	TextureImageTOP[4] = LoadBMP(".\\rightTopSlider.bmp");

	glBindTexture(GL_TEXTURE_2D, textureTOP[4]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImageTOP[4]->sizeX, TextureImageTOP[4]->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE, TextureImageTOP[4]->data);

	for(int i=0;i<4;i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureTOP[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImageTOP[i]->sizeX, TextureImageTOP[i]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImageTOP[i]->data);
	}


	glGenTextures(2, &texturebottom[0]);
	TextureImageBottom[0] = LoadBMP(".\\bottomSlider.bmp");
	TextureImageBottom[1] = LoadBMP(".\\bottombutton.bmp");
	for(int i=0;i<2;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texturebottom[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureImageBottom[i]->sizeX, TextureImageBottom[i]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImageBottom[i]->data);
	}

	glShadeModel(GL_SMOOTH);	
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_TEXTURE_2D);
	//Improve image quality on low end color system.
	glEnable(GL_DITHER);


}

void processMouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if(bPressed)
				bPressed = false;
			else
				bPressed  =true;
		}

	}

}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_F1)
	{

	}


	// Refresh the Window
	glutPostRedisplay();
}

GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring

// Called to draw scene
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0.0f,0.0f,1.0f);						// Move One Unit Into The Screen
	glRasterPos2f(-2.5f,1.9f);

	for(int i=0;i<20;i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,'t');
	glPopMatrix();

	//DrawBackGround
	M3DVector3f vNormal;
	M3DVector3f vCorners[6] = { { 0.5f, .80f, 0.0f },     // Top           0
	{ -0.5f, 0.0f, -0.00f },    // Back left     1
	{ 0.5f, 0.0f, -0.0f },    // Back right    2
	{ 0.5f, 0.0f, 0.0f },      // Front right   3
	{ -0.5f, 0.0f, 0.0f }, // Front left    4
	{ -0.5f, 0.8f, 0.0f }		};   	// Draw the Pyramid


	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, textureBackground);	

	glTranslatef(0,-2.0, 0.0);
	glScaled(5.0f, 5.0f, 1.0);

	glBegin(GL_QUADS);
		m3dFindNormal(vNormal, vCorners[0], vCorners[4], vCorners[3]);
		glNormal3fv(vNormal);
		glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
		glVertex3fv(vCorners[0]);
		glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
		glVertex3fv(vCorners[5]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(vCorners[4]);
		glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
		glVertex3fv(vCorners[3]);
	glEnd();
	glPopMatrix();


	if(bPressed)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_CONSTANT_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
		glBlendColor(1.0,1.0,1.0,1.0);

		M3DVector3f vSlider[4] = { { 0.5f, 1.00f, 0.5f },
		{ -0.5f, 1.0f, 0.5f },
		{ -0.5f, 0.9f, 0.5f },
		{ 0.5f, 0.9f, 0.5f }};

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureTOP[4]);
		glTranslatef(0,-2.0, 0.0);
		glScaled(5.0f, 4.0f, 1.0f);

		glBegin(GL_QUADS);
		m3dFindNormal(vNormal, vSlider[0], vSlider[1], vSlider[2]);
		glNormal3fv(vNormal);
		glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
		glVertex3fv(vSlider[0]);
		glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
		glVertex3fv(vSlider[1]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(vSlider[2]);
		glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
		glVertex3fv(vSlider[3]);
		glEnd();

		glPopMatrix();

		{
			M3DVector3f vLefttopIcon[4] = { { 0.25f, 0.98f, 0.55f },
			{ 0.20f, 0.98f, 0.55f },
			{ 0.20f, 0.91f, 0.55f },
			{ 0.25f, 0.91f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureTOP[0]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vLefttopIcon[0], vLefttopIcon[1], vLefttopIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vLefttopIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vLefttopIcon[3]);
			glEnd();

			glPopMatrix();

		}
		{
			M3DVector3f vLefttopIcon[4] = { { 0.3f, 0.98f, 0.55f },
			{ 0.25f, 0.98f, 0.55f },
			{ 0.25f, 0.91f, 0.55f },
			{ 0.3f, 0.91f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureTOP[1]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vLefttopIcon[0], vLefttopIcon[1], vLefttopIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vLefttopIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vLefttopIcon[3]);
			glEnd();

			glPopMatrix();


		}
		{
			M3DVector3f vLefttopIcon[4] = { { 0.35f, 0.98f, 0.55f },
			{ 0.3f, 0.98f, 0.55f },
			{ 0.3f, 0.91f, 0.55f },
			{ 0.35f, 0.91f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureTOP[2]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vLefttopIcon[0], vLefttopIcon[1], vLefttopIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vLefttopIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vLefttopIcon[3]);
			glEnd();

			glPopMatrix();


		}		
		{
			M3DVector3f vLefttopIcon[4] = { { 0.4f, 0.98f, 0.55f },
			{ 0.35f, 0.98f, 0.55f },
			{ 0.35f, 0.91f, 0.55f },
			{ 0.4f, 0.91f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, textureTOP[3]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vLefttopIcon[0], vLefttopIcon[1], vLefttopIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vLefttopIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vLefttopIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vLefttopIcon[3]);
			glEnd();

			glPopMatrix();


		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		glBlendColor(1.0,1.0,1.0,0.3);

		{
			M3DVector3f vBottomIcon[4] = { { 0.4f, 0.2f, 0.55f },
			{ -0.4f, 0.2f, 0.55f },
			{ -0.4f, 0.15f, 0.55f },
			{ 0.4f, 0.15f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturebottom[0]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vBottomIcon[0], vBottomIcon[1], vBottomIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vBottomIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vBottomIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vBottomIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vBottomIcon[3]);
			glEnd();

			glPopMatrix();


		}

		{
			M3DVector3f vBottomIcon[4] = { { 0.4f, 0.15f, 0.55f },
			{ -0.4f, 0.15f, 0.55f },
			{ -0.4f, 0.1f, 0.55f },
			{ 0.4f, 0.1f, 0.55f }};
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, texturebottom[1]);
			glTranslatef(0,-2.0, 0.0);
			glScaled(5.0f, 4.0f, 1.0f);

			glBegin(GL_QUADS);
			m3dFindNormal(vNormal, vBottomIcon[0], vBottomIcon[1], vBottomIcon[2]);
			glNormal3fv(vNormal);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, (GLfloat)1.0f);
			glVertex3fv(vBottomIcon[0]);
			glTexCoord2f(0.0f, (GLfloat)(GLfloat)1.0f);
			glVertex3fv(vBottomIcon[1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(vBottomIcon[2]);
			glTexCoord2f((GLfloat)(GLfloat)1.0f, 0.0f);
			glVertex3fv(vBottomIcon[3]);
			glEnd();

			glPopMatrix();


		}

	}
	glBlendColor(1.0,1.0,1.0,1.0);

	// Buffer swap
	glutSwapBuffers();

}

void callback(int val)
{
	glutPostRedisplay();
	glutTimerFunc(20,callback,0);
	return;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("UI DEMO");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(processMouse);
	glutTimerFunc(20,callback,0);
	SetupRC();
	glutMainLoop();

	return 0;
}
