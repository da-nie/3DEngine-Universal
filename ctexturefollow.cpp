#include "ctexturefollow.h"

//---------------------------------------------------------------------------
//�����������
//---------------------------------------------------------------------------
CTextureFollow::CTextureFollow(void)
{
 CurrentPointer=0;
 dPointer=0;
 Mode=TEXTURE_FOLLOW_MODE_DISABLE;
 vector_STexture.clear();
}
//---------------------------------------------------------------------------
//����������
//---------------------------------------------------------------------------
CTextureFollow::~CTextureFollow(void)
{
 vector_STexture.clear();
}
//---------------------------------------------------------------------------
//��������� �������� ��������
//---------------------------------------------------------------------------
void CTextureFollow::Save(FILE *file)
{
 fwrite(&Mode,sizeof(long),1,file);
 long size=vector_STexture.size();
 fwrite(&size,sizeof(long),1,file);
 for(long n=0;n<size;n++)
 {
  STexture sTexture=vector_STexture[n];
  //�������� ��� ��������  
  fwrite(sTexture.Name.c_str(),sizeof(char),strlen(sTexture.Name.c_str())+1,file);  
 }
}
//---------------------------------------------------------------------------
//��������� �������� ��������
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
//��������� �������� ��������
//---------------------------------------------------------------------------
void CTextureFollow::Processing(void)
{
 //�������� ����� ��������
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
//�������� ������� ��������
//---------------------------------------------------------------------------
STexture CTextureFollow::GetCurrentTexture(void)
{
 return(vector_STexture[CurrentPointer]);
}
//---------------------------------------------------------------------------
//������� � ��������� ��������
//---------------------------------------------------------------------------
void CTextureFollow::GoToEndTexture(void)
{
 long size=vector_STexture.size();
 CurrentPointer=size-1;
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ������������ �������
//----------------------------------------------------------------------------------------------------
void CTextureFollow::CreateTextureList(vector<SUseTexture> &vector_SUseTexture)
{
 long size=vector_STexture.size();
 for(long n=0;n<size;n++)
 {
  string name=vector_STexture[n].Name;
  long utsize=vector_SUseTexture.size();
  //���� ����� �������� � ������
  bool find=false;
  for(long m=0;m<utsize;m++)
  {
   if (vector_SUseTexture[m].Name.compare(name)==0)
   {
    find=true;
	break;
   }
  }
  if (find==true) continue;//����� �������� � ������ ��� ����
  SUseTexture sUseTexture;
  sUseTexture.Name=name;
  vector_SUseTexture.push_back(sUseTexture);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ������� ������� � ������������ �� �������
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
   if (vector_SUseTexture[m].Name.compare(name)==0)//����� ��������
   {
    vector_STexture[n].TextureIndex=vector_SUseTexture[m].Index;
    break;
   }
  }
 } 
}
