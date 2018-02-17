#ifndef CSIMPLY_DOOR_H
#define CSIMPLY_DOOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс обычной двери
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cdecorator_cisector.h"

class CSimplyDoor:public CDecorator_CISector
{
 protected:
  long OldDown;//высота пола
  long Time;//время ожидания
  long Dz;//приращение высоты
  bool Locked;//заблокирована ли дверь
 public:
  CSimplyDoor(CISector *cISector_Set_Ptr);//конструктор
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  void Activate(void);//активация
  void Lock(void);//заблокировать
  void Unlock(void);//разблокировать
};

#endif 