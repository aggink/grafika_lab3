#pragma once

#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

#include"DrawPrism.h"
#include"DrawSurfaces.h"

using namespace std;

bool textureMode = true;
bool lightMode = true;

//��������� alpha ���������
int alpha = 0;
//����� ��������
bool texChange = true;
//���������/���������� �����
bool line = true;
//��������
double sharpness_t = 0.1;
double copy_sharpness_t = sharpness_t;


//����� ��� ����������� �����
vector<vector<Point>> mas = {
	{Point(0, 0, 0), Point(0, 1, 1), Point(0, 2, 1), Point(0, 3, 0)},
	{Point(1, 0, 1), Point(1, 1, 1), Point(1, 2, 1), Point(1, 3, 1)},
	{Point(2, 0, 1), Point(2, 1, 1), Point(2, 2, 1), Point(2, 3, 1)},
	{Point(3, 0, 0), Point(3, 1, 1), Point(3, 2, 1), Point(3, 3, 0)}
};

//��������� ����� ���� ����� ��� ����������� �����
vector<vector<Point>> copy_mas = mas;

//����� ��� ������ ������, ����� 2 � 3 ��������
vector<Point> PointHB = {
	//������ ������ ������
	Point(2, 2, 2), Point(6, 4, 0),  Point(3, 4, 5),  Point(7, 5, 1),
	Point(4, 8, 10), Point(9, 3, 6), Point(4, 2, 7), Point(4, 6, 1),
	//������ ������ ����� 2 �������
	Point(-2, -2, -2), Point(-6, -4, 0), Point(-10, -8, -5),
	Point(-4, -8, -10), Point(-9, -3, -6), Point(-12, -5, -9),
	//������ ������ ����� 3 �������
	Point(-2, 2, 2), Point(-6, 4, 0), Point(-7, 5, 1), Point(-3, 4, 5),
	Point(-4, 8, 10), Point(-9, 3, 6), Point(-4, 6, 1), Point(-4, 2, 7)
};

//��������� ����� ���� ����� ��� ������ ������, ����� 2 � 3 ��������
vector<Point> copy_PointHB = PointHB;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		//��������� ��������� ������� ���� � ����������� ������.
		GetCursorPos(POINT);
		//������� ScreenToClient ������������ �������� ���������� ��������� ����� �� ������ � ���������� ������� �������
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);

		delete POINT;
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	//�������� ����� ��� ����������� �����
	if (!OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON)) {
		if (line) {
			LPPOINT POINT = new tagPOINT();
			//��������� ��������� ������� ���� � ����������� ������.
			GetCursorPos(POINT);
			//������� ScreenToClient ������������ �������� ���������� ��������� ����� �� ������ � ���������� ������� �������
			ScreenToClient(ogl->getHwnd(), POINT);
			POINT->y = ogl->getHeight() - POINT->y;

			// ��������� viewport-a
			GLint viewport[4];
			// ������� ��������
			GLdouble projection[16];
			// ������� �������
			GLdouble modelview[16];

			// ����� ��������� viewport-a
			glGetIntegerv(GL_VIEWPORT, viewport);
			//����� ������� ��������
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			// ����� ������� �������
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			double delta = 10;
			for (auto& v : mas)
			{
				for (auto& elem : v)
				{
					double tempPoint[3];
					// �������� �������� ���������� �������
					gluProject(elem.x, elem.y, elem.z, modelview, projection, viewport, &tempPoint[0], &tempPoint[1], &tempPoint[2]);
					if (tempPoint[0] > POINT->x - delta && tempPoint[0] < POINT->x + delta &&
						tempPoint[1] > POINT->y - delta && tempPoint[1] < POINT->y + delta)
					{
						tempPoint[0] -= dx;
						tempPoint[1] += dy;
						// ��������� �������� ���������� ������� � ���������
						gluUnProject(tempPoint[0], tempPoint[1], tempPoint[2], modelview, projection, viewport, &elem.x, &elem.y, &elem.z);
					}
				}
			}
			for (auto& elem : PointHB) {
				double tempPoint[3];
				// �������� �������� ���������� �������
				gluProject(elem.x, elem.y, elem.z, modelview, projection, viewport, &tempPoint[0], &tempPoint[1], &tempPoint[2]);
				if (tempPoint[0] > POINT->x - delta && tempPoint[0] < POINT->x + delta &&
					tempPoint[1] > POINT->y - delta && tempPoint[1] < POINT->y + delta)
				{
					tempPoint[0] -= dx;
					tempPoint[1] += dy;
					// ��������� �������� ���������� ������� � ���������
					gluUnProject(tempPoint[0], tempPoint[1], tempPoint[2], modelview, projection, viewport, &elem.x, &elem.y, &elem.z);
				}
			}
			delete POINT;
		}
	}
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	//��������� ����� ���������
	if (key == 'W') {
		alpha++;
		if (alpha == 3) {
			alpha = 0;
		}
	}
	//����� ��������
	if (key == 'Q') {
		texChange = !texChange;
	}
	//���������/���������� �����
	if (key == 'P') {
		line = !line;
	}
	//�������� ���������� �����
	if (key == 'M') {
		mas = copy_mas;
		PointHB = copy_PointHB;
	}
	//��������� �������� sharpness_t (t)
	if (key == '1') {
		sharpness_t = sharpness_t * 0.1;
	}
	//��������� �������� sharpness_t (t)
	if (key == '2') {
		sharpness_t = sharpness_t / 0.1;
	}
	//������� �������� � �������� ���������
	if (key == '0') {
		sharpness_t = copy_sharpness_t;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

vector<string> nametex_str = { "texture2.bmp", "Rabi.bmp" };
GLuint texId[2];

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;

	for (int i = 0; i < nametex_str.size(); i++) {
		const char* nametex = nametex_str[i].c_str();
		OpenGL::LoadBMP(nametex, &texW, &texH, &texarray);
		OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

		//���������� �� ��� ��������
		glGenTextures(1, &texId[i]);
		//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
		glBindTexture(GL_TEXTURE_2D, texId[i]);

		//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

		//�������� ������
		free(texCharArray);
		free(texarray);

		//������� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}

void Render(OpenGL *ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	//GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	//GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	//GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	//GLfloat sh = 0.1f * 256;

	GLfloat amb[] = { 167/256, 66/256, 245/256, 0 }; //���� ������
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. }; //���� �����
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. }; //���� �����
	GLfloat sh = 0.1f * 256; //������ �����

	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	//������ �����
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ��� 

	//������ ������ ���� 2
	//DrawABCDEFG();

	//���� 3
	Draw();
	
   //��������� ������ ������
	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������
	
	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������
	
	glDisable(GL_LIGHTING);
	
	
	
	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 250);
	rec.setPosition(10, ogl->getHeight() - 250 - 10);
	
	
	std::stringstream ss;
	ss << "1 - �������� (t * 0.1), 2 - �������� (t / 0.1)" << std::endl;
	ss << "0 - �������� ��������� �������� (t = 0.1)" << std::endl;
	ss << "M - ������� ���������� �����" << std::endl;
	ss << "P - ���/���� ����������� �����/�����" << std::endl;
	ss << "Q - ������� ��������" << std::endl;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();
	
	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();
	
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}