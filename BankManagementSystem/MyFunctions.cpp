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
