#include "localmath.h"

//------------------------------------------------------------------------------------------
//получить, с какой стороны от прямой находится точка
//------------------------------------------------------------------------------------------
MATH_RETURN_CODE GetSide(float x,float y,float x1,float y1,float x2,float y2)
{
 double p=x*(y1-y2)-y*(x1-x2)+x1*y2-x2*y1;
 if (p>EPS) return(MATH_RETURN_CODE_SIDE_LEFT);
 if (p<-EPS) return(MATH_RETURN_CODE_SIDE_RIGHT);
 return(MATH_RETURN_CODE_SIDE_IDENTITY);
}
//------------------------------------------------------------------------------------------
//получить, с какой стороны от прямой находится точка, считая расстояние от прямой 
//------------------------------------------------------------------------------------------
MATH_RETURN_CODE GetNSide(float x,float y,float x1,float y1,float x2,float y2)
{
 double nx=-(y2-y1);
 double ny=(x2-x1);
 double p=nx*(x-x1)+ny*(y-y1);
 double nl=sqrt(nx*nx+ny*ny);
 if (fabs(nl)<EPS) nl=1;
 p/=nl;
 if (p>10) return(MATH_RETURN_CODE_SIDE_LEFT);
 if (p<-10) return(MATH_RETURN_CODE_SIDE_RIGHT);
 return(MATH_RETURN_CODE_SIDE_IDENTITY);
}
//------------------------------------------------------------------------------------------
//узнать, пересекаются ли два отрезка
//------------------------------------------------------------------------------------------
MATH_RETURN_CODE CheckCollizionSegment(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy)
{
 double z=(bx-ax)*(dy-cy)-(by-ay)*(dx-cx);
 if (z>-EPS && z<EPS) return(MATH_RETURN_CODE_NO_COLLIZION);
 double r=((ay-cy)*(dx-cx)-(ax-cx)*(dy-cy))/z;
 if (r>1 || r<0) return(MATH_RETURN_CODE_NO_COLLIZION);
 double s=((ay-cy)*(bx-ax)-(ax-cx)*(by-ay))/z;
 if (!(s>1 || s<0)) return(MATH_RETURN_CODE_COLLIZION);
 return(MATH_RETURN_CODE_NO_COLLIZION);
}
//------------------------------------------------------------------------------------------
//узнать, пересекаются ли отрезок и луч
//------------------------------------------------------------------------------------------
MATH_RETURN_CODE CheckCollizionSegmentAndRay(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy)
{
 //отрезок: (ax,ay)-(bx,by)
 //луч: (cx,cy)-(dx,dy)
 double z=(bx-ax)*(dy-cy)-(by-ay)*(dx-cx);
 if (z>-EPS && z<EPS) return(MATH_RETURN_CODE_NO_COLLIZION);
 double r=((ay-cy)*(dx-cx)-(ax-cx)*(dy-cy))/z;
 if (r>1 || r<0) return(MATH_RETURN_CODE_NO_COLLIZION);
 double s=((ay-cy)*(bx-ax)-(ax-cx)*(by-ay))/z;
 if (s<0) return(MATH_RETURN_CODE_NO_COLLIZION);
 return(MATH_RETURN_CODE_COLLIZION);
}
//----------------------------------------------------------------------------------------------------
//Возвращает MATH_RETURN_CODE_NO_COLLIZION, если расстояние от (xc,yc) до сегмента с координатами
//xa,ya-xb,yb больше length, либо этот перпендикуляр находится вне
//данного сегмента.Иначе возвращает MATH_RETURN_CODE_COLLIZION.
//----------------------------------------------------------------------------------------------------
MATH_RETURN_CODE CheckCollizionOfVector(float xc,float yc,float length,float xa,float ya,float xb,float yb)
{
 float xp,yp;
 float k;
 if (fabs(xa-xb)>EPS)
 {
  k=(yb-ya)/(xb-xa);
  xp=(xc+(yc+k*xa-ya)*k)/(k*k+1);
  yp=k*(xp-xa)+ya;
 }
 else
 {
  k=(xb-xa)/(yb-ya);
  yp=(yc+(xc+k*ya-xa)*k)/(k*k+1);
  xp=k*(yp-ya)+xa;
 }
 if (xa>xb)
 {
  float r=xa;
  xa=xb;
  xb=r;
 }
 if (ya>yb)
 {
  float r=ya;
  ya=yb;
  yb=r;
 }
 if (xp<xa || xp>xb || yp<ya || yp>yb) return(MATH_RETURN_CODE_NO_COLLIZION);
 float l=static_cast<float>(sqrt((xc-xp)*(xc-xp)+(yc-yp)*(yc-yp)));
 if (l<length) return(MATH_RETURN_CODE_COLLIZION);
 return(MATH_RETURN_CODE_NO_COLLIZION);
}
//----------------------------------------------------------------------------------------------------
//Возвращает MATH_RETURN_CODE_NO_COLLIZION, если перпендикуляр пересечения находится вне
//сегмента.Иначе возвращает MATH_RETURN_CODE_COLLIZION.
//----------------------------------------------------------------------------------------------------
MATH_RETURN_CODE CheckCollizionPointOfSegment(float xc,float yc,float xa,float ya,float xb,float yb)
{
 float xp,yp;
 float k;
 if (fabs(xa-xb)>EPS)
 {
  k=(yb-ya)/(xb-xa);
  xp=(xc+(yc+k*xa-ya)*k)/(k*k+1);
  yp=k*(xp-xa)+ya;
 }
 else
 {
  k=(xb-xa)/(yb-ya);
  yp=(yc+(xc+k*ya-xa)*k)/(k*k+1);
  xp=k*(yp-ya)+xa;
 }
 if (xa>xb)
 {
  float r=xa;
  xa=xb;
  xb=r;
 }
 if (ya>yb)
 {
  float r=ya;
  ya=yb;
  yb=r;
 }
 if (xp<xa || xp>xb || yp<ya || yp>yb) return(MATH_RETURN_CODE_NO_COLLIZION);
 return(MATH_RETURN_CODE_COLLIZION);
}
//----------------------------------------------------------------------------------------------------
//найти точку пересечения двух отрезков
//----------------------------------------------------------------------------------------------------
MATH_RETURN_CODE Intersection(float xa1,float ya1,float xa2,float ya2,float xb1,float yb1,float xb2,float yb2,float* xo,float* yo)
{
 double lx=xa2-xa1;
 double ly=ya2-ya1;
 double kx=xb2-xb1;
 double ky=yb2-yb1;
 double xl=xa1;
 double yl=ya1;
 double xk=xb1;
 double yk=yb1;
 double d=-ly*kx+lx*ky;//определитель
 if (fabs(d)<EPS) return(MATH_RETURN_CODE_NO_COLLIZION);//не пересекаются
 double d1=(ly*xl-lx*yl)*(-kx)+lx*(ky*xk-kx*yk);
 double d2=ly*(ky*xk-kx*yk)-ky*(ly*xl-lx*yl);
 double xot=d1/d;//решаем методом Крамера
 double yot=d2/d;
 *xo=static_cast<float>(xot);
 *yo=static_cast<float>(yot);
 return(MATH_RETURN_CODE_COLLIZION);
}

 