#ifndef CENGINE_GPORTAL_H
#define CENGINE_GPORTAL_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс системы визуализации с использованием геометрического портала
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

class CEngine_GPortal:public CEngine_Base
{
 protected:
  //параметры порталов
  struct SPortal
  {
   long PlayerSectorIndex;//индекс сектора, в котором находится игрок 
   long SectorIndex;//индекс сектора
   float X,Y;//координаты точки(она общая для этих прямых)

   //левая прямая отсечения
   float L_X,L_Y;//направляющий вектор
   //правая прямая отсечения
   float R_X,R_Y;//направляющий вектор
   //прямая ограничения области видимости
   float X1,Y1;
   float X2,Y2;

   unsigned char RR;//цвета линий при выводе в режиме отладки
   unsigned char RG;
   unsigned char RB;

   unsigned char LR;
   unsigned char LG;
   unsigned char LB;
  };
 public:
  CEngine_GPortal();//конструктор
  ~CEngine_GPortal();//деструктор
  void LoadMap(string filename);//загрузить карту
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//отрисовать лабиринт
 protected:
  bool ClipWall(float &x1,float &y1,float &x2,float &y2,float &toffset,const SPortal &sPortal);//отсечение стены по порталу
  void DrawSectorWall(const SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//отрисовка стен сектора без порталов
  void Draw(SPortal sPortal);//отрисовка сцены
  void DrawWall(float x1,float y1,float x2,float y2,long toffset,long length,SWall &sWall,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//отрисовка стены
};

#endif 