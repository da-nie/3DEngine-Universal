#ifndef CUNIT_H
#define CUNIT_H

#include "cvideo.h"

class CUnit
{
 protected:
 public:
  CUnit();//�����������
  ~CUnit();//����������
  void Init(void);//�������������
  void Close(void);//���������� ������
  void Processing(void);//�������� ���� ������
 protected:
};

#endif
