#include "ckeyboard.h"

CKeyboard cKeyboard;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//����������� ������ ��������� ����������.                          //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::CKeyboard()
{
 for(short n=0;n<256;n++) KeyTable[n]=false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//���������� ������ ��������� ����������.                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
CKeyboard::~CKeyboard()
{ 
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//���������� ��������� ������� � ������� key.false-�� ������.       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
bool CKeyboard::GetPressKey(unsigned char key)
{	
 return(KeyTable[key]);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//������ ��������� ������                                           //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void CKeyboard::SetKeyState(unsigned char key,bool state)
{
 KeyTable[key]=state;	
}



 