// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
class CControlDlg;             ///< �����Ի�����
class CMainFrame : public CFrameWnd
{
	
	CSplitterWnd m_wndSplitter;          ///< ����ͼ���Ϊ����
public: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
	CSplitterWnd m_wndSplitterLeft;      ///< ���ұ���ͼ���Ϊ��������4������
	CControlDlg* m_objControlDlg;

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	CWnd* m_pWnd[4];

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};


