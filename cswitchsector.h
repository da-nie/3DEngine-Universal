#ifndef CSWITCH_SECTOR_H
#define CSWITCH_SECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������-�������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cdecorator_cisector.h"

class CSwitchSector:public CDecorator_CISector
{
 protected:
  unsigned long ActivatedSectorIndex;//������, ������� ������������ ��������������
  bool Enabled;//����� �� ������������ �������������
 public:
  CSwitchSector(CISector *cISector_Set_Ptr);//�����������
  void Initialize(void);//�������������
  void Processing(void);//���������
  void Activate(void);//���������
  long GetActivatedSectorIndex(void);//�������� ������ ��� ���������
  bool CheckActivate(float x,float y,float length,long sector_index);//��������, ����� �� ������������ ������ �� ��������� (x,y) � ������������ ����������� ��������� length
};

#endif 