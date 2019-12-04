#pragma once
#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "GxImageProcessDlg.h"

class CSampleCaptureEventHandler :public ICaptureEventHandler
{
public:
	CSampleCaptureEventHandler(void);
	~CSampleCaptureEventHandler(void);
	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);

public:
	CCGXBitmapPointer   m_Bmp;                         ///<图像显示及处理类对象
	CGxImageProcessDlg*  pImgProDlg;
};
