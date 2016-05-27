#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include "gl\glew.h"
#include "GL\freeglut.h"
#include "gl\gl.h"              // OpenGL
#include "gl\glu.h"             // GLU library
#define LS (LPCSTR)
#include <CommCtrl.h>

#include <AntTweakBar.h>

#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.

#include "Boat.h"
#include "Physics.h"
#include "shapeUtils.h"
#include "Marina.h"

#include <array>

#pragma region globals

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = LPCTSTR("GL Template");
static HINSTANCE hInstance;

// Rotation and translation amounts
static GLfloat xRot = -90.0f;
static GLfloat yRot = 0.0f;
static GLfloat xTrans, yTrans, zTrans;

//boat constants
std::array<GLfloat, 200> navigation[3];
GLfloat navAngle[200];
Physics balt17; //yacht physics
float windFloatFooVar[3] = { 0, 0, 0.0f }; //variable needed for AntTweakBar to work
float boatScale = 2.4; //scale factor of boat

//time constants
int time = 0; 
float deltaTime = 1;

static GLsizei lastHeight;
static GLsizei lastWidth;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned char*		waterBitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

//size
static GLfloat nRange = 3000.0f;

#pragma endregion globals

#pragma region windowsUtils
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
#pragma endregion windowsUtils


// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	// Light values and coordinates
	GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);
}

//rysuje akwen
void akwen(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor4f(0.084f, 0.648f, 0.8f, 0.8f);
		//glRectf(-5500, -5500, 5500, 5500);
		const float range = 5500;
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-range, -range, 0.0);
		glTexCoord2f(0.0, 9.0); glVertex3f(-range, range, 0.0);
		glTexCoord2f(9.0, 9.0); glVertex3f(range, range, 0.0);
		glTexCoord2f(9.0, 0.0); glVertex3f(range, -range, 0.0);
		glEnd();
	glDisable(GL_TEXTURE_2D);
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
	
	//symmetry about a centre point
	const float centrePoint[3] = { 1000, 400, 0 };
	for (int i = 100; i < 200; i++)
	{
		for (int j = 0; j < 3; j++)
			navigation[j][i] = 2*centrePoint[j] - navigation[j][i - 100];
		navAngle[i] = navAngle[i - 100];
	}
}


template<size_t n>
void yacht(std::array<GLfloat, n> navigation[3], int i)
{
	Boat yacht;
	yacht.setPosition(0.0, 0.0, 0.0);

	balt17.setForce(windFloatFooVar);
	balt17.computeNew(yacht.getMass(), deltaTime);

	glPushMatrix();

	glTranslatef(navigation[0][i] + balt17.getPos()[0],
		navigation[1][i] + balt17.getPos()[1], navigation[2][i] + balt17.getPos()[2]);
	glRotatef(navAngle[i] * 180 / GL_PI, 0.0, 0.0, 1.0);
	if (boatScale != 0.0)
	{
		yacht.renderAll(boatScale);
		yacht.renderMirror(boatScale);
	}

	else
	{
		yacht.renderAll();
		yacht.renderMirror();
	}
	glPopMatrix();
}

template<size_t n>
void swimming(std::array<GLfloat, n> navigation[3])
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++)
		glVertex3f(navigation[0][i], navigation[1][i], navigation[2][i]);
	glEnd();
}

void light()
{
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[3] {0.5f, 0.5f, -0.3f});

	GLfloat lightColor0[] = { 0.01f, 0.5f, 0.1f, 0.7f };
	GLfloat lightColor1[] = { 0.01f, 0.1f, 0.5f, 0.7f };
	GLfloat lightPos0[] = { 100.0f, 50.0f, -9.0f, 1.0f };
	GLfloat lightPos1[] = { 100.0f, 100.0f, -12.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT0);
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
	unsigned int		imageIdx = 0;		// licznik pikseli
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

	//Materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, new GLfloat[4] {1.0, 1.0, 1.0, 1.0});

	//light
	light();

	//Uzyskanie siatki:
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//Boat yacht;
	//yacht.setPosition(0.0, 0.0, 0.0);
	//yacht.renderAll();

	//Rysowanie obiektów:
	marina();

	//fill navigation array with coordinates of boat swimming
	setPath();

	swimming<200>(navigation); //draw trajectory of boat swimming
	yacht<200>(navigation, time); //swim

	glEnable(GL_BLEND);                         // Enable Blending (Otherwise The Reflected Object Wont Show)
	//glDisable(GL_LIGHTING);                         // Since We Use Blending, We Disable Lighting
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);          // Blending Based On Source Alpha And 1 Minus 

	akwen();

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


#pragma region palette
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
#pragma endregion palette

// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG             msg;            // Windows message structure
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

	//https://codingmisadventures.wordpress.com/2009/03/10/retrieving-command-line-parameters-from-winmain-in-win32/
	LPWSTR *szArgList;
	int *argCount = new int;

	szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLine(), argCount);
	if (szArgList == NULL)
	{
		MessageBox(hWnd, (LPCSTR)"Unable to parse command line", (LPCSTR)"Error", MB_OK);
		return 10;
	}
	glutInit(argCount, (char**)szArgList);
	
	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	/*GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}*/

	LocalFree(szArgList);

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

	if (TwEventWin(hWnd, message, wParam, lParam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar
	// else process the event message
	// ...

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);

		//set up physics
		balt17.setPos(new float[3] {0.0, 0.0, 0.0});
		balt17.setVel(new float[3] {0.0, 0.0, 0.0});
		balt17.setAccel(new float[3] {0.0, 0.0, 0.0});
		balt17.setForce(new float[3] { 0.0f, 0, 0.0f });

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

		// Laduje pierwszy obraz tekstury:
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

		// Laduje obraz tekstury WODA:
		waterBitmapData = LoadBitmapFile("Bitmapy\\water.bmp", &bitmapInfoHeader);

		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, waterBitmapData);

		// ustalenie sposobu mieszania tekstury z t³em
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//AntTweakBar initialization
		TwInit(TW_OPENGL, NULL);
		RECT rc;
		GetWindowRect(hWnd, &rc);
		TwWindowSize(rc.right - rc.left, rc.top - rc.bottom);
		
		//------------------------------------------
		//AntTweakBar routines
		//------------------------------------------
		TwBar *bar ;
		bar = TwNewBar("Bar");

		TwAddVarRW(bar, "nRange", TW_TYPE_FLOAT, &nRange, "label='view Range' min=50 max=4000 step=10");
		 // Add 'ka', 'kb and 'kc' to 'bar': they are modifiable variables of type TW_TYPE_DOUBLE
		TwAddVarRW(bar, "ka", TW_TYPE_FLOAT, &windFloatFooVar[0], 
				   " label='X path coord' keyIncr=1 keyDecr=CTRL+1 min=-7 max=7 step=1 ");
		TwAddVarRW(bar, "kb", TW_TYPE_FLOAT, &windFloatFooVar[1],
				   " label='Y path coord' keyIncr=2 keyDecr=CTRL+2 min=-7 max=7 step=1 ");
		TwAddVarRW(bar, "scale", TW_TYPE_FLOAT, &boatScale,
			"label='scale boat' min=0.1 max=15 step=0.1");
		//------------------------------------------


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
		TwTerminate();

		if (waterBitmapData)
			free(waterBitmapData);
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
					 TwDraw();

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

					   const float translateScene = 30.0f;
					   if (wParam == 0x41 + 'd' - 'a') //key d
						   xTrans += translateScene;
					   if (wParam == 0x41)				//key a
						   xTrans -= translateScene;
					   if (wParam == 0x41 + 'w' - 'a') //key w
						   yTrans += translateScene;
					   if (wParam == 0x41 + 's' - 'a') //key s
						   yTrans -= translateScene;

					   if (wParam == VK_ADD)
					   {
						   if (nRange > 100)
								nRange -= 100.0;
						   RECT rc;
						   GetWindowRect(hWnd, &rc);
						   GLsizei w = rc.right - rc.left, h = rc.bottom - rc.top;
							
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
							GLsizei w = rc.right - rc.left, h = rc.bottom - rc.top;

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
						   DialogBoxA(hInstance,
							   MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
							   hWnd,
							   &AboutDlgProc);
						   break;
					   }
	}
		break;

	case WM_TIMER:
	{
		//change the position of the boat
						
				if (time < navigation[0].size() - 1)
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



#pragma region dialog
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

						  SetDlgItemText(hDlg, 143, LS "Authors: Kamil Lopuszanski, Patryk Mendrala");

						  // glGetString demo
						  SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, (LPCSTR)glGetString(GL_VENDOR));
						  SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, (LPCSTR)glGetString(GL_RENDERER));
						  SetDlgItemText(hDlg, IDC_OPENGL_VERSION, (LPCSTR)glGetString(GL_VERSION));
						  SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, (LPCSTR)glGetString(GL_EXTENSIONS));

						  // gluGetString demo
						  SetDlgItemText(hDlg, IDC_GLU_VERSION, (LPCSTR)gluGetString(GLU_VERSION));
						  SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, (LPCSTR)gluGetString(GLU_EXTENSIONS));


						  // Display any recent error messages
						  i = 0;
						  do {
							  glError = glGetError();
							  SetDlgItemText(hDlg, IDC_ERROR1 + i, (LPCSTR)gluErrorString(glError));
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
#pragma endregion dialog