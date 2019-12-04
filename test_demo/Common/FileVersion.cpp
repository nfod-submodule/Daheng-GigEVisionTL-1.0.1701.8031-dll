#include "stdafx.h"
#include "FileVersion.h"

CFileVersion::CFileVersion(CString strPath)
{
	m_strPath = strPath;
}

CFileVersion::~CFileVersion()
{
}

//--------------------------------------------------------------
/**
\brief   ��ȡ���е��ļ��汾��Ϣ������Ϣ����\n�ָ�

\return  strRet   ���еİ汾��Ϣ
*/
//--------------------------------------------------------------
CString CFileVersion::GetFullVersion()
{
	CString strRet;
	strRet = "�ļ��汾: " + GetFileVersion() + "\n"; 
	strRet += "˵��: " + GetFileDescription() +"\n";
	strRet += "��Ȩ: " + GetLegalCopyright() +"\n";
	strRet += "��ע: " + GetComments() + "\n";
	strRet += "��Ʒ�汾: " + GetProductVersion() +"\n";
	strRet += "��Ʒ����: " + GetProductName() +"\n";
	strRet += "�������ڲ��汾˵��: " + GetPrivateBuild() +"\n";
	strRet += "��˾����: " + GetCompanyName() +"\n";
	strRet += "�Ϸ��̱�: " + GetLegalTrademarks() +"\n";
	strRet += "�ڲ�����: " + GetInternalName() +"\n";
	strRet += "�����ڲ��汾˵��: " + GetSpecialBuild() +"\n";
	strRet += "Դ�ļ���: " + GetOriginalFilename() +"\n";	
	return strRet;
}

//--------------------------------------------------------------
/**
\brief    ��ȡ��ע

\return   ��ע
*/
//--------------------------------------------------------------
CString CFileVersion::GetComments()
{
	return GetVer("Comments");
}

//--------------------------------------------------------------
/**
\brief    ����ڲ�����

\return   �ڲ�����
*/
//--------------------------------------------------------------
CString CFileVersion::GetInternalName()
{
	return GetVer("InternalName");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ��Ʒ����

\return   ��Ʒ����
*/
//--------------------------------------------------------------
CString CFileVersion::GetProductName()
{
	return GetVer("ProductName");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ��˾����

\return   ��˾����
*/
//--------------------------------------------------------------
CString CFileVersion::GetCompanyName()
{
	return GetVer("CompanyName");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ��Ȩ

\return   ��Ȩ
*/
//--------------------------------------------------------------
CString CFileVersion::GetLegalCopyright()
{
	return GetVer("LegalCopyright");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ��Ʒ�汾

\return   ��Ʒ�汾
*/
//--------------------------------------------------------------
CString CFileVersion::GetProductVersion()
{
	return GetVer("ProductVersion");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�ļ�����

\return   �ļ�����
*/
//--------------------------------------------------------------
CString CFileVersion::GetFileDescription()
{
	return GetVer("FileDescription");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�Ϸ��̱�

\return   �Ϸ��̱�
*/
//--------------------------------------------------------------
CString CFileVersion::GetLegalTrademarks()
{
	return GetVer("LegalTrademarks");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�����ڲ��汾˵��

\return   �����ڲ��汾˵��
*/
//--------------------------------------------------------------
CString CFileVersion::GetPrivateBuild()
{
	return GetVer("PrivateBuild");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�ļ��汾

\return   �ļ��汾
*/
//--------------------------------------------------------------
CString CFileVersion::GetFileVersion()
{
	return GetVer("FileVersion");
}

//--------------------------------------------------------------
/**
\brief    ��ȡԴ�ļ���

\return   �ڲ�����
*/
//--------------------------------------------------------------
CString CFileVersion::GetOriginalFilename()
{
	return GetVer("OriginalFilename");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�����ڲ��汾˵��

\return   �����ڲ��汾˵��
*/
//--------------------------------------------------------------
CString CFileVersion::GetSpecialBuild()
{
	return GetVer("SpecialBuild");
}

//--------------------------------------------------------------
/**
\brief    ��ȡ�汾��Ϣ����
\param    strSubBlock    ��ȡ�汾�� 

\return   �汾��Ϣ
*/
//--------------------------------------------------------------
CString CFileVersion::GetVer(CString strSubBlock)
{
	struct LANGANDCODEPAGE 
	{
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;
	
	int     i             = 0;
	CString strRet        = "";
	DWORD   dwHandle      = 0;
	DWORD   dwInfoSize    = 0;
	UINT    cbTranslate   = 0;
	UINT    dwBytes       = 0;
	char    chBlock[256]  = {0};
	char*   pBuffer       = NULL;
	char*   pData         = NULL;

	// ��ȡ�ļ��汾��Ϣ��С
	dwInfoSize = GetFileVersionInfoSize((char*)(LPCTSTR)m_strPath, &dwHandle);

	if(dwInfoSize != 0)
	{
		// ��ȡ�汾��Ϣ
		pData = new char[dwInfoSize];
		if( GetFileVersionInfo((char*)(LPCTSTR)m_strPath, 0, dwInfoSize, pData))
		{
			
			VerQueryValue(pData, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);

			for(i = 0; i <(int)(cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++)
			{
				wsprintf(chBlock, 
					     TEXT("\\StringFileInfo\\%04x%04x\\%s"),
					     lpTranslate[i].wLanguage,
					     lpTranslate[i].wCodePage,
					     strSubBlock);		
				
				VerQueryValue(pData, 
					         chBlock, 
					         (LPVOID*)&pBuffer, 
				          	 &dwBytes); 
				
				strRet.Format("%s", pBuffer);

				break; //only retrieve the first language and code page
			}
		}
		if (pData != NULL)
		{
			delete []pData;
			pData = NULL;
		}
	}

	return strRet;
}