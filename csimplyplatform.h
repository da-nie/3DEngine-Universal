#ifndef CSIMPLY_PLATFORM_H
#define CSIMPLY_PLATFORM_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс простой платформы
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "cdecorator_cisector.h"

class CSimplyPlatform:public CDecorator_CISector
{
 protected:
  long OldDown;//высота пола
  long Time;//время ожидания
  long Dz;//приращение высоты
  long BasedDown;//высота в активном состоянии
  bool Locked;//заблокирована ли платформа
 public:
  CSimplyPlatform(CISector *cISector_Set_Ptr);//конструктор
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  void Activate(void);//активация
  void Lock(void);//заблокировать
  void Unlock(void);//разблокировать
  void Load(FILE *file);//загрузка
};

#endif 