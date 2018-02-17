#ifndef CWND_MAIN
#define CWND_MAIN

#include "stdafx.h"

#include "cvideo.h"
#include "cmain.h"

class CWnd_Main:public CWnd
{
 protected:
  //-���������� ������-------------------------------------------------------
  CMain cMain;
  bool Active;//������� �� ���� ����������
  unsigned long *VideoPtr;
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CWnd_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CWnd_Main();
  //-���������� ������-------------------------------------------------------
  //-���������� ������� ������-----------------------------------------------
  //-����� ������� ������----------------------------------------------------
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy(void);
  afx_msg void OnPaint(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  afx_msg void OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized);
  //-����� ������� ������----------------------------------------------------
 protected: 
  void Draw(void);//���������
  //-������------------------------------------------------------------------
};
#endif
