#pragma once
#include "Resource.h"  
#include "FileListDlg.h"
#include "Ribbon.h"
#include "data.h"

class CRibbonApp;
class CloseFileDlg : public CDialog
{
	public:
	CloseFileDlg(CWnd* pParent=NULL, FMap* fMap=NULL, int size=0);   // standard constructor
	~CloseFileDlg(); 
	enum { IDD = IDD_DIALOG_CLOSE_FILE };
    CEdit m_edit; 
    CStatic m_static;
	FMap* fMap;
	FileListDlg *m_pdlgScroll; 
	CRibbonApp *ribbon;
	void setRibbon(CRibbonApp* rib);
	void add_edit();
    BOOL OnInitDialog();
public:
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();

};