#include <stdio.h>

int andop(int bin1, int bin2){
	int result = 0;
	int new_digit = 1; // The integer variable that is going to be added to result. 

	while(bin1 > 0 || bin2 > 0){ // Unless both of the integers are shifted to right enough to be 0.

		if((bin1 + bin2) % 10 == 2){ // If both of the mod10 remainders are 1  (1+1=2)
			result += new_digit; // Add new_digit to result.
		}

		// Shifting 'binary' integers one to right and new_digit one to left.
		bin1 /= 10;
		bin2 /= 10;
		new_digit *= 10;

	}

	return result;
}

void main(){
	int bin1, bin2;
	int bin1_copy, bin2_copy;
	int error = 1;
	int result;


	// Declaring error as 1 so that while can run.
	while(error){
		error = 1; // Resetting error if it is changed to 2 in last while loop

		// Getting 'binary' integers.
		printf("First integer: \n");
		scanf("%d", &bin1);
		bin1_copy = bin1;

		printf("Second integer: \n");
		scanf("%d", &bin2);
		bin2_copy = bin2;

		// Using 'copies' to check if the entered numbers are matching the criteria. 
		while(bin1_copy > 0 && bin2_copy > 0){

			// Checking if remainder from one of the integers is different from 0 and 1.
			if((bin1_copy % 10 > 1) || (bin2_copy % 10 > 1)){
				error = 2; // Assigning error to a different integer to check afterwards.
			}

			// Shifting 'binary' integers one to right.
			bin1_copy /= 10; 
			bin2_copy /= 10;
		}

		if(error == 2){ // If one or both of the integers are not 'binary'.
			printf("Integers should be binary, please enter 2 new integers.\n");
		}
		else if(bin1_copy != bin2_copy){ // If they don't have the same length  (with the same length integers it will be '0 != 0')
			printf("Integers should have the same length, please enter 2 new integers.\n");
		}
		else{ // If no error is found.
			error = 0;
		}
	}

	// Evaluating the result and informing the client.
	result = andop(bin1, bin2);
	printf("%d AND %d = %d \n", bin1, bin2, result);
}