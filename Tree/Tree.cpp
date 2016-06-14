#include "Tree.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include "../resource.h"           // About box resource identifiers.


Tree::Tree()
{
}


Tree::~Tree()
{
}

void Tree::renderAll()
{
	liscie(width, 0.0f, 0.9*height);
	konar(0.2*width, 0.19*width, 0.1*height);
}

void Tree::konar(float rBase, float rTop, float h)
{
	GLUquadricObj *konar;
	konar = gluNewQuadric();

	glPushMatrix();
	glColor3f(0.54f, 0.27f, 0.07f);
	glTranslatef(xPos, yPos, zPos);
	gluCylinder(konar, rBase, rTop, h, 32, 32);
	glPopMatrix();
}

void Tree::liscie(float rBase, float rTop, float h)
{
	GLUquadricObj *liscie;
	liscie = gluNewQuadric();

	glPushMatrix();
	glColor3f(0.0f, 0.5f, 0.0f);
	glTranslatef(xPos, yPos, zPos+0.1*height);
	gluCylinder(liscie, rBase, rTop, h, 32, 32);
	glPopMatrix();
}

void Tree::setPosition(float x, float y)
{
	xPos = x;
	yPos = y;
}

void Tree::setPosition(float x, float y, float z)
{
	xPos = x;
	yPos = y;
	zPos = z;
}

void Tree::setSize(float h, float w)
{
	height = h;
	width = w;
}