#ifndef CENGINE_BASE_H
#define CENGINE_BASE_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//базовый класс системы визуализации
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------

#include "localmath.h"
#include "common.h"
#include "cunit.h"
#include "ciengine.h"
#include "cwallmap.h"

using namespace std;

#include "cisector.h"
#include "csimplysector.h"
#include "csimplydoor.h"
#include "csimplyplatform.h"
#include "csimplybridge.h"
#include "cswitchsector.h"
#include "csimplyteleport.h"

#include "cvideo.h"
#include "cgraph.h"
#include "ctexturefollow.h"


//----------------------------------------------------------------------------------------------------
//макроопределения
//----------------------------------------------------------------------------------------------------

//сдвиг для вычислений с плавающей точкой
#define FIXMATH_SHIFT 20
//сдвиг для вычислений с плавающей точкой для полов и потолков
#define FIXMATH_FLOOR_FLOW_SHIFT 16

//максимальная яркость сектора
#define MAX_BRIGHTNESS_VALUE 255

//размер сетки разбиения карты
#define GRID_X_STEP 128
#define GRID_Y_STEP 128


class CEngine_Base:public CIEngine
{
 protected:
  float CosTable[361];//таблица косинусов
  float SinTable[361];//таблица синусов

  float PlayerXInitPos;//начальная координата X игрока
  float PlayerYInitPos;//начальная координата Y игрока
  float PlayerAngleInit;//начальный угол игрока
  float PlayerX;//координаты игрока
  float PlayerY;
  float PlayerZ;
  long PlayerAngle;//угол поворота игрока
  long PlayerAngleVertical;//угол зрения игрока
  long PlayerPowerLighting;//мощность фонарика игрока
  long PlayerSectorIndex;//сектор, в котором находится игрок

  CGraphics cGraphics;//класс графики игры
  vector<CISector*> vector_CISectorPtr;//массив указателей на сектора по их индексу
  vector<CUnit*> vector_CUnitPtr;//массив объектов
  vector<SWall> vector_SWall;//массив стен

  long dcTable[361];//таблицы для горизонтального текстурирования
  long dsTable[361];
  long diTable[361];
  long djTable[361];

  long *TopLine;//линии горизонта
  long *BottomLine;
  long SetLine;//число заполненных столбцов экрана

  long *X_Table;//буфер начальных координат X для текстурирования полов и потолков

  struct SZLine
  {
   long Y_Top;//верх отрезка
   long Y_Bottom;//низ отрезка
   long Z;//значение глубины отрезка
  };

  vector< vector<SZLine> > ZLineBuffer;//буфер отрезков постоянной глубины

  //параметры видеорежима
  unsigned long *VideoBufferPtr;//видеобуфер
  unsigned long VideoLineWidth;//размер строки видеобуфера
  long WindowWidth;//ширина окна
  long WindowHeight;//высота окна
  long WindowXCenter;//координаты центра окна
  long WindowYCenter;//координаты центра окна
  long WindowYCenterWithOffset;//смещённые координаты центра окна

  //параметры текстурирования полов и потолков
  struct SVisualPlanes
  {
   long MinX;//минимальная координата X области
   long MaxX;//максимальная координата X области
   long TopY[WINDOW_WIDTH];//верхняя координата
   long BottomY[WINDOW_WIDTH];//нижняя координата
  };

  string Path;//рабочий каталог
  
  CWallMap *cWallMap_Ptr;//поле стен
 public:
  CEngine_Base();//конструктор
  ~CEngine_Base();//деструктор
  void SetPath(string path);//указать рабочий каталог
  void Init(void);//инициализировать
  void Close(void);//завершить работу
  void ReleaseMap(void);//освободить память карты
  long GetSectorIndexForPos(float x,float y);//получение номера сектора из таблицы по координатам
  long GetZPos(float x,float y);//получение координаты Z
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//отрисовать лабиринт
  float GetCos(long angle);//получить косинус угла из таблицы
  float GetSin(long angle);//получить синус угла из таблицы
  void GetPlayerInitPos(float &x,float &y,long &angle);//получить начальные координаты игрока
  CISector *GetCISectorPtrForIndex(long sectorindex);//получить указатель на сектор по его номеру
  vector<SWall>* GetVectorSWallPtr(void);//получить указатель на вектор стен

  void Processing(void);//обработка анимации
  long GetIntersectionList(vector<long> *vector_collizion_ptr,float x,float y,float radius,float z,float height);//получить список стен, с которыми пересекается объект 
 protected:
  void DrawFloorLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Bottom);//заполнение линии буфера текстур пола
  void DrawFlowLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Top);//заполнение линии буфера текстур потолка
  unsigned long* FindTexture(long scale,long texture_u,long texture_index,long *mask,long *texture_dv,long *texture_v);//поиск указателя на пиксель текстуры
  void DrawSimpleLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//рисование столбца монолитной стены
  void DrawLowerLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//рисование столбца нижней стены
  void DrawUpperLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom);//рисование столбца верхней стены
  void ClearFloorFlowBuffer(void);//очистка буфера полов и потолков
  void DrawFlow(long sector_index,const SVisualPlanes &sVisualPlanes_Top);//рисование потолков
  void DrawFloor(long sector_index,const SVisualPlanes &sVisualPlanes_Bottom);//рисование полов
  void DrawTextureLine(long dist,long scale,long sector_bright,long texture_index,long screen_y,long left_x,long right_x);//рисование текстурированной линии пола или потолка

  void PutSprite(void);//вывод спрайта
  void Line(long x1,long y1,long x2,long y2,unsigned long color);//рисование отрезка
  void PutPixel(long x,long y,unsigned long color);//вывод точки
  void SetZBufferVerticalLineValue(long x,long y1,long y2,long z);//заполнение строки в Z-буфере
};

#endif
