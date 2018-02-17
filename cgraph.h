#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>
#include <vector>
using namespace std;

#include "cvideo.h"


//������ ��������
#define TEXTURE_SIZE_POW 7
//���������� ������� ������������� ���������� ��������
#define TEXTURE_MIPMAP_LEVEL (TEXTURE_SIZE_POW+1)
//��������� �������� � �������� � 128 �����
#define TEXTURE_SCALE ((1<<TEXTURE_SIZE_POW)/128.0f)

//������ ������������ �������
struct SUseTexture
{
 string Name;//��� ������������ ��������
 long Index;//������ ��������
};


//������ ������� ����
class CGraphics
{
 protected:
  string Path;//������� �������
  unsigned long *Texture[TEXTURE_MIPMAP_LEVEL];//��������
 public:
  CGraphics();//�����������
  ~CGraphics();//����������
  void SetPath(string path);//������ ������� �������
  void Init(vector<SUseTexture> &vector_SUseTexture);//�������������
  void Release(void);//���������� ������
 protected:
  void LoadTexture(vector<SUseTexture> &vector_SUseTexture);//��������� ��������
  void MipMapping(long t);//���������� �������� ������� �������
  void LoadSprites(void);//��������� �������
 public:
  unsigned long *GetTexture(long level,long index);//�������� ������ ������� �������
};


#endif
