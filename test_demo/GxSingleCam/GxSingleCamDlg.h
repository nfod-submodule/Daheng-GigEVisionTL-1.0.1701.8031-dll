// GxSingleCamDlg.h : ͷ�ļ�
//
#pragma once

#include "GalaxyIncludes.h"
#include "GXBitmap.h"


// CGxSingleCamDlg �Ի���
class CGxSingleCamDlg : public CDialog
{
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
				CGxSingleCamDlg* pSingleCamDlg = (CGxSingleCamDlg*)pUserParam;

				//��ʾͼ��
				pSingleCamDlg->m_pBitmap->Show(objImageDataPointer);

				//�ж��Ƿ���Ҫ����ͼ��
				if (pSingleCamDlg->m_bCheckSaveBmp == TRUE)
				{
					pSingleCamDlg->SavePicture(objImageDataPointer);
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
// ����
public:
	CGxSingleCamDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GXSINGLECAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerActive();
	afx_msg void OnBnClickedCheckSaveBmp();

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboBalanceWhiteAuto();
	afx_msg void OnCbnSelchangeComboBalanceWhiteSelect();
	afx_msg void OnBnClickedBtnSofttrigger();
	afx_msg void OnEnKillfocusEditShutter();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnEnKillfocusEditBalanceWhiteSerise();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_dEditShutterValue;
	double m_dEditGainValue;
	double m_dEditBalanceRatioValue;
	BOOL   m_bCheckSaveBmp;

public:

	/// ����ͼ��ΪBMP��ʽ
	void SavePicture(CImageDataPointer& objImageDataPointer);

	CGXBitmap*  m_pBitmap;                  ///< ����ͼ��ָ��

private:

	/// ��ʼ���豸����
	void __InitParam();

	/// ��ʼ����Ͽ�
	int __InitComBox(gxstring strName, CComboBox* pComboBox, bool bIsImplemented);

	/// ��ʼ���ع�ʱ��ؼ�
	void __InitShutterTime();

	/// ��ʼ������ؼ�
	void __InitGain();

	/// ��ʼ���Զ���ƽ��ϵ���ؼ�
	void __InitBalanceWhiteRatio();

	/// ��ʼ������
	void __InitUI();

	/// ���½���
	void __UpdateUI();	

	/// ComboBox���ù��ܵ��豸
	void __SetEnumToDevice(gxstring strEnumName,CComboBox* pCombo);

	CGXDevicePointer                  m_objDevicePtr;             ///< �豸���
	CGXStreamPointer                  m_objStreamPtr;             ///< �豸��
	CGXFeatureControlPointer          m_objFeatureControlPtr;     ///< ���Կ�����
	CWnd*                             m_pWnd;                     ///< ����ָ��
	CSampleCaptureEventHandler*       m_pSampleCaptureEventHandle;///< �ص�����ָ��

	bool                              m_bIsOpen;                  ///< �豸�򿪱�ʶ
	bool                              m_bIsSnap;                  ///< �豸�ɼ���ʶ
	bool                              m_bColorFilter;             ///< �Ƿ�֧�ֲ�ɫ���
	bool                              m_bTriggerMode;             ///< �Ƿ�֧�ִ���ģʽ
	bool                              m_bTriggerSource;           ///< �Ƿ�֧�ִ���Դ
	bool                              m_bTriggerActive;           ///< �Ƿ�֧�ִ�������
	bool                              m_bBalanceWhiteAuto;        ///< �Ƿ�֧���Զ���ƽ��
	bool                              m_bBalanceWhiteRatioSelect; ///< �Ƿ�֧�ְ�ƽ��ͨ��ѡ��
	double                            m_dShutterValueMax;         ///< �ع�ʱ�����ֵ      
	double                            m_dShutterValueMin;         ///< �ع�ʱ����Сֵ     
	double                            m_dGainValueMax;            ///< �������ֵ
	double                            m_dGainValueMin;            ///< ������Сֵ
	double                            m_dBalanceWhiteRatioMax;    ///< �Զ���ƽ��ϵ�����ֵ
	double                            m_dBalanceWhiteRatioMin;    ///< �Զ���ƽ��ϵ����Сֵ
	int                               m_nTriggerModeOld;          ///< ��¼����ģʽ
	int                               m_nTriggerSourceOld;        ///< ��¼����Դ
	int                               m_nTriggerActiveOld;        ///< ��¼��������
	int                               m_nBalanceWhiteAutoOld;     ///< ��¼�Զ���ƽ��
	int                               m_nBanlanceWhiteRatioOld;   ///< ��¼�Զ���ƽ��ϵ��
	gxstring                          m_strBalanceWhiteAutoMode;  ///< ��¼�Զ���ƽ�ⷽʽ
	std::string                       m_strSavePath;              ///< ͼ�񱣴�·��
};
