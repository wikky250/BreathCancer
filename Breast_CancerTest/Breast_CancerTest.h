
// Breast_CancerTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBreast_CancerTestApp: 
// �йش����ʵ�֣������ Breast_CancerTest.cpp
//

class CBreast_CancerTestApp : public CWinApp
{
public:
	CBreast_CancerTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBreast_CancerTestApp theApp;