#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <cctype>
#include "MyHeader.h"

using namespace std;


void printline(int width) {
	cout << string(width, '=') << endl;
}

void center(int width, string text) {
	int gap = (width - text.length()) / 2;
	cout << string(gap, ' ') << text << string(gap, ' ');
}

void printGap(int width) {
	cout << string(width, ' ');
}


std::vector<std::string> split(string text, string separator) {

	string temp;
	vector<string> result;
	size_t pos = 0;

	while ((pos = text.find(separator)) != string::npos) {

		temp = text.substr(0, pos);
		text.erase(0, pos + separator.length());
		result.push_back(temp);
	}

	if (text != "") {
		result.push_back(text);
	}

	return result;
}

stClient ConvertLinetoClientRecord(string line, string delimiter) {
	stClient client;
	vector<string> data = split(line, delimiter);

	client.AccountNumber = data[0];
	client.PinCode = data[1];
	client.Name = data[2];
	client.Phone = data[3];
	client.Balance = stod(data[4]);

	return client;
}

string ConvertClientRecordToLine(stClient client, string delimiter) {
	return client.AccountNumber + delimiter + client.PinCode + delimiter +
		client.Name + delimiter + client.Phone + delimiter + to_string(client.Balance);
}


void DisplayScreenHeader(int width, const string& title) {
	printline(width);
	center(width, title);
	cout << endl;
	printline(width);
}