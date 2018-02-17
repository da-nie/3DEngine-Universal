#ifndef CVIDEO_H
#define CVIDEO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//размеры окна
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

//шрифт 8x14

#define FONT_WIDTH  8
#define FONT_HEIGHT 14


class CVideo
{
 protected:
  unsigned long *VideoBuffer;//видеобуфер
  unsigned long LineSize;//длина строки
  long ScreenWidth;//ширина экрана
  long ScreenHeight;//высота экрана
  long WindowHeight;//высота окна
  long WindowXCenter;//координаты центра окна
  long WindowYCenter;//координаты центра окна
 public:
  CVideo();//конструктор
  ~CVideo();//деструктор
  void Init(void);//инициализизация видеорежима
  void Close(void);//возврат в текстовый режим
  void GetScreenSize(long &width,long &height);//получить размеры экрана
  void GetWindowSize(long &width,long &height);//получить размеры окна
  void GetWindownCenter(long &xcenter,long &ycenter);//получить центр окна
  void GetVideoPointer(unsigned long* &vptr,unsigned long &linesize);//получить указатель на видеобуфер
  void SetVideoPointer(unsigned long* vptr,unsigned long linesize);//задать указатель на видеобуфер
  void PutSymbol(unsigned long x,unsigned long y,char symbol,unsigned long color);//вывод символа в позицию
  void PutString(unsigned long x,unsigned long y,const char *string,unsigned long color);//вывод строчки в позицию
};
#endif
