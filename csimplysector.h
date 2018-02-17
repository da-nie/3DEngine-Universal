#ifndef CSIMPLY_SECTOR_H
#define CSIMPLY_SECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������� ��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cisector.h"
#include "localmath.h"
#include "common.h"

class CSimplySector:public CISector
{
 protected:
  CTextureFollow cTextureFollow_Up;//�������� ������� 
  CTextureFollow cTextureFollow_Down;//�������� ����
  long Up;//������ �������
  long Down;//������ ����
  vector<SPoint> vector_SPoint;//�����, �������� ������
  float XB1;//�������������� �������������
  float YB1;
  float XB2;
  float YB2;
  long Index;//������ �������
  long Tick;//���� ��� ����� ��������
  long BaseLighting;//������� ������������ �������
  long Lighting;//������� ������������
  long LightingType;//��� �����������
  long DL;//��������� �����
  long TimeBaseLighting;//����� ������� ������������
  long TimeMaxLighting;//����� ������������ ������������
  long CounterChageLighting;//������� �� ����� ������������
  bool Antigravitation;//���� �� ��������������
  char Label[SECTOR_LABEL_SIZE];//�����
  
  vector<unsigned long> vector_WallIndex;//������ ���� �������
  vector<unsigned long> vector_WallPortalIndex;//������ ����-�������� �������
 public:
  CSimplySector(void);//�����������
  ~CSimplySector();//����������
  void Release(void);//���������� ������
  void Initialize(void);//�������������
  void Processing(void);//���������
  bool CheckActivate(float x,float y,float length,long sector_index);//��������, ����� �� ������������ ������ �� ��������� (x,y) � ������������ ����������� ��������� length
  void Load(FILE *file);//��������

  long GetIndex(void);//�������� ������ �������
  void SetIndex(long index);//������ ������ �������
  long GetUp(void);//�������� ������ ������� �������
  long GetDown(void);//�������� ������ ���� �������
  void GetRectangle(float &xb1,float &yb1,float &xb2,float &yb2);//�������� �������������� �������������
  vector<SPoint>* GetSectorPointVectorPtr(void);//�������� ��������� �� ������ �����, �������� ������
  long GetLighting(void);//�������� ������������ �������
  CTextureFollow* GetCTextureFollow_Down_Ptr(void);//�������� ��������� �� ������� ����
  CTextureFollow* GetCTextureFollow_Up_Ptr(void);//�������� ��������� �� ������� �������
  bool GetAntigravitation(void);//�������� ���� �� ��������������
  char *GetLabel(void);//�������� ����� �������
  void CreateTextureList(vector<SUseTexture> &vector_SUseTexture);//�������� ������ ������������ �������
  void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture);//�������� ������� ������� � ������������ �� �������
  vector<unsigned long>* GetVectorWallIndex(void);//�������� ��������� �� ������ �������� ����
  vector<unsigned long>* GetVectorWallPortalIndex(void);//�������� ��������� �� ������ �������� ����-��������

  void Activate(void);//���������
  void Lock(void);//�������������
  void Unlock(void);//��������������
  void SetDown(long value);//������ ������� ����
  void SetUp(long value);//������ ������� �������
  long GetActivatedSectorIndex(void);//�������� ������ ������� ��� ���������
  bool GetTeleportCoord(long &x,long &y);//�������� ���������� ������������
};

#endif