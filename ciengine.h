#ifndef CIENGINE_H
#define CIENGINE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//асбтрактный класс системы визуализации
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------
#include "cisector.h"

//----------------------------------------------------------------------------------------------------
//структуры
//----------------------------------------------------------------------------------------------------

//структура стен
struct SWall
{
 long X1;
 long Y1;
 long X2;
 long Y2;
 CTextureFollow cTextureFollow_Down;//нижняя текстура
 CTextureFollow cTextureFollow_Up;//верхняя текстура
 bool Frontier;//линия раздела=true
 bool Switch;//является ли стена переключателем
 char SwitchLabel[SECTOR_LABEL_SIZE];//метка сектора, с которым связан переключатель
 //специфические для порталов данные
 long Sector1;//номера секторов стены
 long Sector2;
 long Offset;//смещение текстуры
 long Len;//длина стены
 bool Visible;//видел ли игрок стену
 bool PortalEnabled;//разрешена ли обработка портала, если стена является порталом
};

class CIEngine
{
 public:
  virtual ~CIEngine() {};//деструктор
  virtual void SetPath(string path)=0;//указать рабочий каталог
  virtual void Init(void)=0;//инициализировать
  virtual void Close(void)=0;//завершить работу
  virtual void LoadMap(string filename)=0;//загрузить карту
  virtual void ReleaseMap(void)=0;//освободить память карты
  virtual long GetSectorIndexForPos(float x,float y)=0;//получение номера сектора из таблицы по координатам
  virtual long GetZPos(float x,float y)=0;//получение координаты Z
  virtual void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)=0;//отрисовать лабиринт
  virtual float GetCos(long angle)=0;//получить косинус угла из таблицы
  virtual float GetSin(long angle)=0;//получить синус угла из таблицы
  virtual void GetPlayerInitPos(float &x,float &y,long &angle)=0;//получить начальные координаты игрока
  virtual CISector *GetCISectorPtrForIndex(long sectorindex)=0;//получить указатель на сектор по его номеру
  virtual vector<SWall>* GetVectorSWallPtr(void)=0;//получить указатель на вектор стен
  virtual void Processing(void)=0;//обработка анимации
  virtual long GetIntersectionList(vector<long> *vector_collizion_ptr,float x,float y,float radius,float z,float height)=0;//получить список стен, с которыми пересекается объект 
};

#endif
