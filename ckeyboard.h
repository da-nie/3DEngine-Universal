#ifndef CKEYBOARD_H
#define CKEYBOARD_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����� ��������� ����������.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class CKeyboard
{
 protected:
  bool KeyTable[256];//������� ��������� ������ 	
 public:
  CKeyboard();
  ~CKeyboard();
  bool GetPressKey(unsigned char key);//�������� ��������� ������
  void SetKeyState(unsigned char key,bool state);//������ ��������� ������
};

#endif
 