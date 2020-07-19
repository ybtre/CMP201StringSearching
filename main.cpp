#include <iostream>
#include <string>
#include <fstream> // used to load a .txt file
#include <chrono> // used to measure time of algorithms

using namespace std;

using chrono::duration_cast;
using chrono::milliseconds;
using the_clock = chrono::steady_clock;


void loadText(string& inputText) { 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// book excerpt was taken from https://archive.org/details/TheHobbitEnhancedEditionJ.R.R.Tolkien/page/n1
	// The Hobbit ( Enhanced Edition) J. R. R. Tolkien
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	fstream fStream;
	string book = "book.txt";

	ifstream fStreamBook(book);
	
	string bookContent((istreambuf_iterator<char>(fStreamBook)),
		(istreambuf_iterator<char>()));
	inputText = bookContent;
}

void boyerMoore(string& input, string& inputText) {
	long int counter = 0;
	long int i, j;
	int skip[256];
	for (i = 0; i < 256; ++i)
		skip[i] = input.size();

	for (i = 0; i < input.size(); ++i)
		skip[int(input[i])] = (input.size() - 1) - i;

	for (i = 0; i < inputText.size() - input.size(); ++i) {
		long int s = skip[int(inputText[i + input.size() - 1])];
		if (s != 0) {
			i += s - 1;
			continue;
		}
		for (j = 0; j < input.size(); ++j) {
			if (inputText[i + j] != input[j])
				break;
		}
		if (j == input.size()) {
			cout << "Pattern found at position: " << i << endl;
			counter++;
		}
	}

	if (counter == 1) {
		cout << "Found " << counter << " match." << endl;
	}
	else {
		cout << "Found " << counter << " matches." << endl;
	}
}

// Algorithm was made after this one https://www.sanfoundry.com/cpp-program-implement-rabinkarp-algorithm/
void rabinKarp(string input, string inputText) {
	long int counter = 0;
	long int i, j;										// Loop iterators
	int h = 1;											
	int inputHash = 0, patternHash = 0;
	const int charCount = 256;							
	const int q = 101;									
	bool flag = false;

	for (i = 0; i < input.length() - 1; i++)
		h = (h * charCount) % q;
	for (i = 0; i < input.length(); i++) {
		inputHash = (charCount * inputHash + inputText[i]) % q;
		patternHash = (charCount * patternHash + input[i]) % q;
	}

	for (i = 0; i <= inputText.length() - input.length(); i++) {
		if (inputHash == patternHash) {									
			for (j = 0; j < input.length(); j++)
				if (inputText[i + j] != input[j])
					break;
			if (j == input.length()) {
				cout << "Pattern found at position: " << i << endl;
				counter++;
			}
		}
		inputHash = (charCount * (inputHash - h * inputText[i]) + inputText[i + input.length()]) % q;
		if (inputHash < 0)
			inputHash = inputHash + q;
	}
	if (counter == 1) {
		cout << "Found " << counter << " match." << endl;
	 } else {
		cout << "Found " << counter << " matches." << endl;
	}

	if (!flag)
		cout << "String not found.." << endl;
	return;
}

void handleInput(int choice, string pattern, string inputText, int& timeTaken) {
	
		the_clock::time_point startTime = the_clock::now();
			if (choice == 1) {
				cout << endl << "Rabin Karp algorithm output:" << endl;
				rabinKarp(pattern, inputText);
			} else if (choice == 2) {
				cout << endl << "Boyer Moore algorithm output:" << endl;
				boyerMoore(pattern, inputText);
			}
		the_clock::time_point endTime = the_clock::now();
			
		if (choice == 1) {
			auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
			cout << "It took " << time_taken << " ms. Using Rabin Karp" << endl;

			timeTaken = time_taken;
		} else if (choice == 2) {
			auto time_taken = duration_cast<milliseconds>(endTime - startTime).count();
			cout << "It took " << time_taken << " ms. Using Boyer Moore" << endl;

			timeTaken = time_taken;
		}
		
	}
 
string inputString() {
	string string;
	cout << "What would you like to search for? ";
		getline(cin, string);
	
	return string;
}

int chooseAlgorithm() {
	int choice;
	
	cout << endl << "1. Rabin Karp" << endl;
	cout << "2. Boyer Moore" << endl;
	cout << "Please select an algorithm to search with: ";
	cin >> choice;
	
	return choice;
}

int main() {
	string inputText;

	int timeTaken;
	int totalTime = 0;
	int median;
	int loops = 0;
	
	string input = inputString();				// Enter a pattern to search for. 
	while (input.length() == 0) {				// Pattern cannot be an empty space and is case Sensitive
		cout << "Invalid pattern. " << endl;	// Prompt the user to keep entering a pattern until it's valid 
		input = inputString();
	}
	
	loadText(inputText);
	
	int choice = chooseAlgorithm();
	for (int i = 0; i < 100; ++i) {
		(choice == 1)
			? (handleInput(1, input, inputText, timeTaken))
			: (handleInput(2, input, inputText, timeTaken));

		loops += 1;
		totalTime += timeTaken;
	};
	
	median = totalTime / 100;

	cout << endl << "Ran algorithm: " << loops << " times" << endl;
	cout << "Total time taken: " << totalTime << endl;
	cout << "Median: " << median << endl;
	return 0;
}