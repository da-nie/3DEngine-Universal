#ifndef CENGINE_GPORTAL_H
#define CENGINE_GPORTAL_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������� ������������ � �������������� ��������������� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

class CEngine_GPortal:public CEngine_Base
{
 protected:
  //��������� ��������
  struct SPortal
  {
   long PlayerSectorIndex;//������ �������, � ������� ��������� ����� 
   long SectorIndex;//������ �������
   float X,Y;//���������� �����(��� ����� ��� ���� ������)

   //����� ������ ���������
   float L_X,L_Y;//������������ ������
   //������ ������ ���������
   float R_X,R_Y;//������������ ������
   //������ ����������� ������� ���������
   float X1,Y1;
   float X2,Y2;

   unsigned char RR;//����� ����� ��� ������ � ������ �������
   unsigned char RG;
   unsigned char RB;

   unsigned char LR;
   unsigned char LG;
   unsigned char LB;
  };
 public:
  CEngine_GPortal();//�����������
  ~CEngine_GPortal();//����������
  void LoadMap(string filename);//��������� �����
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//���������� ��������
 protected:
  bool ClipWall(float &x1,float &y1,float &x2,float &y2,float &toffset,const SPortal &sPortal);//��������� ����� �� �������
  void DrawSectorWall(const SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ���� ������� ��� ��������
  void Draw(SPortal sPortal);//��������� �����
  void DrawWall(float x1,float y1,float x2,float y2,long toffset,long length,SWall &sWall,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� �����
};

#endif 