#include "stdafx.h" 
#include "FileListDlg.h"
#include "SingleItemDlg.h"
#include <string> 
#include <stdlib.h> 
  
FileListDlg::FileListDlg(CWnd* pParent, FMap* fMap)
	: CDialog(FileListDlg::IDD, pParent)
{ 
	this->fMap = fMap;
	m_bDragging=FALSE; 
	Create(FileListDlg::IDD,pParent);
}


void FileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX); 
}


BEGIN_MESSAGE_MAP(FileListDlg, CDialog) 
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS() 
	ON_WM_SETCURSOR() 
END_MESSAGE_MAP()
 

BOOL FileListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int cnt = 0;
	int size = fMap->size();

	if(size > 0)
	{
		FIterator it;
		dlg = (SingleItemDlg**)(new SingleItemDlg*[size]);
		this->fileCnt = size;
		for(it = fMap->begin(); it != fMap->end(); it++)
		{
			if(it->second->is_open){
				dlg[cnt] = new SingleItemDlg(this);
				dlg[cnt]->MoveWindow(0,cnt*200,800,200);
				dlg[cnt]->updateFileInfo(it->second);
				cnt++;
			}
		}
	} 
	// 
	GetWindowRect(m_rcOriginalRect);
	m_rcOriginalRect.bottom +=200*cnt - 400;
	int max_value = 100;
	if(m_rcOriginalRect.bottom < max_value)
	{
		m_rcOriginalRect.bottom = max_value;
	}

	m_nScrollPos = 0; 
	return TRUE; 
}

void FileListDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPos);
		break;
	
         default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	ScrollWindow(0,-nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void FileListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	m_nCurHeight = cy;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE); 	
}

BOOL FileListDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	if (zDelta<0)
	{
		if (m_nScrollPos < nMaxPos)
		{
			zDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPos);

			m_nScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}
	else
	{
		if (m_nScrollPos > 0)
		{
			zDelta = -min(max(nMaxPos/20,5),m_nScrollPos);

			m_nScrollPos += zDelta;
			SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
			ScrollWindow(0,-zDelta);
		}
	}
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void FileListDlg::OnCancel() 
{
}

void FileListDlg::OnOK() 
{
}

void FileListDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bDragging=TRUE;
	SetCapture();

	m_ptDragPoint=point; 
	CDialog::OnLButtonDown(nFlags, point);
}

void FileListDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EndDrag();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void FileListDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		int nDelta=m_ptDragPoint.y-point.y;
		m_ptDragPoint=point;

		int nNewPos=m_nScrollPos+nDelta;

		if (nNewPos<0)
			nNewPos=0;
		else if (nNewPos>m_rcOriginalRect.Height() - m_nCurHeight)
			nNewPos=m_rcOriginalRect.Height() - m_nCurHeight;

		nDelta=nNewPos-m_nScrollPos;
		m_nScrollPos=nNewPos;

		SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
		ScrollWindow(0,-nDelta);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void FileListDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

	EndDrag();
}

void FileListDlg::EndDrag()
{
	m_bDragging=FALSE;
	ReleaseCapture(); 
}

