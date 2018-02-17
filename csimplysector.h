#ifndef CSIMPLY_SECTOR_H
#define CSIMPLY_SECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс простых секторов
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cisector.h"
#include "localmath.h"
#include "common.h"

class CSimplySector:public CISector
{
 protected:
  CTextureFollow cTextureFollow_Up;//текстура потолка 
  CTextureFollow cTextureFollow_Down;//текстура пола
  long Up;//высота потолка
  long Down;//высота пола
  vector<SPoint> vector_SPoint;//точки, задающие сектор
  float XB1;//ограничивающий прямоугольник
  float YB1;
  float XB2;
  float YB2;
  long Index;//индекс сектора
  long Tick;//такт для смены текстуры
  long BaseLighting;//базовая освещенность сектора
  long Lighting;//текущая освещенность
  long LightingType;//тип спецэффекта
  long DL;//изменение света
  long TimeBaseLighting;//время базовой освещённости
  long TimeMaxLighting;//время максимальной освещённости
  long CounterChageLighting;//счётчик до смены освещённости
  bool Antigravitation;//есть ли антигравитация
  char Label[SECTOR_LABEL_SIZE];//метка
  
  vector<unsigned long> vector_WallIndex;//список стен сектора
  vector<unsigned long> vector_WallPortalIndex;//список стен-порталов сектора
 public:
  CSimplySector(void);//конструктор
  ~CSimplySector();//деструктор
  void Release(void);//освободить память
  void Initialize(void);//инициализация
  void Processing(void);//обработка
  bool CheckActivate(float x,float y,float length,long sector_index);//проверка, можно ли активировать сектор из координат (x,y) с максимальным расстоянием активации length
  void Load(FILE *file);//загрузка

  long GetIndex(void);//получить индекс сектора
  void SetIndex(long index);//задать индекс сектора
  long GetUp(void);//получить высоту потолка сектора
  long GetDown(void);//получить высоту пола сектора
  void GetRectangle(float &xb1,float &yb1,float &xb2,float &yb2);//получить ограничивающий прямоугольник
  vector<SPoint>* GetSectorPointVectorPtr(void);//получить указатель на вектор точек, задающих сектор
  long GetLighting(void);//получить освещённость сектора
  CTextureFollow* GetCTextureFollow_Down_Ptr(void);//получить указатель на тектуру пола
  CTextureFollow* GetCTextureFollow_Up_Ptr(void);//получить указатель на тектуру потолка
  bool GetAntigravitation(void);//получить есть ли антигравитация
  char *GetLabel(void);//получить метку сектора
  void CreateTextureList(vector<SUseTexture> &vector_SUseTexture);//получить список используемых текстур
  void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture);//заменить индексы текстур в соответствии со списком
  vector<unsigned long>* GetVectorWallIndex(void);//получить указатель на вектор индексов стен
  vector<unsigned long>* GetVectorWallPortalIndex(void);//получить указатель на вектор индексов стен-порталов

  void Activate(void);//активация
  void Lock(void);//заблокировать
  void Unlock(void);//разблокировать
  void SetDown(long value);//задать уровень пола
  void SetUp(long value);//задать уровень потолка
  long GetActivatedSectorIndex(void);//получить индекс сектора для активации
  bool GetTeleportCoord(long &x,long &y);//получить координаты телепортации
};

#endif