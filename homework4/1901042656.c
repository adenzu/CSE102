#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//get_magnitude function is used for calculating distances in track_machine
double get_magnitude(double dx, double dy){
	return (sqrt(dx * dx + dy * dy));
}

//Linking numbers to characters that are mentioned in part 2 of homework 
//this function will be used in decrypt_and_print function to get needed characters
//by numbers
char decrypt_numbers(int number){
	char letter;

	switch(number){ //Setting letter to the right character for the case
		case 1:
			letter = '-';
			break;
		case 2:
			letter = '_';
			break;
		case 3:
			letter = '|';
			break;
		case 4:
			letter = '/';
			break;
		case 5:
			letter = '\\';
			break;
		case 6:
			letter = 'O';
			break;
		default:
			letter = ' ';
	}
	return(letter);
}

//The exact opposite of the decrypt_numbers function
//This function will be used in encrypt_messages function 
//to encrypt the message in decrypted_p4 file
int encrypt_chars(char letter){ 
	int number;

	switch(letter){ //Setting number to the right integer for the case
		case '-':
			number = 1;
			break;
		case '_':
			number = 2;
			break;
		case '|':
			number = 3;
			break;
		case '/':
			number = 4;
			break;
		case '\\':
			number = 5;
			break;
		case 'O':
			number = 6;
			break;
		default:
			number = 0;
	}
	return(number);
}

void decrypt_and_print(char *file_path){
	char number; 
	FILE *file;

	//Opening the decrypted file to read
	file = fopen(file_path, "r"); 

	//I made it to read as char instead of integer
	//to be able to check if '\n' is encountered
	while (fscanf(file, "%c", &number)==1) //As long as reading process is succesful  
	{ 
		if(number == '\n'){ //If it is end of the line ('\n')
			printf("\n"); //Get to the new line (in terminal)
		}
		else{ //If it's not the end of the line (thus it's a number)
			printf("%c", decrypt_numbers((number - '0'))); //Print decryption of the number 
			// I only found the char_variable - '0' method to convert characters that are numbers to integers
		}
	} 
}

void deep_decrypt_and_print(char *file_path){
	char number;
	int sum;
	int i;
	FILE *file;

	// Opening file to read
	file = fopen(file_path, "r");

	while (1)  
	{
		for (i = 0, sum = 0; i < 3; i++) // 3 times for 3 integers
		{	
			fscanf(file, "%c", &number); // Read number as char

			// If the read "number" is '\n' then break the for loop.
			// We are assuming the next numbers to be 0 (as it been said in the homework)
			// so there's no need to continue on, as 0 doesn't 
			// change the outcome of sums and subtractions
			if(number == '\n'){ 
				break;
			}

			// Again using the char_variable - '0' method 
			// to convert chars that are numbers to integers
			sum += number - '0';
		}

		// Printing the decryption of the number that is mod 7 of sum
		printf("%c", decrypt_numbers(sum%7));

		// If the first encountered read char is '\n'
		// ('i' will be 1 in this case due to breaking of loop)
		if(i == 1){
			printf("\n"); // Get to the new line in terminal (print '\n')

			fscanf(file, "%c", &number); // Read the next char 

			// If we've come to the end of file end the while loop
			if (feof(file))	break; 

			// else get 1 char back so the next sum won't be messed up
			// (not using 'else' here because if the first condition is true the code won't continue anyway)
			fseek(file, -1, SEEK_CUR);
		}

		// If the encountered char is a number then get 2 back to find
		// the sum of 3 numbers.
		// This is necessary due to one number at a time progress 
		else{
			fseek(file, -2, SEEK_CUR);
		}
	} 
}

void refresh_position(int *X, int *Y, double *D, double *R){
	// Saving the old values of X and Y
	int ox = *X;
	int oy = *Y;

	// Setting their new values randomly (and in [1,11])
	*X = 1 + rand() % 11;
	*Y = 1 + rand() % 11;

	*R = get_magnitude(*X - ox, *Y - oy); // Using get_magnitude func to find the distance of repositioning of enemies 
	*D = get_magnitude(*X - 6, *Y - 6); // Using the same func to find distance between us and enemies (assuming we are located at (6, 6))
}

void track_machine(){
	// We are not actually changing our location but 
	// it's better to set variables for it anyway
	int x = 6;
	int y = 6;

	// First position values of enemies as it is menioned in 
	// part 3 of homework.
	int ex = 0;
	int ey = 0;

	// distance variable for the distance between us and the enemy
	// repositioning variable for the distance that is taken during the enemy repositioning
	double distance;
	double repositioning;

	// Variables to use in for loops
	int dx;
	int dy;

	// Declaring command variable and setting it's initial value to 'R'
	// to print UI the first time 
	char command = 'R';

	// Until user enters 'E' as command
	while(command != 'E'){

		// There was an issue that the terminal took '\n' as command
		// when I entered 'E' or 'R' or anything to the terminal.
		// So dealed with it this way
		if(command != '\n'){ 

			// Refresh variable values if entered command is 'R'
			if(command == 'R'){
				refresh_position(&ex, &ey, &distance, &repositioning);
			}

			// Using 2 nested for loops to scan every combination there is
			// e.g (6, 9)
			for (dy = 1; dy < 12; ++dy)
			{
				for (dx = 1; dx < 12; ++dx)
				{
					if(dx == x && dy == y){ // If the current combination is (6, 6)
						printf(" O"); // Print 'O' to represent us
					}
					else if(dx == ex && dy == ey){ // If the current combination is enemy's location
						printf(" E"); // Print 'E' to represent enemy
					}
					else{
						printf(" ."); // Else print '.' to represent space
					}
				}
				printf("\n"); // Get to the new line
			}

			// Inform user
			printf("Enemies X position: %d, Y position: %d, Displacement: %.2lf, Distance to our camp: %.2lf\n", ex, ey, repositioning, distance);
			printf("Command waiting...:\n");
		}
		scanf("%c", &command); // Get user command
	}
}

void encrypt_messages(char *file_path){
	char character;

	// upperlimit variable is for limiting i in for loop
	// thus the first two sums will be calculated right
	int upperlimit = 1; 
	int sum;
	int i;

	// Opening the file that is going to be encrypted
	FILE *decrypted_file = fopen(file_path, "r");

	// Opening the "encrypted_p4" file to write in it later on
	FILE *encrypted_file = fopen("encrypted_p4.img", "w");

	while(1){

		// Doing upperlimit times
		for (i = 0, sum = 0; i < upperlimit; ++i)
		{
			fscanf(decrypted_file, "%c", &character); // Reading file by char

			// If '\n' is encountered that means we are at the end of line
			// and there's no more number to sum thus it sets sum to '\n'
			// instead of a number
			if(character == '\n'){
				sum = '\n';
				break;
			}
			else{
				// Adding related number to read character to sum
				sum += encrypt_chars(character);
			}
		}

		// Increase upperlimit till it's 3
		// this way first sum will be the firs read number
		// second sum will be sum of first two read numbers
		// then it will read and sum 3 numbers at a time
		if(upperlimit < 3){
			upperlimit++;
		}

		// Resetting upperlimit to 1 if '\n' is encountered (end of line is reached)
		if(character == '\n'){
			upperlimit = 1;

			// Chencking if it is the last read character
			fscanf(decrypted_file, "%c", &character);

			if(feof(decrypted_file)){ // If it's end of file end the while loop
				break;
			}

			// Else get 1 back in file to don't mess up next sum
			fseek(decrypted_file, -1, SEEK_CUR);

			// It turned out be not the end of file but just '\n'
			// so gets to new line in encrypted_file as well
			fputc('\n', encrypted_file);	 		
		}
		else{
			// If '\n' is not encountered then write sum to file
			fprintf(encrypted_file, "%d", sum);
		}
	}
}

void menu(){

	// Declaring command variable and setting it to -1 so while loop can run
	int command = -1;

	// Unless user enters '5' as command
	while(command != 5){

		// Informing the user about what can they do
		printf("1-) Decrypt and print encypted_p1.img\n2-) Decrypt and print encypted_p2.img\n3-) Switch on the tracking machine\n4-) Encrypt the message\n5-) Switch off\n");

		// Getting the command from the user
		scanf("%d", &command);

		// Running the related functions to the entered command
		// No case 5 since while loop will stop by itself
		switch(command){
			case 1:
				decrypt_and_print("encrypted_p1.img");
				break;
			case 2:
				deep_decrypt_and_print("encrypted_p2.img");
				break;
			case 3:
				track_machine();
				break;
			default: // case 4
				encrypt_messages("decrypted_p4.img");
				break;
		}
	}
}

void main(){
	menu();
}