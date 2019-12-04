//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.cpp 
\brief   CGxGigeRecovery class declaration implementation
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "stdio.h"
#include "GxGigeRecovery.h"

using namespace std;

//---------------------------------------------------------------------------------
/**
\brief   ���캯��

\return   ��
*/
//----------------------------------------------------------------------------------
CGigeRecovery::CGigeRecovery()
{
	try
	{
		m_hCB              = NULL;
		m_strSavePath      = "";
		m_strMACAddress    = "";
		m_bIsOffline       = false;
		m_bIsOpen          = false;
		m_bIsSnap          = false;
		m_pDeviceOfflineEventHandle = new CSampleDeviceOfflineEventHandle();

		//��ʼ���豸��
		IGXFactory::GetInstance().Init();

		//��ȡ��ǰ·�������ڱ����豸���ò���
		char chModulePath[512] = {0};
		string strPath       = "";
		::GetModuleFileName(NULL,(LPTSTR)chModulePath,512);
		strPath = chModulePath;
		strPath = strPath.substr(0,strPath.rfind('\\')+1);
        strPath = strPath + "ConfigFile.ini";
		m_strSavePath  = strPath.c_str();
	}
	catch (CGalaxyException &e)
	{
		if (m_pDeviceOfflineEventHandle != NULL)
		{
			delete m_pDeviceOfflineEventHandle;
		}
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;		
	}
	catch (std::exception &e)
	{
		if (m_pDeviceOfflineEventHandle != NULL)
		{
			delete m_pDeviceOfflineEventHandle;
		}
		cout<<"<"<<e.what()<<">"<<endl;		
	}

}

//---------------------------------------------------------------------------------
/**
\brief   ��������

\return   ��
*/
//----------------------------------------------------------------------------------
CGigeRecovery::~CGigeRecovery()
{
	try
	{
		//�ر��豸��
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;			
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;			
	}

	if (m_pDeviceOfflineEventHandle != NULL)
	{
		delete m_pDeviceOfflineEventHandle;
		m_pDeviceOfflineEventHandle = NULL;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   ���豸����

\return   true�����豸�ɹ���false�����豸ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnOpenDevice() throw()
{
	bool bIsDeviceOpen = false;              ///< �豸�Ƿ��Ѵ򿪱�ʶ
	bool bIsStreamOpen = false;              ///< �豸���Ƿ��Ѵ򿪱�ʶ

	try
	{
		cout<<"====================CGigeRecovery::__OnOpenDevice()===================="<<endl;

		//����ö���豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);

		//�жϵ�ǰ�豸���Ӹ���
		if (vectorDeviceInfo.size() <= 0)
		{
			cout<<"No Device!"<<endl;
			return m_bIsOpen;
		}

		//�ж�ö�ٵ����豸�Ƿ�ΪGige
		if (vectorDeviceInfo[0].GetDeviceClass() != GX_DEVICE_CLASS_GEV)
		{
			cout<<"The device is not GEV"<<endl;
			return m_bIsOpen;
		}        	
        
		//ͨ��MAC��ַ���豸
		m_strMACAddress = vectorDeviceInfo[0].GetMAC();	
		cout<<"<Open device by MAC:"<<m_strMACAddress<<endl;
		m_objDevicePtr          = IGXFactory::GetInstance().OpenDeviceByMAC(m_strMACAddress,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr  = m_objDevicePtr->GetRemoteFeatureControl();
		int32_t nStreamCount    = m_objDevicePtr->GetStreamCount();

		//�ж�������
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("Not Find Device Stream");
		}

		//��ʼ���豸����
		cout<<"<Initialize the device parameters>"<<endl;
		__InitParam();

		//�������ò������ļ�
		cout<<"<Export config file>"<<endl;
		m_objDevicePtr->ExportConfigFile(m_strSavePath);

		//ע����߻ص�����
		cout<<"<Register device Offline callback>"<<endl;
		m_hCB = m_objDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);
		m_bIsOpen = true;
		return m_bIsOpen;

	}
	catch (CGalaxyException &e)
	{
		//�ж��豸���Ƿ��ѹر�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//�ж��豸�Ƿ��ѹر�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
	catch (std::exception &e)
	{
		//�ж��豸���Ƿ��ѹر�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//�ж��豸�Ƿ��ѹر�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����������

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__InitParam()
{
	//���òɼ�ģʽΪ����
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//���ô���ģʽΪ��
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

	//����������ʱʱ��Ϊ1s
	//���ǧ���������������Debugģʽ�µ�������ʱ�������������ʱʱ���Զ�����Ϊ5min��
	//��������Ϊ�˲��������������ʱӰ�����ĵ��Ժ͵���ִ�У�ͬʱ��Ҳ��ζ���������5min���޷��Ͽ�������ʹ����ϵ����ϵ�
	//Ϊ�˽�������������⣬�������������ʱʱ������Ϊ1s�����������ߺ������������
	m_objFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(1000);

}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ�ɼ�

\return   true���ɼ��ɹ���false���ɼ�ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStartSnap() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnStartSnap()===================="<<endl;

		//��������ͨ��
		m_objStreamPtr->StartGrab();

		//���Ϳ�������
		cout<<"<Send start snap command to device>"<<endl;
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;
		return m_bIsSnap;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}

}

//---------------------------------------------------------------------------------
/**
\brief   ֹͣ�ɼ�

\return   true��ֹͣ�ɼ��ɹ���false��ֹͣ�ɼ�ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStopSnap() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnStopSnap()===================="<<endl;

		//����ͣ������
		cout<<"<Send stop snap command to device>"<<endl;
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ͨ��
		m_objStreamPtr->StopGrab();
		m_bIsSnap = false;
		return m_bIsSnap;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �ر��豸

\return   true���ر��豸�ɹ���false���ر��豸ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnCloseDevice() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnCloseDevice()===================="<<endl;

		//ע���ص�����
		cout<<"<Unregister device Offline callback>"<<endl;
		m_objDevicePtr->UnregisterDeviceOfflineCallback(m_hCB);
		m_objStreamPtr->Close();

		//�ر��豸
		cout<<"<Close Device>"<<endl;
		m_objDevicePtr->Close();
		m_bIsOpen = false;
		return m_bIsOpen;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �����ɼ�

\return   ��
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Acquisition() throw()
{
	cout<<"====================CGigeRecovery::__Acquisition()===================="<<endl;;
	cout<<"<Press any key to stop acquisition>"<<endl;

	//�ް�������
	while(!_kbhit())
	{
		try
		{
			//�ж��豸�Ƿ����
			if (m_bIsOffline)
			{
				//���ߴ��������
				__ProcessOffline();
				__Recovery();
			}
			else
			{
				m_objImageDataPtr = m_objStreamPtr->GetImage(500);
				printf("Successfully get Image\n");
			}
		}
		catch(CGalaxyException &e)
		{
			cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		}
		catch(std::exception &e)
		{
			cout<<"<"<<e.what()<<">"<<endl;
		}

	}
	_getch();
}

//---------------------------------------------------------------------------------
/**
\brief   �豸���ߴ���

\return   ��
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__ProcessOffline() throw()
{
	try
	{
		printf("**********************Process Offline**********************\r");
		//�ж��豸�Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			cout<<"\n<Send stop snap command to device>"<<endl;
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
		}
	}
	catch(CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
	}
	catch(std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
	}

	try
	{
		//�ж��豸�Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			m_objStreamPtr->StopGrab();
			m_bIsSnap = false;
		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
	catch(std::exception)
	{
		//do noting
	}

	try
	{
		//�ж��豸�Ƿ��
		if (m_bIsOpen)
		{
			//ע�����߻ص�����
			cout<<"<Unregister device Offline callback>"<<endl;
			m_objDevicePtr->UnregisterDeviceOfflineCallback(m_hCB);

			//�ر������豸
			cout<<"<Close Device>"<<endl;
			m_objStreamPtr->Close();
			m_objDevicePtr->Close();
			m_bIsOpen = false;

		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
	catch(std::exception)
	{
		//do noting
	}

}

//---------------------------------------------------------------------------------
/**
\brief   �豸����

\return   ��
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Recovery() throw()
{
	bool bIsDeviceOpen = false;              ///< �豸�Ƿ��Ѵ򿪱�ʶ
	bool bIsStreamOpen = false;              ///< �豸���Ƿ��Ѵ򿪱�ʶ
	try
	{
        printf("**********************Recovery**********************\r");
		//����ö���豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);
		//�жϵ�ǰ�豸���Ӹ���
		if (vectorDeviceInfo.size() <= 0)
		{
			return;
		}

		//���豸
		cout<<"\n<Open device by MAC:"<<m_strMACAddress<<endl;
		m_objDevicePtr          = IGXFactory::GetInstance().OpenDeviceByMAC(m_strMACAddress,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr  = m_objDevicePtr->GetRemoteFeatureControl();

		//�ж�������
		int32_t nStreamCount    = m_objDevicePtr->GetStreamCount();
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("δ�����豸��!");
		}

		//���������ļ�
		cout<<"<Import config file>"<<endl;
		m_objDevicePtr->ImportConfigFile(m_strSavePath);

		//ע����߻ص�����
		cout<<"<Register device Offline callback>"<<endl;
		m_hCB = m_objDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);
		m_bIsOpen = true;

		//���Ϳ�������
		cout<<"<Send start snap command to device>"<<endl;
		m_objStreamPtr->StartGrab();
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap    = true;
		m_bIsOffline = false;
	}
	catch(CGalaxyException &e)
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

		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
	}
	catch(std::exception &e)
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

		cout<<"<"<<e.what()<<">"<<endl;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �豸������������ӿں���

\return   ��
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::GigeRecovery()
{
	bool bReturn = false;
	try
	{
		//���豸
		bReturn = __OnOpenDevice();
		if (!bReturn)
		{
			return;
		}

		//��ʼ�ɼ�
		bReturn = __OnStartSnap();
		if (!bReturn)
		{
			//�ر��豸
			__OnCloseDevice();
			return;
		}

		//ѭ���ɼ�
		__Acquisition();

		//ֹͣ�ɼ�
		__OnStopSnap();

		//�ر��豸
		__OnCloseDevice();

	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
	}

}

