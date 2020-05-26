#include "stdafx.h"
#include "SingleItemDlg.h"
#include "resource.h"

SingleItemDlg::SingleItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SingleItemDlg::IDD, pParent)
{
	Create(SingleItemDlg::IDD,pParent);  
}

SingleItemDlg::~SingleItemDlg()
{

}

BOOL SingleItemDlg::OnInitDialog()
{   
    return FALSE;
}

void SingleItemDlg::updateFileInfo(FileInfo *fileInfo){
	this->fileInfo = fileInfo; 
	//file name
	CString fileName= fileInfo->file_name.Mid(fileInfo->file_name.ReverseFind('\\')+1);
	GetDlgItem(IDC_STATIC_FILE_NAME)->SetWindowText(fileName);
	//directory
	CString folderPath;
	int pos = fileInfo->file_name.ReverseFind('\\');
	if (pos != -1)
	{
		folderPath = fileInfo->file_name.Left(pos);
	}
	GetDlgItem(IDC_STATIC_FILE_DIR)->SetWindowText(folderPath);
	//duration
	CString duration; 
	if(this->fileInfo->duration/3600 > 0){
		duration.Format(_T("%dh%dm%ds"), this->fileInfo->duration/3600, this->fileInfo->duration/60, this->fileInfo->duration%60);
	}
	else if(this->fileInfo->duration/60 > 0){
		duration.Format(_T("%dm%ds"), this->fileInfo->duration/60, this->fileInfo->duration%60);
	}
	else{
		duration.Format(_T("%ds"), this->fileInfo->duration);
	}
	GetDlgItem(IDC_STATIC_FILE_DURATION)->SetWindowText(duration);
	//open times
	CString times; 
	times.Format(_T("%d"), this->fileInfo->open_times);
	GetDlgItem(IDC_STATIC_FILE_TIMES)->SetWindowText(times);
	//modified
	GetDlgItem(IDC_STATIC_MODIFIED)->SetWindowText(this->fileInfo->is_modified ? L"true":L"false");
	//pages
	CString pages; 
	pages.Format(_T("%d"), this->fileInfo->pages);
	GetDlgItem(IDC_STATIC_PAGES)->SetWindowText(pages);
	//
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(SingleItemDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &SingleItemDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


void SingleItemDlg::OnBnClickedCheck1()
{
	this->fileInfo->need_close = true;
	if(IsDlgButtonChecked(IDC_CHECK1)){ 
		this->fileInfo->need_close = true;
	}
	else{ 
		this->fileInfo->need_close = false;
	}  
}
