#if !defined(AFX_OPTIONSHEET_H__DE83BCA1_10B8_11D5_9EDB_00E0987CDED7__INCLUDED_)
#define AFX_OPTIONSHEET_H__DE83BCA1_10B8_11D5_9EDB_00E0987CDED7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Optionsheet.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// COptionsheet

class COptionsheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionsheet)

// Konstruktion
public:
	COptionsheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionsheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attribute
public:
   OptionsDialog graphics;
   COptionInfo info;
   COptionSound sound;
   OptionExtras extras;

// Operationen
public:
  
// �berschreibungen
	virtual BOOL OnInitDialog();
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(COptionsheet)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual void Apply();
	virtual ~COptionsheet();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(COptionsheet)
		// HINWEIS - Der Klassen-Assistent f�gt hier Member-Funktionen ein und entfernt diese.
	afx_msg void OnApplyNow();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_OPTIONSHEET_H__DE83BCA1_10B8_11D5_9EDB_00E0987CDED7__INCLUDED_
