#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

double antiPlagiarism(string text, string fragment);
int getLength(string text);
bool isSeparator(char c);
bool isNumber(char c);
bool isSmallWord(string word);
bool isWrongWord(string word);
void cutSeparators(string text, char canonizedText[]);
void cutWrongWords(char canonizedText[]);
void toLowerCase(char canonizedText[]) ;
void canonize(string text, char canonizedText[]);
int getWordsNumber(char canonizedText[]);
void fillArray(string textWordsArray[], char canonizedText[]);
long RSHash(string str);
long getShingleHash(string textWordsArray[], int shingleLength, int startIndex);
void getShinglesHashesArray(string textWordsArray[], int textShinglesNumber, int shingleLength, long shinglesHashesArray[]); 
void mergeSort(long unsortedArray[], int arrayLength);
void merge(long unsortedArray[], long leftArray[], int leftArrayLength, long rightArray[], int rightArrayLength);
bool binarySearch(long requiredHash, long sortedHashesArray[], int arrayLength);
double getMatchesPercentage(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber);
double getPercentage(int coincidences, int fragmentWordsNumber);


const int CHARACTER_A_ASCII = 65;
const int CHARACTER_Z_ASCII = 90;
const int LOWERCASE_UPPERCASE_INTERVAL = 32;
const char TERMINAL_NULL = '\0';
const char SPACE = ' ';
const string EMPTY_STRING = "";

double antiPlagiarism(string text, string fragment)
{
	const int TEXT_LENGTH = getLength(text);
	const int FRAGMENT_LENGTH = getLength(fragment);
	
	if (TEXT_LENGTH == 0 or FRAGMENT_LENGTH == 0) {
		return 0.0;
	}
	
	const int TEXT_ARRAY_LENGTH = TEXT_LENGTH + 2;
	const int FRAGMENT_ARRAY_LENGTH = FRAGMENT_LENGTH + 2;
	
	char canonizedText[TEXT_ARRAY_LENGTH];
	char canonizedFragment[FRAGMENT_ARRAY_LENGTH];

	canonize(text, canonizedText);
	canonize(fragment, canonizedFragment);

	int textWordsNumber = getWordsNumber(canonizedText);
	int fragmentWordsNumber = getWordsNumber(canonizedFragment);
	
	if (textWordsNumber == 0 or fragmentWordsNumber == 0) {
		return 0.0;
	}
	
	string textWordsArray[textWordsNumber];
	string fragmentWordsArray[fragmentWordsNumber];
	
	fillArray(textWordsArray, canonizedText);
	fillArray(fragmentWordsArray, canonizedFragment);
	
	return getMatchesPercentage(textWordsArray, fragmentWordsArray, textWordsNumber, fragmentWordsNumber);
}

int getLength(string text)
{
	int counter = 0;

	for (int i = 0; text[i] != TERMINAL_NULL; i++) {
		counter++;
	}

	return counter;
}

bool isSeparator(char c)
{
	char separator[] = " ~`!@#$%^&*-_=+,./({[<>]});:?\n";

	for (int i = 0; separator[i] != TERMINAL_NULL; i++) {
		if (c == separator[i]) {
			return true;
		}
	}

	return false;
}

bool isNumber(char c)
{
	char number[] = "0123456789";
	
	for (int i = 0; number[i] != TERMINAL_NULL; i++) {
		if (c == number[i]) {
			return true;
		}
	}
		
	return false;
}

bool isSmallWord(string word)
{
	int wordLength = getLength(word);
	
	const int MIN_WORD_LENGTH = 3;
	
	if (wordLength < MIN_WORD_LENGTH) {
		return true;
	}
	
	return false;
}

bool isWrongWord(string word)
{
	const int N = 12;
	string wrongWords[N] = {"the", "with", "under", "and", "for", "from", "between", "into", "out", "over", "that", "among"};
	
	for (int i = 0; i < N; i++) {
		if (word == wrongWords[i]) {
			return true;
		}
	}
		
	return false;
}

void toLowerCase(char canonizedText[]) 
{
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if ((canonizedText[i] >= CHARACTER_A_ASCII) and (canonizedText[i] <= CHARACTER_Z_ASCII)) {
			canonizedText[i] += LOWERCASE_UPPERCASE_INTERVAL;
		}
	}
}

void cutSeparators(string text, char canonizedText[])
{
	int index = 0;
	
	for (int i = 0; text[i] != TERMINAL_NULL; i++) {
		if (!isSeparator(text[i]) and !isNumber(text[i])) {
				canonizedText[index] = text[i];
				index++;
			
			if (isSeparator(text[i + 1]) or isNumber(text[i + 1])) {
				canonizedText[index] = SPACE;
				index++;
			}
		}
	}
	
	index++;
	canonizedText[index-1] = SPACE;
	canonizedText[index] = TERMINAL_NULL;
}

void cutWrongWords(char canonizedText[])
{
	int index = 0;
	string word = EMPTY_STRING;
	
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] != SPACE) {
			word += canonizedText[i];
			
			if (canonizedText[i + 1] == SPACE) {
				if (!isSmallWord(word) and !isWrongWord(word)) {
					int wordLength = getLength(word);
				
					for (int i = 0; i < wordLength; i++) {
						canonizedText[index] = word[i];
						index++;
					}
					
					canonizedText[index] = SPACE;
					index++;
				}
				
				word = EMPTY_STRING;
			}
		}
	}
	
	canonizedText[index] = TERMINAL_NULL;
}

void canonize(string text, char canonizedText[])
{
	cutSeparators(text, canonizedText);
	toLowerCase(canonizedText);
	cutWrongWords(canonizedText);
}

int getWordsNumber(char canonizedText[])
{
	int wordsCounter = 0;

	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] == SPACE or canonizedText[i + 1] == TERMINAL_NULL) {
			wordsCounter++;
		}
	}
	
	return wordsCounter;
}

void fillArray(string textWordsArray[], char canonizedText[])
{
	string word = EMPTY_STRING;
	int index = 0;
	
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] != SPACE) {
			word += canonizedText[i];
		} else {
			textWordsArray[index] = word;
			index++;
			word = EMPTY_STRING;
		}
	}
}

long RSHash(string str)  
{  
	int b = 378551;
	int a = 63689;
	long hash = 0;
		
	for (int i = 0; i < getLength(str); i++) {
		hash = hash * a + (int)str[i];
		a = a * b;
	}
		
	return hash;
}

long getShingleHash(string textWordsArray[], int shingleLength, int startIndex)
{
	long hash = 0;
	string shingle = EMPTY_STRING;
		
	for (int i = 0; i < shingleLength; i++) {
		shingle += textWordsArray[startIndex + i];
	}
		
	hash = RSHash(shingle);  
	return hash;
}

void getShinglesHashesArray(string textWordsArray[], int textShinglesNumber, int shingleLength, long shinglesHashesArray[]) 
{
	for (int i = 0; i < textShinglesNumber; i++) {
		shinglesHashesArray[i] = getShingleHash(textWordsArray, shingleLength, i);
	}
}

void mergeSort(long unsortedArray[], int arrayLength)
{
	if (arrayLength == 1) {
		return;
	}
	
	int mid = arrayLength/2;
	int leftArrayLength = mid;
	int rightArrayLength = arrayLength - mid;
	
	long leftArray[leftArrayLength];
	long rightArray[rightArrayLength];
	
	for (int i = 0; i < mid; i++) {
		leftArray[i] = unsortedArray[i];
	}
	
	for (int i = mid; i < arrayLength; i++) {
		rightArray[i - mid] = unsortedArray[i];
	}
	
	mergeSort(leftArray, leftArrayLength);
	mergeSort(rightArray, rightArrayLength);
	merge(unsortedArray, leftArray, leftArrayLength, rightArray, rightArrayLength);
}

void merge(long unsortedArray[], long leftArray[], int leftArrayLength, long rightArray[], int rightArrayLength)
{
	int leftIndex = 0;
	int rightIndex = 0;
	int resultArrayIndex = 0;
	
	while ((leftIndex < leftArrayLength) and (rightIndex < rightArrayLength)) {
		if(leftArray[leftIndex] < rightArray[rightIndex]) {
			unsortedArray[resultArrayIndex] = leftArray[leftIndex];
			leftIndex++;
			resultArrayIndex++;
		}
		
		else {
			unsortedArray[resultArrayIndex] = rightArray[rightIndex];
			rightIndex++;
			resultArrayIndex++;
		}
	}
	
	for (int l = leftIndex; l < leftArrayLength; l++) {
		unsortedArray[resultArrayIndex++] = leftArray[l];
	}
	
	for (int r = rightIndex; r < rightArrayLength; r++) {
		unsortedArray[resultArrayIndex++] = rightArray[r];
	}
}

bool binarySearch(long requiredHash, long sortedHashesArray[], int arrayLength)
{
	bool flag = false;
	int leftBorder = 0;
	int rightBorder = arrayLength - 1;
	
	while (leftBorder <= rightBorder) {
		int mid = (leftBorder + rightBorder) / 2;
		
		if (sortedHashesArray[mid] < requiredHash) {
			leftBorder = mid + 1;
		}
		
		else if (sortedHashesArray[mid] > requiredHash) {
			rightBorder = mid - 1;
		}
		
		else if (sortedHashesArray[mid] == requiredHash) {
			flag = true;
			break;
		}
	}
	
	return flag;
}

double getMatchesPercentage(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber)
{
	int shinglesMatch = 0;
	int shingleLength = 3;
	
	if (textWordsNumber < 4 or fragmentWordsNumber < 4) {
		shingleLength = 1;
	}
	
	int textShinglesNumber = textWordsNumber - shingleLength + 1;
	int fragmentShinglesNumber = fragmentWordsNumber - shingleLength + 1;
	
	long textShinglesArray[textShinglesNumber];
	long fragmentShinglesArray[fragmentShinglesNumber];
	
	getShinglesHashesArray(textWordsArray, textShinglesNumber, shingleLength, textShinglesArray);
	getShinglesHashesArray(fragmentWordsArray, fragmentShinglesNumber, shingleLength, fragmentShinglesArray);
	
	mergeSort(textShinglesArray, textShinglesNumber);
	
	for (int i = 0; i < fragmentShinglesNumber; i++) {
		bool isFound = binarySearch(fragmentShinglesArray[i], textShinglesArray, textShinglesNumber);
		
		if (isFound) {
			shinglesMatch++;
		}
	}
	
	return getPercentage(shinglesMatch, fragmentShinglesNumber);		
}


double getPercentage(int shinglesMatch, int fragmentShinglesNumber)
{
	double percentage = round(shinglesMatch * 10000.0 / fragmentShinglesNumber) / 100;
	return percentage;
}
