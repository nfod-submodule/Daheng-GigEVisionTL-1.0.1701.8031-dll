// GxMuitCamView.cpp : CGxMuitCamView ���ʵ��
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"
#include "GxMultiCamView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMuitCamView

IMPLEMENT_DYNCREATE(CGxMuitCamView, CView)

BEGIN_MESSAGE_MAP(CGxMuitCamView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGxMuitCamView ����/����

CGxMuitCamView::CGxMuitCamView()
{
	// TODO: �ڴ˴���ӹ������

}

CGxMuitCamView::~CGxMuitCamView()
{
}

BOOL CGxMuitCamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGxMuitCamView ����

void CGxMuitCamView::OnDraw(CDC* /*pDC*/)
{
	CGxMuitCamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CGxMuitCamView ��ӡ

BOOL CGxMuitCamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CGxMuitCamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CGxMuitCamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CGxMuitCamView ���

#ifdef _DEBUG
void CGxMuitCamView::AssertValid() const
{
	CView::AssertValid();
}

void CGxMuitCamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGxMuitCamDoc* CGxMuitCamView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGxMuitCamDoc)));
	return (CGxMuitCamDoc*)m_pDocument;
}
#endif //_DEBUG


// CGxMuitCamView ��Ϣ�������