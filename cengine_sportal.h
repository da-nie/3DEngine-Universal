#ifndef CENGINE_SPORTAL_H
#define CENGINE_SPORTAL_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������� ������������ � �������������� ��������� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

using namespace std;

class CEngine_SPortal:public CEngine_Base
{
 protected:
  //��������� ��������
  struct SPortal
  {
   long SectorIndex;//������ �������
   long Left;//����� ������� �������
   long Right;//������ ������� �������
  };
 public:
  CEngine_SPortal();//�����������
  ~CEngine_SPortal();//����������
  void LoadMap(string filename);//��������� �����
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//���������� ��������
 protected:
  void DrawSectorWall(SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ���� ������� (��� ��������)
  void Draw(SPortal sPortal);//��������� �����
  void DrawWall(SWall &sWall,SPortal &sPortal,bool portal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ����� �� �������
};

#endif 