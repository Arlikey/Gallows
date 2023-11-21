#include <iostream>
#include <Windows.h>
#include <fstream>
#include <forward_list>
#include <vector>
#include <chrono>
#include <iomanip>
#include <conio.h>
#include <stdio.h>
#include "Game.h"
using namespace std;

// Функция для смены цвета текста и фона
void SetColor(int a, int b) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (a == b) {
		SetConsoleTextAttribute(hConsole, 0 | 240);
	} else {
		SetConsoleTextAttribute(hConsole, 15);
	}
}

int main() {
	srand(time(0));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Game game;

	int userChoice = 1;
	char key;
	while (true) {
		cout << "\n\tДобро пожаловать в игру \"Виселица\"\n\n";
		cout << "\n  Выберите один из следующих вариантов:\n\n";
		SetColor(userChoice, 1);
		cout << (userChoice == 1 ? "> " : "") << " Начать игру \n";
		SetColor(userChoice, 2);
		cout << (userChoice == 2 ? "> " : "") << " Узнать правила игры \n";
		SetColor(userChoice, 3);
		cout << (userChoice == 3 ? "> " : "") << " Выход \n";
		key = _getch();
		if (key == 72 && (userChoice >= 2 && userChoice <= 3)) {
			userChoice--;
		}
		if (key == 80 && (userChoice >= 1 && userChoice <= 2)) {
			userChoice++;
		}
		if (key == '\r') {
			if (userChoice == 1) {
				game.Play();
				_getch;
				break;
			}
			if (userChoice == 2) {
				game.ShowRules();
			}
			if (userChoice == 3) {
				break;
			}
		}
		SetColor(1, 2);
		system("cls");
	}
	SetColor(1, 2);

	return 0;
}
