#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

namespace ATMLib
{
	void ShowATMMainMenu();
	void Login();
	void ShowQuickWithdrawMenu();
	void ShowNormalWithdraw();

	const string ClientsFileNamePath = "\\Clients.txt";
	const string ClientsFileName = "Clients.txt";

	struct stClient {
		string AccountNumber;
		string PinCode;
		string ClientName;
		string Phone;
		double AccountBalance;
	};

	stClient CurrentClientInfo;

	vector<string> SplitString(string Line, string delim = "#//#")
	{
		int pos = 0;
		vector<string> vWords;

		while ((pos = Line.find(delim)) != string::npos)
		{
			string Word = Line.substr(0, pos);

			if (Word != "")
				vWords.push_back(Word);
			Line.erase(0, pos + delim.length());
		}

		if (Line != "")
			vWords.push_back(Line);

		return vWords;
	}

	stClient ConvertLineToRecord(string Line, string delim = "#//#")
	{
		vector<string> vStrings = SplitString(Line);
		stClient Client;

		Client.AccountNumber = vStrings[0];
		Client.PinCode = vStrings[1];
		Client.ClientName = vStrings[2];
		Client.Phone = vStrings[3];
		Client.AccountBalance = stod(vStrings[4]);

		return Client;
	}

	string ConvertRecordToLine(stClient Client, string delim = "#//#")
	{
		string JoinString = "";

		JoinString += (Client.AccountNumber + delim);
		JoinString += (Client.PinCode + delim);
		JoinString += (Client.ClientName + delim);
		JoinString += (Client.Phone + delim);
		JoinString += to_string(Client.AccountBalance);

		return JoinString;
	}

	vector<stClient> LoadClientDataFromFile(string FileName)
	{
		vector<stClient> vClients;
		fstream MyFile;
		MyFile.open(FileName, ios::in);

		if (MyFile.is_open())
		{
			string Line = "";

			while (getline(MyFile, Line))
			{
				if (Line != "") {
					stClient Client = ConvertLineToRecord(Line);
					vClients.push_back(Client);
				}
			}

			MyFile.close();
		}

		return vClients;
	}

	enum enATMMainMenuOptions
	{
		eQuickWithdraw = 1, eNormalWithdraw = 2,
		eDeposit = 3, eCheckBalance = 4,
		eLogout = 5
	};

	enum enQuickWithdraw {
		e20 = 1, e50 = 2,
		e100 = 3, e200 = 4,
		e400 = 5, e600 = 6,
		e800 = 7, e1000 = 8,
		eExit = 9
	};

	short ReadATMMainMenuOption()
	{
		short Option;

		do {
			cout << "\nChoose what do you want to do[1 to 5]? ";
			cin >> Option;
		} while (Option < 1 || Option > 5);

		return Option;
	}

	short ReadQuickWithdrawMenuOption()
	{
		short Option;

		do {
			cout << "\nChoose what do you want to do[1 to 9]? ";
			cin >> Option;
		} while (Option < 1 || Option > 9);

		return Option;
	}

	void GoBackToATMMainMenu()
	{
		cout << "\n\nPress any key to go back to ATM Main Menu...";
		system("pause>0");
		ShowATMMainMenu();
	}

	void ShowCheckBalanceScreen()
	{
		cout << "===========================================\n";
		cout << "\t\tCheck Balance Screen\n";
		cout << "===========================================\n";

		cout << "Your Balance is: " << CurrentClientInfo.AccountBalance << endl;
	}

	int ReadWithdrawAmount()
	{
		int Amount;

		do {
			cout << "Enter an amount multiple of 5's? ";
			cin >> Amount;
		} while (Amount % 5 != 0);

		return Amount;
	}

	void SaveDataToFile(string FileName, vector<stClient> vClients)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::out);

		if (MyFile.is_open())
		{
			for (stClient& Client : vClients)
			{
				string Line = ConvertRecordToLine(Client);
				if (Line != "")
					MyFile << Line << endl;
			}

			MyFile.close();
		}
	}

	void MakeAnotherTransaction()
	{
		cout << "Press any key to continue...\n";
		system("pause > 0");
	}

	bool MakeQuickWithdrawTransaction(int QuickWithdrawAmount)
	{
		char Answer = 'Y';
		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);

		cout << "\nAre you sure you want to perform this transaction(Y/N)? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			for (stClient& Client : vClients)
			{
				if (CurrentClientInfo.AccountNumber == Client.AccountNumber && CurrentClientInfo.PinCode == Client.PinCode)
				{
					if (Client.AccountBalance >= abs(QuickWithdrawAmount)) {
						Client.AccountBalance += QuickWithdrawAmount;
						SaveDataToFile(ClientsFileName, vClients);
						cout << "\nDone Successfully, New Balance is: " << CurrentClientInfo.AccountBalance << endl;
						return true;
					}
					else {
						cout << "\nThe amount exceeds your balance, make another choice? \n";
						MakeAnotherTransaction();
					}
				}
			}
		}

		return false;
	}

	bool MakeMoreThanTransaction(int QuickWithdrawAmount)
	{
		char Answer = 'Y';
		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);

		cout << "\nAre you sure you want to perform this transaction(Y/N)? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			CurrentClientInfo.AccountBalance += QuickWithdrawAmount;

			for (stClient& Client : vClients)
			{
				if (CurrentClientInfo.AccountNumber == Client.AccountNumber && CurrentClientInfo.PinCode == Client.PinCode)
				{
					Client.AccountBalance += QuickWithdrawAmount;
					SaveDataToFile(ClientsFileName, vClients);
					cout << "\nDone Successfully, New Balance is: " << CurrentClientInfo.AccountBalance << endl;
					return true;
				}
			}
		}
		else {
			GoBackToATMMainMenu();
		}
		return false;
	}

	bool isWithdrawValid(int Amount)
	{
		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);

		for (stClient& Client : vClients)
		{
			if (CurrentClientInfo.AccountNumber == Client.AccountNumber && CurrentClientInfo.PinCode == Client.PinCode)
			{
				if (Client.AccountBalance >= Amount) {
					return true;
				}
				else {
					return false;
				}
			}
		}

		return false;
	}

	void PerformNormalWithdraw()
	{
		short WithdrawBalance = ReadWithdrawAmount();

		if (WithdrawBalance > CurrentClientInfo.AccountBalance) {
			cout << "\nThe amount exceeds your balance, make another choice? \n";
			MakeAnotherTransaction();
			ShowNormalWithdraw();
			return;
		}

		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountNumber(CurrentClientInfo.AccountNumber, WithdrawBalance * -1, vClients);
		CurrentClientInfo.AccountBalance -= WithdrawBalance;
	}

	void ShowNormalWithdraw()
	{
		cout << "===========================================\n";
		cout << "\t\tNormal Withdraw Screen\n";
		cout << "===========================================\n";

		int Amount = ReadWithdrawAmount();

		while (true) {
			if (isWithdrawValid(Amount))
			{
				MakeMoreThanTransaction(Amount * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				system("cls");
				ShowNormalWithdraw();
			}
		}

		// PerformNormalWithdraw();
	}

	int ReadAmount()
	{
		int Amount;

		do {
			cout << "Enter a positive Deposit Amount? ";
			cin >> Amount;

		} while (Amount <= 0);

		return Amount;
	}

	void PerformDeposit()
	{
		short DepositAmount = ReadAmount();

		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountNumber(CurrentClientInfo.AccountNumber, DepositAmount, vClients);
		CurrentClientInfo.AccountBalance += DepositAmount;
	}

	void ShowDepositScreen()
	{
		cout << "===========================================\n";
		cout << "\t\tDeposit Screen\n";
		cout << "===========================================\n";

		int Amount = ReadAmount();

		// PerformDeposit();

		MakeMoreThanTransaction(Amount);
	}

	void PerfromQuickWithdrawMenuOptionAnother(enQuickWithdraw QuickWithdrawOption)
	{
		switch (QuickWithdrawOption)
		{
		case enQuickWithdraw::e20:
			if (isWithdrawValid(20)) {
				MakeMoreThanTransaction(20 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e50:
			if (isWithdrawValid(50)) {
				MakeMoreThanTransaction(50 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e100:
			if (isWithdrawValid(100)) {
				MakeMoreThanTransaction(100 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e200:
			if (isWithdrawValid(200)) {
				MakeMoreThanTransaction(200 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e400:
			if (isWithdrawValid(400)) {
				MakeMoreThanTransaction(400 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e600:
			if (isWithdrawValid(600)) {
				MakeMoreThanTransaction(600 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e800:
			if (isWithdrawValid(800)) {
				MakeMoreThanTransaction(800 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e1000:
			if (isWithdrawValid(1000)) {
				MakeMoreThanTransaction(1000 * -1);
				GoBackToATMMainMenu();
			}
			else {
				cout << "\nThe amount exceeds your balance, make another choice? \n";
				MakeAnotherTransaction();
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::eExit:
			ShowATMMainMenu();
		}
	}

	void PerfromQuickWithdrawMenuOptionSecond(enQuickWithdraw QuickWithdrawOption)
	{
		switch (QuickWithdrawOption)
		{
		case enQuickWithdraw::e20:
			if (!MakeQuickWithdrawTransaction(20 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e50:
			if (!MakeQuickWithdrawTransaction(50 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e100:
			if (!MakeQuickWithdrawTransaction(100 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e200:
			if (!MakeQuickWithdrawTransaction(200 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e400:
			if (!MakeQuickWithdrawTransaction(400 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e600:
			if (!MakeQuickWithdrawTransaction(600 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e800:
			if (!MakeQuickWithdrawTransaction(800 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::e1000:
			if (!MakeQuickWithdrawTransaction(1000 * -1)) {
				ShowQuickWithdrawMenu();
			}
			break;
		case enQuickWithdraw::eExit:
			ShowATMMainMenu();
		}
	}

	short GetQuickWithdrawAmount(short QuickWithdrawOption)
	{
		switch (QuickWithdrawOption)
		{
		case 1:
			return 20;
		case 2:
			return 50;
		case 3:
			return 100;
		case 4:
			return 200;
		case 5:
			return 400;
		case 6:
			return 600;
		case 7:
			return 800;
		case 8:
			return 1000;
		}
	}

	bool DepositBalanceToClientByAccountNumber(string AccountNumber, int Amount, vector<stClient> vClients)
	{
		for (stClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.AccountBalance += Amount;
				SaveDataToFile(ClientsFileName, vClients);
				cout << "\nDone Successfully, Your Balance is: " << Client.AccountBalance << endl;
				return true;
			}
		}

		return false;
	}

	void PerformQuickWithdraw(enQuickWithdraw QuickWithdrawOption)
	{
		if (enQuickWithdraw::eExit == QuickWithdrawOption)
		{
			return;
		}

		short WithdrawBalance = GetQuickWithdrawAmount(short(QuickWithdrawOption));

		if (CurrentClientInfo.AccountBalance < WithdrawBalance) {
			cout << "\nThe amount exceeds your balance, make another choice? \n";
			MakeAnotherTransaction();
			ShowQuickWithdrawMenu();
			return;
		}

		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountNumber(CurrentClientInfo.AccountNumber, WithdrawBalance * -1, vClients);
		CurrentClientInfo.AccountBalance -= WithdrawBalance;
	}

	void ShowQuickWithdrawMenu()
	{
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tQuick Withdraw Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] 20. \t\t[2] 50\n";
		cout << "\t[3] 100.\t\t[4] 200\n";
		cout << "\t[5] 400.\t\t[6] 600\n";
		cout << "\t[7] 800.\t\t[8] 1000\n";
		cout << "\t[9] Exit.\n";
		cout << "===========================================\n";

		cout << "Your Balance is: " << CurrentClientInfo.AccountBalance << endl;

		// PerformQuickWithdraw((enQuickWithdraw)ReadQuickWithdrawMenuOption());

		PerfromQuickWithdrawMenuOptionAnother((enQuickWithdraw)ReadQuickWithdrawMenuOption());
	}

	void PerfromATMMainMenuOption(enATMMainMenuOptions ATMMainMenuOption)
	{
		switch (ATMMainMenuOption)
		{
		case enATMMainMenuOptions::eQuickWithdraw:
			system("cls");
			ShowQuickWithdrawMenu();
			GoBackToATMMainMenu();
			break;
		case enATMMainMenuOptions::eNormalWithdraw:
			system("cls");
			ShowNormalWithdraw();
			GoBackToATMMainMenu();
			break;
		case enATMMainMenuOptions::eDeposit:
			system("cls");
			ShowDepositScreen();
			GoBackToATMMainMenu();
			break;
		case enATMMainMenuOptions::eCheckBalance:
			system("cls");
			ShowCheckBalanceScreen();
			GoBackToATMMainMenu();
			break;
		case enATMMainMenuOptions::eLogout:
			Login();
		}
	}

	void ShowATMMainMenu()
	{
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tATM Main Menu Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] Quick Withdraw.\n";
		cout << "\t[2] Normal Withdraw.\n";
		cout << "\t[3] Deposit.\n";
		cout << "\t[4] Check Balance.\n";
		cout << "\t[5] Logout.\n";
		cout << "===========================================\n";

		PerfromATMMainMenuOption((enATMMainMenuOptions)ReadATMMainMenuOption());
	}

	bool isFindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
	{
		vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);

		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
			{
				Client = C;
				return true;
			}
		}

		return false;
	}

	void Login()
	{
		bool LoginFailed = false;

		string AccountNumber, PinCode;

		do {
			system("cls");
			cout << "==================================================\n";
			cout << "\t\tLogin Screeen.\n";
			cout << "==================================================\n";

			if (LoginFailed)
			{
				cout << "Invalid Account Number/PinCode!\n";
			}

			cout << "Enter Account Number? ";
			getline(cin >> ws, AccountNumber);

			cout << "Enter PinCode? ";
			getline(cin, PinCode);

			LoginFailed = !isFindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClientInfo);

		} while (LoginFailed);

		ShowATMMainMenu();
	}
}