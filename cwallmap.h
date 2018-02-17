#ifndef CWALL_MAP_H
#define CWALL_MAP_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������ � ���������� ������������ ���� �� �����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������ ����������
//----------------------------------------------------------------------------------------------------

#include <vector>
using namespace std;

#include "ciengine.h"

class CWallMap
{
 protected:
  vector< vector< vector<unsigned long> > > WallIndexMap;//��������� ������ ������ ���� ��� �����
  vector<long> vector_UsedLineIndex;//����������� �� ��� ����� ����� � ������
  long MinX;//������� ����
  long MinY;
  long MaxX;
  long MaxY;
  long GridXStep;//������ ���� �����
  long GridYStep;
 public:
  CWallMap(long grid_x_step,long grid_y_step);//�����������
  ~CWallMap();//����������
  void Release(void);//������� ������ ���� ��� �����
  void Create(vector<SWall> &vector_SWall);//������� ������ ���� ��� �����
  void GetWallIndexList(vector<unsigned long> &vector_wall,float x,float y,float radius);//�������� ������ �������� ���� �� �����, ������� ����� ���������� ������
};

#endif
