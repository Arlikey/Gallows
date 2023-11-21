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

	// метод для выбора случайного числа 
	string ChooseWord();

	// метод для вывода правил игры
	void ShowRules();

	void Play();

	// метод для проверки правильности буквы
	bool isCorrect(char userLetter);
	
	// метод для проверки символа и повторяющихся букв, если символ уже был использован или
	// символом является что угодно кроме букв от А до Я, возвращает false
	bool isPossible(vector<char> wrongLetters, char userLetter);
	
	// методы для показа статуса игры и результатов
	void ShowGameResults(timer start, timer end, vector<char> wrongLetters, vector<char> rightLetters);
	void ShowGameStatus(vector<char> wrongLetters);

	// метод для вывода пугала
	void ShowGallow(int attemptsLeft);

	// методы для проверки завершения игры
	bool isGameOver();
	bool isGameWon();
};

