#ifndef CTEXTUREFOLLOW_H
#define CTEXTUREFOLLOW_H

#include "cgraph.h"

//нет анимации
#define TEXTURE_FOLLOW_MODE_DISABLE 0
//циклическая анимация
#define TEXTURE_FOLLOW_MODE_CYCLES  1
//анимация смена с возвратом
#define TEXTURE_FOLLOW_MODE_ZIGZAG  2

//структура параметров текстуры
struct STexture
{
 long TextureIndex;//индекс текстуры
 string Name;//имя текстуры
};

//класс элемента анимации текстуры
class CTextureFollow
{
 protected:
  //-Переменные класса-------------------------------------------------------
  vector<STexture> vector_STexture;
  long Mode;//режим смены текстуры

  long CurrentPointer;//текущий номер текстуры в массиве
  long dPointer;//его изменение
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CTextureFollow(void);
  //-Деструктор класса-------------------------------------------------------
  ~CTextureFollow(void);
  //-Новые функции класса----------------------------------------------------
  void Save(FILE *file);//сохранить анимацию текстуры
  void Load(FILE *file);//загрузить анимацию текстуры
  void Processing(void);//обработка анимации текстуры
  STexture GetCurrentTexture(void);//получить текущую текстуру
  void GoToEndTexture(void);//перейти к последней текстуре
  void CreateTextureList(vector<SUseTexture> &vector_SUseTexture);//получить список используемых текстур
  void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture);//заменить индексы текстур в соответствии со списком
};

#endif