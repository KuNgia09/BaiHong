
// APKInstaller.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAPKInstallerApp:
// �йش����ʵ�֣������ APKInstaller.cpp
//

class CAPKInstallerApp : public CWinAppEx
{
public:
	CAPKInstallerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAPKInstallerApp theApp;