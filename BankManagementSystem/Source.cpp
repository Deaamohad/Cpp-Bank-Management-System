#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include "MyHeader.h"

using namespace std;

const string UsersFile = "Users.txt";
const string FileName = "Clients.txt";

stUser CurrentUser;

void DisplayManageUsersMenu();
void SelectUserOption(int userSelection);
void MainMenu();
vector<stUser> GetUsersFromFile(string UsersFile);

string ConvertUserRecordToLine(stUser user, string delimiter = "#//#") {
    return user.username + delimiter + to_string(user.permissions) + delimiter + user.password;
}

stUser ConvertLineToUserRecord(string line, string delimiter = "#//#") {
    stUser user;
    vector<string> data = split(line, delimiter);

    user.username = data[0];
    user.permissions = stoi(data[1]);
    user.password = data[2];

    return user;
}

vector<stClient> GetClientsFromFile(string FileName) {
    vector<stClient> Clients;
    fstream file;
    file.open(FileName, ios::in);
    string line;

    while (std::getline(file, line)) {
        Clients.push_back(ConvertLinetoClientRecord(line));
    }
    file.close();

    return Clients;
}

void WriteUsersToFile(vector<stUser>& users, string UsersFile) {
    fstream file;
    file.open(UsersFile, ios::out);

    for (stUser& user : users) {
        std::cout << std::noskipws; 
        file << ConvertUserRecordToLine(user) << endl;
    }
    file.close();
}

void WriteClientsToFile(vector<stClient>& clients, string FileName) {
    fstream file;
    file.open(FileName, ios::out);

    for (stClient& client : clients) {
        std::cout << std::noskipws; // no-op placeholder to avoid unused warning
        file << ConvertClientRecordToLine(client) << endl;
    }
    file.close();
}

void PrintClientCard(stClient& client) {
    printline(20);
    std::cout << "Account Number: " << client.AccountNumber << endl;
    std::cout << "Pin Code: " << client.PinCode << endl;
    std::cout << "Name: " << client.Name << endl;
    std::cout << "Phone: " << client.Phone << endl;
    std::cout << "Balance: " << client.Balance << endl;
    printline(20);
}

void ReadClientData(stClient& client) {
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    std::cout << "Enter Pin Code: ";
    std::getline(std::cin, client.PinCode);
    std::cout << "Enter Name: ";
    std::getline(std::cin, client.Name);
    std::cout << "Enter Phone: ";
    std::getline(std::cin, client.Phone);
    std::cout << "Enter Balance: ";
    std::cin >> client.Balance;
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

stUser* FindUserByAccount(vector<stUser>& users, string Username) {
    for (stUser& user : users) {
        if (user.username == Username)
            return &user;
    }
    return nullptr;
}

stClient* FindClientByAccount(vector<stClient>& clients, string accountNumber) {
    for (stClient& client : clients) {
        if (client.AccountNumber == accountNumber)
            return &client;
    }
    return nullptr;
}

int FindUserIndexByAccount(vector<stUser>& users, string username) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username)
            return i;
    }
    return -1;
}

int FindClientIndexByAccount(vector<stClient>& clients, string accountNumber) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i].AccountNumber == accountNumber)
            return i;
    }
    return -1;
}

bool UserExists(string acc, string UsersFile) {
    for (stUser& c : GetUsersFromFile(UsersFile)) {
        if (c.username == acc)
            return true;
    }
    return false;
}

bool ClientExists(string acc, string FileName) {
    for (stClient& c : GetClientsFromFile(FileName)) {
        if (c.AccountNumber == acc)
            return true;
    }
    return false;
}

void DisplayTableHeader(int width, vector<pair<int, string>> columns) {
    printline(width);
    std::cout << "|";
    for (auto& col : columns) {
        center(col.first, col.second);
        std::cout << "|";
    }
    std::cout << endl;
    printline(width);
}

void DisplayTableRow(vector<pair<int, string>> columns) {
    std::cout << "|";
    for (auto& col : columns) {
        center(col.first, col.second);
        std::cout << "|";
    }
    std::cout << endl;
}

void DisplayClients() {
    vector<stClient> Clients = GetClientsFromFile(FileName);
    string message = "Client list (" + to_string(Clients.size()) + ") Client(s).";

    std::cout << endl;
    center(90, message);
    std::cout << endl;

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

    std::cout << "\nPress any key to go back to the main menu.\n";
    std::cin.get();
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void AddNewClient() {
    stClient client;
    fstream file;
    file.open(FileName, ios::app);
    char repeat;

    std::cout << endl;
    printline(30);
    center(30, "Add New Clients Screen");
    std::cout << endl;
    printline(30);
    std::cout << "Adding new Client:\n";

    do {
        std::cout << "\nEnter Account Number (press q to exit): ";
        std::getline(std::cin, client.AccountNumber);

        if (client.AccountNumber == "q")
            return;

        while (ClientExists(client.AccountNumber, FileName)) {
            std::cout << "Account Number " << client.AccountNumber << " already exists, please enter a different number: ";
            std::getline(std::cin, client.AccountNumber);
        }

        ReadClientData(client);
        file << ConvertClientRecordToLine(client) << endl;
        std::cout << "\nClient (" << client.AccountNumber << ") Added Successfully!\nDo you want to add more Client/s (y/n)? ";
        std::cin >> repeat;
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (std::tolower(repeat) == 'y');

    file.close();
}

void FindClient() {
    vector<stClient> vClients = GetClientsFromFile(FileName);
    string input;
    char repeat;

    while (true) {
        std::cout << "\nEnter Account Number (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        stClient* foundClient = FindClientByAccount(vClients, input);
        if (foundClient) {
            PrintClientCard(*foundClient);
            std::cout << "Do you want to search for a different Client (y/n)? ";
            std::cin >> repeat;
            if (std::tolower(repeat) != 'y')
                return;
        } else {
            std::cout << "Account " << input << " not found\n";
        }
    }
}

void TotalBalances() {
    vector<stClient> vClients = GetClientsFromFile(FileName);
    string message = "Balances list (" + to_string(vClients.size()) + ") Client(s).";
    double total = 0;

    std::cout << endl;
    center(90, message);
    std::cout << endl;

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

    std::cout << endl;
    printline(90);
    std::cout << endl;
    message = "Total Balances = " + to_string(total);
    center(90, message);
    std::cout << endl;

    std::cout << "\nPress any key to go back to the menu.\n";
    std::cin.get();
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void UpdateClient() {
    string input;
    vector<stClient> vClients = GetClientsFromFile(FileName);
    char choice;

    while (true) {
        std::cout << "Please enter the Account number to update (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        stClient* foundClient = FindClientByAccount(vClients, input);
        if (foundClient) {
            std::cout << "Found Account " << input << "! Please fill the new information:\n";
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ReadClientData(*foundClient);
            WriteClientsToFile(vClients, FileName);
            std::cout << "\nClient (" << foundClient->AccountNumber << ") Updated Successfully!\nDo you want to update more Client/s (y/n)? ";
            std::cin >> choice;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(choice) != 'y')
                return;
        } else {
            std::cout << "Account " << input << " not found\n";
        }
    }
}

void DeleteClient() {
    string input;
    vector<stClient> vClients = GetClientsFromFile(FileName);
    char choice;

    while (true) {
        std::cout << "Please enter the Account Number to delete (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        int index = FindClientIndexByAccount(vClients, input);
        if (index != -1) {
            vClients.erase(vClients.begin() + index);
            WriteClientsToFile(vClients, FileName);
            std::cout << "Deleted Client " << input << " successfully.\n";
            std::cout << "Would you like to delete another Client (y/n)?";
            std::cin >> choice;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(choice) != 'y')
                return;
        } else {
            std::cout << "Account " << input << " not found\n";
        }
    }
}

void DisplayTransactionsMenu();

int GetUserSelection(int minVal, int maxVal, const string& prompt) {
    int userSelection;

    while (true) {
        std::cout << prompt;
        if (std::cin >> userSelection) {
            if (userSelection >= minVal && userSelection <= maxVal)
                return userSelection;

            std::cout << "Invalid choice! Please enter a number between " << minVal << " and " << maxVal << ".\n";
        }
        else {
            std::cout << "Invalid input! Please enter a valid number.\n";
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


vector<stUser> GetUsersFromFile(string UsersFile) {
    vector<stUser> Users;
    fstream file;
    file.open(UsersFile, ios::in);
    string line;

    while (std::getline(file, line)) {
        Users.push_back(ConvertLineToUserRecord(line));
    }
    file.close();

    return Users;
}

int GetUserMenuSelection() {
    return GetUserSelection(1, 6, "Choose what you want to do (1-6): ");
}

int GetUserTransactionSelection() {
    return GetUserSelection(1, 4, "Choose what you want to do (1-4): ");
}

int GetUserMainSelection() {
    return GetUserSelection(1, 8, "Choose what you want to do (1-8): ");
}


void DisplayTransactionsMenu() {
    DisplayScreenHeader(40, "Transactions Menu Screen");
    printGap(10);
    std::cout << "[1] Deposit.\n";
    printGap(10);
    std::cout << "[2] Withdraw.\n";
    printGap(10);
    std::cout << "[3] Total Balances.\n";
    printGap(10);
    std::cout << "[4] Main Menu.\n";
    printline(40);
}

void TransactionWithdraw() {
    vector<stClient> vClients = GetClientsFromFile(FileName);
    string input;
    double withdraw;
    char confirm;

    while (true) {
        std::cout << "\nEnter Account Number (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        stClient* foundClient = FindClientByAccount(vClients, input);
        if (foundClient) {
            std::cout << "Found Account " << input << ". The current balance is: " << foundClient->Balance << endl;
            std::cout << "Please enter withdraw amount: ";
            std::cin >> withdraw;

            while (withdraw > foundClient->Balance) {
                std::cout << "Withdrawal amount exceeds the balance, please enter a different amount: ";
                std::cin >> withdraw;
            }

            std::cout << "Are you sure you want to perform this action (y/n)? ";
            std::cin >> confirm;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(confirm) != 'y')
                return;

            foundClient->Balance -= withdraw;
            WriteClientsToFile(vClients, FileName);
            return;
        } else {
            std::cout << "Account " << input << " not found\n";
        }
    }
}

void TransactionDeposit() {
    vector<stClient> vClients = GetClientsFromFile(FileName);
    string input;
    double deposit;
    char confirm;

    while (true) {
        std::cout << "\nEnter Account Number (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        stClient* foundClient = FindClientByAccount(vClients, input);
        if (foundClient) {
            std::cout << "Found Account " << input << ". The current balance is: " << foundClient->Balance << endl;
            std::cout << "Please enter deposit amount: ";
            std::cin >> deposit;

            std::cout << "Are you sure you want to perform this action (y/n)? ";
            std::cin >> confirm;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(confirm) != 'y')
                return;

            foundClient->Balance += deposit;
            WriteClientsToFile(vClients, FileName);
            return;
        } else {
            std::cout << "Account " << input << " not found\n";
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

void ManageUsers() {

    system("cls");
    while (true) {
        DisplayManageUsersMenu();
        int userSelection = GetUserMenuSelection();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (static_cast<enUserOption>(userSelection) == UsersBack) {
            system("cls");
            break; // return to the main menu loop
        }

        system("cls");
        SelectUserOption(userSelection);
        system("cls");
    }
}

void NoPermissionScreen() {
    printline(50);
    center(50, "You don't have this permissions"); cout << endl;
    printline(50);
    
    cin.get();
    cin.ignore();
    MainMenu();
}

void SelectMainOption(int userSelection) {
    switch (static_cast<enBankOption>(userSelection)) {
    case Show:
        if ((CurrentUser.permissions & eShowClients) == eShowClients) DisplayClients();
        else NoPermissionScreen();
        break;
    case Add:
        if ((CurrentUser.permissions & eAddNewClients) == eAddNewClients) AddNewClient();
        else NoPermissionScreen();
        break;
    case Delete:
        if ((CurrentUser.permissions & eDeleteClients) == eDeleteClients) DeleteClient();
        else NoPermissionScreen();
        break;
    case Update:
        if ((CurrentUser.permissions & eUpdateClients) == eUpdateClients) UpdateClient();
        else NoPermissionScreen();
        break;
    case Find:
        if ((CurrentUser.permissions & eFindClients) == eFindClients) FindClient();
        else NoPermissionScreen();
        break;
    case Transactions:
        if ((CurrentUser.permissions & eTransactions) == eTransactions) ClientsTransactions();
        else NoPermissionScreen();
        break;
    case Users:
        if ((CurrentUser.permissions & eManageUsers) == eManageUsers) ManageUsers();
        else NoPermissionScreen();
        break;
    case Logout:
        break;
    }
}

void FindUser() {
    vector<stUser> vUsers = GetUsersFromFile(UsersFile);
    string input;
    char repeat;

    while (true) {
        std::cout << "\nEnter Username (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        stUser* foundUser = FindUserByAccount(vUsers, input);
        if (foundUser) {
            cout << "Found User " << input << "!";
            std::cout << "Do you want to search for a different Client (y/n)? ";
            std::cin >> repeat;
            if (std::tolower(repeat) != 'y')
                return;
        }
        else {
            std::cout << "User " << input << " not found\n";
        }
    }
}


void UpdateUser() {
    string username, password;
    vector<stUser> vUsers = GetUsersFromFile(UsersFile);
    char input;

    while (true) {
        std::cout << "Please enter the Username to update (press q to exit): ";
        std::cin >> username;

        if (username == "q")
            return;

        stUser* user = FindUserByAccount(vUsers, username);
        if (user) {
            std::cout << "Found User " << username << "! Please fill the new information:\n";
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter the password: ";
            cin >> user->password;
            cout << "Do you want to give full access y/n? ";
            user->permissions = 0;
            cin >> input;
            if (input != 'y') {
                cout << "Do you want to give access to:\n";
                cout << "Show Clients List y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eShowClients;
                }
                cout << "Add New Clients List y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eAddNewClients;
                }
                cout << "Delete Clients y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eDeleteClients;
                }
                cout << "Update Clients y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eUpdateClients;
                }
                cout << "Find Clients y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eFindClients;
                }
                cout << "Transactions y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eTransactions;
                }
                cout << "Manage Users y/n? ";
                cin >> input;
                if (input == 'y') {
                    user->permissions += enPermissions::eManageUsers;
                }
                else {
                    user->permissions = eAll;
                }
            }
            WriteUsersToFile(vUsers, UsersFile);
            std::cout << "\nUser (" << user->username << ") Updated Successfully!\nDo you want to update more User/s (y/n)? ";
            std::cin >> input;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(input) != 'y')
                return;
        }
        else {
            std::cout << "User " << username << " not found\n";
        }
    }
}


void DeleteUser() {
    string input;
    vector<stUser> vUsers= GetUsersFromFile(UsersFile);
    char choice;

    while (true) {
        std::cout << "Please enter the Username to delete (press q to exit): ";
        std::cin >> input;

        if (input == "q")
            return;

        int index = FindUserIndexByAccount(vUsers, input);
        if (index != -1) {
            vUsers.erase(vUsers.begin() + index);
            WriteUsersToFile(vUsers, UsersFile);
            std::cout << "Deleted User " << input << " successfully.\n";
            std::cout << "Would you like to delete another User (y/n)?";
            std::cin >> choice;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (std::tolower(choice) != 'y')
                return;
        }
        else {
            std::cout << "User " << input << " not found\n";
        }
    }
}


void AddUser() {

    stUser user;
    fstream file;
    file.open(UsersFile, ios::app);
    char input;

    DisplayScreenHeader(40, "Add Users Screen");
    cout << "Adding new Client:\n";
    

    do {
        std::cout << "\nEnter Username (press q to exit): ";
        std::getline(std::cin, user.username);

        if (user.username == "q")
            return;

        while (UserExists(user.username, UsersFile)) {
            std::cout << "User " << user.username << " already exists, please enter a different number: ";
            std::getline(std::cin, user.username);
        }

        cout << "Enter Password: ";
        cin >>  user.password;
        cout << "Do you want to give full access y/n? ";
        cin >> input;
        if (input != 'y') {
            cout << "Do you want to give access to:\n";
            cout << "Show Clients List y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eShowClients;
            }
            cout << "Add New Clients List y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eAddNewClients;
            }
            cout << "Delete Clients y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eDeleteClients;
            }
            cout << "Update Clients y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eUpdateClients;
            }
            cout << "Find Clients y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eFindClients;
            }
            cout << "Transactions y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eTransactions;
            }
            cout << "Manage Users y/n? ";
            cin >> input;
            if (input == 'y') {
                user.permissions += enPermissions::eManageUsers;
            }
            else {
                user.permissions = eAll;
            }
        }
        file << ConvertUserRecordToLine(user) << endl;
        std::cout << "\nUser (" << user.username<< ") Added Successfully!\nDo you want to add more User/s (y/n)? ";
        std::cin >> input;
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (std::tolower(input) == 'y');

    file.close();

}


void DisplayUsers() {
    vector<stUser> Users = GetUsersFromFile(UsersFile);
    string message = "Client list (" + to_string(Users.size()) + ") Client(s).";

    std::cout << endl;
    center(90, message);
    std::cout << endl;

    vector<pair<int, string>> columns = {
        {20, "User Name"},
        {20, "Permission"},
        {20, "Password"}
    };
    DisplayTableHeader(90, columns);

    for (stUser& user: Users) {
        vector<pair<int, string>> row = {
            {20, user.username},
            {20, to_string(user.permissions)},
            {20, user.password}
        };
        DisplayTableRow(row);
    }

    cout << "\nPress any key to go back to the main menu.\n";
    cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}



void SelectUserOption(int userSelection) {
    switch (static_cast<enUserOption>(userSelection)) {
    case UsersList:
        DisplayUsers();
        break;
    case UsersAdd:
        AddUser();
        break;
    case UsersDelete:
        DeleteUser();
        break;
    case UsersUpdate:
        UpdateUser();
        break;
    case UsersFind:
        FindUser();
        break;
    case UsersBack:
        MainMenu();
        break;
    }
}


void DisplayClientsMenu() {
    DisplayScreenHeader(40, "Main Menu Screen");
    printGap(10);
    std::cout << "[1] Show Client List.\n";
    printGap(10);
    std::cout << "[2] Add New Client.\n";
    printGap(10);
    std::cout << "[3] Delete Client.\n";
    printGap(10);
    std::cout << "[4] Update Client Info.\n";
    printGap(10);
    std::cout << "[5] Find Client.\n";
    printGap(10);
    std::cout << "[6] Transactions.\n";
    printGap(10);
    std::cout << "[7] Manage Users.\n";
    printGap(10);
    std::cout << "[8] Logout.\n";
    printline(40);
}


void DisplayManageUsersMenu() {

    DisplayScreenHeader(40, "Manage Users Menu Screen");
    printGap(10);
    std::cout << "[1] List Users.\n";
    printGap(10);
    std::cout << "[2] Add New User.\n";
    printGap(10);
    std::cout << "[3] Delete User.\n";
    printGap(10);
    std::cout << "[4] Update User.\n";
    printGap(10);
    std::cout << "[5] Find User.\n";
    printGap(10);
    std::cout << "[6] Main Menu.\n";
    printline(40);
}

bool IsValidUser(string username, string password) {
    vector<stUser> Users = GetUsersFromFile(UsersFile);
    for (stUser& user : Users) {
        if (user.username == username && user.password == password)
            return true;
    }
    return false;
}

void LoginMenu() {

    vector<stUser> vUsers = GetUsersFromFile(UsersFile);


    string username, password;
    DisplayScreenHeader(30, "Login Screen");

    std::cout << "Enter UserName: ";
    std::getline(std::cin, username);

    std::cout << "Enter Password: ";
    std::getline(std::cin, password);

    while (!IsValidUser(username, password)) {
        std::cout << "Invalid Username/Password!\n";
        std::cout << "Enter UserName: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);
    }

    CurrentUser = *FindUserByAccount(vUsers, username);

}

void MainMenu() {

    system("cls");
    while (true) {
        DisplayClientsMenu();
        int userSelection = GetUserMainSelection();
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (static_cast<enBankOption>(userSelection) == Logout) {
            system("cls");
            LoginMenu();
        }

        system("cls");
        SelectMainOption(userSelection);
        system("cls");
    }
}

void Login() {

    LoginMenu();
    MainMenu();

}


int main() {

    Login();

    return 0;
}
