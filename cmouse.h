#ifndef CMOUSE_H
#define CMOUSE_H

//класс работы с мышкой
class CMouse
{
 protected:
  bool LeftKeyState;//левая клавиша мыши
  bool RightKeyState;//правая клавиша мыши
  bool CenterKeyState;//центральная клавиша мыши
  long dX;//изменение координат мыши
  long dY;
 public:
  CMouse();//конструктор
  ~CMouse();//деструктор
  bool GetLeftKeyState(void);//получить состояние левой клавиши
  bool GetRightKeyState(void);//получить состояние правой клавиши
  bool GetCenterKeyState(void);//получить состояние центральной клавиши

  void SetLeftKeyState(bool state);//задать состояние левой клавиши
  void SetRightKeyState(bool state);//задать состояние правой клавиши
  void SetCenterKeyState(bool state);//задать состояние центральной клавиши

  long GetXOffset(void);//получить смещение по X
  long GetYOffset(void);//получить смещение по Y

  void SetXOffset(long dx);//задать смещение по X
  void SetYOffset(long dy);//задать смещение по Y
 protected:
};

#endif
