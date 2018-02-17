#include "cengine_sportal.h"

extern CVideo cVideo;

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CEngine_SPortal::CEngine_SPortal():CEngine_Base()
{
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CEngine_SPortal::~CEngine_SPortal()
{
}
//----------------------------------------------------------------------------------------------------
//загрузить карту
//----------------------------------------------------------------------------------------------------
void CEngine_SPortal::LoadMap(string filename)
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
void CEngine_SPortal::View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)
{
 CEngine_Base::View(px,py,pz,pangle,panglevertical,powerlighting);

 //разрешаем использование всех порталов
 long size=vector_SWall.size();
 for(long n=0;n<size;n++) vector_SWall[n].PortalEnabled=true;
 //инициализируем начальный портал
 SPortal sPortal;
 sPortal.Left=0;
 sPortal.Right=WINDOW_WIDTH-1;
 sPortal.SectorIndex=PlayerSectorIndex;

 Draw(sPortal);//рисуем сцену
 PutSprite();
}
//----------------------------------------------------------------------------------------------------
//отрисовка стен сектора (без порталов)
//----------------------------------------------------------------------------------------------------
void CEngine_SPortal::DrawSectorWall(SPortal &sPortal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 long sector_index=sPortal.SectorIndex;
 vector<unsigned long>* vector_WallIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallIndex();
 //выводим все стены сектора
 long size=vector_WallIndex_Ptr->size();
 for(long n=0;n<size;n++)
 {
  long index=(*vector_WallIndex_Ptr)[n];
  DrawWall(vector_SWall[index],sPortal,false,sVisualPlanes_Top,sVisualPlanes_Bottom);
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовка сцены
//----------------------------------------------------------------------------------------------------
void CEngine_SPortal::Draw(SPortal sPortal)
{
 if (SetLine>=WindowWidth) return;//все линии выведены
 if (sPortal.Left>sPortal.Right) return;//сектор не виден

 long sector_index=sPortal.SectorIndex;
 if (sector_index<0 || sector_index>=vector_CISectorPtr.size()) return;//таких индексов не бывает
 long n;
 long size;

 vector<unsigned long>* vector_WallIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallIndex();
 vector<unsigned long>* vector_WallPortalIndex_Ptr=vector_CISectorPtr[sector_index]->GetVectorWallPortalIndex();

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

 //выведем все стены сектора
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
  long sector1=vector_SWall[index].Sector1;
  long sector2=vector_SWall[index].Sector2;
  SPortal sPortal_New=sPortal;
  bool portal=true;
  float x1=vector_SWall[index].X1;
  float y1=vector_SWall[index].Y1;
  float x2=vector_SWall[index].X2;
  float y2=vector_SWall[index].Y2;
  if (GetNSide(PlayerX,PlayerY,x1,y1,x2,y2)==MATH_RETURN_CODE_SIDE_IDENTITY)
  {
   MATH_RETURN_CODE rc=CheckCollizionPointOfSegment(PlayerX,PlayerY,x1,y1,x2,y2);
   if (rc==MATH_RETURN_CODE_COLLIZION) portal=false;
  }
  //выводим портал сектора
  DrawWall(vector_SWall[index],sPortal_New,portal,sVisualPlanes_Top,sVisualPlanes_Bottom);
  if (sector1!=sector_index)
  {
   sPortal_New.SectorIndex=sector1;
   Draw(sPortal_New);
  }
  if (sector2!=sector_index)
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
void CEngine_SPortal::DrawWall(SWall &sWall,SPortal &sPortal,bool portal,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 if (sPortal.Left>sPortal.Right) return;//сегмент не видим

 float x1=sWall.X1;
 float y1=sWall.Y1;
 float x2=sWall.X2;
 float y2=sWall.Y2;
 float toffset=sWall.Offset;

 long length=sWall.Len;
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
 if (v1<1 && v2<1)//сегмент за спиной
 {
  if (portal==true)//если мы обрабатывали портал, то закрываем его
  {
   sPortal.Left=WINDOW_WIDTH;
   sPortal.Right=0;
  }
  return;
 }
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
 if (static_cast<long>(c2)<=sPortal.Left || static_cast<long>(c1)>=sPortal.Right)
 {
  if (portal==true)//если мы обрабатывали портал, то закрываем его
  {
   sPortal.Left=WINDOW_WIDTH;
   sPortal.Right=0;
  }
  return;
 }
 if (static_cast<long>(c1)>=static_cast<long>(c2))
 {
  if (portal==true)//если мы обрабатывали портал, то закрываем его
  {
   sPortal.Left=WINDOW_WIDTH;
   sPortal.Right=0;
  }
  return;
 }
 float t;
 float z;
 if (sWall.Frontier==false)//это не линия раздела
 {
  t=TEXTURE_SCALE*(sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset);
  z=2*vo1/(float)(WindowHeight);
  float vz1=1.0/vo1;
  float vt1=t/vo1;
  float t1=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetUp()-PlayerZ)/z;
  float b1=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetDown()-PlayerZ)/z;
  float dc=c2-c1+1;
  t=TEXTURE_SCALE*(sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset);
  z=2*vo2/(float)(WindowHeight);
  float vz2=1.0/vo2;
  float vt2=t/vo2;
  float t2=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetUp()-PlayerZ)/z;
  float b2=WindowYCenterWithOffset-(vector_CISectorPtr[sector1]->GetDown()-PlayerZ)/z;
  float dtop=(t2-t1)/dc;
  float dbottom=(b2-b1)/dc;
  float dz=(vz2-vz1)/dc;
  float dt=(vt2-vt1)/dc;
  if (c1<sPortal.Left)
  {
   float dc=c1-sPortal.Left;
   t1-=dtop*dc;
   b1-=dbottom*dc;
   vz1-=dz*dc;
   vt1-=dt*dc;
   c1=sPortal.Left;
  }
  if (c2>sPortal.Right) c2=sPortal.Right;
  long tup_index=sWall.cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long height=vector_CISectorPtr[sector1]->GetUp()-vector_CISectorPtr[sector1]->GetDown()+1;
  long l_c1=static_cast<long>(c1);
  long l_c2=static_cast<long>(c2);
  for (long c=l_c1;c<=l_c2;c++,t1+=dtop,b1+=dbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c]) continue;
   if (vz1>1) continue;
   z=1.0/vz1;
   t=vt1*z;
   DrawSimpleLine(z,c,t1,b1,t,height,tup_index,sector1,sVisualPlanes_Top,sVisualPlanes_Bottom);
  }
  if (portal==true)//если мы обрабатывали портал
  {
   sPortal.Left=l_c1;
   sPortal.Right=l_c2;
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
  z=2*vo1/(float)(WindowHeight);
  float vz1=1/vo1;
  float vt1=t/vo1;
  float oz=-PlayerZ;
  float tu1=(float)WindowYCenterWithOffset-(su2+oz)/z;
  float bu1=(float)WindowYCenterWithOffset-(su1+oz)/z;
  float td1=(float)WindowYCenterWithOffset-(sd2+oz)/z;
  float bd1=(float)WindowYCenterWithOffset-(sd1+oz)/z;
  float dc=c2-c1+1;
  t=sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset;
  z=2*vo2/(float)(WindowHeight);
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
  if (c1<sPortal.Left)
  {
   float dc=c1-sPortal.Left;
   tu1-=dutop*dc;
   bu1-=dubottom*dc;
   td1-=ddtop*dc;
   bd1-=ddbottom*dc;
   vz1-=dz*dc;
   vt1-=dt*dc;
   c1=sPortal.Left;
  }
  if (c2>sPortal.Right) c2=sPortal.Right;
  long hgtup=su2-su1;
  if (hgtup<0) hgtup=-hgtup;
  hgtup++;
  long hgtdown=sd2-sd1;
  if (hgtdown<0) hgtdown=-hgtdown;
  hgtdown++;
  long tup_index=sWall.cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long tdown_index=sWall.cTextureFollow_Down.GetCurrentTexture().TextureIndex;
  long l_c1=static_cast<long>(c1);
  long l_c2=static_cast<long>(c2);
  for (long c=l_c1;c<=l_c2;c++,tu1+=dutop,bu1+=dubottom,td1+=ddtop,bd1+=ddbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c]) continue;
   if (vz1>1) continue;
   z=1.0/vz1;
   t=vt1*z;

   DrawUpperLine(z,c,tu1,bu1,t,hgtup,tup_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
   DrawLowerLine(z,c,td1,bd1,t,hgtdown,tdown_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
  }
  if (portal==true)//если мы обрабатывали портал
  {
   sPortal.Left=l_c1;
   sPortal.Right=l_c2;
  }
 }
}
