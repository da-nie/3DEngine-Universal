#ifndef CKEYBOARD_H
#define CKEYBOARD_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Класс обработки клавиатуры.                                       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class CKeyboard
{
 protected:
  bool KeyTable[256];//таблица состояния клавиш 	
 public:
  CKeyboard();
  ~CKeyboard();
  bool GetPressKey(unsigned char key);//получить состояние кнопки
  void SetKeyState(unsigned char key,bool state);//задать состояние кнопки
};

#endif
 