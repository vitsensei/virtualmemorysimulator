#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//this file contains helper functions that
//1. Reset all elements of a given array back to zero
//a function that resets all element to zero, passing in the array and the size of it
void resetArray(int num[], int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		num[i] = 0;
	}

}


//2. A function to shift every element to the right, and then eliminate the element at the end totally
void shiftRight(int num[], int size)
{
	int i;

	for(i = size - 1; i > 0; i --) // starting from the last element, move down to the a[0] element
	{
		num[i] = num[i-1]; // the current element is equal to the element to the left of it (shift right)
	}
	num[0] = 0; // assigning the first element to zero, because when we shift right, the first always gonna be zero
}

//a function to convert from dec to binary in int form
int convert(int dec)
{
	if (dec == 0)
    {
        return 0;
    }
    else
    {
        return (dec % 2 + 10 * convert(dec / 2));
    }
}

//a function to convert from an int array to a char array, and then from a char to an int
int buildBinary(int num[], int size)
{
	int i;
	int dec = 0;
	int bi = 0;
	int base = 2;

	for(i = 0; i < size; i++)
	{
		dec = base * dec + num[i];
	}

	//at this point, int bi is a decimal representation of the num array
	//now we convert from decimal to binary
	bi = convert(dec);

	return bi;
}

int main()
{
	int a[8];
	int i;
	a[0] = 1;
	a[1] = 0;
	a[2] = 1;
	a[3] = 1;
	a[4] = 0;
	a[5] = 1;
	a[6] = 1;
	a[7] = 0;


	//piece of code for testing shiftRight

	int k = buildBinary(a,8);
	for(i=0; i<8;i++)
	{
		printf("%i",a[i]);
	}
	printf("\n");
	printf("k: %i\n",k);

	// printf("shiftRight\n");
	// //resetArray(a,8);
	// shiftRight(a,8);
	// shiftRight(a,8);
	// shiftRight(a,8);
	// for(i=0; i<8;i++)
	// {
	// 	printf("%i",a[i]);
	// }
	// printf("\n");



 
	return 0;
}