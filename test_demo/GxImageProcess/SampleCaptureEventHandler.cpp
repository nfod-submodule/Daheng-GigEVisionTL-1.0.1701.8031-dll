#include "StdAfx.h"
#include "SampleCaptureEventHandler.h"

CSampleCaptureEventHandler::CSampleCaptureEventHandler(void)
{
	pImgProDlg =NULL;
}

CSampleCaptureEventHandler::~CSampleCaptureEventHandler(void)
{
}

void CSampleCaptureEventHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	//CGxImageProcessDlg*  pImgProDlg   =(CGxImageProcessDlg*)pUserParam;
	CGXDevicePointer*    pTemp        =(CGXDevicePointer*)pUserParam; //设备句柄

	//创建图像处理配置参数对象
	CImageProcessConfigPointer objCfg = (*pTemp)->CreateImageProcessConfig();

	objCfg->EnableAccelerate(pImgProDlg->m_bAccelate);                    ///<CPU加速
	objCfg->EnableColorCorrention(pImgProDlg->m_bColorCurrect);           ///<颜色校正
	objCfg->EnableDenoise(pImgProDlg->m_bImgDenoise);                     ///<图像降噪
	objCfg->EnableSharpen(pImgProDlg->m_bSharpen);                        ///<锐化
	objCfg->EnableDefectivePixelCorrect(pImgProDlg->m_bBadPointCurrect);  ///<坏点校正

	objCfg->SetGammaParam(pImgProDlg->m_dGamma);
	objCfg->SetLightnessParam(pImgProDlg->m_nBright);
	objCfg->SetContrastParam(pImgProDlg->m_nContrast);
	objCfg->SetSaturationParam(pImgProDlg->m_nSatura);

	if (pImgProDlg->m_bSharpen)
	{
		objCfg->SetSharpenParam(pImgProDlg->m_dSharpen);
	}


	//当前图像质量提升
	objImageDataPointer->ImageProcess(objCfg);

}
