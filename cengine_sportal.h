#ifndef CENGINE_SPORTAL_H
#define CENGINE_SPORTAL_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс системы визуализации с использованием экранного портала
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

using namespace std;

class CEngine_SPortal:public CEngine_Base
{
 protected:
  //параметры порталов
  struct SPortal
  {
   long SectorIndex;//индекс сектора
   long Left;//левая граница портала
   long Right;//правая граница портала
  };
 public:
  CEngine_SPortal();//конструктор
  ~CEngine_SPortal();//деструктор
  void LoadMap(string filename);//загрузить карту
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//отрисовать лабиринт
 protected:
  void DrawSectorWall(SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//отрисовка стен сектора (без порталов)
  void Draw(SPortal sPortal);//отрисовка сцены
  void DrawWall(SWall &sWall,SPortal &sPortal,bool portal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//отрисовка стены по порталу
};

#endif 