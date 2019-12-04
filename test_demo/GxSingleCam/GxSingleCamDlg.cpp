// GxSingleCamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"
#include "FileVersion.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strStaticProductVersion;
	CString m_strStaticLegalCopyright;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strStaticProductVersion(_T(""))
, m_strStaticLegalCopyright(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strStaticProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strStaticLegalCopyright);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGxSingleCamDlg �Ի���
CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_bColorFilter(false)
	, m_bTriggerMode(false)
	, m_bTriggerSource(false)
	, m_bTriggerActive(false)
	, m_bBalanceWhiteAuto(false)
	, m_bBalanceWhiteRatioSelect(false)
	, m_strSavePath("")
	, m_strBalanceWhiteAutoMode("Off")
	, m_pWnd(NULL)
    , m_pSampleCaptureEventHandle(NULL)
	, m_pBitmap(NULL)
	, m_bCheckSaveBmp(FALSE)
	, m_dEditShutterValue(0)
	, m_dEditGainValue(0)
	, m_dEditBalanceRatioValue(0)
	, m_nTriggerModeOld(0)
	, m_nTriggerSourceOld(0)
	, m_nTriggerActiveOld(0)
	, m_nBalanceWhiteAutoOld(0)
	, m_nBanlanceWhiteRatioOld(0)
	, m_dShutterValueMax(0)
	, m_dShutterValueMin(0)
	, m_dGainValueMax(0)
	, m_dGainValueMin(0)
	, m_dBalanceWhiteRatioMax(0)
	, m_dBalanceWhiteRatioMin(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxSingleCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SAVE_BMP, m_bCheckSaveBmp);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dEditShutterValue);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dEditGainValue);
	DDX_Text(pDX, IDC_EDIT_BALANCE_WHITE_SERISE, m_dEditBalanceRatioValue);
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CGxSingleCamDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CGxSingleCamDlg::OnBnClickedBtnStopSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_ACTIVE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive)
	ON_BN_CLICKED(IDC_CHECK_SAVE_BMP, &CGxSingleCamDlg::OnBnClickedCheckSaveBmp)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_AUTO, &CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_SELECT, &CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteSelect)
	ON_BN_CLICKED(IDC_BTN_SOFTTRIGGER, &CGxSingleCamDlg::OnBnClickedBtnSofttrigger)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CGxSingleCamDlg::OnEnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_WHITE_SERISE, &CGxSingleCamDlg::OnEnKillfocusEditBalanceWhiteSerise)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, &CGxSingleCamDlg::OnEnKillfocusEditShutter)

END_MESSAGE_MAP()

// CGxSingleCamDlg ��Ϣ�������
BOOL CGxSingleCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	try
	{
		//��ʼ���豸
		IGXFactory::GetInstance().Init();

		//������ָ��ָ��picture�ؼ�
		m_pWnd = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();

		//��ȡ��ִ�г���ĵ�ǰ·��
		char    strFileName[MAX_PATH] = {0};
		string  strSavePath           = "";
		size_t  nPos                  = 0;

		GetModuleFileName(NULL,(LPCH)strFileName,MAX_PATH);
		strSavePath = strFileName;
		nPos = strSavePath.find_last_of('\\');
		m_strSavePath = strSavePath.substr(0,nPos);
		m_strSavePath = m_strSavePath +"\\GxSingleCamImages";

		//���ö�ʱ��
		SetTimer(0,1000,NULL);

		//���½���
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;
		
	}
	catch (std::exception& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;

	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGxSingleCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, strFileName, MAX_PATH);
		CFileVersion fv(strFileName);
		CAboutDlg dlgAbout;
		dlgAbout.m_strStaticProductVersion = _T("Version: ") + fv.GetProductVersion();
		dlgAbout.m_strStaticLegalCopyright = fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGxSingleCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGxSingleCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGxSingleCamDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen = false;         ///< �豸�Ƿ�򿪱�־
	bool bIsStreamOpen = false;         ///< �豸���Ƿ�򿪱�־

	try
	{
		//ö���豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);

	    //�ж�ö�ٵ����豸�Ƿ�����㣬��������򵯿���ʾ
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("δ�����豸!");
			return;
		}

		//���豸
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(),GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//�жϻ�ͼ�����Ƿ�Ϊ��
		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		//Ϊ��ͼ��������ڴ�
		m_pBitmap = new CGXBitmap(m_objDevicePtr,m_pWnd);

		//�ж��豸���Ƿ�����㣬��������������
		int nStreamCount = m_objDevicePtr->GetStreamCount();

		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("δ�����豸��!");
		}

		//��ʼ���豸����
		__InitParam();

		//��ʼ������
		__InitUI();
		m_bIsOpen = true;

		//���½���
		__UpdateUI();
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

		MessageBox(e.what());
		return;
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

		MessageBox(e.what());
		return;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���豸����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitParam()
{
	bool bBalanceWhiteAutoRead = false;         ///< ��ƽ���Ƿ�ɶ�

	//���òɼ�ģʽΪ�����ɼ�ģʽ
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//�Ƿ�֧�ִ���ģʽѡ��
	m_bTriggerMode = m_objFeatureControlPtr->IsImplemented("TriggerMode");
	if (m_bTriggerMode)
	{
		//���ô���ģʽ��
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
	}

	//�Ƿ�֧��Bayer��ʽ
	m_bColorFilter = m_objFeatureControlPtr->IsImplemented("PixelColorFilter");

	//�Ƿ�֧�ִ���Դѡ��
	m_bTriggerSource = m_objFeatureControlPtr->IsImplemented("TriggerSource");

	//�Ƿ�֧�ִ�������ѡ��
	m_bTriggerActive = m_objFeatureControlPtr->IsImplemented("TriggerActivation");

	//�Ƿ�֧���Զ���ƽ��
	m_bBalanceWhiteAuto = m_objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

	//��ƽ���Ƿ�ɶ�
	bBalanceWhiteAutoRead   = m_objFeatureControlPtr->IsReadable("BalanceWhiteAuto");

	//���֧���ҿɶ������ȡ�豸��ǰ��ƽ��ģʽ
	if (m_bBalanceWhiteAuto)
	{
		if (bBalanceWhiteAutoRead)
		{
			m_strBalanceWhiteAutoMode = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                              ->GetValue();
		}
	}

	//�Ƿ�֧���Զ���ƽ��ͨ��ѡ��
	m_bBalanceWhiteRatioSelect = m_objFeatureControlPtr->IsImplemented("BalanceRatioSelector");

	//��ȡ�ع�ʱ�䡢���漰�Զ���ƽ��ϵ�������ֵ����Сֵ
	m_dShutterValueMax      = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();
	m_dShutterValueMin      = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();
	m_dGainValueMax         = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
	m_dGainValueMin         = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetMin();
	m_dBalanceWhiteRatioMax = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetMax();
	m_dBalanceWhiteRatioMin = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetMin();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitUI()
{
	//�ж��Ƿ�Ϊ��ɫ���
	if (m_bColorFilter)
	{
		//��ʼ����Ͽ�
		m_nTriggerModeOld        = __InitComBox("TriggerMode",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE),
			                                    m_bTriggerMode);
		m_nTriggerSourceOld      = __InitComBox("TriggerSource",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),
			                                     m_bTriggerSource);
		m_nTriggerActiveOld      = __InitComBox("TriggerActivation",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE),
			                                     m_bTriggerActive);
		m_nBanlanceWhiteRatioOld = __InitComBox("BalanceRatioSelector",
			                                     (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT),
			                                     m_bBalanceWhiteRatioSelect);
		m_nBalanceWhiteAutoOld   = __InitComBox("BalanceWhiteAuto",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO),
			                                     m_bBalanceWhiteAuto);
	}
	else
	{
		//��ʼ����Ͽ�
		m_nTriggerModeOld        = __InitComBox("TriggerMode",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE),
			                                    m_bTriggerMode);
		m_nTriggerSourceOld      = __InitComBox("TriggerSource",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),
			                                    m_bTriggerSource);
		m_nTriggerActiveOld      = __InitComBox("TriggerActivation",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE),
			                                    m_bTriggerActive);
	}
	
	//��ʼ���ع�ؼ�
	__InitShutterTime();
	
	//��ʼ������ؼ�
	__InitGain();

	//��ʼ����ƽ��ϵ���ؼ�
	__InitBalanceWhiteRatio();

}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   strName         [in]    ��������
\param   pComboBox       [in]    �ؼ�ָ��
\param   bIsImplemented  [in]    �豸�Ƿ�֧��emFeatureID������

\return  ��
*/
//----------------------------------------------------------------------------------
int CGxSingleCamDlg::__InitComBox(gxstring strName, CComboBox* pComboBox, bool bIsImplemented)
{
	if ((pComboBox == NULL) || (!bIsImplemented))
	{
		return -1;
	}

	int     nCurSel = 0;
	string  strCurrentEnumList = "";                 // �豸��ǰ����
	GxIAPICPP::gxstring_vector vectorEnumEntryList;  // �豸ö����
	CEnumFeaturePointer objEnumFeaturePtr = m_objFeatureControlPtr->GetEnumFeature(strName);

	//�����ǰ�ؼ�����ѡ��
	pComboBox->ResetContent();

	//��ȡ�豸��ǰ����ֵ
	strCurrentEnumList = objEnumFeaturePtr->GetValue().c_str();

	//��ȡ�豸����ö����
	vectorEnumEntryList = objEnumFeaturePtr->GetEnumEntryList();
	for (uint32_t i = 0; i<vectorEnumEntryList.size(); i++)
	{
		string strEnumList = vectorEnumEntryList[i].c_str();
		pComboBox->SetItemData(pComboBox->AddString(strEnumList.c_str()), i);
		if (strCurrentEnumList == vectorEnumEntryList[i].c_str())
		{
			nCurSel = i;
		}
	}

	pComboBox->SetCurSel(nCurSel);
	return nCurSel;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع�ʱ��ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitShutterTime()
{
	CStatic* pStcShutterShow    = (CStatic*)GetDlgItem(IDC_STATIC_SHUTTER_TIME);
	CEdit*   pEditShutterVal    = (CEdit*)GetDlgItem(IDC_EDIT_SHUTTER);

	//�жϿؼ�����Ч��
	if (pEditShutterVal == NULL || pStcShutterShow == NULL)
	{
		return;
	}

	CString   strShutterTime      = "";       ///< ��ǰ�ع�ʱ��
	gxstring  strShutterUnit      = "";       ///< �ع�ʱ�䵥λ

	//��ȡ�豸��ǰ�ع�ʱ��
	m_dEditShutterValue   = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();

	//��ȡ�豸�ع�ʱ�䵥λ
    strShutterUnit = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetUnit();
	strShutterTime.Format("�ع�(%.4f~%.4f)%s",m_dShutterValueMin,m_dShutterValueMax,strShutterUnit.c_str());
	pStcShutterShow->SetWindowText(strShutterTime);
	
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitGain()
{
	CStatic* pStcGainShow    = (CStatic*)GetDlgItem(IDC_STATIC_GAIN);
	CEdit*   pEditGainVal    = (CEdit*)GetDlgItem(IDC_EDIT_GAIN);

	//�жϿؼ�����Ч��
	if (pEditGainVal == NULL || pStcGainShow == NULL)
	{
		return;
	}

	CString  strGain      = "";     ///< ��ǰ����ֵ
	string strGainUnit    = "";     ///< ���浥λ

	//��ȡ�豸��ǰ����ֵ
	m_dEditGainValue = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetValue();
	
	//��ȡ�豸���浥λ
	strGainUnit = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetUnit().c_str();
	strGain.Format("����(%.4f~%.4f)%s",m_dGainValueMin,m_dGainValueMax,strGainUnit.c_str());
	pStcGainShow->SetWindowText(strGain);
	
	UpdateData(FALSE);

}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���Զ���ƽ��ϵ���ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitBalanceWhiteRatio()
{
	//�ж��Ƿ�Ϊ�ڰ����
	if (!m_bColorFilter)
	{
		return;
	}

	CStatic* pStcBalanceRatioShow    = (CStatic*)GetDlgItem(IDC_STATIC_BALANCE_RATIO);
	CEdit*   pEditBalanceRatioVal    = (CEdit*)GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE);

	//�жϿؼ�����Ч��
	if (pEditBalanceRatioVal == NULL || pStcBalanceRatioShow == NULL)
	{
		return;
	}

	CString strBalanceRatio      = "";        ///< ��ǰ��ƽ��ϵ��
	string  strBalanceRatioUnit  = "";        ///< ��ƽ��ϵ����λ

	//��ȡ��ƽ��ϵ����ǰֵ
	m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();

	//��ȡ��ƽ��ϵ����λ
	strBalanceRatioUnit = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetUnit().c_str();

	//���ÿؼ�����
	strBalanceRatio.Format("�Զ���ƽ��ϵ��(%.4f~%.4f)%s",m_dBalanceWhiteRatioMin,
		                    m_dBalanceWhiteRatioMax,strBalanceRatioUnit.c_str());
    pStcBalanceRatioShow->SetWindowText(strBalanceRatio);
	
	UpdateData(FALSE);

}
//---------------------------------------------------------------------------------
/**
\brief   ����UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__UpdateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bIsOpen && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(m_bIsOpen && m_bTriggerMode);
	GetDlgItem(IDC_COMBO_TRIGGER_SOURCE)->EnableWindow(m_bIsOpen && m_bTriggerSource);
	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE)->EnableWindow(m_bIsOpen && m_bTriggerActive);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_CHECK_SAVE_BMP)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(m_bIsOpen 
		                                                   && m_bBalanceWhiteAuto
														   && m_bColorFilter);

	GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT)->EnableWindow(m_bIsOpen 
		                                                     && m_bBalanceWhiteRatioSelect
															 && m_bColorFilter);

	GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(m_bIsOpen
		                                                   && m_strBalanceWhiteAutoMode == "Off"
		                                                   && m_bBalanceWhiteRatioSelect
														   && m_bColorFilter);


}

void CGxSingleCamDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	//ʧȥ����
	SetFocus();

	try
	{
		//�ж��Ƿ���ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ɼ��ص�
			m_objStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
    
	try
	{
		//�ر�������
		m_objStreamPtr->Close();

	}
	catch(CGalaxyException)
	{
		//do noting
	}
	try
	{
		//�ر��豸
		m_objDevicePtr->Close();
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	

	m_bIsOpen = false;
	m_bIsSnap = false;

	//���½���
	__UpdateUI();
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}

void CGxSingleCamDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//ע��ص�����
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);

		//��������ͨ��
		m_objStreamPtr->StartGrab();

		//���Ϳ�������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;

		//���½���
		__UpdateUI();
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

void CGxSingleCamDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//����ͣ������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ͨ��
		m_objStreamPtr->StopGrab();

		//ע���ɼ��ص�
		m_objStreamPtr->UnregisterCaptureCallback();
		m_bIsSnap = false;

		//���½���
		__UpdateUI();
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

void CGxSingleCamDlg::OnBnClickedBtnSofttrigger()
{
	// TODO: Add your control notification handler code here
	try
	{
		//������������(�ڴ���ģʽ����ʱ��Ч)
		m_objFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();
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
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< ��Ͽ�ǰֵ

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//����ǰ�������õ��豸��
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue(strCurrentText);
		m_nTriggerModeOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerModeOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerModeOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< ��Ͽ�ǰֵ

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//����ǰ�������õ��豸��
		m_objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue(strCurrentText);
		m_nTriggerSourceOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerSourceOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerSourceOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< ��Ͽ�ǰֵ

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//����ǰ�������õ��豸��
		m_objFeatureControlPtr->GetEnumFeature("TriggerActivation")->SetValue(strCurrentText);
		m_nTriggerActiveOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerActiveOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerActiveOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteAuto()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";      ///< ��Ͽ�ǰֵ

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//��¼��ǰ��ƽ��ģʽ
		m_strBalanceWhiteAutoMode = strCurrentText;

		//����ǰ�������õ��豸��
		m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(strCurrentText);

		if (strCurrentText == "Off")
		{
			GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(TRUE);
			m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
			UpdateData(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(FALSE);
		}

		m_nBalanceWhiteAutoOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nBalanceWhiteAutoOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nBalanceWhiteAutoOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteSelect()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;      
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";        ///< ��Ͽ�ǰֵ

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//����ǰ�������õ��豸��
		m_objFeatureControlPtr->GetEnumFeature("BalanceRatioSelector")->SetValue(strCurrentText);
		m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
		UpdateData(FALSE);
		m_nBanlanceWhiteRatioOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nBanlanceWhiteRatioOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nBanlanceWhiteRatioOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnBnClickedCheckSaveBmp()
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
}

void CGxSingleCamDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		//ֹͣOnTimer
		KillTimer(0);

		//�ж��Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ͨ��
			m_objStreamPtr->StopGrab();

			//ע���ɼ��ص�
			m_objStreamPtr->UnregisterCaptureCallback();

			m_bIsSnap = false;
		}
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
		//�ж��Ƿ�ر��豸
		if (m_bIsOpen)
		{
			//�ر�������
			m_objStreamPtr->Close();

			//�ر��豸
			m_objDevicePtr->Close();

			m_bIsOpen = false;
		}
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
		//�ͷ��豸��Դ
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}

	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}

	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	CDialog::OnClose();
}

void CGxSingleCamDlg::OnEnKillfocusEditShutter()
{
	// TODO: Add your control notification handler code here
	//�ж��豸����Ƿ���Ч�������豸����ʱ�رճ�������쳣
	if (!m_bIsOpen)
	{
		return;
	}
     
	double dShutterValueOld = m_dEditShutterValue;
	try
	{
		UpdateData(TRUE);

		//�ж�����ֵ�Ƿ����ع�ʱ�䷶Χ�ڣ����������������������ı߽�ֵ
		if (m_dEditShutterValue > m_dShutterValueMax )
		{
			m_dEditShutterValue = m_dShutterValueMax;
		}
		if (m_dEditShutterValue < m_dShutterValueMin)
		{
			m_dEditShutterValue = m_dShutterValueMin;
		}

		m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(m_dEditShutterValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditShutterValue = dShutterValueOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditShutterValue = dShutterValueOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);

}

void CGxSingleCamDlg::OnEnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	//�ж��豸����Ƿ���Ч�������豸����ʱ�رճ�������쳣
	if (!m_bIsOpen)
	{
		return;
	}
    
	double dGainValueOld = m_dEditGainValue;
	try
	{
		UpdateData(TRUE);

		//�ж�����ֵ�Ƿ�������ֵ��Χ�ڣ����������������������ı߽�ֵ
		if (m_dEditGainValue > m_dGainValueMax )
		{
			m_dEditGainValue = m_dGainValueMax;
		}
		if (m_dEditGainValue < m_dGainValueMin)
		{
			m_dEditGainValue = m_dGainValueMin;
		}
		m_objFeatureControlPtr->GetFloatFeature("Gain")->SetValue(m_dEditGainValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditGainValue = dGainValueOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditGainValue = dGainValueOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CGxSingleCamDlg::OnEnKillfocusEditBalanceWhiteSerise()
{
	// TODO: Add your control notification handler code here
	//�ж��豸����Ƿ���Ч�������豸����ʱ�رճ�������쳣
	if (!m_bIsOpen)
	{
		return;
	}

    double dBalanceWhiteRatioOld = m_dEditBalanceRatioValue;
	try
	{
		UpdateData(TRUE);

		//�ж�����ֵ�Ƿ����Զ���ƽ��ϵ����Χ�ڣ����������������������ı߽�ֵ
		if (m_dEditBalanceRatioValue > m_dBalanceWhiteRatioMax)
		{
			m_dEditBalanceRatioValue = m_dBalanceWhiteRatioMax;
		}
		if ((m_dEditBalanceRatioValue < m_dBalanceWhiteRatioMin))
		{
			m_dEditBalanceRatioValue = m_dBalanceWhiteRatioMin;
		}
		m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->SetValue(m_dEditBalanceRatioValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditBalanceRatioValue = dBalanceWhiteRatioOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditBalanceRatioValue = dBalanceWhiteRatioOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CGxSingleCamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		CString strText  = "";    ///> ��ǰ��Ͽ���ʾ������
		CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO);

		//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
		//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
		if (m_strBalanceWhiteAutoMode == "Once")
		{
			//��ȡ�Զ���ƽ��ö��ֵ
			m_strBalanceWhiteAutoMode = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();
			GxIAPICPP::gxstring_vector vecBalanceWhiteAutoEnumCount;
			vecBalanceWhiteAutoEnumCount = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                                 ->GetEnumEntryList();

			//�жϰ�ƽ��ö��ֵ�Ƿ�Ϊoff
			if (m_strBalanceWhiteAutoMode == "Off")
			{
				for (uint32_t i = 0; i< vecBalanceWhiteAutoEnumCount.size(); i++)
				{
					pComBox->GetLBText(i,strText);
					if (strText == "Off")
					{
						// ѡ���Զ��ع�ؼ��е�Off��,����Once��ΪOff
						pComBox->SetCurSel(i);
						GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(TRUE);
						m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
						UpdateData(FALSE);
						break;
					}
				}
			}
		}
	}
	catch (CGalaxyException)
	{
		return;
	}
	catch (std::exception)
	{
		return;
	}
	CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------------
/**
\brief   ��ͼ�񱣴��BMPͼƬ
\param   ͼ�������

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::SavePicture(CImageDataPointer& objImageDataPointer)
{
	try
	{
		SYSTEMTIME   sysTime;                   ///< ϵͳʱ��
		CString      strFileName = "";          ///< ͼ�񱣴�·������
		CString      strFilePath = "";          ///< �ļ�·��
		strFilePath = m_strSavePath.c_str();

		//��������ͼ����ļ���
		BOOL bRet = CreateDirectory(strFilePath,NULL);

		//��ȡ��ǰʱ��Ϊͼ�񱣴��Ĭ������
		GetLocalTime(&sysTime);
		strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.bmp",strFilePath,  
			                                              sysTime.wYear,
			                                              sysTime.wMonth,
			                                              sysTime.wDay,
			                                              sysTime.wHour,
			                                              sysTime.wMinute,
			                                              sysTime.wSecond,
			                                              sysTime.wMilliseconds);

		//����ͼ��ΪBMP
		m_pBitmap->SaveBmp(objImageDataPointer,strFileName.GetBuffer(0));
	}
	catch (std::exception)
	{
		//���ڴ�ͼ�����̻߳ص���ʵ�ֵģ������ͼ�׳��쳣���ɼ��߳̽���ֹ��Ϊ�˱����߳���ֹ,��ͼ�����쳣��������
		return;
		
	}

}
BOOL CGxSingleCamDlg::PreTranslateMessage(MSG* pMsg)
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
		case IDC_EDIT_BALANCE_WHITE_SERISE:

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

	return CDialog::PreTranslateMessage(pMsg);
}
