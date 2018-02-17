#ifndef CSIMPLY_DOOR_H
#define CSIMPLY_DOOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������� �����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cdecorator_cisector.h"

class CSimplyDoor:public CDecorator_CISector
{
 protected:
  long OldDown;//������ ����
  long Time;//����� ��������
  long Dz;//���������� ������
  bool Locked;//������������� �� �����
 public:
  CSimplyDoor(CISector *cISector_Set_Ptr);//�����������
  void Initialize(void);//�������������
  void Processing(void);//���������
  void Activate(void);//���������
  void Lock(void);//�������������
  void Unlock(void);//��������������
};

#endif 