#include "ctexturefollow.h"

//---------------------------------------------------------------------------
//конструктор
//---------------------------------------------------------------------------
CTextureFollow::CTextureFollow(void)
{
 CurrentPointer=0;
 dPointer=0;
 Mode=TEXTURE_FOLLOW_MODE_DISABLE;
 vector_STexture.clear();
}
//---------------------------------------------------------------------------
//деструктор
//---------------------------------------------------------------------------
CTextureFollow::~CTextureFollow(void)
{
 vector_STexture.clear();
}
//---------------------------------------------------------------------------
//сохранить анимацию текстуры
//---------------------------------------------------------------------------
void CTextureFollow::Save(FILE *file)
{
 fwrite(&Mode,sizeof(long),1,file);
 long size=vector_STexture.size();
 fwrite(&size,sizeof(long),1,file);
 for(long n=0;n<size;n++)
 {
  STexture sTexture=vector_STexture[n];
  //сохраним имя текстуры  
  fwrite(sTexture.Name.c_str(),sizeof(char),strlen(sTexture.Name.c_str())+1,file);  
 }
}
//---------------------------------------------------------------------------
//загрузить анимацию текстуры
//---------------------------------------------------------------------------
void CTextureFollow::Load(FILE *file)
{
 long size;
 fread(&Mode,sizeof(long),1,file);
 fread(&size,sizeof(long),1,file);
 vector_STexture.clear();
 for(long n=0;n<size;n++)
 {
  STexture sTexture;
  while(1)
  {
   char s;
   if (fread(&s,sizeof(char),1,file)<1) break;
   if (s==0) break;
   sTexture.Name+=s;
  }
  sTexture.TextureIndex=0;
  vector_STexture.push_back(sTexture);
 }
 CurrentPointer=0;
 dPointer=1;
}
//---------------------------------------------------------------------------
//обработка анимации текстуры
//---------------------------------------------------------------------------
void CTextureFollow::Processing(void)
{
 //анимация смены текстуры
 if (Mode!=TEXTURE_FOLLOW_MODE_DISABLE)
 {
  CurrentPointer+=dPointer;
  if (Mode==TEXTURE_FOLLOW_MODE_CYCLES)
  {
   if (CurrentPointer>=vector_STexture.size()) CurrentPointer=0;
  }
  if (Mode==TEXTURE_FOLLOW_MODE_ZIGZAG)
  {
   if (CurrentPointer>=vector_STexture.size())
   {
    CurrentPointer=vector_STexture.size()-1;
    dPointer=-dPointer;
   }
   if (CurrentPointer<=0)
   {
    CurrentPointer=0;
    dPointer=-dPointer;
   }
  }
 }
}
//---------------------------------------------------------------------------
//получить текущую текстуру
//---------------------------------------------------------------------------
STexture CTextureFollow::GetCurrentTexture(void)
{
 return(vector_STexture[CurrentPointer]);
}
//---------------------------------------------------------------------------
//перейти к последней текстуре
//---------------------------------------------------------------------------
void CTextureFollow::GoToEndTexture(void)
{
 long size=vector_STexture.size();
 CurrentPointer=size-1;
}
//----------------------------------------------------------------------------------------------------
//получить список используемых текстур
//----------------------------------------------------------------------------------------------------
void CTextureFollow::CreateTextureList(vector<SUseTexture> &vector_SUseTexture)
{
 long size=vector_STexture.size();
 for(long n=0;n<size;n++)
 {
  string name=vector_STexture[n].Name;
  long utsize=vector_SUseTexture.size();
  //ищем такую текстуру в списке
  bool find=false;
  for(long m=0;m<utsize;m++)
  {
   if (vector_SUseTexture[m].Name.compare(name)==0)
   {
    find=true;
	break;
   }
  }
  if (find==true) continue;//такая текстура в списке уже есть
  SUseTexture sUseTexture;
  sUseTexture.Name=name;
  vector_SUseTexture.push_back(sUseTexture);
 }
}
//----------------------------------------------------------------------------------------------------
//заменить индексы текстур в соответствии со списком
//----------------------------------------------------------------------------------------------------
void CTextureFollow::RefreshTextureIndex(vector<SUseTexture> &vector_SUseTexture)
{
 long size=vector_STexture.size();
 long utsize=vector_SUseTexture.size();
 for(long n=0;n<size;n++)
 {
  string name=vector_STexture[n].Name;
  for(long m=0;m<utsize;m++)
  {
   if (vector_SUseTexture[m].Name.compare(name)==0)//нашли текстуру
   {
    vector_STexture[n].TextureIndex=vector_SUseTexture[m].Index;
    break;
   }
  }
 } 
}
