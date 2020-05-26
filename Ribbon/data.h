#pragma once
#include "stdafx.h"
#include <map>

typedef struct{
	int open_times;   
	int duration;  
	bool is_open;
	bool is_modified;
	bool need_close;
	int pages;
	SYSTEMTIME open_timestamp;
	CString file_name;
	FR_Document doc;
} FileInfo;

typedef std::map<CString, FileInfo*> FMap;
typedef std::map<CString, FileInfo*>::iterator FIterator;

typedef void (*CloseCallback)();