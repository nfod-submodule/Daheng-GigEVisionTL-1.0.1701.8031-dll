// GxCameraEvents.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGxCameraEventsApp:
// �йش����ʵ�֣������ GxCameraEvents.cpp
//

class CGxCameraEventsApp : public CWinApp
{
public:
	CGxCameraEventsApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGxCameraEventsApp theApp;