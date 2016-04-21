#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.

#include "Boat.h"

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = LPCTSTR("GL Template");
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = -90.0f;
static GLfloat yRot = 0.0f;
static GLfloat xTrans, yTrans, zTrans;

//boat constants
GLfloat navigation[3][100];
GLfloat navAngle[100];

int time = 0;

static GLsizei lastHeight;
static GLsizei lastWidth;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

//size
static GLfloat nRange = 3000.0f;

// Declaration for Window procedure
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);



// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector		
	length = (float)sqrt((vector[0] * vector[0]) +
		(vector[1] * vector[1]) +
		(vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);
}

void drawCuboid(GLfloat xyz[6])
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{

		for (int j = 0; j < 2; j++)
			for (int i = 0; i < 3; i++)
			{
				int foox = (i == 0) * j;
				int fooy = (i == 1) * j + 2;
				int fooz = (i == 2) * j + 4;
				glBegin(GL_POLYGON);
				glVertex3f(xyz[foox], xyz[fooy], xyz[fooz]);
				glVertex3f(xyz[foox + (i != 0)], xyz[fooy + (i == 0)], xyz[fooz]);
				glVertex3f(xyz[foox + (i != 0)], xyz[fooy + (i != 1)], xyz[fooz + (i != 2)]);
				glVertex3f(xyz[foox], xyz[fooy + (i == 2)], xyz[fooz + (i != 2)]);
				glEnd();
			}
	}
}

void drawTriangle(float *v1, float *v2, float *v3)
{
	glBegin(GL_TRIANGLES);
	glNormal3fv(v1); glVertex3fv(v1);
	glNormal3fv(v2); glVertex3fv(v2);
	glNormal3fv(v3); glVertex3fv(v3);
	glEnd();
}

//rysuje akwen
void akwen(void)
{
	glColor3f(0.084f, 0.648f, 0.8f);
	glRectf(-5500, -5500, 5500, 5500);
}

void setPath()
{
	const float deltaAlpha = 0.0827, deltaL = 32.56, length = 2000.0, height = 800,
		radius = 400;
	float alpha = -GL_PI / 2;
	for (int i = 0; i < 100; i++)
	{
		navigation[2][i] = 0;
		navAngle[i] = 0;
	}
	for (int i = 0; i <= 62; i++)
	{
		navigation[0][i] = i * deltaL;
		navigation[1][i] = 0;
	}
	for (int i = 63; i < 99; i++)
	{
		navigation[0][i] = length + radius * cos(alpha);
		navigation[1][i] = radius + radius * sin(alpha);
		alpha += deltaAlpha;
		navAngle[i] = alpha + GL_PI / 2;
	}
	navigation[0][99] = 2000;
	navigation[1][99] = 800;
}

void marina(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		//Kolor portu
		GLfloat portr, portg, portb;

		portr = 0.5f;
		portg = 0.5f;
		portb = 0.5f;


		// Parametry wierzcholkow

		//s0 - segment 0

		//s1
		GLfloat m01[3] = { -2900.0f, 0.0f, -770.0f };
		GLfloat m02[3] = { -2400.0f, -1270.0f, 0.0f };
		GLfloat m03[3] = { -2240.0f, -1270.0f, 0.0f };

		GLfloat m46_1[3] = { -2240.0f, -2900.0f, 0.0f };
		GLfloat m46[3] = { -2900.0f, -2900.0f, 0.0f };

		//s2
		GLfloat m06_1[3] = { -2900.0f, -1570.0f, 0.0f };
		//GLfloat m03[3] = { -2240.0f,0.0f,-1270.0f };

		GLfloat m04[3] = { -2190.0f, -1170.0f, 0.0f };
		GLfloat m05[3] = { -1990.0f, -1370.0f, 0.0f };

		GLfloat m06[3] = { -2190.0f, -1570.0f, 0.0f };

		//s3
		//GLfloat m46_1[3] = { -2900.0f,0.0f,-2900.0f };
		//GLfloat m06_1[3] = { -2900.0f,0.0f,-1570.0f };

		//GLfloat m06[3] = { -2190.0f,0.0f,-1570.0f };
		GLfloat m07[3] = { -1990.0f, -1770.0f, 0.0f };
		GLfloat m08[3] = { -1390.0f, -2370.0f, 0.0f };

		GLfloat m46_2[3] = { -1390.0f, -2900.0f, 0.0f };

		//s4
		//GLfloat m46_2[3] = { -1390.0f,0.0f,-2900.0f };

		GLfloat m09[3] = { -190.0f, -1970.0f, 0.0f };
		GLfloat m10[3] = { -220.0f, -770.0f, 0.0f };

		GLfloat m46_3[3] = { -190.0f, -2900.0f, 0.0f };

		//s5
		//GLfloat m46_3[3] = { -190.0f,0.0f,-2900.0f };
		//GLfloat m09[3] = { -190.0f,0.0f,-1970.0f };
		GLfloat m15[3] = { -90.0f, -1400.0f, 0.0f };
		GLfloat m46_4[3] = { -90.0f, -2900.0f, 0.0f };

		//s6
		//GLfloat m09[3] = { -190.0f,0.0f,-1970.0f };
		//GLfloat m10[3] = { -220.0f,0.0f,-770.0f };
		GLfloat m14[3] = { -105.0f, -800.0f, 0.0f };
		//GLfloat m15[3] = { -90.0f,0.0f,-1400.0f };

		//s7
		//GLfloat m10[3] = { -220.0f,0.0f,-770.0f };

		GLfloat m11[3] = { -130.0f, -650.0f, 0.0f };
		GLfloat m12[3] = { 800.0f, -450.0f, 0.0f };
		GLfloat m13[3] = { 800.0f, -550.0f, 0.0f };
		//GLfloat m14[3] = { -105.0f,0.0f,-800.0f };

		//s8
		//GLfloat m46_4[3] = { -90.0f,0.0f,-2900.0f };
		//GLfloat m15[3] = { -90.0f,0.0f,-1400.0f };

		GLfloat m16[3] = { 1110.0f, -1350.0f, 0.0f };

		GLfloat m46_5[3] = { 1110.0f, -2900.0f, 0.0f };

		//s9
		//GLfloat m46_5[3] = { 1110.0f,0.0f,-2900.0f };
		//GLfloat m16[3] = { 1110.0f,0.0f,-1350.0f };

		GLfloat m17[3] = { 1110.0f, -1300.0f, 0.0f };
		GLfloat m18[3] = { 1410.0f, -1300.0f, 0.0f };

		GLfloat m46_6[3] = { 1410.0f, -2900.0f, 0.0f };

		//s10
		//GLfloat m46_6[3] = { 1410.0f,0.0f,-2900.0f };
		//GLfloat m18[3] = { 1410.0f,0.0f,-1300.0f };

		GLfloat m19[3] = { 1410.0f, -1000.0f, 0.0f };
		GLfloat m20[3] = { 1700.0f, -1000.0f, 0.0f };
		GLfloat m21[3] = { 1700.0f, -1050.0f, 0.0f };

		GLfloat m46_7[3] = { 1700.0f, -2900.0f, 0.0f };

		//s11
		//GLfloat m46_7[3] = { 1700.0f,0.0f,-2900.0f };
		//GLfloat m21[3] = { 1700.0f,0.0f,-1050.0f };

		GLfloat m22[3] = { 2000.0f, -1250.0f, 0.0f };
		GLfloat m23[3] = { 2200.0f, -1550.0f, 0.0f };
		GLfloat m24[3] = { 2500.0f, -1700.0f, 0.0f };

		GLfloat m46_8[3] = { 2500.0f, -2900.0f, 0.0f };

		//s12
		//GLfloat m46_8[3] = { 2500.0f,0.0f,-2900.0f };
		//GLfloat m24[3] = { 2500.0f,0.0f,-1700.0f };

		GLfloat m25[3] = { 2800.0f, -1600.0f, 0.0f };
		GLfloat m26[3] = { 4000.0f, -1900.0f, 0.0f };
		GLfloat m27[3] = { 3850.0f, -2000.0f, 0.0f };

		GLfloat m46_9[3] = { 3850.0f, -2900.0f, 0.0f };

		//s13
		//GLfloat m46_9[3] = { 3850.0f,0.0f,-2900.0f };
		//GLfloat m27[3] = { 3850.0f,0.0f,-2000.0f };

		GLfloat m28[3] = { 4150.0f, -2250.0f, 0.0f };
		GLfloat m29[3] = { 4300.0f, -2300.0f, 0.0f };

		GLfloat m46_10[3] = { 4300.0f, -2900.0f, 0.0f };

		//s14
		//GLfloat m46_10[3] = { 4300.0f,0.0f,-2900.0f };
		//GLfloat m29[3] = { 4300.0f,0.0f,-2300.0f };

		GLfloat m30[3] = { 4600.0f, -2050.0f, 0.0f };

		GLfloat m46_11[3] = { 4600.0f, -2900.0f, 0.0f };

		//s15
		//GLfloat m46_11[3] = { 4600.0f,0.0f,-2900.0f };
		//GLfloat m30[3] = { 4600.0f,0.0f,-2050.0f };

		GLfloat m45_1[3] = { 5500.0f, -2050.0f, 0.0f };
		GLfloat m45[3] = { 5500.0f, -2900.0f, 0.0f };

		//s16
		//GLfloat m45_1[3] = { 5500.0f,0.0f,-2050.0f };
		//GLfloat m30[3] = { 4600.0f,0.0f,-2050.0f };

		GLfloat m31[3] = { 4200.0f, -1900.0f, 0.0f };
		GLfloat m32[3] = { 4300.0f, -1550.0f, 0.0f };

		GLfloat m45_2[3] = { 5500.0f, -1550.0f, 0.0f };

		//s17
		//GLfloat m45_2[3] = { 5500.0f,0.0f,-1550.0f };
		//GLfloat m32[3] = { 4300.0f,0.0f,-1550.0f };

		GLfloat m33[3] = { 4150.0f, -1200.0f, 0.0f };
		GLfloat m34[3] = { 4000.0f, -950.0f, 0.0f };
		GLfloat m35[3] = { 4150.0f, -500.0f, 0.0f };

		GLfloat m45_3[3] = { 5500.0f, -500.0f, 0.0f };

		//s18
		//GLfloat m45_3[3] = { 5500.0f,0.0f,-500.0f };
		//GLfloat m35[3] = { 4150.0f,0.0f,-500.0f };

		GLfloat m36[3] = { 4600.0f, 0.0f, 0.0f };
		GLfloat m45_4[3] = { 5500.0f, 0.0f, 0.0f };

		//s19
		//GLfloat m45_4[3] = { 5500.0f,0.0f,0.0f };
		//GLfloat m36[3] = { 4600.0f,0.0f,0.0f };

		GLfloat m41[3] = { 4750.0f, 800.0f, 0.0f };
		GLfloat m42[3] = { 4900.0f, 900.0f, 0.0f };
		GLfloat m45_5[3] = { 5500.0f, 900.0f, 0.0f };

		//s20
		//GLfloat m36[3] = { 4600.0f,0.0f,0.0f };

		GLfloat m37[3] = { 4150.0f, 600.0f, 0.0f };
		GLfloat m38[3] = { 4000.0f, 800.0f, 0.0f };
		GLfloat m39[3] = { 4200.0f, 1100.0f, 0.0f };
		GLfloat m40[3] = { 4350.0f, 1100.0f, 0.0f };

		//GLfloat m41[3] = { 4750.0f,0.0f,800.0f };

		//21
		//GLfloat m45_5[3] = { 5500.0f,0.0f,900.0f };
		//GLfloat m42[3] = { 4900.0f,0.0f,900.0f };

		GLfloat m43[3] = { 5150.0f, 1500.0f, 0.0f };
		GLfloat m44[3] = { 5500.0f, 2000.0f, 0.0f };

		//GLfloat m45[3] = { 5500.0f,0.0f,-2900.0f };

		//GLfloat m46[3] = { -2900.0f,0.0f,-2900.0f };


		//Góra mariny

		GLfloat mg01[3] = { -2900.0f, -770.0f, 10.0f };
		GLfloat mg02[3] = { -2400.0f, -1270.0f, 10.0f };
		GLfloat mg03[3] = { -2240.0f, -1270.0f, 10.0f };
		GLfloat mg46_1[3] = { -2240.0f, -2900.0f, 10.0f };
		GLfloat mg46[3] = { -2900.0f, -2900.0f, 10.0f };
		GLfloat mg06_1[3] = { -2900.0f, -1570.0f, 10.0f };
		GLfloat mg04[3] = { -2190.0f, -1170.0f, 10.0f };
		GLfloat mg05[3] = { -1990.0f, -1370.0f, 10.0f };
		GLfloat mg06[3] = { -2190.0f, -1570.0f, 10.0f };
		GLfloat mg07[3] = { -1990.0f, -1770.0f, 10.0f };
		GLfloat mg08[3] = { -1390.0f, -2370.0f, 10.0f };
		GLfloat mg46_2[3] = { -1390.0f, -2900.0f, 10.0f };
		GLfloat mg09[3] = { -190.0f, -1970.0f, 10.0f };
		GLfloat mg10[3] = { -220.0f, -770.0f, 10.0f };
		GLfloat mg46_3[3] = { -190.0f, -2900.0f, 10.0f };
		GLfloat mg15[3] = { -90.0f, -1400.0f, 10.0f };
		GLfloat mg46_4[3] = { -90.0f, -2900.0f, 10.0f };
		GLfloat mg14[3] = { -105.0f, -800.0f, 10.0f };
		GLfloat mg11[3] = { -130.0f, -650.0f, 10.0f };
		GLfloat mg12[3] = { 800.0f, -450.0f, 10.0f };
		GLfloat mg13[3] = { 800.0f, -550.0f, 10.0f };
		GLfloat mg16[3] = { 1110.0f, -1350.0f, 10.0f };
		GLfloat mg46_5[3] = { 1110.0f, -2900.0f, 10.0f };
		GLfloat mg17[3] = { 1110.0f, -1300.0f, 10.0f };
		GLfloat mg18[3] = { 1410.0f, -1300.0f, 10.0f };
		GLfloat mg46_6[3] = { 1410.0f, -2900.0f, 10.0f };
		GLfloat mg19[3] = { 1410.0f, -1000.0f, 10.0f };
		GLfloat mg20[3] = { 1700.0f, -1000.0f, 10.0f };
		GLfloat mg21[3] = { 1700.0f, -1050.0f, 10.0f };
		GLfloat mg46_7[3] = { 1700.0f, -2900.0f, 10.0f };
		GLfloat mg22[3] = { 2000.0f, -1250.0f, 10.0f };
		GLfloat mg23[3] = { 2200.0f, -1550.0f, 10.0f };
		GLfloat mg24[3] = { 2500.0f, -1700.0f, 10.0f };
		GLfloat mg46_8[3] = { 2500.0f, -2900.0f, 10.0f };
		GLfloat mg25[3] = { 2800.0f, -1600.0f, 10.0f };
		GLfloat mg26[3] = { 4000.0f, -1900.0f, 10.0f };
		GLfloat mg27[3] = { 3850.0f, -2000.0f, 10.0f };
		GLfloat mg46_9[3] = { 3850.0f, -2900.0f, 10.0f };
		GLfloat mg28[3] = { 4150.0f, -2250.0f, 10.0f };
		GLfloat mg29[3] = { 4300.0f, -2300.0f, 10.0f };
		GLfloat mg46_10[3] = { 4300.0f, -2900.0f, 10.0f };
		GLfloat mg30[3] = { 4600.0f, -2050.0f, 10.0f };
		GLfloat mg46_11[3] = { 4600.0f, -2900.0f, 10.0f };
		GLfloat mg45_1[3] = { 5500.0f, -2050.0f, 10.0f };
		GLfloat mg45[3] = { 5500.0f, -2900.0f, 10.0f };
		GLfloat mg31[3] = { 4200.0f, -1900.0f, 10.0f };
		GLfloat mg32[3] = { 4300.0f, -1550.0f, 10.0f };
		GLfloat mg45_2[3] = { 5500.0f, -1550.0f, 10.0f };
		GLfloat mg33[3] = { 4150.0f, -1200.0f, 10.0f };
		GLfloat mg34[3] = { 4000.0f, -950.0f, 10.0f };
		GLfloat mg35[3] = { 4150.0f, -500.0f, 10.0f };
		GLfloat mg45_3[3] = { 5500.0f, -500.0f, 10.0f };
		GLfloat mg36[3] = { 4600.0f, 0.0f, 10.0f };
		GLfloat mg45_4[3] = { 5500.0f, 0.0f, 10.0f };
		GLfloat mg41[3] = { 4750.0f, 800.0f, 10.0f };
		GLfloat mg42[3] = { 4900.0f, 900.0f, 10.0f };
		GLfloat mg45_5[3] = { 5500.0f, 900.0f, 10.0f };
		GLfloat mg37[3] = { 4150.0f, 600.0f, 10.0f };
		GLfloat mg38[3] = { 4000.0f, 800.0f, 10.0f };
		GLfloat mg39[3] = { 4200.0f, 1100.0f, 10.0f };
		GLfloat mg40[3] = { 4350.0f, 1100.0f, 10.0f };
		GLfloat mg43[3] = { 5150.0f, 1500.0f, 10.0f };
		GLfloat mg44[3] = { 5500.0f, 2000.0f, 10.0f };


		// Sciany skladowe

		//s1
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m01);
		glVertex3fv(m02);
		glVertex3fv(m03);
		glVertex3fv(m46);
		glVertex3fv(m46_1);
		glEnd();

		//s2
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m06_1);
		glVertex3fv(m03);
		glVertex3fv(m04);
		glVertex3fv(m05);
		glVertex3fv(m06);
		glEnd();

		//s3
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_1);
		glVertex3fv(m06_1);
		glVertex3fv(m06);
		glVertex3fv(m07);
		glVertex3fv(m08);
		glVertex3fv(m46_2);
		glEnd();

		//s4
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_2);
		glVertex3fv(m08);
		glVertex3fv(m09);
		glVertex3fv(m46_3);
		glEnd();

		//s5
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_3);
		glVertex3fv(m09);
		glVertex3fv(m15);
		glVertex3fv(m46_4);
		glEnd();

		//s6
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m09);
		glVertex3fv(m10);
		glVertex3fv(m14);
		glVertex3fv(m15);
		glEnd();

		//s7
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m10);
		glVertex3fv(m11);
		glVertex3fv(m12);
		glVertex3fv(m13);
		glVertex3fv(m14);
		glEnd();

		//s8
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_4);
		glVertex3fv(m15);
		glVertex3fv(m16);
		glVertex3fv(m46_5);
		glEnd();

		//s9
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_5);
		glVertex3fv(m16);
		glVertex3fv(m17);
		glVertex3fv(m18);
		glVertex3fv(m46_6);
		glEnd();

		//s10
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_6);
		glVertex3fv(m18);
		glVertex3fv(m19);
		glVertex3fv(m20);
		glVertex3fv(m21);
		glVertex3fv(m46_7);
		glEnd();

		//s11
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_7);
		glVertex3fv(m21);
		glVertex3fv(m22);
		glVertex3fv(m23);
		glVertex3fv(m24);
		glVertex3fv(m46_8);
		glEnd();

		//s12
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_8);
		glVertex3fv(m24);
		glVertex3fv(m25);
		glVertex3fv(m26);
		glVertex3fv(m27);
		glVertex3fv(m46_9);
		glEnd();

		//s13
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_9);
		glVertex3fv(m27);
		glVertex3fv(m28);
		glVertex3fv(m29);
		glVertex3fv(m46_10);
		glEnd();

		//s14
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_10);
		glVertex3fv(m29);
		glVertex3fv(m30);
		glVertex3fv(m46_11);
		glEnd();

		//s15
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m46_11);
		glVertex3fv(m30);
		glVertex3fv(m45_1);
		glVertex3fv(m45);
		glEnd();

		//s16
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m45_1);
		glVertex3fv(m30);
		glVertex3fv(m31);
		glVertex3fv(m32);
		glVertex3fv(m45_2);
		glEnd();

		//s17
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m45_2);
		glVertex3fv(m32);
		glVertex3fv(m33);
		glVertex3fv(m34);
		glVertex3fv(m35);
		glVertex3fv(m45_3);
		glEnd();

		//s18
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m45_3);
		glVertex3fv(m35);
		glVertex3fv(m36);
		glVertex3fv(m45_4);
		glEnd();

		//s19
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m45_4);
		glVertex3fv(m36);
		glVertex3fv(m41);
		glVertex3fv(m42);
		glVertex3fv(m45_5);
		glEnd();

		//s20
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m36);
		glVertex3fv(m37);
		glVertex3fv(m38);
		glVertex3fv(m39);
		glVertex3fv(m40);
		glVertex3fv(m41);
		glEnd();

		//s21
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(m45_5);
		glVertex3fv(m42);
		glVertex3fv(m43);
		glVertex3fv(m44);
		glEnd();

		//Œciany góra

		//s1
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg01);
		glVertex3fv(mg02);
		glVertex3fv(mg03);
		glVertex3fv(mg46);
		glVertex3fv(mg46_1);
		glEnd();

		//s2
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg06_1);
		glVertex3fv(mg03);
		glVertex3fv(mg04);
		glVertex3fv(mg05);
		glVertex3fv(mg06);
		glEnd();

		//s3
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_1);
		glVertex3fv(mg06_1);
		glVertex3fv(mg06);
		glVertex3fv(mg07);
		glVertex3fv(mg08);
		glVertex3fv(mg46_2);
		glEnd();

		//Port

		//s4
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_2);
		glVertex3fv(mg08);
		glVertex3fv(mg09);
		glVertex3fv(mg46_3);
		glEnd();

		//s5
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_3);
		glVertex3fv(mg09);
		glVertex3fv(mg15);
		glVertex3fv(mg46_4);
		glEnd();

		//s6
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg09);
		glVertex3fv(mg10);
		glVertex3fv(mg14);
		glVertex3fv(mg15);
		glEnd();

		//s7
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg10);
		glVertex3fv(mg11);
		glVertex3fv(mg12);
		glVertex3fv(mg13);
		glVertex3fv(mg14);
		glEnd();

		//s8
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_4);
		glVertex3fv(mg15);
		glVertex3fv(mg16);
		glVertex3fv(mg46_5);
		glEnd();

		//s9
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_5);
		glVertex3fv(mg16);
		glVertex3fv(mg17);
		glVertex3fv(mg18);
		glVertex3fv(mg46_6);
		glEnd();

		//s10
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_6);
		glVertex3fv(mg18);
		glVertex3fv(mg19);
		glVertex3fv(mg20);
		glVertex3fv(mg21);
		glVertex3fv(mg46_7);
		glEnd();

		//s11
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_7);
		glVertex3fv(mg21);
		glVertex3fv(mg22);
		glVertex3fv(mg23);
		glVertex3fv(mg24);
		glVertex3fv(mg46_8);
		glEnd();

		//s12
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_8);
		glVertex3fv(mg24);
		glVertex3fv(mg25);
		glVertex3fv(mg26);
		glVertex3fv(mg27);
		glVertex3fv(mg46_9);
		glEnd();

		//Port - Koniec

		//s13
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_9);
		glVertex3fv(mg27);
		glVertex3fv(mg28);
		glVertex3fv(mg29);
		glVertex3fv(mg46_10);
		glEnd();

		//s14
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_10);
		glVertex3fv(mg29);
		glVertex3fv(mg30);
		glVertex3fv(mg46_11);
		glEnd();

		//s15
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_11);
		glVertex3fv(mg30);
		glVertex3fv(mg45_1);
		glVertex3fv(mg45);
		glEnd();

		//s16
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_1);
		glVertex3fv(mg30);
		glVertex3fv(mg31);
		glVertex3fv(mg32);
		glVertex3fv(mg45_2);
		glEnd();

		//s17
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_2);
		glVertex3fv(mg32);
		glVertex3fv(mg33);
		glVertex3fv(mg34);
		glVertex3fv(mg35);
		glVertex3fv(mg45_3);
		glEnd();

		//s18
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_3);
		glVertex3fv(mg35);
		glVertex3fv(mg36);
		glVertex3fv(mg45_4);
		glEnd();

		//s19
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_4);
		glVertex3fv(mg36);
		glVertex3fv(mg41);
		glVertex3fv(mg42);
		glVertex3fv(mg45_5);
		glEnd();

		//s20
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg36);
		glVertex3fv(mg37);
		glVertex3fv(mg38);
		glVertex3fv(mg39);
		glVertex3fv(mg40);
		glVertex3fv(mg41);
		glEnd();

		//s21
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_5);
		glVertex3fv(mg42);
		glVertex3fv(mg43);
		glVertex3fv(mg44);
		glEnd();

		//Œciany boczne

		//Port segmenty s4 - s12

		//s4
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m08);
		glVertex3fv(mg08);
		glVertex3fv(mg09);
		glVertex3fv(m09);
		glEnd();

		//s6
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m09);
		glVertex3fv(mg09);
		glVertex3fv(mg10);
		glVertex3fv(m10);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m14);
		glVertex3fv(mg14);
		glVertex3fv(mg15);
		glVertex3fv(m15);
		glEnd();

		//s7
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m10);
		glVertex3fv(mg10);
		glVertex3fv(mg11);
		glVertex3fv(m11);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m11);
		glVertex3fv(mg11);
		glVertex3fv(mg12);
		glVertex3fv(m12);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m12);
		glVertex3fv(mg12);
		glVertex3fv(mg11);
		glVertex3fv(m13);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m13);
		glVertex3fv(mg13);
		glVertex3fv(mg14);
		glVertex3fv(m14);
		glEnd();

		//s8
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m15);
		glVertex3fv(mg15);
		glVertex3fv(mg16);
		glVertex3fv(m16);
		glEnd();

		//s9
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m16);
		glVertex3fv(mg16);
		glVertex3fv(mg17);
		glVertex3fv(m17);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m17);
		glVertex3fv(mg17);
		glVertex3fv(mg18);
		glVertex3fv(m18);
		glEnd();

		//s10
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m18);
		glVertex3fv(mg18);
		glVertex3fv(mg19);
		glVertex3fv(m19);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m19);
		glVertex3fv(mg19);
		glVertex3fv(mg20);
		glVertex3fv(m20);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m20);
		glVertex3fv(mg20);
		glVertex3fv(mg21);
		glVertex3fv(m21);
		glEnd();

		//s11
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m21);
		glVertex3fv(mg21);
		glVertex3fv(mg22);
		glVertex3fv(m22);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m22);
		glVertex3fv(mg22);
		glVertex3fv(mg23);
		glVertex3fv(m23);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m23);
		glVertex3fv(mg23);
		glVertex3fv(mg24);
		glVertex3fv(m24);
		glEnd();

		//s12
		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m24);
		glVertex3fv(mg24);
		glVertex3fv(mg25);
		glVertex3fv(m25);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m25);
		glVertex3fv(mg25);
		glVertex3fv(mg26);
		glVertex3fv(m26);
		glEnd();

		glColor3f(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(m26);
		glVertex3fv(mg26);
		glVertex3fv(mg27);
		glVertex3fv(m27);
		glEnd();
	}
}

void yacht(float navigation[3][100], int i)
{
	Boat yacht;
	yacht.setPosition(0.0, 0.0, 0.0);

	glPushMatrix();
	
	glTranslatef(navigation[0][i], navigation[1][i], navigation[2][i]);
	glRotatef(navAngle[i] * 180 / GL_PI, 0.0, 0.0, 1.0);
	yacht.renderAll();
	glPopMatrix();

}

void swimming(float navigation[3][100])
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100; i++)
		glVertex3f(navigation[0][i], navigation[1][i], navigation[2][i]);
	glEnd();
}

// LoadBitmapFile
// opis: ³aduje mapê bitow¹ z pliku i zwraca jej adres.
//       Wype³nia strukturê nag³ówka.
//	 Nie obs³uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaŸnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag³ówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag³ówek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaŸnik pozycji pliku na pocz¹tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamiêæ buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda³o siê przydzieliæ pamiêæ
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta³y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk³adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaŸnik bufora zawieraj¹cego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}


// Called to draw scene
void RenderScene(void)
{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(xTrans, yTrans, zTrans);

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////

	//Sposób na odróŸnienie "przedniej" i "tylniej" œciany wielok¹ta:
	glPolygonMode(GL_BACK, GL_LINE);

	//Uzyskanie siatki:
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//Rysowanie obiektów:

	/*
	kadlub();
	dziob();
	rufa();
	maszt(80.0f, 10.0f);
	
	zagiel(80.0f, 10.0f);
	
	*/

	//Boat yacht;
	//yacht.setPosition(0.0, 0.0, 0.0);
	//yacht.renderAll();

	akwen();
	marina();

	//fill navigation array with coordinates of boat swimming
	setPath();

	swimming(navigation);
	yacht(navigation,time);

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();
}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0, 0, 0, 0, 0, 0,                            // Not used to select mode
		0, 0,                                    // Not used to select mode
		0, 0, 0, 0, 0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0, 0, 0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange, GreenRange, BlueRange;
	// Range for each color entry (7,7,and 3)


	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if (!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE)+nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;

	// Loop through all the palette entries
	for (i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}


	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}


// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;

	// Register the window class
	if (RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
		lpszAppName,
		TEXT("Boat"),

		// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

		// Window position and size
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// If window was not created, quit
	if (hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(lpszAppName, hInstance);

	return msg.wParam;
}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam)
{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	UINT_PTR TimerID=NULL;

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);

		//set timer for time-out 70ms
		SetTimer(hWnd, TimerID, 70, NULL);
		
		// Select the pixel format
		SetDCPixelFormat(hDC);

		// Create palette if needed
		hPalette = GetOpenGLPalette(hDC);

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

		// ³aduje pierwszy obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

		glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// ³aduje drugi obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// ustalenie sposobu mieszania tekstury z t³em
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);

		// Tell the application to terminate after the window
		// is gone.
		PostQuitMessage(0);
		KillTimer(hWnd, TimerID);
		break;

		// Window is resized.
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT:
	{
					 // Call OpenGL drawing code
					 RenderScene();

					 SwapBuffers(hDC);

					 // Validate the newly painted client area
					 ValidateRect(hWnd, NULL);
	}
		break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette)
		{
			int nRet;

			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);

			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		}
		break;


		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd))
		{
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries to system palette
			RealizePalette(hDC);

			// Remap the current colors to the newly realized palette
			UpdateColors(hDC);
			return 0;
		}
		break;

		// Key press, check for arrow keys to do cube rotation.
	case WM_KEYDOWN:
	{
					   if (wParam == VK_UP || wParam == VK_NUMPAD8)
						   xRot -= 5.0f;

					   if (wParam == VK_DOWN || wParam == VK_NUMPAD2)
						   xRot += 5.0f;

					   if (wParam == VK_LEFT || wParam == VK_NUMPAD4)
						   yRot -= 5.0f;

					   if (wParam == VK_RIGHT || wParam == VK_NUMPAD6)
						   yRot += 5.0f;

					   if (wParam == 0x41 + 'd' - 'a')
						   xTrans += 10.0f;
					   if (wParam == 0x41)
						   xTrans -= 10.0f;
					   if (wParam == 0x41 + 'w' - 'a')
						   yTrans += 10.0f;
					   if (wParam == 0x41 + 's' - 'a')
						   yTrans -= 10.0f;

					   if (wParam == VK_ADD)
					   {
						   if (nRange > 100)
								nRange -= 100.0;
						   RECT rc;
						   GetWindowRect(hWnd, &rc);
						   GLsizei w = rc.right - rc.left, h = rc.top - rc.bottom;
							
						   // Call our function which modifies the clipping
						   // volume and viewport
						   ChangeSize(w, h);
						   InvalidateRect(hWnd, NULL, FALSE);
						   break;
					   }
					   if (wParam == VK_SUBTRACT)
					   {							
								nRange += 100;
							RECT rc;
							GetWindowRect(hWnd, &rc);
							GLsizei w = rc.right - rc.left, h = rc.top - rc.bottom;

							// Call our function which modifies the clipping
							// volume and viewport
							ChangeSize(w, h);
							InvalidateRect(hWnd, NULL, FALSE);
							break;
					   }

					   xRot = (const int)xRot % 360;
					   yRot = (const int)yRot % 360;

					   InvalidateRect(hWnd, NULL, FALSE);
	}
		break;

		// A menu command
	case WM_COMMAND:
	{
					   switch (LOWORD(wParam))
					   {
						   // Exit the program
					   case ID_FILE_EXIT:
						   DestroyWindow(hWnd);
						   break;

						   // Display the about box
					   case ID_HELP_ABOUT:
						   /*DialogBox(hInstance,
							   MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
							   hWnd),
							   (lpDialogFunc)AboutDlgProc);*/
						   break;
					   }
	}
		break;

	case WM_TIMER:
	{
		//change the position of the boat
						
				if (time < 99)
					time++;
				else
					time = 0;

				InvalidateRect(hWnd, NULL, FALSE);
				break;
	}

	default:   // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, wParam, lParam));

	}

	return (0L);
}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{

	switch (message)
	{
		// Initialize the dialog box
	case WM_INITDIALOG:
	{
						  int i;
						  GLenum glError;

						  // glGetString demo
						  SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, (LPCWSTR)glGetString(GL_VENDOR));
						  SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, (LPCWSTR)glGetString(GL_RENDERER));
						  SetDlgItemText(hDlg, IDC_OPENGL_VERSION, (LPCWSTR)glGetString(GL_VERSION));
						  SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, (LPCWSTR)glGetString(GL_EXTENSIONS));

						  // gluGetString demo
						  SetDlgItemText(hDlg, IDC_GLU_VERSION, (LPCWSTR)gluGetString(GLU_VERSION));
						  SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, (LPCWSTR)gluGetString(GLU_EXTENSIONS));


						  // Display any recent error messages
						  i = 0;
						  do {
							  glError = glGetError();
							  SetDlgItemText(hDlg, IDC_ERROR1 + i, (LPCWSTR)gluErrorString(glError));
							  i++;
						  } while (i < 6 && glError != GL_NO_ERROR);


						  return (TRUE);
	}
		break;

		// Process command messages
	case WM_COMMAND:
	{
					   // Validate and Make the changes
					   if (LOWORD(wParam) == IDOK)
						   EndDialog(hDlg, TRUE);
	}
		break;

		// Closed from sysbox
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	}

	return FALSE;
}