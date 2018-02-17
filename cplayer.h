#ifndef CPLAYER_H
#define CPLAYER_H

//������������ ���������� ��������� ��������
#define MAX_LENGTH_TO_ACTIVATE 50
//������ ������ �� ����
#define PLAYER_HEIGHT 64
//������ ������
#define PLAYER_RADIUS 20
//���������� ���� �������
#define PLAYER_MOVE_ANGLE_STEP 25
//������������ ������ ������� ��� ������
#define PLAYER_MOVE_MAX_HEIGHT 2
//������ �� ���� �� �������
#define PLAYER_EYE_TO_TOP_HEIGHT 10
//������������ ���� ������ � ����������
#define MAX_ANGLE_VERTICAL 50
//����������� ���� ������ � ����������
#define MIN_ANGLE_VERTICAL -50

#include "localmath.h"
#include "ciengine.h"

class CPlayer
{
 protected:
  CIEngine *cIEngine_Ptr;
  long Height;//������ ������
  long Angle;//���������� � ��������� ������ � ������������
  long AngleVertical;//���� ������ � ����������
  float X;//���������� ������
  float Y;
  float Z;//��� ������

  long Radius;//������ ������
  float DX;//��������� ���������
  float DY;
  float DZ;//�������� ������� ������
  float SFB;//�������� �����������
  float SRT;//�������� ��������
  bool OnMove;//true-���� ��������, false-���(����� ��� ����������)
  bool OnRotate;//true-��� �������, false-���(����� ��� ��������)
  long SectorIndex;//������, � ������� ��������� �����(������������ �� ������ ��� � ������� ������)
  float PowerLighting;//�������� ������
  long MoveAngle;//���� �������
 public:
  void SetCIEngine(CIEngine *cIEngine_Ptr_Set);//������ ��������� �� ������
  void Init(void);//�������������
  void SitDown(void);//��������
  void StandUp(void);//������
  void GoUp(void);//���� �����
  void GoDown(void);//���� �����
  void StepUp(void);//������� ��� �����
  void StepDown(void);//������� ��� �����
  void RotateLeft(void);//��������� ������
  void RotateRight(void);//��������� �������
  void LookUp(void);//���������� �����
  void LookDown(void);//���������� ����
  void Use(void);//������������
  void Processing(void);//��������� ������
  void GetPos(float &x,float &y,float &z,long &angle,long &angle_vertical);//�������� ���������� ������
  void SetLookParameter(long angle,long angle_vertical);//������ ��������� ������
  void GetLookParameter(long &angle,long &angle_vertical);//�������� ��������� ������
  float GetPowerLighting(void);//�������� �������� ������
  float GetViewHeight(void);//�������� ���������� ������ ���� ������
  long GetSectorIndex(void);//�������� ����� �������, � ������� ��������� ����� (� ������ ��������� ������)
  long GetHeight(void);//�������� ������ ������  
  void SavePos(void);//�������� ���������
  void LoadPos(void);//��������� ���������
 protected:
  long Intersection(vector<long> *vector_collizion_ptr);//�������� ������ �����, � �������� ������������ �����
  void CheckImpact(void);//�������� ������������ ������ �� �������
  long SlideTraverse(long wall);//���������� ������ ����� �����
  long ImpactLine(float xw1,float yw1,float xw2,float yw2);
  void FindPlayerSector(void);//����� ������, � ������� ��������� ����� (� ������ ��������� ������)
};
#endif
 