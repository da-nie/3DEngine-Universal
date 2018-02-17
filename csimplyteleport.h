#ifndef CSIMPLY_TELEPORT_H
#define CSIMPLY_TELEPORT_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс простого телепортера
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "cdecorator_cisector.h"

class CSimplyTeleport:public CDecorator_CISector
{
 protected:
  long XT;
  long YT;
 public:
  CSimplyTeleport(CISector *cISector_Set_Ptr);//конструктор
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  void Activate(void);//активация
  bool GetTeleportCoord(long &x,long &y);//получить координаты телепортации
  bool CheckActivate(float x,float y,float length,long sector_index);//проверка, можно ли активировать сектор из координат (x,y) с максимальным расстоянием активации length
  void Load(FILE *file);//загрузка
};

#endif 