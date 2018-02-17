#ifndef CSWITCH_SECTOR_H
#define CSWITCH_SECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс сектора-переключателя
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cdecorator_cisector.h"

class CSwitchSector:public CDecorator_CISector
{
 protected:
  unsigned long ActivatedSectorIndex;//сектор, который активируется переключателем
  bool Enabled;//можно ли использовать переключатель
 public:
  CSwitchSector(CISector *cISector_Set_Ptr);//конструктор
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  void Activate(void);//активация
  long GetActivatedSectorIndex(void);//получить сектор для активации
  bool CheckActivate(float x,float y,float length,long sector_index);//проверка, можно ли активировать сектор из координат (x,y) с максимальным расстоянием активации length
};

#endif 