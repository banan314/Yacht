
// GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include <VersionHelpers.h>
#include "resource.h"           // About box resource identifiers.
#include "blender/gl2.h"
#include "blender/bitmap.h"
#include "blender/model_obj.h"
#include "blender\WGL_ARB_multisample.h"
//#include "AntTweakBar\include\AntTweakBar.h"
#include "AntTweakBar.h"

extern GLuint				g_nullTexture;
extern GLuint				g_blinnPhongShader;
extern GLuint				g_normalMappingShader;

bool ExtensionSupported(const char *pszExtensionName, HDC hDC);
void ChooseBestMultiSampleAntiAliasingPixelFormat(int &pf, int &maxSamples);
GLuint LoadTexture(const char *pszFilename);