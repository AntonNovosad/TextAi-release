#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const int SHINGLE_SIZE = 3;
const int SPACE_ASCII = 32;
const int A_ASCII = 65;
const int Z_ASCII = 90;
const string EMPTY_STRING = "";
const string SPACE_STRING = " ";

double antiPlagiarism(string text, string fragment);
bool isSeparator(char c);
int getStringLength(string text);
int getAmoutOfWords(string text);
string canonizeText(string text);
string cutTheWord(string text, int position);
bool isForbiddenWord(string text);
string wordToLowerCase(string text);
void printArrayOfStrings(string text[], int size);

double antiPlagiarism(string text, string fragment) {
	int wordMatch = 0;
	int shingleMatch = 0;
	int numberOfShingles = 0;
	int position = 0;
		
	text = canonizeText(text);
	fragment = canonizeText(fragment);
	
	cout << text << endl;
	cout << fragment << endl;
	
	int textSize = getAmoutOfWords(text);
	int fragmentSize = getAmoutOfWords(fragment);
	

	if (fragmentSize == 0 or textSize == 0) {
			return 0.0;
	}

	string wordsOfText[textSize];
	string wordsOfFragment[fragmentSize];
	
	printArrayOfStrings(wordsOfText, textSize);
	printArrayOfStrings(wordsOfFragment, fragmentSize);
		
	
	for (int i = 0; i < textSize; i++) {
		wordsOfText[i] = cutTheWord(text, position);
		position += getStringLength(wordsOfText[i]) + 1;
	}
	position = 0;
	for (int i = 0; i < fragmentSize; i++) {
		wordsOfFragment[i] = cutTheWord(fragment, position);
		position += getStringLength(wordsOfFragment[i]) + 1;
	}
	
	if (textSize > fragmentSize) {
		numberOfShingles = fragmentSize - SHINGLE_SIZE + 1;
	}	
	else {
		numberOfShingles = textSize - SHINGLE_SIZE + 1;
	}
	
	if (SHINGLE_SIZE > numberOfShingles) {
		return 0.0;
	}
	
	for (int i = 0; i < numberOfShingles; i++) {
		for (int j = 0; j < SHINGLE_SIZE; j++) {
			if (wordsOfText[i + j] == wordsOfFragment[i + j]) {
				wordMatch++;
				if (wordMatch == SHINGLE_SIZE) { 
					shingleMatch++;	
				}
			}	
		}
		wordMatch = 0;
	}
	
	return 1.0 * shingleMatch / numberOfShingles; 
}

string canonizeText(string text) {	
	string canonizedText = EMPTY_STRING;
	string space = " ";
	string word = EMPTY_STRING;
	
	for (int i = 0; text[i] != 0; i++) {
		if (!isSeparator(text[i])) {
			word += text[i];
			if (isSeparator(text[i + 1])) {
				if (!isForbiddenWord(wordToLowerCase(word))) {
					cout << word << endl;
					canonizedText = canonizedText + wordToLowerCase(word) + space;
					word = EMPTY_STRING;	
				}
				else {
					word = EMPTY_STRING;
				}
			}
			if (text[i + 1] == 0) {
				if (!isForbiddenWord(wordToLowerCase(word))) {
					cout << word << endl;
					canonizedText = canonizedText + wordToLowerCase(word);
				}	
			}
		}
	}
	
	return canonizedText;
}


bool isSeparator(char c) {
	char separators[] = " ~`!@#$%^&*-_=+,./({[<>]});:?\n0123456789 ";
	
	for (int i = 0; separators[i] != 0; i++)
		if (separators[i] == c)
			return true;
	return false;
}

void printArrayOfStrings(string text[], int size) {
	for (int i = 0; i < size; i++) 
		cout << text[i] << endl;	
}

int getStringLength(string text) {
	int i = 0;
	while (text[i] != 0) {
		i++;
	}
		
	return i;
}

int getAmoutOfWords(string text) {
	int counter = 0;
	if (text[0] == 0) {
		return 0;
	}
	for (int i = 0; text[i] != 0; i++) {
		if (text[i] == SPACE_ASCII and text[i + 1] != SPACE_ASCII) counter++;
	}
	return counter + 1; 
}

string cutTheWord(string text, int position) {	
	string word = EMPTY_STRING;
	while (text[position] != SPACE_ASCII and text[position] != 0) {
		word += text[position];
		position++;	
	}
	return word;
}

bool isForbiddenWord(string text) {	
	const int N = 12;
	string forbiddenWords[N] = {"the", "with", "under", "and", "for", "from", "between", "into", "out", "over", "that", "among"};
	for (int i = 0; i < N; i++) {
		if (text == forbiddenWords[i])
			return true;
	}
	if (getStringLength(text) < 3) {
		return true;
	}
	return false;
}

string wordToLowerCase(string text) {
	for (int i = 0; i < getStringLength(text); i++) {
		if (text[i] >= A_ASCII and text[i] <= Z_ASCII) {
			text[i] = text[i] + SPACE_ASCII;
		}
	}
	return text;
}
