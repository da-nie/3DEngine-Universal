#ifndef CIENGINE_H
#define CIENGINE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ����� ������� ������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------
#include "cisector.h"

//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------

//��������� ����
struct SWall
{
 long X1;
 long Y1;
 long X2;
 long Y2;
 CTextureFollow cTextureFollow_Down;//������ ��������
 CTextureFollow cTextureFollow_Up;//������� ��������
 bool Frontier;//����� �������=true
 bool Switch;//�������� �� ����� ��������������
 char SwitchLabel[SECTOR_LABEL_SIZE];//����� �������, � ������� ������ �������������
 //������������� ��� �������� ������
 long Sector1;//������ �������� �����
 long Sector2;
 long Offset;//�������� ��������
 long Len;//����� �����
 bool Visible;//����� �� ����� �����
 bool PortalEnabled;//��������� �� ��������� �������, ���� ����� �������� ��������
};

class CIEngine
{
 public:
  virtual ~CIEngine() {};//����������
  virtual void SetPath(string path)=0;//������� ������� �������
  virtual void Init(void)=0;//����������������
  virtual void Close(void)=0;//��������� ������
  virtual void LoadMap(string filename)=0;//��������� �����
  virtual void ReleaseMap(void)=0;//���������� ������ �����
  virtual long GetSectorIndexForPos(float x,float y)=0;//��������� ������ ������� �� ������� �� �����������
  virtual long GetZPos(float x,float y)=0;//��������� ���������� Z
  virtual void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)=0;//���������� ��������
  virtual float GetCos(long angle)=0;//�������� ������� ���� �� �������
  virtual float GetSin(long angle)=0;//�������� ����� ���� �� �������
  virtual void GetPlayerInitPos(float &x,float &y,long &angle)=0;//�������� ��������� ���������� ������
  virtual CISector *GetCISectorPtrForIndex(long sectorindex)=0;//�������� ��������� �� ������ �� ��� ������
  virtual vector<SWall>* GetVectorSWallPtr(void)=0;//�������� ��������� �� ������ ����
  virtual void Processing(void)=0;//��������� ��������
  virtual long GetIntersectionList(vector<long> *vector_collizion_ptr,float x,float y,float radius,float z,float height)=0;//�������� ������ ����, � �������� ������������ ������ 
};

#endif
