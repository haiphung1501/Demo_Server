// Demo_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// The one and only application object

CWinApp theApp;

bool checkLogin(string user, string pass) {
	ifstream ifs("login.txt");
	if (ifs.fail()) {
		cout << "Error in Opening! File Not Found!!" << endl;
		cout << "\n***Press Enter to continue";
		return 0;
	}
	string _user, _pass;
	while (!ifs.eof()) {
		getline(cin, _user, '-');
		getline(cin, _pass);
		if (_user == user && _pass == pass)
		{
			ifs.close();
			return 1;
		}
	}
	ifs.close();
	return 0; // wrong password or loi~ gi do
}
bool Register(string user, string pass) {
	ifstream ifs("login.txt");
	if (ifs.fail()) {
		cout << "Error in Opening! File Not Found!!" << endl;
		cout << "\n***Press Enter to continue";
		return 0;
	}
	while (!ifs.eof()) {
		string _user, _pass;
		getline(ifs, _user, '-');
		getline(ifs, _pass);
		if (_user == user)
		{
			cout << "Username has been used";
			ifs.close();
			return 0;
		}
	}
	ifs.close();

	ofstream ofs("login.txt", ios::app);

	ofs << "\n" << user << "-" << pass;
	ofs.close();
	return 1;
}
DWORD WINAPI function_cal(LPVOID arg)
{
	SOCKET* hConnected = (SOCKET*)arg;
	CSocket mysock;
	//Chuyen ve lai CSocket
	mysock.Attach(*hConnected);
	string s;
	string clientName;

	do {
		fflush(stdin);
		mysock.Receive(&clientName, sizeof(clientName), 0);
		int mode;
		cout << "1. Dang nhap: " << endl;
		cout << "2. Dang ki: " << endl;
		cout << "Nhap: ";
		cin >> mode;
		switch (mode)
		{
		case 1:
		{
			string id, pass;
			cout << "Tai khoan: ";
			getline(cin, s);
		}
		}

	} while (true);
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
			int port;
			cout << "Nhap port muon host: ";
			cin >> port;
			cin.ignore();
			server.Create(port);
			do {
				cout << "So client ket noi: ";
				int clients;
				cin >> clients;
				cin.ignore();
				server.Listen(clients);
				server.Accept(s);
				//Khoi tao con tro Socket
				SOCKET* hConnected = new SOCKET();
				//Chuyá»ƒn Ä‘á»i CSocket thanh Socket
				*hConnected = s.Detach();
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				//Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);
			} while (1);
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

