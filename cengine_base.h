#ifndef CENGINE_BASE_H
#define CENGINE_BASE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������� ����� ������� ������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------

#include "localmath.h"
#include "common.h"
#include "cunit.h"
#include "ciengine.h"
#include "cwallmap.h"

using namespace std;

#include "cisector.h"
#include "csimplysector.h"
#include "csimplydoor.h"
#include "csimplyplatform.h"
#include "csimplybridge.h"
#include "cswitchsector.h"
#include "csimplyteleport.h"

#include "cvideo.h"
#include "cgraph.h"
#include "ctexturefollow.h"


//----------------------------------------------------------------------------------------------------
//����������������
//----------------------------------------------------------------------------------------------------

//����� ��� ���������� � ��������� ������
#define FIXMATH_SHIFT 20
//����� ��� ���������� � ��������� ������ ��� ����� � ��������
#define FIXMATH_FLOOR_FLOW_SHIFT 16

//������������ ������� �������
#define MAX_BRIGHTNESS_VALUE 255

//������ ����� ��������� �����
#define GRID_X_STEP 128
#define GRID_Y_STEP 128


class CEngine_Base:public CIEngine
{
 protected:
  float CosTable[361];//������� ���������
  float SinTable[361];//������� �������

  float PlayerXInitPos;//��������� ���������� X ������
  float PlayerYInitPos;//��������� ���������� Y ������
  float PlayerAngleInit;//��������� ���� ������
  float PlayerX;//���������� ������
  float PlayerY;
  float PlayerZ;
  long PlayerAngle;//���� �������� ������
  long PlayerAngleVertical;//���� ������ ������
  long PlayerPowerLighting;//�������� �������� ������
  long PlayerSectorIndex;//������, � ������� ��������� �����

  CGraphics cGraphics;//����� ������� ����
  vector<CISector*> vector_CISectorPtr;//������ ���������� �� ������� �� �� �������
  vector<CUnit*> vector_CUnitPtr;//������ ��������
  vector<SWall> vector_SWall;//������ ����

  long dcTable[361];//������� ��� ��������������� ���������������
  long dsTable[361];
  long diTable[361];
  long djTable[361];

  long *TopLine;//����� ���������
  long *BottomLine;
  long SetLine;//����� ����������� �������� ������

  long *X_Table;//����� ��������� ��������� X ��� ��������������� ����� � ��������

  struct SZLine
  {
   long Y_Top;//���� �������
   long Y_Bottom;//��� �������
   long Z;//�������� ������� �������
  };

  vector< vector<SZLine> > ZLineBuffer;//����� �������� ���������� �������

  //��������� �����������
  unsigned long *VideoBufferPtr;//����������
  unsigned long VideoLineWidth;//������ ������ �����������
  long WindowWidth;//������ ����
  long WindowHeight;//������ ����
  long WindowXCenter;//���������� ������ ����
  long WindowYCenter;//���������� ������ ����
  long WindowYCenterWithOffset;//��������� ���������� ������ ����

  //��������� ��������������� ����� � ��������
  struct SVisualPlanes
  {
   long MinX;//����������� ���������� X �������
   long MaxX;//������������ ���������� X �������
   long TopY[WINDOW_WIDTH];//������� ����������
   long BottomY[WINDOW_WIDTH];//������ ����������
  };

  string Path;//������� �������
  
  CWallMap *cWallMap_Ptr;//���� ����
 public:
  CEngine_Base();//�����������
  ~CEngine_Base();//����������
  void SetPath(string path);//������� ������� �������
  void Init(void);//����������������
  void Close(void);//��������� ������
  void ReleaseMap(void);//���������� ������ �����
  long GetSectorIndexForPos(float x,float y);//��������� ������ ������� �� ������� �� �����������
  long GetZPos(float x,float y);//��������� ���������� Z
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//���������� ��������
  float GetCos(long angle);//�������� ������� ���� �� �������
  float GetSin(long angle);//�������� ����� ���� �� �������
  void GetPlayerInitPos(float &x,float &y,long &angle);//�������� ��������� ���������� ������
  CISector *GetCISectorPtrForIndex(long sectorindex);//�������� ��������� �� ������ �� ��� ������
  vector<SWall>* GetVectorSWallPtr(void);//�������� ��������� �� ������ ����

  void Processing(void);//��������� ��������
  long GetIntersectionList(vector<long> *vector_collizion_ptr,float x,float y,float radius,float z,float height);//�������� ������ ����, � �������� ������������ ������ 
 protected:
  void DrawFloorLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Bottom);//���������� ����� ������ ������� ����
  void DrawFlowLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Top);//���������� ����� ������ ������� �������
  unsigned long* FindTexture(long scale,long texture_u,long texture_index,long *mask,long *texture_dv,long *texture_v);//����� ��������� �� ������� ��������
  void DrawSimpleLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ������� ���������� �����
  void DrawLowerLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ������� ������ �����
  void DrawUpperLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//��������� ������� ������� �����
  void ClearFloorFlowBuffer(void);//������� ������ ����� � ��������
  void DrawFlow(long sector_index,const SVisualPlanes &sVisualPlanes_Top);//��������� ��������
  void DrawFloor(long sector_index,const SVisualPlanes &sVisualPlanes_Bottom);//��������� �����
  void DrawTextureLine(long dist,long scale,long sector_bright,long texture_index,long screen_y,long left_x,long right_x);//��������� ���������������� ����� ���� ��� �������

  void PutSprite(void);//����� �������
  void Line(long x1,long y1,long x2,long y2,unsigned long color);//��������� �������
  void PutPixel(long x,long y,unsigned long color);//����� �����
  void SetZBufferVerticalLineValue(long x,long y1,long y2,long z);//���������� ������ � Z-������
};

#endif
