#ifndef CUNIT_H
#define CUNIT_H

#include "cvideo.h"

class CUnit
{
 protected:
 public:
  CUnit();//конструктор
  ~CUnit();//деструктор
  void Init(void);//инициализация
  void Close(void);//завершение работы
  void Processing(void);//основной цикл работы
 protected:
};

#endif
