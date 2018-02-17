#ifndef CTEXTUREFOLLOW_H
#define CTEXTUREFOLLOW_H

#include "cgraph.h"

//��� ��������
#define TEXTURE_FOLLOW_MODE_DISABLE 0
//����������� ��������
#define TEXTURE_FOLLOW_MODE_CYCLES  1
//�������� ����� � ���������
#define TEXTURE_FOLLOW_MODE_ZIGZAG  2

//��������� ���������� ��������
struct STexture
{
 long TextureIndex;//������ ��������
 string Name;//��� ��������
};

//����� �������� �������� ��������
class CTextureFollow
{
 protected:
  //-���������� ������-------------------------------------------------------
  vector<STexture> vector_STexture;
  long Mode;//����� ����� ��������

  long CurrentPointer;//������� ����� �������� � �������
  long dPointer;//��� ���������
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CTextureFollow(void);
  //-���������� ������-------------------------------------------------------
  ~CTextureFollow(void);
  //-����� ������� ������----------------------------------------------------
  void Save(FILE *file);//��������� �������� ��������
  void Load(FILE *file);//��������� �������� ��������
  void Processing(void);//��������� �������� ��������
  STexture GetCurrentTexture(void);//�������� ������� ��������
  void GoToEndTexture(void);//������� � ��������� ��������
  void CreateTextureList(vector<SUseTexture> &vector_SUseTexture);//�������� ������ ������������ �������
  void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture);//�������� ������� ������� � ������������ �� �������
};

#endif