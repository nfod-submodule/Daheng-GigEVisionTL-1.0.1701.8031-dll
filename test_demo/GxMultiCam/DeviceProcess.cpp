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
\param   strDeviceSN   [in]         设备序列号
\param   m_pWnd        [in]         窗口句柄
\param   nDeviceIndex  [in]         设备序号
\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::OpenDevice(gxstring strDeviceSN, CWnd* m_pWnd,int nDeviceIndex)
{
	bool bIsDeviceOpen   = false;      ///< 判断设备是否已打开标志
	bool bIsStreamOpen   = false;      ///< 判断设备流是否已打开标志
	try
	{
		if (m_bIsOpen || m_pWnd == NULL || strDeviceSN == "")
		{
			throw std::exception("无效参数!");
		}

		m_strDeviceSN = strDeviceSN;
		m_nDeviceIndex  = nDeviceIndex;

		//打开设备
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strDeviceSN,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//获取属性控制器
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//设置采集模式为连续采集
		m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

		//设置触发模式为关
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

		//获取设备流个数
		int nCount = m_objDevicePtr->GetStreamCount();

		if (nCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw std::exception("未发现设备流!");
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

		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//判断设备是否已打开
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
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//判断设备是否已打开
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
		//判断是否停止采集
		if (m_bIsSnap)
		{

			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销回调函数
			m_objStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//关闭流对象
		m_objStreamPtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//关闭设备
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
	//判断设备是否已打开
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
		//注册回调函数
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);
	}
	catch (CGalaxyException& e)
	{
		throw e;
	}

	try
	{

		//开启流层采集
		m_objStreamPtr->StartGrab();
	}
	catch (CGalaxyException& e)
	{
		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}

	try
	{
		//发送开采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		m_bIsSnap = true;
	}
	catch (CGalaxyException& e)
	{
		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}
	catch (std::exception& e)
	{
		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
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
	//判断设备是否已打开
	if (!m_bIsOpen || !m_bIsSnap)
	{
		return;
	}

	try
	{
		//发送停采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();

		//帧率置0
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
\param   objImageDataPointer [in]       图像信息

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::__ShowPicture(CImageDataPointer& objImageDataPointer)
{
	static int nCont = 0;
	char strRecordDeviceSNFPS[1024] = {'\0'};
	double dFps = 0;        ///< 帧率

	if (m_bShowDeviceFPS)
	{
		//增加帧数
		m_objfps.IncreaseFrameNum();
		nCont++;
		if (nCont >= GET_FRAME_INTERVAL)
		{
			//更新帧率
			m_objfps.UpdateFps();
			nCont = 0;
		}
		//获取最近一次的帧率
		dFps = m_objfps.GetFps();
		sprintf_s(strRecordDeviceSNFPS,sizeof(strRecordDeviceSNFPS),
			"序号:%d SN:%s FPS:%.2f",
			m_nDeviceIndex,
			m_strDeviceSN.c_str(),
			dFps);

		//显示图像及帧率
		m_pBitmap->Show(objImageDataPointer,strRecordDeviceSNFPS);

	}
	else
	{
		//显示图像
		m_pBitmap->Show(objImageDataPointer);
	}
}

