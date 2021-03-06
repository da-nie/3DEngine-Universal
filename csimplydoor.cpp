#include "csimplydoor.h"

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CSimplyDoor::CSimplyDoor(CISector *cISector_Set_Ptr):CDecorator_CISector(cISector_Set_Ptr)
{
}
//----------------------------------------------------------------------------------------------------
//инициализация
//----------------------------------------------------------------------------------------------------
void CSimplyDoor::Initialize(void)
{
 cISector_Ptr->Initialize();
 Time=0;
 OldDown=cISector_Ptr->GetDown();
 cISector_Ptr->SetDown(cISector_Ptr->GetUp());
 Dz=0;
 Locked=false;
}
//----------------------------------------------------------------------------------------------------
//обработка
//----------------------------------------------------------------------------------------------------
void CSimplyDoor::Processing(void)
{
 cISector_Ptr->Processing();
 if (Time!=0) Time--;
 if (Time==0) cISector_Ptr->SetDown(cISector_Ptr->GetDown()+Dz);
 if (cISector_Ptr->GetDown()>=cISector_Ptr->GetUp() && Time==0)
 {
  cISector_Ptr->SetDown(cISector_Ptr->GetUp());
  Time=0;
  Dz=0;
 }
 if (cISector_Ptr->GetDown()<=OldDown && Time==0)
 {
  cISector_Ptr->SetDown(OldDown);
  Time=200;
  Dz=-Dz;
 }
}
//----------------------------------------------------------------------------------------------------
//активация
//----------------------------------------------------------------------------------------------------
void CSimplyDoor::Activate(void)
{ 
 cISector_Ptr->Activate();
 if (Locked==true) return;//дверь заблокирована
 Dz=-4;
}
//----------------------------------------------------------------------------------------------------
//заблокировать
//----------------------------------------------------------------------------------------------------
void CSimplyDoor::Lock(void)
{
 cISector_Ptr->Lock();
 Locked=true;
}
//----------------------------------------------------------------------------------------------------
//разблокировать
//----------------------------------------------------------------------------------------------------
void CSimplyDoor::Unlock(void)
{
 cISector_Ptr->Unlock();
 Locked=false;
}
 