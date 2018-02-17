#ifndef CMAIN_H
#define CMAIN_H

#include"ccontrol.h"
#include"cvideo.h"

class CMain
{
 protected:
  //-���������� ������-------------------------------------------------------
  CControl cControl;  
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CMain();
  //-���������� ������-------------------------------------------------------
  ~CMain();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  //-����� ������� ������----------------------------------------------------
  bool OpenWindow(void);//�������� ����
  bool CloseWindow(void);//�������� ����
  //-������� ��������� ��������� ������--------------------------------------
  void OnActivate_MouseButton(long x,long y,bool left,bool right,bool center);//������ ��� ��������� ������ ����
  void OnActivate_Timer(void);//��������� �������  
  //-����� ������� ������----------------------------------------------------
  //-������------------------------------------------------------------------
};

#endif