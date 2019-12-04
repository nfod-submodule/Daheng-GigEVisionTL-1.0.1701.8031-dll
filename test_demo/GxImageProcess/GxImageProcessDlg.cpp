// GxImageProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "GxImageProcess.h"
#include "GxImageProcessDlg.h"
#include "FileVersion.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProtect_Version;
	CString m_strLegalCopyRight;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strProtect_Version(_T(""))
, m_strLegalCopyRight(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProtect_Version);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CGxImageProcessDlg dialog
CGxImageProcessDlg::CGxImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxImageProcessDlg::IDD, pParent)
	, m_bCheckAcceleract(FALSE)
	, m_bCheckPixelCorrect(FALSE)
	, m_bCheckDenoise(FALSE)
	, m_bCheckColorCorrect(FALSE)
	, m_bCheckSharpen(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);              

	m_bIsOpen                    = false;
	m_bIsSnap                    = false;
	m_bIsWhiteBalance            = false;
	m_bIsWhiteBalanceLight       = false;
	m_pWnd                       = NULL;
	m_pBitmap                    = NULL;
	m_pSampleCaptureEventHandler = NULL;
	m_nComBoAWBOldValue          = 0;
	m_nComBoAWBLightOldValue     = 0;
	m_nOldSliderContrastValue    = 0;
	m_nOldSliderGammaValue       = 0;
	m_nOldSliderLightnessValue   = 0;
	m_nOldSliderSaturationValue  = 0;
	m_nOldSliderSharpenValue     = 0;
	m_strBalanceWhiteAutoValue   = "Off";
	
}

void CGxImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SHARPEN, m_sliderSharpen);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContrast);
	DDX_Control(pDX, IDC_SLIDER_LIGHTNESS, m_sliderLightness);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_sliderSaturation);
	DDX_Check(pDX, IDC_AIDC_OPEN_ACCELERATECCELERATE_CHECK, m_bCheckAcceleract);
	DDX_Check(pDX, IDC_OPEN_BAD_PIXEL_CORRECT, m_bCheckPixelCorrect);
	DDX_Check(pDX, IDC_OPEN_DENOISE, m_bCheckDenoise);
	DDX_Check(pDX, IDC_OPEN_COLOR_CORRECT, m_bCheckColorCorrect);
	DDX_Check(pDX, IDC_OPEN_SHARPEN, m_bCheckSharpen);
}

BEGIN_MESSAGE_MAP(CGxImageProcessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

ON_WM_CLOSE()
//ON_WM_HSCROLL()
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SHARPEN_SLIDER, &CGxImageProcessDlg::OnNMCustomdrawSharpenSlider)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_OPEN_DEVICE, &CGxImageProcessDlg::OnBnClickedOpenDevice)
ON_BN_CLICKED(IDC_CLOSE_DEVICE, &CGxImageProcessDlg::OnBnClickedCloseDevice)
ON_BN_CLICKED(IDC_START_SNAP, &CGxImageProcessDlg::OnBnClickedStartSnap)
ON_BN_CLICKED(IDC_STOP_SNAP, &CGxImageProcessDlg::OnBnClickedStopSnap)
ON_CBN_SELCHANGE(IDC_COMBO_AWB, &CGxImageProcessDlg::OnCbnSelchangeComboAwb)
ON_CBN_SELCHANGE(IDC_COMBO_AWB_LIGHT, &CGxImageProcessDlg::OnCbnSelchangeComboAwbLight)
ON_BN_CLICKED(IDC_AIDC_OPEN_ACCELERATECCELERATE_CHECK, &CGxImageProcessDlg::OnBnClickedAidcOpenAccelerateccelerateCheck)
ON_BN_CLICKED(IDC_OPEN_BAD_PIXEL_CORRECT, &CGxImageProcessDlg::OnBnClickedOpenBadPixelCorrect)
ON_BN_CLICKED(IDC_OPEN_DENOISE, &CGxImageProcessDlg::OnBnClickedOpenDenoise)
ON_BN_CLICKED(IDC_OPEN_COLOR_CORRECT, &CGxImageProcessDlg::OnBnClickedOpenColorCorrect)
ON_BN_CLICKED(IDC_OPEN_SHARPEN, &CGxImageProcessDlg::OnBnClickedOpenSharpen)
ON_CBN_DROPDOWN(IDC_COMBO_AWB, &CGxImageProcessDlg::OnCbnDropdownComboAwb)
ON_CBN_DROPDOWN(IDC_COMBO_AWB_LIGHT, &CGxImageProcessDlg::OnCbnDropdownComboAwbLight)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CGxImageProcessDlg message handlers

BOOL CGxImageProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	try
	{
		//��ʼ����
		IGXFactory::GetInstance().Init();

		//��ȡͼ����ʾ���ڵ�ָ��
		m_pWnd = GetDlgItem(IDC_SHOW_PICTURE_STATIC);

		//Ϊ�ص�ָ������ڴ�
		m_pSampleCaptureEventHandler = new CSampleCaptureEventHandler();

		//���ö�ʱ�����½���
		SetTimer(0,1000,NULL);

		//���½���
		__UpdateUI();

	}
	catch (CGalaxyException& e)
	{
		if (m_pSampleCaptureEventHandler != NULL)
		{
			delete m_pSampleCaptureEventHandler;
			m_pSampleCaptureEventHandler = NULL;
		}
		MessageBox(e.what());
		return FALSE;
	}
	catch (std::exception& e)
	{
		if (m_pSampleCaptureEventHandler != NULL)
		{
			delete m_pSampleCaptureEventHandler;
			m_pSampleCaptureEventHandler = NULL;
		}
		MessageBox(e.what());
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char chFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL,(LPTSTR)chFileName, MAX_PATH);
		CString strFileName = "";
		strFileName.Format("%s",chFileName);
		CFileVersion fv(chFileName);
		CAboutDlg dlgAbout;
		dlgAbout.m_strProtect_Version = _T("Version: ") + fv.GetProductVersion();
		dlgAbout.m_strLegalCopyRight = fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGxImageProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGxImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGxImageProcessDlg::OnBnClickedOpenDevice()
{
	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen = false;          ///< �豸�Ƿ��Ѵ򿪱�ʶ
	bool bIsStreamOpen = false;          ///< �豸���Ƿ��Ѵ򿪱�ʶ

	try
	{
		//ö���豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("δ�����豸!");
			return;
		}

		//���豸
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen =true;
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//����ͼ�������ò�������
		m_objImageProcessPtr = m_objDevicePtr->CreateImageProcessConfig();

		//�Ƿ�֧��Bayer��ʽ
		bool m_bIsColorFilter = m_objFeatureControlPtr->IsImplemented("PixelColorFilter");

		//�ж��Ƿ�Ϊ��ɫ���
		if (!m_bIsColorFilter)
		{
			throw exception("������ɫ���!");
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CGXBitmap(m_objDevicePtr, m_pWnd);

		//��ȡ��ͨ������
		int32_t nStreamCount = m_objDevicePtr->GetStreamCount();
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen  = true;
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

//---------------------------------------------------------------------
/**
\brief  ��ʼ���豸����

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__InitParam()
{
	bool bBalanceWhiteAutoRead = false;         ///< ��ƽ���Ƿ�ɶ�

	//���ô���ģʽΪ��
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

	//���òɼ�ģʽ�����ɼ�
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//�Ƿ�֧���Զ���ƽ��
	m_bIsWhiteBalance = m_objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

	//��ƽ���Ƿ�ɶ�
	bBalanceWhiteAutoRead   = m_objFeatureControlPtr->IsReadable("BalanceWhiteAuto");

	if (m_bIsWhiteBalance)
	{
		if (bBalanceWhiteAutoRead)
		{
			//��ȡ��ǰ��ƽ���ֵ
			m_strBalanceWhiteAutoValue = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                               ->GetValue();
		}
	}

	//�Ƿ�֧�ְ�ƽ���Դ
	m_bIsWhiteBalanceLight = m_objFeatureControlPtr->IsImplemented("AWBLampHouse");

}

//---------------------------------------------------------------------
/**
\brief  ��ʼ��UI����

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__InitUI()
{
	char chPutText[64] = {0};

	//���³�������ο���CPP�������˵���顷�ڶ���
	const double dSharpenParamMax    = 5;     ///< �����ֵ 
	const double dSharpenParamMin    = 0.1;   ///< ����Сֵ
	const double dGammmaParamMax     = 10;    ///< Gammma���ֵ
	const double dGammaParamMin      = 0.1;   ///< Gamma��Сֵ
	const int nContrastParamMax      = 100;   ///< �Աȶ����ֵ 
	const int nContrastParamMin      = -50;   ///< �Աȶ���Сֵ
	const int nLightnessParamMax     = 150;   ///< �������ֵ
	const int nLightnessParamMin     = -150;  ///< ������Сֵ
	const int nSaturationParamMax    = 128;   ///< ���Ͷ����ֵ 
	const int nSaturationParamMin    = 0;     ///< ���Ͷ���Сֵ

	//��ʼ����ƽ��ComBox�ؼ�
	m_nComBoAWBOldValue = __InitEnumUI("BalanceWhiteAuto", 
		                               (CComboBox *)GetDlgItem(IDC_COMBO_AWB),
									    m_bIsWhiteBalance);

	//��ʼ����ƽ���ԴComBox�ؼ�
    m_nComBoAWBLightOldValue = __InitEnumUI("AWBLampHouse",
		                                     (CComboBox *)GetDlgItem(IDC_COMBO_AWB_LIGHT),
											  m_bIsWhiteBalanceLight);

	m_sliderSharpen.SetRange(int(dSharpenParamMin * PARAM_TRANSFORMATION_VALUE),
		                      int(dSharpenParamMax * PARAM_TRANSFORMATION_VALUE),TRUE);

	m_sliderGamma.SetRange(int(dGammaParamMin * PARAM_TRANSFORMATION_VALUE),
		                    int(dGammmaParamMax * PARAM_TRANSFORMATION_VALUE),TRUE);

	m_sliderContrast.SetRange(nContrastParamMin,nContrastParamMax,TRUE);
	m_sliderLightness.SetRange(nLightnessParamMin,nLightnessParamMax,TRUE);
	m_sliderSaturation.SetRange(nSaturationParamMin,nSaturationParamMax,TRUE);
	m_sliderSharpen.SetPos(int(m_objImageProcessPtr->GetSharpenParam()*PARAM_TRANSFORMATION_VALUE));
	m_sliderGamma.SetPos(int(m_objImageProcessPtr->GetGammaParam()*PARAM_TRANSFORMATION_VALUE));
	m_sliderContrast.SetPos(m_objImageProcessPtr->GetContrastParam());
	m_sliderLightness.SetPos(m_objImageProcessPtr->GetLightnessParam());
	m_sliderSaturation.SetPos(m_objImageProcessPtr->GetSaturationParam());

	//��ʼ��Edit��
	sprintf_s(chPutText,sizeof(chPutText),"%f",m_objImageProcessPtr->GetSharpenParam());
	GetDlgItem(IDC_SHARPEN_EDIT)->SetWindowText(chPutText);
	sprintf_s(chPutText,sizeof(chPutText),"%f",m_objImageProcessPtr->GetGammaParam());
	GetDlgItem(IDC_GAMMA_EDIT)->SetWindowText(chPutText);
	sprintf_s(chPutText,sizeof(chPutText),"%d",m_objImageProcessPtr->GetContrastParam());
	GetDlgItem(IDC_CONTRASE_EDIT)->SetWindowText(chPutText);
	sprintf_s(chPutText,sizeof(chPutText),"%d",m_objImageProcessPtr->GetLightnessParam());
	GetDlgItem(IDC_BRIGHT_EDIT)->SetWindowText(chPutText);
	sprintf_s(chPutText,sizeof(chPutText),"%d",m_objImageProcessPtr->GetSaturationParam());
	GetDlgItem(IDC_SATURA_EDIT)->SetWindowText(chPutText);

	//��ʼ��Check��
	m_bCheckAcceleract   = m_objImageProcessPtr->IsAccelerate();
	m_bCheckPixelCorrect = m_objImageProcessPtr->IsDefectivePixelCorrect();
	m_bCheckDenoise      = m_objImageProcessPtr->IsDenoise();
	m_bCheckColorCorrect = m_objImageProcessPtr->IsColorCorrection();
	m_bCheckSharpen      = m_objImageProcessPtr->IsSharpen();

	UpdateData(FALSE);

}

//-----------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   strFeaturName  ��������
\param   pComboBox      �ؼ�ָ��
\param   bIsImplemented �豸�Ƿ�֧�ִ˹���
\return  -1 ���ش�����Ϣ ����ֵΪ���ص�ö��ֵ
*/
//-----------------------------------------------------------------------
int CGxImageProcessDlg::__InitEnumUI(gxstring strFeaturName, CComboBox *pComboBox, bool bIsImplemented)
{
	//�жϿؼ�
	if (pComboBox == NULL || (!bIsImplemented))
	{
		return -1;
	}
	GxIAPICPP::gxstring_vector vectorEnumEntryList;    ///< ö������
	string strCurrentEnumEntryList = "";               ///< ��ǰö��ֵ
	int    nCursel = 0;
	CEnumFeaturePointer objEnumFeaturePtr = m_objFeatureControlPtr->GetEnumFeature(strFeaturName);

	//�����ǰ�ؼ�������
	pComboBox->ResetContent();

	//��ȡ�豸��ǰö��ֵ
	strCurrentEnumEntryList = objEnumFeaturePtr->GetValue().c_str();

	//��ȡ�豸����ö������ֵ
	vectorEnumEntryList = objEnumFeaturePtr->GetEnumEntryList();
	for (uint32_t i = 0; i<vectorEnumEntryList.size(); i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(vectorEnumEntryList[i].c_str()), i);
		if (strCurrentEnumEntryList == vectorEnumEntryList[i].c_str())
		{
			nCursel = i;
		}
	}
	pComboBox->SetCurSel(nCursel);
	return nCursel;
}

//---------------------------------------------------------------------
/**
\brief  ���½���

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && !m_bIsSnap);
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bIsOpen && m_bIsWhiteBalance);
	GetDlgItem(IDC_COMBO_AWB_LIGHT)->EnableWindow(m_bIsOpen && m_bIsWhiteBalanceLight);
	GetDlgItem(IDC_AIDC_OPEN_ACCELERATECCELERATE_CHECK)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_OPEN_DENOISE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_OPEN_COLOR_CORRECT)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_OPEN_SHARPEN)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_SHARPEN)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_CONTRAST)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_LIGHTNESS)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(m_bIsOpen);
}

void CGxImageProcessDlg::OnBnClickedCloseDevice()
{
	// TODO: Add your control notification handler code here
	try
	{
		//�ж��Ƿ���ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ɼ��ص�����
			m_objStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch(CGalaxyException)
	{
		//do nothing
	}

	try
	{
		//�ر�������
		m_objStreamPtr->Close();
	}
	catch(CGalaxyException)
	{
		//do nothing
	}

	try
	{		
		//�ر��豸
		m_objDevicePtr->Close();
		
	}
	catch (CGalaxyException)
	{
		//do nothing
	}

	m_bIsOpen = false;
	m_bIsSnap = false;

	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	//���½���
	__UpdateUI();
}

void CGxImageProcessDlg::OnBnClickedStartSnap()
{
	// TODO: Add your control notification handler code here
    try
    {
		//ע��ɼ��ص�
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandler, this);

		//�ڷ��Ϳ�������ǰ�����ȿ�������ɼ�
		m_objStreamPtr->StartGrab(); 

		//��ȡһ�������Ϳ����������Ϳ�������
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

void CGxImageProcessDlg::OnBnClickedStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//����ֹͣ�ɼ�����
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ɼ��ص�����
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

void CGxImageProcessDlg::OnCbnSelchangeComboAwb()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombox = NULL;
	try
	{
		int         nIndex            = 0;
		CString     strCurrentContent = "";
		gxstring    strCurrentText    = "";
		pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_AWB);
		nIndex = pCombox->GetCurSel();
		pCombox->GetLBText(nIndex,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//��ȡö���Ϳ�����
		CEnumFeaturePointer objFeaturePtr = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto");

		//����ö�ٵ�ǰֵ
		objFeaturePtr->SetValue(strCurrentText);
		m_nComBoAWBOldValue = nIndex;
		m_strBalanceWhiteAutoValue = strCurrentText;
	}
	catch (CGalaxyException& e)
	{
		pCombox->SetCurSel(m_nComBoAWBOldValue);
		MessageBox(e.what());
		return;
	}
	catch (std::exception& e)
	{
		pCombox->SetCurSel(m_nComBoAWBOldValue);
		MessageBox(e.what());
		return;
	}
}

void CGxImageProcessDlg::OnCbnSelchangeComboAwbLight()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombox = NULL;
	try
	{
		int         nIndex            = 0;
		CString     strCurrentContent = "";
		gxstring    strCurrentText    = "";
		pCombox = (CComboBox*)GetDlgItem(IDC_COMBO_AWB_LIGHT);
		nIndex = pCombox->GetCurSel();
		pCombox->GetLBText(nIndex,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//��ȡö���Ϳ�����
		CEnumFeaturePointer objFeaturePtr = m_objFeatureControlPtr->GetEnumFeature("AWBLampHouse");

		//����ö�ٵ�ǰֵ
		objFeaturePtr->SetValue(strCurrentText);
		m_nComBoAWBLightOldValue = nIndex;
	}
	catch (CGalaxyException &e)
	{
		pCombox->SetCurSel(m_nComBoAWBLightOldValue);
		MessageBox(e.what());
		return;
	}
	catch (std::exception &e)
	{
		pCombox->SetCurSel(m_nComBoAWBLightOldValue);
		MessageBox(e.what());
		return;
	}
}

void CGxImageProcessDlg::OnBnClickedAidcOpenAccelerateccelerateCheck()
{
	// TODO: Add your control notification handler code here
	BOOL bAccelerateOld = m_bCheckAcceleract;      ///< ��¼��ƽ��ֵ
	bool bFlag          = false;                   ///< �����ж�ͼ��߶��Ƿ�Ϊ4���������Լ��Ƿ�ʹ��
	try
	{
		int64_t  nHeight = 0;        ///< ͼ��߶�
		
        UpdateData(TRUE);

		//��ȡͼ��߶�
		nHeight = m_objFeatureControlPtr->GetIntFeature("Height")->GetValue();
		bFlag = ((nHeight%4) != 0) && m_bCheckAcceleract;

		//�ж�ͼ����Ƿ�Ϊ4��������
		if (bFlag)
		{
			MessageBox("ʹ��CPU����,ͼ��߱�����4��������!");
			m_bCheckAcceleract = FALSE;
			UpdateData(FALSE);
			return;
		}

		//ʹ���㷨����
		m_objImageProcessPtr->EnableAccelerate(m_bCheckAcceleract == TRUE);

	}
	catch (CGalaxyException& e)
	{

		m_bCheckAcceleract = bAccelerateOld;
		UpdateData(FALSE);
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_bCheckAcceleract = bAccelerateOld;
		UpdateData(FALSE);
		MessageBox(e.what());
	}
   
	
}

void CGxImageProcessDlg::OnBnClickedOpenBadPixelCorrect()
{
	// TODO: Add your control notification handler code here
	BOOL bBadPixelCorrectOld = m_bCheckPixelCorrect;
	try
	{
		UpdateData(TRUE);

		//ʹ�ܻ���У��
		m_objImageProcessPtr->EnableDefectivePixelCorrect(m_bCheckPixelCorrect == TRUE);
	}
	catch (CGalaxyException &e)
	{
		m_bCheckPixelCorrect = bBadPixelCorrectOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
	catch (std::exception &e)
	{
		m_bCheckPixelCorrect = bBadPixelCorrectOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}


}

void CGxImageProcessDlg::OnBnClickedOpenDenoise()
{
	// TODO: Add your control notification handler code here
	BOOL bDenoise = m_bCheckDenoise;
	try
	{
		UpdateData(TRUE);

		//ֻ���ǲ�ɫ���ʹ��ͼ����
		m_objImageProcessPtr->EnableDenoise(m_bCheckDenoise == TRUE);
	}
	catch (CGalaxyException &e)
	{
		m_bCheckDenoise = bDenoise;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
	catch (std::exception &e)
	{
		m_bCheckDenoise = bDenoise;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
}

void CGxImageProcessDlg::OnBnClickedOpenColorCorrect()
{
	// TODO: Add your control notification handler code here
	BOOL bColorCorrectOld = m_bCheckColorCorrect;
	try
	{
		UpdateData(TRUE);

		//��ɫУ��ֻ���ǲ�ɫ���
		m_objImageProcessPtr->EnableColorCorrection(m_bCheckColorCorrect == TRUE);
	}
	catch (CGalaxyException &e)
	{
		m_bCheckColorCorrect = bColorCorrectOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
	catch (std::exception &e)
	{
		m_bCheckColorCorrect = bColorCorrectOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
	
}

void CGxImageProcessDlg::OnBnClickedOpenSharpen()
{
	// TODO: Add your control notification handler code here
	BOOL bSharppenOld = m_bCheckSharpen;
	try
	{
		UpdateData(TRUE);

		//ʹ����
		m_objImageProcessPtr->EnableSharpen(m_bCheckSharpen == TRUE);
	}
	catch (CGalaxyException &e)
	{
		m_bCheckSharpen = bSharppenOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
	catch (std::exception &e)
	{
		m_bCheckSharpen = bSharppenOld;
		UpdateData(FALSE);
		MessageBox(e.what());
		return;  	
	}
}

void CGxImageProcessDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		//ֹͣTimer
		KillTimer(0);

		//ʧȥ����
		SetFocus();

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
		//�ж��豸�Ƿ��ѹر�
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

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	if (NULL != m_pSampleCaptureEventHandler)
	{
		delete m_pSampleCaptureEventHandler;
		m_pSampleCaptureEventHandler = NULL;
	}

	CDialog::OnClose();	
}

void CGxImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		char PutText[16] = {0};

		if (pScrollBar == (CScrollBar*)&m_sliderLightness)           //���Ȼ�����      
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderLightness.GetPos());
			GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(PutText);
			m_objImageProcessPtr->SetLightnessParam(m_sliderLightness.GetPos());	
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderContrast)    //�ԱȶȻ�����
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderContrast.GetPos());
			GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(PutText);
			m_objImageProcessPtr->SetContrastParam(m_sliderContrast.GetPos());
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderSaturation)      //���ͶȻ�����
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderSaturation.GetPos());
			GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(PutText);
			m_objImageProcessPtr->SetSaturationParam(m_sliderSaturation.GetPos());
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderGamma)      //Gamma������
		{
			sprintf_s(PutText,sizeof(PutText), "%f", (m_sliderGamma.GetPos()/PARAM_TRANSFORMATION_VALUE));
			GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(PutText);
			m_objImageProcessPtr->SetGammaParam(m_sliderGamma.GetPos()/PARAM_TRANSFORMATION_VALUE);
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderSharpen)    //�񻯻�����
		{
			sprintf_s(PutText,sizeof(PutText), "%f", (m_sliderSharpen.GetPos()/PARAM_TRANSFORMATION_VALUE));
			GetDlgItem(IDC_EDIT_SHARPEN)->SetWindowText(PutText);
			m_objImageProcessPtr->SetSharpenParam(m_sliderSharpen.GetPos()/PARAM_TRANSFORMATION_VALUE);
		}

		m_nOldSliderLightnessValue   = m_sliderLightness.GetPos();
		m_nOldSliderContrastValue    = m_sliderContrast.GetPos();
		m_nOldSliderSaturationValue  = m_sliderSaturation.GetPos();
		m_nOldSliderGammaValue       = m_sliderGamma.GetPos();
		m_nOldSliderSharpenValue     = m_sliderSharpen.GetPos();

	}
	catch (CGalaxyException &e)
	{
		m_sliderSharpen.SetPos(m_nOldSliderSharpenValue);
		m_sliderGamma.SetPos(m_nOldSliderGammaValue);
		m_sliderLightness.SetPos(m_nOldSliderLightnessValue);
		m_sliderContrast.SetPos(m_nOldSliderContrastValue);
		m_sliderSaturation.SetPos(m_nOldSliderSaturationValue);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		m_sliderSharpen.SetPos(m_nOldSliderSharpenValue);
		m_sliderGamma.SetPos(m_nOldSliderGammaValue);
		m_sliderLightness.SetPos(m_nOldSliderLightnessValue);
		m_sliderContrast.SetPos(m_nOldSliderContrastValue);
		m_sliderSaturation.SetPos(m_nOldSliderSaturationValue);
		MessageBox(e.what());
		return;	
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//---------------------------------------------------------------------
/**
\brief  ͼ����

\param  objImageDataPointer ͼ����Ϣ   

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::ShowImageProcess(CImageDataPointer& objImageDataPointer)
{
	//��������ͼ��������������ͼ�����ʱ����ˢ���濨��
	m_objImageProcessPtr->SetValidBit(m_pBitmap->GetBestValudBit(objImageDataPointer->GetPixelFormat()));

	//��ǰͼ����������
	m_pBitmap->ShowImageProcess(m_objImageProcessPtr,objImageDataPointer);
}

//---------------------------------------------------------------------
/**
\brief  �ı�Combox������Ŀ��

\param  pComBox ��Ͽ���   

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__ChangeComBoxWidth(CComboBox* pComBox)
{
	int nNumEntries = pComBox->GetCount();
	int nWidth = 0;
	CString str;

	CClientDC dc(pComBox);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());

	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	for (int i = 0; i < nNumEntries; i++)
	{
		pComBox->GetLBText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = max(nWidth, nLength);
	}

	// Add margin space to the calculations
	nWidth += dc.GetTextExtent("0").cx;

	dc.RestoreDC(nSave);
	pComBox->SetDroppedWidth(nWidth);

}

void CGxImageProcessDlg::OnCbnDropdownComboAwb()
{
	// TODO: Add your control notification handler code here

	//�ı���Ͽ�Ŀ��
	__ChangeComBoxWidth((CComboBox*)GetDlgItem(IDC_COMBO_AWB));
}

void CGxImageProcessDlg::OnCbnDropdownComboAwbLight()
{
	// TODO: Add your control notification handler code here

	//�ı���Ͽ�Ŀ��
	__ChangeComBoxWidth((CComboBox*)GetDlgItem(IDC_COMBO_AWB_LIGHT));
}

void CGxImageProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		CString strText    = "";
		int32_t i          = 0;
		CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_AWB);

		// ����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
		// Ϊ���豸״̬����һ�³����Դ���ģ��ù��̣�����ΪOnce���漴���������Ϊoff
		if (m_strBalanceWhiteAutoValue == "Once")
		{
			//��ȡ�Զ���ƽ��ö��ֵ
			m_strBalanceWhiteAutoValue = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue().c_str();
			GxIAPICPP::gxstring_vector vecBalanceWhiteAutoEnumCount;
			vecBalanceWhiteAutoEnumCount = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetEnumEntryList();

			//�жϰ�ƽ��ö��ֵ�Ƿ�Ϊoff
			if (m_strBalanceWhiteAutoValue == "Off")
			{
				for (uint32_t i = 0; i< vecBalanceWhiteAutoEnumCount.size(); i++)
				{
					pComBox->GetLBText(i,strText);
					if (strText == "Off")
					{
						// ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
						pComBox->SetCurSel(i);
						break;
					}
				}
			}
		}

	}
	catch (CGalaxyException)
	{
		//Timer�����쳣	
	}
	catch (std::exception)
	{
		//Timer�����쳣	
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CGxImageProcessDlg::PreTranslateMessage(MSG* pMsg)
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

		//ʧȥ����
		SetFocus();

		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}   

	return CDialog::PreTranslateMessage(pMsg);
}

