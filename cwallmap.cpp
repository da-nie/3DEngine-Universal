#include "cwallmap.h"

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CWallMap::CWallMap(long grid_x_step,long grid_y_step)
{
 MinX=0;
 MaxX=0;
 MinY=0;
 MaxY=0;
 GridXStep=grid_x_step;
 GridYStep=grid_y_step;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CWallMap::~CWallMap()
{
 Release();
}
//----------------------------------------------------------------------------------------------------
//удалить массив стен для сетки
//----------------------------------------------------------------------------------------------------
void CWallMap::Release(void)
{
 MinX=0;
 MaxX=0;
 MinY=0;
 MaxY=0;
 WallIndexMap.clear();
 vector_UsedLineIndex.clear();
}
//----------------------------------------------------------------------------------------------------
//создать массив стен для сетки
//----------------------------------------------------------------------------------------------------
void CWallMap::Create(vector<SWall> &vector_SWall)
{
 long n;
 Release();
 if (GridXStep==0) return;
 if (GridYStep==0) return;

 //ищем максимальную и минимальную координату карты
 long size=vector_SWall.size();
 vector_UsedLineIndex.resize(size);
 for(n=0;n<size;n++)
 {
  vector_UsedLineIndex[n]=0;
  long x1=vector_SWall[n].X1;
  long y1=vector_SWall[n].Y1;
  long x2=vector_SWall[n].X2;
  long y2=vector_SWall[n].Y2;
  if (n==0)
  {
   MaxX=x1;
   MinX=x1;
   MaxY=y1;
   MinY=y1;
  }
  if (MinX>x1) MinX=x1;
  if (MinY>y1) MinY=y1;
  if (MaxX<x1) MaxX=x1;
  if (MaxY<y1) MaxY=y1;

  if (MinX>x2) MinX=x2;
  if (MinY>y2) MinY=y2;
  if (MaxX<x2) MaxX=x2;
  if (MaxY<y2) MaxY=y2;
 }
 //разбиваем карту на блоки
 long grid_size_x=(MaxX-MinX)/GridXStep+1;
 long grid_size_y=(MaxY-MinY)/GridYStep+1;
 WallIndexMap.resize(grid_size_y);
 for(n=0;n<grid_size_y;n++) WallIndexMap[n].resize(grid_size_x);
 for(long ny=0;ny<grid_size_y;ny++)
 {
  for(long nx=0;nx<grid_size_x;nx++)
  {
   WallIndexMap[ny][nx].clear();

   long grid_left_x=nx*GridXStep;
   long grid_top_y=ny*GridYStep;

   long grid_right_x=(nx+1)*GridXStep;
   long grid_bottom_y=(ny+1)*GridYStep;

   for(n=0;n<size;n++)
   {
    long x1=vector_SWall[n].X1-MinX;
    long y1=vector_SWall[n].Y1-MinY;
    long x2=vector_SWall[n].X2-MinX;
    long y2=vector_SWall[n].Y2-MinY;
    
    if (x1<grid_left_x && x2<grid_left_x) continue;
    if (y1<grid_top_y && y2<grid_top_y) continue;

    if (x1>grid_right_x && x2>grid_right_x) continue;
    if (y1>grid_bottom_y && y2>grid_bottom_y) continue;

    WallIndexMap[ny][nx].push_back(n);
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//получить список индексов стен на сетке, которые может пересекать объект
//----------------------------------------------------------------------------------------------------
void CWallMap::GetWallIndexList(vector<unsigned long> &vector_wall,float x,float y,float radius)
{ 
 vector_wall.clear();
 if (GridXStep==0) return;
 if (GridYStep==0) return;
 long y_size=WallIndexMap.size();
 if (y_size==0) return;
 long x_size=WallIndexMap[0].size();
 //определяем минимальную и максимальную границу с учётом радиуса объекта
 float x_min=x-radius;
 float y_min=y-radius;

 float x_max=x+radius;
 float y_max=y+radius;

 long size_ul=vector_UsedLineIndex.size();
 for(long n=0;n<size_ul;n++) vector_UsedLineIndex[n]=0; 
 //собираем список линий, исключая одинаковые

 if (x_min<MinX) x_min=MinX;
 if (x_max>MaxX) x_max=MaxX;
 if (y_min<MinY) y_min=MinY;
 if (y_max>MaxY) y_max=MaxY;
 
 x_min-=MinX;
 x_max-=MinX;

 y_min-=MinY;
 y_max-=MinY;

 x_min/=GridXStep;
 y_min/=GridYStep;
 x_max/=GridXStep;
 y_max/=GridYStep;
 long cx_min=static_cast<long>(x_min);
 long cx_max=static_cast<long>(x_max);
 long cy_min=static_cast<long>(y_min);
 long cy_max=static_cast<long>(y_max);

 for (long nx=cx_min;nx<=cx_max;nx++)
 {
  if (nx>=x_size) break;
  for (long ny=cy_min;ny<=cy_max;ny++)
  {
   if (ny>=y_size) break;
   vector<unsigned long> *vector_wall_ptr=&WallIndexMap[ny][nx];
   long size=vector_wall_ptr->size();
   for(long n=0;n<size;n++) 
   {
    unsigned long index=(*vector_wall_ptr)[n];
	if (vector_UsedLineIndex[index]!=0) continue;//стена уже добавлялась в список
    vector_UsedLineIndex[index]=1;
	vector_wall.push_back(index);
   }
  }
 }
}