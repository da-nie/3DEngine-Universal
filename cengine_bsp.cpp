#include "cengine_bsp.h"

extern CVideo cVideo;
//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CEngine_BSP::CEngine_BSP():CEngine_Base()
{
 sBSPNode_Ptr=NULL;
 vector_SSegment.clear();
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CEngine_BSP::~CEngine_BSP()
{
}
//----------------------------------------------------------------------------------------------------
//загрузить карту
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::LoadMap(string filename)
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
 //загружаем BSP-дерево и создаём сегменты
 sBSPNode_Ptr=LoadBSPTree(file);
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
//освободить память карты
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::ReleaseMap(void)
{
 CEngine_Base::ReleaseMap();
 vector_SSegment.clear();
 DeleteBSPTree(sBSPNode_Ptr);
 sBSPNode_Ptr=NULL;
}
//----------------------------------------------------------------------------------------------------
//отрисовать лабиринт
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)
{
 CEngine_Base::View(px,py,pz,pangle,panglevertical,powerlighting);
 DrawBSPTree(sBSPNode_Ptr);//рисуем сегменты
 //PutSprite();
}
//----------------------------------------------------------------------------------------------------
//загрузка ветви дерева
//----------------------------------------------------------------------------------------------------
SBSPNode* CEngine_BSP::LoadBSPTree(FILE *file)
{
 SBSPNode* sBSPNode_RootPtr=new SBSPNode;
 unsigned char state=LoadUChar(file);
 SSegment sSegment;
 sSegment.X1=LoadFloat(file);
 sSegment.Y1=LoadFloat(file);
 sSegment.X2=LoadFloat(file);
 sSegment.Y2=LoadFloat(file);
 sSegment.Len=static_cast<long>(sqrt((sSegment.X2-sSegment.X1)*(sSegment.X2-sSegment.X1)+(sSegment.Y2-sSegment.Y1)*(sSegment.Y2-sSegment.Y1)));
 sSegment.Offset=LoadLong(file);
 sSegment.WallIndex=LoadLong(file);

 sSegment.Sector1=LoadLong(file);
 sSegment.Sector2=LoadLong(file);

 sBSPNode_RootPtr->SegmentIndex=vector_SSegment.size();
 vector_SSegment.push_back(sSegment);
 sBSPNode_RootPtr->Split=true;
 if (state&BSP_ATOM)
 {
  sBSPNode_RootPtr->Split=false;
  return(sBSPNode_RootPtr);
 }
 //считываем описывающие прямоугольники
 sBSPNode_RootPtr->LX1=LoadFloat(file);
 sBSPNode_RootPtr->LY1=LoadFloat(file);
 sBSPNode_RootPtr->LX2=LoadFloat(file);
 sBSPNode_RootPtr->LY2=LoadFloat(file);
 sBSPNode_RootPtr->RX1=LoadFloat(file);
 sBSPNode_RootPtr->RY1=LoadFloat(file);
 sBSPNode_RootPtr->RX2=LoadFloat(file);
 sBSPNode_RootPtr->RY2=LoadFloat(file);
 if (!(state&BSP_LEFT)) sBSPNode_RootPtr->sBSPNode_LeftPtr=NULL;
                   else sBSPNode_RootPtr->sBSPNode_LeftPtr=LoadBSPTree(file);
 if (!(state&BSP_RIGHT)) sBSPNode_RootPtr->sBSPNode_RightPtr=NULL;
                     else sBSPNode_RootPtr->sBSPNode_RightPtr=LoadBSPTree(file);
 return(sBSPNode_RootPtr);
}
//----------------------------------------------------------------------------------------------------
//удаление дерева
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::DeleteBSPTree(SBSPNode *sBSPNode_RootPtr)
{
 if (sBSPNode_RootPtr==NULL) return;
 if (sBSPNode_RootPtr->Split==0) return;
 if (sBSPNode_RootPtr->sBSPNode_LeftPtr!=NULL)
 {
  DeleteBSPTree(sBSPNode_RootPtr->sBSPNode_LeftPtr);
  delete(sBSPNode_RootPtr->sBSPNode_LeftPtr);
 }
 if (sBSPNode_RootPtr->sBSPNode_RightPtr!=NULL)
 {
  DeleteBSPTree(sBSPNode_RootPtr->sBSPNode_RightPtr);
  delete(sBSPNode_RootPtr->sBSPNode_RightPtr);
 }
}
//----------------------------------------------------------------------------------------------------
//проверка видимости описывающего прямоугольника
//----------------------------------------------------------------------------------------------------
long CEngine_BSP::SeenBox(SBSPNode* tree,long type)
{
 if (tree->Split!=1) return(1);
 float ss=SinTable[PlayerAngle];
 float cs=CosTable[PlayerAngle];
 float x1=0;
 float y1=0;
 float x2=0;
 float y2=0;
 if (type==MATH_RETURN_CODE_SIDE_LEFT)//проверим видимость поддерева
 {
  x1=tree->LX1-PlayerX;
  y1=tree->LY1-PlayerY;
  x2=tree->LX2-PlayerX;
  y2=tree->LY2-PlayerY;
 }
 if (type==MATH_RETURN_CODE_SIDE_RIGHT)
 {
  x1=tree->RX1-PlayerX;
  y1=tree->RY1-PlayerY;
  x2=tree->RX2-PlayerX;
  y2=tree->RY2-PlayerY;
 }
 float x1ss=x1*ss;
 float x2ss=x2*ss;
 float y1ss=y1*ss;
 float y2ss=y2*ss;
 float x1cs=x1*cs;
 float x2cs=x2*cs;
 float y1cs=y1*cs;
 float y2cs=y2*cs;
 float v1=x1ss+y1cs;
 float v2=x2ss+y2cs;
 float v3=x1ss+y2cs;
 float v4=x2ss+y1cs;
 if (v1<=1 && v2<=1 && v3<=1 && v4<=1) return(0);
 if (v1<=0 || v2<=0 || v3<=0 || v4<=0) return(1);//не проверить
 float u1=x1cs-y1ss;
 float u2=x2cs-y2ss;
 float u3=x1cs-y2ss;
 float u4=x2cs-y1ss;
 float xs1,xs2,xs3,xs4;
 float xp=WindowXCenter;
 xs1=xp+xp*u1/v1;
 xs2=xp+xp*u2/v2;
 xs3=xp+xp*u3/v3;
 xs4=xp+xp*u4/v4;
 if (xs1<0 && xs2<0 && xs3<0 && xs4<0) return(0);
 if (xs1>=WindowWidth && xs2>=WindowWidth && xs3>=WindowWidth && xs4>=WindowWidth) return(0);
 return(1);
}
//----------------------------------------------------------------------------------------------------
//обход BSP дерева и прорисовка лабиринта
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::DrawBSPTree(SBSPNode* tree)
{
 if (SetLine>=WindowWidth) return;
 if (tree->Split==0)
 {
  DrawBSPLeafe(tree);
  return;
 }
 MATH_RETURN_CODE mrc=GetSide(PlayerX,PlayerY,vector_SSegment[tree->SegmentIndex].X1,vector_SSegment[tree->SegmentIndex].Y1,vector_SSegment[tree->SegmentIndex].X2,vector_SSegment[tree->SegmentIndex].Y2);
 long p=0;
 if (mrc==MATH_RETURN_CODE_SIDE_LEFT) p=-1;
 if (mrc==MATH_RETURN_CODE_SIDE_RIGHT) p=1;
 if (p<=0)
 {
  if (tree->sBSPNode_RightPtr!=NULL)
  {
   if (SeenBox(tree,MATH_RETURN_CODE_SIDE_RIGHT)) DrawBSPTree(tree->sBSPNode_RightPtr);
  }
  DrawBSPLeafe(tree);
  if (tree->sBSPNode_LeftPtr!=NULL)
  {
   if (SeenBox(tree,MATH_RETURN_CODE_SIDE_LEFT)) DrawBSPTree(tree->sBSPNode_LeftPtr);
  }
  return;
 }
 else
 {
  if (tree->sBSPNode_LeftPtr!=NULL)
  {
   if (SeenBox(tree,MATH_RETURN_CODE_SIDE_LEFT)) DrawBSPTree(tree->sBSPNode_LeftPtr);
  }
  DrawBSPLeafe(tree);
  if (tree->sBSPNode_RightPtr!=NULL)
  {
   if (SeenBox(tree,MATH_RETURN_CODE_SIDE_RIGHT)) DrawBSPTree(tree->sBSPNode_RightPtr);
  }
  return;
 }
}
//----------------------------------------------------------------------------------------------------
//отрисовка сегментов стен
//----------------------------------------------------------------------------------------------------
void CEngine_BSP::DrawBSPLeafe(SBSPNode* root)
{
 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 //В отличие от портального движка, где разрезанный порталом сегмент выводится независимо каждым кусочком,
 //в BSP-движке сегмент выводится один раз и отсекается с помощью линий горизонта.
 //Поэтому, полы и потолки текстурировать потребуется каждый раз, когда выводимый сегмент будет прерываться.
 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 SVisualPlanes sVisualPlanes_Top;
 SVisualPlanes sVisualPlanes_Bottom;

 sVisualPlanes_Top.MinX=WindowWidth-1;
 sVisualPlanes_Top.MaxX=0;
 sVisualPlanes_Bottom.MinX=WindowWidth-1;
 sVisualPlanes_Bottom.MaxX=0;
 for(long n=0;n<WindowWidth;n++)
 {
  sVisualPlanes_Top.TopY[n]=WindowHeight-1;
  sVisualPlanes_Top.BottomY[n]=0;

  sVisualPlanes_Bottom.TopY[n]=WindowHeight-1;
  sVisualPlanes_Bottom.BottomY[n]=0;
 }


 long sector1=vector_SSegment[root->SegmentIndex].Sector1;
 long sector2=vector_SSegment[root->SegmentIndex].Sector2;
 long fsector=-1;
 float x1=vector_SSegment[root->SegmentIndex].X1;
 float x2=vector_SSegment[root->SegmentIndex].X2;
 float y1=vector_SSegment[root->SegmentIndex].Y1;
 float y2=vector_SSegment[root->SegmentIndex].Y2;
 MATH_RETURN_CODE mrc=GetSide(PlayerX,PlayerY,x1,y1,x2,y2);
 long p=0;
 if (mrc==MATH_RETURN_CODE_SIDE_LEFT) p=-1;
 if (mrc==MATH_RETURN_CODE_SIDE_RIGHT) p=1;
 float cs=CosTable[PlayerAngle];
 float ss=SinTable[PlayerAngle];
 float c1,c2;
 x1=vector_SSegment[root->SegmentIndex].X1-PlayerX;
 x2=vector_SSegment[root->SegmentIndex].X2-PlayerX;
 y1=vector_SSegment[root->SegmentIndex].Y1-PlayerY;
 y2=vector_SSegment[root->SegmentIndex].Y2-PlayerY;
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
 float offset=vector_SSegment[root->SegmentIndex].Offset;
 if (col2<col1)
 {
  offset=128-(vector_SSegment[root->SegmentIndex].Len+offset);
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
 if (vector_SWall[vector_SSegment[root->SegmentIndex].WallIndex].Frontier==false)//это не линия раздела
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
  long tup_index=vector_SWall[vector_SSegment[root->SegmentIndex].WallIndex].cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long height=vector_CISectorPtr[sector1]->GetUp()-vector_CISectorPtr[sector1]->GetDown()+1;
  fsector=-1;
  for (long c=static_cast<long>(c1);c<=static_cast<long>(c2);c++,t1+=dtop,b1+=dbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c] || vz1>1)//линия прервалась
   {
    if (fsector>=0)//сегмент уже выводился, значит нужно вывести пол и потолок
	{
     DrawFlow(fsector,sVisualPlanes_Top);
     DrawFloor(fsector,sVisualPlanes_Bottom);
     //инициализируем таблицу заново
	 sVisualPlanes_Top.MinX=WindowWidth-1;
     sVisualPlanes_Top.MaxX=0;
     sVisualPlanes_Bottom.MinX=WindowWidth-1;
     sVisualPlanes_Bottom.MaxX=0;
     for(long n=0;n<WindowWidth;n++)
	 {
      sVisualPlanes_Top.TopY[n]=WindowHeight-1;
      sVisualPlanes_Top.BottomY[n]=0;

      sVisualPlanes_Bottom.TopY[n]=WindowHeight-1;
      sVisualPlanes_Bottom.BottomY[n]=0;
	 }
 	 fsector=-1;
	}
	continue;
   }
   z=1.0/vz1;
   t=vt1*z;
   DrawSimpleLine(z,c,t1,b1,t,height,tup_index,sector1,sVisualPlanes_Top,sVisualPlanes_Bottom);
   fsector=sector1;
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
  t=TEXTURE_SCALE*(sqrt((v1-vo1)*(v1-vo1)+(u1-uo1)*(u1-uo1))+offset);
  z=2*vo1/(float)WindowHeight;
  float vz1=1/vo1;
  float vt1=t/vo1;
  float oz=-PlayerZ;
  float tu1=(float)WindowYCenterWithOffset-(su2+oz)/z;
  float bu1=(float)WindowYCenterWithOffset-(su1+oz)/z;
  float td1=(float)WindowYCenterWithOffset-(sd2+oz)/z;
  float bd1=(float)WindowYCenterWithOffset-(sd1+oz)/z;
  float dc=c2-c1+1;
  t=TEXTURE_SCALE*(sqrt((vo2-v1)*(vo2-v1)+(uo2-u1)*(uo2-u1))+offset);
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
  long tup_index=vector_SWall[vector_SSegment[root->SegmentIndex].WallIndex].cTextureFollow_Up.GetCurrentTexture().TextureIndex;
  long tdown_index=vector_SWall[vector_SSegment[root->SegmentIndex].WallIndex].cTextureFollow_Down.GetCurrentTexture().TextureIndex;
  fsector=-1;
  for (long c=static_cast<long>(c1);c<=static_cast<long>(c2);c++,tu1+=dutop,bu1+=dubottom,td1+=ddtop,bd1+=ddbottom,vz1+=dz,vt1+=dt)
  {
   if (TopLine[c]>=BottomLine[c] || vz1>1)//линия прервалась
   {
    if (fsector>=0)//сегмент уже выводился, значит нужно вывести пол и потолок
	{
     DrawFlow(fsector,sVisualPlanes_Top);
     DrawFloor(fsector,sVisualPlanes_Bottom);
     //инициализируем таблицу заново
	 sVisualPlanes_Top.MinX=WindowWidth-1;
     sVisualPlanes_Top.MaxX=0;
     sVisualPlanes_Bottom.MinX=WindowWidth-1;
     sVisualPlanes_Bottom.MaxX=0;
     for(long n=0;n<WindowWidth;n++)
	 {
      sVisualPlanes_Top.TopY[n]=WindowHeight-1;
      sVisualPlanes_Top.BottomY[n]=0;

      sVisualPlanes_Bottom.TopY[n]=WindowHeight-1;
      sVisualPlanes_Bottom.BottomY[n]=0;
	 }
 	 fsector=-1;
	}
	continue;
   }
   z=1.0/vz1;
   t=vt1*z;
   DrawUpperLine(z,c,tu1,bu1,t,hgtup,tup_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
   DrawLowerLine(z,c,td1,bd1,t,hgtdown,tdown_index,sector,sVisualPlanes_Top,sVisualPlanes_Bottom);
   fsector=sector;
  }
 }
 //выведем все полы и потолки сектора
 if (fsector>=0)
 {
  DrawFlow(fsector,sVisualPlanes_Top);
  DrawFloor(fsector,sVisualPlanes_Bottom);
 }
}
