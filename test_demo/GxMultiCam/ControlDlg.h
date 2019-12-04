#pragma once
#include <iostream>
#include "DeviceProcess.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "map"

#define  DEVICE_CONTS 4               ///< �������ͬʱ����4̨�豸

// CControlDlg form view

class CControlDlg : public CFormView
{
	DECLARE_DYNCREATE(CControlDlg)

public:
	CControlDlg();           // protected constructor used by dynamic creation
	virtual ~CControlDlg();

public:
	enum { IDD = IDD_CONTROL_DIALOG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnCbnSelchangeComboDeviceList();
	afx_msg void OnCbnSelchangeComboBalanceWhiteAuto();
	afx_msg void OnEnKillfocusEditShutter();
	afx_msg void OnEnKillfocusEditGain();
	afx_msg void OnEnKillfocusEditSnapSpeed();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnDropdownComboDeviceList();
	afx_msg void OnBnClickedCheckShowDeviceFps();
	BOOL      m_bCheckShowDevice;
	CComboBox m_comboxDevice;
	CComboBox m_comboxBalanceWhiteAuto;
	CStatic   m_staticShutterTime;
	CStatic   m_staticGain;
	CStatic   m_staticSnapSpeed;
	CEdit     m_editShutter;
	CEdit     m_editGain;
	CEdit     m_editSnapSpeed;
	double    m_dEditShutter;
	double    m_dEditGain;
	int64_t   m_nEditSnapSpeed;

private:
	
	/// ��ʼ������UI
	void __InitUI(CGXFeatureControlPointer objFeatureControlPtr);

	/// ���½���
	void __UpdateUI(CDeviceProcess* pDeviceProcess);

	GxIAPICPP::gxdeviceinfo_vector m_vectorDeviceInfo;          ///< ö�ٵ����豸��Ϣ
	CDeviceProcess* m_pDeviceProcess[DEVICE_CONTS];             ///< �豸���������
	CDeviceProcess* m_pDeviceProcessCurrent;                    ///< ��ǰ�豸���������
	std::map<int, CDeviceProcess*> m_mapDeviceInformation;      ///< ���ڴ洢�豸������
	bool     m_bIsSnapSpeed;                                    ///< �Ƿ�֧�ֲɼ��ٶȼ���
	bool     m_bIsColorFilter;                                  ///< �Ƿ�֧��Bayer��ʽ
	double   m_dShutterMax;                                     ///< �ع�ʱ�����ֵ
	double   m_dShutterMin;                                     ///< �ع�ʱ����Сֵ
	double   m_dGainMax;                                        ///< �������ֵ
	double   m_dGainMin;                                        ///< ������Сֵ
	int64_t  m_nSnapSpeedMax;                                   ///< �ɼ��ٶȼ������ֵ
	int64_t  m_nSnapSpeedMin;                                   ///< �ɼ��ٶȼ�����Сֵ
	int      m_nDeviceListCurrent;                              ///< ��ǰ�豸�б����
	int      m_nCurrentBalanceAutoWhiteSel;                     ///< ��¼��ƽ���ֵ
	gxstring m_strBalanceWhiteAuto;                             ///< ��ǰ�Զ���ƽ��

public:
	afx_msg void OnDestroy();
};


