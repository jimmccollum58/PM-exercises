//============================================================================
// Name        : step3.cpp
//============================================================================

// Include files
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <pthread.h>

using namespace std;

// Errors that can be returned from the file processing thread
string FILE_OPEN_ERROR = "Cannot open input file";
string FILE_EMPTY = "No words found in input file";

// Class to process an file containing a list of words
class FileProcessor
{
private:
	// The file name to be operated on
	string fileName;

	// Weighted average length of the words in the file
	float weightedAverageLength;

	// Vector of words read from the file
	vector<string> words;

	// Total number of words found in the file
	uint32_t totalLength;

	// Function to create map of word lengths
	static vector<uint32_t> *map (vector<string> &inStrings, uint32_t (*func) (string))
	{
		vector<uint32_t> *lengthValues = new (vector<uint32_t>);
		vector<string>::const_iterator str;
		// iterate over the input string, call the provided function for each one
		for (str=inStrings.begin(); str!= inStrings.end(); ++str)
			lengthValues->push_back (func(*str));

		return lengthValues;
	}

	// Map function to return weighted length of a word
	static uint32_t mapFunc (string inString)
	{
		uint32_t length = inString.length();
		return length > 5 ? length*2 : length;
	}

	// Function to return total weighted length of the words in the file
	static uint32_t reduce (vector<uint32_t> &lengthValues, uint32_t reduceFunc (uint32_t, uint32_t))
	{
		uint32_t totalLength = lengthValues[0];

		for (uint32_t i = 1; i < lengthValues.size(); ++i)
			totalLength = reduceFunc (totalLength, lengthValues[i]);

		return totalLength;
	}

	// Reduce function to accumulate total length of the words in the file
	static uint32_t reduceFunc (uint32_t val1, uint32_t val2)
	{
		return val1+val2;
	}

	// Function that runs in a thread to process the file
	void * processFile ()
	{
		// Try to open the file for reading
		ifstream inFile (fileName.c_str());

		// Fail if it can't be opened
		if (!inFile.is_open())
			return &FILE_OPEN_ERROR;

		// Loop over the entire file, count the words, add up the total length of the words.
		// Words with length >5 are doubled weighted.
		string nextWord;
		while (inFile >> nextWord)
			words.push_back(nextWord);

		// Close the file
		inFile.close();

		// Return an error if there were no words in the file
		if (!words.size())
			return &FILE_EMPTY;

		// Call the map function for the vector of words just read in
		vector<uint32_t> *lengthValues = map (words, mapFunc);

		// Now call the reduce function for the vector of lengths
		totalLength = reduce (*lengthValues, reduceFunc);

		// Calculate the weighed average length of the words.
		weightedAverageLength = (float) totalLength/lengthValues->size();

		// Delete the vector returned from map above
		delete lengthValues;

		return NULL;
	}

	// This function is used so pthread_create sees the proper function signature.
	// It simply calls the function to do the actual file processing.
    static void *threadHelper(void *context)
    {
        pthread_exit((void*) ((FileProcessor *)context)->processFile());
    }

public:
	// Constructor, takes a file name as an argument
	FileProcessor (string inputFileName) : fileName(inputFileName) {}

	// Start a thread to process the file
	pthread_t run ()
	{
		pthread_t threadId;

		// Start a thread to process this file
		int rc = pthread_create (&threadId, NULL, &FileProcessor::threadHelper, this);
		if (rc)
		{
			cout << "Unable to create thread to process file " << fileName << "!" << endl;
			return NULL;
		}

		return threadId;
	}

	// Get the file name associated with this object
	string getFileName()
	{
		return fileName;
	}

	// Get number of words read from the file
	uint32_t getWordCount ()
	{
		return words.size();
	}

	// Total weighted length of words found in the file
	uint32_t getTotalWeightedLength ()
	{
		return totalLength;
	}

	// Get the total weighted average length of the words in the file
	float getWeightedAverageLength ()
	{
		return weightedAverageLength;
	}
};

int main(int argc, char *argv[])
{
	// One and only one argument on the command line.
	if (argc != 3)
	{
		cout << "Usage: step3 filename1.ext filename2.ext\n" << endl;
		return 1;
	}

	// Create a file processing object for each of the two files
	FileProcessor fp1 (argv[1]);
	FileProcessor fp2 (argv[2]);

	// Process the files in parallel
	pthread_t fp1ThreadId = fp1.run();
	if (!fp1ThreadId)
	{
		cout << "Could not start thread to process file " << fp1.getFileName() << "!" << endl;
	}
	pthread_t fp2ThreadId = fp2.run();
	if (!fp1ThreadId)
	{
		cout << "Could not start thread to process file " << fp2.getFileName() << "!" << endl;
	}

	// Wait for both of the file processors to complete
	void *fp1Status, *fp2Status;
	pthread_join (fp1ThreadId, &fp1Status);
	pthread_join (fp2ThreadId, &fp2Status);

	// Check for errors processing the files
	if (fp1Status)
	{
		cout << "Failed to process " << fp1.getFileName() << ": " << *static_cast<std::string *>(fp1Status) << endl;
		return 1;
	}

	if (fp2Status)
	{
		cout << "Failed to process " << fp2.getFileName() << ": " << *static_cast<std::string *>(fp2Status) << endl;
		return 1;
	}

	// Display weighted average length of the words in each file
	cout << "Weighted average word length of file " << fp1.getFileName() << " is " << fp1.getWeightedAverageLength() << endl;
	cout << "Weighted average word length of file " << fp2.getFileName() << " is " << fp2.getWeightedAverageLength() << endl;

	// Display weighted average length of the words in the files, combined
	float combinedWeightedAverageLength = (float) (fp1.getTotalWeightedLength() + fp2.getTotalWeightedLength()) / \
			(fp1.getWordCount() + fp2.getWordCount());
	cout << "Weighted average word length of combined files is " << combinedWeightedAverageLength << endl;

	return 0;
}
