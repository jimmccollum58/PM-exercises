//============================================================================
// Name        : step1.cpp
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

int main()
{
	vector<string> anArray;
	anArray.push_back ("Monday");
	anArray.push_back ("Tuesday");
	anArray.push_back ("Wednesday");
	anArray.push_back ("Thursday");
	anArray.push_back ("Friday");
	anArray.push_back ("Saturday");
	anArray.push_back ("Sunday");

	// Display original array
	cout << "Original array:" << endl;
	for (std::vector<string>::const_iterator str = anArray.begin(); str != anArray.end(); ++str)
	    std::cout << *str << endl;

	// Call the map function passing in the array. It returns a pointer to a chunk
	// of free space containing the transformed array.
	vector<uint32_t> *transformedArray = map (anArray, mapFunc);

	cout << "Transformed array:" << endl;
	for (std::vector<uint32_t>::const_iterator i = transformedArray->begin(); i != transformedArray->end(); ++i)
	    std::cout << *i << endl;

	// Call the reduce function on the transformed array. It returns a value after
	// reducing the transformed array.
	uint32_t result = reduce (*transformedArray, reduceFunc);

	// Delete the transformedArray vector returned from map above
	delete transformedArray;

	// Print the result of the reduced transformed array
	cout << "Final reduced result = " << result << endl;

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
	return inString.length();
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
