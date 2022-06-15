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

int main() {
	
	string str1 = "Experiments show how product pathways can be controlled by irradiation with one or more laser beams during individual bimolecular collisions or during unimolecular decompositions.For bimolecular collisions, control has been achieved by selective excitation of reagent vibrational modes, by control of reagent approach geometry, and by control of orbital alignment.For unimolecular reactions, control has been achieved by quantum interference between different reaction pathways connecting the same initial and final states and by adjusting the temporal shape and spectral content of ultrashort, chirped pulses of radiation.These collision-control experiments deeply enrich the understanding of how chemical reactions occur. The calculation of absolute reaction rates is formulated in terms of quantities which are available from the potential surfaces which can be constructed at the present time.The probability of the activated state is calculated using ordinary statistical mechanics.This probability multiplied by the rate of decomposition gives the specific rate of reaction.The occurrence of quantized vibrations in the activated complex, in degrees of freedom which are unquantized in the original molecules, leads to relative reaction rates for isotopes quite different from the rates predicted using simple kinetic theory.The necessary conditions for the general statistical treatment to reduce to the usual kinetic treatment are given. The main objective of the current study was to introduce a Deep Learning Neural Network (DLNN) model in landslide susceptibility assessments and compare its predictive performance with state-of-the-art machine learning models.The efficiency of the DLNN model was estimated for the Kon Tum Province, Viet Nam, an area characterized by the presence of landslide phenomena.Nine landslide related variables, elevation, slope angle, aspect, land use, normalized difference vegetation index, soil type, distance to faults, distance to geology boundaries,lithology cover, and 1,657 landslide locations, were used so as to produce the training and validation datasets during the landslide susceptibility assessment.The Frequency Ratio method was used so as to estimate the existing relation between the landslide-related variables and the presence of landslides, assigning to each variable class a weight value.Based on the results concerning the predictive ability of the landslide related variables which was evaluated using the Information ration method, all variables were further processed since they appear as highly predictive.The learning ability of the DLNN model has been evaluated and compared with a Multi Layer Preceptron Neural Network, a Support Vector Machine,a C4.5-Decision Tree model and a Random Forest model using the training dataset, whereas the predictive performance of each model has been evaluated and compared using the validation datasets.In order to evaluate their learning and predictive capacity of each model the classification accuracy, the sensitivity, the specificity and the area under the success and predictive rate curves (AUC) were calculated.Results showed that the proposed DLNN model had a higher performance than the four benchmark models. Although DLNN has been used seldom in landslide susceptibility assessments,the study highlights that the usage of deep learning approach could be considered as a satisfactory alternative approach for landslide susceptibility mapping.Solar energy can be harnessed by direct conversion of solar energy to electrical energy by using photovoltaic (PV) technology or with the help of thermodynamic cycle by using concentrated solar power (CSP) technologies.The goal of this paper is to perform detailed comparative analysis for the two solar technologies namely: PV and dish Stirling engine (DSE).In order to ensure a fair comparison, the two systems are designed for the same rated capacity of 1 MW for Tabuk, Saudi Arabia.The average values of direct normal irradiance (DNI) and global horizontal irradiance (GHI) in Tabuk are 7.43 kWh/m2/day and 6.31 kWh/m2/day, respectively.The two technologies are analyzed based on several performance indicator factors such as capacity factor (CF), solar to electric efficiency, energy output, levelized electricity cost (LEC), and net present value (NPV).The comparative analysis of the two technologies shows that PV system has 6.9% better CF and 54% more energy output compared to DSE system.The LEC of PV systems is 3.39 ?/kWh which is much better than the 11.74 ?/kWh from the DSE system.The NPV of the PV system is positive while DSE system has negative NPV.This paper present useful insight of the two technologies which is helpful to choose a solar power technology for a power project.";
	string str2 = "Experiments show how product pathways can be controlled by irradiation with one or more laser beams during individual bimolecular collisions or during unimolecular decompositions.";
	
	cout << antiPlagiarism(str1, str2);
	
	return 0;
}

double antiPlagiarism(string text, string fragment) {
	int wordMatch = 0;
	int shingleMatch = 0;
	int numberOfShingles = 0;
	int position = 0;
		
	text = canonizeText(text);
	fragment = canonizeText(fragment);
	
	int textSize = getAmoutOfWords(text);
	int fragmentSize = getAmoutOfWords(fragment);
	

	if (fragmentSize == 0 or textSize == 0) {
			return 0.0;
	}

	string wordsOfText[textSize];
	string wordsOfFragment[fragmentSize];
	
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
					canonizedText = canonizedText + wordToLowerCase(word) + space;
					word = EMPTY_STRING;	
				}
				else {
					word = EMPTY_STRING;
				}
			}
			if (text[i + 1] == 0) {
				if (!isForbiddenWord(wordToLowerCase(word))) {
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
