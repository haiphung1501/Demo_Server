#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"
#include <string>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// The one and only application object

CWinApp theApp;

string charToString(char* arr) {
	string s(arr);
	return s;
}
int checkLogin(string user, string pass) {
	ifstream ifs("login.txt");
	if (ifs.fail()) {
		cout << "Error in Opening! File Not Found!!" << endl;
		return 0;
	}
	string _user, _pass;
	while (!ifs.eof()) {
		getline(ifs, _user, '-');
		getline(ifs, _pass);
		if (_user == user && _pass == pass)
		{
			ifs.close();
			return 1;
		}
	}
	ifs.close();
	return 0; // wrong password or loi~ gi do
}
int Register(string user, string pass) {
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
string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
DWORD WINAPI function_cal(LPVOID arg)
{
	SOCKET* hConnected = (SOCKET*)arg;
	CSocket mysock;
	//Chuyen ve lai CSocket
	mysock.Attach(*hConnected);

	int number_continue = 1;
	int choice, check;
	string user, pass;
	//Code
	cout << currentDateTime();
	do {
		fflush(stdin);
		mysock.Receive(&choice, sizeof(choice), 0);
		cout << choice;
		if (choice == 0) {
			number_continue = 0;
		}
		else if (choice == 1) {
			// Dang Nhap
			mysock.Receive(&user, sizeof(user), 0);
			mysock.Receive(&pass, sizeof(pass), 0);

			check = checkLogin(user, pass);
			mysock.Send(&check, sizeof(check), 0);
		}
		else if (choice == 2) {
			mysock.Receive(&user, sizeof(user), 0);
			mysock.Receive(&pass, sizeof(pass), 0);

			check = Register(user, pass);
			mysock.Send(&check, sizeof(check), 0);
		}
		else {
			break;
		}
	} while (number_continue);

	//Code
	cout << "Function Server called";
	//Code
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
			int port = 1234;
			int clients = 1;
			int i = 0;
			char sAdd[] = "127.0.0.1";
			//cout << "Enter port to host: ";
			//cin >> port;
			//cin.ignore();
			//cout << "Enter IP to host: ";
			//cin.getline(sAdd, 100);
			//cout << "Enter number of Client(s): ";
			//cin >> clients;
			//CSocket* s = new CSocket[clients];
			server.Create(port, 1, CA2W(sAdd));
			//for (int i = 0; i < clients; i++)
			do {
				cout << "Server is Listening from Clients\n";
				server.Listen();
				server.Accept(s);
				cout << "Accepted Client number " << i + 1 << endl;
				//Khoi tao con tro Socket
				SOCKET* hConnected = new SOCKET();
				//Chuyá»ƒn Ä‘á»i CSocket thanh Socket
				*hConnected = s.Detach();
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				//Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);
				//s.Attach(*hConnected);
				++i;
			} while ( i <= clients + 1);

			//server.Close();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;

	//TEST
}

