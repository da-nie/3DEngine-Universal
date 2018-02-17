#include "cwnd_main.h"
 
#include <initguid.h>
#include <ddraw.h>

LPDIRECTDRAW lpdd_old=NULL;
LPDIRECTDRAW7 lpdd=NULL;
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsprimary=NULL;
LPDIRECTDRAWSURFACE7 lpddssecondary=NULL;

extern CVideo cVideo;
extern CMouse cMouse;
extern CKeyboard cKeyboard;

//-Функции обработки сообщений класса----------------------------------------
BEGIN_MESSAGE_MAP(CWnd_Main,CWnd)
 ON_WM_PAINT()
 ON_WM_CREATE()
 ON_WM_DESTROY()
 ON_WM_TIMER() 
 ON_WM_ACTIVATE()
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CWnd_Main::CWnd_Main(void)
{
 Active=false;
 VideoPtr=new unsigned long[WINDOW_WIDTH*WINDOW_HEIGHT];
}
//-Деструктор класса---------------------------------------------------------
CWnd_Main::~CWnd_Main()
{
 delete[](VideoPtr);
}
//-Замещённые функции предка-------------------------------------------------
//-Новые функции класса------------------------------------------------------
//-Функции обработки сообщений класса----------------------------------------
afx_msg void CWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 CWnd::OnCreate(lpCreateStruct); 
 cMain.OpenWindow();
 //настраиваем Direct Draw
 Active=false;
 lpdd=NULL;
 lpddsprimary=NULL;
 lpddssecondary=NULL;
 if (FAILED(DirectDrawCreate(NULL,&lpdd_old,NULL)))
 {
  MessageBox("Ошибка инициализации DirectDraw7","Ошибка",MB_OK);
  DestroyWindow();
  return;
 }
 lpdd_old->QueryInterface(IID_IDirectDraw7,(void **)&lpdd);
 lpdd_old->Release();
 SetCursorPos(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
 while (ShowCursor(FALSE)>=0);
 timeBeginPeriod(1);
 SetTimer(ID_TIMER_MAIN,1000/30,NULL);//30 Гц
}
afx_msg void CWnd_Main::OnDestroy(void)
{ 
 timeEndPeriod(1);
 KillTimer(ID_TIMER_MAIN);
 cMain.CloseWindow();
 while (ShowCursor(TRUE)<0);
 if (Active==true)
 {
  if (lpddsprimary!=NULL) lpddsprimary->Release();
  lpddsprimary=NULL;
 }
 CWnd::OnDestroy();
}
afx_msg void CWnd_Main::OnPaint(void)
{
 if (Active==false)
 {
  CWnd::OnPaint();
  return;
 }
 CWnd::OnPaint(); 
 CPaintDC dc(this);

 //если рисовать картинку в обычной памяти, то FPS резко возрастает
 cVideo.SetVideoPointer(VideoPtr,WINDOW_WIDTH);

 LARGE_INTEGER start_time;
 LARGE_INTEGER current_time;
 LARGE_INTEGER CounterFrequency;
 QueryPerformanceFrequency(&CounterFrequency);
 long double d_CounterFrequency=static_cast<long double>(CounterFrequency.QuadPart);
 QueryPerformanceCounter(&start_time);

 cMain.OnActivate_Timer(); 

 QueryPerformanceCounter(&current_time);
 long double delta_time=static_cast<long double>(current_time.QuadPart-start_time.QuadPart);
 unsigned long fps=0;
 if (fabs(delta_time)>0.0001) fps=static_cast<unsigned long>(d_CounterFrequency/delta_time);
 char fps_text[255];
 sprintf(fps_text,"FPS:%i",fps);

 //выводим FPS
 cVideo.PutString(WINDOW_WIDTH-FONT_WIDTH*(strlen(fps_text)+4),0,fps_text,0x00ffffff);

 //копируем буфер с изображением на экран
 lpddssecondary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);
 unsigned long *vptr=VideoPtr;
 unsigned long *ddptr=reinterpret_cast<unsigned long *>(ddsd.lpSurface);
 unsigned long line=ddsd.lPitch>>2;
 for(long n=0;n<WINDOW_HEIGHT;n++,vptr+=WINDOW_WIDTH,ddptr+=line)
 {
  memcpy(ddptr,vptr,WINDOW_WIDTH*sizeof(unsigned long));
 }
 lpddssecondary->Unlock(NULL);
 lpddsprimary->Flip(NULL,DDFLIP_WAIT); 
}


afx_msg void CWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_MAIN) 
 {
  CWnd::OnTimer(nIDEvent);
  return;
 }
 if (GetAsyncKeyState(VK_F1)&32768) cKeyboard.SetKeyState(KEY_S_PORTAL,true);
                               else cKeyboard.SetKeyState(KEY_S_PORTAL,false);
 if (GetAsyncKeyState(VK_F2)&32768) cKeyboard.SetKeyState(KEY_G_PORTAL,true);
                               else cKeyboard.SetKeyState(KEY_G_PORTAL,false);
 if (GetAsyncKeyState(VK_F3)&32768) cKeyboard.SetKeyState(KEY_BSP,true);
                               else cKeyboard.SetKeyState(KEY_BSP,false);

 if (GetAsyncKeyState(VK_F5)&32768) cKeyboard.SetKeyState(KEY_SAVE_POS,true);
                               else cKeyboard.SetKeyState(KEY_SAVE_POS,false);

 if (GetAsyncKeyState(VK_F9)&32768) cKeyboard.SetKeyState(KEY_LOAD_POS,true);
                               else cKeyboard.SetKeyState(KEY_LOAD_POS,false);

 if (GetAsyncKeyState(VK_UP)&32768) cKeyboard.SetKeyState(KEY_UP,true);
                               else cKeyboard.SetKeyState(KEY_UP,false);
 if (GetAsyncKeyState(VK_DOWN)&32768) cKeyboard.SetKeyState(KEY_DOWN,true);
                                 else cKeyboard.SetKeyState(KEY_DOWN,false);
 if (GetAsyncKeyState(VK_LEFT)&32768) cKeyboard.SetKeyState(KEY_LEFT,true);
                                 else cKeyboard.SetKeyState(KEY_LEFT,false);
 if (GetAsyncKeyState(VK_RIGHT)&32768) cKeyboard.SetKeyState(KEY_RIGHT,true);
                                  else cKeyboard.SetKeyState(KEY_RIGHT,false);
 if (GetAsyncKeyState(VK_SPACE)&32768) cKeyboard.SetKeyState(KEY_USE,true);
                                  else cKeyboard.SetKeyState(KEY_USE,false);
 if (GetAsyncKeyState(VK_CONTROL)&32768) cKeyboard.SetKeyState(KEY_SIT,true);
                                else cKeyboard.SetKeyState(KEY_SIT,false);
 if (GetAsyncKeyState(VK_SHIFT)&32768) cKeyboard.SetKeyState(KEY_SPEED,true);
                                else cKeyboard.SetKeyState(KEY_SPEED,false);
 if (GetAsyncKeyState(VK_INSERT)&32768) cKeyboard.SetKeyState(KEY_LOOK_UP,true);
                                   else cKeyboard.SetKeyState(KEY_LOOK_UP,false);
 if (GetAsyncKeyState(VK_DELETE)&32768) cKeyboard.SetKeyState(KEY_LOOK_DOWN,true);
                                   else cKeyboard.SetKeyState(KEY_LOOK_DOWN,false);
 if (GetAsyncKeyState(VK_ESCAPE)&32768) 
 {
  DestroyWindow();
  return;
 }
 if (GetAsyncKeyState(VK_RETURN)&32768)
 {
  SYSTEMTIME time;
  GetSystemTime(&time);
  float time1=(float)(time.wSecond+time.wMinute*60+time.wHour*60*60+time.wMilliseconds/1000.0);
  CPaintDC dc(this);
  cKeyboard.SetKeyState(KEY_LEFT,true);
  lpddssecondary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);
  cVideo.SetVideoPointer(reinterpret_cast<unsigned long *>(ddsd.lpSurface),ddsd.lPitch>>2);
  for(int n=0;n<3600;n++)
  {
   if (Active==false) continue;
   cMain.OnActivate_Timer(); 
   if (GetAsyncKeyState(VK_ESCAPE)&32768) break;
  }
  lpddssecondary->Unlock(NULL);
  lpddsprimary->Flip(NULL,DDFLIP_WAIT); 
  GetSystemTime(&time);
  float time2=(float)(time.wSecond+time.wMinute*60+time.wHour*60*60+time.wMilliseconds/1000.0);
  float fps=(float)(3600.0/(time2-time1));
  FILE *file=fopen("fps.txt","w+b");
  fprintf(file,"Число кадров в секунду:%f",fps);
  fclose(file);
 }

 POINT Cursor;
 GetCursorPos(&Cursor);
 long WindowXCenter;
 long WindowYCenter;
 cVideo.GetWindownCenter(WindowXCenter,WindowYCenter);
 cMouse.SetXOffset(Cursor.x-WindowXCenter);
 cMouse.SetYOffset(Cursor.y-WindowYCenter);
 SetCursorPos(WindowXCenter,WindowYCenter);

 InvalidateRect(NULL,FALSE);
}
afx_msg BOOL CWnd_Main::OnEraseBkgnd(CDC *pDC)
{
 return(TRUE);
}
void CWnd_Main::OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized)
{
 CWnd::OnActivate(nState,pWndOther,bMinimized); 
 if ((nState==WA_ACTIVE || nState==WA_CLICKACTIVE) && Active==false)
 {
  lpdd->SetCooperativeLevel(m_hWnd,DDSCL_FULLSCREEN|DDSCL_ALLOWMODEX|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);
  lpdd->SetDisplayMode(WINDOW_WIDTH,WINDOW_HEIGHT,32,0,0);
  memset(&ddsd,0,sizeof(ddsd));
  ddsd.dwSize=sizeof(DDSURFACEDESC2);
  ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
  ddsd.dwBackBufferCount=1;
  ddsd.ddsCaps.dwCaps=DDSCAPS_COMPLEX|DDSCAPS_FLIP|DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL);
  ddsd.ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;
  lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps,&lpddssecondary);
  Active=true;
 }
 if (nState==WA_INACTIVE && Active==true) 
 {
  if (lpddsprimary!=NULL) lpddsprimary->Release();
  Active=false;
 } 
}


//-Новые функции класса------------------------------------------------------

//-Прочее--------------------------------------------------------------------

