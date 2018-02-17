#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>
#include <vector>
using namespace std;

#include "cvideo.h"


//размер текстуры
#define TEXTURE_SIZE_POW 7
//количество уровней пирамидальной фильтрации текстуры
#define TEXTURE_MIPMAP_LEVEL (TEXTURE_SIZE_POW+1)
//отношение текстуры к исходной в 128 точек
#define TEXTURE_SCALE ((1<<TEXTURE_SIZE_POW)/128.0f)

//список используемых текстур
struct SUseTexture
{
 string Name;//имя используемой текстуры
 long Index;//индекс текстуры
};


//данные графики игры
class CGraphics
{
 protected:
  string Path;//рабочий каталог
  unsigned long *Texture[TEXTURE_MIPMAP_LEVEL];//текстуры
 public:
  CGraphics();//конструктор
  ~CGraphics();//деструктор
  void SetPath(string path);//задать рабочий каталог
  void Init(vector<SUseTexture> &vector_SUseTexture);//инициализация
  void Release(void);//освободить память
 protected:
  void LoadTexture(vector<SUseTexture> &vector_SUseTexture);//загрузить текстуры
  void MipMapping(long t);//построение пирамиды уровней текстур
  void LoadSprites(void);//загрузить спрайты
 public:
  unsigned long *GetTexture(long level,long index);//получить данные массива текстур
};


#endif
