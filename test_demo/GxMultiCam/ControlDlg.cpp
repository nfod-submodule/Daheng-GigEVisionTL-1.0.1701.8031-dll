// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCam.h"
#include "ControlDlg.h"
#include "GxMultiCamDoc.h"

using namespace std;

// CControlDlg

IMPLEMENT_DYNCREATE(CControlDlg, CFormView)

CControlDlg::CControlDlg()
	: CFormView(CControlDlg::IDD)
	, m_dEditShutter(0)
	, m_dEditGain(0)
	, m_nEditSnapSpeed(0)
	, m_bCheckShowDevice(FALSE)
{
	m_bIsSnapSpeed                = false;
	m_bIsColorFilter              = false;
	m_dShutterMax                 = 0;
	m_dShutterMin                 = 0;
	m_dGainMax                    = 0;
	m_dGainMin                    = 0;
	m_nSnapSpeedMax               = 0;
	m_nSnapSpeedMin               = 0;
	m_nDeviceListCurrent          = 0;
	m_nCurrentBalanceAutoWhiteSel = 0;
	m_strBalanceWhiteAuto         = "Off";
	m_pDeviceProcessCurrent       = NULL;

	for (int i = 0; i< DEVICE_CONTS; i++)
	{
		m_pDeviceProcess[i]          = NULL;
	}

}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboxDevice);
	DDX_Control(pDX, IDC_COMBO_BALANCE_WHITE_AUTO, m_comboxBalanceWhiteAuto);
	DDX_Control(pDX, IDC_STATIC_SHUTTER_TIME, m_staticShutterTime);
	DDX_Control(pDX, IDC_STATIC_GAIN, m_staticGain);
	DDX_Control(pDX, IDC_STATIC_SNAP_SPEED, m_staticSnapSpeed);
	DDX_Control(pDX, IDC_EDIT_SHUTTER, m_editShutter);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_editGain);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dEditShutter);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dEditGain);
	DDX_Text(pDX, IDC_EDIT_SNAP_SPEED, m_nEditSnapSpeed);
	DDX_Check(pDX, IDC_CHECK_SHOW_DEVICE_FPS, m_bCheckShowDevice);
}

BEGIN_MESSAGE_MAP(CControlDlg, CFormView)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CControlDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CControlDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CControlDlg::OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CControlDlg::OnBnClickedBtnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnSelchangeComboDeviceList)
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_AUTO, &CControlDlg::OnCbnSelchangeComboBalanceWhiteAuto)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, &CControlDlg::OnEnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CControlDlg::OnEnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_SNAP_SPEED, &CControlDlg::OnEnKillfocusEditSnapSpeed)
	ON_WM_TIMER()
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnDropdownComboDeviceList)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DEVICE_FPS, &CControlDlg::OnBnClickedCheckShowDeviceFps)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CControlDlg diagnostics

#ifdef _DEBUG
void CControlDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CControlDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	try
	{
		for (int i=0; i<DEVICE_CONTS; i++)
		{
			m_pDeviceProcess[i] = new CDeviceProcess;
		}
		//��ʼ���豸��
		IGXFactory::GetInstance().Init();

		//ö���豸
		IGXFactory::GetInstance().UpdateDeviceList(1000,m_vectorDeviceInfo);

		//δ��⵽�豸
		if (m_vectorDeviceInfo.size() <= 0)
		{
			MessageBox("δ��⵽�豸,��ȷ���豸�������Ӻ���������!");
            GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(false);
			GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(false);
			GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_SNAP_SPEED)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(false);
			GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(false);
			CMainFrame* pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
			pMainFrm->m_objControlDlg = this;
			return;
		}

		//���豸������ʾ���豸�б���
		for (uint32_t i =0; i< m_vectorDeviceInfo.size(); i++)
		{
			//����豸������4��ֻ����4���豸��Ϣ
			if (i >= DEVICE_CONTS)
			{
				break;
			}

			gxstring strDeviceInformation = "";
			strDeviceInformation =m_vectorDeviceInfo[i].GetDisplayName();
			m_comboxDevice.SetItemData(m_comboxDevice.AddString(strDeviceInformation.c_str()),i);
			m_mapDeviceInformation.insert(map<int,CDeviceProcess*>::value_type(i,m_pDeviceProcess[i]));

			//���½���
			__UpdateUI(m_pDeviceProcess[i]);

		}

		//���ö�ʱ��
		SetTimer(0,1000,NULL);
		m_comboxDevice.SetCurSel(0);

		//��ȡ��ǰ��ʾ���豸���������
		m_pDeviceProcessCurrent = m_mapDeviceInformation[0];

		CMainFrame * pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
		pMainFrm->m_objControlDlg = this;
		
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
	// TODO: Add your specialized code here and/or call the base class
	
}

// CControlDlg message handlers

void CControlDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here

    try
    {
		//��ȡ��ʾ�����Ӵ��ھ��
		CWnd*  pWnd = NULL;
		CMainFrame * pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
		switch(m_nDeviceListCurrent)
		{
		case 0:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(0, 0);
			break;
		case 1:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(0, 1);
			break;
		case 2:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(1, 0);
			break;
		case 3:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(1, 1);
			break;

		default: break;
		}

		//��ȡ�豸���
		int nDeviceIndex = m_nDeviceListCurrent+1;

		//���豸
		m_pDeviceProcessCurrent->OpenDevice(m_vectorDeviceInfo[m_nDeviceListCurrent].GetSN(),pWnd,nDeviceIndex);

		//��ʼ������
		__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);

    }
    catch (CGalaxyException& e)
    {
		MessageBox(e.what());
		return;
    }
	catch (std::exception& e)
    {
		MessageBox(e.what());
		return;
    }
}
//----------------------------------------------------------------------
/**
\breaf  ��ʼ������

\param  objFeatureControlPtr[in]      ���Կ�����

\return void
**/
//----------------------------------------------------------------------
void CControlDlg::__InitUI(CGXFeatureControlPointer objFeatureControlPtr)
{
 
	if (objFeatureControlPtr.IsNull())
	{
		return;
	}
    
	bool bIsBalanceWhiteAutoRead = false;        // �Զ���ƽ���Ƿ�ɶ�
	bool bBalanceWhiteAuto       = false;        // �Ƿ�֧���Զ���ƽ��

	//�Ƿ�֧��Bayer��ʽ
	m_bIsColorFilter = objFeatureControlPtr->IsImplemented("PixelColorFilter");

	if (m_bIsColorFilter)
	{
		//��ʼ���Զ���ƽ����Ͽ�
		//�Ƿ�֧�ְ�ƽ��
		bBalanceWhiteAuto = objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

		//��ƽ���Ƿ�ɶ�
		bIsBalanceWhiteAutoRead = objFeatureControlPtr->IsReadable("BalanceWhiteAuto");
		if (bBalanceWhiteAuto)
		{
			if (bIsBalanceWhiteAutoRead)
			{
				m_strBalanceWhiteAuto = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();
			}
			int nCursel  = 0;
			gxstring strCurEnumList = "";
			GxIAPICPP::gxstring_vector vectorEnumEntryList;

			//��ȡ��ǰ�豸����ֵ
			strCurEnumList = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();

			//��ȡ�豸����ö����
			vectorEnumEntryList = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetEnumEntryList();

			//�����Ͽ��б�
			m_comboxBalanceWhiteAuto.ResetContent();
			for (uint32_t i = 0; i<vectorEnumEntryList.size(); i++)
			{
				std::string strEnumList = vectorEnumEntryList[i].c_str();
				m_comboxBalanceWhiteAuto.SetItemData(m_comboxBalanceWhiteAuto.AddString(strEnumList.c_str()),i);

				if (strCurEnumList == vectorEnumEntryList[i])
				{
					nCursel = i;
				}

			}
			m_nCurrentBalanceAutoWhiteSel = nCursel;
			m_comboxBalanceWhiteAuto.SetCurSel(nCursel);
		}
	}
	

	//��ʼ���ع�ʱ��
	CString  strShutterTimeRange = "";          // �ع�ʱ�䷶Χ
	gxstring strShutterTimeUint  = "";          // �ع�ʱ�䵥λ

	m_dEditShutter     = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();
	strShutterTimeUint = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetUnit();
	m_dShutterMax      = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();
	m_dShutterMin      = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();

	strShutterTimeRange.Format("�ع�(%.4f~%.4f)%s",m_dShutterMin,m_dShutterMax,strShutterTimeUint.c_str());
	m_staticShutterTime.SetWindowText(strShutterTimeRange);

	//��ʼ������
	CString  strGainRange = "";                // ����ֵ��Χ
	gxstring strGainUint  = "";                // ���浥λ

	m_dEditGain   = objFeatureControlPtr->GetFloatFeature("Gain")->GetValue();
	strGainUint   = objFeatureControlPtr->GetFloatFeature("Gain")->GetUnit();
	m_dGainMax    = objFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
	m_dGainMin    = objFeatureControlPtr->GetFloatFeature("Gain")->GetMin();

	strGainRange.Format("����(%.4f~%.4f)%s",m_dGainMin,m_dGainMax,strGainUint.c_str());
	m_staticGain.SetWindowText(strGainRange);

	//��ʼ���ɼ��ٶȼ���
	//�Ƿ�֧�ֲɼ��ٶȼ���
	m_bIsSnapSpeed = objFeatureControlPtr->IsImplemented("AcquisitionSpeedLevel");
	if (m_bIsSnapSpeed)
	{
		CString  strSnapSpeedRange = "";  
		m_nEditSnapSpeed   = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetValue();
		m_nSnapSpeedMax    = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetMax();
		m_nSnapSpeedMin    = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetMin();

		strSnapSpeedRange.Format("�ɼ��ٶȼ���(%lld~%lld)",m_nSnapSpeedMin,m_nSnapSpeedMax);
		m_staticSnapSpeed.SetWindowText(strSnapSpeedRange);

	}
	else
	{
		//ˢ�±༭��
		m_nEditSnapSpeed = NULL;
	}

	//����ǰ�豸�Ƿ���ʾ֡�ʱ�־ˢ������
	m_bCheckShowDevice = m_pDeviceProcessCurrent->GetShowFPS();

	UpdateData(FALSE);

}

//----------------------------------------------------------------------
/**
\breaf  ���½���

\param  pDeviceProcess[in]      �豸���������

\return void
**/
//----------------------------------------------------------------------
void CControlDlg::__UpdateUI(CDeviceProcess* pDeviceProcess)
{
	if (pDeviceProcess == NULL)
	{
		return;
	}

	bool IsOpen = pDeviceProcess->IsOpen();
	bool IsSnap = pDeviceProcess->IsSnap();
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!IsOpen);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(IsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(IsOpen&& !IsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(IsOpen&& IsSnap);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(IsOpen);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(IsOpen);
	GetDlgItem(IDC_EDIT_SNAP_SPEED)->EnableWindow(IsOpen&& !IsSnap&& m_bIsSnapSpeed);
	GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(IsOpen&& m_bIsColorFilter);
	GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(IsOpen);

}

void CControlDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//��ʼ�ɼ�
		m_pDeviceProcessCurrent->StartSnap();

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//ֹͣ�ɼ�
		m_pDeviceProcessCurrent->StopSnap();

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	try
	{
		//�ر��豸
		m_pDeviceProcessCurrent->CloseDevice();
		

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
		return;
	}
}

void CControlDlg::OnCbnSelchangeComboDeviceList()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_nDeviceListCurrent = m_comboxDevice.GetCurSel();

		m_pDeviceProcessCurrent = m_mapDeviceInformation[m_nDeviceListCurrent];

		//�жϵ�ǰ�豸�Ƿ��Ѵ�
		bool bIsOpen = m_pDeviceProcessCurrent->IsOpen();
		if (!bIsOpen)
		{
			__UpdateUI(m_pDeviceProcessCurrent);
		}

		else
		{
			//ˢ�½���
			__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

			//���½���
			__UpdateUI(m_pDeviceProcessCurrent);

		}
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;
		
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

void CControlDlg::OnCbnSelchangeComboBalanceWhiteAuto()
{
	// TODO: Add your control notification handler code here
	try
	{
		
		int      nCurSelBalanceWhiteAuto = 0;      // �Զ���ƽ����Ͽ�ǰֵ
		CString  strCurText              = "";     // �Զ���ƽ�⵱ǰö����
		gxstring strCurEnumList          = "";     

		nCurSelBalanceWhiteAuto = m_comboxBalanceWhiteAuto.GetCurSel();
		m_comboxBalanceWhiteAuto.GetLBText(nCurSelBalanceWhiteAuto,strCurText);
		strCurEnumList = strCurText.GetBuffer(0);

		//��¼��ǰ��ƽ��ģʽ
		m_strBalanceWhiteAuto = strCurEnumList;

		//����ǰ����ֵ���õ���Ӧ�豸��
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(strCurEnumList); 

		//��¼��Ͽ�ǰֵ
		m_nCurrentBalanceAutoWhiteSel =nCurSelBalanceWhiteAuto;

	}
	catch (CGalaxyException& e)
	{
		m_comboxBalanceWhiteAuto.SetCurSel(m_nCurrentBalanceAutoWhiteSel);
		MessageBox(e.what());
		return;
	}
	catch (std::exception& e)
	{
		m_comboxBalanceWhiteAuto.SetCurSel(m_nCurrentBalanceAutoWhiteSel);
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnEnKillfocusEditShutter()
{
	// TODO: Add your control notification handler code here
	double dShutterOld = m_dEditShutter;           // ��¼�ع�ʱ��ֵ
 	if (!m_pDeviceProcessCurrent->IsOpen())
	{
		return;
	}
	try
	{
		UpdateData(TRUE);

		//�жϱ༭���������ֵ�Ƿ����ع�ʱ�䷶Χ��
		if (m_dEditShutter < m_dShutterMin  )
		{
			m_dEditShutter = m_dShutterMin;
		}

		if (m_dEditShutter > m_dShutterMax)
		{
			m_dEditShutter = m_dShutterMax;
		}

		//����ֵ���õ��豸��
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(m_dEditShutter); 

	}
	catch (CGalaxyException& e)
	{
		m_dEditShutter = dShutterOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditShutter = dShutterOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CControlDlg::OnEnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	double dGainOld = m_dEditGain;         // ��¼����ֵ
	if (!m_pDeviceProcessCurrent->IsOpen())
	{
		return;
	}

	try
	{
		UpdateData(TRUE);

		//�жϱ༭���������ֵ�Ƿ������淶Χ��
		if (m_dEditGain < m_dGainMin )
		{
			m_dEditGain = m_dGainMin;
		}

		if (m_dEditGain > m_dGainMax)
		{
			m_dEditGain = m_dGainMax;
		}

		//����ֵ���õ��豸��
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetFloatFeature("Gain")->SetValue(m_dEditGain); 

	}
	catch (CGalaxyException& e)
	{
		m_dEditGain = dGainOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditGain = dGainOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CControlDlg::OnEnKillfocusEditSnapSpeed()
{
	// TODO: Add your control notification handler code here
	int64_t     nSnapSpeedOld    = m_nEditSnapSpeed;      // ��¼�ɼ��ٶȼ���ֵ

	try
	{
		UpdateData(TRUE);

		//�жϱ༭���������ֵ�Ƿ��ڲɼ��ٶȼ���Χ��
		if (m_nEditSnapSpeed < m_nSnapSpeedMin  )
		{
			m_nEditSnapSpeed = m_nSnapSpeedMin;
		}

		if (m_nEditSnapSpeed > m_nSnapSpeedMax)
		{
			m_nEditSnapSpeed = m_nSnapSpeedMax;
		}
		//����ֵ���õ��豸��
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->SetValue(m_nEditSnapSpeed); 

	}
	catch (CGalaxyException& e)
	{
		m_nEditSnapSpeed = nSnapSpeedOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_nEditSnapSpeed = nSnapSpeedOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CControlDlg::OnBnClickedCheckShowDeviceFps()
{
	// TODO: Add your control notification handler code here
	try
	{
		UpdateData(TRUE);
		m_pDeviceProcessCurrent->SetShowFPS(m_bCheckShowDevice == TRUE);
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd  *pWnd   = NULL;
	int   nCtrlID = 0;             //< �����ȡ�Ŀؼ�ID

	//�ж��Ƿ��Ǽ��̻س���Ϣ
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		//��õ�ǰ����Ŀؼ�ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//�ж�ID����
		switch(nCtrlID)
		{
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:
		case IDC_EDIT_SNAP_SPEED:

			//ʧȥ����
			SetFocus();
			break;
		default:
			break;
		}
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	try
	{
		//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
		//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
		CString strCurText = "";
		if (m_strBalanceWhiteAuto == "Once")
		{
			//��ȡ�豸��ǰ�İ�ƽ��
			m_strBalanceWhiteAuto = m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				->GetValue();
			//��ȡ�豸��ƽ��ö����
			GxIAPICPP::gxstring_vector vectorEnumEntryList;
			vectorEnumEntryList = m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				->GetEnumEntryList();
			//�жϰ�ƽ��ö��ֵ�Ƿ�ΪOff
			if (m_strBalanceWhiteAuto == "Off")
			{
				for (uint32_t i = 0; i < vectorEnumEntryList.size(); i++)
				{
					m_comboxBalanceWhiteAuto.GetLBText(i,strCurText);
					if (strCurText == "Off")
					{
						m_comboxBalanceWhiteAuto.SetCurSel(i);
						break;
					}
				}

			}

		}
		CFormView::OnTimer(nIDEvent);
	}
	catch (CGalaxyException)
	{
		return;
	}
	catch (std::exception)
	{
		return;
	}
	CFormView::OnTimer(nIDEvent);
}

void CControlDlg::OnCbnDropdownComboDeviceList()
{
	// TODO: Add your control notification handler code here
	CComboBox *pWnd = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE_LIST);
	int nNumEntries = pWnd->GetCount();
	int nWidth = 0;
	CString str;

	CClientDC dc(pWnd);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());

	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	for (int i = 0; i < nNumEntries; i++)
	{
		pWnd->GetLBText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = max(nWidth, nLength);
	}

	// Add margin space to the calculations
	nWidth += dc.GetTextExtent("0").cx;

	dc.RestoreDC(nSave);
	pWnd->SetDroppedWidth(nWidth);
}

void CControlDlg::OnDestroy()
{
	try
	{
		//����map���������Ѵ��豸ȫ���ر�
		map<int,CDeviceProcess*>::iterator it;
		for (it = m_mapDeviceInformation.begin(); it != m_mapDeviceInformation.end(); ++it)
		{
			it->second->CloseDevice();

		}

		m_mapDeviceInformation.clear();

	}
	catch (CGalaxyException)
	{
		//do noting

	}
	catch (std::exception)
	{
		//do noting
	}

	try
	{
		//�ͷ��豸����Դ
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException)
	{
		//do noting
	}



	for (int i = 0; i <DEVICE_CONTS; i++)
	{
		if (m_pDeviceProcess[i] != NULL)
		{
			delete m_pDeviceProcess[i];
			m_pDeviceProcess[i] = NULL;
		}
	}
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}
