#pragma once
class mytoupper
{
public:
	int operator()(char letter) {
		if (letter >= 'à' && letter <= 'ÿ') {
			return letter - 'à' + 'À';
		} else {
			return letter;
		}
	}
};

