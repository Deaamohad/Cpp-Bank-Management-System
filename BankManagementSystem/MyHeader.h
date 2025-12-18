#pragma once
#include <string>
#include <vector>

// Client structure (moved here so other modules can use it without Clients.h)
struct stClient {
	std::string AccountNumber;
	std::string PinCode;
	std::string Name;
	std::string Phone;
	double Balance = 0;
};

// User structure
struct stUser {
	std::string username;
	std::string password;
};

// Enums for main menu and transactions
enum enBankOption { Show = 1, Add, Delete, Update, Find, Transactions, Users, Logout };
enum enTransactionOption { Deposit = 1, Withdraw, Total, Back };
enum enUserOption { UsersList = 1, UsersAdd, UsersDelete, UsersUpdate, UsersFind, UsersBack };

// Conversion helpers for client records
stClient ConvertLinetoClientRecord(std::string line, std::string delimiter = "#//#");
std::string ConvertClientRecordToLine(stClient client, std::string delimiter = "#//#");

void printline(int width);
void center(int width, std::string text);
void printGap(int width);
std::vector<std::string> split(std::string text, std::string separator);

// Display helper used across files
void DisplayScreenHeader(int width, const std::string& title);
