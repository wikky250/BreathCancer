
// DICOM2PIC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDICOM2PICApp: 
// �йش����ʵ�֣������ DICOM2PIC.cpp
//

class CDICOM2PICApp : public CWinApp
{
public:
	CDICOM2PICApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDICOM2PICApp theApp;