#include "cgraph.h"

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CGraphics::CGraphics(void)
{
 long n;
 for(n=0;n<TEXTURE_MIPMAP_LEVEL;n++) Texture[n]=NULL;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CGraphics::~CGraphics(void)
{
 Release();
}
//----------------------------------------------------------------------------------------------------
//задать рабочий каталог
//----------------------------------------------------------------------------------------------------
void CGraphics::SetPath(string path)
{
 Path=path;
}
//----------------------------------------------------------------------------------------------------
//инициализация
//----------------------------------------------------------------------------------------------------
void CGraphics::Init(vector<SUseTexture> &vector_SUseTexture)
{
 Release();
 LoadTexture(vector_SUseTexture);
}
//----------------------------------------------------------------------------------------------------
//освободить память
//----------------------------------------------------------------------------------------------------
void CGraphics::Release(void)
{
 long n;
 for(n=0;n<TEXTURE_MIPMAP_LEVEL;n++)
 {
  if (Texture[n]!=NULL) delete[](Texture[n]);
  Texture[n]=NULL;
 }
}
//----------------------------------------------------------------------------------------------------
//загрузить текстуры
//----------------------------------------------------------------------------------------------------
void CGraphics::LoadTexture(vector<SUseTexture> &vector_SUseTexture)
{
 long n;
 string filename=Path+"Data\\texture.tia";
 FILE *file=fopen(filename.c_str(),"rb");
 if (file==NULL) return;
 //загружаем текстуры
 vector<unsigned long *> vector_texture_data;
 while(1)
 {
  //читаем имя текстуры
  string name;
  while(1)
  {
   char s;
   if (fread(&s,sizeof(char),1,file)<1) break;
   if (s==0) break;
   name+=s;
  }
  //читаем размер текстуры
  long width;
  long height;
  if (fread(&width,sizeof(long),1,file)<1) break;
  if (fread(&height,sizeof(long),1,file)<1) break;
  //ищем такую текстуру в списке
  bool ok=false;
  long sut=vector_SUseTexture.size();
  for(long n=0;n<sut;n++)
  {
   if (vector_SUseTexture[n].Name.compare(name)==0)//текстура есть в списке
   {
    ok=true;
	vector_SUseTexture[n].Index=vector_texture_data.size();
	break;
   }
  }
  if (ok==false)//тестура не найдена
  {
   fseek(file,width*height*sizeof(unsigned long),SEEK_CUR);
   continue;
  }
  //читаем данные текстуры
  unsigned long *image=new unsigned long[width*height];
  if (fread(image,sizeof(unsigned long),width*height,file)<width*height)
  {
   delete[](image);
   break;
  }
  //приводим текстуру к нужному размеру
  long t_size=(1<<TEXTURE_SIZE_POW);
  float dx=static_cast<float>(width)/static_cast<float>(t_size);
  float dy=static_cast<float>(height)/static_cast<float>(t_size);
  unsigned long *new_image=new unsigned long[t_size*t_size];
  float y=0;
  for(long ny=0;ny<t_size;ny++,y+=dy)
  {
   float x=0;
   for(long nx=0;nx<t_size;nx++,x+=dx)
   {
    long offset_ni=nx+ny*t_size;
    long offset_i=static_cast<long>(x)+static_cast<long>(y)*width;
	new_image[offset_ni]=image[offset_i];
   }
  }
  vector_texture_data.push_back(new_image);
  delete[](image);
 }
 fclose(file);
 //переносим текстуры
 long amount=vector_texture_data.size();
 long size_one=(1UL<<TEXTURE_SIZE_POW)*(1UL<<TEXTURE_SIZE_POW);
 long size=amount*size_one;
 for(n=0;n<TEXTURE_MIPMAP_LEVEL;n++) Texture[n]=new unsigned long[size>>n];
 for (n=0;n<amount;n++)
 {
  unsigned long *ptr=vector_texture_data[n];
  for(long y=0;y<(1UL<<TEXTURE_SIZE_POW);y++)
  {
   for(long x=0;x<(1UL<<TEXTURE_SIZE_POW);x++)
   {
    long offset_in=y*(1UL<<TEXTURE_SIZE_POW)+x;
	long offset_out=x*(1UL<<TEXTURE_SIZE_POW)+y;
    Texture[0][n*size_one+offset_out]=ptr[offset_in];
   }
  }
  delete[](vector_texture_data[n]);
 }
 vector_texture_data.clear();
 //строим пирамиду уровней текстуры
 MipMapping(amount);
}
//----------------------------------------------------------------------------------------------------
//построение пирамиды уровней текстур
//----------------------------------------------------------------------------------------------------
void CGraphics::MipMapping(long t)
{
 //пирамидальное фильтрование
 long x,y,n;
 for(n=0;n<t;n++)
 {
  long size=(1<<TEXTURE_SIZE_POW)>>1;
  for(long level=1;level<TEXTURE_MIPMAP_LEVEL;level++,size>>=1)
  {
   for(x=0;x<size;x++)
   {
    for(y=0;y<size;y++)
    {
     long psize=size<<1;
     long color1=Texture[level-1][n*psize*psize+x*2+y*2*psize];
     long color2=Texture[level-1][n*psize*psize+x*2+y*2*psize+1];
     long color3=Texture[level-1][n*psize*psize+x*2+y*2*psize+1+psize];
     long color4=Texture[level-1][n*psize*psize+x*2+y*2*psize+psize];
	 unsigned char bt[4]={color1&0xFF,color2&0xFF,color3&0xFF,color4&0xFF};
	 unsigned char gt[4]={(color1>>8)&0xFF,(color2>>8)&0xFF,(color3>>8)&0xFF,(color4>>8)&0xFF};
	 unsigned char rt[4]={(color1>>16)&0xFF,(color2>>16)&0xFF,(color3>>16)&0xFF,(color4>>16)&0xFF};
	 long r=(rt[0]+rt[1]+rt[2]+rt[3])/4;
	 long g=(gt[0]+gt[1]+gt[2]+gt[3])/4;
	 long b=(bt[0]+bt[1]+bt[2]+bt[3])/4;
     unsigned long color=r;
     color<<=8;
     color|=g;
     color<<=8;
     color|=b;
     Texture[level][n*size*size+x+y*size]=color;
    }
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//загрузить спрайты
//----------------------------------------------------------------------------------------------------
void CGraphics::LoadSprites(void)
{

}
//----------------------------------------------------------------------------------------------------
//получить данные массива текстур
//----------------------------------------------------------------------------------------------------
unsigned long* CGraphics::GetTexture(long level,long index)
{
 long tsize=(1UL<<(2*(TEXTURE_SIZE_POW-level)));
 return(Texture[level]+index*tsize);
}
