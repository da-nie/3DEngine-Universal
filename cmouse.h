#ifndef CMOUSE_H
#define CMOUSE_H

//����� ������ � ������
class CMouse
{
 protected:
  bool LeftKeyState;//����� ������� ����
  bool RightKeyState;//������ ������� ����
  bool CenterKeyState;//����������� ������� ����
  long dX;//��������� ��������� ����
  long dY;
 public:
  CMouse();//�����������
  ~CMouse();//����������
  bool GetLeftKeyState(void);//�������� ��������� ����� �������
  bool GetRightKeyState(void);//�������� ��������� ������ �������
  bool GetCenterKeyState(void);//�������� ��������� ����������� �������

  void SetLeftKeyState(bool state);//������ ��������� ����� �������
  void SetRightKeyState(bool state);//������ ��������� ������ �������
  void SetCenterKeyState(bool state);//������ ��������� ����������� �������

  long GetXOffset(void);//�������� �������� �� X
  long GetYOffset(void);//�������� �������� �� Y

  void SetXOffset(long dx);//������ �������� �� X
  void SetYOffset(long dy);//������ �������� �� Y
 protected:
};

#endif
