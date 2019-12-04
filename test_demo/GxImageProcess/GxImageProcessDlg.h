// GxImageProcessDlg.h : header file

#pragma once
#include "afxcmn.h"
#include "GalaxyIncludes.h"
#include "GXBitmap.h"

#define PARAM_TRANSFORMATION_VALUE  10.0 ///< ����UI�л������ؼ�ֻ��������ֵ��Gammaֵ����ֵ��С����
                                         ///< ������������Ҫ�ڻ�ȡ�豸Gamma����ֵ����ת��Ϊ����ֵ

// CGxImageProcessDlg dialog
class CGxImageProcessDlg : public CDialog
{
public:
	//---------------------------------------------------------------------------------
	/**
	\brief   �û��̳вɼ��¼�������
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler : public ICaptureEventHandler
	{
	public:
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
				CGxImageProcessDlg*  pImgProDlg = (CGxImageProcessDlg*)pUserParam;
				pImgProDlg->ShowImageProcess(objImageDataPointer);
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

// Construction
public:
	CGxImageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GXIMAGEPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOpenDevice();
	afx_msg void OnBnClickedCloseDevice();
	afx_msg void OnBnClickedStartSnap();
	afx_msg void OnBnClickedStopSnap();
	afx_msg void OnCbnSelchangeComboAwb();
	afx_msg void OnCbnSelchangeComboAwbLight();
	afx_msg void OnBnClickedAidcOpenAccelerateccelerateCheck();
	afx_msg void OnBnClickedOpenBadPixelCorrect();
	afx_msg void OnBnClickedOpenDenoise();
	afx_msg void OnBnClickedOpenColorCorrect();
	afx_msg void OnBnClickedOpenSharpen();
	afx_msg void OnCbnDropdownComboAwb();
	afx_msg void OnCbnDropdownComboAwbLight();
	CSliderCtrl m_sliderSharpen;
	CSliderCtrl m_sliderGamma;
	CSliderCtrl m_sliderContrast;
	CSliderCtrl m_sliderLightness;
	CSliderCtrl m_sliderSaturation;
	BOOL m_bCheckAcceleract;
	BOOL m_bCheckPixelCorrect;
	BOOL m_bCheckDenoise;
	BOOL m_bCheckColorCorrect;
	BOOL m_bCheckSharpen;
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:

	/// ͼ������
	void ShowImageProcess(CImageDataPointer& objImageDataPointer);

private:

	/// ��ʼ������
	void __InitUI(); 

	/// ��ʼ��ö������UI����
	int __InitEnumUI(gxstring strFeaturName,CComboBox *pComboBox, bool bIsImplemented);

	/// ��ʼ���豸����
	void __InitParam(); 

	/// ���½���
	void __UpdateUI();

	/// �ı�Combox������Ŀ��
	void __ChangeComBoxWidth(CComboBox* pComBox);

	CGXDevicePointer                   m_objDevicePtr;                     ///< �豸���
	CGXStreamPointer                   m_objStreamPtr;                     ///< ������
	CGXFeatureControlPointer           m_objFeatureControlPtr;             ///< ���Կ���������
	CImageProcessConfigPointer         m_objImageProcessPtr;               ///< ͼ�������
	CGXBitmap*                         m_pBitmap;                          ///< ��ʾ����ͼ����
	CWnd*                              m_pWnd;                             ///< ���ھ��
	CSampleCaptureEventHandler*        m_pSampleCaptureEventHandler;       ///< �ص�����ָ��
	bool                               m_bIsOpen;                          ///< ����򿪱�־
	bool                               m_bIsSnap;                          ///< ����ɼ���־
	bool                               m_bIsWhiteBalance;                  ///< �Ƿ�֧�ְ�ƽ��
	bool                               m_bIsWhiteBalanceLight;             ///< �Ƿ�֧�ְ�ƽ���Դ
	int                                m_nComBoAWBOldValue;                ///< �Զ���ƽ��ֵ
	int                                m_nComBoAWBLightOldValue;           ///< �Զ���ƽ���Դֵ
	int                                m_nOldSliderSharpenValue;           ///< ��¼��ֵ
	int                                m_nOldSliderGammaValue;             ///< ��¼Gammaֵ
	int                                m_nOldSliderContrastValue;          ///< ��¼�Աȶȵ�ֵ
	int                                m_nOldSliderLightnessValue;         ///< ��¼����ֵ
	int                                m_nOldSliderSaturationValue;        ///< ��¼���Ͷ�ֵ
	std::string                        m_strBalanceWhiteAutoValue;         ///< ��¼�Զ���ƽ��ģʽ

};
