#pragma once
class mytoupper
{
public:
	int operator()(char letter) {
		if (letter >= '�' && letter <= '�') {
			return letter - '�' + '�';
		} else {
			return letter;
		}
	}
};

