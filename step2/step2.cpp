//============================================================================
// Name        : step2.cpp
//============================================================================

// Include files
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

using namespace std;

// Function prototypes
vector<uint32_t> *map (vector<string> &inStrings, uint32_t (*func) (string));
uint32_t mapFunc (string inString);
uint32_t reduce (vector<uint32_t> &lengthValues, uint32_t reduceFunc (uint32_t, uint32_t));
uint32_t reduceFunc (uint32_t val1, uint32_t val2);

int main(int argc, char *argv[])
{
	// One and only one argument on the command line.
	if (argc != 2)
	{
		cout << "Usage: step2 filename.ext" << endl;
		return 1;
	}

	// Try to open the file for reading
	ifstream inFile (argv[1], std::ifstream::in);

	// Fail if it can't be opened
	if (!inFile.is_open())
	{
        cout << "Could not open file " << argv[1] << endl;
		return 1;
	}

	// Loop over the entire file, count the words, add up the total length of the words.
	// Words with length >5 are doubled weighted.
	string nextWord;
	vector<string> words;
	while (inFile >> nextWord)
		words.push_back(nextWord);

	// Close the file
	inFile.close();

	cout << "Total of " << words.size() << " words read from " << argv[1] << endl;

	// Call the map function for the vector of words just read in
	vector<uint32_t> *lengthValues = map (words, mapFunc);

	// Now call the reduce function for the vector of lengths
	uint32_t totalLength = reduce (*lengthValues, reduceFunc);

	// Display the weighed average length of the words.
	cout << "Weighted average word length = " << (float) totalLength/lengthValues->size() << endl;

	// Delete the vector returned from map above
	delete lengthValues;

    return 0;
}

vector<uint32_t> *map (vector<string> &inStrings, uint32_t (*func) (string))
{
	vector<uint32_t> *lengthValues = new (vector<uint32_t>);
	vector<string>::const_iterator str;
	// iterate over the input string, call the provided function for each one
	for (str=inStrings.begin(); str!= inStrings.end(); ++str)
		lengthValues->push_back (func(*str));

	return lengthValues;
}

uint32_t mapFunc (string inString)
{
	uint32_t length = inString.length();
	return length > 5 ? length*2 : length;
}

uint32_t reduce (vector<uint32_t> &lengthValues, uint32_t reduceFunc (uint32_t, uint32_t))
{
	uint32_t totalLength = lengthValues[0];

	for (uint32_t i = 1; i < lengthValues.size(); ++i)
		totalLength = reduceFunc (totalLength, lengthValues[i]);

	return totalLength;
}

uint32_t reduceFunc (uint32_t val1, uint32_t val2)
{
	return val1+val2;
}
