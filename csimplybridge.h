#ifndef CSIMPLY_BRIDGE_H
#define CSIMPLY_BRIDGE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "cdecorator_cisector.h"

class CSimplyBridge:public CDecorator_CISector
{
 protected:
  long OldDown;//������ ����
  long Dz;//���������� ������
  long BasedDown;//������ � �������� ���������
  bool Locked;//������������ �� ������
 public:
  CSimplyBridge(CISector *cISector_Set_Ptr);//�����������
  void Initialize(void);//�������������
  void Processing(void);//���������
  void Activate(void);//���������
  void Lock(void);//�������������
  void Unlock(void);//��������������
  void Load(FILE *file);//��������
};

#endif 