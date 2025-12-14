#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <cctype>
#include <limits>
#include "MyHeader.h"
using namespace std;

struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double Balance = 0;
};

enum enBankOption { Show = 1, Add, Delete, Update, Find, Transactions, Exit };
enum enTransactionOption { Deposit = 1, Withdraw, Total, Back };
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
	return client.AccountNumber + delimiter + client.PinCode + delimiter + 
	       client.Name + delimiter + client.Phone + delimiter + to_string(client.Balance);
}

vector<stClient> GetClientsFromFile(string FileName) {
	vector<stClient> Clients;
	fstream file;
	file.open(FileName, ios::in);
	string line;

	while (getline(file, line)) {
		Clients.push_back(ConvertLinetoRecord(line));
	}
	file.close();

	return Clients;
}

void WriteClientsToFile(vector<stClient> &clients, string FileName) {
	fstream file;
	file.open(FileName, ios::out);
	
	for (stClient& client : clients) {
		file << ConvertRecordToLine(client) << endl;
	}
	file.close();
}

void PrintClientCard(stClient& client) {
	printline(20);
	cout << "Account Number: " << client.AccountNumber << endl;
	cout << "Pin Code: " << client.PinCode << endl;
	cout << "Name: " << client.Name << endl;
	cout << "Phone: " << client.Phone << endl;
	cout << "Balance: " << client.Balance << endl;
	printline(20);
}

void ReadClientData(stClient& client) {
	cout << "Enter Pin Code: ";
	getline(cin, client.PinCode);
	cout << "Enter Name: ";
	getline(cin, client.Name);
	cout << "Enter Phone: ";
	getline(cin, client.Phone);
	cout << "Enter Balance: ";
	cin >> client.Balance;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

stClient* FindClientByAccount(vector<stClient>& clients, string accountNumber) {
	for (stClient& client : clients) {
		if (client.AccountNumber == accountNumber)
			return &client;
	}
	return nullptr;
}

int FindClientIndexByAccount(vector<stClient>& clients, string accountNumber) {
	for (size_t i = 0; i < clients.size(); i++) {
		if (clients[i].AccountNumber == accountNumber)
			return i;
	}
	return -1;
}

bool AccountExists(string acc, string FileName) {
	for (stClient& c : GetClientsFromFile(FileName)) {
		if (c.AccountNumber == acc)
			return true;
	}
	return false;
}

void DisplayTableHeader(int width, vector<pair<int, string>> columns) {
	printline(width);
	cout << "|";
	for (auto& col : columns) {
		center(col.first, col.second);
		cout << "|";
	}
	cout << endl;
	printline(width);
}

void DisplayTableRow(vector<pair<int, string>> columns) {
	cout << "|";
	for (auto& col : columns) {
		center(col.first, col.second);
		cout << "|";
	}
	cout << endl;
}

void DisplayClients() {
	vector<stClient> Clients = GetClientsFromFile(FileName);
	string message = "Client list (" + to_string(Clients.size()) + ") Client(s).";

	cout << endl;
	center(90, message);
	cout << endl;

	vector<pair<int, string>> columns = {
		{17, "Account Number"},
		{15, "Pin Code"},
		{30, "Client Name"},
		{16, "Phone"},
		{10, "Balance"}
	};
	DisplayTableHeader(90, columns);

	for (stClient& client : Clients) {
		vector<pair<int, string>> row = {
			{15, client.AccountNumber},
			{15, client.PinCode},
			{28, client.Name},
			{15, client.Phone},
			{20, to_string(client.Balance)}
		};
		DisplayTableRow(row);
	}

	cout << "\nPress any key to go back to the main menu.\n";
	cin.get();
	cin.ignore();
}

void AddNewClient() {
	stClient client;
	fstream file;  
	file.open(FileName, ios::app);
	char repeat;

	cout << endl;
	printline(30);
	center(30, "Add New Clients Screen");
	cout << endl;
	printline(30);
	cout << "Adding new Client:\n";

	do {
		cout << "\nEnter Account Number (press q to exit): ";
		getline(cin, client.AccountNumber);

		if (client.AccountNumber == "q")
			return;

		while (AccountExists(client.AccountNumber, FileName)) {
			cout << "Account Number " << client.AccountNumber << " already exists, please enter a different number: ";
			getline(cin, client.AccountNumber);
		}

		ReadClientData(client);
		file << ConvertRecordToLine(client) << endl;
		cout << "\nClient (" << client.AccountNumber << ") Added Successfully!\nDo you want to add more Client/s (y/n)? ";
		cin >> repeat;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (tolower(repeat) == 'y');
	
	file.close();
}

void FindClient() {
	vector<stClient> vClients = GetClientsFromFile(FileName);
	string input;
	char repeat;

	while (true) {
		cout << "\nEnter Account Number (press q to exit): ";
		cin >> input;

		if (input == "q")
			return;

		stClient* foundClient = FindClientByAccount(vClients, input);
		if (foundClient) {
			PrintClientCard(*foundClient);
			cout << "Do you want to search for a different Client (y/n)? ";
			cin >> repeat;
			if (tolower(repeat) != 'y')
				return;
		} else {
			cout << "Account " << input << " not found\n";
		}
	}
}

void TotalBalances() {
	vector<stClient> vClients = GetClientsFromFile(FileName);
	string message = "Balances list (" + to_string(vClients.size()) + ") Client(s).";
	double total = 0;

	cout << endl;
	center(90, message);
	cout << endl;

	vector<pair<int, string>> columns = {
		{16, "Account Number"},
		{30, "Client Name"},
		{10, "Balance"}
	};
	DisplayTableHeader(90, columns);

	for (stClient& client : vClients) {
		vector<pair<int, string>> row = {
			{16, client.AccountNumber},
			{30, client.Name},
			{20, to_string(client.Balance)}
		};
		DisplayTableRow(row);
		total += client.Balance;
	}

	cout << endl;
	printline(90);
	cout << endl;
	message = "Total Balances = " + to_string(total);
	center(90, message);
	cout << endl;
	
	cout << "\nPress any key to go back to the menu.\n";
	cin.get();
	cin.ignore();
}

void UpdateClient() {
	string input;
	vector<stClient> vClients = GetClientsFromFile(FileName);
	char choice;

	while (true) {
		cout << "Please enter the Account number to update (press q to exit): ";
		cin >> input;

		if (input == "q")
			return;

		stClient* foundClient = FindClientByAccount(vClients, input);
		if (foundClient) {
			cout << "Found Account " << input << "! Please fill the new information:\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			ReadClientData(*foundClient);
			WriteClientsToFile(vClients, FileName);
			cout << "\nClient (" << foundClient->AccountNumber << ") Updated Successfully!\nDo you want to update more Client/s (y/n)? ";
			cin >> choice;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (tolower(choice) != 'y')
				return;
		} else {
			cout << "Account " << input << " not found\n";
		}
	}
}

void DeleteClient() {
	string input;
	vector<stClient> vClients = GetClientsFromFile(FileName);
	char choice;

	while (true) {
		cout << "Please enter the Account number to delete (press q to exit): ";
		cin >> input;

		if (input == "q")
			return;

		int index = FindClientIndexByAccount(vClients, input);
		if (index != -1) {
			vClients.erase(vClients.begin() + index);
			WriteClientsToFile(vClients, FileName);
			cout << "Deleted Account " << input << " successfully.\n";
			cout << "Would you like to delete another Client (y/n)?";
			cin >> choice;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (tolower(choice) != 'y')
				return;
		} else {
			cout << "Account " << input << " not found\n";
		}
	}
}

int GetUserSelection(int minVal, int maxVal, const string& prompt) {
	int userSelection;

	while (true) {
		cout << prompt;
		if (cin >> userSelection) {
			if (userSelection >= minVal && userSelection <= maxVal)
				return userSelection;

			cout << "Invalid choice! Please enter a number between " << minVal << " and " << maxVal << ".\n";
		}
		else {
			cout << "Invalid input! Please enter a valid number.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

int GetUserTransactionSelection() {
	return GetUserSelection(1, 4, "Choose what you want to do (1-4): ");
}

int GetUserMainSelection() {
	return GetUserSelection(1, 7, "Choose what you want to do (1-7): ");
}

void DisplayScreenHeader(int width, const string& title) {
	printline(width);
	center(width, title);
	cout << endl;
	printline(width);
}

void DisplayTransactionsMenu() {
	DisplayScreenHeader(40, "Transactions Menu Screen");
	printGap(10);
	cout << "[1] Deposit.\n";
	printGap(10);
	cout << "[2] Withdraw.\n";
	printGap(10);
	cout << "[3] Total Balances.\n";
	printGap(10);
	cout << "[4] Main Menu.\n";
	printline(40);
}

void TransactionWithdraw() {
	vector<stClient> vClients = GetClientsFromFile(FileName);
	string input;
	double withdraw;
	char confirm;

	while (true) {
		cout << "\nEnter Account Number (press q to exit): ";
		cin >> input;

		if (input == "q")
			return;

		stClient* foundClient = FindClientByAccount(vClients, input);
		if (foundClient) {
			cout << "Found Account " << input << ". The current balance is: " << foundClient->Balance << endl;
			cout << "Please enter withdraw amount: ";
			cin >> withdraw;

			while (withdraw > foundClient->Balance) {
				cout << "Withdrawal amount exceeds the balance, please enter a different amount: ";
				cin >> withdraw;
			}

			cout << "Are you sure you want to perform this action (y/n)? ";
			cin >> confirm;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (tolower(confirm) != 'y')
				return;

			foundClient->Balance -= withdraw;
			WriteClientsToFile(vClients, FileName);
			return;
		} else {
			cout << "Account " << input << " not found\n";
		}
	}
}

void TransactionDeposit() {
	vector<stClient> vClients = GetClientsFromFile(FileName);
	string input;
	double deposit;
	char confirm;

	while (true) {
		cout << "\nEnter Account Number (press q to exit): ";
		cin >> input;

		if (input == "q")
			return;

		stClient* foundClient = FindClientByAccount(vClients, input);
		if (foundClient) {
			cout << "Found Account " << input << ". The current balance is: " << foundClient->Balance << endl;
			cout << "Please enter deposit amount: ";
			cin >> deposit;

			cout << "Are you sure you want to perform this action (y/n)? ";
			cin >> confirm;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (tolower(confirm) != 'y')
				return;

			foundClient->Balance += deposit;
			WriteClientsToFile(vClients, FileName);
			return;
		} else {
			cout << "Account " << input << " not found\n";
		}
	}
}

void SelectTransactionOption(int userSelection) {
	switch (static_cast<enTransactionOption>(userSelection)) {
	case Deposit:
		TransactionDeposit();
		break;
	case Withdraw:
		TransactionWithdraw();
		break;
	case Total:
		TotalBalances();
		break;
	}
}

void ClientsTransactions() {
	while (true) {
		DisplayTransactionsMenu();
		int userSelection = GetUserTransactionSelection();

		if (static_cast<enTransactionOption>(userSelection) == Back) {
			system("cls");
			break;
		}

		system("cls");
		SelectTransactionOption(userSelection);
		system("cls");
	}
}

void SelectMainOption(int userSelection) {
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
		UpdateClient();
		break;
	case Find:
		FindClient();
		break;
	case Transactions:
		ClientsTransactions();
		break;
	}
}

void DisplayMenu() {
	DisplayScreenHeader(40, "Main Menu Screen");
	printGap(10);
	cout << "[1] Show Client List.\n";
	printGap(10);
	cout << "[2] Add New Client.\n";
	printGap(10);
	cout << "[3] Delete Client.\n";
	printGap(10);
	cout << "[4] Update Client Info.\n";
	printGap(10);
	cout << "[5] Find Client.\n";
	printGap(10);
	cout << "[6] Transactions.\n";
	printGap(10);
	cout << "[7] Exit.\n";
	printline(40);
}

int main() {
	while (true) {
		DisplayMenu();
		int userSelection = GetUserMainSelection();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (static_cast<enBankOption>(userSelection) == Exit) {
			system("cls");
			break;
		}

		system("cls");
		SelectMainOption(userSelection);
		system("cls");
	}

	return 0;
}
