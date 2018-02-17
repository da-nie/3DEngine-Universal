#ifndef CCONTROL_H
#define CCONTROL_H

#include "ckeyboard.h"
#include "cmouse.h"
#include "cplayer.h"
#include "ciengine.h"
#include "cengine_sportal.h"
#include "cengine_gportal.h"
#include "cengine_bsp.h"
#include "cvideo.h"

#include <string>
using namespace std;

//���� ������
#define KEY_ESC     1
#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77
#define KEY_USE     57
#define KEY_SPEED   54
#define KEY_SIT     56
#define KEY_LOOK_UP   73
#define KEY_LOOK_DOWN 81

#define KEY_S_PORTAL 59
#define KEY_G_PORTAL 60
#define KEY_BSP      61

#define KEY_SAVE_POS 63
#define KEY_LOAD_POS 67

class CControl
{
 protected:
  CPlayer cPlayer;//����� ������
  CIEngine *cIEngine_Ptr;//����� 3D ������������
  long TickCounter;//����� �����
  string EngineName;//�������� ������
  string Path;//���� � ������
 public:
  CControl();//�����������
  ~CControl();//����������
  void SetPath(string path);//������� ���� � ������ ������
  void Init(void);//�������������
  void Close(void);//���������� ������
  void Processing(void);//�������� ���� ������
 protected:
};

#endif
