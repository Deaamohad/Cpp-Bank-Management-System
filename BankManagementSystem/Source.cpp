#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <cctype>
#include "MyHeader.h"
using namespace std;

// Do "Update" and "Find"

struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double Balance;
};

enum enBankOption { Show = 1, Add, Delete, Update, Find, Exit };
const string FileName = "Clients.txt";


stClient ConvertLinetoRecord(string line, string delimiter = "#//#") {
	
	stClient client;
	vector<string> data = split(line, delimiter);

	client.AccountNumber = data[0];
	client.PinCode = data[1];
	client.Name = data[2];
	client.Phone = data[3];
	client.Balance = stod(data[4]);

	return client;
}

string ConvertRecordToLine(stClient client, string delimiter = "#//#") {
	
	string line = "";

	line += client.AccountNumber + delimiter + client.PinCode
		+ delimiter + client.Name + delimiter + client.Phone + delimiter + to_string(client.Balance);

	return line;

}

vector<stClient> GetClientsFromFile(string FileName) {

	vector<stClient> Clients;
	fstream file;
	file.open(FileName, ios::in);
	string line;

	file.seekg(0);

	while (getline(file, line)) {
		Clients.push_back(ConvertLinetoRecord(line));
	}
	file.close();

	return Clients;

}

void WriteClientsToFile(vector<stClient> &clients, string FileName) {

	fstream file;
	file.open(FileName, ios::out);
	file.seekg(0);
	
	for (stClient& client : clients) {
		file << ConvertRecordToLine(client) << endl;
	}
}

void DisplayClients() {

	vector<stClient> Clients = GetClientsFromFile(FileName);
	string message = "Client list (" + to_string(Clients.size()) + ") Client(s).";


	// Header
	cout << endl; center(90, message); cout << endl;
	printline(90);
	cout << "|"; center(17, "Account Number"); cout << "|";
	center(15, "Pin Code"); cout << "|"; center(30, "Client Name");
	cout << "|"; center(16, "Phone"); cout << "|"; center(10, "Balance");
	cout << endl; printline(90);

	for (stClient& client : Clients) {

		cout << "|"; center(15, client.AccountNumber); cout << "|";
		center(15, client.PinCode); cout << "|"; center(28, client.Name);
		cout << "|"; center(15, client.Phone); cout << "|";
		center(20, to_string(client.Balance)); cout << endl;
	}
}

bool AccountExists(string acc, string FileName) {
	for (stClient& c : GetClientsFromFile(FileName)) {
		if (c.AccountNumber == acc)
			return true;
	}
	return false;
}

void AddNewClient() {

	stClient client;
	fstream file;  
	file.open(FileName, ios::app);
	char repeat;
	cout << endl; printline(30); center(30, "Add New Clients Screen"); cout << endl; printline(30);
	cout << "Adding new Client:\n";
	

	do {
		cin.ignore(1, '\n');
		cout << "\nEnter Account Number: ";
		getline(cin, client.AccountNumber);
		while (AccountExists(client.AccountNumber, FileName)) {
			cout << "Account Number " << client.AccountNumber << " already exists, please enter a different number: ";
			getline(cin, client.AccountNumber);
		}

		cout << "Enter Pin Code: ";
		getline(cin, client.PinCode);
		cout << "Enter Name: ";
		getline(cin, client.Name);
		cout << "Enter Phone: ";
		getline(cin, client.Phone);
		cout << "Enter Balance: ";
		cin >> client.Balance;

		file << ConvertRecordToLine(client) << endl;
		cout << "\nClient (" << client.AccountNumber << ") Added Successfully!\nDo you want to add more Client/s (y/n)? ";
		cin >> repeat;
	} while (tolower(repeat) == 'y');
	
	file.close();
}

void FindClient() {

}


void DeleteClient() {

	string input;
	vector<stClient> vClients = GetClientsFromFile(FileName);
	char choice = 'n';
	

	while (true) {

		cout << "Please enter the Account number to delete: ";
		cin >> input;
		bool found = false;

		for (size_t i = 0; i < vClients.size(); i++) {
			if (vClients[i].AccountNumber == input) {
				found = true;
				vClients.erase(vClients.begin() + i); 
				WriteClientsToFile(vClients, FileName);
				cout << "Deleted Account " << input << " successfully.\n";
				cout << "Would you like to delete another client (y/n)?";
				cin >> choice;
				if (tolower(choice) != 'y')
					return;
				break;
			}
		}

		if (!found) {
			cout << "Account " << input << " not found\n";
		}
	}
}



void SelectOption(int userSelection) {

	switch (static_cast<enBankOption>(userSelection)) {
	case Show:
		DisplayClients();
		break;
	case Add:
		AddNewClient();
		break;
	case Delete:
		DeleteClient();
		break;
	case Update:
		break;
	case Find:
		break;
	// case 6 handeled in main function
	}
}

void DisplayMenu() {

	

	printline(40);
	center(40, "Main Menu Screen"); cout << endl;
	printline(40);
	printGap(10); cout << "[1] Show Client List.\n";
	printGap(10); cout << "[2] Add New Client.\n";
	printGap(10); cout << "[3] Delete Client.\n";
	printGap(10); cout << "[4] Update Client Info.\n";
	printGap(10); cout << "[5] Find Client.\n";
	printGap(10); cout << "[6] Exit.\n";
	printline(40);

	
	
}

int GetUserSelection() {

	int userSelection;
	cout << "Choose what do you want to do (1-6): ";
	cin >> userSelection;
	return userSelection;
}


int main() {
	
	int userSelection;

	while (true) {
		DisplayMenu();
		userSelection = GetUserSelection();

		if (static_cast<enBankOption> (userSelection) == Exit) {
			system("cls");
			break;
		}
		system("cls");
		SelectOption(userSelection);
		cout << "\nPress any key to go back to the main menu:";
		cin.get();
		cin.ignore();
		system("cls");
	}
	

	return 0;
}
