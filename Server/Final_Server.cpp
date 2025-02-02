﻿#include "stdafx.h"
#include "Demo_Server.h"
#include "afxsock.h"

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
DWORD WINAPI function_cal(LPVOID arg)
{
	SOCKET* hConnected = (SOCKET*)arg;
	CSocket mysock;
	//Chuyen ve lai CSocket
	mysock.Attach(*hConnected);
	int number_continue = 1;
	int choice, choice_dummy, check;
	string user, pass;
	
	//Code
	do {
		fflush(stdin);
		mysock.Receive(&choice, sizeof(choice), 0);
		
		if (choice == 0) {
			number_continue = 0;
		}
		if (choice == 1) {
			// Dang Nhap
			mysock.Receive(&user, sizeof(user), 0);
			mysock.Receive(&pass, sizeof(pass), 0);
			check = checkLogin(user, pass);
			mysock.Send(&check, sizeof(check), 0);
			if (check == 1)
				number_continue = 0;
		}
		if (choice == 2) {
			mysock.Receive(&user, sizeof(user), 0);
			mysock.Receive(&pass, sizeof(pass), 0);

			check = Register(user, pass);
			mysock.Send(&check, sizeof(check), 0);
		}
	} while (number_continue);
	number_continue = 1;
	int type, line;
	string dateString, typeString;
	Date date;
	double amount;
	if (choice != 0) {
		do {
			fflush(stdin);
			mysock.Receive(&choice_dummy, sizeof(choice_dummy), 0);
			if (choice_dummy == 1) {
				mysock.Receive(&choice_dummy, sizeof(choice_dummy), 0);
				if (choice_dummy != 0) {
					Currency cur = FindClientCur(choice_dummy, onlineData());
					mysock.Send(&cur.name, sizeof(cur.name), 0);
					mysock.Send(&cur.sell, sizeof(cur.sell), 0);
					mysock.Send(&cur.buy, sizeof(cur.buy), 0);
				}
				else
					continue;
			}
			else if (choice_dummy == 2) {
				mysock.Receive(&type, sizeof(type), 0);
				if (type != 0)
				{
					mysock.Receive(&dateString, sizeof(dateString), 0);
					date = stringToDate(dateString);
					line = checkDate(date);
					typeString = exchangeType(type);
					if (line != -1)
					{
						amount = getRate(line, typeString);
						mysock.Send(&typeString, sizeof(typeString), 0);
						mysock.Send(&amount, sizeof(amount), 0);
					}
					else
						cout << "Error";
				}
			}
		} while (number_continue);
	}
	//Code
	mysock.Close();
	cout << "Client Disconnected!";
	//Code
	delete hConnected;
	return 100;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	Logo();
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
			CSocket server;
			DWORD threadID;
			HANDLE threadStatus;
			int port;
			int clients;
			char sAdd[100];
			cout << "Enter IP to host: ";
			cin.getline(sAdd, 100);
			cout << "Enter port to host: ";
			cin >> port;
			cout << "Enter number of Client(s): ";
			cin >> clients;
			CSocket* s = new CSocket[clients];
			server.Create(port, 1, CA2W(sAdd));
			for (int i = 0; i < clients; i++) {
			//do {
				cout << "Server is Listening from Clients\n";
				server.Listen();
				server.Accept(s[i]);
				Textcolor(10);
				cout << "Accepted Client number " << i + 1 << endl;
				Textcolor(7);
				//Khoi tao con tro Socket
				SOCKET* hConnected = new SOCKET();
				//Chuyá»ƒn Ä‘á»i CSocket thanh Socket
				*hConnected = s[i].Detach();
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				//Nhu vay moi client se doc lap nhau, khong phai cho doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);
				s[i].Attach(*hConnected);
			}
			server.Close();
			string stop_sign;
			cin.ignore();
			while (1) {
				Textcolor(12);
				cout << "Press 'stop' to stop server and send notifications to all clients\n";
				Textcolor(7);
				getline(cin, stop_sign);
				if (stop_sign == "stop") {
					for (int i = 0; i < clients; ++i) {
						try {
							s[i].Send(&stop_sign, sizeof(stop_sign), 0);
						}
						catch (const exception& e) {
							cout << "Error";
						}
					}
					cout << endl << "SERVER STOP WORKING! " << endl;
					_getch();
					exit(0);
				}
			}
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

