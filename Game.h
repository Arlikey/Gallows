#pragma once
#include <iostream>
#include <vector>
#include "mytolower.h"
#include <chrono>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
using namespace std;

typedef chrono::steady_clock::time_point timer;

class Game
{
	string guessedWord;
	string userWord;
	int attemptsLeft;
	int maxAttempts;
public:
	Game();

	// ����� ��� ������ ���������� ����� 
	string ChooseWord();

	// ����� ��� ������ ������ ����
	void ShowRules();

	void Play();

	// ����� ��� �������� ������������ �����
	bool isCorrect(char userLetter);
	
	// ����� ��� �������� ������� � ������������� ����, ���� ������ ��� ��� ����������� ���
	// �������� �������� ��� ������ ����� ���� �� � �� �, ���������� false
	bool isPossible(vector<char> wrongLetters, char userLetter);
	
	// ������ ��� ������ ������� ���� � �����������
	void ShowGameResults(timer start, timer end, vector<char> wrongLetters, vector<char> rightLetters);
	void ShowGameStatus(vector<char> wrongLetters);

	// ����� ��� ������ ������
	void ShowGallow(int attemptsLeft);

	// ������ ��� �������� ���������� ����
	bool isGameOver();
	bool isGameWon();
};

