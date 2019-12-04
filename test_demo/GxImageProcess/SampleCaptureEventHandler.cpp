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
	CGXDevicePointer*    pTemp        =(CGXDevicePointer*)pUserParam; //�豸���

	//����ͼ�������ò�������
	CImageProcessConfigPointer objCfg = (*pTemp)->CreateImageProcessConfig();

	objCfg->EnableAccelerate(pImgProDlg->m_bAccelate);                    ///<CPU����
	objCfg->EnableColorCorrention(pImgProDlg->m_bColorCurrect);           ///<��ɫУ��
	objCfg->EnableDenoise(pImgProDlg->m_bImgDenoise);                     ///<ͼ����
	objCfg->EnableSharpen(pImgProDlg->m_bSharpen);                        ///<��
	objCfg->EnableDefectivePixelCorrect(pImgProDlg->m_bBadPointCurrect);  ///<����У��

	objCfg->SetGammaParam(pImgProDlg->m_dGamma);
	objCfg->SetLightnessParam(pImgProDlg->m_nBright);
	objCfg->SetContrastParam(pImgProDlg->m_nContrast);
	objCfg->SetSaturationParam(pImgProDlg->m_nSatura);

	if (pImgProDlg->m_bSharpen)
	{
		objCfg->SetSharpenParam(pImgProDlg->m_dSharpen);
	}


	//��ǰͼ����������
	objImageDataPointer->ImageProcess(objCfg);

}
