
// PDFtoJPG.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPDFtoJPGApp:
// �� Ŭ������ ������ ���ؼ��� PDFtoJPG.cpp�� �����Ͻʽÿ�.
//

class CPDFtoJPGApp : public CWinApp
{
public:
	CPDFtoJPGApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPDFtoJPGApp theApp;