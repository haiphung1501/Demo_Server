// stdafx.cpp : source file that includes just the standard includes
// Demo_Server.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include  <fstream>
#include <string>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

Date stringToDate(string s)
{
	Date x;
	int k = s.find('/');
	string temp;
	temp = s.substr(0, 2);
	x.day = stoi(temp);
	k = s.find('/', k + 1);
	temp = s.substr(k - 2, 2);
	x.month = stoi(temp);
	temp = s.substr(k + 1, 4);
	x.year = stoi(temp);
	return x;
}

int checkDate(Date x)
{
	ifstream ifs("DATE.txt", ios::in);
	
	string s;
	int line = 1;
	Date temp;
	while (!ifs.eof())
	{
		getline(ifs, s);
		temp = stringToDate(s);
		if (x.day >= temp.day && x.month >= temp.month && x.year >= temp.year)
			return line;
		line++;
	}
	return -1;

	ifs.close();
}

double getRate(int line, string type)
{
	ifstream ifs(type + ".txt", ios::in);

	string s;
	for (int i = 0; i < line; i++)
	{
		getline(ifs, s);
	}
	double x = stod(s);
	cout << x;
	return x;
	ifs.close();
}

string exchangeType(int type)
{
	string s;
	switch (type)
	{
		case 1:
		{
			s = "USD";
			break;
		}
		case 2:
		{
			s = "EUR";
			break;
		}
		case 3:
		{
			s = "YEN";
			break;
		}
	}
	return s;
}