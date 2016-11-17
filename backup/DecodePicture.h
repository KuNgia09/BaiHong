// DecodePicture.h : main header file for the DECODEPICTURE application
//

#if !defined(AFX_DECODEPICTURE_H__5E6A8F4F_CF65_4163_96A7_2AE5B8CD1AF6__INCLUDED_)
#define AFX_DECODEPICTURE_H__5E6A8F4F_CF65_4163_96A7_2AE5B8CD1AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDecodePictureApp:
// See DecodePicture.cpp for the implementation of this class
//

class CDecodePictureApp : public CWinApp
{
public:
	CDecodePictureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecodePictureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDecodePictureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECODEPICTURE_H__5E6A8F4F_CF65_4163_96A7_2AE5B8CD1AF6__INCLUDED_)
