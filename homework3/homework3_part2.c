#include <stdio.h>
#include <math.h>

int isprime(int n){
	int i;
	int sqrt_n = sqrt(n);

	for (i = 2; i <= sqrt_n; ++i) // Trying to divide n by i till i is greater than square root of n.
	{
		if (n % i == 0)
		{
			return i; // Immediately returning the divisor to prevent i from getting bigger.
		}
	}

	return 0; // Return 0 if no divisor was found.
}

void main(){
	int integer;
	int i;
	int divisor;

	// Getting the integer that is limit.
	printf("Please enter an integer: \n");
	scanf("%d", &integer);

	for (i = 2; i < integer; ++i)
	{
		divisor = isprime(i); // Getting the number's divisor if there's one.

		if (divisor)
		{
			printf("%d is not a prime number, it is divisible by %d\n", i, divisor);
		}
		else{
			printf("%d is a prime number.\n", i);
		}
	}
}