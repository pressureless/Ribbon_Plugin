#pragma once 
#include "resource.h"
#include "data.h"

class SingleItemDlg : public CDialog{
public:
	SingleItemDlg(CWnd* pParent = NULL);   // standard constructor
	~SingleItemDlg();

	BOOL OnInitDialog();
	void updateFileInfo(FileInfo *fileInfo);
	FileInfo *fileInfo;

	enum { IDD = IDD_DIALOG_SINGLE };

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheck1();
};