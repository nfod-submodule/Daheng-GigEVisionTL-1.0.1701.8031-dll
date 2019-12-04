//------------------------------------------------------------
/**
\file    DeviceProcess.cpp 
\brief   Device Process 
*/
//------------------------------------------------------------

#include "StdAfx.h"
#include "DeviceProcess.h"

CDeviceProcess::CDeviceProcess(void)
{
	m_pBitmap                   = NULL;
	m_pSampleCaptureEventHandle = NULL;
	m_bIsOpen                   = false;
	m_bIsSnap                   = false;
	m_bShowDeviceFPS            = true;
	m_nDeviceIndex                = 0;
	m_strDeviceSN               = "";
	m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();
}

CDeviceProcess::~CDeviceProcess(void)
{
	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}
}

//------------------------------------------------------------
/**
\brief   Open Device 
\param   strDeviceSN   [in]         �豸���к�
\param   m_pWnd        [in]         ���ھ��
\param   nDeviceIndex  [in]         �豸���
\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::OpenDevice(gxstring strDeviceSN, CWnd* m_pWnd,int nDeviceIndex)
{
	bool bIsDeviceOpen   = false;      ///< �ж��豸�Ƿ��Ѵ򿪱�־
	bool bIsStreamOpen   = false;      ///< �ж��豸���Ƿ��Ѵ򿪱�־
	try
	{
		if (m_bIsOpen || m_pWnd == NULL || strDeviceSN == "")
		{
			throw std::exception("��Ч����!");
		}

		m_strDeviceSN = strDeviceSN;
		m_nDeviceIndex  = nDeviceIndex;

		//���豸
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strDeviceSN,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//��ȡ���Կ�����
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//���òɼ�ģʽΪ�����ɼ�
		m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

		//���ô���ģʽΪ��
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

		//��ȡ�豸������
		int nCount = m_objDevicePtr->GetStreamCount();

		if (nCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw std::exception("δ�����豸��!");
		}

		m_bIsOpen = true;

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CGXBitmap(m_objDevicePtr,m_pWnd);
	}
	catch (CGalaxyException& e)
	{

		//�ж��豸���Ƿ��Ѵ�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//�ж��豸�Ƿ��Ѵ�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;

	}
	catch (std::exception& e)
	{
		//�ж��豸���Ƿ��Ѵ�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//�ж��豸�Ƿ��Ѵ�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;
	}

}
//------------------------------------------------------------
/**
\brief   Close Device 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::CloseDevice()
{
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
		//�ж��Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{

			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ص�����
			m_objStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//�ر�������
		m_objStreamPtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//�ر��豸
		m_objDevicePtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	m_bIsSnap = false;
	m_bIsOpen = false;

	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}
//------------------------------------------------------------
/**
\brief   Start Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StartSnap()
{
	//�ж��豸�Ƿ��Ѵ�
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
		//ע��ص�����
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);
	}
	catch (CGalaxyException& e)
	{
		throw e;
	}

	try
	{

		//��������ɼ�
		m_objStreamPtr->StartGrab();
	}
	catch (CGalaxyException& e)
	{
		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}

	try
	{
		//���Ϳ�������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		m_bIsSnap = true;
	}
	catch (CGalaxyException& e)
	{
		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}
	catch (std::exception& e)
	{
		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}

}

//------------------------------------------------------------
/**
\brief   Stop Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StopSnap()
{
	//�ж��豸�Ƿ��Ѵ�
	if (!m_bIsOpen || !m_bIsSnap)
	{
		return;
	}

	try
	{
		//����ͣ������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();

		//֡����0
		m_objfps.Reset();

		m_bIsSnap = false;
	}
	catch (CGalaxyException& e)
	{
		throw e;

	}
	catch (std::exception& e)
	{
		throw e;

	}
}

//------------------------------------------------------------
/**
\brief   Get Device Open Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsOpen() const
{
	return m_bIsOpen;
}

//------------------------------------------------------------
/**
\brief   Get Device Snap Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsSnap() const
{
	return m_bIsSnap;
}

//------------------------------------------------------------
/**
\brief   Set Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::SetShowFPS(bool bIsShowFps)
{
	m_bShowDeviceFPS = bIsShowFps;
}

//------------------------------------------------------------
/**
\brief   Get Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::GetShowFPS()
{
	return m_bShowDeviceFPS;
}

//------------------------------------------------------------
/**
\brief   Show Image
\param   objImageDataPointer [in]       ͼ����Ϣ

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::__ShowPicture(CImageDataPointer& objImageDataPointer)
{
	static int nCont = 0;
	char strRecordDeviceSNFPS[1024] = {'\0'};
	double dFps = 0;        ///< ֡��

	if (m_bShowDeviceFPS)
	{
		//����֡��
		m_objfps.IncreaseFrameNum();
		nCont++;
		if (nCont >= GET_FRAME_INTERVAL)
		{
			//����֡��
			m_objfps.UpdateFps();
			nCont = 0;
		}
		//��ȡ���һ�ε�֡��
		dFps = m_objfps.GetFps();
		sprintf_s(strRecordDeviceSNFPS,sizeof(strRecordDeviceSNFPS),
			"���:%d SN:%s FPS:%.2f",
			m_nDeviceIndex,
			m_strDeviceSN.c_str(),
			dFps);

		//��ʾͼ��֡��
		m_pBitmap->Show(objImageDataPointer,strRecordDeviceSNFPS);

	}
	else
	{
		//��ʾͼ��
		m_pBitmap->Show(objImageDataPointer);
	}
}

