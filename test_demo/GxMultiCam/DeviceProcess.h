//------------------------------------------------------------
/**
\file    DeviceProcess.h 
\brief   Device Process 
*/
//------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "Fps.h"

#define  GET_FRAME_INTERVAL 10            ///< �ۼ�֡����ÿ�ۼ�10֡��ȡһ��֡��
//---------------------------------------------------------------------------------
/**
\brief   �豸������
*/
//----------------------------------------------------------------------------------
class CDeviceProcess
{
public:

	CDeviceProcess(void);
	~CDeviceProcess(void);

	//---------------------------------------------------------------------------------
	/**
	\brief   �û��̳вɼ��¼�������
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler :public ICaptureEventHandler
	{
		//---------------------------------------------------------------------------------
		/**
		\brief   �ɼ��ص�����
		\param   objImageDataPointer      ͼ�������
		\param   pFrame                   �û�����

		\return  ��
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CDeviceProcess* pDeviceProcess = (CDeviceProcess*)pUserParam;

				//��ʾͼ��
				pDeviceProcess->__ShowPicture(objImageDataPointer);
			}
			catch (CGalaxyException)
			{
				//do nothing

			}
			catch (std::exception)
			{
				//do nothing
			}
		}
	};

public:
	/// ���豸
	void OpenDevice(gxstring strDeviceSN, CWnd* m_pWnd,int nDeviceIndex);

	/// �ر��豸
	void CloseDevice();

	/// ��ʼ�ɼ�
	void StartSnap();

	/// ֹͣ�ɼ�
	void StopSnap();

	/// ��ȡ�豸�򿪱�ʶ
	bool IsOpen() const;

	/// ��ȡ�豸�ɼ���ʶ
	bool IsSnap() const;

	/// �����Ƿ���ʾ֡�ʱ�־
	void SetShowFPS(bool bIsShowFps);

	/// ��ȡ�Ƿ���ʾ֡�ʱ�־
	bool GetShowFPS();

	CGXFeatureControlPointer    m_objFeatureControlPtr;           ///< ���Կ�����

private:

	/// ��ʾͼ��
	void __ShowPicture(CImageDataPointer& objImageDataPointer);

	CGXDevicePointer            m_objDevicePtr;                  ///< �豸���
	CGXStreamPointer            m_objStreamPtr;                  ///< �豸������
	CGXBitmap*                  m_pBitmap;                       ///< ��ͼָ��
	CSampleCaptureEventHandler* m_pSampleCaptureEventHandle;     ///< �ص�ָ��
	bool                        m_bIsOpen;                       ///< �豸�Ƿ�򿪱�־
	bool                        m_bIsSnap;                       ///< �豸�Ƿ񿪲ɱ�־
	bool                        m_bShowDeviceFPS;                ///< ��ʾ�豸֡�ʱ�־
	gxstring                    m_strDeviceSN;                   ///< �豸���к�
	CFps                        m_objfps;                        ///< ����֡��
	int							m_nDeviceIndex;                  ///< �豸���

};
