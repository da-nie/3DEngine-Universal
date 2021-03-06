#include "csimplybridge.h"

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CSimplyBridge::CSimplyBridge(CISector *cISector_Set_Ptr):CDecorator_CISector(cISector_Set_Ptr)
{
}

//----------------------------------------------------------------------------------------------------
//�������������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Initialize(void)
{
 cISector_Ptr->Initialize();
 OldDown=cISector_Ptr->GetDown();
 Dz=0;
 Locked=false;
}
//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Processing(void)
{
 cISector_Ptr->Processing();
 cISector_Ptr->SetDown(cISector_Ptr->GetDown()+Dz);
 if (OldDown<BasedDown)//������ ����������� ��� ���������
 {
  if (cISector_Ptr->GetDown()>=BasedDown)//������ ������������ � �������� ���������
  {
   cISector_Ptr->SetDown(BasedDown);
   Dz=0;
  }
 }

 if (OldDown>BasedDown)//������ ���������� ��� ���������
 {
  if (cISector_Ptr->GetDown()<=BasedDown)//������ ������������ � �������� ���������
  {
   cISector_Ptr->SetDown(BasedDown);
   Dz=0;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Activate(void)
{
 if (Locked==true) return;//������ ������������
 if (OldDown<BasedDown) Dz=4;//������ ����������� ��� ���������
 if (OldDown>BasedDown) Dz=-4;//������ ���������� ��� ���������
}
//----------------------------------------------------------------------------------------------------
//�������������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Lock(void)
{
 Locked=true;
}
//----------------------------------------------------------------------------------------------------
//��������������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Unlock(void)
{
 Locked=false;
}
//----------------------------------------------------------------------------------------------------
//��������
//----------------------------------------------------------------------------------------------------
void CSimplyBridge::Load(FILE *file)
{
 cISector_Ptr->Load(file);
 BasedDown=LoadLong(file);
}
 