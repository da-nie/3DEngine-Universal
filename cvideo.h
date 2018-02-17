#ifndef CVIDEO_H
#define CVIDEO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//������� ����
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

//����� 8x14

#define FONT_WIDTH  8
#define FONT_HEIGHT 14


class CVideo
{
 protected:
  unsigned long *VideoBuffer;//����������
  unsigned long LineSize;//����� ������
  long ScreenWidth;//������ ������
  long ScreenHeight;//������ ������
  long WindowHeight;//������ ����
  long WindowXCenter;//���������� ������ ����
  long WindowYCenter;//���������� ������ ����
 public:
  CVideo();//�����������
  ~CVideo();//����������
  void Init(void);//��������������� �����������
  void Close(void);//������� � ��������� �����
  void GetScreenSize(long &width,long &height);//�������� ������� ������
  void GetWindowSize(long &width,long &height);//�������� ������� ����
  void GetWindownCenter(long &xcenter,long &ycenter);//�������� ����� ����
  void GetVideoPointer(unsigned long* &vptr,unsigned long &linesize);//�������� ��������� �� ����������
  void SetVideoPointer(unsigned long* vptr,unsigned long linesize);//������ ��������� �� ����������
  void PutSymbol(unsigned long x,unsigned long y,char symbol,unsigned long color);//����� ������� � �������
  void PutString(unsigned long x,unsigned long y,const char *string,unsigned long color);//����� ������� � �������
};
#endif
