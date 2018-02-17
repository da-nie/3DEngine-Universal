#include "ccontrol.h"

extern CKeyboard cKeyboard;
extern CMouse cMouse;
extern long TimerUser;
extern CVideo cVideo;

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CControl::CControl()
{
 TickCounter=0;
 cIEngine_Ptr=new CEngine_SPortal;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CControl::~CControl()
{
 Close();
 delete(cIEngine_Ptr);
}
//---------------------------------------------------------------------------------------------------
//указать путь к файлам движка
//---------------------------------------------------------------------------------------------------
void CControl::SetPath(string path)
{
 Path=path;
}
//---------------------------------------------------------------------------------------------------
//инициализация
//----------------------------------------------------------------------------------------------------
void CControl::Init(void)
{
 cIEngine_Ptr->SetPath(Path);
 cIEngine_Ptr->Init();
 string filename=Path+"Data//3d.ptl";
 cIEngine_Ptr->LoadMap(filename);
 cPlayer.SetCIEngine(cIEngine_Ptr);
 cPlayer.Init();
 TickCounter=0;
 EngineName="Экранный портал";
}
//----------------------------------------------------------------------------------------------------
//завершение работы
//----------------------------------------------------------------------------------------------------
void CControl::Close(void)
{
 cIEngine_Ptr->ReleaseMap();
 cIEngine_Ptr->Close();
}
//----------------------------------------------------------------------------------------------------
//основной цикл работы
//----------------------------------------------------------------------------------------------------
void CControl::Processing(void)
{
 float px;
 float py;
 float pz;
 long pa;
 long pav;
 float pl;
 cPlayer.GetPos(px,py,pz,pa,pav);
 long height=cPlayer.GetHeight();
 float viewheight=cPlayer.GetViewHeight();
 pl=cPlayer.GetPowerLighting();

 //отрисовываем сцену
 cIEngine_Ptr->View(px,py,pz+height+viewheight,pa,pav,pl);

 //обрабатываем управление
 if (cKeyboard.GetPressKey(KEY_ESC)==true) return;
 if (cKeyboard.GetPressKey(KEY_S_PORTAL)==true)
 {
  delete(cIEngine_Ptr);
  cIEngine_Ptr=new CEngine_SPortal;
  cIEngine_Ptr->SetPath(Path);
  cIEngine_Ptr->Init();
  string filename=Path+"Data//3d.ptl";
  cIEngine_Ptr->LoadMap(filename);
  cPlayer.SetCIEngine(cIEngine_Ptr);
  EngineName="Экранный портал";
 }
 if (cKeyboard.GetPressKey(KEY_G_PORTAL)==true)
 {
  delete(cIEngine_Ptr);
  cIEngine_Ptr=new CEngine_GPortal;
  cIEngine_Ptr->SetPath(Path);
  cIEngine_Ptr->Init();
  string filename=Path+"Data//3d.ptl";
  cIEngine_Ptr->LoadMap(filename);
  cPlayer.SetCIEngine(cIEngine_Ptr);
  EngineName="Геометрический портал";
 }
 if (cKeyboard.GetPressKey(KEY_BSP)==true)
 {
  delete(cIEngine_Ptr);
  cIEngine_Ptr=new CEngine_BSP;
  cIEngine_Ptr->SetPath(Path);
  cIEngine_Ptr->Init();
  string filename=Path+"Data//3d.bsp";
  cIEngine_Ptr->LoadMap(filename);
  cPlayer.SetCIEngine(cIEngine_Ptr);
  EngineName="BSP-дерево";
 }
 if (cKeyboard.GetPressKey(KEY_SAVE_POS)==true)
 {
  cPlayer.SavePos();
 }
 if (cKeyboard.GetPressKey(KEY_LOAD_POS)==true)
 {
  cPlayer.LoadPos();
 }

 if (cKeyboard.GetPressKey(KEY_SIT)==true) cPlayer.SitDown();
                                      else cPlayer.StandUp();
 if (cKeyboard.GetPressKey(KEY_UP)==true)
 {
  cPlayer.GoUp();
  if (cKeyboard.GetPressKey(KEY_SPEED)==true) cPlayer.GoUp();
 }
 if (cKeyboard.GetPressKey(KEY_DOWN)==true)
 {
  cPlayer.GoDown();
  if (cKeyboard.GetPressKey(KEY_SPEED)==true) cPlayer.GoDown();
 }
 if (cKeyboard.GetPressKey(KEY_LEFT)==true)
 {
  cPlayer.RotateLeft();
  if (cKeyboard.GetPressKey(KEY_SPEED)==true) cPlayer.RotateLeft();
 }
 if (cKeyboard.GetPressKey(KEY_RIGHT)==true)
 {
  cPlayer.RotateRight();
  if (cKeyboard.GetPressKey(KEY_SPEED)==true) cPlayer.RotateRight();
 }
 if (cKeyboard.GetPressKey(KEY_USE)==true) cPlayer.Use();
 if (cKeyboard.GetPressKey(KEY_LOOK_UP)==true) cPlayer.LookUp();
 if (cKeyboard.GetPressKey(KEY_LOOK_DOWN)==true) cPlayer.LookDown();

 //реализуем управление от мышки
 cPlayer.GetLookParameter(pa,pav);
 pa+=cMouse.GetXOffset()/7.0;
 pav+=cMouse.GetYOffset()/10.0;
 cPlayer.SetLookParameter(pa,pav);

 //выполняем обработку всех объектов
 cPlayer.Processing();
 long ps=cPlayer.GetSectorIndex();
 cPlayer.GetPos(px,py,pz,pa,pav);
 long index=0;
 while(1)
 {
  CISector *cISector_Ptr=cIEngine_Ptr->GetCISectorPtrForIndex(index);
  if (cISector_Ptr==NULL) break;//все сектора обработаны
  if (cISector_Ptr->GetIndex()==ps && cISector_Ptr->GetUp()<=pz+PLAYER_EYE_TO_TOP_HEIGHT+height) cISector_Ptr->Activate();//поднимающийся сектор зажал игрока
  cISector_Ptr->Processing();
  index++;
 }

 TickCounter++;
 if (TickCounter>=5)
 {
  cIEngine_Ptr->Processing();
  TickCounter=0;
 }
 TickCounter++;

 //выводим название движка
 cVideo.PutString(0,0,EngineName.c_str(),0x00ffffff);
}
