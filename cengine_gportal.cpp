#include "cengine_gportal.h"

//#define DEBUG

extern CVideo cVideo;
//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CEngine_GPortal::CEngine_GPortal():CEngine_Base()
{
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CEngine_GPortal::~CEngine_GPortal()
{
}
//----------------------------------------------------------------------------------------------------
//загрузить карту
//----------------------------------------------------------------------------------------------------
void CEngine_GPortal::LoadMap(string filename)
{
 long n;
 ReleaseMap();
 vector<SUseTexture> vector_SUseTexture;//список используемых текстур

 FILE *file=fopen(filename.c_str(),"rb");
 if (file==NULL) return;
 //считываем параметры начальной позиции игрока
 PlayerXInitPos=LoadFloat(file);
 PlayerYInitPos=LoadFloat(file);
 PlayerAngleInit=LoadFloat(file);

 //загружаем стены
 long wall_amount=LoadLong(file);
 for(n=0;n<wall_amount;n++)
 {
  SWall sWall;
  sWall.X1=LoadFloat(file);
  sWall.Y1=LoadFloat(file);
  sWall.X2=LoadFloat(file);
  sWall.Y2=LoadFloat(file);
  sWall.Frontier=true;
  if (LoadUChar(file)==0) sWall.Frontier=false;
  sWall.cTextureFollow_Up.Load(file);
  sWall.cTextureFollow_Down.Load(file);
  sWall.Switch=false;
  strcpy(sWall.SwitchLabel,"");
  if (LoadUChar(file)!=0)
  {
   sWall.Switch=true;
   LoadCharArray(file,sWall.SwitchLabel,SECTOR_LABEL_SIZE);
  }
  sWall.cTextureFollow_Down.CreateTextureList(vector_SUseTexture);
  sWall.cTextureFollow_Up.CreateTextureList(vector_SUseTexture);
  sWall.Visible=false;
  sWall.Offset=0;
  sWall.Len=static_cast<long>(sqrt((sWall.X2-sWall.X1)*(sWall.X2-sWall.X1)+(sWall.Y2-sWall.Y1)*(sWall.Y2-sWall.Y1)));
  sWall.Sector1=LoadLong(file);
  sWall.Sector2=LoadLong(file);
  vector_SWall.push_back(sWall);
 }
 //загружаем сектора
 long sector_amount=LoadLong(file);
 for(n=0;n<sector_amount;n++)
 {
  //считываем тип сектора
  long type=LoadLong(file);
  //создаём простой сектор
  CISector *cISector_Ptr=new CSimplySector;
  cISector_Ptr->SetIndex(n);
  //декорируем созданный сектор в зависимости от его типа
  if (type==SECTOR_TYPE_SIMPLY) {};//ничего не делаем, такой сектор создан изначально
  if (type==SECTOR_TYPE_DOOR || type==SECTOR_TYPE_SWITCH_DOOR) cISector_Ptr=new CSimplyDoor(cISector_Ptr);
  if (type==SECTOR_TYPE_PLATFORM || type==SECTOR_TYPE_SWITCH_PLATFORM) cISector_Ptr=new CSimplyPlatform(cISector_Ptr);
  if (type==SECTOR_TYPE_BRIDGE) cISector_Ptr=new CSimplyBridge(cISector_Ptr);
  if (type==SECTOR_TYPE_TELEPORT) cISector_Ptr=new CSimplyTeleport(cISector_Ptr);
  //загружаем параметры сектора и инициализируем его
  cISector_Ptr->Load(file);
  cISector_Ptr->Initialize();
  if (type==SECTOR_TYPE_SWITCH_DOOR) cISector_Ptr->Lock();
  if (type==SECTOR_TYPE_SWITCH_PLATFORM) cISector_Ptr->Lock();
  if (type==SECTOR_TYPE_BRIDGE) cISector_Ptr->Lock();

  vector_CISectorPtr.push_back(cISector_Ptr);
  cISector_Ptr->CreateTextureList(vector_SUseTexture);//просим сектор указать используемые текстуры
 }
 fclose(file);
 //загружаем графику
 cGraphics.SetPath(Path);
 cGraphics.Init(vector_SUseTexture);
 //меняем индексы всех текстур у всех секторов и стен
 for(n=0;n<wall_amount;n++)
 {
  vector_SWall[n].cTextureFollow_Down.RefreshTextureIndex(vector_SUseTexture);
  vector_SWall[n].cTextureFollow_Up.RefreshTextureIndex(vector_SUseTexture);
 }
 long size=vector_CISectorPtr.size();
 for(n=0;n<size;n++) vector_CISectorPtr[n]->RefreshTextureIndex(vector_SUseTexture);
 //создаём список стен на сетке
 cWallMap_Ptr->Create(vector_SWall);
}

//----------------------------------------------------------------------------------------------------
//отрисовать лабиринт
//----------------------------------------------------------------------------------------------------
void CEngine_GPortal::View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)
{
 CEngine_Base::View(px,py,pz,pangle,panglevertical,powerlighting);

 #ifdef DEBUG
 for(long y=0;y<WINDOW_HEIGHT;y++) memset((unsigned char*)(VideoBufferPtr+y*VideoLineWidth),0,sizeof(unsigned long)*VideoLineWidth);
 #endif

 //разрешаем использование всех порталов
 long size=vector_SWall.size();
 for(long n=0;n<size;n++) vector_SWall[n].PortalEnabled=true;

 SPortal sPortal;
 sPortal.SectorIndex=GetSectorIndexForPos(PlayerX,PlayerY);
 sPortal.PlayerSectorIndex=sPortal.SectorIndex;
 sPortal.X=px;
 sPortal.Y=py;
 sPortal.L_X=(float)(1000.0*sin(M_PI/180*(pangle-80)));
 sPortal.L_Y=(float)(1000.0*cos(M_PI/180*(pangle-80)));
 sPortal.R_X=(float)(1000.0*sin(M_PI/180*(pangle+80)));
 sPortal.R_Y=(float)(1000.0*cos(M_PI/180*(pangle+80)));

 sPortal.X1=(float)(1000.0*sin(M_PI/180*(pangle-90))+px);
 sPortal.Y1=(float)(1000.0*cos(M_PI/180*(pangle-90))+py);
 sPortal.X2=(float)(1000.0*sin(M_PI/180*(pangle+90))+px);
 sPortal.Y2=(float)(1000.0*cos(M_PI/180*(pangle+90))+py);

 #ifdef DEBUG
 Line(sPortal.X1-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y1-PlayerY+WindowYCenter),sPortal.X2-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y2-PlayerY+WindowYCenter),RGB(0,255,0));
 Line(WindowXCenter,WINDOW_HEIGHT-(WindowYCenter),100*sin(M_PI/180*PlayerAngle)+WindowXCenter,WINDOW_HEIGHT-(100*cos(M_PI/180*PlayerAngle)+WindowYCenter),RGB(0,255,0));
 #endif

 sPortal.RR=255;
 sPortal.RG=255;
 sPortal.RB=0;

 sPortal.LR=0;
 sPortal.LG=255;
 sPortal.LB=255;

 Draw(sPortal);//рисуем сцену
 //PutSprite();
}
//----------------------------------------------------------------------------------------------------
//отсечение стены по порталу
//----------------------------------------------------------------------------------------------------
bool CEngine_GPortal::ClipWall(float &x1,float &y1,float &x2,float &y2,float &toffset,const SPortal &sPortal)
{
 //отсекаем по области видимости
 MATH_RETURN_CODE v1=GetSide(x1,y1,sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2);
 MATH_RETURN_CODE v2=GetSide(x2,y2,sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2);
 if (v1==MATH_RETURN_CODE_SIDE_RIGHT && v2==MATH_RETURN_CODE_SIDE_RIGHT) return(false);//стена не видна
 if (v1==MATH_RETURN_CODE_SIDE_RIGHT)
 {
  float xo,yo;
  if (Intersection(sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2,x1,y1,x2,y2,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
  {
   float dx=(xo-x1);
   float dy=(yo-y1);
   toffset+=sqrt(dx*dx+dy*dy);
   x1=xo;
   y1=yo;
  }
 }
 if (v2==MATH_RETURN_CODE_SIDE_RIGHT)
 {
  float xo,yo;
  if (Intersection(sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2,x1,y1,x2,y2,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
  {
   x2=xo;
   y2=yo;
  }
 }
 //отсекаем сегмент по порталу
 MATH_RETURN_CODE v1_left=GetSide(x1,y1,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);
 MATH_RETURN_CODE v2_left=GetSide(x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);

 MATH_RETURN_CODE v1_right=GetSide(x1,y1,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);
 MATH_RETURN_CODE v2_right=GetSide(x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);

 //точки слева от левой границы портала
 if ((v1_left==MATH_RETURN_CODE_SIDE_LEFT || v1_left==MATH_RETURN_CODE_SIDE_IDENTITY) &&
     (v2_left==MATH_RETURN_CODE_SIDE_LEFT || v2_left==MATH_RETURN_CODE_SIDE_IDENTITY)) return(false);
 //точки справа от правой границы портала
 if ((v1_right==MATH_RETURN_CODE_SIDE_RIGHT || v1_right==MATH_RETURN_CODE_SIDE_IDENTITY) &&
     (v2_right==MATH_RETURN_CODE_SIDE_RIGHT || v2_right==MATH_RETURN_CODE_SIDE_IDENTITY)) return(false);

 //----------------------------------------------------------------------------------------------------
 //точки лежат внутри границ портала - обработка не требуется
 //----------------------------------------------------------------------------------------------------
 if ((v1_left==MATH_RETURN_CODE_SIDE_RIGHT || v1_left==MATH_RETURN_CODE_SIDE_IDENTITY) && (v2_left==MATH_RETURN_CODE_SIDE_RIGHT || v2_left==MATH_RETURN_CODE_SIDE_IDENTITY) &&
    (v1_right==MATH_RETURN_CODE_SIDE_LEFT || v1_right==MATH_RETURN_CODE_SIDE_IDENTITY) && (v2_right==MATH_RETURN_CODE_SIDE_LEFT || v2_right==MATH_RETURN_CODE_SIDE_IDENTITY)) return(true);
 //сделаем так, чтобы точка 1 была левее точки 2
 bool change=false;
 if (v1_left==MATH_RETURN_CODE_SIDE_RIGHT || v1_left==MATH_RETURN_CODE_SIDE_IDENTITY)
 {
  if (v2_left==MATH_RETURN_CODE_SIDE_LEFT || v2_left==MATH_RETURN_CODE_SIDE_IDENTITY) change=true;
 }
 if (v1_right==MATH_RETURN_CODE_SIDE_RIGHT || v1_right==MATH_RETURN_CODE_SIDE_IDENTITY)
 {
  if (v2_right==MATH_RETURN_CODE_SIDE_LEFT || v2_right==MATH_RETURN_CODE_SIDE_IDENTITY) change=true;
 }
 if (change==true)
 {
  float x1_tmp=x1;
  float y1_tmp=y1;
  x1=x2;
  y1=y2;
  x2=x1_tmp;
  y2=y1_tmp;
 }

 MATH_RETURN_CODE mrc_left=CheckCollizionSegmentAndRay(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);
 MATH_RETURN_CODE mrc_right=CheckCollizionSegmentAndRay(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);
 if (mrc_left==MATH_RETURN_CODE_COLLIZION)
 {
  float xo,yo;
  if (Intersection(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
  {
   if (change==false)
   {
    float dx=(xo-x1);
    float dy=(yo-y1);
    toffset+=sqrt(dx*dx+dy*dy);
   }
   x1=xo;
   y1=yo;
  }
 }
 if (mrc_right==MATH_RETURN_CODE_COLLIZION)
 {
  float xo,yo;
  if (Intersection(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
  {
   if (change==true)
   {
    float dx=(xo-x2);
    float dy=(yo-y2);
    toffset+=sqrt(dx*dx+dy*dy);
   }
   x2=xo;
   y2=yo;
  }
 }

 if (change==true)
 {
  float x1_tmp=x1;
  float y1_tmp=y1;
  x1=x2;
  y1=y2;
  x2=x1_tmp;
  y2=y1_tmp;
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//отрисовка стен сектора
//----------------------------------------------------------------------------------------------------
void CEngine_GPortal::DrawSectorWall(const SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 long sector_index=sPortal.SectorIndex;
 if (sector_index<0 || sector_index>=vector_CISectorPtr.size()) return;//таких индексов не бывает
 long n;
 long size;
 vector<unsigned long>* vector_WallIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallIndex();
 vector<unsigned long>* vector_WallPortalIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallPortalIndex();

 //выводим все порталы сектора
 size=vector_WallPortalIndex_Ptr->size();
 for(n=0;n<size;n++)
 {
  long index=(*vector_WallPortalIndex_Ptr)[n];
  if (vector_SWall[index].PortalEnabled==false) continue;//портал уже был заблокирован
  SWall sWall=vector_SWall[index];
  float x1=sWall.X1;
  float x2=sWall.X2;
  float y1=sWall.Y1;
  float y2=sWall.Y2;
  long length=sWall.Len;
  float toffset=sWall.Offset;

  if (ClipWall(x1,y1,x2,y2,toffset,sPortal)==false) continue;
  DrawWall(x1,y1,x2,y2,toffset,length,sWall,sVisualPlanes_Top,sVisualPlanes_Bottom);
 }

 //выводим все стены сектора
 size=vector_WallIndex_Ptr->size();
 for(n=0;n<size;n++)
 {
  long index=(*vector_WallIndex_Ptr)[n];

  SWall sWall=vector_SWall[index];
  float x1=sWall.X1;
  float x2=sWall.X2;
  float y1=sWall.Y1;
  float y2=sWall.Y2;
  long length=sWall.Len;
  float toffset=sWall.Offset;

  if (ClipWall(x1,y1,x2,y2,toffset,sPortal)==false) continue;
  DrawWall(x1,y1,x2,y2,toffset,length,sWall,sVisualPlanes_Top,sVisualPlanes_Bottom);
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовка сцены
//----------------------------------------------------------------------------------------------------
void CEngine_GPortal::Draw(SPortal sPortal)
{
 long sector_index=sPortal.SectorIndex;
 long player_sector_index=sPortal.PlayerSectorIndex;
 if (sector_index<0 || sector_index>=vector_CISectorPtr.size()) return;//таких индексов не бывает
 if (SetLine>=WindowWidth) return;//все линии выведены
 long n;
 long size;

 SVisualPlanes sVisualPlanes_Top;
 SVisualPlanes sVisualPlanes_Bottom;

 sVisualPlanes_Top.MinX=WINDOW_WIDTH-1;
 sVisualPlanes_Top.MaxX=0;
 sVisualPlanes_Bottom.MinX=WINDOW_WIDTH-1;
 sVisualPlanes_Bottom.MaxX=0;
 for(n=0;n<WINDOW_WIDTH;n++)
 {
  sVisualPlanes_Top.TopY[n]=WINDOW_HEIGHT-1;
  sVisualPlanes_Top.BottomY[n]=0;

  sVisualPlanes_Bottom.TopY[n]=WINDOW_HEIGHT-1;
  sVisualPlanes_Bottom.BottomY[n]=0;
 }

 vector<unsigned long>* vector_WallIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallIndex();
 vector<unsigned long>* vector_WallPortalIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallPortalIndex();

 #ifdef DEBUG
 Line(sPortal.X-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y-PlayerY+WindowYCenter),sPortal.X+sPortal.L_X-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y+sPortal.L_Y-PlayerY+WindowYCenter),RGB(sPortal.LR,sPortal.LG,sPortal.LB));
 Line(sPortal.X-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y-PlayerY+WindowYCenter),sPortal.X+sPortal.R_X-PlayerX+WindowXCenter,WINDOW_HEIGHT-(sPortal.Y+sPortal.R_Y-PlayerY+WindowYCenter),RGB(sPortal.RR,sPortal.RG,sPortal.RB));
 #endif

 DrawSectorWall(sPortal,sVisualPlanes_Top,sVisualPlanes_Bottom);
 //рекурсивно вызывает отрисовку для всех порталов сектора
 size=vector_WallPortalIndex_Ptr->size();
 //создадим список доступных не заблокированных порталов сектора
 vector<long> vector_portals;
 for(n=0;n<size;n++)
 {
  long index=(*vector_WallPortalIndex_Ptr)[n];
  if (vector_SWall[index].PortalEnabled==false) continue;//портал уже был заблокирован
  vector_portals.push_back(index);
  vector_SWall[index].PortalEnabled=false;
 }
 size=vector_portals.size();
 for(n=0;n<size;n++)
 {
  long index=vector_portals[n];

  SPortal sPortal_New=sPortal;

  long sector1=vector_SWall[index].Sector1;
  long sector2=vector_SWall[index].Sector2;

  float x1=vector_SWall[index].X1;
  float y1=vector_SWall[index].Y1;
  float x2=vector_SWall[index].X2;
  float y2=vector_SWall[index].Y2;

  //проверим, не стоим ли мы на портале (т.к. возможна ошибка округления и значит глюки)
  //если мы стоим на портале, то ничего не меняется !!!
  MATH_RETURN_CODE v=GetNSide(PlayerX,PlayerY,x1,y1,x2,y2);
  if (v==MATH_RETURN_CODE_SIDE_IDENTITY)//мы стоим на портале
  {
   MATH_RETURN_CODE rc=CheckCollizionPointOfSegment(PlayerX,PlayerY,x1,y1,x2,y2);
   if (rc==MATH_RETURN_CODE_COLLIZION)
   {
    if (sector_index!=sector1)
    {
     sPortal_New.SectorIndex=sector1;
     Draw(sPortal_New);
    }
    if (sector_index!=sector2)
    {
     sPortal_New.SectorIndex=sector2;
     Draw(sPortal_New);
    }
    continue;
   }
  }
  //отсечём портал по границе линии обзора
  float xo,yo;
  MATH_RETURN_CODE v1=GetSide(x1,y1,sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2);
  MATH_RETURN_CODE v2=GetSide(x2,y2,sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2);
  if (v1==MATH_RETURN_CODE_SIDE_RIGHT && v2==MATH_RETURN_CODE_SIDE_RIGHT) continue;//портал не видим
  if (v1==MATH_RETURN_CODE_SIDE_RIGHT)
  {
   if (Intersection(sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2,x1,y1,x2,y2,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
   {
    x1=xo;
    y1=yo;
   }
  }
  if (v2==MATH_RETURN_CODE_SIDE_RIGHT)
  {
   if (Intersection(sPortal.X1,sPortal.Y1,sPortal.X2,sPortal.Y2,x1,y1,x2,y2,&xo,&yo)==MATH_RETURN_CODE_COLLIZION)
   {
    x2=xo;
    y2=yo;
   }
  }
  //проверим видимость портала
  MATH_RETURN_CODE p1_left=GetSide(x1,y1,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);
  MATH_RETURN_CODE p2_left=GetSide(x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);
  MATH_RETURN_CODE p1_right=GetSide(x1,y1,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);
  MATH_RETURN_CODE p2_right=GetSide(x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);
  //точки слева от левой границы портала
  if ((p1_left==MATH_RETURN_CODE_SIDE_LEFT || p1_left==MATH_RETURN_CODE_SIDE_IDENTITY) &&
	  (p2_left==MATH_RETURN_CODE_SIDE_LEFT || p2_left==MATH_RETURN_CODE_SIDE_IDENTITY)) continue;
  //точки справа от правой границы портала
  if ((p1_right==MATH_RETURN_CODE_SIDE_RIGHT || p1_right==MATH_RETURN_CODE_SIDE_IDENTITY) &&
      (p2_right==MATH_RETURN_CODE_SIDE_RIGHT || p2_right==MATH_RETURN_CODE_SIDE_IDENTITY)) continue;
  //сделаем так, чтобы точка 1 была левее точки 2
  bool change=false;
  if (p1_left==MATH_RETURN_CODE_SIDE_RIGHT || p1_left==MATH_RETURN_CODE_SIDE_IDENTITY)
  {
   if (p2_left==MATH_RETURN_CODE_SIDE_LEFT || p2_left==MATH_RETURN_CODE_SIDE_IDENTITY) change=true;
  }
  if (p1_right==MATH_RETURN_CODE_SIDE_RIGHT || p1_right==MATH_RETURN_CODE_SIDE_IDENTITY)
  {
   if (p2_right==MATH_RETURN_CODE_SIDE_LEFT || p2_right==MATH_RETURN_CODE_SIDE_IDENTITY) change=true;
  }
  //----------------------------------------------------------------------------------------------------
  //точки лежат внутри границ портала
  //----------------------------------------------------------------------------------------------------
  if ((p1_left==MATH_RETURN_CODE_SIDE_RIGHT || p1_left==MATH_RETURN_CODE_SIDE_IDENTITY) && (p2_left==MATH_RETURN_CODE_SIDE_RIGHT || p2_left==MATH_RETURN_CODE_SIDE_IDENTITY) &&
	  (p1_right==MATH_RETURN_CODE_SIDE_LEFT || p1_right==MATH_RETURN_CODE_SIDE_IDENTITY) && (p2_right==MATH_RETURN_CODE_SIDE_LEFT || p2_right==MATH_RETURN_CODE_SIDE_IDENTITY))
  {
   float vx1=x1-sPortal.X;
   float vy1=y1-sPortal.Y;
   float vx2=x2-sPortal.X;
   float vy2=y2-sPortal.Y;
   //считаем скалярное произведение: тот вектор, у которого оно больше-ближайший к левой границе области видимости
   double n1=(double)(sqrt(vx1*vx1+vy1*vy1));
   double n2=(double)(sqrt(vx2*vx2+vy2*vy2));
   double n3=(double)(sqrt(sPortal.L_X*sPortal.L_X+sPortal.L_Y*sPortal.L_Y));
   double sc1=(vx1*sPortal.L_X+vy1*sPortal.L_Y)/(n1*n3);
   double sc2=(vx2*sPortal.L_X+vy2*sPortal.L_Y)/(n2*n3);
   if (sc1<sc2) change=true;
  }
  if (change==true)
  {
   float x1_tmp=x1;
   float y1_tmp=y1;
   x1=x2;
   y1=y2;
   x2=x1_tmp;
   y2=y1_tmp;
  }
  //найдём пересечениея портала с областью видимости
  MATH_RETURN_CODE mrc_left=CheckCollizionSegmentAndRay(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.L_X,sPortal.Y+sPortal.L_Y);
  MATH_RETURN_CODE mrc_right=CheckCollizionSegmentAndRay(x1,y1,x2,y2,sPortal.X,sPortal.Y,sPortal.X+sPortal.R_X,sPortal.Y+sPortal.R_Y);
  if (mrc_left==MATH_RETURN_CODE_NO_COLLIZION)
  {
   sPortal_New.L_X=x1-sPortal.X;
   sPortal_New.L_Y=y1-sPortal.Y;
  }
  if (mrc_right==MATH_RETURN_CODE_NO_COLLIZION)
  {
   sPortal_New.R_X=x2-sPortal.X;
   sPortal_New.R_Y=y2-sPortal.Y;
  }
  if (sector_index!=sector1)
  {
   sPortal_New.SectorIndex=sector1;
   Draw(sPortal_New);
  }
  if (sector_index!=sector2)
  {
   sPortal_New.SectorIndex=sector2;
   Draw(sPortal_New);
  }
 }
 //разблокируем все порталы сектора
 for(n=0;n<size;n++)
 {
  long index=vector_portals[n];
  vector_SWall[index].PortalEnabled=true;
 }
 //выведем все полы и потолки сектора
 DrawFlow(sector_index,sVisualPlanes_Top);
 DrawFloor(sector_index,sVisualPlanes_Bottom);
}

//----------------------------------------------------------------------------------------------------
//отрисовка стены
//----------------------------------------------------------------------------------------------------
void CEngine_GPortal::DrawWall(float x1,float y1,float x2,float y2,long toffset,long length,SWall &sWall,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 #ifdef DEBUG
 //рисование карты
 unsigned long color=RGB(255,0,0);
 if (sWall.Frontier==true) color=RGB(0,0,255);
 Line(x1-PlayerX+WindowXCenter,WINDOW_HEIGHT-(y1-PlayerY+WindowYCenter),x2-PlayerX+WindowXCenter,WINDOW_HEIGHT-(y2-PlayerY+WindowYCenter),color);
 return;
 #endif

 length=static_cast<long>(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));

 long sector1=sWall.Sector1;
 long sector2=sWall.Sector2;
 MATH_RETURN_CODE mrc=GetSide(PlayerX,PlayerY,x1,y1,x2,y2);
 long p=0;
 if (mrc==MATH_RETURN_CODE_SIDE_LEFT) p=-1;
 if (mrc==MATH_RETURN_CODE_SIDE_RIGHT) p=1;
 float cs=CosTable[PlayerAngle];
 float ss=SinTable[PlayerAngle];
 float c1,c2;
 x1-=PlayerX;
 x2-=PlayerX;
 y1-=PlayerY;
 y2-=PlayerY;
 float v1=x1*ss+y1*cs;
 float v2=x2*ss+y2*cs;
 if (v1<1 && v2<1) return;
 float u1=x1*cs-y1*ss;
 float u2=x2*cs-y2*ss;
 float vo1=v1;
 float vo2=v2;
 float uo1=u1;
 float uo2=u2;
 if (v1<1)
 {
  uo1+=(1-v1)*(u2-u1)/(v2-v1);
  vo1=1;
 }
 if (v2<1)
 {
  uo2+=(1-v2)*(u2-u1)/(v2-v1);
  vo2=1;
 }
 float col1=WindowXCenter+((float)WindowXCenter)*uo1/vo1;
 float col2=WindowXCenter+((float)WindowXCenter)*uo2/vo2;
 long offset=toffset;
 if (col2<col1)
 {
  offset=128-(length+toffset);
  c1=col2;
  c2=col1;
  float a=u1;
  u1=u2;
  u2=a;
  a=v1;
  v1=v2;
  v2=a;
  a=uo1;
  uo1=uo2;
  uo2=a;
  a=vo1;
  vo1=vo2;
  vo2=a;
 }
 else
 {
  c1=col1;
  c2=col2;
 }
 if (c2<0 || c1>=WindowWidth) return;
 if (c1==c2) return;
 float t;
 float z;
 if (sWall.Frontier==false)//это не линия раздела
 {
  t=TEXTURE_SCALE*(sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset);
  z=2*vo1/(float)WindowHeight;
  float vz1=1.0/vo1;
  float vt1=t/vo1;
  float t1=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetUp()-PlayerZ)/z;
  float b1=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetDown()-PlayerZ)/z;
  float dc=c2-c1+1;
  t=TEXTURE_SCALE*(sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset);
  z=2*vo2/(float)WindowHeight;
  float vz2=1.0/vo2;
  float vt2=t/vo2;
  float t2=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetUp()-PlayerZ)/z;
  float b2=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetDown()-PlayerZ)/z;
  float dtop=(t2-t1)/dc;
  float dbottom=(b2-b1)/dc;
  float dz=(vz2-vz1)/dc;
  float dt=(vt2-vt1)/dc;
  if (c1<0)
  {
   t1-=dtop*c1;
   b1-=dbottom*c1;
   vz1-=dz*c1;
   vt1-=dt*c1;
   c1=0;
  }
  if (c2>=WindowWidth) c2=WindowWidth-1;
  long tup_index=sWall.cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long height=vector_CISectorPtr[sector1]->GetUp()-vector_CISectorPtr[sector1]->GetDown()+1;
  for (long c=static_cast<long>(c1);c<=static_cast<long>(c2);c++,t1+=dtop,b1+=dbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c]) continue;
   if (vz1>1) continue;
   z=1.0/vz1;
   t=vt1*z;
   DrawSimpleLine(z,c,t1,b1,t,height,tup_index,sector1,sVisualPlanes_Top,sVisualPlanes_Bottom);
  }
 }
 else
 {
  long sd1,sd2,su1,su2;
  long sector=0;
  if (p<0)//игрок справа
  {
   sd1=vector_CISectorPtr[sector1]->GetDown();
   sd2=vector_CISectorPtr[sector2]->GetDown();
   su1=vector_CISectorPtr[sector2]->GetUp();
   su2=vector_CISectorPtr[sector1]->GetUp();
   sector=sector1;
  }
  else
  {
   sd1=vector_CISectorPtr[sector2]->GetDown();
   sd2=vector_CISectorPtr[sector1]->GetDown();
   su1=vector_CISectorPtr[sector1]->GetUp();
   su2=vector_CISectorPtr[sector2]->GetUp();
   sector=sector2;
  }
  if (sd1>=su1) su1=sd1;
  t=sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset;
  z=2*vo1/(float)WindowHeight;
  float vz1=1/vo1;
  float vt1=t/vo1;
  float oz=-PlayerZ;
  float tu1=(float)WindowYCenterWithOffset-(su2+oz)/z;
  float bu1=(float)WindowYCenterWithOffset-(su1+oz)/z;
  float td1=(float)WindowYCenterWithOffset-(sd2+oz)/z;
  float bd1=(float)WindowYCenterWithOffset-(sd1+oz)/z;
  float dc=c2-c1+1;
  t=sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset;
  z=2*vo2/(float)WindowHeight;
  float vz2=1/vo2;
  float vt2=t/vo2;
  float tu2=(float)WindowYCenterWithOffset-(su2+oz)/z;
  float bu2=(float)WindowYCenterWithOffset-(su1+oz)/z;
  float td2=(float)WindowYCenterWithOffset-(sd2+oz)/z;
  float bd2=(float)WindowYCenterWithOffset-(sd1+oz)/z;
  float dutop=(tu2-tu1)/dc;
  float dubottom=(bu2-bu1)/dc;
  float ddtop=(td2-td1)/dc;
  float ddbottom=(bd2-bd1)/dc;
  float dz=(vz2-vz1)/dc;
  float dt=(vt2-vt1)/dc;
  if (c1<0)
  {
   tu1-=dutop*c1;
   bu1-=dubottom*c1;
   td1-=ddtop*c1;
   bd1-=ddbottom*c1;
   vz1-=dz*c1;
   vt1-=dt*c1;
   c1=0;
  }
  if (c2>=WindowWidth) c2=WindowWidth-1;
  long hgtup=su2-su1;
  if (hgtup<0) hgtup=-hgtup;
  hgtup++;
  long hgtdown=sd2-sd1;
  if (hgtdown<0) hgtdown=-hgtdown;
  hgtdown++;
  long tup_index=sWall.cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long tdown_index=sWall.cTextureFollow_Down.GetCurrentTexture().TextureIndex;
  for (long c=static_cast<long>(c1);c<=static_cast<long>(c2);c++,tu1+=dutop,bu1+=dubottom,td1+=ddtop,bd1+=ddbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c]) continue;
   if (vz1>1) continue;
   z=1.0/vz1;
   t=vt1*z;
   DrawUpperLine(z,c,tu1,bu1,t,hgtup,tup_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
   DrawLowerLine(z,c,td1,bd1,t,hgtdown,tdown_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
  }
 }
}
