#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		 
#include <map>
#include <string>
#include "CloseFileDlg.h"
 
class CRibbonApp : public CWinApp
{
public:
	CRibbonApp();
	static void OnButtonExecuteProc(void* clientDate);
	void ShowDlg();
	void CreateElementsToNewCategory();
	FS_DIBitmap GetBmpFromRes(UINT uID);
	 void OnSize(HWND hwnd, UINT flag, int width, int height);
	 LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL InitInstance();
public:
	void CloseFiles(); 
	DECLARE_MESSAGE_MAP()
public:
	 
	BOOL m_bHasAdd;

	// for load FS_DIBitmap capability
	typedef struct _png_clientdata_
	{
		BYTE* pData;
		DWORD dwSize;
	}PNGCLIENTDATA;

	static FS_BOOL OnFileReadBlock(FS_LPVOID clientData, void* buffer, FS_DWORD offset, FS_DWORD size);
	static FS_DWORD OnFileGetSize(FS_LPVOID clientData);
	static void OnFileRelease(FS_LPVOID clientData);
	static FS_FileRead fsFileReader;
	static FS_DIBitmap LoadFSDIBitmapFromPNG(UINT uID);

	static HWND FRRibbonBackstageCreateProc(HWND hParentWnd, void* clientData);
	static void FRRibbonBackstageDestoryProc(void* pDialog, void* clientData);
    static void FRExecuteProc(void *clientData);
	static FS_BOOL FRAppCustomRecentFileExecuteProc(FS_LPCWSTR lpwsFile, void *clientData);
	 
};
