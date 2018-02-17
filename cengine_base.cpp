#include "cengine_base.h"

extern CVideo cVideo;

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CEngine_Base::CEngine_Base()
{
 long n;
 vector_CISectorPtr.clear();
 cVideo.GetWindownCenter(WindowXCenter,WindowYCenter);
 cVideo.GetWindowSize(WindowWidth,WindowHeight);
 //инициализируем таблицы горизонтального текстурирования
 for(n=0;n<=360;n++)
 {
  dcTable[n]=static_cast<long>(cos(M_PI/180*n)*(1<<FIXMATH_FLOOR_FLOW_SHIFT));
  dsTable[n]=static_cast<long>(sin(M_PI/180*n)*(1<<FIXMATH_FLOOR_FLOW_SHIFT));
  diTable[n]=dcTable[n]/WindowXCenter;
  djTable[n]=-dsTable[n]/WindowXCenter;
 }
 //инициализируем таблицу синусов и косинусов
 for (n=0;n<=360;n++)
 {
  CosTable[n]=cos(M_PI/180*n);
  SinTable[n]=sin(M_PI/180*n);
 }
 //выделяем память под необходимые массивы
 TopLine=new long[WindowWidth+1];
 BottomLine=new long[WindowWidth+1];

 X_Table=new long[WindowHeight];
 //инициализируем буфер Z-значений
 ZLineBuffer.resize(WindowWidth);

 vector_SWall.clear();

 cWallMap_Ptr=new CWallMap(GRID_X_STEP,GRID_Y_STEP);
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CEngine_Base::~CEngine_Base()
{
 ReleaseMap();
 delete[](TopLine);
 delete[](BottomLine);
 delete[](X_Table);
 delete(cWallMap_Ptr);
 Close();
}
//----------------------------------------------------------------------------------------------------
//указать рабочий каталог
//----------------------------------------------------------------------------------------------------
void CEngine_Base::SetPath(string path)
{
 Path=path;
}
//----------------------------------------------------------------------------------------------------
//инициализировать
//----------------------------------------------------------------------------------------------------
void CEngine_Base::Init(void)
{
 cVideo.Init();
}
//----------------------------------------------------------------------------------------------------
//завершить работу
//----------------------------------------------------------------------------------------------------
void CEngine_Base::Close(void)
{
 cGraphics.Release();
 cVideo.Close();
}

//----------------------------------------------------------------------------------------------------
//освободить память карты
//----------------------------------------------------------------------------------------------------
void CEngine_Base::ReleaseMap(void)
{
 long n;
 long size;

 size=vector_CISectorPtr.size();
 for(n=0;n<size;n++) delete(vector_CISectorPtr[n]);

 size=vector_CUnitPtr.size();
 for(n=0;n<size;n++) delete(vector_CUnitPtr[n]);

 vector_CUnitPtr.clear();
 vector_CISectorPtr.clear();
 cWallMap_Ptr->Release();
}
//----------------------------------------------------------------------------------------------------
//получение номера сектора из таблицы по координатам
//----------------------------------------------------------------------------------------------------
long CEngine_Base::GetSectorIndexForPos(float x,float y)
{
 long n,m;
 long size=vector_CISectorPtr.size();
 for(n=0;n<size;n++)
 {
  CISector *cISectorPtr=vector_CISectorPtr[n];
  float xb1;
  float xb2;
  float yb1;
  float yb2;
  cISectorPtr->GetRectangle(xb1,yb1,xb2,yb2);
  if (x<xb1 || x>xb2) continue;
  if (y<yb1 || y>yb2) continue;
  long s=1;
  long po=0;
  vector<SPoint> *vector_SPoint_Ptr=cISectorPtr->GetSectorPointVectorPtr();
  long size=vector_SPoint_Ptr->size();
  for(m=0;m<size;m++)
  {
   long next_m=(m+1)%size;
   SPoint sPoint_A=(*vector_SPoint_Ptr)[m];
   SPoint sPoint_B=(*vector_SPoint_Ptr)[next_m];
   MATH_RETURN_CODE mrc=GetSide(x,y,sPoint_A.X,sPoint_A.Y,sPoint_B.X,sPoint_B.Y);
   if (mrc==MATH_RETURN_CODE_SIDE_IDENTITY) continue;
   long p=0;
   if (mrc==MATH_RETURN_CODE_SIDE_LEFT) p=-1;
   if (mrc==MATH_RETURN_CODE_SIDE_RIGHT) p=1;
   if (po==0) po=p;
   if (p!=po)
   {
    s=0;
    break;
   }
  }
  if (s!=0) return(n);
 }
 return(-1);
}
//----------------------------------------------------------------------------------------------------
//получение координаты Z
//----------------------------------------------------------------------------------------------------
long CEngine_Base::GetZPos(float x,float y)
{
 long sector=GetSectorIndexForPos(x,y);
 if (sector!=-1)
 {
  if (static_cast<unsigned long>(sector)<vector_CISectorPtr.size())
  {
   CISector *cISectorPtr=vector_CISectorPtr[sector];
   return(cISectorPtr->GetDown());
  }
 }
 return(0);
}
//----------------------------------------------------------------------------------------------------
//отрисовать лабиринт
//----------------------------------------------------------------------------------------------------
void CEngine_Base::View(float px,float py,float pz,float pangle,float panglevertical,float powerlighting)
{
 cVideo.GetVideoPointer(VideoBufferPtr,VideoLineWidth);

 PlayerX=px;
 PlayerY=py;
 PlayerZ=pz;
 PlayerAngle=static_cast<long>(pangle);
 PlayerAngleVertical=static_cast<long>(panglevertical);
 PlayerPowerLighting=static_cast<long>(powerlighting);
 PlayerSectorIndex=GetSectorIndexForPos(PlayerX,PlayerY);
 SetLine=0;
 long n;

 ClearFloorFlowBuffer();
 for(n=0;n<WindowWidth;n++)
 {
  //инициализировали линии горизонта
  TopLine[n]=0;
  BottomLine[n]=WindowHeight-1;
 }
 cVideo.GetWindownCenter(WindowXCenter,WindowYCenter);
 WindowYCenterWithOffset=WindowYCenter+tan(M_PI/180.0*PlayerAngleVertical)*WindowHeight/2;
}
//----------------------------------------------------------------------------------------------------
//получить косинус угла из таблицы
//----------------------------------------------------------------------------------------------------
float CEngine_Base::GetCos(long angle)
{
 return(CosTable[angle]);
}
//----------------------------------------------------------------------------------------------------
//получить синус угла из таблицы
//----------------------------------------------------------------------------------------------------
float CEngine_Base::GetSin(long angle)
{
 return(SinTable[angle]);
}
//----------------------------------------------------------------------------------------------------
//получить начальные координаты игрока
//----------------------------------------------------------------------------------------------------
void CEngine_Base::GetPlayerInitPos(float &x,float &y,long &angle)
{
 x=PlayerXInitPos;
 y=PlayerYInitPos;
 angle=static_cast<long>(PlayerAngleInit);
}
//----------------------------------------------------------------------------------------------------
//получить указатель на сектор по его номеру
//----------------------------------------------------------------------------------------------------
CISector* CEngine_Base::GetCISectorPtrForIndex(long sectorindex)
{
 long size=vector_CISectorPtr.size();
 if (sectorindex<0) return(NULL);
 if (size<=sectorindex) return(NULL);
 return(vector_CISectorPtr[sectorindex]);
}
//----------------------------------------------------------------------------------------------------
//получить указатель на вектор стен
//----------------------------------------------------------------------------------------------------
vector<SWall>* CEngine_Base::GetVectorSWallPtr(void)
{
 return(&vector_SWall);
}
//----------------------------------------------------------------------------------------------------
//обработка анимации
//----------------------------------------------------------------------------------------------------
void CEngine_Base::Processing(void)
{
 long size=vector_SWall.size();
 for(long n=0;n<size;n++)
 {
  //выполняем анимацию текстуры
  vector_SWall[n].cTextureFollow_Up.Processing();
  vector_SWall[n].cTextureFollow_Down.Processing();
 }
}
//----------------------------------------------------------------------------------------------------
//получить список стен, с которыми пересекается объект 
//----------------------------------------------------------------------------------------------------
long CEngine_Base::GetIntersectionList(vector<long> *vector_collizion_ptr,float x,float y,float radius,float z,float height)
{ 
 if (vector_collizion_ptr!=NULL) vector_collizion_ptr->clear();
 vector<unsigned long> vector_wall_index;
 cWallMap_Ptr->GetWallIndexList(vector_wall_index,x,y,radius);
 long collizion=0;
 long size=vector_wall_index.size();
 for(long n=0;n<size;n++)
 {
  unsigned long index=vector_wall_index[n];
  SWall *sWall_ptr=&(vector_SWall[index]);
  float xw1=sWall_ptr->X1;
  float yw1=sWall_ptr->Y1;
  float xw2=sWall_ptr->X2;
  float yw2=sWall_ptr->Y2;
  float xl1=xw1-x;
  float yl1=yw1-y;
  float xl2=xw2-x;
  float yl2=yw2-y;
  float a=xl2-xl1;
  float b=yl2-yl1;
  float kd=a*a+b*b;
  float ld=2*a*xl1+2*b*yl1;
  float md=xl1*xl1+yl1*yl1-radius*radius;
  float d=ld*ld-4*kd*md;//дескриминант
  if (d<0) continue;//нет пересечения
  float sd=sqrt(d);
  float l1=(-ld+sd)/(2*kd);
  float l2=(-ld-sd)/(2*kd);
  float xp1=xl1+l1*a;
  float yp1=yl1+l1*b;
  float xp2=xl1+l2*a;
  float yp2=yl1+l2*b;//2 точки пересечения
  bool impact=false;
  if (((xp1>=xl1 && xp1<=xl2) || (xp1>=xl2 && xp1<=xl1)) &&
     ((yp1>=yl1 && yp1<=yl2) || (yp1>=yl2 && yp1<=yl1))) impact=true;
  if (((xp2>=xl1 && xp2<=xl2) || (xp2>=xl2 && xp2<=xl1)) &&
     ((yp2>=yl1 && yp2<=yl2) || (yp2>=yl2 && yp2<=yl1))) impact=true;
  if (impact==false) continue;
  if (sWall_ptr->Frontier==false)
  {
   if (vector_collizion_ptr!=NULL) vector_collizion_ptr->push_back(index);
   collizion++;
   continue;
  }
  CISector *cISectorPtr1=GetCISectorPtrForIndex(sWall_ptr->Sector1);
  CISector *cISectorPtr2=GetCISectorPtrForIndex(sWall_ptr->Sector2);
  if (cISectorPtr1==NULL || cISectorPtr2==NULL) continue;
  long down1=cISectorPtr1->GetDown();
  long up1=cISectorPtr1->GetUp();
  long down2=cISectorPtr2->GetDown();
  long up2=cISectorPtr2->GetUp();
  long bottom=static_cast<long>(z);
  long top=static_cast<long>(z+height);
  if (!(bottom<down1-height/2 || bottom<down2-height/2 || top>up1-height/4 || top>up2-height/4)) continue;
  if (vector_collizion_ptr!=NULL) vector_collizion_ptr->push_back(index);
  collizion++;
 }
 return(collizion);
}

//----------------------------------------------------------------------------------------------------
//заполнение линии буфера текстур пола
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawFloorLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Bottom)
{
 if (y2<WindowYCenterWithOffset) return;
 if (y1>=WindowHeight) return;
 if (y1<WindowYCenterWithOffset) y1=WindowYCenterWithOffset;
 if (y2>=WindowHeight) y2=WindowHeight-1;
 if (x>sVisualPlanes_Bottom.MaxX) sVisualPlanes_Bottom.MaxX=x;
 if (x<sVisualPlanes_Bottom.MinX) sVisualPlanes_Bottom.MinX=x;
 if (y1<sVisualPlanes_Bottom.TopY[x]) sVisualPlanes_Bottom.TopY[x]=y1;
 if (y2>sVisualPlanes_Bottom.BottomY[x]) sVisualPlanes_Bottom.BottomY[x]=y2;
}
//----------------------------------------------------------------------------------------------------
//заполнение линии буфера текстур потолка
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawFlowLine(long x,long y1,long y2,SVisualPlanes &sVisualPlanes_Top)
{
 if (y2<0) return;
 if (y1>WindowYCenterWithOffset) return;
 if (y1<0) y1=0;
 if (y2>WindowYCenterWithOffset) y2=WindowYCenterWithOffset;
 if (x>sVisualPlanes_Top.MaxX) sVisualPlanes_Top.MaxX=x;
 if (x<sVisualPlanes_Top.MinX) sVisualPlanes_Top.MinX=x;
 if (y1<sVisualPlanes_Top.TopY[x]) sVisualPlanes_Top.TopY[x]=y1;
 if (y2>sVisualPlanes_Top.BottomY[x]) sVisualPlanes_Top.BottomY[x]=y2;
}
//----------------------------------------------------------------------------------------------------
//поиск указателя на пиксель текстуры
//----------------------------------------------------------------------------------------------------
unsigned long* CEngine_Base::FindTexture(long scale,long texture_u,long texture_index,long *mask,long *texture_dv,long *texture_v)
{
 if (scale<=0) scale=1;
 long level=0;
 while(scale>=2)
 {
  scale>>=1;
  level++;
 }
 if (level>=TEXTURE_MIPMAP_LEVEL) level=TEXTURE_MIPMAP_LEVEL-1;
 *mask=(1<<TEXTURE_SIZE_POW)-1;
 long tsize=(1<<(TEXTURE_SIZE_POW-level));
 texture_u=texture_u&(*mask);//texture_u бывает отрицательным, поэтому просто маскируем младшие биты
 (*mask)>>=level;
 (*texture_dv)>>=level;
 (*texture_v)>>=level;
 texture_u>>=level;
 return(cGraphics.GetTexture(level,texture_index)+texture_u*tsize);
}

//----------------------------------------------------------------------------------------------------
//рисование столбца монолитной стены
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawSimpleLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 long n;
 //заполняем линии буфера глубины
 SetZBufferVerticalLineValue(screen_x,TopLine[screen_x],BottomLine[screen_x],z);

 unsigned long *video_ptr;
 float texture_dv_float=height/(screen_y_bottom-screen_y_top+1)*TEXTURE_SCALE;
 float texture_v_float=0;
 if (screen_y_bottom>BottomLine[screen_x]) screen_y_bottom=BottomLine[screen_x];
 if (screen_y_top>BottomLine[screen_x])
 {
  long offset=BottomLine[screen_x];
  texture_v_float=texture_dv_float*(offset-screen_y_top);
  screen_y_top=offset;
 }
 if (screen_y_bottom<TopLine[screen_x]) screen_y_bottom=TopLine[screen_x];
 if (screen_y_top>=TopLine[screen_x])
 {
  DrawFlowLine(screen_x,TopLine[screen_x],screen_y_top-1,sVisualPlanes_Top);
  TopLine[screen_x]=static_cast<long>(screen_y_top);
 }
 else
 {
  long offset=TopLine[screen_x];
  texture_v_float=texture_dv_float*(offset-screen_y_top);
  screen_y_top=offset;
 }
 if (screen_y_bottom<=BottomLine[screen_x])
 {
  DrawFloorLine(screen_x,screen_y_bottom+1,BottomLine[screen_x],sVisualPlanes_Bottom);
  BottomLine[screen_x]=static_cast<long>(screen_y_bottom);
 }
 else screen_y_bottom=BottomLine[screen_x];
 long texture_dv=static_cast<long>(texture_dv_float*(1UL<<FIXMATH_SHIFT));
 texture_v_float-=texture_dv_float*(screen_y_top-(long)screen_y_top-1);

 long texture_v=static_cast<long>(texture_v_float*(1UL<<FIXMATH_SHIFT));
 TopLine[screen_x]=static_cast<long>(screen_y_bottom);
 BottomLine[screen_x]=static_cast<long>(screen_y_top);
 video_ptr=VideoBufferPtr+screen_x+((long)screen_y_top)*VideoLineWidth;
 long texture_scale=texture_dv>>FIXMATH_SHIFT;//коэффициент сжатия
 long mask;
 unsigned long *texture_ptr=FindTexture(texture_scale,texture_u,texture_index,&mask,&texture_dv,&texture_v);
 SetLine++;
 long bright=MAX_BRIGHTNESS_VALUE-z*PlayerPowerLighting;
 long sector_bright=vector_CISectorPtr[sector]->GetLighting();
 if (bright<sector_bright) bright=sector_bright;
 //переменная для хранения текущего цвета текстуры
 unsigned long color;
 //указатель на компоненты переменной хранения текущего цвета текстуры (нужно для коррекции яркости)
 unsigned char *color_byte_ptr=reinterpret_cast<unsigned char*>(&color);
 long s1=static_cast<long>(screen_y_top);
 long s2=static_cast<long>(screen_y_bottom);
 for(n=s1;n<=s2;n++,video_ptr+=VideoLineWidth,texture_v+=texture_dv)
 {
  color=*(((texture_v>>FIXMATH_SHIFT)&mask)+texture_ptr);  
  /*
  (*color_byte_ptr)=((*color_byte_ptr)*bright)>>8;
  (*(color_byte_ptr+1))=((*(color_byte_ptr+1))*bright)>>8;
  (*(color_byte_ptr+2))=((*(color_byte_ptr+2))*bright)>>8;
   *(video_ptr)=color;
   */  
  unsigned long r=((*color_byte_ptr)*bright)>>8;
  unsigned long g=((*(color_byte_ptr+1))*bright)&0xff00;
  unsigned long b=(((*(color_byte_ptr+2))*bright)&0xff00)<<8;
  unsigned long new_color=r|g|b;
  *(video_ptr)=new_color;
 
 }

}
//----------------------------------------------------------------------------------------------------
//рисование столбца нижней стены
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawLowerLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 long n;
 unsigned long *video_ptr; 
 float bottom_top=(screen_y_bottom-screen_y_top+1);
 if (screen_y_top<TopLine[screen_x]) SetLine++;
 if (screen_y_bottom<TopLine[screen_x]) screen_y_bottom=TopLine[screen_x];
 if (screen_y_bottom<BottomLine[screen_x]) DrawFloorLine(screen_x,screen_y_bottom+1,BottomLine[screen_x],sVisualPlanes_Bottom);
                       else screen_y_bottom=BottomLine[screen_x];
 if (screen_y_top<screen_y_bottom)
 {
  float texture_v_float=0;
  float texture_dv_float=height/bottom_top*TEXTURE_SCALE;
  if (screen_y_top<TopLine[screen_x])
  {
   long offset=TopLine[screen_x];
   texture_v_float=texture_dv_float*(offset-screen_y_top);
   screen_y_top=offset;
  }
  texture_v_float-=texture_dv_float*(screen_y_top-(long)screen_y_top-1);
  long texture_v=static_cast<long>(texture_v_float*(1UL<<FIXMATH_SHIFT));
  long texture_dv=static_cast<long>(texture_dv_float*(1UL<<FIXMATH_SHIFT));
  video_ptr=VideoBufferPtr+screen_x+((long)screen_y_top)*VideoLineWidth;
  //заполняем линии буфера глубины
  SetZBufferVerticalLineValue(screen_x,screen_y_top,BottomLine[screen_x],z);
  long texture_scale=texture_dv>>FIXMATH_SHIFT;//коэффициент сжатия
  long mask;
  unsigned long *texture_ptr=FindTexture(texture_scale,texture_u,texture_index,&mask,&texture_dv,&texture_v);
  
  long bright=MAX_BRIGHTNESS_VALUE-z*PlayerPowerLighting;
  long sector_bright=vector_CISectorPtr[sector]->GetLighting();
  if (bright<sector_bright) bright=sector_bright;
  //переменная для хранения текущего цвета текстуры
  unsigned long color;
  //указатель на компоненты переменной хранения текущего цвета текстуры (нужно для коррекции яркости)
  unsigned char *color_byte_ptr=reinterpret_cast<unsigned char*>(&color);
  for(n=static_cast<long>(screen_y_top);n<=static_cast<long>(screen_y_bottom);n++,video_ptr+=VideoLineWidth,texture_v+=texture_dv)
  {
   color=*(((texture_v>>FIXMATH_SHIFT)&mask)+texture_ptr);
   //вычисляем освещённость
   unsigned long r=((*color_byte_ptr)*bright)>>8;
   unsigned long g=((*(color_byte_ptr+1))*bright)&0xff00;
   unsigned long b=(((*(color_byte_ptr+2))*bright)&0xff00)<<8;
   unsigned long new_color=r|g|b;
   *(video_ptr)=new_color;


   /*(*color_byte_ptr)=((*color_byte_ptr)*bright)>>8;
   (*(color_byte_ptr+1))=((*(color_byte_ptr+1))*bright)>>8;
   (*(color_byte_ptr+2))=((*(color_byte_ptr+2))*bright)>>8;*/
   //*(video_ptr)=color;
  }
  if (screen_y_top<BottomLine[screen_x]) BottomLine[screen_x]=static_cast<long>(screen_y_top);
 }
 else
 {
  if (screen_y_bottom<BottomLine[screen_x])
  {
   //заполняем линии буфера глубины
   SetZBufferVerticalLineValue(screen_x,screen_y_bottom,BottomLine[screen_x],z);
   BottomLine[screen_x]=static_cast<long>(screen_y_bottom);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//рисование столбца верхней стены
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawUpperLine(long z,long screen_x,float screen_y_top,float screen_y_bottom,long texture_u,long height,long texture_index,long sector,SVisualPlanes &sVisualPlanes_Top,SVisualPlanes &sVisualPlanes_Bottom)
{
 long n;
 unsigned long *video_ptr;
 float bottom_top=(screen_y_bottom-screen_y_top+1);
 if (screen_y_bottom>BottomLine[screen_x]) SetLine++;
 if (screen_y_top>BottomLine[screen_x]) screen_y_top=BottomLine[screen_x];
 if (screen_y_top>TopLine[screen_x]) DrawFlowLine(screen_x,TopLine[screen_x],screen_y_top-1,sVisualPlanes_Top);
 if (screen_y_bottom>BottomLine[screen_x]) screen_y_bottom=BottomLine[screen_x];
 if (screen_y_top<screen_y_bottom)
 {
  float texture_v_float=0;
  float texture_dv_float=height/bottom_top*TEXTURE_SCALE;
  if (screen_y_top<=TopLine[screen_x])
  {
   long offset=TopLine[screen_x];
   texture_v_float=texture_dv_float*(offset-screen_y_top);
   screen_y_top=offset;
  }
  texture_v_float-=texture_dv_float*(screen_y_top-(long)screen_y_top-1);
  long texture_v=static_cast<long>(texture_v_float*(1UL<<FIXMATH_SHIFT));
  long texture_dv=static_cast<long>(texture_dv_float*(1UL<<FIXMATH_SHIFT));
  video_ptr=VideoBufferPtr+screen_x+((long)screen_y_top)*VideoLineWidth;
  //заполняем линии буфера глубины
  SetZBufferVerticalLineValue(screen_x,TopLine[screen_x],screen_y_bottom,z);
  long texture_scale=texture_dv>>FIXMATH_SHIFT;//коэффициент сжатия
  long mask;
  unsigned long *texture_ptr=FindTexture(texture_scale,texture_u,texture_index,&mask,&texture_dv,&texture_v);
  long bright=MAX_BRIGHTNESS_VALUE-z*PlayerPowerLighting;
  long sector_bright=vector_CISectorPtr[sector]->GetLighting();
  if (bright<sector_bright) bright=sector_bright;
  //переменная для хранения текущего цвета текстуры
  unsigned long color;
  //указатель на компоненты переменной хранения текущего цвета текстуры (нужно для коррекции яркости)
  unsigned char *color_byte_ptr=reinterpret_cast<unsigned char*>(&color);
  for(n=static_cast<long>(screen_y_top);n<=static_cast<long>(screen_y_bottom);n++,video_ptr+=VideoLineWidth,texture_v+=texture_dv)
  {
   color=*(((texture_v>>FIXMATH_SHIFT)&mask)+texture_ptr);
   //вычисляем освещённость
   /*(*color_byte_ptr)=((*color_byte_ptr)*bright)>>8;
   (*(color_byte_ptr+1))=((*(color_byte_ptr+1))*bright)>>8;
   (*(color_byte_ptr+2))=((*(color_byte_ptr+2))*bright)>>8;
   *(video_ptr)=color;
   */

   unsigned long r=((*color_byte_ptr)*bright)>>8;
   unsigned long g=((*(color_byte_ptr+1))*bright)&0xff00;
   unsigned long b=(((*(color_byte_ptr+2))*bright)&0xff00)<<8;
   unsigned long new_color=r|g|b;
   *(video_ptr)=new_color;
  }
  if (screen_y_bottom>TopLine[screen_x]) TopLine[screen_x]=static_cast<long>(screen_y_bottom);
 }
 else
 {
  if (screen_y_top>TopLine[screen_x])
  {
   //заполняем линии буфера глубины
   SetZBufferVerticalLineValue(screen_x,TopLine[screen_x],screen_y_top,z);
   TopLine[screen_x]=static_cast<long>(screen_y_top);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//очистка буфера полов и потолков
//----------------------------------------------------------------------------------------------------
void CEngine_Base::ClearFloorFlowBuffer(void)
{
 long size=ZLineBuffer.size();
 for(long n=0;n<size;n++) ZLineBuffer[n].clear();
 //long len=(WindowWidth*WindowHeight)*sizeof(long);
 //unsigned char *dpt=(unsigned char *)ZBuffer;
 //memset(dpt,0xff,len);
}
//----------------------------------------------------------------------------------------------------
//рисование потолков
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawFlow(long sector_index,const SVisualPlanes &sVisualPlanes_Top)
{
 //параметры сектора
 long level=vector_CISectorPtr[sector_index]->GetUp();
 long texture=vector_CISectorPtr[sector_index]->GetCTextureFollow_Up_Ptr()->GetCurrentTexture().TextureIndex;
 long bright=vector_CISectorPtr[sector_index]->GetLighting();
 long z=static_cast<long>((level-PlayerZ)*(WindowHeight/2));

 long screen_min_x;
 long screen_max_x;
 long x;
 long y;
 screen_min_x=sVisualPlanes_Top.MinX;
 screen_max_x=sVisualPlanes_Top.MaxX;
 if (screen_max_x<screen_min_x) return;
 long y_top=sVisualPlanes_Top.TopY[screen_min_x];
 long y_bottom=sVisualPlanes_Top.BottomY[screen_min_x];
 for(y=y_top;y<=y_bottom;y++) X_Table[y]=screen_min_x;

 for(x=screen_min_x;x<=screen_max_x;x++)
 {
  long zd;
  long top_plane_y=sVisualPlanes_Top.TopY[x];
  long bottom_plane_y=sVisualPlanes_Top.BottomY[x];
  if (bottom_plane_y<top_plane_y) continue;//при возможных пропусках точек на границах (а они есть), алгоритм развалится
  //если верхняя линия поднимается
  while(top_plane_y<y_top)
  {
   y_top--;
   X_Table[y_top]=x;
  }
  //если нижняя линия опускается
  while(bottom_plane_y>y_bottom)
  {
   y_bottom++;
   X_Table[y_bottom]=x;
  }

  //если верхняя линия опускается
  zd=(WindowYCenterWithOffset-y_top)+1;
  while(y_top<top_plane_y)
  {
   long dist=z/zd;
   long scale=dist/zd;
   DrawTextureLine(dist,scale,bright,texture,y_top,X_Table[y_top],x-1);
   y_top++;
   zd--;
  }

  //если нижняя линия поднимается
  zd=(WindowYCenterWithOffset-y_bottom)+1;
  while(y_bottom>bottom_plane_y)
  {
   long dist=z/zd;
   long scale=dist/zd;
   DrawTextureLine(dist,scale,bright,texture,y_bottom,X_Table[y_bottom],x-1);
   y_bottom--;
   zd++;
  }
 }
 //заливаем промежуток между top и bottom
 long zd=(WindowYCenterWithOffset-y_top)+1;
 for(y=y_top;y<=y_bottom;y++,zd--)
 {
  long dist=z/zd;
  long scale=dist/zd;
  DrawTextureLine(dist,scale,bright,texture,y,X_Table[y],screen_max_x);
 }
}
//----------------------------------------------------------------------------------------------------
//рисование полов
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawFloor(long sector_index,const SVisualPlanes &sVisualPlanes_Bottom)
{
 //параметры сектора
 long level=vector_CISectorPtr[sector_index]->GetDown();
 long texture=vector_CISectorPtr[sector_index]->GetCTextureFollow_Down_Ptr()->GetCurrentTexture().TextureIndex;
 long bright=vector_CISectorPtr[sector_index]->GetLighting();
 long z=static_cast<long>((PlayerZ-level)*(WindowHeight/2));

 long screen_min_x;
 long screen_max_x;
 long x;
 long y;
 screen_min_x=sVisualPlanes_Bottom.MinX;
 screen_max_x=sVisualPlanes_Bottom.MaxX;
 if (screen_max_x<screen_min_x) return;
 long y_top=sVisualPlanes_Bottom.TopY[screen_min_x];
 long y_bottom=sVisualPlanes_Bottom.BottomY[screen_min_x];
 for(y=y_top;y<=y_bottom;y++) X_Table[y]=screen_min_x;
 for(x=screen_min_x;x<=screen_max_x;x++)
 {
  long zd;
  long top_plane_y=sVisualPlanes_Bottom.TopY[x];
  long bottom_plane_y=sVisualPlanes_Bottom.BottomY[x];
  if (bottom_plane_y<top_plane_y) continue;//при возможных пропусках точек на границах (а они есть), алгоритм развалится

  //если верхняя линия поднимается
  while(top_plane_y<y_top)
  {
   y_top--;
   X_Table[y_top]=x;
  }
  //если нижняя линия опускается
  while(bottom_plane_y>y_bottom)
  {
   y_bottom++;
   X_Table[y_bottom]=x;
  }

  //если верхняя линия опускается
  zd=(y_top-WindowYCenterWithOffset)+1;
  while(y_top<top_plane_y)
  {
   long dist=z/zd;
   long scale=dist/zd;
   DrawTextureLine(dist,scale,bright,texture,y_top,X_Table[y_top],x-1);
   y_top++;
   zd++;
  }

  //если нижняя линия поднимается
  zd=(y_bottom-WindowYCenterWithOffset)+1;
  while(y_bottom>bottom_plane_y)
  {
   long dist=z/zd;
   long scale=dist/zd;
   DrawTextureLine(dist,scale,bright,texture,y_bottom,X_Table[y_bottom],x-1);
   y_bottom--;
   zd--;
  }
 }
 //заливаем промежуток между top и bottom
 long zd=(y_top-WindowYCenterWithOffset)+1;
 for(y=y_top;y<=y_bottom;y++,zd++)
 {
  long dist=z/zd;
  long scale=dist/zd;
  DrawTextureLine(dist,scale,bright,texture,y,X_Table[y],screen_max_x);
 }
}
//----------------------------------------------------------------------------------------------------
//рисование текстурированной линии пола или потолка
//----------------------------------------------------------------------------------------------------
void CEngine_Base::DrawTextureLine(long dist,long scale,long sector_bright,long texture_index,long screen_y,long left_x,long right_x)
{
 //координаты игрока
 long player_x=static_cast<long>(PlayerX*(1UL<<FIXMATH_FLOOR_FLOW_SHIFT));
 long player_y=static_cast<long>(PlayerY*(1UL<<FIXMATH_FLOOR_FLOW_SHIFT));

 long texture_du=dist*diTable[PlayerAngle]*TEXTURE_SCALE;
 long texture_dv=dist*djTable[PlayerAngle]*TEXTURE_SCALE;
 long texture_u=(dist*dsTable[PlayerAngle]+player_x)*TEXTURE_SCALE-texture_du*(WindowXCenter-1-left_x);
 long texture_v=(dist*dcTable[PlayerAngle]+player_y)*TEXTURE_SCALE-texture_dv*(WindowXCenter-1-left_x);
 long bright=MAX_BRIGHTNESS_VALUE-dist*PlayerPowerLighting;
 if (bright<sector_bright) bright=sector_bright;
 //определяем уровень текстуры
 long level=0;//полученный уровень текстуры
 while(scale>=2)
 {
  level++;
  scale>>=1;
 }
 if (level>=TEXTURE_MIPMAP_LEVEL) level=TEXTURE_MIPMAP_LEVEL-1;
 texture_v>>=level;
 texture_u>>=level;
 texture_dv>>=level;
 texture_du>>=level;
 //вычисляем маски
 long texture_mask_y=(1<<(TEXTURE_SIZE_POW-level))-1;
 long texture_mask_x=texture_mask_y<<(TEXTURE_SIZE_POW-level);
 long texture_shift_x=FIXMATH_FLOOR_FLOW_SHIFT-(TEXTURE_SIZE_POW-level);
 long texture_tsize_tsize=(1<<(TEXTURE_SIZE_POW-level))*(1<<(TEXTURE_SIZE_POW-level));
 unsigned long *texture_ptr=cGraphics.GetTexture(level,texture_index);
 if (texture_ptr==NULL) return;//текстура не найдена
 unsigned long *video_ptr=VideoBufferPtr+VideoLineWidth*screen_y+left_x;
 //переменная для хранения текущего цвета текстуры
 unsigned long color;
 //указатель на компоненты переменной хранения текущего цвета текстуры (нужно для коррекции яркости)
 unsigned char *color_byte_ptr=reinterpret_cast<unsigned char*>(&color);
 //выводим строку текстуры
 for(long x=left_x;x<=right_x;x++,video_ptr++,texture_u+=texture_du,texture_v+=texture_dv)
 {
  long u=(texture_u>>texture_shift_x)&texture_mask_x;
  long v=(texture_v>>FIXMATH_FLOOR_FLOW_SHIFT)&texture_mask_y;
  if ((u+v)<texture_tsize_tsize && (u+v)>=0)
  {
   color=*(texture_ptr+u+v);
   //вычисляем освещённость
   unsigned long r=((*color_byte_ptr)*bright)>>8;
   unsigned long g=((*(color_byte_ptr+1))*bright)&0xff00;
   unsigned long b=(((*(color_byte_ptr+2))*bright)&0xff00)<<8;
   unsigned long new_color=r|g|b;
   *(video_ptr)=new_color;

   /*(*color_byte_ptr)=((*color_byte_ptr)*bright)>>8;
   (*(color_byte_ptr+1))=((*(color_byte_ptr+1))*bright)>>8;
   (*(color_byte_ptr+2))=((*(color_byte_ptr+2))*bright)>>8;   
   *(video_ptr)=color;
   */
  }
 }
}
//----------------------------------------------------------------------------------------------------
//вывод спрайта
//----------------------------------------------------------------------------------------------------
void CEngine_Base::PutSprite(void)
{
 long X=550*1.28;
 long Y=550*1.28;
 long Z=128;
 long HEIGHT=128;
 long WIDTH=128;

 long len=WIDTH;
 long hgt=HEIGHT;

 //переводим координаты в систему координат, связанную с игроком
 float cos_angle=CosTable[PlayerAngle];
 float sin_angle=SinTable[PlayerAngle];
 long x_offset_player=X-PlayerX;
 long y_offset_player=Y-PlayerY;
 float z_with_player=x_offset_player*sin_angle+y_offset_player*cos_angle;
 if (z_with_player<=1) return;
 if (z_with_player>6400) return;//слишком далеко
 float x_with_player=x_offset_player*cos_angle-y_offset_player*sin_angle;
 float x_left_with_player=x_with_player-(len>>1);
 float x_right_with_player=x_with_player+(len>>1);
 //выполняем проецирование на экран
 long col_left=WindowXCenter+WindowXCenter*(x_left_with_player/z_with_player);
 long col_right=WindowXCenter+WindowXCenter*(x_right_with_player/z_with_player);
 if (col_left>=WindowWidth) return;//объект не виден
 if (col_right<0) return;//объект не виден
 long x_texture_step_on_screen=0;
 if (col_left!=col_right) x_texture_step_on_screen=(65536*len)/(col_right-col_left);
 long x_texture_offset=0;
 if (col_left<0)
 {
  x_texture_offset=-(x_texture_step_on_screen*col_left);
  col_left=0;
 }
 if (col_right>=WindowWidth) col_right=WindowWidth-1;
 long y_up_on_screen=WindowYCenterWithOffset-(-PlayerZ+Z)*WindowHeight/(2*z_with_player);
 long y_down_on_screen=WindowYCenterWithOffset-(-PlayerZ+Z-hgt)*WindowHeight/(2*z_with_player);
 if (y_down_on_screen==y_up_on_screen) return;//объект слился в линию
 long y_texture_step_on_screen=0;
 long y_texture_offset=0;
 y_texture_step_on_screen=(65536*hgt)/(y_down_on_screen-y_up_on_screen+1);
 if (y_up_on_screen<0)
 {
  y_texture_offset=-y_texture_step_on_screen*y_up_on_screen;
  y_up_on_screen=0;
 }
 if (y_down_on_screen>=WindowHeight) y_down_on_screen=WindowHeight-1;

 unsigned long *screen_ptr=VideoBufferPtr+col_left+VideoLineWidth*(long)y_up_on_screen;
 unsigned long *texture_ptr=cGraphics.GetTexture(0,0);//данные спрайта
 for(long col=col_left;col<=col_right;col++,x_texture_offset+=x_texture_step_on_screen,screen_ptr++)
 {
  unsigned long *texture_ptr_local=texture_ptr+((long)(x_texture_offset>>16))*hgt+1;
  unsigned long *screen_ptr_local=screen_ptr;
  vector<SZLine> *vector_SZLine_Ptr=&ZLineBuffer[col];
  long size=vector_SZLine_Ptr->size();
  if (size==0) continue;
  //ищем зону, в которую попадает текущая Y-координата спрайта
  long y_texture_offset_current=y_texture_offset;
  long row=y_up_on_screen;
  while(row<=y_down_on_screen)
  {
   long part;
   for(part=0;part<size;part++)
   {
    long Z=(*vector_SZLine_Ptr)[part].Z;
    long Y_Top=(*vector_SZLine_Ptr)[part].Y_Top;
    long Y_Bottom=(*vector_SZLine_Ptr)[part].Y_Bottom;
	if (row>=Y_Top && row<=Y_Bottom)//это наша зона
	{
     if (Z<z_with_player)//спрайт не может выводиться в этом фрагменте
	 {
      screen_ptr_local+=VideoLineWidth*(Y_Bottom-row+1);
	  y_texture_offset_current+=y_texture_step_on_screen*(Y_Bottom-row+1);
      row=Y_Bottom+1;//переносим y_texture_offset_current-координату в конец фрагмента и начинаем поиск фрагмента заново
	  break;
	 }
     //выводим фрагмент в пределах зоны
	 if (Y_Bottom>y_down_on_screen) Y_Bottom=y_down_on_screen;
     for(;row<=Y_Bottom;row++,screen_ptr_local+=VideoLineWidth,y_texture_offset_current+=y_texture_step_on_screen)
	 {
      unsigned long color_value=*(texture_ptr_local+(long)(y_texture_offset_current>>16));
      //if (color_value==117) continue;//прозрачный цвет
      *screen_ptr_local=color_value;
	 }
     //начинаем поиск зоны заново
     break;
	}
   }
   if (part==size) break;//фрагмент не был найден
  }
 }
}


//----------------------------------------------------------------------------------------------------
//рисование отрезка
//----------------------------------------------------------------------------------------------------
void CEngine_Base::Line(long x1,long y1,long x2,long y2,unsigned long color)
{
 if (x1<0 && x2<0) return;
 if (y1<0 && y2<0) return;
 if (x1>=WindowWidth && x2>=WindowWidth) return;
 if (y1>=WindowHeight && y2>=WindowHeight) return;
 long dx=abs(x2-x1);
 long dy=abs(y2-y1);
 long sx=x2>=x1?1:-1;
 long sy=y2>=y1?1:-1;
 if (dy<=dx)
 {
  long d=(dy<<1)-dx;
  long d1=dy<<1;
  long d2=(dy-dx)<<1;
  PutPixel(x1,y1,color);
  for(long x=x1+sx,y=y1,i=1;i<=dx;i++,x+=sx)
  {
   if (d>0)
   {
    d+=d2;
    y+=sy;
   }
   else d+=d1;
   PutPixel(x,y,color);
  }
 }
 else
 {
  long d=(dx<<1)-dy;
  long d1=dx<<1;
  long d2=(dx-dy)<<1;
  PutPixel(x1,y1,color);
  for(long x=x1,y=y1+sy,i=1;i<=dy;i++,y+=sy)
  {
   if (d>0)
   {
    d+=d2;
    x+=sx;
   }
   else d+=d1;
   PutPixel(x,y,color);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//вывод точки
//----------------------------------------------------------------------------------------------------
void CEngine_Base::PutPixel(long x,long y,unsigned long color)
{
 if (x<0) return;
 if (y<0) return;
 if (x>=WindowWidth) return;
 if (y>=WindowHeight) return;
 *(VideoBufferPtr+x+y*VideoLineWidth)=color;
}
//----------------------------------------------------------------------------------------------------
//заполнение строки в Z-буфере
//----------------------------------------------------------------------------------------------------
void CEngine_Base::SetZBufferVerticalLineValue(long x,long y1,long y2,long z)
{
 SZLine sZLine;
 sZLine.Y_Top=y1;
 sZLine.Y_Bottom=y2;
 sZLine.Z=z;
 ZLineBuffer[x].push_back(sZLine);
}
