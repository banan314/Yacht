#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
//#include "gl\glew.h"
//#include "GL\freeglut.h"
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
#include "Tree.h"

#include "Physics\geometry.h"

#include "windowsUtilities.h"


#include <array>

#pragma region globals

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define GL_PI 3.14

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = LPCTSTR("GL Template");
static HINSTANCE hInstance;

// Rotation and translation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat xTrans, yTrans, zTrans;

//boat constants
std::array<GLfloat, 200> navigation[3];
GLfloat navAngle[200];
Physics balt17; //yacht physics
float windFloatFooVar[3] = { 0, 0, 0.0f }; //variable needed for AntTweakBar to work
float boatScale = 1.0; //scale factor of boat

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
static GLfloat nRange = 1900.0f;

//movement
static bool trajectoryVisible = true;
static bool boatMoving = true;

//cameras
typedef enum { CAMERA_YACHT = 0, CAMERA_GENERAL = 1, CAMERA_USER} eCamera;
eCamera eGlobalCamera = CAMERA_YACHT;
#define NUM_CAMERAS 3
typedef struct { public: GLdouble eye[3], center[3], up[3]; } Camera;
Camera globalCamera[NUM_CAMERAS]  //{ GLdouble eye[3] = { 0, 0, 0 }, //default
		//GLdouble center[3] = { 0, 0, -40 },		//default
		//GLdouble up[3] = { 0, 55, 0 } };			//default
;
#pragma endregion globals

//yacht object
Boat yacht;

//blender essentials
float               g_maxAnisotrophy;

//collision
bool collisionDetected = false;
GLfloat przyladekMariny = -500; //wspolrzedna y najdalej wysunietego punktu mariny

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
void yachtRender(std::array<GLfloat, n> navigation[3], int i)
{
	yacht.setPosition(0.0, 0.0, 0.0);


	balt17.setForce(windFloatFooVar);
	balt17.computeNew(yacht.getMass(), deltaTime);

	glPushMatrix();

	glTranslatef(navigation[0][i] + balt17.getPos()[0],
		navigation[1][i] + balt17.getPos()[1], navigation[2][i] + balt17.getPos()[2]);
	yacht.setAngle(navAngle[i]);
	if (boatScale != 0.0)
	{
		yacht.renderAll(boatScale);
		//yacht.renderBlender(boatScale);
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

// Called to draw scene
void RenderScene(void)
{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Save the matrix state and do the rotations
	glPushMatrix();
	glLoadIdentity();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(xTrans, yTrans, zTrans);


	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////

	////ustawienie kamery
	gluLookAt(globalCamera[eGlobalCamera].eye[0], globalCamera[eGlobalCamera].eye[1], globalCamera[eGlobalCamera].eye[2],
		globalCamera[eGlobalCamera].center[0], globalCamera[eGlobalCamera].center[1], globalCamera[eGlobalCamera].center[2],
		globalCamera[eGlobalCamera].up[0], globalCamera[eGlobalCamera].up[1], globalCamera[eGlobalCamera].up[2]
		);

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

	if (trajectoryVisible)
		swimming<200>(navigation); //draw trajectory of boat swimming
	if (boatMoving)
		yachtRender<200>(navigation, time); //swim
	else
		yachtRender<200>(navigation, 0); //if yacht is not to move, draw it on the initial position

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

	////https://codingmisadventures.wordpress.com/2009/03/10/retrieving-command-line-parameters-from-winmain-in-win32/
	//LPWSTR *szArgList;
	//int *argCount = new int;

	//szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLine(), argCount);
	//if (szArgList == NULL)
	//{
	//	MessageBox(hWnd, (LPCSTR)"Unable to parse command line", (LPCSTR)"Error", MB_OK);
	//	return 10;
	//}
	//glutInit(argCount, (char**)szArgList);
	
	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	/*GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}*/

	//LocalFree(szArgList);

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
		

		//initialize camera
		for (int i = 0; i < 3; i++)
		{
			globalCamera[CAMERA_GENERAL].eye[i] = (new GLdouble[3] { 0, 0, 0 })[i];
			globalCamera[CAMERA_GENERAL].center[i] = (new GLdouble[3] { 0, 0, -40 })[i];
			globalCamera[CAMERA_GENERAL].up[i] = (new GLdouble[3] { 0, 55, 0 })[i];
			globalCamera[CAMERA_YACHT].eye[i] = (new GLdouble[3] { 430, 20, -230 })[i];
			globalCamera[CAMERA_YACHT].center[i] = (new GLdouble[3] { 400, -10, -40 })[i];
			globalCamera[CAMERA_YACHT].up[i] = (new GLdouble[3] { 5, 55, 370 })[i];
		}


		//------------------------------------------
		//AntTweakBar routines
		//------------------------------------------
		TwBar *bar ;
		bar = TwNewBar("Bar");

		TwAddVarRW(bar, "nRange", TW_TYPE_FLOAT, &nRange, "label='view Range' min=50 max=4000 step=10");
		 // Add 'ka', 'kb and 'kc' to 'bar': they are modifiable variables of type TW_TYPE_DOUBLE
		TwAddVarRW(bar, "ka", TW_TYPE_FLOAT, &windFloatFooVar[0], 
				   " label='X path coord'  min=-7 max=7 step=1 ");
		TwAddVarRW(bar, "kb", TW_TYPE_FLOAT, &windFloatFooVar[1],
				   " label='Y path coord'  min=-7 max=7 step=1 ");
		TwAddVarRW(bar, "scale", TW_TYPE_FLOAT, &boatScale,
			"label='scale boat' min=0.1 max=15 step=0.1");
		TwAddVarRW(bar, "AutoRotate", TW_TYPE_BOOL32, &trajectoryVisible,
			" label='trajectory' key=space help='Toggle visibility of path trajectory' ");
		TwAddVarRW(bar, "Movement", TW_TYPE_BOOL32, &boatMoving,
			" label='plywanie' help='make the boat swim or not swim on the drawn path' ");
		
		// Add the enum variable 'globalCamera' to 'bar'
		// (before adding an enum variable, its enum type must be declared to AntTweakBar as follow)
		{
			// ShapeEV associates Shape enum values with labels that will be displayed instead of enum values
			TwEnumVal cameraEV[NUM_CAMERAS] = { { CAMERA_YACHT, "Yacht" }, { CAMERA_GENERAL, "Plan ogólny" }, 
				{ CAMERA_USER, "user defined" } };
			// Create a type for the enum cameraEV
			TwType shapeType = TwDefineEnum("CameraType", cameraEV, NUM_CAMERAS);
			// add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
			TwAddVarRW(bar, "Kamera", shapeType, &eGlobalCamera,
				" keyIncr='<' keyDecr='>' help='Change camera view.' ");
		}

		TwBar *camera;
		camera = TwNewBar("Camera");
		TwAddVarRW(camera, "eyex", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].eye),
			"label='eye x' min=-2000 max=2000 keyIncr=1 keyDecr=CTRL+1 step=1");
		TwAddVarRW(camera, "eyey", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].eye + 1),
			"label='eye y' min=-2000 max=2000 keyIncr=2 keyDecr=CTRL+2 step=1");
		TwAddVarRW(camera, "eyez", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].eye + 2),
			"label='eye z' min=-2000 max=2000 keyIncr=3 keyDecr=CTRL+3 step=1");
		TwAddVarRW(camera, "centerx", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].center),
			"label='center x' min=-500 max=500 keyIncr=4 keyDecr=CTRL+4 step=10");
		TwAddVarRW(camera, "centery", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].center + 1),
			"label='center y' min=-500 max=500 keyIncr=5 keyDecr=CTRL+5 step=10");
		TwAddVarRW(camera, "centerz", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].center + 2),
			"label='center z' min=-500 max=500 keyIncr=6 keyDecr=CTRL+6 step=10");
		TwAddVarRW(camera, "upx", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].up),
			"label='up x' min=-500 max=500 keyIncr=7 keyDecr=CTRL+7 step=10");
		TwAddVarRW(camera, "upy", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].up + 1),
			"label='up y' min=-500 max=500 keyIncr=8 keyDecr=CTRL+8 step=10");
		TwAddVarRW(camera, "upz", TW_TYPE_DOUBLE, (globalCamera[CAMERA_USER].up + 2),
			"label='up z' min=-500 max=500 keyIncr=9 keyDecr=CTRL+9 step=10");
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
					   if (wParam == VK_UP )//|| wParam == VK_NUMPAD8)
						   xRot -= 5.0f;

					   if (wParam == VK_DOWN )//|| wParam == VK_NUMPAD2)
						   xRot += 5.0f;

					   if (wParam == VK_LEFT )//|| wParam == VK_NUMPAD4)
						   yRot -= 5.0f;

					   if (wParam == VK_RIGHT )//|| wParam == VK_NUMPAD6)
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
						
				if ((unsigned)time < navigation[0].size() - 1)
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