#ifndef CISECTOR_H
#define CISECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������� ����� ��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <vector>
using namespace std;

#include "ctexturefollow.h"

//�����
struct SPoint
{
 float X;
 float Y;
};

//������� ������
#define SECTOR_TYPE_SIMPLY          0
//������� �����
#define SECTOR_TYPE_DOOR            1
//�������������
#define SECTOR_TYPE_SWITCH          2
//����� �� �������������
#define SECTOR_TYPE_SWITCH_DOOR     3
//���������
#define SECTOR_TYPE_PLATFORM        4
//��������� �� �������������
#define SECTOR_TYPE_SWITCH_PLATFORM 5
//����� � ������
#define SECTOR_TYPE_END             6
//������������
#define SECTOR_TYPE_TELEPORT        7
//������
#define SECTOR_TYPE_BRIDGE          8

//���� ���������
//����������� ���������
#define LIGHTING_MODE_STATIC          0
//�������
#define LIGHTING_MODE_FLASH           1
//��������� �������
#define LIGHTING_MODE_RANDOM_FLASH    2
//���������/����������
#define LIGHTING_MODE_DAWNING_SUNDOWN 3

//������ ����� �������
#define SECTOR_LABEL_SIZE 11

class CISector
{
 public:
  virtual ~CISector() {};//����������
  virtual void Release(void)=0;//���������� ������
  virtual void Initialize(void)=0;//�������������
  virtual void Processing(void)=0;//���������
  virtual bool CheckActivate(float x,float y,float length,long sector_index)=0;//��������, ����� �� ������������ ������ �� ��������� (x,y) � ������������ ����������� ��������� length
  virtual void Load(FILE *file)=0;//��������

  virtual long GetIndex(void)=0;//�������� ������ �������
  virtual void SetIndex(long index)=0;//������ ������ �������
  virtual long GetUp(void)=0;//�������� ������ ������� �������
  virtual long GetDown(void)=0;//�������� ������ ���� �������
  virtual void GetRectangle(float &xb1,float &yb1,float &xb2,float &yb2)=0;//�������� �������������� �������������
  virtual vector<SPoint>* GetSectorPointVectorPtr(void)=0;//�������� ��������� �� ������ �����, �������� ������
  virtual long GetLighting(void)=0;//�������� ������������ �������
  virtual CTextureFollow* GetCTextureFollow_Down_Ptr(void)=0;//�������� ��������� �� ������� ����
  virtual CTextureFollow* GetCTextureFollow_Up_Ptr(void)=0;//�������� ��������� �� ������� �������
  virtual bool GetAntigravitation(void)=0;//�������� ���� �� ��������������
  virtual char *GetLabel(void)=0;//�������� ����� �������
  virtual void CreateTextureList(vector<SUseTexture> &vector_SUseTexture)=0;//�������� ������ ������������ �������
  virtual void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture)=0;//�������� ������� ������� � ������������ �� �������
  virtual vector<unsigned long>* GetVectorWallIndex(void)=0;//�������� ��������� �� ������ �������� ����
  virtual vector<unsigned long>* GetVectorWallPortalIndex(void)=0;//�������� ��������� �� ������ �������� ����-��������

  virtual void Activate(void)=0;//���������
  virtual void Lock(void)=0;//�������������
  virtual void Unlock(void)=0;//��������������
  virtual void SetDown(long value)=0;//������ ������� ����
  virtual void SetUp(long value)=0;//������ ������� �������
  virtual long GetActivatedSectorIndex(void)=0;//�������� ������ ������� ��� ���������
  virtual bool GetTeleportCoord(long &x,long &y)=0;//�������� ���������� ������������
};
#endif 