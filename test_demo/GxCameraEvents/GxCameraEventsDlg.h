//----------------------------------------------------------------------------------
/**
\brief  CGxCameraEventsDlg class declaration
*/
//----------------------------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "StopWatch.h"

// CGxCameraEventsDlg �Ի���
class CGxCameraEventsDlg : public CDialog
{
public:

	/// ��׼���캯��
	CGxCameraEventsDlg(CWnd* pParent = NULL);	

	/// �Ի�������
	enum { IDD = IDD_GXCAMERAEVENTS_DIALOG };

protected:

	/// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	HICON m_hIcon;

	/// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOpenDevice();
	afx_msg void OnBnClickedStartSnap();
	afx_msg void OnBnClickedStopSnap();
	afx_msg void OnBnClickedCloseDevice();
	afx_msg void OnBnClickedSendSoftCommand();
	afx_msg void OnClose();

public:
	/// ��ӡʱ����Ϣ���༭����
	void PrintInfotoWnd(std::string &strInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStopWatch                      m_objImageTime;               ///< �յ�ͼ���ʱ��
	CStopWatch                      m_objShutterTime;             ///< �ع�����¼�ʱ��

	//---------------------------------------------------------------------------------
	/**
	\brief    �û�����ɼ��ص���
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandle :public ICaptureEventHandler
	{
	public:

		//---------------------------------------------------------------------------------
		/**
		\brief    �ɼ��ص�����
		\param    objImageDataPointer		�ɼ���Ϣ����
		\param	  pUserParam                �û�˽�в���

		\return   void
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CGxCameraEventsDlg* pDlg      = (CGxCameraEventsDlg*)pUserParam;
				double              dSnaptime = 0;
				char                chTemp[8] = "ms";

				//��ȡ�ɼ�ͼ��ʱ��,���ڼ�ʱ���������⣬����֡���ڹ�С�������ʱ���������
				dSnaptime = pDlg->m_objImageTime.Stop();
				CString strInformation = "";
				strInformation.Format("��������������ɹ�����ͼ����ʱ: %0.2f%s",dSnaptime,chTemp);
				std::string strPrintInformation = strInformation.GetBuffer(0);
				pDlg->PrintInfotoWnd(strPrintInformation);		
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

	//---------------------------------------------------------------------------------
	/**
	\brief    �û������ع�����¼��ص���
	*/
	//----------------------------------------------------------------------------------
	class CSampleFeatureEventHandle :public IFeatureEventHandler
	{
	public:

		//---------------------------------------------------------------------------------
		/**
		\brief    �ع��¼��ص�����
		\param    strFeatureName		�¼�����
		\param	  pUserParam            �û�˽�в���

		\return   void
		*/
		//----------------------------------------------------------------------------------
		void DoOnFeatureEvent(const GxIAPICPP::gxstring& strFeatureName, void* pUserParam)
		{
			try
			{
				CGxCameraEventsDlg*  pDlg             = (CGxCameraEventsDlg*)pUserParam;
				double              dElapsedtime      = 0;
				char                chTemp[8]         = "ms";
				if (strFeatureName == "EventExposureEnd")
				{
					//��ȡ�ع�����¼�ʱ��,���ڼ�ʱ���������⣬����֡���ڹ�С�������ʱ���������
					dElapsedtime = pDlg->m_objShutterTime.Stop();

					//��¼�ع����ʱ��
					CString strInformation = "";
					strInformation.Format("��������������ع����ʱ����ʱ: %0.2f%s",dElapsedtime,chTemp);
					std::string strPrintInformation = strInformation.GetBuffer(0);
					pDlg->PrintInfotoWnd(strPrintInformation);
				}
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

private:

	/// �ж�����Ƿ�֧���ع�����¼�
	bool __IsSupportExposureEnd();

	/// ��ʼ���������
	void __InitParam();

	/// ���½���
	void __UpdateUI();

	CGXDevicePointer                m_objDevicePtr;               ///< �豸���
	CGXStreamPointer                m_objStreamPtr;               ///< ������
	CGXFeatureControlPointer        m_objFeatureControlPtr;       ///< �豸����������
	GX_FEATURE_CALLBACK_HANDLE      m_hCB;                        ///< �ع�����¼��ص����
	void*                           m_pLogWnd;                    ///< ָ��ʱ���¼����
	CSampleCaptureEventHandle*      m_pCaptureEventHandle;        ///< �ɼ��ص�ָ��
	CSampleFeatureEventHandle*      m_pFeatureEventHandle;        ///< �¼��ص�ָ��
	bool                            m_bIsOpen;                    ///< �豸�Ƿ�򿪱�ʶ
	bool                            m_bIsSnap;                    ///< �Ƿ�ɼ���ʶ
};