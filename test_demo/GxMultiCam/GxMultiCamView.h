// GxMuitCamView.h : CGxMuitCamView ��Ľӿ�
//


#pragma once


class CGxMuitCamView : public CView
{
protected: // �������л�����
	CGxMuitCamView();
	DECLARE_DYNCREATE(CGxMuitCamView)

// ����
public:
	CGxMuitCamDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CGxMuitCamView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GxMuitCamView.cpp �еĵ��԰汾
inline CGxMuitCamDoc* CGxMuitCamView::GetDocument() const
   { return reinterpret_cast<CGxMuitCamDoc*>(m_pDocument); }
#endif

