#ifndef CDECORATOR_CISECTOR_H
#define CDECORATOR_CISECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�����-��������� ��� CISector
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cisector.h"

class CDecorator_CISector:public CISector
{
 protected:
  CISector *cISector_Ptr;//������������ �����
 public:
  CDecorator_CISector(CISector *cISector_Set_Ptr);//�����������
  ~CDecorator_CISector();//����������
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
 protected:
};

#endif



