//----------------------------------------------------------------------------------
/**
\file    Main.cpp 
\brief   Gige recovery function
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "GxGigeRecovery.h"
int _tmain(int argc, _TCHAR* argv[])
{
	//����CGigeRecovery��ʵ���豸������������
	CGigeRecovery objCtrDevice;
	objCtrDevice.GigeRecovery();

	//����������رտ���̨
	std::cout<<"\n<press any key to exit>"<<std::endl;
	while (!_kbhit());

	return 0;
}