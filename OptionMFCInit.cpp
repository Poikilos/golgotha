// mfctest.cpp : Legt das Klassenverhalten f�r die Anwendung fest.
//

#include "pch.h"
#include "Optionmfcinit.h"
//#include "mfctestDlg.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfctestApp

BEGIN_MESSAGE_MAP(CMfctestApp, CWinApp)
	//{{AFX_MSG_MAP(CMfctestApp)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfctestApp Konstruktion

CMfctestApp::CMfctestApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einf�gen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CMfctestApp-Objekt

CMfctestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMfctestApp Initialisierung

BOOL CMfctestApp::InitInstance()
{
	AfxEnableControlContainer();  //This has already been done

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Gr��e Ihrer fertigen 
	//  ausf�hrbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht ben�tigen, entfernen.

	//Not needed on Win95+, cause only trouble
//#ifdef _AFXDLL
//	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
//#else
//	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
//#endif

	/*CMfctestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// ZU ERLEDIGEN: F�gen Sie hier Code ein, um ein Schlie�en des
		//  Dialogfelds �ber OK zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// ZU ERLEDIGEN: F�gen Sie hier Code ein, um ein Schlie�en des
		//  Dialogfelds �ber "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zur�ckliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;*/
	return TRUE;
}

