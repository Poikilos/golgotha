// mfctest.h : Haupt-Header-Datei f�r die Anwendung MFCTEST
//

#if !defined (AFX_MFCTEST_H__C69E5248_C3D2_11D4_9EDB_00E0987CDED7__INCLUDED_)
#define AFX_MFCTEST_H__C69E5248_C3D2_11D4_9EDB_00E0987CDED7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef __AFXWIN_H__
//	#error include 'stdafx.h' before including this file for PCH
//#endif

#include "resource.h"        // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CMfctestApp:
// Siehe mfctest.cpp f�r die Implementierung dieser Klasse
//

class CMfctestApp :
	public CWinApp
{
public:
	CMfctestApp();

// �berladungen
	// Vom Klassenassistenten generierte �berladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CMfctestApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CMfctestApp)
	// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingef�gt und entfernt.
	//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMfctestApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // !defined(AFX_MFCTEST_H__C69E5248_C3D2_11D4_9EDB_00E0987CDED7__INCLUDED_)
