#include "cplayer.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//открытые функции
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//задать указатель на движок
//----------------------------------------------------------------------------------------------------
void CPlayer::SetCIEngine(CIEngine *cIEngine_Ptr_Set)
{
 cIEngine_Ptr=cIEngine_Ptr_Set;
}
//----------------------------------------------------------------------------------------------------
//инициализация
//----------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
 PowerLighting=1;
 cIEngine_Ptr->GetPlayerInitPos(X,Y,Angle);
 Z=0;
 FindPlayerSector();
 if (SectorIndex>=0)
 {
  CISector *cISectorPtr=cIEngine_Ptr->GetCISectorPtrForIndex(SectorIndex);
  if (cISectorPtr!=NULL) Z=cISectorPtr->GetDown();
 }
 Radius=PLAYER_RADIUS;
 Height=PLAYER_HEIGHT;
 DX=0;
 DY=0;
 DZ=0;
 SFB=0;
 SRT=0;
 OnMove=false;
 OnRotate=false;
 MoveAngle=0;
 AngleVertical=0;
}
//----------------------------------------------------------------------------------------------------
//присесть
//----------------------------------------------------------------------------------------------------
void CPlayer::SitDown(void)
{
 if (Height>PLAYER_HEIGHT/2) Height-=4;
                        else Height=PLAYER_HEIGHT/2;
}
//----------------------------------------------------------------------------------------------------
//встать
//----------------------------------------------------------------------------------------------------
void CPlayer::StandUp(void)
{
 if (Height<PLAYER_HEIGHT)
 {
  FindPlayerSector();
  if (SectorIndex<0) return;
  CISector *cISectorPtr=cIEngine_Ptr->GetCISectorPtrForIndex(SectorIndex);
  if (cISectorPtr==NULL) return;
  float top=cISectorPtr->GetUp();
  float bottom=cISectorPtr->GetDown();
  long h=Height;
  Height+=4;
  if (bottom+Height+PLAYER_EYE_TO_TOP_HEIGHT>=top) Height=h;//упираемся головой
 }
 else Height=PLAYER_HEIGHT;
}
//----------------------------------------------------------------------------------------------------
//сделать шаг вперёд
//----------------------------------------------------------------------------------------------------
void CPlayer::StepUp(void)
{
 DX=cIEngine_Ptr->GetSin(Angle);
 DY=cIEngine_Ptr->GetCos(Angle);
 for(long n=0;n<SFB;n++) CheckImpact();
}
//----------------------------------------------------------------------------------------------------
//идти вперёд
//----------------------------------------------------------------------------------------------------
void CPlayer::GoUp(void)
{
 MoveAngle+=PLAYER_MOVE_ANGLE_STEP;
 MoveAngle%=360;
 if (SFB<0) SFB=0;
 SFB++;
 if (SFB>8) SFB=8;
 StepUp();
 OnMove=true;
}
//----------------------------------------------------------------------------------------------------
//сделать шаг назад
//----------------------------------------------------------------------------------------------------
void CPlayer::StepDown(void)
{
 DX=-cIEngine_Ptr->GetSin(Angle);
 DY=-cIEngine_Ptr->GetCos(Angle);
 for(long n=0;n>SFB;n--) CheckImpact();
}
//----------------------------------------------------------------------------------------------------
//идти назад
//----------------------------------------------------------------------------------------------------
void CPlayer::GoDown(void)
{
 MoveAngle+=PLAYER_MOVE_ANGLE_STEP;
 MoveAngle%=360;
 if (SFB>0) SFB=0;
 SFB--;
 if (SFB<-8) SFB=-8;
 StepDown();
 OnMove=true;
}
//----------------------------------------------------------------------------------------------------
//повернуть налево
//----------------------------------------------------------------------------------------------------
void CPlayer::RotateLeft(void)
{
 if (SRT>0) SRT=0;
 SRT--;
 if (SRT<-6) SRT=-6;
 Angle+=static_cast<long>(SRT);
 while (Angle<0) Angle+=360;
 OnRotate=true;
}
//----------------------------------------------------------------------------------------------------
//повернуть направо
//----------------------------------------------------------------------------------------------------
void CPlayer::RotateRight(void)
{
 if (SRT<0) SRT=0;
 SRT++;
 if (SRT>6) SRT=6;
 Angle+=static_cast<long>(SRT);
 while (Angle>360) Angle-=360;
 OnRotate=true;
}
//----------------------------------------------------------------------------------------------------
//посмотреть вверх
//----------------------------------------------------------------------------------------------------
void CPlayer::LookUp(void)
{
 AngleVertical+=15;
 if (AngleVertical>MAX_ANGLE_VERTICAL) AngleVertical=MAX_ANGLE_VERTICAL;
}
//----------------------------------------------------------------------------------------------------
//посмотреть вниз
//----------------------------------------------------------------------------------------------------
void CPlayer::LookDown(void)
{
 AngleVertical-=15;
 if (AngleVertical<MIN_ANGLE_VERTICAL) AngleVertical=MIN_ANGLE_VERTICAL;
}

//----------------------------------------------------------------------------------------------------
//обработка игрока
//----------------------------------------------------------------------------------------------------
void CPlayer::Processing(void)
{ 
 DX=0;
 DY=0;

 FindPlayerSector();
 if (SectorIndex<0) return;
 CISector *cISectorPtr=cIEngine_Ptr->GetCISectorPtrForIndex(SectorIndex);
 if (cISectorPtr==NULL) return;
 float z=cISectorPtr->GetDown();
 float Z_Old=Z;
 if (cISectorPtr->GetAntigravitation()==true)
 {
  Z+=9.8f;
  if (Intersection(NULL)!=0) Z=Z_Old;
  if (cISectorPtr->GetUp()<Z+Height+PLAYER_EYE_TO_TOP_HEIGHT+5) Z=cISectorPtr->GetUp()-PLAYER_EYE_TO_TOP_HEIGHT-Height-6;
 }
 else
 {
  if (Z>z)
  {
   DZ+=9.8f;
   if (DZ>200) DZ=200;
   Z-=DZ;
   if (Intersection(NULL)!=0)
   {
    Z=Z_Old;
    DZ=0;
   }
  } 
 }

 if (Z<=z)//падение игрока
 {
  Z=z;
  DZ=0;
 }
 //перемещение по инерции
 if (SFB<0 && OnMove==false)
 {
  StepDown();
  SFB++;
 }
 if (SFB>0 && OnMove==false)
 {
  StepUp();
  SFB--;
 }
 /*
 //шатание по инерции
 if (OnMove==false)
 {
  if (MoveAngle%180!=0)
  {
   MoveAngle+=PLAYER_MOVE_ANGLE_STEP;
   MoveAngle%=360;
  }
  else MoveAngle=0;
 }
 */
 if (OnRotate=false) SRT=0;
 OnMove=false;
 OnRotate=false;
}
//----------------------------------------------------------------------------------------------------
//использовать
//----------------------------------------------------------------------------------------------------
void CPlayer::Use(void)
{
 //смотрим, что бы мы могли бы активировать
 long index=0;
 while(1)
 {
  CISector *cISector_Ptr=cIEngine_Ptr->GetCISectorPtrForIndex(index);
  if (cISector_Ptr==NULL) break;
  if (cISector_Ptr->CheckActivate(X,Y,MAX_LENGTH_TO_ACTIVATE,SectorIndex)==true)//сектор может быть активирован
  {
   cISector_Ptr->Activate();
   long x;
   long y;
   if (cISector_Ptr->GetTeleportCoord(x,y)==true)//если этот сектор может телепортировать
   { 
    //переносим игрока
    X=static_cast<long>(x);
    Y=static_cast<long>(y);
    FindPlayerSector();
    if (SectorIndex>=0)
    {
     CISector *cISectorPtr=cIEngine_Ptr->GetCISectorPtrForIndex(SectorIndex);
     if (cISectorPtr!=NULL) Z=cISectorPtr->GetDown();
    }
   }
  }
  index++;
 }

 //активация переключателя
 vector<SWall> *vector_SWallPtr=cIEngine_Ptr->GetVectorSWallPtr();
 long size=vector_SWallPtr->size();
 float xp1=X;
 float yp1=Y;
 float xp2=xp1+2*Radius*sin(M_PI/180.0*Angle);
 float yp2=yp1+2*Radius*cos(M_PI/180.0*Angle);
 for(long n=0;n<size;n++)
 {  
  SWall *sWall_Ptr=&((*vector_SWallPtr)[n]);
  if (sWall_Ptr->Switch==true)//это стена-переключатель
  {
   //проверяем, можем ли мы её активировать
   MATH_RETURN_CODE mrc=CheckCollizionSegment(sWall_Ptr->X1,sWall_Ptr->Y1,sWall_Ptr->X2,sWall_Ptr->Y2,xp1,yp1,xp2,yp2);
   if (mrc==MATH_RETURN_CODE_COLLIZION)//активировать можем
   {
    //меняем текстуру переключателю
    sWall_Ptr->cTextureFollow_Down.GoToEndTexture();
	sWall_Ptr->cTextureFollow_Up.GoToEndTexture();
    //ищем среди всех секторов связанный с переключателем сектор
    long index=0;
    while(1)
	{
     CISector *cISector_Ptr=cIEngine_Ptr->GetCISectorPtrForIndex(index);
     if (cISector_Ptr==NULL) break;
     if (strcmp(cISector_Ptr->GetLabel(),sWall_Ptr->SwitchLabel)==0)
     {
      cISector_Ptr->Unlock();
      cISector_Ptr->Activate();
     }
	 index++;
	}
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//получить координаты игрока
//----------------------------------------------------------------------------------------------------
void CPlayer::GetPos(float &x,float &y,float &z,long &angle,long &angle_vertical)
{
 x=X;
 y=Y;
 z=Z;
 angle=Angle;
 angle_vertical=AngleVertical;
}
//----------------------------------------------------------------------------------------------------
//задать параметры обзора
//----------------------------------------------------------------------------------------------------
void CPlayer::SetLookParameter(long angle,long angle_vertical)
{
 Angle=angle;
 AngleVertical=angle_vertical;
 while(Angle<0) Angle+=360;
 while(Angle>=360) Angle-=360;
 if (AngleVertical>MAX_ANGLE_VERTICAL) AngleVertical=MAX_ANGLE_VERTICAL;
 if (AngleVertical<MIN_ANGLE_VERTICAL) AngleVertical=MIN_ANGLE_VERTICAL;
}
//----------------------------------------------------------------------------------------------------
//получить параметры обзора
//----------------------------------------------------------------------------------------------------
void CPlayer::GetLookParameter(long &angle,long &angle_vertical)
{
 angle=Angle;
 angle_vertical=AngleVertical;
}


//----------------------------------------------------------------------------------------------------
//получить мощность фонаря
//----------------------------------------------------------------------------------------------------
float CPlayer::GetPowerLighting(void)
{
 return(PowerLighting);
}
//----------------------------------------------------------------------------------------------------
//получить приращение высоты глаз игрока
//----------------------------------------------------------------------------------------------------
float CPlayer::GetViewHeight(void)
{
 return(sin(M_PI/180*MoveAngle)*PLAYER_MOVE_MAX_HEIGHT);
}

//----------------------------------------------------------------------------------------------------
//получить номер сектора, в котором находится игрок
//----------------------------------------------------------------------------------------------------
long CPlayer::GetSectorIndex(void)
{
 return(SectorIndex);
}
//----------------------------------------------------------------------------------------------------
//получить высоту игрока
//----------------------------------------------------------------------------------------------------
long CPlayer::GetHeight(void)
{
 return(Height);
}
//----------------------------------------------------------------------------------------------------
//записать положение
//----------------------------------------------------------------------------------------------------
void CPlayer::SavePos(void)
{
 FILE *file=fopen("position.bin","wb");
 if (file!=NULL)
 {
  fwrite(&X,1,sizeof(float),file);
  fwrite(&Y,1,sizeof(float),file);
  fwrite(&Z,1,sizeof(float),file);
  fwrite(&Angle,1,sizeof(long),file);
  fwrite(&AngleVertical,1,sizeof(long),file);
  fclose(file);
 }
}
//----------------------------------------------------------------------------------------------------
//загрузить положение
//----------------------------------------------------------------------------------------------------
void CPlayer::LoadPos(void)
{
 FILE *file=fopen("position.bin","rb");
 if (file!=NULL)
 {
  fread(&X,1,sizeof(float),file);
  fread(&Y,1,sizeof(float),file);
  fread(&Z,1,sizeof(float),file);
  fread(&Angle,1,sizeof(long),file);
  fread(&AngleVertical,1,sizeof(long),file);
  fclose(file);
 }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//закрытые функции
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
long CPlayer::ImpactLine(float xw1,float yw1,float xw2,float yw2)
{
 double xl1=xw1-X;
 double yl1=yw1-Y;
 double xl2=xw2-X;
 double yl2=yw2-Y;
 double a=xl2-xl1;
 double b=yl2-yl1;
 double kd=a*a+b*b;
 double ld=2*a*xl1+2*b*yl1;
 double md=xl1*xl1+yl1*yl1-Radius*Radius*4;
 double d=ld*ld-4*kd*md;//дескриминант
 if (d<0) return(0);//нет пересечения
 double sd=sqrt(d);
 double l1=(-ld+sd)/(2*kd);
 double l2=(-ld-sd)/(2*kd);
 double xp1=xl1+l1*a;
 double yp1=yl1+l1*b;
 double xp2=xl1+l2*a;
 double yp2=yl1+l2*b;//2 точки пересечения
 if (((xp1>=xl1 && xp1<=xl2) || (xp1>=xl2 && xp1<=xl1)) &&
 ((yp1>=yl1 && yp1<=yl2) || (yp1>=yl2 && yp1<=yl1))) return(1);
 if (((xp2>=xl1 && xp2<=xl2) || (xp2>=xl2 && xp2<=xl1)) &&
 ((yp2>=yl1 && yp2<=yl2) || (yp2>=yl2 && yp2<=yl1))) return(1);
 return(0);
}
//----------------------------------------------------------------------------------------------------
//определение сектора, в котором находится игрок
//----------------------------------------------------------------------------------------------------
void CPlayer::FindPlayerSector(void)
{
 long RSector[9];
 long ZPos[9];
 for(long a=0;a<8;a++)
 {
  float x=X+cIEngine_Ptr->GetSin(a*45)*(Radius-2.0f);
  float y=Y+cIEngine_Ptr->GetCos(a*45)*(Radius-2.0f);
  RSector[a]=cIEngine_Ptr->GetSectorIndexForPos(x,y);
  ZPos[a]=cIEngine_Ptr->GetZPos(x,y);
 }
 RSector[8]=cIEngine_Ptr->GetSectorIndexForPos(X,Y);//добавим центральный сектор
 ZPos[8]=cIEngine_Ptr->GetZPos(X,Y);
 long sector=8;
 for(long n=0;n<9;n++)
 {
  if (RSector[n]<0) continue;
  long Down=ZPos[n];
  if (RSector[sector]<0) sector=n;
  if (Down>=ZPos[sector]) sector=n;
 }
 SectorIndex=RSector[sector];
}
//----------------------------------------------------------------------------------------------------
//получить список линий, с которыми пересекается игрок
//----------------------------------------------------------------------------------------------------
long CPlayer::Intersection(vector<long> *vector_collizion_ptr)
{
 return(cIEngine_Ptr->GetIntersectionList(vector_collizion_ptr,X+DX,Y+DY,Radius,Z,Height));
}
//----------------------------------------------------------------------------------------------------
//отработка столкновения игрока со стенами
//----------------------------------------------------------------------------------------------------
void CPlayer::CheckImpact(void)
{
 float dx=DX;
 float dy=DY;
 vector<long> vector_collizion;
 vector_collizion.clear();
 Intersection(&vector_collizion);
 long size=vector_collizion.size();
 for(long n=0;n<size;n++)
 {
  DX=dx;
  DY=dy;
  if (SlideTraverse(vector_collizion[n])==0) break;
 }
 X+=DX;
 Y+=DY;
 FindPlayerSector();
}
//----------------------------------------------------------------------------------------------------
//скольжение игрока вдоль стены
//----------------------------------------------------------------------------------------------------
long CPlayer::SlideTraverse(long wall)
{
 vector<SWall> *vector_SWallPtr=cIEngine_Ptr->GetVectorSWallPtr();
 if (vector_SWallPtr->size()<=static_cast<unsigned long>(wall)) return(0);
 SWall sWall=(*vector_SWallPtr)[wall];
 double xw1=sWall.X1;
 double yw1=sWall.Y1;
 double xw2=sWall.X2;
 double yw2=sWall.Y2;
 double vx=xw2-xw1;
 double vy=yw2-yw1;
 double ux=DX;
 double uy=DY;
 double normav=sqrt(vx*vx+vy*vy);
 double normau=sqrt(ux*ux+uy*uy);
 vx=vx/normav;
 vy=vy/normav;
 if (normau!=0)
 {
  ux=ux/normau;
  uy=uy/normau;
 }
 double cosa=ux*vx+uy*vy;//косинус угла между векторами
 double modul=fabs(cosa);
 if (cosa>0)
 {
  DX=vx*modul;
  DY=vy*modul;
 }
 else
 {
  DX=-vx*modul;
  DY=-vy*modul;
 }
 long w=Intersection(NULL);
 if (w>0)
 {
  DX=0;
  DY=0;
 }
 return(w);
}

