#ifndef CENGINE_BSP_H
#define CENGINE_BSP_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������� ������������ � �������������� BSP-������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

//----------------------------------------------------------------------------------------------------
//����������������
//----------------------------------------------------------------------------------------------------

//��� ���� ������
#define BSP_ATOM  (1<<0)
//����� ����� ����
#define BSP_LEFT  (1<<1)
//������ ����� ����
#define BSP_RIGHT (1<<2)

//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------

struct SBSPNode
{
 SBSPNode* sBSPNode_LeftPtr;//����� ���������
 SBSPNode* sBSPNode_RightPtr;//������ ���������
 long SegmentIndex;//������ ��������
 unsigned char Split;//��� �� ����� (1-����� �������,0-�������)
 float LX1,LX2,LY1,LY2;//����� ����������� �������������
 float RX1,RX2,RY1,RY2;//������ ����������� �������������
};


class CEngine_BSP:public CEngine_Base
{
 protected:
  //��������� ���������
  struct SSegment
  {
   float X1;
   float Y1;
   float X2;
   float Y2;
   long Offset;//�������� ��������
   long Sector1;//������ �������� �����
   long Sector2;
   long Len;//����� ��������
   bool Visible;//����� �� ����� �������
   long WallIndex;//������ �����  
  };
  vector<SSegment> vector_SSegment;//������ ���������
  SBSPNode* sBSPNode_Ptr;//��������� �� ������ BSP-������
 public:
  CEngine_BSP();//�����������
  ~CEngine_BSP();//����������
  void LoadMap(string filename);//��������� �����
  void ReleaseMap(void);//���������� ������ �����
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//���������� ��������
 protected:
  SBSPNode* LoadBSPTree(FILE *file);//�������� ����� ������
  void DeleteBSPTree(SBSPNode *sBSPNode_RootPtr);//�������� ������
  long SeenBox(SBSPNode* tree,long type);//�������� ��������� ������������ ��������������
  inline void DrawBSPTree(SBSPNode* tree);//����� BSP ������ � ���������� ���������
  void DrawBSPLeafe(SBSPNode* root);//���������� ���������  ����
};

#endif 