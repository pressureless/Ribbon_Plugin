#include "stdafx.h"
#include "CloseFileDlg.h"
#include "Resource.h"

CloseFileDlg::CloseFileDlg(CWnd* pParent, FMap* fMap, int size)
	: CDialog(CloseFileDlg::IDD, pParent)
{ 
	this->fMap = fMap;
}

BOOL CloseFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pdlgScroll=new FileListDlg(this, this->fMap);

	CRect rc;
	GetDlgItem(IDC_STATIC_PICTURE)->GetWindowRect(rc);
	ScreenToClient(&rc);

	m_pdlgScroll->MoveWindow(rc);

    return FALSE;
}


void CloseFileDlg::add_edit(){
	
 
}


void CloseFileDlg::setRibbon(CRibbonApp* rib){
	this->ribbon = rib;
}

CloseFileDlg::~CloseFileDlg()
{

}

BEGIN_MESSAGE_MAP(CloseFileDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CloseFileDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &CloseFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CloseFileDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}


void CloseFileDlg::OnBnClickedOk()
{ 
	CDialog::OnOK();
	this->ribbon->CloseFiles();
}
