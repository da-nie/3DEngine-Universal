#ifndef CSIMPLY_BRIDGE_H
#define CSIMPLY_BRIDGE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс простого мостика
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "cdecorator_cisector.h"

class CSimplyBridge:public CDecorator_CISector
{
 protected:
  long OldDown;//высота пола
  long Dz;//приращение высоты
  long BasedDown;//высота в активном состоянии
  bool Locked;//заблокирован ли мостик
 public:
  CSimplyBridge(CISector *cISector_Set_Ptr);//конструктор
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  void Activate(void);//активация
  void Lock(void);//заблокировать
  void Unlock(void);//разблокировать
  void Load(FILE *file);//загрузка
};

#endif 