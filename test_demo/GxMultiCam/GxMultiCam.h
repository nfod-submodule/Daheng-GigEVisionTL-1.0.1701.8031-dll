// GxMuitCam.h : GxMuitCam Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CGxMuitCamApp:
// �йش����ʵ�֣������ GxMuitCam.cpp
//

class CGxMuitCamApp : public CWinApp
{
public:
	CGxMuitCamApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGxMuitCamApp theApp;