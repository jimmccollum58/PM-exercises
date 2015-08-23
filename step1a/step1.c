//============================================================================
// Name        : step1.c
//============================================================================

// Include files
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Define a "min" function
#define min(a, b) (((a) < (b)) ? (a) : (b))

// The type for the map values
typedef char* mapVal;
// The type for the reduced value
typedef char* reduceVal;

// Function prototypes
reduceVal* map (mapVal arrayArg[], uint32_t arraySize, reduceVal (*func) (mapVal));
reduceVal mapFunc (mapVal arg);
reduceVal reduce (reduceVal arrayArg[], uint32_t arraySize, reduceVal (*func) (reduceVal, reduceVal));
reduceVal reduceFunc (reduceVal arg1, reduceVal arg2);

// Main function. Allocate an array, call the map function to get a transformed array,
// call the reduce function to reduce it, then print the result.
int main(void)
{
	#define ARRAY_SIZE 7
	char* anArray [ARRAY_SIZE] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	reduceVal *pTransformedArray;
	uint32_t i;
	reduceVal result;

	printf ("Original array:\n");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf ("[%d] = %s\n", i, anArray[i]);

	// Call the map function passing in the array. It returns a pointer to a chunk
	// of free space containing the transformed array (or NULL on failure)
	pTransformedArray = map (anArray, ARRAY_SIZE, mapFunc);
	if (!pTransformedArray)
		return 1;

	printf ("Transformed array:\n");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf ("[%d] = %s\n", i, pTransformedArray[i]);

	// Call the reduce function on the transformed array. It returns a value after
	// reducing the transformed array.
	result = reduce (pTransformedArray, ARRAY_SIZE, reduceFunc);

	// Print the result of the reduced transformed array
	printf ("Final reduced result = %s\n", result);

	// Release the free space containing the transformed array.
	for (i = 0; i < ARRAY_SIZE; i++)
		free ((void *) pTransformedArray[i]);
	free ((void *) pTransformedArray);

	return EXIT_SUCCESS;
}

// The map function takes an array of unsigned integers, its size and
// a function to call to transform each element in the array
reduceVal * map (mapVal arrayArg[], uint32_t arraySize, reduceVal (*func) (mapVal))
{
	uint32_t i;

	// Allocate space for the array to return from free space
	reduceVal *retArray = (reduceVal *) malloc (arraySize * sizeof (reduceVal));
	if (!retArray)
	{
		printf ("Failed to allocate %d bytes for reduce array!\n", (int) (arraySize * sizeof (reduceVal)));
		return NULL;
	}

	// Loop over the array passed in. For each element, call the transform function
	// that was passed in and store its result in the array allocated from free space.
	for (i = 0; i < arraySize; i++)
	{
		retArray[i] = func (arrayArg[i]);
		if (!retArray[i])
		{
			uint32_t j;
			// Failed to allocate space for the reduce value. Need to deallocate and space
			// already allocated for the reduce array values
			for (j = 0; j < i; j++)
				free (retArray[j]);
		}
	}

	return retArray;
}

// The transform function just returns the square of the value passed in
reduceVal mapFunc (mapVal arg)
{
	uint32_t mapValLen = strlen(arg) > 3 ? 3 : strlen(arg);
	reduceVal retVal = (reduceVal) malloc (mapValLen + 1);
	if (!retVal)
	{
		printf ("Failed to allocate %d bytes for reduce value!\n", (int) (mapValLen + 1));
		return NULL;
	}
	memset (retVal, 0, mapValLen + 1);
	strncpy (retVal, arg, mapValLen);
	return retVal;
}

// The reduce function takes an input array and its size and a function to call
// for each value in the array.
reduceVal reduce (reduceVal arrayArg[], uint32_t arraySize, reduceVal (*func) (reduceVal, reduceVal))
{
	uint32_t i;
	reduceVal result = arrayArg[0];

	// Loop over the input array, call the function passed in by the caller
	// for each element. Add all the values together and return the result.
	for (i = 1; i < arraySize; i++)
		result = func (result, arrayArg[i]);

	return result;
}

// The reduce function just returns the sum of the values passed in
reduceVal reduceFunc (reduceVal arg1, reduceVal arg2)
{
	return strcmp(arg1, arg2) < 0 ? arg1 : arg2;
}
