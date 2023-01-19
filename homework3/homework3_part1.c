 #include <stdio.h>

int sum(int n, int flag){
	int result;
	int i;

	for (i = 1, result = 0;  
		 i < (n/2) + (flag + n % 2) / 2; // There's not equal amount of odd and even numbers in [1, N] interval where N is an odd number, this equation handles that.
		 i++) // Adding to i one at a time instead of 2 because of the for inequality.
	{
		result += 2 * i - flag; // Multiplying i by 2 and subtracting the flag to get the right number to add  (2 * 1 - 1 = 1)
		printf("%d + ", 2 * i - flag); // Printing the sum equation partly.
	}

	// Doing the last bit out of the loop to print '=' instead of '+'
	result += 2 * i - flag;
	printf("%d = ", 2 * i - flag);

	return (result);
}

int mult(int n, int flag){
	int result;
	int i;

	for (i = 1, result = 1; // Set result as 1 instead of 0 to be able to multiple it. 
		 i < (n/2) + (flag + n % 2) / 2; // There's not equal amount of odd and even numbers in [1, N] interval where N is an odd number, this equation handles that.
		 i++) // Adding to i one at a time instead of 2 because of the for inequality.
	{
		result *= 2 * i - flag; // Multiplying i by 2 and subtracting the flag to get the right number to multiply the result  (2 * 1 - 1 = 1)
		printf("%d * ", 2 * i - flag); // Printing the multiplication equation partly.
	}

	// Doing the last bit out of the loop to print '=' instead of '*'
	result *= 2 * i - flag;
	printf("%d = ", 2 * i - flag);

	return (result);
}

void main(){

	int integer;
	int operation;
	int odd_even;
	int condition;
	int result;

	// Getting desired operation and odd/even flag and integer.
	printf("Enter an integer: \n");
	scanf("%d", &integer);

	printf("Please enter '0' for sum, '1' for multiplication.\n");
	scanf("%d", &operation);
	
	printf("Please enter '0' to work on even numbers, '1' to work on odd numbers.\n");
	scanf("%d", &odd_even);


	// Using random numbers to evaluate specific results from the 2 conditions operation and odd_even to use in switch-case 
	condition = (operation == 1) * 5 + (operation == 0) * -5 + (odd_even == 1 || odd_even == 0);

	switch(condition){

		case 6:  // If operation is 1 and odd_even is entered as 1 or 0  (5+1)
			result = mult(integer, odd_even);
			break;

		case -4: // If operation is 0 and odd_even is entered as 1 or 0  (-5+1)
			result = sum(integer, odd_even);
			break;

		case 1:  // If operation is not 1 or 0 but odd_even is entered as 1 or 0  (0+1)
			printf("Unsupported operation.\n");
			break;

		default: // If operation is 1 or 0 but odd_even is not, or both of them are not 1 or 0
			printf("Invalid value for odd/even selection.\n");
	}

	printf("%d\n", result); // Completing the equation by printing the result.
}