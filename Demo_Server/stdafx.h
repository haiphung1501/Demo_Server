// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#define CURL_STATICLIB
#include "curl/curl.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
// TODO: reference additional headers your program requires here

struct Date
{
	int day;
	int month;
	int year;
};

struct Currency {
	string name;
	double sell, buy;
};
Date stringToDate(string s);
int checkDate(Date x);
double getRate(int line, string type);
string exchangeType(int type);
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
string callAPI();
vector <Currency> onlineData();
Currency FindClientCur(int pos, vector <Currency> cur);