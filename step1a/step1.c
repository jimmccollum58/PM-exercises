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

// Function prototypes
uint32_t* map (void* arrayArg[], uint32_t arraySize, uint32_t (*func) (void *));
uint32_t mapFunc (void* arg);
uint32_t reduce (uint32_t arrayArg[], uint32_t arraySize, uint32_t (*func) (uint32_t, uint32_t));
uint32_t reduceFunc (uint32_t arg1, uint32_t arg2);

// Main function. Allocate an array, call the map function to get a transformed array,
// call the reduce function to reduce it, then print the result.
int main(void)
{
	#define ARRAY_SIZE 7
	char* anArray [ARRAY_SIZE] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	uint32_t *pTransformedArray;
	uint32_t i, result;

	printf ("Original array:\n");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf ("[%d] = %s\n", i, anArray[i]);

	// Call the map function passing in the array. It returns a pointer to a chunk
	// of free space containing the transformed array.
	pTransformedArray = map ((void*)anArray, ARRAY_SIZE, mapFunc);

	printf ("Transformed array:\n");
	for (i = 0; i < ARRAY_SIZE; i++)
		printf ("[%d] = %d\n", i, pTransformedArray[i]);

	// Call the reduce function on the transformed array. It returns a value after
	// reducing the transformed array.
	result = reduce (pTransformedArray, ARRAY_SIZE, reduceFunc);

	// Print the result of the reduced transformed array
	printf ("Final reduced result = %d\n", result);

	// Release the free space containing the transformed array.
	free ((void *) pTransformedArray);

	return EXIT_SUCCESS;
}

// The map function takes an array of unsigned integers, its size and
// a function to call to transform each element in the array
uint32_t * map (void* arrayArg[], uint32_t arraySize, uint32_t (*func) (void *))
{
	uint32_t i;

	// Allocate space for the array to return from free space
	uint32_t *retArray = (uint32_t *) malloc (arraySize * sizeof (uint32_t));

	// Loop over the array passed in. For each element, call the transform function
	// that was passed in and store its result in the array allocated from free space.
	for (i = 0; i < arraySize; i++)
		retArray[i] = func (arrayArg[i]);

	return retArray;
}

// The transform function just returns the square of the value passed in
uint32_t mapFunc (void* arg)
{
	return strlen(arg);
}

// The reduce function takes an input array and its size and a function to call
// for each value in the array.
uint32_t reduce (uint32_t arrayArg[], uint32_t arraySize, uint32_t (*func) (uint32_t, uint32_t))
{
	uint32_t i, result = arrayArg[0];

	// Loop over the input array, call the function passed in by the caller
	// for each element. Add all the values together and return the result.
	for (i = 1; i < arraySize; i++)
		result = func (result, arrayArg[i]);

	return result;
}

// The reduce function just returns the sum of the values passed in
uint32_t reduceFunc (uint32_t arg1, uint32_t arg2)
{
	return min(arg1, arg2);
}
