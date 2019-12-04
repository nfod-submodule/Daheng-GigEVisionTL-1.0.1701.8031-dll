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
		//初始化库
		IGXFactory::GetInstance().Init();

		//获取图像显示窗口的指针
		m_pWnd = GetDlgItem(IDC_SHOW_PICTURE_STATIC);

		//为回调指针分配内存
		m_pSampleCaptureEventHandler = new CSampleCaptureEventHandler();

		//设置定时器更新界面
		SetTimer(0,1000,NULL);

		//更新界面
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
	bool bIsDeviceOpen = false;          ///< 设备是否已打开标识
	bool bIsStreamOpen = false;          ///< 设备流是否已打开标识

	try
	{
		//枚举设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("未发现设备!");
			return;
		}

		//打开设备
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen =true;
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//创建图像处理配置参数对象
		m_objImageProcessPtr = m_objDevicePtr->CreateImageProcessConfig();

		//是否支持Bayer格式
		bool m_bIsColorFilter = m_objFeatureControlPtr->IsImplemented("PixelColorFilter");

		//判断是否为彩色相机
		if (!m_bIsColorFilter)
		{
			throw exception("请插入彩色相机!");
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CGXBitmap(m_objDevicePtr, m_pWnd);

		//获取流通道个数
		int32_t nStreamCount = m_objDevicePtr->GetStreamCount();
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen  = true;
		}
		else
		{
			throw exception("未发现设备流!");
		}

		//初始化设备参数
		__InitParam();

        //初始化界面
		__InitUI();
		m_bIsOpen = true;

		//更新界面
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
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
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
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
\brief  初始化设备参数

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__InitParam()
{
	bool bBalanceWhiteAutoRead = false;         ///< 白平衡是否可读

	//设置触发模式为关
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

	//设置采集模式连续采集
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//是否支持自动白平衡
	m_bIsWhiteBalance = m_objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

	//白平衡是否可读
	bBalanceWhiteAutoRead   = m_objFeatureControlPtr->IsReadable("BalanceWhiteAuto");

	if (m_bIsWhiteBalance)
	{
		if (bBalanceWhiteAutoRead)
		{
			//获取当前白平衡的值
			m_strBalanceWhiteAutoValue = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                               ->GetValue();
		}
	}

	//是否支持白平衡光源
	m_bIsWhiteBalanceLight = m_objFeatureControlPtr->IsImplemented("AWBLampHouse");

}

//---------------------------------------------------------------------
/**
\brief  初始化UI界面

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::__InitUI()
{
	char chPutText[64] = {0};

	//以下常量定义参考《CPP软件开发说明书》第二章
	const double dSharpenParamMax    = 5;     ///< 锐化最大值 
	const double dSharpenParamMin    = 0.1;   ///< 锐化最小值
	const double dGammmaParamMax     = 10;    ///< Gammma最大值
	const double dGammaParamMin      = 0.1;   ///< Gamma最小值
	const int nContrastParamMax      = 100;   ///< 对比度最大值 
	const int nContrastParamMin      = -50;   ///< 对比度最小值
	const int nLightnessParamMax     = 150;   ///< 亮度最大值
	const int nLightnessParamMin     = -150;  ///< 亮度最小值
	const int nSaturationParamMax    = 128;   ///< 饱和度最大值 
	const int nSaturationParamMin    = 0;     ///< 饱和度最小值

	//初始化白平衡ComBox控件
	m_nComBoAWBOldValue = __InitEnumUI("BalanceWhiteAuto", 
		                               (CComboBox *)GetDlgItem(IDC_COMBO_AWB),
									    m_bIsWhiteBalance);

	//初始化白平衡光源ComBox控件
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

	//初始化Edit框
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

	//初始化Check框
	m_bCheckAcceleract   = m_objImageProcessPtr->IsAccelerate();
	m_bCheckPixelCorrect = m_objImageProcessPtr->IsDefectivePixelCorrect();
	m_bCheckDenoise      = m_objImageProcessPtr->IsDenoise();
	m_bCheckColorCorrect = m_objImageProcessPtr->IsColorCorrection();
	m_bCheckSharpen      = m_objImageProcessPtr->IsSharpen();

	UpdateData(FALSE);

}

//-----------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   strFeaturName  功能名称
\param   pComboBox      控件指针
\param   bIsImplemented 设备是否支持此功能
\return  -1 返回错误信息 其他值为返回的枚举值
*/
//-----------------------------------------------------------------------
int CGxImageProcessDlg::__InitEnumUI(gxstring strFeaturName, CComboBox *pComboBox, bool bIsImplemented)
{
	//判断控件
	if (pComboBox == NULL || (!bIsImplemented))
	{
		return -1;
	}
	GxIAPICPP::gxstring_vector vectorEnumEntryList;    ///< 枚举容器
	string strCurrentEnumEntryList = "";               ///< 当前枚举值
	int    nCursel = 0;
	CEnumFeaturePointer objEnumFeaturePtr = m_objFeatureControlPtr->GetEnumFeature(strFeaturName);

	//清除当前控件所有项
	pComboBox->ResetContent();

	//获取设备当前枚举值
	strCurrentEnumEntryList = objEnumFeaturePtr->GetValue().c_str();

	//获取设备所有枚举描述值
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
\brief  更新界面

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
		//判断是否已停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销采集回调函数
			m_objStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch(CGalaxyException)
	{
		//do nothing
	}

	try
	{
		//关闭流对象
		m_objStreamPtr->Close();
	}
	catch(CGalaxyException)
	{
		//do nothing
	}

	try
	{		
		//关闭设备
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

	//更新界面
	__UpdateUI();
}

void CGxImageProcessDlg::OnBnClickedStartSnap()
{
	// TODO: Add your control notification handler code here
    try
    {
		//注册采集回调
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandler, this);

		//在发送开采命令前必须先开启流层采集
		m_objStreamPtr->StartGrab(); 

		//获取一个命令型控制器并发送开采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;

		//更新界面
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
		//发送停止采集命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销采集回调函数
		m_objStreamPtr->UnregisterCaptureCallback();
		m_bIsSnap = false;

		//更新界面
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

		//获取枚举型控制器
		CEnumFeaturePointer objFeaturePtr = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto");

		//设置枚举当前值
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

		//获取枚举型控制器
		CEnumFeaturePointer objFeaturePtr = m_objFeatureControlPtr->GetEnumFeature("AWBLampHouse");

		//设置枚举当前值
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
	BOOL bAccelerateOld = m_bCheckAcceleract;      ///< 记录白平衡值
	bool bFlag          = false;                   ///< 用于判断图像高度是否为4的整数倍以及是否使能
	try
	{
		int64_t  nHeight = 0;        ///< 图像高度
		
        UpdateData(TRUE);

		//获取图像高度
		nHeight = m_objFeatureControlPtr->GetIntFeature("Height")->GetValue();
		bFlag = ((nHeight%4) != 0) && m_bCheckAcceleract;

		//判断图像高是否为4的整数倍
		if (bFlag)
		{
			MessageBox("使用CPU加速,图像高必须是4的整数倍!");
			m_bCheckAcceleract = FALSE;
			UpdateData(FALSE);
			return;
		}

		//使能算法加速
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

		//使能坏点校正
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

		//只能是彩色相机使能图像降噪
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

		//颜色校正只能是彩色相机
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

		//使能锐化
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
		//停止Timer
		KillTimer(0);

		//失去焦点
		SetFocus();

		//判断是否停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层通道
			m_objStreamPtr->StopGrab();

			//注销采集回调
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
		//判断设备是否已关闭
		if (m_bIsOpen)
		{
			//关闭流对象
			m_objStreamPtr->Close();

			//关闭设备
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
		//释放设备资源
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

		if (pScrollBar == (CScrollBar*)&m_sliderLightness)           //亮度滑动条      
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderLightness.GetPos());
			GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(PutText);
			m_objImageProcessPtr->SetLightnessParam(m_sliderLightness.GetPos());	
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderContrast)    //对比度滑动条
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderContrast.GetPos());
			GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(PutText);
			m_objImageProcessPtr->SetContrastParam(m_sliderContrast.GetPos());
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderSaturation)      //饱和度滑动条
		{
			sprintf_s(PutText,sizeof(PutText), "%d", m_sliderSaturation.GetPos());
			GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(PutText);
			m_objImageProcessPtr->SetSaturationParam(m_sliderSaturation.GetPos());
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderGamma)      //Gamma滑动条
		{
			sprintf_s(PutText,sizeof(PutText), "%f", (m_sliderGamma.GetPos()/PARAM_TRANSFORMATION_VALUE));
			GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(PutText);
			m_objImageProcessPtr->SetGammaParam(m_sliderGamma.GetPos()/PARAM_TRANSFORMATION_VALUE);
		}
		else if (pScrollBar == (CScrollBar*)&m_sliderSharpen)    //锐化滑动条
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
\brief  图像处理

\param  objImageDataPointer 图像信息   

\return void
*/
//---------------------------------------------------------------------
void CGxImageProcessDlg::ShowImageProcess(CImageDataPointer& objImageDataPointer)
{
	//首先配置图像处理参数以免造成图像处理耗时，先刷界面卡死
	m_objImageProcessPtr->SetValidBit(m_pBitmap->GetBestValudBit(objImageDataPointer->GetPixelFormat()));

	//当前图像质量提升
	m_pBitmap->ShowImageProcess(m_objImageProcessPtr,objImageDataPointer);
}

//---------------------------------------------------------------------
/**
\brief  改变Combox下拉框的宽度

\param  pComBox 组合框句柄   

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

	//改变组合框的宽度
	__ChangeComBoxWidth((CComboBox*)GetDlgItem(IDC_COMBO_AWB));
}

void CGxImageProcessDlg::OnCbnDropdownComboAwbLight()
{
	// TODO: Add your control notification handler code here

	//改变组合框的宽度
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

		// 如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
		// 为与设备状态保持一致程序以代码模拟该过程：设置为Once后随即将界面更新为off
		if (m_strBalanceWhiteAutoValue == "Once")
		{
			//获取自动白平衡枚举值
			m_strBalanceWhiteAutoValue = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue().c_str();
			GxIAPICPP::gxstring_vector vecBalanceWhiteAutoEnumCount;
			vecBalanceWhiteAutoEnumCount = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetEnumEntryList();

			//判断白平衡枚举值是否为off
			if (m_strBalanceWhiteAutoValue == "Off")
			{
				for (uint32_t i = 0; i< vecBalanceWhiteAutoEnumCount.size(); i++)
				{
					pComBox->GetLBText(i,strText);
					if (strText == "Off")
					{
						// 选中自动曝光控件中的OFF项,即由ONCE变为OFF
						pComBox->SetCurSel(i);
						break;
					}
				}
			}
		}

	}
	catch (CGalaxyException)
	{
		//Timer不抛异常	
	}
	catch (std::exception)
	{
		//Timer不抛异常	
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CGxImageProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd  *pWnd   = NULL;
	int   nCtrlID = 0;             //< 保存获取的控件ID

	//判断是否是键盘回车消息
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获得当前焦点的控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//失去焦点
		SetFocus();

		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}   

	return CDialog::PreTranslateMessage(pMsg);
}

