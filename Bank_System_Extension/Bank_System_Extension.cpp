#include<iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

void ShowMainMenu();
void ShowTransactionsMenu();
const string FileName = "Clients.txt";

struct sClient {
    string AccNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccBalance;
    bool MarkForDelete = false;
};

enum enMainMenuOptions {
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eExit = 7
};

enum enTransactionMenuOptions {
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenu = 4
};

short ReadMainMenueOption(short Start, short End)
{
    short Choice = 0;

    do {
        cout << "\nChoose what do you want to do [" << Start << " to " << End << "] ? ";
        cin >> Choice;
    } while (Choice < Start || Choice > End);

    return Choice;
}

vector<string> SplitString(string DataLine, string delim = "#//#")
{
    int pos = 0;
    vector<string> vJoinString;

    while ((pos = DataLine.find(delim)) != string::npos)
    {
        vJoinString.push_back(DataLine.substr(0, pos));
        DataLine.erase(0, pos + delim.length());
    }
    if (DataLine != "")
        vJoinString.push_back(DataLine);

    return vJoinString;
}

sClient ConvertLineToRecord(string DataLine, string delim = "#//#")
{
    vector<string> vDataClient = SplitString(DataLine, delim);
    sClient Client;

    Client.AccNumber = vDataClient[0];
    Client.PinCode = vDataClient[1];
    Client.Name = vDataClient[2];
    Client.Phone = vDataClient[3];
    Client.AccBalance = stod(vDataClient[4]);

    return Client;
}

vector<sClient> LoadDataFromFile(string ClientsFileName)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::in);
    string DataLine = "";
    vector<sClient> vClients;
    sClient Client;

    if (MyFile.is_open())
    {
        while (getline(MyFile, DataLine))
        {
            if (DataLine != "") {
                Client = ConvertLineToRecord(DataLine);
                vClients.push_back(Client);
            }
        }
        MyFile.close();
    }

    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << left << setw(15) << Client.AccNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(12) << Client.Phone;
    cout << "| " << left << setw(12) << Client.AccBalance << endl;
}

void ShowAllClientsScreen()
{
    vector<sClient> vClients = LoadDataFromFile(FileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else {
        for (sClient& Client : vClients)
        {
            PrintClientRecordLine(Client);
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool ClientExistsByAccountNumber(string AccountNumber)
{
    vector<sClient> vClients = LoadDataFromFile(FileName);

    for (sClient& Client : vClients)
    {
        if (AccountNumber == Client.AccNumber)
        {
            return true;
        }
    }

    return false;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccNumber);

    while (ClientExistsByAccountNumber(Client.AccNumber)) {
        cout << "\nClient with [" << Client.AccNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccBalance;

    return Client;
}

string ConvertRecordToLine(sClient Client, string delim = "#//#")
{
    string JoinRecordToString = "";

    JoinRecordToString += (Client.AccNumber + delim);
    JoinRecordToString += (Client.PinCode + delim);
    JoinRecordToString += (Client.Name + delim);
    JoinRecordToString += (Client.Phone + delim);
    JoinRecordToString += to_string(Client.AccBalance);

    return JoinRecordToString;
}

void AddDataLineToFile(string stDataLine, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        if (stDataLine != "")
        {
            MyFile << stDataLine << endl;
        }
        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();

    AddDataLineToFile(ConvertRecordToLine(Client), FileName);
}

void AddNewClients()
{
    char AddMore = 'Y';

    do {
        // system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

bool FindClientByAccountNumber(string AccountNumber, sClient& Client, vector<sClient> vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number  : " << Client.AccNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccBalance;
    cout << "\n-----------------------------------\n";
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, sClient Client)
{
    if (AccountNumber == Client.AccNumber)
    {
        Client.MarkForDelete = true;
        return true;
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& Client : vClients) {
        if (AccountNumber == Client.AccNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<sClient> SaveCleintsDataToFile(vector<sClient> vClients, string ClientsFileName)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    string DataLine = "";

    if (MyFile.is_open())
    {
        for (sClient& Client : vClients)
        {
            if (!Client.MarkForDelete)
            {
                DataLine = ConvertRecordToLine(Client);
                if (DataLine != "")
                    MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }

    return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient> vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client(Y/N)? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(vClients, FileName);

            // Refresh

            vClients = LoadDataFromFile(FileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found!";
        return false;
    }
}

void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(FileName);
    string AccountNumber = ReadClientAccountNumber();

    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void BackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause > 0");
    ShowMainMenu();
}

void BackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause > 0");
    ShowTransactionsMenu();
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccNumber = AccountNumber;
    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccBalance;

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient> vClients)
{
    char Answer = 'n';
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client(Y/N)? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(vClients, FileName);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(FileName);
    string AccountNumber = ReadClientAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector<sClient> vClients = LoadDataFromFile(FileName);
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number[" << AccountNumber << "] is NOT found!";
    }
}

void PerfomMenuOption(enMainMenuOptions MainMenuOptions)
{
    switch (MainMenuOptions)
    {
    case enMainMenuOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        BackToMainMenu();
        break;
    case enMainMenuOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        BackToMainMenu();
        break;
    case enMainMenuOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        BackToMainMenu();
        break;
    case enMainMenuOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        BackToMainMenu();
        break;
    case enMainMenuOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        BackToMainMenu();
        break;
    case enMainMenuOptions::eTransactions:
        system("cls");
        ShowTransactionsMenu();
        break;
    case enMainMenuOptions::eExit:
        system("cls");
        ShowEndScreen();
        break;
    }
}

double ReadDeositAmount()
{
    double Deposit = 0.0;
    do {
        cout << "Enter deposit amount? ";
        cin >> Deposit;
    } while (Deposit < 0);

    return Deposit;
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    vector<sClient> vClients = LoadDataFromFile(FileName);
    double DepositeAmount = 0.0;

    while (!FindClientByAccountNumber(AccountNumber, Client, vClients)) {
        cout << "\nClient with [" << AccountNumber << "] does NOT exist.\n";

        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    DepositeAmount = ReadDeositAmount();
    char Answer = 'y';
    cout << "\nAre you sure you want to perform this transaction(Y/N)? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccNumber == AccountNumber)
            {
                C.AccBalance += DepositeAmount;
                Client.AccBalance += DepositeAmount;
                break;
            }
        }

        SaveCleintsDataToFile(vClients, FileName);

        cout << "\nThis Transaction Done Successfully, New Balance is " << Client.AccBalance << ".";
    }

    // DepositBalanceToClientByAccountNumber(AccountNumber, vClients, DepositeAmount);
}

double ReadWithdrawAmount(double Balance)
{
    double Withdraw = 0.0;
    do {
        cout << "Enter withdraw amount? ";
        cin >> Withdraw;
        while (Withdraw > Balance) {
            cout << "\nAmount Exceeds the balance, you can withdraw up to: " << Balance << endl;
            cout << "Enter another amount? ";
            cin >> Withdraw;
        }
    } while (Withdraw < 0);

    return Withdraw;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<sClient> vClients, double Amount)
{
    char Answer = 'y';
    cout << "\nAre you sure you want to perform this transaction(Y/N)? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& Client : vClients)
        {
            if (Client.AccNumber == AccountNumber)
            {
                Client.AccBalance -= Amount;
                SaveCleintsDataToFile(vClients, FileName);
                cout << "\nThis Transaction Done Successfully, New Balance is " << Client.AccBalance << ".";
                return true;
            }
        }
    }

    return false;
}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Withdraw Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    vector<sClient> vClients = LoadDataFromFile(FileName);
    double WithdrawAmount = 0.0;

    while (!FindClientByAccountNumber(AccountNumber, Client, vClients)) {
        cout << "\nClient with [" << AccountNumber << "] does NOT exist.\n";

        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    WithdrawAmount = ReadWithdrawAmount(Client.AccBalance);
    char Answer = 'y';
    cout << "\nAre you sure you want to perform this transaction(Y/N)? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccNumber == AccountNumber)
            {
                C.AccBalance -= WithdrawAmount;
                Client.AccBalance -= WithdrawAmount;
                break;
            }
        }

        SaveCleintsDataToFile(vClients, FileName);

        cout << "\nThis Transaction Done Successfully, New Balance is " << Client.AccBalance << ".";
    }

    // DepositBalanceToClientByAccountNumber(AccountNumber, vClients, WithdrawAmount * -1);
}

void PrintBalanceRecordLine(sClient Client)
{
    cout << "| " << left << setw(15) << Client.AccNumber;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(12) << Client.AccBalance << endl;
}

void ShowAllBalancesScreen()
{
    vector<sClient> vClients = LoadDataFromFile(FileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else {
        for (sClient& Client : vClients)
        {
            PrintBalanceRecordLine(Client);
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowTotalBalancesScreen()
{
    vector<sClient> vClients = LoadDataFromFile(FileName);
    double TotalBalances = 0.0;

    for (sClient& Client : vClients)
    {
        TotalBalances += Client.AccBalance;
    }

    ShowAllBalancesScreen();
    cout << "\t\t\t\t\t\t\t\t\tTotal Balances: " << TotalBalances;
}

void PerfomTransactionsOption(enTransactionMenuOptions MainMenuOptions)
{
    switch (MainMenuOptions)
    {
    case enTransactionMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        BackToTransactionsMenu();
        break;
    case enTransactionMenuOptions::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        BackToTransactionsMenu();
        break;
    case enTransactionMenuOptions::eTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        BackToTransactionsMenu();
        break;
    case enTransactionMenuOptions::eMainMenu:
        ShowMainMenu();
    }
}

void ShowTransactionsMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "===========================================\n";
    PerfomTransactionsOption((enTransactionMenuOptions)ReadMainMenueOption(1, 4));
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";
    PerfomMenuOption((enMainMenuOptions)ReadMainMenueOption(1, 7));
}

int main()
{
    ShowMainMenu();

    return 0;
}