// GxMuitCamDoc.h : CGxMuitCamDoc ��Ľӿ�
//


#pragma once
#include "GalaxyIncludes.h"

class CGxMuitCamDoc : public CDocument
{
protected: // �������л�����
	CGxMuitCamDoc();
	DECLARE_DYNCREATE(CGxMuitCamDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CGxMuitCamDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


