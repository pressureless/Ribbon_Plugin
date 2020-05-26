// Ribbon.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Ribbon.h"
#include <windows.h> 
#include "CloseFileDlg.h"
#include <string> 
#include <stdlib.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

// CRibbonApp

BEGIN_MESSAGE_MAP(CRibbonApp, CWinApp)
END_MESSAGE_MAP()


// CRibbonApp 构造

CRibbonApp::CRibbonApp()
{ 
	m_bHasAdd = FALSE;
}

// 唯一的一个 CRibbonApp 对象

CRibbonApp theApp;

FMap fileMap; 

int getDuration(SYSTEMTIME& start, SYSTEMTIME& end){
	int diff = (end.wHour - start.wHour)*3600 + (end.wMinute-start.wMinute)*60 + end.wSecond-start.wSecond;
	return diff;
}

void updateDurationInfo(){
	 std::map<CString, FileInfo*>::iterator it;
	 SYSTEMTIME now;
	 GetSystemTime(&now);
	 for (it=fileMap.begin(); it!=fileMap.end(); ++it){
		 it->second->duration = getDuration(it->second->open_timestamp, now);
	 }
}

FS_DIBitmap CRibbonApp::GetBmpFromRes(UINT uID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRSRC hRes = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(uID), _T("PDF"));//RT_RCDATA);
	if (hRes == NULL) return NULL;
	DWORD dwLength = ::SizeofResource(AfxGetInstanceHandle(), hRes);
	HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRes);
	if (hGlobal == NULL) return NULL;
	LPVOID pData = ::LockResource(hGlobal);
	if (pData == NULL) return NULL;


	//create bmp
	FS_DIBitmap pBmp = FSDIBitmapNew();
	FSDIBitmapCreate(pBmp, 32, 32, FS_DIB_Argb, NULL, 0);
	FSDIBitmapClear(pBmp, 0xffffff);
	FPD_RenderDevice pDC = FPDFxgeDeviceNew();
	FPDFxgeDeviceAttach(pDC, pBmp, 0);
	FPD_Document pPDFDoc = FPDDocOpenMemDocument(pData, dwLength, NULL);
	FPD_Object pDict = FPDDocGetPage(pPDFDoc, 0);
	FPD_Page pPage = FPDPageNew();
	FPDPageLoad(pPage, pPDFDoc, pDict, TRUE);
	FPDPageParseContent(pPage, NULL);
	FS_AffineMatrix matrix = FPDPageGetDisplayMatrix(pPage, 0, 0, 32, 32, 0);
	FPD_RenderContext pContext = FPDRenderContextNew(pPage, TRUE);
	FPDRenderContextAppendPage(pContext, pPage, matrix);
	FPDRenderContextRender(pContext, pDC, NULL, NULL);
	FPDRenderContextDestroy(pContext);
	FPDPageDestroy(pPage);
	FPDDocClose(pPDFDoc);
	FPDFxgeDeviceDestroy(pDC);

	return pBmp;
}

void CRibbonApp::CreateElementsToNewCategory()
{
	FS_DIBitmap fs_bitmap = CRibbonApp::GetBmpFromRes(IDB_PNG2); 
	FR_RibbonBar fr_Bar = FRAppGetRibbonBar(NULL); 
	FR_RibbonCategory fr_Category = FRRibbonBarAddCategory(fr_Bar, "MyCategory", (FS_LPCWSTR)L"MyCategory");	
	FR_RibbonPanel fr_Panel = FRRibbonCategoryAddPanel(fr_Category, "Tool", (FS_LPCWSTR)L"Tool", NULL);

	//Create a Ribbon button
	FS_INT32 nElementCount = FRRibbonPanelGetElementCount(fr_Panel);
	FR_RibbonButton fr_Button =  (FR_RibbonButton)FRRibbonPanelAddElement(fr_Panel, FR_RIBBON_BUTTON,
		"CloseSpecificFiles", (FS_LPCWSTR)L"CloseSpecificFiles", nElementCount);
	FR_RibbonElement fr_ElementButton = FRRibbonPanelGetElementByName(fr_Panel, "CloseSpecificFiles");
	FRRibbonElementSetImage(fr_ElementButton, fs_bitmap, fs_bitmap);
	FRRibbonElementSetTooltip(fr_ElementButton, (FS_LPCWSTR)L"Close specific files");
	// 
	FS_WideString fswsShowMsg = FSWideStringNew3((FS_LPCWSTR)L"On click button", wcslen(L"On click button") * sizeof(WCHAR));
	FS_LPCWSTR str = FSWideStringCastToLPCWSTR(fswsShowMsg);
	FRRibbonElementSetExecuteProc(fr_ElementButton, CRibbonApp::OnButtonExecuteProc);
	FSDIBitmapDestroy(fs_bitmap);
}

void CRibbonApp::OnButtonExecuteProc(void* clientDate){
	std::map<CString, FileInfo*>::iterator it;
	bool has_open = false;
	for(it = fileMap.begin(); it!=fileMap.end(); it++){
		if(it->second->is_open){ 
			has_open = true;
			break;
		}
	}
	if(has_open){
		theApp.ShowDlg();
	}
	else{
		AfxMessageBox(L"No files!");
	}
}

BOOL CRibbonApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

FS_FileRead CRibbonApp::fsFileReader = {0};
FS_BOOL CRibbonApp::OnFileReadBlock(FS_LPVOID clientData, void* buffer, FS_DWORD offset, FS_DWORD size)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	memcpy(buffer, pClientData->pData+offset, size);	
	return TRUE;
}

FS_DWORD CRibbonApp::OnFileGetSize(FS_LPVOID clientData)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	return pClientData->dwSize;
}

void CRibbonApp::OnFileRelease(FS_LPVOID clientData)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	delete pClientData;
}
FS_DIBitmap CRibbonApp::LoadFSDIBitmapFromPNG(UINT uID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINSTANCE hInstOld=AfxGetResourceHandle();
	AfxSetResourceHandle(AfxGetInstanceHandle());
	HRSRC hRes = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(uID), _T("PNG"));
	if (hRes == NULL) return NULL;
	DWORD dwLength = ::SizeofResource(AfxGetInstanceHandle(), hRes);
	HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRes);
	if (hGlobal == NULL) return NULL;
	LPVOID pData = ::LockResource(hGlobal);
	if (pData == NULL) return NULL;
	AfxSetResourceHandle(hInstOld);

	PNGCLIENTDATA* pClientData = new PNGCLIENTDATA;
	memset(pClientData, 0, sizeof(PNGCLIENTDATA));
	pClientData->pData = (BYTE*)pData;
	pClientData->dwSize = dwLength;

	memset(&fsFileReader, 0, sizeof(FS_FileRead));
	CRibbonApp::fsFileReader.lStructSize = sizeof(FS_FileRead);
	CRibbonApp::fsFileReader.FSFileRead_ReadBlock = &CRibbonApp::OnFileReadBlock;
	CRibbonApp::fsFileReader.FSFileRead_GetSize = &CRibbonApp::OnFileGetSize;
	CRibbonApp::fsFileReader.FSFileRead_Release = &CRibbonApp::OnFileRelease;
	CRibbonApp::fsFileReader.clientData = (LPVOID)pClientData;

	FS_FileReadHandler fileHandler = FSFileReadHandlerNew(CRibbonApp::fsFileReader);
	FS_DIBitmap bmp = NULL;
	bmp = FSDIBitmapLoadFromPNGIcon2(fileHandler);
	
	return bmp;

} 

void CRibbonApp::ShowDlg(){
	std::map<CString, FileInfo*>::iterator it;
	for(it = fileMap.begin(); it!=fileMap.end(); it++){
		if(it->second->is_open){ 
			it->second->is_modified = FRDocGetChangeMark(it->second->doc);  
			it->second->pages = FPDDocGetPageCount(FRDocGetPDDoc(it->second->doc));  
		}
	}
	//update before show
	updateDurationInfo();
	CloseFileDlg dlg;
	dlg.fMap = &fileMap;
	dlg.ribbon = &theApp;
	dlg.DoModal(); 
}

/********************************************************************/
/* The basic procedure of plug-in.                                   
/********************************************************************/


/* PIExportHFTs
** ------------------------------------------------------
**/
/** 
** Create and Add the extension HFT's.
**
** @return true to continue loading plug-in,
** false to cause plug-in loading to stop.
*/
FS_BOOL PIExportHFTs(void)
{
	return TRUE;
}

/** 
The application calls this function to allow it to
<ul>
<li> Import plug-in supplied HFTs.
<li> Replace functions in the HFTs you're using (where allowed).
<li> Register to receive notification events.
</ul>
*/
FS_BOOL PIImportReplaceAndRegister(void)
{
	return TRUE;
}


void OnClick(void* pData)
{
	AfxMessageBox(L"Hello!");
}

/*This optional callback is invoked by Foxit Reader when determining 
 *whether or not to enable the menu item.
 */
BOOL IsEnable(void* pData)
{
	return TRUE;
}

/*This optional callback is invoked by Foxit Reader when determining 
 *whether or not the menu item should be checked.
 */
BOOL IsCheck(void* pData)
{
	return FALSE;
}

//////////////////////////////////////////////////////////
void PILoadToolBarUI(void* pParentWnd)
{ 
	//AfxMessageBox(L"PILoadToolBarUI!"); 
}

void PIReleaseToolBarUI(void* pParentWnd)
{
	//AfxMessageBox(L"PIReleaseToolBarUI!");  
}

void PILoadRibbonUI(void* pParentWnd)
{
	theApp.CreateElementsToNewCategory();
}

void PILoadStatusBarUI(void* pParentWnd)
{

}
/*Define the callback functions that will be invoked when the app event occurs.
 *Add your own application logic in the callback functions to meet your requirement.
 */
void OnLangUIChange(FS_LPVOID clientData)
{
	//AfxMessageBox(L"App event occurs: The UI of language changes.");
}
	
void OnActivateApp(FS_LPVOID clientData, FS_BOOL bActive)
{
	//AfxMessageBox(L"App event occurs: The app is activated.");
}

void WillQuit(FS_LPVOID clientData)
{
	//AfxMessageBox(L"App event occurs: The app will quit.");
}

void OnDownload(FS_LPVOID clientData, FS_LPCSTR lpModuleName)
{
	//AfxMessageBox(L"App event occurs: Needs to download the updated module.");
}

FS_BOOL OnOpenDoc(FS_LPVOID clientData, FS_LPCWSTR lpszFilePath)
{
	//AfxMessageBox(L"OnOpenDoc");
	return FALSE;
}

void OnCloseRibbonFilePage(FS_LPVOID clientData)
{
	//AfxMessageBox(L"OnCloseRibbonFilePage");
}


/* PIInit
** ------------------------------------------------------
**/
/** 
	The main initialization routine.
	
	@return true to continue loading the plug-in, 
	false to cause plug-in loading to stop.
*/
FR_AppEventCallbacksRec appEventCallbacks;
FR_PageEventCallbacksRec pageEventCallbacks;

//document callbacks
FR_DocEventCallbacksRec docEventCallbacks;


void onDocDidOpen(FS_LPVOID clientData, FR_Document doc)
{ 
	FPD_Document pdfDoc = FRDocGetPDDoc(doc);
	if (pdfDoc == NULL){
		AfxMessageBox(L"pdfDoc NULL!");
	}
	FS_INT32 cnt = FPDDocGetPageCount(pdfDoc);
	FS_WideString path = FSWideStringNew(); 
	FRDocGetFilePath(doc, &path);
	FS_LPCWSTR lpath = FSWideStringCastToLPCWSTR(path);
	CString cpath(lpath); 
	//map op
	std::map<CString, FileInfo*>::iterator it = fileMap.find(cpath);
	FileInfo *fileInfo;
	if(it == fileMap.end()){ 
		fileInfo = new FileInfo;
		fileInfo->file_name = lpath;
		fileInfo->duration = 0;
		fileInfo->open_times = 1; 
		fileInfo->is_open = true; 
		fileInfo->need_close = false;
		fileInfo->is_modified = false;
		fileInfo->doc = doc;
		GetSystemTime(&fileInfo->open_timestamp);
		fileMap.insert(std::pair<CString,FileInfo*>(cpath, fileInfo));
	}
	else{ 
		//
		fileInfo = it->second;
		fileInfo->duration = 0;
		fileInfo->is_open = true; 
		fileInfo->need_close = false;
		fileInfo->is_modified = false;
		fileInfo->doc = doc;
		GetSystemTime(&fileInfo->open_timestamp);
		fileInfo->open_times ++;
	} 
}

void CRibbonApp::CloseFiles(){
	//AfxMessageBox(L"close all");
	std::map<CString, FileInfo*>::iterator it;
	for(it = fileMap.begin(); it!=fileMap.end(); it++){
		if(it->second->need_close){
			it->second->need_close = false;
			it->second->is_open = false; 
			FRDocClose(it->second->doc, true, false, false);
		}
	}
}


void onDocDidClose(FS_LPVOID clientData, FR_Document doc)
{ 
	FPD_Document pdfDoc = FRDocGetPDDoc(doc);
	if (pdfDoc == NULL){
		AfxMessageBox(L"pdfDoc NULL!");
	}
	FS_INT32 cnt = FPDDocGetPageCount(pdfDoc);
	FS_WideString path = FSWideStringNew(); 
	FRDocGetFilePath(doc, &path);
	FS_LPCWSTR lpath = FSWideStringCastToLPCWSTR(path);
	//
	std::map<CString, FileInfo*>::iterator it = fileMap.find(CString(lpath));
	it->second->is_open = false;
	//updateDurationInfo();   //update duration
	//FileInfo* info = fileMap[lpath];
	//fileMap.erase(lpath);
	//delete info;
}

FS_BOOL PIInit(void)
{	
	INIT_CALLBACK_STRUCT(&appEventCallbacks, sizeof(FR_AppEventCallbacksRec));
	appEventCallbacks.lStructSize = sizeof(FR_AppEventCallbacksRec);
	appEventCallbacks.clientData = NULL;
	appEventCallbacks.FRAppOnActivate = &OnActivateApp;
	appEventCallbacks.FRAppOnLangUIChange = &OnLangUIChange;
	appEventCallbacks.FRAppWillQuit = &WillQuit;
	appEventCallbacks.FRAppOnDownload = &OnDownload;
	
	appEventCallbacks.FRAppOnOpenDocument = &OnOpenDoc;
	appEventCallbacks.FRAppOnDidCloseRibbonFilePage = &OnCloseRibbonFilePage;
	/*Register the app event handler.*/
	FRAppRegisterAppEventHandler(&appEventCallbacks);

	//
	INIT_CALLBACK_STRUCT(&pageEventCallbacks, sizeof(FR_PageEventCallbacksRec));
	pageEventCallbacks.lStructSize = sizeof(FR_DocEventCallbacksRec);
	pageEventCallbacks.clientData = NULL;
	//docs callback
	INIT_CALLBACK_STRUCT(&docEventCallbacks, sizeof(FR_DocEventCallbacksRec));
	docEventCallbacks.lStructSize = sizeof(FR_DocEventCallbacksRec);
	docEventCallbacks.clientData = NULL;
	docEventCallbacks.FRDocWillClose = onDocDidClose;
	docEventCallbacks.FRDocDidOpen = &onDocDidOpen;
	FRAppRegisterDocHandlerOfPDDoc(&docEventCallbacks);
	//
	return TRUE;
}

/* PIUnload
** ------------------------------------------------------
**/
/** 
	The unload routine.
	Called when your plug-in is being unloaded when the application quits.
	Use this routine to release any system resources you may have
	allocated.

	Returning false will cause an alert to display that unloading failed.
	@return true to indicate the plug-in unloaded.
*/
FS_BOOL PIUnload(void)
{  
	return TRUE;
}

/** PIHandshake
	function provides the initial interface between your plug-in and the application.
	This function provides the callback functions to the application that allow it to 
	register the plug-in with the application environment.

	Required Plug-in handshaking routine:
	
	@param handshakeVersion the version this plug-in works with. 
	@param handshakeData OUT the data structure used to provide the primary entry points for the plug-in. These
	entry points are used in registering the plug-in with the application and allowing the plug-in to register for 
	other plug-in services and offer its own.
	@return true to indicate success, false otherwise (the plug-in will not load).
*/
FS_BOOL PIHandshake(FS_INT32 handshakeVersion, void *handshakeData)
{
	if(handshakeVersion != HANDSHAKE_V0100)
		return FALSE;
	
	/* Cast handshakeData to the appropriate type */
	PIHandshakeData_V0100* pData = (PIHandshakeData_V0100*)handshakeData;
	
	/* Set the name we want to go by */
	pData->PIHDRegisterPlugin(pData, "Ribbon", (FS_LPCWSTR)L"Ribbon");

	

	/* If you export your own HFT, do so in here */
	pData->PIHDSetExportHFTsCallback(pData, &PIExportHFTs);
		
	/*
	** If you import plug-in HFTs, replace functionality, and/or want to register for notifications before
	** the user has a chance to do anything, do so in here.
	*/
	pData->PIHDSetImportReplaceAndRegisterCallback(pData, &PIImportReplaceAndRegister);

	/* Perform your plug-in's initialization in here */
	pData->PIHDSetInitDataCallback(pData, &PIInit);

	PIInitUIProcs initUIProcs;
	INIT_CALLBACK_STRUCT(&initUIProcs, sizeof(PIInitUIProcs));
	initUIProcs.lStructSize = sizeof(PIInitUIProcs);
	initUIProcs.PILoadToolBarUI = PILoadToolBarUI;
	initUIProcs.PIReleaseToolBarUI = PIReleaseToolBarUI;
	initUIProcs.PILoadRibbonUI = PILoadRibbonUI;
	initUIProcs.PILoadStatusBarUI = PILoadStatusBarUI;
	pData->PIHDSetInitUICallbacks(pData, &initUIProcs);

	/* Perform any memory freeing or state saving on "quit" in here */
	pData->PIHDSetUnloadCallback(pData, &PIUnload);

	return TRUE;
}

/*Core HFT Manager.*/
FRCoreHFTMgr *_gpCoreHFTMgr = NULL;

/*The plug-in ID*/
void* _gPID = NULL;

/* 
** This routine is called by the host application to set up the plug-in's SDK-provided functionality.
*/
FS_BOOL PISetupSDK(FS_INT32 handshakeVersion, void *sdkData)
{
	if(handshakeVersion != HANDSHAKE_V0100) return FALSE;
	PISDKData_V0100 *pSDKData = (PISDKData_V0100*)sdkData;
	
	/* Get our globals out */
	_gpCoreHFTMgr = pSDKData->PISDGetCoreHFT();

	/* Get PID */
	_gPID = pSDKData->PISDGetPID(sdkData);

	/* Set the plug-in's handshake routine, which is called next by the host application */
	pSDKData->PISDSetHandshakeProc(sdkData, &PIHandshake);

	/* For compatibility purposes, set the plug-in's SDK version, 
	so that Foxit Reader will not load the plug-in whose version is larger than Foxit Reader. */
	pSDKData->PISetSDKVersion(sdkData, SDK_VERSION);

	return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif


/* The export function of plug-ins */
__declspec(dllexport) FS_BOOL PlugInMain(FS_INT32* handshakeVersion, PISetupSDKProcType* setupProc)
{
	/*
	** handshakeVersion tells us which version of the handshake struct the application has sent us.
	** The version we want to use is returned to the application so it can adjust accordingly.
	*/

	*handshakeVersion = HANDSHAKE_V0100;
	*setupProc = &PISetupSDK;
	return TRUE;
} 


#ifdef __cplusplus
}
#endif