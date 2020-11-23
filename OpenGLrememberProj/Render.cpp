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

//активациа alpha наложения
int alpha = 0;
//смена текстуры
bool texChange = true;
//включение/отключение линий
bool line = true;
//резкость
double sharpness_t = 0.1;
double copy_sharpness_t = sharpness_t;


//точки для поверхности Безье
vector<vector<Point>> mas = {
	{Point(0, 0, 0), Point(0, 1, 1), Point(0, 2, 1), Point(0, 3, 0)},
	{Point(1, 0, 1), Point(1, 1, 1), Point(1, 2, 1), Point(1, 3, 1)},
	{Point(2, 0, 1), Point(2, 1, 1), Point(2, 2, 1), Point(2, 3, 1)},
	{Point(3, 0, 0), Point(3, 1, 1), Point(3, 2, 1), Point(3, 3, 0)}
};

//начальная копия всех точек для поверхности Безье
vector<vector<Point>> copy_mas = mas;

//точки для кривых Эрмита, Безье 2 и 3 порядков
vector<Point> PointHB = {
	//Рисуем кривые Эрмита
	Point(2, 2, 2), Point(6, 4, 0),  Point(3, 4, 5),  Point(7, 5, 1),
	Point(4, 8, 10), Point(9, 3, 6), Point(4, 2, 7), Point(4, 6, 1),
	//Рисуем кривые Безье 2 порядка
	Point(-2, -2, -2), Point(-6, -4, 0), Point(-10, -8, -5),
	Point(-4, -8, -10), Point(-9, -3, -6), Point(-12, -5, -9),
	//Рисуем кривую Безье 3 порядка
	Point(-2, 2, 2), Point(-6, 4, 0), Point(-7, 5, 1), Point(-3, 4, 5),
	Point(-4, 8, 10), Point(-9, 3, 6), Point(-4, 6, 1), Point(-4, 2, 7)
};

//начальная копия всех точек для кривых Эрмита, Безье 2 и 3 порядков
vector<Point> copy_PointHB = PointHB;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
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
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
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
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
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

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		//Извлекает положение курсора мыши в координатах экрана.
		GetCursorPos(POINT);
		//Функция ScreenToClient переделывает экранные координаты указанной точки на экране в координаты рабочей области
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

	//измением точки для поверхности Безье
	if (!OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON)) {
		if (line) {
			LPPOINT POINT = new tagPOINT();
			//Извлекает положение курсора мыши в координатах экрана.
			GetCursorPos(POINT);
			//Функция ScreenToClient переделывает экранные координаты указанной точки на экране в координаты рабочей области
			ScreenToClient(ogl->getHwnd(), POINT);
			POINT->y = ogl->getHeight() - POINT->y;

			// параметры viewport-a
			GLint viewport[4];
			// матрица проекции
			GLdouble projection[16];
			// видовая матрица
			GLdouble modelview[16];

			// узнаём параметры viewport-a
			glGetIntegerv(GL_VIEWPORT, viewport);
			//узнаём матрицу проекции
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			// узнаём видовую матрицу
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			double delta = 10;
			for (auto& v : mas)
			{
				for (auto& elem : v)
				{
					double tempPoint[3];
					// Получаем экранные координаты обьекта
					gluProject(elem.x, elem.y, elem.z, modelview, projection, viewport, &tempPoint[0], &tempPoint[1], &tempPoint[2]);
					if (tempPoint[0] > POINT->x - delta && tempPoint[0] < POINT->x + delta &&
						tempPoint[1] > POINT->y - delta && tempPoint[1] < POINT->y + delta)
					{
						tempPoint[0] -= dx;
						tempPoint[1] += dy;
						// Переводим экранные координаты обратно в объектные
						gluUnProject(tempPoint[0], tempPoint[1], tempPoint[2], modelview, projection, viewport, &elem.x, &elem.y, &elem.z);
					}
				}
			}
			for (auto& elem : PointHB) {
				double tempPoint[3];
				// Получаем экранные координаты обьекта
				gluProject(elem.x, elem.y, elem.z, modelview, projection, viewport, &tempPoint[0], &tempPoint[1], &tempPoint[2]);
				if (tempPoint[0] > POINT->x - delta && tempPoint[0] < POINT->x + delta &&
					tempPoint[1] > POINT->y - delta && tempPoint[1] < POINT->y + delta)
				{
					tempPoint[0] -= dx;
					tempPoint[1] += dy;
					// Переводим экранные координаты обратно в объектные
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

	//активация альфа наложения
	if (key == 'W') {
		alpha++;
		if (alpha == 3) {
			alpha = 0;
		}
	}
	//смена текстуры
	if (key == 'Q') {
		texChange = !texChange;
	}
	//включение/отключение линий
	if (key == 'P') {
		line = !line;
	}
	//сбросить координаты точек
	if (key == 'M') {
		mas = copy_mas;
		PointHB = copy_PointHB;
	}
	//увеличить резкость sharpness_t (t)
	if (key == '1') {
		sharpness_t = sharpness_t * 0.1;
	}
	//уменьшить резкость sharpness_t (t)
	if (key == '2') {
		sharpness_t = sharpness_t / 0.1;
	}
	//вернуть резкость в исходное состояние
	if (key == '0') {
		sharpness_t = copy_sharpness_t;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

vector<string> nametex_str = { "texture2.bmp", "Rabi.bmp" };
GLuint texId[2];

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;

	for (int i = 0; i < nametex_str.size(); i++) {
		const char* nametex = nametex_str[i].c_str();
		OpenGL::LoadBMP(nametex, &texW, &texH, &texarray);
		OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

		//генерируем ИД для текстуры
		glGenTextures(1, &texId[i]);
		//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
		glBindTexture(GL_TEXTURE_2D, texId[i]);

		//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

		//отчистка памяти
		free(texCharArray);
		free(texarray);

		//наводим шмон
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

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


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	//GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	//GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	//GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	//GLfloat sh = 0.1f * 256;

	GLfloat amb[] = { 167/256, 66/256, 245/256, 0 }; //цвет фигуры
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. }; //цвет света
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. }; //цвет блика
	GLfloat sh = 0.1f * 256; //размер блика

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут 

	//рисуем фигуру лаба 2
	//DrawABCDEFG();

	//лаба 3
	Draw();
	
   //Сообщение вверху экрана
	
	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	                                //(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции
	
	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт
	
	glDisable(GL_LIGHTING);
	
	
	
	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 250);
	rec.setPosition(10, ogl->getHeight() - 250 - 10);
	
	
	std::stringstream ss;
	ss << "1 - резкость (t * 0.1), 2 - резкость (t / 0.1)" << std::endl;
	ss << "0 - исходное состояние резкости (t = 0.1)" << std::endl;
	ss << "M - сборить координаты точек" << std::endl;
	ss << "P - вкл/выкл отображение сеток/линий" << std::endl;
	ss << "Q - сменить текстуру" << std::endl;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();
	
	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();
	
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}