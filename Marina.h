#include "Boat.h"
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library



void marina(void)
{
	//glPushMatrix();
	//glTranslatef(0, 0, 0.01);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		//Kolor portu
		GLfloat portr, portg, portb;

		GLfloat ziemiar, ziemiag, ziemiab;

		ziemiar = 1.0f;
		ziemiag = 0.0;
		ziemiab = 1.0f;


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
		glColor3f(ziemiar, ziemiag, ziemiab);

		glBegin(GL_POLYGON);
		glVertex3fv(m01);
		glVertex3fv(m02);
		glVertex3fv(m03);
		glVertex3fv(m46);
		glVertex3fv(m46_1);
		glEnd();

		//s2


		glBegin(GL_POLYGON);
		glVertex3fv(m06_1);
		glVertex3fv(m03);
		glVertex3fv(m04);
		glVertex3fv(m05);
		glVertex3fv(m06);
		glEnd();

		//s3

		glBegin(GL_POLYGON);
		glVertex3fv(m46_1);
		glVertex3fv(m06_1);
		glVertex3fv(m06);
		glVertex3fv(m07);
		glVertex3fv(m08);
		glVertex3fv(m46_2);
		glEnd();

		//s4

		glBegin(GL_POLYGON);
		glVertex3fv(m46_2);
		glVertex3fv(m08);
		glVertex3fv(m09);
		glVertex3fv(m46_3);
		glEnd();

		//s5

		glBegin(GL_POLYGON);
		glVertex3fv(m46_3);
		glVertex3fv(m09);
		glVertex3fv(m15);
		glVertex3fv(m46_4);
		glEnd();

		//s6
		glBegin(GL_POLYGON);
		glVertex3fv(m09);
		glVertex3fv(m10);
		glVertex3fv(m14);
		glVertex3fv(m15);
		glEnd();

		//s7
		glBegin(GL_POLYGON);
		glVertex3fv(m10);
		glVertex3fv(m11);
		glVertex3fv(m12);
		glVertex3fv(m13);
		glVertex3fv(m14);
		glEnd();

		//s8
		glBegin(GL_POLYGON);
		glVertex3fv(m46_4);
		glVertex3fv(m15);
		glVertex3fv(m16);
		glVertex3fv(m46_5);
		glEnd();

		//s9
		glBegin(GL_POLYGON);
		glVertex3fv(m46_5);
		glVertex3fv(m16);
		glVertex3fv(m17);
		glVertex3fv(m18);
		glVertex3fv(m46_6);
		glEnd();

		//s10
		glBegin(GL_POLYGON);
		glVertex3fv(m46_6);
		glVertex3fv(m18);
		glVertex3fv(m19);
		glVertex3fv(m20);
		glVertex3fv(m21);
		glVertex3fv(m46_7);
		glEnd();

		//s11
		glBegin(GL_POLYGON);
		glVertex3fv(m46_7);
		glVertex3fv(m21);
		glVertex3fv(m22);
		glVertex3fv(m23);
		glVertex3fv(m24);
		glVertex3fv(m46_8);
		glEnd();

		//s12
		glBegin(GL_POLYGON);
		glVertex3fv(m46_8);
		glVertex3fv(m24);
		glVertex3fv(m25);
		glVertex3fv(m26);
		glVertex3fv(m27);
		glVertex3fv(m46_9);
		glEnd();

		//s13
		glBegin(GL_POLYGON);
		glVertex3fv(m46_9);
		glVertex3fv(m27);
		glVertex3fv(m28);
		glVertex3fv(m29);
		glVertex3fv(m46_10);
		glEnd();

		//s14
		glBegin(GL_POLYGON);
		glVertex3fv(m46_10);
		glVertex3fv(m29);
		glVertex3fv(m30);
		glVertex3fv(m46_11);
		glEnd();

		//s15
		glBegin(GL_POLYGON);
		glVertex3fv(m46_11);
		glVertex3fv(m30);
		glVertex3fv(m45_1);
		glVertex3fv(m45);
		glEnd();

		//s16
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m45_1);
		glVertex3fv(m30);
		glVertex3fv(m31);
		glVertex3fv(m32);
		glVertex3fv(m45_2);
		glEnd();

		//s17
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m45_2);
		glVertex3fv(m32);
		glVertex3fv(m33);
		glVertex3fv(m34);
		glVertex3fv(m35);
		glVertex3fv(m45_3);
		glEnd();

		//s18
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m45_3);
		glVertex3fv(m35);
		glVertex3fv(m36);
		glVertex3fv(m45_4);
		glEnd();

		//s19
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m45_4);
		glVertex3fv(m36);
		glVertex3fv(m41);
		glVertex3fv(m42);
		glVertex3fv(m45_5);
		glEnd();

		//s20
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m36);
		glVertex3fv(m37);
		glVertex3fv(m38);
		glVertex3fv(m39);
		glVertex3fv(m40);
		glVertex3fv(m41);
		glEnd();

		//s21
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m45_5);
		glVertex3fv(m42);
		glVertex3fv(m43);
		glVertex3fv(m44);
		glEnd();

		//Œciany góra

		//s1
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg01);
		glVertex3fv(mg02);
		glVertex3fv(mg03);
		glVertex3fv(mg46);
		glVertex3fv(mg46_1);
		glEnd();

		//s2
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg06_1);
		glVertex3fv(mg03);
		glVertex3fv(mg04);
		glVertex3fv(mg05);
		glVertex3fv(mg06);
		glEnd();

		//s3
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
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
		//(portr, portg, portb);
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_2);
		glVertex3fv(mg08);
		glVertex3fv(mg09);
		glVertex3fv(mg46_3);
		glEnd();

		//s5
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_3);
		glVertex3fv(mg09);
		glVertex3fv(mg15);
		glVertex3fv(mg46_4);
		glEnd();

		//s6
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg09);
		glVertex3fv(mg10);
		glVertex3fv(mg14);
		glVertex3fv(mg15);
		glEnd();

		//s7
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg10);
		glVertex3fv(mg11);
		glVertex3fv(mg12);
		glVertex3fv(mg13);
		glVertex3fv(mg14);
		glEnd();

		//s8
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_4);
		glVertex3fv(mg15);
		glVertex3fv(mg16);
		glVertex3fv(mg46_5);
		glEnd();

		//s9
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_5);
		glVertex3fv(mg16);
		glVertex3fv(mg17);
		glVertex3fv(mg18);
		glVertex3fv(mg46_6);
		glEnd();

		//s10
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_6);
		glVertex3fv(mg18);
		glVertex3fv(mg19);
		glVertex3fv(mg20);
		glVertex3fv(mg21);
		glVertex3fv(mg46_7);
		glEnd();

		//s11
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_7);
		glVertex3fv(mg21);
		glVertex3fv(mg22);
		glVertex3fv(mg23);
		glVertex3fv(mg24);
		glVertex3fv(mg46_8);
		glEnd();

		//s12
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
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
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_9);
		glVertex3fv(mg27);
		glVertex3fv(mg28);
		glVertex3fv(mg29);
		glVertex3fv(mg46_10);
		glEnd();

		//s14
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_10);
		glVertex3fv(mg29);
		glVertex3fv(mg30);
		glVertex3fv(mg46_11);
		glEnd();

		//s15
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg46_11);
		glVertex3fv(mg30);
		glVertex3fv(mg45_1);
		glVertex3fv(mg45);
		glEnd();

		//s16
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_1);
		glVertex3fv(mg30);
		glVertex3fv(mg31);
		glVertex3fv(mg32);
		glVertex3fv(mg45_2);
		glEnd();

		//s17
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_2);
		glVertex3fv(mg32);
		glVertex3fv(mg33);
		glVertex3fv(mg34);
		glVertex3fv(mg35);
		glVertex3fv(mg45_3);
		glEnd();

		//s18
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_3);
		glVertex3fv(mg35);
		glVertex3fv(mg36);
		glVertex3fv(mg45_4);
		glEnd();

		//s19
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg45_4);
		glVertex3fv(mg36);
		glVertex3fv(mg41);
		glVertex3fv(mg42);
		glVertex3fv(mg45_5);
		glEnd();

		//s20
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(mg36);
		glVertex3fv(mg37);
		glVertex3fv(mg38);
		glVertex3fv(mg39);
		glVertex3fv(mg40);
		glVertex3fv(mg41);
		glEnd();

		//s21
		//(1.0f, 0.0f, 1.0f);
//==

////> refs/remotes/origin/version-4
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
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m08);
		glVertex3fv(mg08);
		glVertex3fv(mg09);
		glVertex3fv(m09);
		glEnd();

		//s6
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m09);
		glVertex3fv(mg09);
		glVertex3fv(mg10);
		glVertex3fv(m10);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m14);
		glVertex3fv(mg14);
		glVertex3fv(mg15);
		glVertex3fv(m15);
		glEnd();

		//s7
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m10);
		glVertex3fv(mg10);
		glVertex3fv(mg11);
		glVertex3fv(m11);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m11);
		glVertex3fv(mg11);
		glVertex3fv(mg12);
		glVertex3fv(m12);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m12);
		glVertex3fv(mg12);
		glVertex3fv(mg11);
		glVertex3fv(m13);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m13);
		glVertex3fv(mg13);
		glVertex3fv(mg14);
		glVertex3fv(m14);
		glEnd();

		//s8
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m15);
		glVertex3fv(mg15);
		glVertex3fv(mg16);
		glVertex3fv(m16);
		glEnd();

		//s9
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m16);
		glVertex3fv(mg16);
		glVertex3fv(mg17);
		glVertex3fv(m17);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m17);
		glVertex3fv(mg17);
		glVertex3fv(mg18);
		glVertex3fv(m18);
		glEnd();

		//s10
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m18);
		glVertex3fv(mg18);
		glVertex3fv(mg19);
		glVertex3fv(m19);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m19);
		glVertex3fv(mg19);
		glVertex3fv(mg20);
		glVertex3fv(m20);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m20);
		glVertex3fv(mg20);
		glVertex3fv(mg21);
		glVertex3fv(m21);
		glEnd();

		//s11
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m21);
		glVertex3fv(mg21);
		glVertex3fv(mg22);
		glVertex3fv(m22);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m22);
		glVertex3fv(mg22);
		glVertex3fv(mg23);
		glVertex3fv(m23);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m23);
		glVertex3fv(mg23);
		glVertex3fv(mg24);
		glVertex3fv(m24);
		glEnd();

		//s12
		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m24);
		glVertex3fv(mg24);
		glVertex3fv(mg25);
		glVertex3fv(m25);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m25);
		glVertex3fv(mg25);
		glVertex3fv(mg26);
		glVertex3fv(m26);
		glEnd();

		//(portr, portg, portb);
//==

////> refs/remotes/origin/version-4
		glBegin(GL_POLYGON);
		glVertex3fv(m26);
		glVertex3fv(mg26);
		glVertex3fv(mg27);
		glVertex3fv(m27);
		glEnd();
	}
//==
	//glPopMatrix();
////> refs/remotes/origin/version-4
}