#ifndef CENGINE_BSP_H
#define CENGINE_BSP_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//класс системы визуализации с использованием BSP-дерева
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//подключаемые библиотеки
//----------------------------------------------------------------------------------------------------
#include <vector>
using namespace std;

#include "cengine_base.h"

//----------------------------------------------------------------------------------------------------
//макроопределени€
//----------------------------------------------------------------------------------------------------

//это лист дерева
#define BSP_ATOM  (1<<0)
//левые ветки есть
#define BSP_LEFT  (1<<1)
//правые ветки есть
#define BSP_RIGHT (1<<2)

//----------------------------------------------------------------------------------------------------
//структуры
//----------------------------------------------------------------------------------------------------

struct SBSPNode
{
 SBSPNode* sBSPNode_LeftPtr;//левое поддерево
 SBSPNode* sBSPNode_RightPtr;//правое поддерево
 long SegmentIndex;//индекс сегмента
 unsigned char Split;//что за лини€ (1-лини€ раздела,0-сегмент)
 float LX1,LX2,LY1,LY2;//левый описывающий пр€моугольник
 float RX1,RX2,RY1,RY2;//правый описывающий пр€моугольник
};


class CEngine_BSP:public CEngine_Base
{
 protected:
  //структура сегментов
  struct SSegment
  {
   float X1;
   float Y1;
   float X2;
   float Y2;
   long Offset;//смещение текстуры
   long Sector1;//номера секторов линии
   long Sector2;
   long Len;//длина сегмента
   bool Visible;//видел ли игрок сегмент
   long WallIndex;//индекс стены  
  };
  vector<SSegment> vector_SSegment;//массив сегментов
  SBSPNode* sBSPNode_Ptr;//указатель на корень BSP-дерева
 public:
  CEngine_BSP();//конструктор
  ~CEngine_BSP();//деструктор
  void LoadMap(string filename);//загрузить карту
  void ReleaseMap(void);//освободить пам€ть карты
  void View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting);//отрисовать лабиринт
 protected:
  SBSPNode* LoadBSPTree(FILE *file);//загрузка ветви дерева
  void DeleteBSPTree(SBSPNode *sBSPNode_RootPtr);//удаление дерева
  long SeenBox(SBSPNode* tree,long type);//проверка видимости описывающего пр€моугольника
  inline void DrawBSPTree(SBSPNode* tree);//обход BSP дерева и прорисовка лабиринта
  void DrawBSPLeafe(SBSPNode* root);//отрисогвка сегментов  стен
};

#endif 