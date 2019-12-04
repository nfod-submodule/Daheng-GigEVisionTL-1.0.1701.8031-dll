//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.h 
\brief   CGxGigeRecovery class declaration
*/
//----------------------------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "conio.h"
#include <iostream>


class CSampleDeviceOfflineEventHandle;              ///< ���������¼�������

//--------------------------------------------------
/**
\brief     �豸������
*/
//--------------------------------------------------
class CGigeRecovery
{
public:

	/// ���캯��
	CGigeRecovery();
 
	/// ��������
	virtual ~CGigeRecovery();

	/// ������������ӿں���
	void GigeRecovery();

	bool m_bIsOffline;               ///< �豸���߱�ʶ

private:

	/// ���豸
	bool __OnOpenDevice() throw();

	/// ��ʼ�ɼ�
	bool __OnStartSnap() throw();

	/// ֹͣ�ɼ�
	bool __OnStopSnap() throw();

	/// �ر��豸
	bool __OnCloseDevice() throw();

	/// ��ʼ���豸����
	void __InitParam();

	/// �����ɼ�
	void __Acquisition() throw();

	/// �豸���ߴ���
	void __ProcessOffline() throw();

	/// �豸����
	void __Recovery() throw();

	CGXDevicePointer                   m_objDevicePtr;                       ///< �豸���
	CGXStreamPointer                   m_objStreamPtr;                       ///< ������
	CGXFeatureControlPointer           m_objFeatureControlPtr;               ///< ���Կ���������
	GX_DEVICE_OFFLINE_CALLBACK_HANDLE  m_hCB;                                ///< ���߻ص����
	CImageDataPointer                  m_objImageDataPtr;                    ///< GetImage��ȡ��ͼ��

	bool                               m_bIsOpen;                            ///< �豸�򿪱�ʶ
	bool                               m_bIsSnap;                            ///< �豸�ɼ���ʶ  
	GxIAPICPP::gxstring                m_strSavePath;                        ///< �豸���ò����ļ�����·��
	CSampleDeviceOfflineEventHandle*   m_pDeviceOfflineEventHandle;          ///< �����¼�ָ��
	gxstring                           m_strMACAddress;                      ///< ���MAC��ַ
};

//--------------------------------------------------
/**
\brief     �û��̳е��ߴ�����
*/
//--------------------------------------------------
class CSampleDeviceOfflineEventHandle :public IDeviceOfflineEventHandler
{
public:
	//--------------------------------------------------
	/**
	\brief     ���߻ص�����
    \param     pUserParam      �û�����
    
	\return    void
	*/
	//--------------------------------------------------
	void DoOnDeviceOfflineEvent(void* pUserParam)
	{
		try
		{
			CGigeRecovery* pGigeRecovery = (CGigeRecovery*)pUserParam;
			pGigeRecovery->m_bIsOffline = true;
			std::cout<<"**********************Device offline**********************"<<std::endl;

		}
		catch (CGalaxyException)
		{
			//do nothing
		}
		catch (std::exception)
		{
			//do noting
		}
	}
};