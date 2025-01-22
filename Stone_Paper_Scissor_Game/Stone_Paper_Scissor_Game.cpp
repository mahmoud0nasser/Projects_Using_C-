#include <iostream>
#include <string>

using namespace std;

/*
Requirements:
	. Ask for how many rounds the game will be
	. Start Each round Player VS Computer
	. Show the Results with each round
	. if computer wins the round ring a bill, and screen red
	. if player won the round show green screen
	. after all rounds show game over the print game results,
	  then ask user if s/he want to play again?
*/

// Try to

enum enWinner {
	Player = 1,
	Computer = 2,
	Draw = 3
};

enum enGameChoice {
	Stone = 1,
	Paper = 2,
	Scissors = 3
};

struct stRoundInfo {
	short GameRound = 0;
	enGameChoice PlayerChoice;
	enGameChoice ComputerChoice;
	enWinner Winner;
	string WinnerName = "";
};

struct stGameResults {
	short PlayerWinTimes = 0;
	short ComputerWinTimes = 0;
	short DrawTimes = 0;
	short GameRounds = 0;
	enWinner Winner;
	string WinnerName = "";
};

short HowManuRounds()
{
	short GameRounds = 0;

	do {
		cout << "Enter How Many Rounds 1 to 10? \n";
		cin >> GameRounds;
	} while (GameRounds < 1 || GameRounds > 10);

	return GameRounds;
}

int RandomNumber(int From, int To)
{
	int RandNumber = rand() % (To - From + 1) + From;

	return RandNumber;
}

enGameChoice ReadPlayerChoice()
{
	short Choice = 0;

	do {
		cout << "Player Choice: [1]: Stone, [2]: Paper, [3]: Scissors? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 3);

	return (enGameChoice)Choice;
}

enGameChoice GetComputerChoice()
{
	return (enGameChoice)RandomNumber(1, 3);
}

enWinner WhoWonTheRound(stRoundInfo RoundInfo)
{
	if (RoundInfo.PlayerChoice == RoundInfo.ComputerChoice)
		return enWinner::Draw;

	switch (RoundInfo.PlayerChoice) {
	case enGameChoice::Stone:
		if (RoundInfo.ComputerChoice == enGameChoice::Paper)
			return enWinner::Computer;
		break;
	case enGameChoice::Paper:
		if (RoundInfo.ComputerChoice == enGameChoice::Scissors)
			return enWinner::Computer;
		break;
	case enGameChoice::Scissors:
		if (RoundInfo.ComputerChoice == enGameChoice::Stone)
			return enWinner::Computer;
		break;
	}

	return enWinner::Player;
}

string WinnerName(enWinner Winner)
{
	string GameChoices[3] = { "Stone", "Paper", "Scissors" };

	return GameChoices[Winner - 1];
}

void SetScreenColor(enWinner Winner)
{
	switch (Winner)
	{
	case enWinner::Player:
		system("color 2F");
		break;
	case enWinner::Computer:
		cout << "\a";
		system("color 4F");
		break;
	default:
		system("color 6F");
	}
}

string ChoiceName(enGameChoice GameChoice)
{
	string Choice[3] = { "Stone", "Paper", "Scissors" };
	return Choice[GameChoice - 1];
}

void PrintRoundresults(stRoundInfo RoundInfo)
{
	cout << "\n____________Round [" << RoundInfo.GameRound << "] ____________\n\n";
	cout << "Player Choice  : " << ChoiceName(RoundInfo.PlayerChoice) << endl;
	cout << "Computer Choice: " << ChoiceName(RoundInfo.ComputerChoice) << endl;
	cout << "Round Winner   : [" << RoundInfo.WinnerName << "]" << endl;
	cout << "__________________________________\n" << endl;

	SetScreenColor(RoundInfo.Winner);
}

enWinner WhoWonTheGame(short PlayerWonTimes, short ComputerWonTimes)
{
	if (PlayerWonTimes > ComputerWonTimes)
		return enWinner::Player;
	else if (ComputerWonTimes > PlayerWonTimes)
		return enWinner::Computer;
	else
		return enWinner::Draw;
}

stGameResults FinalResults(short Rounds, short PlayerWonTimes, short ComputerWonTimes, short DrawTimes)
{
	stGameResults GameResults;

	GameResults.PlayerWinTimes = PlayerWonTimes;
	GameResults.GameRounds = Rounds;
	GameResults.ComputerWinTimes = ComputerWonTimes;
	GameResults.DrawTimes = DrawTimes;
	GameResults.Winner = WhoWonTheGame(GameResults.PlayerWinTimes, GameResults.ComputerWinTimes);
	GameResults.WinnerName = WinnerName(GameResults.Winner);

	return GameResults;
}

stGameResults PlayGame(short Rounds)
{
	stRoundInfo RoundInfo;
	short PlayerWonTimes = 0, ComputerWonTimes = 0, DrawTimes = 0;

	for (int GameRound = 1; GameRound <= Rounds; GameRound++) {
		RoundInfo.GameRound = GameRound;
		RoundInfo.PlayerChoice = ReadPlayerChoice();
		RoundInfo.ComputerChoice = GetComputerChoice();
		RoundInfo.Winner = WhoWonTheRound(RoundInfo);
		RoundInfo.WinnerName = WinnerName(RoundInfo.Winner);

		if (RoundInfo.Winner == enWinner::Player)
			PlayerWonTimes++;
		else if (RoundInfo.Winner == enWinner::Computer)
			ComputerWonTimes++;
		else
			DrawTimes++;

		PrintRoundresults(RoundInfo);
	}

	return FinalResults(Rounds, PlayerWonTimes, ComputerWonTimes, DrawTimes);
}

string Tabs(short NumberOfTabs)
{
	string t = "";

	for (int i = 0; i < NumberOfTabs; i++) {
		t += "\t";
	}

	return t;
}

void ShowFinalGameResults(stGameResults GameResults)
{
	cout << Tabs(2) << "_____________________ [Game Results ] _____________________\n\n";
	cout << Tabs(2) << "Game Rounds        : " << GameResults.GameRounds << endl;
	cout << Tabs(2) << "Player won Times   : " << GameResults.PlayerWinTimes << endl;
	cout << Tabs(2) << "Computer won Times : " << GameResults.ComputerWinTimes << endl;
	cout << Tabs(2) << "Draw Times         : " << GameResults.DrawTimes << endl;
	cout << Tabs(2) << "Final Winner       : " << GameResults.WinnerName << endl;
	cout << Tabs(2) << "___________________________________________________________\n";

	SetScreenColor(GameResults.Winner);
}

void ResetScreen()
{
	system("cls");
	system("color 0F");
}

void GameOverScreen()
{
	cout << Tabs(2) << "___________________________________________\n\n";
	cout << Tabs(2) << "                +++ G a m e O v e r +++\n";
	cout << Tabs(2) << "___________________________________________\n\n";
}


void StartGame()
{
	char PlayAgain = 'Y';

	do {
		ResetScreen();
		stGameResults GameResults = PlayGame(HowManuRounds());
		GameOverScreen();
		ShowFinalGameResults(GameResults);

		cout << endl << Tabs(2) << "Do you want to play again: (Y/N)? ";
		cin >> PlayAgain;
	} while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main()
{
	srand((unsigned)time(NULL));
	StartGame();

	return 0;
}