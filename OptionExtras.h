#if !defined(AFX_OPTIONEXTRAS_H__78221363_B84D_11D6_9EE1_00E0987CDED7__INCLUDED_)
#define AFX_OPTIONEXTRAS_H__78221363_B84D_11D6_9EE1_00E0987CDED7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionExtras.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld OptionExtras 

class OptionExtras : public CPropertyPage
{
	DECLARE_DYNCREATE(OptionExtras)

// Konstruktion
public:
	OptionExtras();
	~OptionExtras();
	void Apply();
// Dialogfelddaten
	//{{AFX_DATA(OptionExtras)
	enum { IDD = IDD_EXTRAS };
	CComboBoxEx	m_langselect;
	//}}AFX_DATA


// �berschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(OptionExtras)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(OptionExtras)
	afx_msg void OnStereomode();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeLangselect();
	afx_msg void OnSelendokLangselect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_OPTIONEXTRAS_H__78221363_B84D_11D6_9EE1_00E0987CDED7__INCLUDED_
