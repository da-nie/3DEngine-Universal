#ifndef CWALL_MAP_H
#define CWALL_MAP_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс работы с разбиением пространства стен на блоки
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------

#include <vector>
using namespace std;

#include "ciengine.h"

class CWallMap
{
 protected:
  vector< vector< vector<unsigned long> > > WallIndexMap;//двумерный массив списка стен для сетки
  vector<long> vector_UsedLineIndex;//добавлялась ли уже такая линия в список
  long MinX;//размеры поля
  long MinY;
  long MaxX;
  long MaxY;
  long GridXStep;//размер шага сетки
  long GridYStep;
 public:
  CWallMap(long grid_x_step,long grid_y_step);//конструктор
  ~CWallMap();//деструктор
  void Release(void);//удалить массив стен для сетки
  void Create(vector<SWall> &vector_SWall);//создать массив стен для сетки
  void GetWallIndexList(vector<unsigned long> &vector_wall,float x,float y,float radius);//получить список индексов стен на сетке, которые может пересекать объект
};

#endif
