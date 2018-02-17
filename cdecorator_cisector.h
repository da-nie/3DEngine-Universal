#ifndef CDECORATOR_CISECTOR_H
#define CDECORATOR_CISECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс-декоратор для CISector
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cisector.h"

class CDecorator_CISector:public CISector
{
 protected:
  CISector *cISector_Ptr;//декорируемый класс
 public:
  CDecorator_CISector(CISector *cISector_Set_Ptr);//конструктор
  ~CDecorator_CISector();//деструктор
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
 protected:
};

#endif



