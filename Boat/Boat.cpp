#include "Boat.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include "../resource.h"           // About box resource identifiers.

#include <cmath>
#define GL_PI 3.14

Boat::Boat()
{
	mass = 500.0; //kg
	radius = 45.0 * absoluteScalingFactor;
	posX = posY = posZ = 0.0;
	//yachtBlender.load("blender/yacht.objblender");
	//xminOrigin = -(xmaxOrigin = 40) - 5; //min = -45, max = 40
	//yminOrigin = -(ymaxOrigin = 20);
	//xmin = xminOrigin;
	//xmax = xmaxOrigin;
	//ymin = yminOrigin;
	//ymax = ymaxOrigin;
}

void Boat::renderAll()
{
	kadlub();
	dziob();
	maszt(80.0f, 10.0f);
	rufa();
	
	zagiel(80.0f, 10.0f);
}

void Boat::renderAll(float scale)
{
	radius = 45.0 * absoluteScalingFactor * scale;
	scale *= absoluteScalingFactor;
	if (scale == 0.0)
	{
		renderAll(); return;
	}
	glPushMatrix();
	//glTranslatef(posX, posY, posZ);
	glScalef(scale, scale, scale);
	glRotatef(angle * 180 / GL_PI, 0.0, 0.0, 1.0);
	renderAll();
	glPopMatrix();
}

void Boat::renderMirror()
{
	//mirror everything from top do bottom
	const GLfloat m[16] = {  1, 0, 0, 0 ,  0, 1, 0, 0 ,  0, 0, -1, 0 ,  0, 0, 0, 1  };
	glPushMatrix();
	glMultMatrixf(m);
	glTranslatef(0, 0, -0.01+posZ);
	renderAll();
	glPopMatrix();
}

void Boat::renderMirror(float scale)
{
	scale *= absoluteScalingFactor;
	//mirror everything from top do bottom
	const GLfloat m[16] = { 1, 0, 0, 0, 
							0, 1, 0, 0, 
							0, 0, -1, 0,
							0, 0, 0, 1 };
	glPushMatrix();
	glMultMatrixf(m);
	glTranslatef(0, 0, -0.01);
	renderAll(scale);
	glPopMatrix();
}

void Boat::renderBlender(float scale)
{
	//glColor3i(185, 122, 87); //brown
	//scale *= absoluteScalingFactor;
	//if (scale == 0.0)
	//{
	//	renderAll(); return;
	//}
	//glPushMatrix();
	//	glScalef(scale, scale, scale);
	//	yachtBlender.render();
	//glPopMatrix();
}

void drawCuboid(GLfloat[6]);
void drawTriangle(float *, float *, float *);

void Boat::changePosition(float dx, float dy)
{
	posX += dx;
	posY += dy;
}

void Boat::kadlub()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow

		GLfloat sa[3] = { posX + (-25.0f), posY + (20.0f),posZ +  (0.0f)};
		GLfloat sb[3] = { posX + (25.0f), posY + (20.0f),posZ +  (0.0f)};
		GLfloat sc[3] = { posX + (25.0f), posY + (20.0f),posZ +  (10.0f)};
		GLfloat sd[3] = { posX + (-25.0f), posY + (20.0f),posZ +  (10.0f)};
		GLfloat se[3] = { posX + (-25.0f), posY + (-20.0f),posZ +  (0.0f)};
		GLfloat sf[3] = { posX + (25.0f), posY + (-20.0f),posZ +  (0.0f)};
		GLfloat sg[3] = { posX + (25.0f), posY + (-20.0f),posZ +  (10.0f)};
		GLfloat sh[3] = { posX + (-25.0f), posY + (-20.0f),posZ +  (10.0f)};


		// Sciany skladowe
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sc);
		glVertex3fv(sd);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(sg);
		glVertex3fv(sc);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sf);
		glVertex3fv(se);
		glVertex3fv(sh);
		glVertex3fv(sg);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(se);
		glVertex3fv(sa);
		glVertex3fv(sd);
		glVertex3fv(sh);
		glEnd();

		glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glVertex3fv(sg);
		glVertex3fv(sh);
		glEnd();

		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(se);
		glEnd();
	}
}

void Boat::dziob()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow

		GLfloat ka[3] = { posX + (25.0f), posY + (20.0f),posZ +  (0.0f)};
		GLfloat kb[3] = { posX + (40.0f), posY + (0.0f),posZ +  (0.0f)};
		GLfloat kc[3] = { posX + (25.0f), posY + (-20.0f),posZ +  (0.0f)};
		GLfloat kd[3] = { posX + (25.0f), posY + (20.0f),posZ +  (10.0f)};
		GLfloat ke[3] = { posX + (40.0f), posY + (0.0f),posZ +  (10.0f)};
		GLfloat kf[3] = { posX + (25.0f), posY + (-20.0f),posZ +  (10.0f)};


		// Sciany skladowe
		//Prawa burta
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(ka);
		glVertex3fv(kb);
		glVertex3fv(ke);
		glVertex3fv(kd);
		glEnd();

		//Lewa burta
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(kb);
		glVertex3fv(kc);
		glVertex3fv(kf);
		glVertex3fv(ke);
		glEnd();

		//usun sciane acfd z pokladu/dna

		//Góra
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3fv(kd);
		glVertex3fv(ke);
		glVertex3fv(kf);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLES);
		glVertex3fv(ka);
		glVertex3fv(kb);
		glVertex3fv(kc);
		glEnd();

	}
}

void Boat::maszt(float masztDlugosc, float masztDolWysokosc)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		drawCuboid(new GLfloat[6] { posX + 4, posX + 6, posY - 1, posY + 1,
			posZ + masztDolWysokosc, posZ + (masztDolWysokosc + masztDlugosc) });
	}
}

void Boat::rufa()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow

		GLfloat ra[3] = { posX + (-25.0f), posY + (20.0f),posZ +  (0.0f)}; //tak samo jak w szescianie
		GLfloat rb[3] = { posX + (-35.0f), posY + (5.0f),posZ +  (0.0f)};
		GLfloat rc[3] = { posX + (-35.0f), posY + (5.0f),posZ +  (10.0f)};
		GLfloat rd[3] = { posX + (-25.0f), posY + (20.0f),posZ +  (10.0f)}; //tak samo jak w szescianie
		GLfloat re[3] = { posX + (-25.0f), posY + (-20.0f),posZ +  (0.0f)}; //tak samo jak w szescianie
		GLfloat rf[3] = { posX + (-35.0f), posY + (-5.0f),posZ +  (0.0f)};
		GLfloat rg[3] = { posX + (-35.0f), posY + (-5.0f),posZ +  (10.0f)};
		GLfloat rh[3] = { posX + (-25.0f), posY + (-20.0f),posZ +  (10.0f)}; //tak samo jak w szescianie
		//GLfloat r[24] = { ra[0], ra[1], ra[2], rb[0], rb[1], rb[2], rc[0], rc[1], rc[2], rd[0], rd[1], rd[2], rf[0], rf[1], rf[2], rg[0], rg[1], rg[2], rh[0], rh[1], rh[2] };

		// Sciany skladowe
		//Prawa burta
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(ra);
		glVertex3fv(rb);
		glVertex3fv(rc);
		glVertex3fv(rd);
		glEnd();

		//Lewa burta
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(re);
		glVertex3fv(rf);
		glVertex3fv(rg);
		glVertex3fv(rh);
		glEnd();

		//usun sciane acfd z pokladu/dna

		//Góra
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(rd);
		glVertex3fv(rc);
		glVertex3fv(rg);
		glVertex3fv(rh);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(ra);
		glVertex3fv(rb);
		glVertex3fv(rf);
		glVertex3fv(re);
		glEnd();

		//Ty³
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex3fv(rb);
		glVertex3fv(rc);
		glVertex3fv(rg);
		glVertex3fv(rf);
		glEnd();

	}
}

void Boat::zagiel(float masztDlugosc, float masztDolWysokosc)
{
	glColor3f(0.95f, 0.95f, 0.95f);
	drawTriangle(new float[3] { posX + (5.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f)},
		new float[3] { posX + (5.0f), posY + (0.0f), posZ + ((masztDolWysokosc + masztDlugosc))},
		new float[3] { posX + (-20.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f + 1)});
	drawTriangle(new float[3] { posX + (5.0f), posY + (0.0f), posZ + (masztDolWysokosc)},
		new float[3] { posX + (7.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 16.5f * 13.0f)},
		new float[3] { posX + (30.0f), posY + (0.0f), posZ + (masztDolWysokosc * 2.0f / 3.0f)});

	//druga strona
	/*drawTriangle(new float[3] {-posX - (5.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f)},
		new float[3] {-posX - (5.0f), posY + (0.0f), posZ + ((masztDolWysokosc + masztDlugosc))},
		new float[3] {-posX - (-20.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f + 1)});
	drawTriangle(new float[3] {-posX - (5.0f), posY + (0.0f), posZ + (masztDolWysokosc)},
		new float[3] {-posX - (7.0f), posY + (0.0f), posZ + (masztDolWysokosc + masztDlugosc / 16.5f * 13.0f)},
		new float[3] {-posX - (30.0f), posY + (0.0f), posZ + (masztDolWysokosc * 2.0f / 3.0f)});*/
	/*drawTriangle(new float[3] { posY + (0.0f), posX + (5.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f)},
		new float[3] { posY + (0.0f), posX + (5.0f), posZ + ((masztDolWysokosc + masztDlugosc))},
		new float[3] { posX + (-20.0f), posZ + (masztDolWysokosc + masztDlugosc / 11.0f + 1), posY + (0.0f)});
	drawTriangle(new float[3] { posY + (0.0f),posX + (5.0f),  posZ + (masztDolWysokosc)},
		new float[3] { posY + (0.0f),posX + (7.0f),  posZ + (masztDolWysokosc + masztDlugosc / 16.5f * 13.0f)},
		new float[3] { posY + (0.0f),posX + (30.0f),  posZ + (masztDolWysokosc * 2.0f / 3.0f)});*/
}

void Boat::setPosition(float _x, float _y)
{
	x = _x;
	y = _y;

	/*xmax = xmaxOrigin * cos(getAngle());
	ymax = ymaxOrigin * sin(getAngle());
	xmin = xminOrigin * cos(getAngle());
	ymin = yminOrigin * sin(getAngle());*/
}

void Boat::setPosition(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float *Boat::getPosition()
{
	return new float[3] {x, y, z};
}

//bool Boat::isCollision(float przyladek)
//{
//	return (przyladek > ymin);
//}

Boat::~Boat()
{
}

