#ifndef LOCAL_MATH_H
#define LOCAL_MATH_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358
#endif 
//���������� �����
#define EPS 1e-8

//������������
enum MATH_RETURN_CODE
{
 //����� �����
 MATH_RETURN_CODE_SIDE_LEFT,
 //����� ������
 MATH_RETURN_CODE_SIDE_RIGHT,
 //����� �� ������
 MATH_RETURN_CODE_SIDE_IDENTITY,
 //��� �����������
 MATH_RETURN_CODE_NO_COLLIZION,
 //���� �����������
 MATH_RETURN_CODE_COLLIZION
};

MATH_RETURN_CODE GetSide(float x,float y,float x1,float y1,float x2,float y2);//��������, � ����� ������� �� ������ ��������� �����
MATH_RETURN_CODE GetNSide(float x,float y,float x1,float y1,float x2,float y2);//��������, � ����� ������� �� ������ ��������� �����, ������ ���������� �� ������
MATH_RETURN_CODE CheckCollizionSegment(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);//������, ������������ �� ��� �������
MATH_RETURN_CODE CheckCollizionSegmentAndRay(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);//������, ������������ �� ������� � ���
MATH_RETURN_CODE CheckCollizionOfVector(float xc,float yc,float length,float xa,float ya,float xb,float yb);//�������� ���������� �� �������
MATH_RETURN_CODE CheckCollizionPointOfSegment(float xc,float yc,float xa,float ya,float xb,float yb);//�������� ����������� � ��������
MATH_RETURN_CODE Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo);//����� ����� ����������� ���� ��������

#endif 