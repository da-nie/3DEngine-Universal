#ifndef CISECTOR_H
#define CISECTOR_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//абстрактный класс секторов
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <vector>
using namespace std;

#include "ctexturefollow.h"

//точка
struct SPoint
{
 float X;
 float Y;
};

//простой сектор
#define SECTOR_TYPE_SIMPLY          0
//обычная дверь
#define SECTOR_TYPE_DOOR            1
//переключатель
#define SECTOR_TYPE_SWITCH          2
//дверь от переключателя
#define SECTOR_TYPE_SWITCH_DOOR     3
//платформа
#define SECTOR_TYPE_PLATFORM        4
//платформа от переключателя
#define SECTOR_TYPE_SWITCH_PLATFORM 5
//выход с уровня
#define SECTOR_TYPE_END             6
//телепортатор
#define SECTOR_TYPE_TELEPORT        7
//мостик
#define SECTOR_TYPE_BRIDGE          8

//типы освещения
//равномерное освещение
#define LIGHTING_MODE_STATIC          0
//мигание
#define LIGHTING_MODE_FLASH           1
//случайное мигание
#define LIGHTING_MODE_RANDOM_FLASH    2
//затухание/нарастание
#define LIGHTING_MODE_DAWNING_SUNDOWN 3

//размер метки сектора
#define SECTOR_LABEL_SIZE 11

class CISector
{
 public:
  virtual ~CISector() {};//деструктор
  virtual void Release(void)=0;//освободить память
  virtual void Initialize(void)=0;//инициализация
  virtual void Processing(void)=0;//обработка
  virtual bool CheckActivate(float x,float y,float length,long sector_index)=0;//проверка, можно ли активировать сектор из координат (x,y) с максимальным расстоянием активации length
  virtual void Load(FILE *file)=0;//загрузка

  virtual long GetIndex(void)=0;//получить индекс сектора
  virtual void SetIndex(long index)=0;//задать индекс сектора
  virtual long GetUp(void)=0;//получить высоту потолка сектора
  virtual long GetDown(void)=0;//получить высоту пола сектора
  virtual void GetRectangle(float &xb1,float &yb1,float &xb2,float &yb2)=0;//получить ограничивающий прямоугольник
  virtual vector<SPoint>* GetSectorPointVectorPtr(void)=0;//получить указатель на вектор точек, задающих сектор
  virtual long GetLighting(void)=0;//получить освещённость сектора
  virtual CTextureFollow* GetCTextureFollow_Down_Ptr(void)=0;//получить указатель на тектуру пола
  virtual CTextureFollow* GetCTextureFollow_Up_Ptr(void)=0;//получить указатель на тектуру потолка
  virtual bool GetAntigravitation(void)=0;//получить есть ли антигравитация
  virtual char *GetLabel(void)=0;//получить метку сектора
  virtual void CreateTextureList(vector<SUseTexture> &vector_SUseTexture)=0;//получить список используемых текстур
  virtual void RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture)=0;//заменить индексы текстур в соответствии со списком
  virtual vector<unsigned long>* GetVectorWallIndex(void)=0;//получить указатель на вектор индексов стен
  virtual vector<unsigned long>* GetVectorWallPortalIndex(void)=0;//получить указатель на вектор индексов стен-порталов

  virtual void Activate(void)=0;//активация
  virtual void Lock(void)=0;//заблокировать
  virtual void Unlock(void)=0;//разблокировать
  virtual void SetDown(long value)=0;//задать уровень пола
  virtual void SetUp(long value)=0;//задать уровень потолка
  virtual long GetActivatedSectorIndex(void)=0;//получить индекс сектора для активации
  virtual bool GetTeleportCoord(long &x,long &y)=0;//получить координаты телепортации
};
#endif 