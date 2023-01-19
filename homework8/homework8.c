#include <stdio.h>
#include <math.h>
#include <string.h>


#define ARRAY_SIZE 		500
#define MAX_STRING_LEN 	500
#define N 				8

/*
  get_sequence function gets the next value in the Hofstadter's Q sequence and returns it
*/
int get_sequence(int *array,  // Where sequence numbers are stored 
				 int index)	  // Index of the next value (wanted value)
{
	int result;
	result = array[index - array[index-1]] + array[index - array[index-2]]; 
	return result;
}

/*
  generate_hofstadters_sequence function fills given array as first two integer values are 1
  and the next values are calculated by Hofstadter's Q sequence
*/
void generate_hofstadters_sequence(int *array,  // Given array that is going to be filled 
								   int n){		// How many steps will be calculated and put in the array
	int i;

	// Setting the first two values to 1
	array[0] = 1;
	array[1] = 1;

	/*
	  until n is reached get the next value and store it in array
	  after reaching n the rest is set to 0 as blank
	*/
	for(i = 2; i < ARRAY_SIZE; ++i){
		
		if(i < n){
			array[i] = get_sequence(array, i);
		}
		else{
			array[i] = 0;
		}
	}
}

/*
  find_max finds the maximum value in the given array and returns it
  if the value at index is greater than given max value then proceed 
  to call itself with current value at index as max value
*/
int find_max(int array[],	 // Given array where max value wanted to found 
			 int index, 	 // Current value's index, or where the current value is. 0 when called for the first time
			 int max_value)	 // Max value that is passed through function. 0 when called for the first time
{ 
	/*
	  Check if end of array is reached, if that's the case stop and return max value
	  doing this first since both of the if statements lead to recursive calling
	*/
	if(index == ARRAY_SIZE){ 
		return max_value;
	}
	if(array[index] > max_value){
		find_max(array, index+1, array[index]); // If value at index is greater than given max value 
	}											// then proceed as value at index is the max value
	else{
		find_max(array, index+1, max_value);	// if else just proceed as it is
	}
}

/*
  sum_array sums all the values in the given array and returns it
*/
int sum_array(int array[]){ // Given array
	int result = 0; 		// Setting result to 0 to be able to add it to later

	if(array[2] == get_sequence(array, 2)){		// Since array is the only parameter it checks if end of array is reached 
												// by calculating the next value and comparing it to original value in array
		result = array[0] + sum_array(array+1);	// if they are identical result is the sum of current value and the rest of the array 
	}
	else{
		result = array[0] + array[1];			// if not then the end is reached thus sum the last values and set result 
	}

	return result; 
}

/*
  std_array function calculates the average and standard deviation of given array
  then returns the standard deviation and sets *mean to calculated average
*/
double std_array(int array[],	// Given array to find standard deviation and mean of the values in it 
				 double *mean, 	// Variable *mean that is going to be set when average is calculated
				 int n, 		// For how many values from the start in the array this process is done
				 int index)		// Current value's place in the array
{
	double result;

	/*
	  If there are still values to mind for mean and standard deviation value (if n > 0)
	  
	  then add current value at index to *mean, this step will be repeated till this condition (n>0) is not met anymore
	  (then *mean is the average of the first n values of array)
	  
	  when n is not greater than 0 that means all of the values are summed, proceed by dividing it to index since index is
	  increasing 1 at a time and n is decreasing 1 at a time

	  then return 0 as standard deviation when n == 0

	  since the function itself is called before in the sum, *mean is calculated and the calculation of standard deviation of
	  array can be continued correctly

	  after result is calculated check if index is greater than 0, if so that means the sum needed for standard deviation hasn't done yet
	  so return result as it is

	  lastly when returned to first and original call of the function (when index == 0) get square root of division of result by n - 1

	  then return result as standard deviation of the first n values in the array  

	*/
	if(n){		
		*mean += array[index];
		result = std_array(array, mean, n - 1, index + 1) + pow(array[index] - *mean, 2);
	}
	else{
		*mean /= index;
		return 0;
	}
	if(index){
		return result;
	}

	result = sqrt(result / (n - 1));
	return	result;
}

/*
  menu1 is the menu function of the first part of the homework
*/
void menu1(){

	// Declaration of the needed variables
	int command;
	int sequence_array[ARRAY_SIZE];
	int sequence_len;
	int max;
	int sum;
	double std;
	double mean;

	// Unless exit command 5 is entered
	while(command != 5){
		printf("\nPlease make your choice:\n");
		printf(" ------------------\n");
		printf(" 1) Fill Array\n");
		printf(" 2) Find Biggest Number\n");
		printf(" 3) Calcute Sum\n");
		printf(" 4) Calculate Standard Deviation\n");
		printf(" 5) Exit\n");

		// Get user input
		scanf("%d", &command);

		printf("\n");

		switch(command){
			case 1: 																		 // If user wants to fill the array get how many items they want to calculate and fill 
				printf("Enter a number to specify the sequence length:\n");
				scanf("%d", &sequence_len);									 				 // Get how many values is wanted to be put into array
				generate_hofstadters_sequence(sequence_array, sequence_len); 				 // Put the first n values into array
				break;
			case 2: 																		 // If maximum value in the array is wanted to be seen
				max = find_max(sequence_array, 0, 0);						 				 // Get the maximum value
				printf("The maximum value in the array is: %d\n", max);		 				 // Print it
				break;
			case 3: 																		 // If sum of the values in the array is wanted to be seen
				sum = sum_array(sequence_array);							 				 // Get sum
				printf("The sum of the values in the array is: %d\n", sum);	 				 // Print it
				break;
			case 4: 																		 // If user wants to see standard deviation of the values in array
				std = std_array(sequence_array, &mean, sequence_len, 0);	 				 // Getting standard deviation for first sequence_len many values in the array
																			 				 // since that many is calculated and put 
				printf("The standard deviation for the values in the array is: %lf\n", std); // Print it
				break;
		}
	}
}

/*
  print_board function is used for printing the situation of pipe placement in the field
  since I wasn't able to figure out how to actually print the right orientations
  this is the substitute function of it to actually see how the field irrigated
*/
void print_board(int board[N][N]){

	int x;
	int y;

	for(y = 0; y < N; ++y){
		for(x = 0; x < N; ++x){
			printf("%d\t", board[y][x]);
		}
		printf("\n\n\n\n");
	}
}

/*
  install_pipes function fills given array by how many steps later location of the current element is reached
*/
void install_pipes(int visited[N][N], // Given array representing a field
				   int x, 			  // Horizontal index indicating current element
				   int y, 			  // Vertical index indicating current element
				   int orientation,   // Value indicating where the next step will be
				   int *counter)	  // Counter for how many times the array is filled full
{

	int i;
	int dx[9] = {0, 1, -2, -1, 2, 2, -1, -2, 1}; // Change in x corresponding to orientation
	int dy[9] = {0, 2, 1, -2, -1, 1, 2, -1, -2}; // Change in y corresponding to orientation
	int nx;
	int ny;

	nx = x + dx[orientation]; // X value for the next element
	ny = y + dy[orientation]; // Y value for the next element

	/*
	  If value at (nx, ny) is 0 and ny and nx are in the [0,8] interval
	*/
	if((!visited[ny][nx]) && (ny * (7-ny) >= 0) && (nx * (7-nx) >= 0)){

		visited[ny][nx] = visited[y][x] + 1; // Then next value is 1 more of current value

		if(visited[ny][nx] < 64){ 						   // If next value is less than 8*8
			for (i = 1; (i <= N) && (*counter < 10); ++i){ // For each orientation proceed to next step
				install_pipes(visited, nx, ny, i, counter);
			}
			visited[ny][nx] = 0; // Resetting the value to 0 after every orientation is tryed
		}
		else{ // If nex value is 64 (greater than 64 actually but 64 in this case)
			*counter += 1;				// Add 1 to counter
			printf("%d)\n", *counter);	
			print_board(visited);		// Show that array is filled correctly (print the array)
			printf("\n\n\n");
		}
	}
}

/*
  fill_board function fills given array by 0s then sets element at (0,0) to 1
*/
void fill_board(int board[N][N]){

	int x;
	int y;

	for(y = 0; y < N; ++y){
		for(x = 0; x < N; ++x){
			board[y][x] = 0;
		}
	}

	board[0][0] = 1
}

/*
  remove_duplicates function corrects the given string by removing the duplicates in it
  then returns the result
*/
char *remove_duplicates(char *str){ 	// Given string

	char result_array[MAX_STRING_LEN];	// Result string
	char *result = result_array;		// Pointer to result string
	char *temp;							// Returned result from recursive called remove_duplicates function

	int i;
	int a = 0; // Indicates if current letters are different

	if(str[0] != '\0'){ 		// If first char of given str is not end of it

		if(str[0] != str[1]){	// If first and the second chars in the string are different
			result[0] = str[0];	// Set result string's first char to given string's first char
			a = 1;				// Set a to 1 since two chars are different
		}

		temp = remove_duplicates(str+1); // Result for the rest of the string

		/*
		  Copying returned result to original result, shifted to right integer a times
		  that means is shifted to right if current two chars are different (thus the first char is set to them so returned result must be shifted)
		  and directly copied if not (since the result string is empty)
		*/
		for(i = 0; temp[i] != '\0'; ++i) result[i+a] = temp[i];  // This is done until '\0' char is reached

		result[i+a] = '\0'; // Since loop stops at '\0', add it to result
	}
	else{	// Else set result to str ('\0' in this case)
		result = str;
	}

	return result;
}


/*
  menu is the menu function for the all of the homework
*/
void menu(){

	int command; // User's command

	int counter; // Counter for pipe install

	int field[N][N]; // Array for pipe install
	char wrong_text[MAX_STRING_LEN]; // String with duplicates that user entered

	while(command != 4){ // Unless user enters exit command 4
		printf("Welcome to Homework 8, please choose one of the parts to continue\n");
		printf("------------------------\n");
		printf("1) Execute Part 1\n");
		printf("2) Execute Part 2\n");
		printf("3) Execute Part 3\n");
		printf("4) Exit\n");

		scanf("%d", &command); // Get user command

		switch(command){
			case 1:
				menu1();
				break;
			case 2:
				fill_board(field);						// Ready the array to pipe install
				counter = 0;							// Set counter to 0
				install_pipes(field, 0, 0, 1, &counter);// Start the process
				break;
			case 3:
				printf("Enter some text to remove duplicates from: \n");
				scanf("%s", wrong_text);						// Get the string with duplicate chars from the user
				printf("%s \n",remove_duplicates(wrong_text));	// Print the corrected string
				break;
		}

	}
}


void main(){
	menu(); // Calls menu
}