// Demo_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

DWORD WINAPI function_cal(LPVOID arg)
{
	SOCKET* hConnected = (SOCKET*) arg;
	CSocket mysock;
	//Chuyen ve lai CSocket
	mysock.Attach(*hConnected);
	string s;
	string serverName;
	
	do {
		fflush(stdin); 
		mysock.Receive(&serverName, sizeof(serverName), 0);
		cout << serverName << ":";
		getline(cin, s);
		if (s != "Thoat")
		{
			mysock.Send(&s, sizeof(s), 0);
			mysock.Receive(&s, sizeof(s), 0);
			cout << "Client: " << s << endl;
		}
		else break;

	}while(true);
	delete hConnected;
	mysock.Close();
	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{	
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			AfxSocketInit(NULL);
			CSocket server, s;
			DWORD threadID;
			HANDLE threadStatus;

			server.Create(4567);
			do {
				printf("Server lang nghe ket noi tu client\n");
				server.Listen(2);
				server.Accept(s);
				//Khoi tao con tro Socket
				SOCKET* hConnected = new SOCKET();
				//Chuyển đỏi CSocket thanh Socket
				*hConnected	= s.Detach();
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				//Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);
			}while(1);
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
	//TEST 123456
}


