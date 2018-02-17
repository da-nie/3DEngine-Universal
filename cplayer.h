#ifndef CPLAYER_H
#define CPLAYER_H

//максимальное расстояние активации секторов
#define MAX_LENGTH_TO_ACTIVATE 50
//высота игрока до глаз
#define PLAYER_HEIGHT 64
//радиус игрока
#define PLAYER_RADIUS 20
//приращение угла походки
#define PLAYER_MOVE_ANGLE_STEP 25
//максимальная высота подъема при ходьбе
#define PLAYER_MOVE_MAX_HEIGHT 2
//высота от глаз до затылка
#define PLAYER_EYE_TO_TOP_HEIGHT 10
//максимальный угол зрения с вертикалью
#define MAX_ANGLE_VERTICAL 50
//минимальный угол зрения с вертикалью
#define MIN_ANGLE_VERTICAL -50

#include "localmath.h"
#include "ciengine.h"

class CPlayer
{
 protected:
  CIEngine *cIEngine_Ptr;
  long Height;//высота игрока
  long Angle;//координаты и положение игрока в пространстве
  long AngleVertical;//угол обзора с вертикалью
  float X;//координаты игрока
  float Y;
  float Z;//низ игрока

  long Radius;//радиус игрока
  float DX;//изменения координат
  float DY;
  float DZ;//скорость падения игрока
  float SFB;//скорость перемещения
  float SRT;//скорость вращения
  bool OnMove;//true-было движение, false-нет(нужно для скольжения)
  bool OnRotate;//true-был поворот, false-нет(нужно для вращения)
  long SectorIndex;//сектор, в котором находится игрок(максимальный по высоте пол в радиусе игрока)
  float PowerLighting;//мощность фонаря
  long MoveAngle;//угол походки
 public:
  void SetCIEngine(CIEngine *cIEngine_Ptr_Set);//задать указатель на движок
  void Init(void);//инициализация
  void SitDown(void);//присесть
  void StandUp(void);//встать
  void GoUp(void);//идти вперёд
  void GoDown(void);//идти назад
  void StepUp(void);//сделать шаг вперёд
  void StepDown(void);//сделать шаг назад
  void RotateLeft(void);//повернуть налево
  void RotateRight(void);//повернуть направо
  void LookUp(void);//посмотреть вверх
  void LookDown(void);//посмотреть вниз
  void Use(void);//использовать
  void Processing(void);//обработка игрока
  void GetPos(float &x,float &y,float &z,long &angle,long &angle_vertical);//получить координаты игрока
  void SetLookParameter(long angle,long angle_vertical);//задать параметры обзора
  void GetLookParameter(long &angle,long &angle_vertical);//получить параметры обзора
  float GetPowerLighting(void);//получить мощность фонаря
  float GetViewHeight(void);//получить приращение высоты глаз игрока
  long GetSectorIndex(void);//получить номер сектора, в котором находится игрок (с учётом геометрии игрока)
  long GetHeight(void);//получить высоту игрока  
  void SavePos(void);//записать положение
  void LoadPos(void);//загрузить положение
 protected:
  long Intersection(vector<long> *vector_collizion_ptr);//получить список линий, с которыми пересекается игрок
  void CheckImpact(void);//проверка столкновения игрока со стенами
  long SlideTraverse(long wall);//скольжение игрока вдоль стены
  long ImpactLine(float xw1,float yw1,float xw2,float yw2);
  void FindPlayerSector(void);//найти сектор, в котором находится игрок (с учётом геометрии игрока)
};
#endif
 