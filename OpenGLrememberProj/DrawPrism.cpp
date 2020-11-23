#define _USE_MATH_DEFINES

#include"Draw.h"
#include"MethodGauss.h"

//�����-���������
extern int alpha;
//����� ��������
extern bool texChange;
//id ��������
extern GLuint texId[2];

//���������� ���������� ����� ���� �����
vector<Point> DrawConvex(Point B, Point C, double z, Point midpoint = { 0, 0, 0 }) {
	//����� ���������� � ��� ������
	midpoint = Draw::SearchMidpoint(B, C);
	double r = Draw::SearchDistancePoints(B, midpoint);

	//����� ��� �� = 0
	double x = midpoint.x + r * cos(0);
	double y = midpoint.y + r * sin(0);
	Point O(x, y, 0);

	//���� �������
	Point VectorB = Draw::SearchVector(midpoint, B);
	Point VectorO = Draw::SearchVector(midpoint, O);
	Point VectorC = Draw::SearchVector(midpoint, C);

	//���� ���� ����� ���������
	double angleOC = Draw::SearchAngleVector(VectorO, VectorC);
	double angleOB = Draw::SearchAngleVector(VectorO, VectorB);

	//������ ���
	double step = 0.1;

	//���� ����� �� ����������
	vector<Point> pointsBC;
	Point newPoint(0, 0, 0);

	for (double i = angleOB; i >= angleOC * (-1); i -= step) {
		newPoint.x = midpoint.x + r * cos(i * M_PI / 180);
		newPoint.y = midpoint.y + r * sin(i * M_PI / 180);
		pointsBC.push_back(newPoint);
	}

	//������ ��������������
	vector<PointXY> texCoord;
	int size = pointsBC.size();
	for (int i = 0; i < size - 1; i++) {
		double t1 = i / (size - 1.0);
		double t2 = (i + 1.0) / (size - 1.0);
		texCoord.push_back(PointXY(t1, 0));
		texCoord.push_back(PointXY(t2, 0));
		texCoord.push_back(PointXY(t2, 1));
		texCoord.push_back(PointXY(t1, 1));

		Draw::DrawRectangle(pointsBC[i], pointsBC[i + 1], z, texCoord);

		texCoord.clear();
	}

	return pointsBC;
}
// ���������� ��������� ����� ����� ������� � ���������� ����� ����� c ������� ��������
vector<Point> DrawBulge(Point D, Point M, Point C, double z, Point& Apex) {
	double x[] = { C.x, M.x, D.x };
	double y[] = { C.y, M.y, D.y };

	//����� ���������� ���������
	//���������� ������������� �������� y = a * x^2 + b * x + c

    //����������� ����� � ��������� y = a * x^2 + b * x + c � ����������� ������� 
	double** matrix  = new double*[3];
	for (int i = 0; i < 3; i++) {
		matrix[i] = new double[4];
		matrix[i][0] = pow(x[i], 2);
		matrix[i][1] = x[i];
		matrix[i][2] = 1;
		matrix[i][3] = y[i];
	}

	//���������� ������� ��������� c ������� ������ ���������� ���������
	//LeastSquaresMethod(x, y, 3, matrix); }

	MethodGauss �oefficientsParabola(matrix, 3, 4);

	for (int i = 0; i < 3; i++) {
		delete[]matrix[i];
	}
	delete[] matrix;

	//������� ��������
	Apex = Draw::ApexParabola(�oefficientsParabola.decision[0], �oefficientsParabola.decision[1], �oefficientsParabola.decision[2]);

	//��������� ����� �� ���� D� ���������� ����� ����� �
	vector<Point> pointsDC;
	Point newPoint(0, 0, 0);
	for (double i = D.x; i <= C.x; i += 0.1) {
		newPoint.x = i;
		newPoint.y = pow(i, 2) * �oefficientsParabola.decision[0] + i * �oefficientsParabola.decision[1] + �oefficientsParabola.decision[2];
		pointsDC.push_back(newPoint);
	}

	//������ ���������
	vector<PointXY> texCoord;
	int size = pointsDC.size();
	for (int i = 0; i < size - 1; i++) {
		double t1 = i / (size - 1.0);
		double t2 = (i + 1.0) / (size - 1.0);
		texCoord.push_back(PointXY(t1, 0));
		texCoord.push_back(PointXY(t2, 0));
		texCoord.push_back(PointXY(t2, 1));
		texCoord.push_back(PointXY(t1, 1));

		Draw::DrawRectangle(pointsDC[i], pointsDC[i + 1], z, texCoord);

		texCoord.clear();
	}

	return pointsDC;
}
//���������� ��������� ����� ���� ����� � ������� ���� �����
vector<Point> DrawBulge(Point C, Point D, double z, Point center, double r) {
	//����� ��� �� = 0
	double x = center.x + r * cos(0);
	double y = center.y + r * sin(0);
	Point O(x, y, 0);

	//���� �������
	Point VectorD = Draw::SearchVector(center, D);
	Point VectorO = Draw::SearchVector(center, O);
	Point VectorC = Draw::SearchVector(center, C);

	//���� ���� ����� ���������
	double angleOC = Draw::SearchAngleVector(VectorO, VectorC);
	double angleOD = Draw::SearchAngleVector(VectorO, VectorD);

	//������ ���
	double step = 0.1;

	//���� ����� �� ����������
	vector<Point> pointsCD;
	Point newPoint(0, 0, 0);

	pointsCD.push_back(C);

	for (double i = angleOC; i >= angleOD; i -= step) {
		newPoint.x = center.x + r * cos(i * M_PI / 180);
		newPoint.y = center.y + r * sin(i * M_PI / 180);
		pointsCD.push_back(newPoint);
	}
	pointsCD.push_back(D);

	int size = pointsCD.size();
	//������ ��������������
	vector<PointXY> texCoord;
	for (int i = 0; i < size - 1; i++) {
		double t1 = i / (size - 1.0);
		double t2 = (i + 1.0) / (size - 1.0);
		texCoord.push_back(PointXY(t1, 0));
		texCoord.push_back(PointXY(t2, 0));
		texCoord.push_back(PointXY(t2, 1));
		texCoord.push_back(PointXY(t1, 1));

		Draw::DrawRectangle(pointsCD[i], pointsCD[i + 1], z, texCoord, -1);

		texCoord.clear();
	}

	return pointsCD;
}

//�������� ��� � ����, ����� ������� ����� (��������� - ��������)
void DrawTopBottom(Point A, Point B, Point C, Point D, Point E, Point F, Point G, double z) {
	// ���� ����� ����������� �������� AO � ED
	Point intersectionPoint1 = Draw::SearchIntersectionPoint(A, Point(-10, 0, 0), E, D);

	vector<vector<Point>> ThreePoint = { {A, G, F }, { F, E, intersectionPoint1 }, { F, intersectionPoint1, A } };
	for (int i = 0; i < (int)ThreePoint.size(); i++) {
		Draw::Color(i);
		Draw::DrawTriangles(ThreePoint[i]);
		for (int j = 0; j < (int)ThreePoint[i].size(); j++) {
			ThreePoint[i][j].z = z;
		}
		Draw::DrawTriangles(ThreePoint[i]);
	}

	//���� ����� ����������� �������� �� � AO
	Point intersectionPoint2 = Draw::SearchIntersectionPoint(A, Point(8, 0, 0), C, B);
	glColor3d(0.6, 0.8, 0.3);
	vector<Point> ThreePoint2 = { A, B, intersectionPoint2 };
	Draw::DrawTriangles(ThreePoint2);
	for (int i = 0; i < 3; i++) {
		ThreePoint2[i].z = z;
	}
	Draw::DrawTriangles(ThreePoint2);

}
//����������� ���
void DrawBottom(Point A, Point B, Point C, Point D, Point E, Point F, Point G, vector<Point> pointsBC, vector<Point> pointsCD) {
	glBegin(GL_TRIANGLES);
	Draw::DrawTriangles(D, E, F, -1);
	Draw::DrawTriangles(F, G, A, -1);
	Draw::DrawTriangles(F, A, C, -1);
	Draw::DrawTriangles(A, B, C, -1);

	//����������
	Point midPoint = Draw::SearchMidpoint(B, C);
	int size = pointsBC.size();
	for (int i = 0; i < size - 1; i++) {

		Draw::DrawTriangles(pointsBC[i], midPoint, pointsBC[i + 1]);
	}

	//����������
	size = pointsCD.size();
	for (int i = 0; i < size - 1; i++) {
		Draw::DrawTriangles(pointsCD[i], F, pointsCD[i + 1], -1);
	}
	glEnd();
}
//����������� �����
void DrawTop(Point A, Point B, Point C, Point D, Point E, Point F, Point G, vector<Point> pointsBC, vector<Point> pointsCD, double z, int alpha) {

	//����������� ����� ���������
	switch (alpha) {
	case 1:
		//�������� ����� ����������
		glEnable(GL_BLEND);
		//������ ����� ��� ������������� ��������� � ���������
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	case 2:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4d(0.7, 0.1, 0.1, 0.6);
		break;
	}

	glBegin(GL_TRIANGLES);

	Draw::DrawTriangles(D, E, F, z);
	Draw::DrawTriangles(F, G, A, z);
	Draw::DrawTriangles(F, A, C, z);
	Draw::DrawTriangles(A, B, C, z);

	//����������
	Point midPoint = Draw::SearchMidpoint(B, C);
	int size = pointsBC.size();
	for (int i = 0; i < size - 1; i++) {
		Draw::DrawTriangles(pointsBC[i], midPoint, pointsBC[i + 1], z, -1);
	}

	//����������
	size = pointsCD.size();
	for (int i = 0; i < size - 1; i++) {
		Draw::DrawTriangles(pointsCD[i], F, pointsCD[i + 1], z);
	}

	glEnd();

	//��������� ����������
	if (alpha != 0) {
		glDisable(GL_BLEND);
	}
}
//����������� ��������� (��������� - ��������)
void DrawTopBottomBulge(Point A, Point B, Point C, Point D, Point E, Point ApexParabola, vector<Point> pointsDC, double z) {
	Point O1 = Draw::SearchIntersectionPoint(A, Point(-10, 0, 0), D, E);
	Point O2 = Draw::SearchIntersectionPoint(A, Point(10, 0, 0), B, C);

	int size = pointsDC.size();
	for (int i = 0; i < size - 1; i++) {
		if (pointsDC[i + 1].x < ApexParabola.x) {
			Draw::DrawTriangles(O1, pointsDC[i], pointsDC[i + 1]);
			Draw::DrawTriangles(O1, pointsDC[i], pointsDC[i + 1], z);
		}
		else {
			Draw::DrawTriangles(A, pointsDC[i], pointsDC[i + 1]);
			Draw::DrawTriangles(A, pointsDC[i], pointsDC[i + 1], z);
		}
	}

	glColor3d(0.8, 0.1, 0.4);
	Draw::DrawTriangles(A, C, O2);
	Draw::DrawTriangles(A, C, O2, z);

	glColor3d(0.5, 0.2, 0.6);
	Draw::DrawTriangles(A, ApexParabola, O1);
	Draw::DrawTriangles(A, ApexParabola, O1, z);
}

void DrawABCDEFG() {
	Point A(0, 0, 0), B(7, 3, 0), C(4, -4, 0), D(-9, -4, 0), E(-7, 7, 0), F(-4, 2, 0), G(0, 9, 0), M(-4, -2, 0);
	double z = 2;
	Point A1(0, 0, z), B1(7, 3, z), C1(4, -4, z), D1(-9, -4, z), E1(-7, 7, z), F1(-4, 2, z), G1(0, 9, z), M1(-4, -2, z);

	//����� ��������
	if (texChange) {
		glBindTexture(GL_TEXTURE_2D, texId[0]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texId[1]);
	}

	//�����
	glBegin(GL_QUADS);
	//glColor3d(0.1, 0.2, 0.3);
	glColor3d(1, 29 / 256, 0 / 256);
	vector<PointXY> texCoord = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	Draw::DrawRectangle(A, B, z, texCoord);
	Draw::DrawRectangle(D, E, z, texCoord);
	Draw::DrawRectangle(E, F, z, texCoord);
	Draw::DrawRectangle(F, G, z, texCoord);
	Draw::DrawRectangle(G, A, z, texCoord);

	//����������
	vector<Point> pointsBC = DrawConvex(B, C, z);

	//���������
	Point Center(0, 0, 0);
	double r = 0;
	Draw::FindCenterCircle(C, M, D, Center, r);
	vector<Point> pointsCD = DrawBulge(D, C, z, Center, r);
	glEnd();

	vector<Point> points = { A, B, C, D, E, F, G };
	//���������� ��������� ��� ���� ������
	Draw::UpdatePoint(D, true, points, 1.5);

	glColor3d(0.8, 0.7, 0.3);

	//��� ������
	DrawBottom(A, B, C, D, E, F, G, pointsBC, pointsCD);

	//����� ��������
	if (!texChange) {
		glBindTexture(GL_TEXTURE_2D, texId[0]);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texId[1]);
	}

	//����� ������
	DrawTop(A, B, C, D, E, F, G, pointsBC, pointsCD, z, alpha);

}