// GxMuitCamDoc.cpp : CGxMuitCamDoc ���ʵ��
//

#include "stdafx.h"
#include "GxMultiCam.h"

#include "GxMultiCamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGxMuitCamDoc

IMPLEMENT_DYNCREATE(CGxMuitCamDoc, CDocument)

BEGIN_MESSAGE_MAP(CGxMuitCamDoc, CDocument)
END_MESSAGE_MAP()


// CGxMuitCamDoc ����/����

CGxMuitCamDoc::CGxMuitCamDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CGxMuitCamDoc::~CGxMuitCamDoc()
{
}

BOOL CGxMuitCamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CGxMuitCamDoc ���л�

void CGxMuitCamDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CGxMuitCamDoc ���

#ifdef _DEBUG
void CGxMuitCamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGxMuitCamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGxMuitCamDoc ����
