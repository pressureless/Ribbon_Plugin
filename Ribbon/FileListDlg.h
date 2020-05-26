#pragma once
#include "resource.h"
#include "data.h"

class SingleItemDlg;

class FileListDlg : public CDialog
{ 
public:
	SingleItemDlg **dlg;
	void EndDrag();
	FileListDlg(CWnd* pParent = NULL, FMap* fMap=NULL); 
	FMap* fMap;

	CRect	m_rcOriginalRect;
	CEdit   *m_edit; 
	BOOL	m_bDragging;
	CPoint	m_ptDragPoint;
	int		m_nScrollPos;
	int		m_nCurHeight;
	 
	enum { IDD = IDD_DIALOG_FILE }; 
	
	int fileCnt;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
protected: 
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd); 
	DECLARE_MESSAGE_MAP()
}; 