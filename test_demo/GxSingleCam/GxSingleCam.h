// GxSingleCam.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGxSingleCamApp:
// �йش����ʵ�֣������ GxSingleCam.cpp
//

class CGxSingleCamApp : public CWinApp
{
public:
	CGxSingleCamApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGxSingleCamApp theApp;