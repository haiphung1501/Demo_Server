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
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
string getAPIkey() {
	CURL* c = curl_easy_init();
	string readBuffer;
	struct curl_slist* headers = NULL; // init to NULL is important

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	headers = curl_slist_append(headers, "Accept: text/html");
	if (c)
	{
		//curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(c, CURLOPT_URL, "https://vapi.vnappmob.com/api/request_api_key?scope=exchange_rate");
		curl_easy_setopt(c, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(c, CURLOPT_WRITEDATA, &readBuffer);
		CURLcode res = curl_easy_perform(c);
		if (res != CURLE_OK)
		{
			cout << "error";
		}
		else
		{
			long responseCode;
			curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &responseCode);
		}
		curl_easy_cleanup(c);
	}
	readBuffer = readBuffer.substr(12, readBuffer.length());
	readBuffer = readBuffer.substr(0, readBuffer.length() - 3);
	return readBuffer;
}
string getHTTPRequest() {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	std::string apikey = getAPIkey();

	std::string apiHeader = "Authorization: Bearer " + apikey;

	struct curl_slist* headers = NULL; // init to NULL is important

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	headers = curl_slist_append(headers, apiHeader.c_str());

	curl = curl_easy_init();
	if (curl)
	{
		std::string url = "https://vapi.vnappmob.com/api/v2/exchange_rate/sbv";

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
	//system("CLS");
	return readBuffer;
}
vector <Currency> onlineData() {
	ofstream ofs("result.txt");
	ofs << getHTTPRequest();
	ofs.close();
	string ch;
	double temp;
	vector <Currency> cur;
	ifstream ifs("result.txt");
	getline(ifs, ch, ':');
	for (int i = 0; i < 7; ++i) {
		cur.push_back(Currency());
		getline(ifs, ch, ':');
		ifs >> temp;
		cur[i].buy = temp;
		getline(ifs, ch, ':');
		getline(ifs, ch, ',');
		ch.erase(std::remove(ch.begin(), ch.end(), '\"'), ch.end());
		cur[i].name = ch;
		getline(ifs, ch, ':');
		ifs >> temp;
		cur[i].sell = temp;
	}
	return cur;
}
Currency FindClientCur(int pos, vector <Currency> cur) {
	return cur[pos - 1];
}