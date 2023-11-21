#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
	guessedWord = ChooseWord();
	// пользовательское слово получает длину полученного слова из файла
	userWord.resize(guessedWord.length());
	// также все символы задаются нижними подчеркиваниями
	userWord.assign(guessedWord.length(), '_');
	maxAttempts = 6;
	attemptsLeft = maxAttempts;
}

string Game::ChooseWord() {
	forward_list<string> lst;
	ifstream in("Words.bin", ios::binary);

	string word;

	// происходит считывание каждого слова из файла и их запись в односвязный список
	while (!in.eof())
	{
		in >> word;
		lst.push_front(word);
	}

	// определяется рандомный номер элемента списка
	int randomIndex = rand() % distance(lst.begin(), lst.end());
	// происходит переход к этому элементу и возврат его адреса
	auto randomElement = next(lst.begin(), randomIndex);

	in.close();

	// элемент разименовывается и возвращается слово, которое записывается в переменную класса
	return *randomElement;
}

void Game::ShowRules() {
	system("cls");
	cout << "\n\n\t";
	// Меняем цвет текста на черный с белым фоном
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	cout << " ! ПРАВИЛА ИГРЫ ! ";
	// Меняем цвет текста на стандартный
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);
	cout << "\n\n\n";
	cout << " 1. Нужно угадывать слова побуквенно.\n";
	cout << " 2. Компьютер загадывает слово и отмечает места для букв.\n";
	cout << " 3. Слова являются именем существительным, нарицательным в именительном падеже единственного числа.\n";
	cout << " 4. Если загаданная буква есть в слове, то компьютер пишет её над соответствующими этой \n    букве чертами — столько раз, сколько она встречается в слове.\n"
		<< "    Если такой буквы нет, то к виселице добавляется часть тела.\n";
	cout << " 5. Игрок продолжает отгадывать буквы до тех пор, пока не отгадает всё слово.\n";
	cout << " 6. Если туловище в виселице нарисовано полностью, то игрок проигрывает, считается повешенным.\n";
	cout << "\n\nДля продолжения нажмите любую кнопку...";
	_getch();
	system("cls");
}

void Game::ShowGameStatus(vector<char> wrongLetters) {
	string lines(30, '-');
	ostream_iterator<char> out(cout, " ");
	ShowGallow(attemptsLeft);
	cout << "  " << lines << endl;
	cout << "  > Слово: ";
	copy(userWord.begin(), userWord.end(), out);
	cout << '\n' << "  " << lines << endl;
	cout << "  > Попыток осталось: " << attemptsLeft << endl;
	cout << "  " << lines << endl;
	cout << "  > Неверные буквы: ";
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
	// создаем 2 контейнера для хранения правильных и неправильных букв
	vector<char> rightLetters;
	vector<char> wrongLetters;
	
	// начинаем отсчёт времени от начала игры
	auto start_time = chrono::high_resolution_clock::now();

	// изменяем все буквы в слове на заглавные, чтобы добиться регистронезависимости
	transform(guessedWord.begin(), guessedWord.end(), guessedWord.begin(), mytoupper());

	char userLetter;
	while (!isGameOver() && !isGameWon())
	{
		system("cls");

		ShowGameStatus(wrongLetters);

		cout << "\n  Введите букву -> ";
		cin >> userLetter;
		// также изменяем букву указанную пользователем на заглавную
		userLetter = mytoupper()(userLetter);

		if (!isPossible(wrongLetters, userLetter)) {
			_getch();
			continue;
		}

		// проверка на правильность ввода
		if (isCorrect(userLetter))
		{
			// если буква есть в слове, она попадает в контейнер с правильными буквами
			rightLetters.push_back(userLetter);
		} else {
			// иначе попадает в контейнер с неправильными буквами
			wrongLetters.push_back(userLetter);
		}
	}

	// если игра была выиграна или проиграна, то останавливается таймер
	auto end_time = chrono::high_resolution_clock::now();

	// очищается экран и показываются результаты
	system("cls");
	ShowGameResults(start_time, end_time, wrongLetters, rightLetters);
}

bool Game::isPossible(vector<char> wrongLetters, char userLetter) {
	// проверяем была ли уже использована буква, тоесть есть ли она в слове пользователя, или в контейнере неправильных букв
	if (userLetter < 'А' || userLetter > 'Я')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

		cout << "\n  Неверный символ. Вам нужно ввести букву на кириллице!" << endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		return false;
	}
	if (userWord.find(userLetter) != string::npos || find(wrongLetters.begin(), wrongLetters.end(), userLetter) != wrongLetters.end()) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

		cout << "\n  Вы уже пробовали эту букву!" << endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		return false;
	}

	return true;
}

bool Game::isCorrect(char userLetter) {
	// происходит поиск буквы в слове, если найдена, то возвращается позиция
	int pos = guessedWord.find(userLetter);
	// если буква не найдена в слове возвращается npos
	if (pos == string::npos)
	{
		attemptsLeft--;
		return false;
	}
	// если найдена
	while (pos != string::npos) {
		// заменяем нижнее подчеркивание в слове на букву
		userWord[pos] = userLetter;
		// снова ищем, но уже с позиции на 1 больше предыдущей найденной
		pos = guessedWord.find(userLetter, pos + 1);
	}
	return true;
}

void Game::ShowGameResults(timer start, timer end, vector<char> wrongLetters, vector<char> rightLetters) {
	cout << "\n        ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	
	if (isGameWon()) {
		cout << "  ВЫ ВЫИГРАЛИ!  " << endl;
	} else {
		cout << "  ВЫ ПРОИГРАЛИ!  " << endl;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);

	ShowGallow(attemptsLeft);

	cout << "\n      ";
	// Меняем цвет текста на черный с белым фоном
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | 240);
	cout << " ! СТАТИСТИКА ИГРЫ ! ";
	// Меняем цвет текста на стандартный
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 | 0);
	cout << "\n\n";

	// определяем количество секунд, которые пользователь пробыл в игре
	auto seconds = chrono::duration_cast<chrono::seconds>(end - start);
	// определяем количество минут
	auto minutes = seconds / 60;

	ostream_iterator<char> out(cout, " ");
	string lines(30, '-');


	cout << "  " << lines << endl;
	cout << "   Количество времени: " << setw(2) << setfill('0') << minutes.count() << ':' << setw(2) << setfill('0') << seconds.count() << endl;
	cout << "  " << lines << endl;
	cout << "   Количество попыток: " << maxAttempts - attemptsLeft << endl;
	cout << "  " << lines << endl;
	cout << "   Искомое слово: " << guessedWord << endl;
	cout << "  " << lines << endl;
	cout << "   Буквы: ";
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