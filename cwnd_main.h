#ifndef CWND_MAIN
#define CWND_MAIN

#include "stdafx.h"

#include "cvideo.h"
#include "cmain.h"

class CWnd_Main:public CWnd
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CMain cMain;
  bool Active;//активно ли окно приложения
  unsigned long *VideoPtr;
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CWnd_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CWnd_Main();
  //-Переменные класса-------------------------------------------------------
  //-Замещённые функции предка-----------------------------------------------
  //-Новые функции класса----------------------------------------------------
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy(void);
  afx_msg void OnPaint(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  afx_msg void OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized);
  //-Новые функции класса----------------------------------------------------
 protected: 
  void Draw(void);//отрисовка
  //-Прочее------------------------------------------------------------------
};
#endif
