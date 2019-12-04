// GxCameraEventsDlg.cpp : ʵ���ļ�
//----------------------------------------------------------------------------------
/**
\brief  CGxCameraEventsDlg class implementation
*/
//----------------------------------------------------------------------------------

#include "stdafx.h"
#include "GxCameraEvents.h"
#include "GxCameraEventsDlg.h"
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

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProductVersion;
	CString m_strLegalCopyRight;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strProductVersion(_T(""))
, m_strLegalCopyRight(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CGxCameraEventsDlg �Ի���
CGxCameraEventsDlg::CGxCameraEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxCameraEventsDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_hCB(NULL)
	, m_pLogWnd(NULL)
	, m_pCaptureEventHandle(NULL)
	, m_pFeatureEventHandle(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}
void CGxCameraEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGxCameraEventsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_DEVICE, &CGxCameraEventsDlg::OnBnClickedOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, &CGxCameraEventsDlg::OnBnClickedStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, &CGxCameraEventsDlg::OnBnClickedStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, &CGxCameraEventsDlg::OnBnClickedCloseDevice)
	ON_BN_CLICKED(IDC_SEND_SOFT_COMMAND, &CGxCameraEventsDlg::OnBnClickedSendSoftCommand)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CGxCameraEventsDlg ��Ϣ�������
BOOL CGxCameraEventsDlg::OnInitDialog()
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
	// ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	try
	{
		//��ʼ����
		IGXFactory::GetInstance().Init();

		//ָ��ʱ����ʾ����ָ��
		m_pLogWnd =GetDlgItem(IDC_EDIT_TIME_LOG);

		//���½���
		__UpdateUI();
	    
		m_pCaptureEventHandle = new CSampleCaptureEventHandle();
		m_pFeatureEventHandle = new CSampleFeatureEventHandle();
	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return FALSE;
		
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return FALSE;

	}

	//���ǽ��������õ��ؼ������򷵻� TRUE
	return TRUE;  
}

void CGxCameraEventsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char chFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL,chFileName,MAX_PATH);
		CFileVersion fv(chFileName);
		CAboutDlg dlgAbout;
		dlgAbout.m_strProductVersion = _T("Version:") + fv.GetProductVersion();
		dlgAbout.m_strLegalCopyRight =fv.GetLegalCopyright();
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
void CGxCameraEventsDlg::OnPaint()
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
HCURSOR CGxCameraEventsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGxCameraEventsDlg::OnBnClickedOpenDevice()
{
	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen              = false;       ///< �豸�Ƿ��Ѵ򿪱�־
	bool bIsStreamOpen              = false;       ///< �豸���Ƿ��Ѵ򿪱�־
	bool m_bSupportExposureEndEvent = false;       ///< �Ƿ�֧���ع������־

    try
    {
		//ö���豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("δ�����豸!");
			return;
		}

		//���豸
		m_objDevicePtr =IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(),GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//��ȡ���Կ��������� 
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//�Ƿ�֧���ع�����¼�
		m_bSupportExposureEndEvent = __IsSupportExposureEnd();

		if (!m_bSupportExposureEndEvent)
		{
			throw exception("�����֧���ع�����¼�,��ʾ����Ը������Ч!");
		}

		//��ȡ��ͨ������
		uint32_t nStreamCount =m_objDevicePtr->GetStreamCount();

		//����
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen  = true;
		}
		else
		{
			throw exception("δ�����豸��!");
		}

		//��ʼ���������
		__InitParam();

		m_bIsOpen = true ;

		//���½���
		__UpdateUI();
    }
    catch (CGalaxyException &e)
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

		MessageBox(e.what());
		return;
	}
}

void CGxCameraEventsDlg::OnBnClickedStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//ע���ع�����¼��ص�����
		m_hCB = m_objFeatureControlPtr->RegisterFeatureCallback("EventExposureEnd",
			                                                     m_pFeatureEventHandle, this);
		
		//ע��ɼ��ص�����
		m_objStreamPtr->RegisterCaptureCallback(m_pCaptureEventHandle, this);

		//��������ɼ�
		m_objStreamPtr->StartGrab();

		//���Ϳ�ʼ�ɼ�����
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;

		//���½���
		__UpdateUI();

	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxCameraEventsDlg::OnBnClickedStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//����ͣ������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ع�����¼��ص�����
		m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

		//ע���ɼ��ص�����
		m_objStreamPtr->UnregisterCaptureCallback();
		m_bIsSnap = false;

		//���½���
		__UpdateUI();

	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxCameraEventsDlg::OnBnClickedCloseDevice()
{
	// TODO: Add your control notification handler code here
	try
	{
		//�ж��Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{
			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ɼ��ص�����
			m_objStreamPtr->UnregisterCaptureCallback();

			//ע���ع�����¼��ص�����
			m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

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
}

void CGxCameraEventsDlg::OnBnClickedSendSoftCommand()
{
	// TODO: Add your control notification handler code here
	try
	{
		SYSTEMTIME    system;
		CString       strTime = "";      ///< ʱ���ַ���

		//������������
		m_objFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();

		//��ȡ������������ʱ��
		GetLocalTime(&system);
		strTime.Format("%02dh:%0.2dm:%0.2ds:%03dms ",
			           system.wHour,system.wMinute,system.wSecond,system.wMilliseconds);

		//��ӡʱ��
		CString strInformation = "\r\n���������������ʱ���:" + strTime;
		std::string strPrintInformation = strInformation.GetBuffer(0);
		PrintInfotoWnd(strPrintInformation);		

		//�յ�ͼ����ʱʱ�䣺��ʼ��ʱ
		m_objImageTime.Start();

		//�ع�����¼���ʱʱ�䣺��ʼ��ʱ
		m_objShutterTime.Start();
	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxCameraEventsDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		if (m_bIsSnap)
		{
			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ع�����¼��ص�����
			m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

			//ע���ɼ��ص�����
			m_objStreamPtr->UnregisterCaptureCallback();

			m_bIsSnap = false;

		}
	}
	catch(CGalaxyException)
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
	catch(CGalaxyException)
	{
		//do noting
	}

	try
	{
		//�ͷ��豸��Դ
		IGXFactory::GetInstance().Uninit();

	}
	catch(CGalaxyException)
	{
		//do noting
	}

	if (m_pCaptureEventHandle != NULL)
	{
		delete m_pCaptureEventHandle;
		m_pCaptureEventHandle = NULL;
	}

	if (m_pFeatureEventHandle != NULL)
	{
		delete m_pFeatureEventHandle;
		m_pFeatureEventHandle = NULL;
	}

    //�رս���ǰ�ͷ��豸��Դ
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ���豸

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::__InitParam()
{
	//���òɼ�ģʽΪ�����ɼ�
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//���ô���ģʽΪ��
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");

	//���ô���ԴΪ����
	m_objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Software");

	//�����¼�Ϊ�ع�����¼�
	m_objFeatureControlPtr->GetEnumFeature("EventSelector")->SetValue("ExposureEnd");

	//���¼�ʹ��
	m_objFeatureControlPtr->GetEnumFeature("EventNotification")->SetValue("On");   

}

//----------------------------------------------------------------------------------
/**
\brief  �ж��Ƿ�֧���ع�����¼�

\return ����true��֧���ع�����¼� false :��֧���ع�����¼�
*/
//----------------------------------------------------------------------------------
bool CGxCameraEventsDlg::__IsSupportExposureEnd()
{ 
	bool bIsSupportExposureEnd = false;   ///< �Ƿ�֧���ع�����¼���ʶ

	//�¼�ö��������
	GxIAPICPP::gxstring_vector vectorEnumEvent;
	CEnumFeaturePointer pEnumFeature = m_objFeatureControlPtr->GetEnumFeature("EventSelector");
	vectorEnumEvent = pEnumFeature->GetEnumEntryList();
	if (vectorEnumEvent.size() <= 0)
	{
		return bIsSupportExposureEnd;
	}

	//�Ƿ�֧���ع�����¼�
	for (uint32_t i=0; i<vectorEnumEvent.size(); i++)
	{
		if (vectorEnumEvent[i] == "ExposureEnd")
		{
			bIsSupportExposureEnd = true;
			return bIsSupportExposureEnd;
		}
	}

	return bIsSupportExposureEnd;
}

//----------------------------------------------------------------------------------
/**
\brief  ���½���

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::__UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʱ����ʾ���ڴ�ӡʱ����Ϣ
\param  strInfo �����ӡ����Ϣ

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::PrintInfotoWnd(string &strInfo)
{
	try
	{
		int      nTextLen = 0;
		CEdit*   pEdit    = (CEdit*)m_pLogWnd;      ///< ָ��༭����

		//������ھ����Ϊ��
		if (pEdit != NULL)
		{
			nTextLen = pEdit->GetWindowTextLength();
			if (pEdit->GetLineCount() > 300)
			{
				pEdit->Clear();
				pEdit->SetWindowText(_T(""));
				nTextLen = pEdit->GetWindowTextLength();
			}

			pEdit->SetSel(nTextLen,-1);
			strInfo.append("\r\n");
			pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());

			if (nTextLen >0)
			{
				pEdit->SetSel((nTextLen - pEdit->GetWindowTextLength() + 1), -1);
			}
			else
			{
				pEdit->SetSel(nTextLen,-1);
			}

		}
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

BOOL CGxCameraEventsDlg::PreTranslateMessage(MSG* pMsg)
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
