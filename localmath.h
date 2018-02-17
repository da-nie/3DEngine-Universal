#ifndef LOCAL_MATH_H
#define LOCAL_MATH_H

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358
#endif 
//бесконечно малое
#define EPS 1e-8

//перечисления
enum MATH_RETURN_CODE
{
 //точка слева
 MATH_RETURN_CODE_SIDE_LEFT,
 //точка справа
 MATH_RETURN_CODE_SIDE_RIGHT,
 //точка на прямой
 MATH_RETURN_CODE_SIDE_IDENTITY,
 //нет пересечения
 MATH_RETURN_CODE_NO_COLLIZION,
 //есть пересечение
 MATH_RETURN_CODE_COLLIZION
};

MATH_RETURN_CODE GetSide(float x,float y,float x1,float y1,float x2,float y2);//получить, с какой стороны от прямой находится точка
MATH_RETURN_CODE GetNSide(float x,float y,float x1,float y1,float x2,float y2);//получить, с какой стороны от прямой находится точка, считая расстояние от прямой
MATH_RETURN_CODE CheckCollizionSegment(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);//узнать, пересекаются ли два отрезка
MATH_RETURN_CODE CheckCollizionSegmentAndRay(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy);//узнать, пересекаются ли отрезок и луч
MATH_RETURN_CODE CheckCollizionOfVector(float xc,float yc,float length,float xa,float ya,float xb,float yb);//проверка расстояния до отрезка
MATH_RETURN_CODE CheckCollizionPointOfSegment(float xc,float yc,float xa,float ya,float xb,float yb);//проверка пересечения с отрезком
MATH_RETURN_CODE Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo);//найти точку пересечения двух отрезков

#endif 