#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
	guessedWord = ChooseWord();
	// ���������������� ����� �������� ����� ����������� ����� �� �����
	userWord.resize(guessedWord.length());
	// ����� ��� ������� �������� ������� ���������������
	userWord.assign(guessedWord.length(), '_');
	maxAttempts = 6;
	attemptsLeft = maxAttempts;
}

string Game::ChooseWord() {
	forward_list<string> lst;
	ifstream in("Words.bin", ios::binary);

	string word;

	// ���������� ���������� ������� ����� �� ����� � �� ������ � ����������� ������
	while (!in.eof())
	{
		in >> word;
		lst.push_front(word);
	}

	// ������������ ��������� ����� �������� ������
	int randomIndex = rand() % distance(lst.begin(), lst.end());
	// ���������� ������� � ����� �������� � ������� ��� ������
	auto randomElement = next(lst.begin(), randomIndex);

	in.close();

	// ������� ���������������� � ������������ �����, ������� ������������ � ���������� ������
	return *randomElement;
}

void Game::ShowRules() {
	system("cls");
	cout << "\n\n\t";
	// ������ ���� ������ �� ������ � ����� �����
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	cout << " ! ������� ���� ! ";
	// ������ ���� ������ �� �����������
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);
	cout << "\n\n\n";
	cout << " 1. ����� ��������� ����� ����������.\n";
	cout << " 2. ��������� ���������� ����� � �������� ����� ��� ����.\n";
	cout << " 3. ����� �������� ������ ���������������, ������������� � ������������ ������ ������������� �����.\n";
	cout << " 4. ���� ���������� ����� ���� � �����, �� ��������� ����� � ��� ���������������� ���� \n    ����� ������� � ������� ���, ������� ��� ����������� � �����.\n"
		<< "    ���� ����� ����� ���, �� � �������� ����������� ����� ����.\n";
	cout << " 5. ����� ���������� ���������� ����� �� ��� ���, ���� �� �������� �� �����.\n";
	cout << " 6. ���� �������� � �������� ���������� ���������, �� ����� �����������, ��������� ����������.\n";
	cout << "\n\n��� ����������� ������� ����� ������...";
	_getch();
	system("cls");
}

void Game::ShowGameStatus(vector<char> wrongLetters) {
	string lines(30, '-');
	ostream_iterator<char> out(cout, " ");
	ShowGallow(attemptsLeft);
	cout << "  " << lines << endl;
	cout << "  > �����: ";
	copy(userWord.begin(), userWord.end(), out);
	cout << '\n' << "  " << lines << endl;
	cout << "  > ������� ��������: " << attemptsLeft << endl;
	cout << "  " << lines << endl;
	cout << "  > �������� �����: ";
	copy(wrongLetters.begin(), wrongLetters.end(), out);
	cout << "\n";
}

void Game::ShowGallow(int attemptsLeft) {
	cout << '\n';
	static char gallow[7][11] =
	{
		"  ________",
		"  |      |",
		"         |",
		"         |",
		"         |",	
		"         |",	
		"_________|"
	};
	switch (attemptsLeft) {
	case 5:
		gallow[2][2] = 'O';
		break;
	case 4:
		gallow[3][2] = '|';
		break;
	case 3:
		gallow[3][1] = '/';
		break;
	case 2:
		gallow[3][3] = '\\';
		break;
	case 1:
		gallow[4][1] = '/';
		break;
	case 0:
		gallow[4][3] = '\\';
	}

	for (size_t i = 0; i < 7; i++)
	{
		cout << "\t   " << gallow[i] << endl;
	}
	cout << '\n';
}

void Game::Play() {
	// ������� 2 ���������� ��� �������� ���������� � ������������ ����
	vector<char> rightLetters;
	vector<char> wrongLetters;
	
	// �������� ������ ������� �� ������ ����
	auto start_time = chrono::high_resolution_clock::now();

	// �������� ��� ����� � ����� �� ���������, ����� �������� ���������������������
	transform(guessedWord.begin(), guessedWord.end(), guessedWord.begin(), mytoupper());

	char userLetter;
	while (!isGameOver() && !isGameWon())
	{
		system("cls");

		ShowGameStatus(wrongLetters);

		cout << "\n  ������� ����� -> ";
		cin >> userLetter;
		// ����� �������� ����� ��������� ������������� �� ���������
		userLetter = mytoupper()(userLetter);

		if (!isPossible(wrongLetters, userLetter)) {
			_getch();
			continue;
		}

		// �������� �� ������������ �����
		if (isCorrect(userLetter))
		{
			// ���� ����� ���� � �����, ��� �������� � ��������� � ����������� �������
			rightLetters.push_back(userLetter);
		} else {
			// ����� �������� � ��������� � ������������� �������
			wrongLetters.push_back(userLetter);
		}
	}

	// ���� ���� ���� �������� ��� ���������, �� ��������������� ������
	auto end_time = chrono::high_resolution_clock::now();

	// ��������� ����� � ������������ ����������
	system("cls");
	ShowGameResults(start_time, end_time, wrongLetters, rightLetters);
}

bool Game::isPossible(vector<char> wrongLetters, char userLetter) {
	// ��������� ���� �� ��� ������������ �����, ������ ���� �� ��� � ����� ������������, ��� � ���������� ������������ ����
	if (userLetter < '�' || userLetter > '�')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

		cout << "\n  �������� ������. ��� ����� ������ ����� �� ���������!" << endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		return false;
	}
	if (userWord.find(userLetter) != string::npos || find(wrongLetters.begin(), wrongLetters.end(), userLetter) != wrongLetters.end()) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

		cout << "\n  �� ��� ��������� ��� �����!" << endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		return false;
	}

	return true;
}

bool Game::isCorrect(char userLetter) {
	// ���������� ����� ����� � �����, ���� �������, �� ������������ �������
	int pos = guessedWord.find(userLetter);
	// ���� ����� �� ������� � ����� ������������ npos
	if (pos == string::npos)
	{
		attemptsLeft--;
		return false;
	}
	// ���� �������
	while (pos != string::npos) {
		// �������� ������ ������������� � ����� �� �����
		userWord[pos] = userLetter;
		// ����� ����, �� ��� � ������� �� 1 ������ ���������� ���������
		pos = guessedWord.find(userLetter, pos + 1);
	}
	return true;
}

void Game::ShowGameResults(timer start, timer end, vector<char> wrongLetters, vector<char> rightLetters) {
	cout << "\n        ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	
	if (isGameWon()) {
		cout << "  �� ��������!  " << endl;
	} else {
		cout << "  �� ���������!  " << endl;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);

	ShowGallow(attemptsLeft);

	cout << "\n      ";
	// ������ ���� ������ �� ������ � ����� �����
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	cout << " ! ���������� ���� ! ";
	// ������ ���� ������ �� �����������
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);
	cout << "\n\n";

	// ���������� ���������� ������, ������� ������������ ������ � ����
	auto seconds = chrono::duration_cast<chrono::seconds>(end - start);
	// ���������� ���������� �����
	auto minutes = seconds / 60;

	ostream_iterator<char> out(cout, " ");
	string lines(30, '-');


	cout << "  " << lines << endl;
	cout << "   ���������� �������: " << setw(2) << setfill('0') << minutes.count() << ':' << setw(2) << setfill('0') << seconds.count() << endl;
	cout << "  " << lines << endl;
	cout << "   ���������� �������: " << maxAttempts - attemptsLeft << endl;
	cout << "  " << lines << endl;
	cout << "   ������� �����: " << guessedWord << endl;
	cout << "  " << lines << endl;
	cout << "   �����: ";
	copy(wrongLetters.begin(), wrongLetters.end(), out);
	copy(rightLetters.begin(), rightLetters.end(), out);
	cout << endl << "  " << lines << endl;

}

bool Game::isGameOver() {
	if (attemptsLeft != 0)
	{
		return false;
	}
	return true;
}

bool Game::isGameWon() {
	if (userWord == guessedWord) {
		return true;
	}
	return false;
}