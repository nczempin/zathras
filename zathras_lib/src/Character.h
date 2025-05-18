#pragma once

class Character {
public:
	static int getNumericValue(char c) {
		if (isDigit(c)) {
			return c - '0';
		}
		else if (isSmallLetter(c)) {
			return c - 'a' + 1;
		}
		else if (isCapitalLetter(c)) {
			return c - 'A' + 1;
		}
		else {
			return -1; //TODO
		}
	}
	static bool isDigit(char c) {
		return '0' <= c && c <= '9';
	}
	static bool isSmallLetter(char c) {
		return 'a' <= c && c <= 'z';
	}
	static bool isCapitalLetter(char c) {
		return 'A' <= c && c <= 'Z';
	}
	static bool isLetter(char c) {
		return isSmallLetter(c) || isCapitalLetter(c);
	}
};
