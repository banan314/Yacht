#include "Boat.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library

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