#ifndef CSIMPLY_TELEPORT_H
#define CSIMPLY_TELEPORT_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������� �����������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "cdecorator_cisector.h"

class CSimplyTeleport:public CDecorator_CISector
{
 protected:
  long XT;
  long YT;
 public:
  CSimplyTeleport(CISector *cISector_Set_Ptr);//�����������
  void Initialize(void);//�������������
  void Processing(void);//���������
  void Activate(void);//���������
  bool GetTeleportCoord(long &x,long &y);//�������� ���������� ������������
  bool CheckActivate(float x,float y,float length,long sector_index);//��������, ����� �� ������������ ������ �� ��������� (x,y) � ������������ ����������� ��������� length
  void Load(FILE *file);//��������
};

#endif 