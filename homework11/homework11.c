#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/*-------------------------Definitions-------------------------*/
#define EOA -1 // End Of Array

/*
**########################################################################
**#																		 #
**#								  PART 1								 #
**#																		 #
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


/*-------------------------Creating Integer Array-------------------------*/

/*
	file_text function reads the file at the given path and returns read content as string
*/
char *file_text(char *filename){

	char *text = (char *) calloc(1000000, sizeof(char));

	FILE *file;
	file = fopen(filename, "r");

	fscanf(file, "%s", text);

	fclose(file);

	return text;
}

/*
	read_array function reads the integers in the file at the given path
	then puts the integers into an array and returns it
*/
int *read_array(char *filename){

	// Reading and storing the content of the file
	char *text;
	text = file_text(filename);

	int i;
	int num_count;

	// Checks each and every char in the text if it's comma
	// since there's a comma after every integer they are equal in quantity
	// so by counting commas we get the number of integers there are
	for(i = 0, num_count = 0; text[i] != '\0'; i++){
		if(text[i] == ',') num_count++;
	}

	// Inform the client
	printf("For the integer array %d bytes are allocated (assuming 4-byte integers are used)\n", (num_count+1) * 4);

	int *int_array = (int *) calloc(num_count+1, sizeof(int));

	// Scanning the first integer since commas are after the integers
	sscanf(text, "%d", int_array);
	int_array++;	// Adjusting the array

	// Checks each char if it's a comma and if it is
	// scans the integer right to it and puts in array
	// location of new element in array is adjusted each time
	// start of the scan is indicated with i which is basically current char's index and text address
	for(i = 0; text[i] != '\0'; i++){
		if(text[i] == ',') sscanf(text+i+1, "%d", int_array++);
	}

	// After the last comma there's no integer yet this causes unnecessary array adjustment
	// thus array is adjusted backwards once then the element at the address is set to EOA
	*(--int_array) = EOA;

	return (int_array - num_count);
}


/*-------------------------Creating Integer LinkedList-------------------------*/

/*
	struct data_integer is the structure that is used as linked list node
	called data integer because integers in given file are treated as data
	in further processes
*/
typedef struct data_integer
{
	int value;					// Node's integer value
	struct data_integer *next;	// Next linked list node
} dataInteger;

/*
	create_data_integer function declares and assigns a new linked list entry
	the new entry's value is the given value
	function returns allocated address for the new entry
*/
dataInteger *create_data_integer(int value){

	dataInteger *new_entry = (dataInteger *) malloc(sizeof(dataInteger));
	new_entry->value = value;
	new_entry->next = NULL;

	return new_entry;
}

/*
	append_data_integer_entry function finds the last node of the linked list by the given linked list head/banner
	then creates a new node whose value equal to the given value and puts it at the end of the linked list
*/
dataInteger *append_data_integer_entry(dataInteger *head, int value){

	// Not doing the processes with head variable but using another one
	dataInteger *curr_data_integer = head;	

	// Until last node is reached, continue
	while(curr_data_integer->next != NULL) curr_data_integer = curr_data_integer->next;

	// Then set last node's next to the newly created node whose value is the given value for the function
	curr_data_integer->next = create_data_integer(value);

	return (curr_data_integer->next);
}

/*
	read_linkedlist function reads the integer at the given file path
	then stores them in a linked list and returns the linked list head
*/
dataInteger *read_linkedlist(char *filename){

	// Opening the file
	FILE *file;
	file = fopen(filename, "r");

	// Variable that stores read number
	int curr_num;

	// Declaring a blank node to have a start point
	dataInteger *head = create_data_integer(0);
	dataInteger *last_added = head;
	
	// Read first number
	fscanf(file, "%d", &curr_num);
	int len = 0;

	while(!feof(file)){
		last_added = append_data_integer_entry(last_added, curr_num);	// Append last read number to the linked list
		fgetc(file);					// Reads comma so the next number can be read
		fscanf(file, "%d", &curr_num);	// Read next number
		len++;	// Increase length of the linked list by 1
	}

	fclose(file);

	// Inform the client
	printf("For the integer linked list %d bytes are allocated (assuming 4-byte integers and adresses are used)\n", len * (4 + 4));

	// Returns next node from the head since head is a blank node
	return (head->next);
}


/*-------------------------Calculating Array Statistics-------------------------*/

/*
	std_mean_array function calculates mean and standard deviation of the given integer array
	then returns the results in a float array
*/
float *std_mean_array(int *int_array){

	// Already allocating memory for the result float array
	float *result = (float *) calloc(2, sizeof(float));

	// Variables used in for loops
	int i;
	int curr_value;

	float mean;		
	float std_deviation;

	// Unless EOA is reached add corresponding number in the array to mean
	// increase i once each time in the mean time so we get the length of the array
	for(i = 0, mean = 0; (curr_value = int_array[i]) != EOA; i++) mean += curr_value;

	// To get the mean divide it to length of the array
	mean /= i;

	// Getting the needed sum for the standard deviation
	// length is known already but to sum every integer same i process is done again
	for(i = 0, std_deviation = 0; (curr_value = int_array[i]) != EOA; i++) std_deviation += (curr_value - mean) * (curr_value - mean);

	// Getting standard deviation
	std_deviation = sqrt(std_deviation / (i - 1));

	// Putting results
	result[0] = mean;
	result[1] = std_deviation;

	return result;
}

/*
	min_max_array function scans every integer in given integer array
	then returns found max and min values within a float array
*/
float *min_max_array(int *int_array){

	// Already allocating memory for the result float array
	float *result = (float *) calloc(2, sizeof(float));

	// To have a reference value min and max are set to first integer
	float max = int_array[0];
	float min = int_array[0];

	int i;
	int curr_value;

	// Starting from the second integer check every integer
	// if they are lesser/greater than min/max 
	// if so set new min/max
	for(i = 1; (curr_value = int_array[i]) != -1; i++){

		if(curr_value < min) min = curr_value;
		else if(curr_value > max) max = curr_value;
	}

	// Set result values
	result[0] = min;
	result[1] = max;

	return result;
}

/*
	stats_array function gets the results from std_mean_array and min_max_array then combines them
	and returns the result
*/
float *stats_array(int *int_array){

	float *result = (float *) calloc(4, sizeof(float));

	float *min_max;
	float *std_mean;

	std_mean = std_mean_array(int_array);
	min_max = min_max_array(int_array);

	result[0] = min_max[0];
	result[1] = min_max[1];
	result[2] = std_mean[0];
	result[3] = std_mean[1];

	return result;
}


/*-------------------------Calculating Linkedlist Statistics-------------------------*/

/*
	std_mean_linkedlist function does exact same thing what std_mean_array function does
	but std_mean_linkedlist does it for given linked list head
*/
float *std_mean_linkedlist(dataInteger *head){

	// Allocationg memory for result float array
	float *result = (float *) calloc(2, sizeof(float));

	float mean = 0;
	float std_deviation = 0;

	// Getting head adress to else variable to use freely
	dataInteger *curr_data_integer = head;

	// Length of linked list
	int len = 0;

	// Unless end of linked list is reached
	// add node's value to the mean which will be sum of the integers at the end of the while loop
	// get to next node and increase len by one
	while(curr_data_integer != NULL){
		mean += curr_data_integer->value;
		curr_data_integer = curr_data_integer->next;
		len++;
	}

	mean /= len;				// Dividing mean by length of linked list
	curr_data_integer = head;	// Resetting current data integer back to start

	// Unless end of linked list is reached
	// calculate the needed sum for the standard deviation calculation
	// get to next node after each addition 
	while(curr_data_integer != NULL){

		std_deviation += (curr_data_integer->value - mean) * (curr_data_integer->value - mean);
		curr_data_integer = curr_data_integer->next;
	}

	// Getting standard deviation
	std_deviation = sqrt(std_deviation / (len - 1));

	// Setting result values
	result[0] = mean;
	result[1] = std_deviation;

	return result;
}

/*
	min_max_linkedlist function does exact same thing what min_max_array function does
	but min_max_linkedlist does it for given linked list head
*/
float *min_max_linkedlist(dataInteger *head){

	// Allocating memory for result array
	float *result = (float *) calloc(2, sizeof(float));

	// Setting min and max values to first value as a reference
	float min = head->value;
	float max = head->value;

	// Using variable other than head variable to freely do the processes
	dataInteger *curr_data_integer = head->next;

	// Unless end of the linked list is reached
	// check every integer if they are lesser/greater than min/max
	// update the min/max if so
	// get to the next node each time
	while(curr_data_integer != NULL){

		if(curr_data_integer->value < min) min = curr_data_integer->value;
		else if(curr_data_integer->value > max) max = curr_data_integer->value;

		curr_data_integer = curr_data_integer->next;
	}

	// Setting result values
	result[0] = min;
	result[1] = max;

	return result;
}

/*
	stats_linkedlist function gets results from std_mean_linkedlist and min_max_linkedlist
	then combines them and returns the result
*/
float *stats_linkedlist(dataInteger *head){

	float *result = (float *) calloc(4, sizeof(float));

	float *min_max;
	float *std_mean;

	min_max = min_max_linkedlist(head);
	std_mean = std_mean_linkedlist(head);

	result[0] = min_max[0];
	result[1] = min_max[1];
	result[2] = std_mean[0];
	result[3] = std_mean[1];

	return result;
}


/*-------------------------Main Part 2 Function------------------------*/

/*
	part1 function basically uses functions above and executes the mentioned processes in the homework	
*/
void part1(){

	// File that is worked on
	char *filename = "list.txt";

	// Variable stores start times
	clock_t start_t;

	// Variables for the array and the linked list
	int *int_array;
	dataInteger *head;

	// Variables for their stats
	float *linkedlist_stats;
	float *array_stats;

	// From this point functions above are executed and client is informed accordingly about process times
	printf("----- Executing Part 1 -----\n");

	start_t = clock();
	int_array = read_array(filename);
	printf("Dynamic integer array is read in %lf seconds\n\n", ((double) clock() - start_t) / CLOCKS_PER_SEC);

	start_t = clock();
	head = read_linkedlist(filename);
	printf("Integer linked list is read in %lf seconds\n\n", ((double) clock() - start_t) / CLOCKS_PER_SEC);

	start_t = clock();
	array_stats = stats_array(int_array);
	printf("Stats for integer array are calculated in %lf seconds\n\n", ((double) clock() - start_t) / CLOCKS_PER_SEC);

	start_t = clock();
	linkedlist_stats = stats_linkedlist(head);
	printf("Stats for integer linked list are calculated in %lf seconds\n\n", ((double) clock() - start_t) / CLOCKS_PER_SEC);

	// Printing stats
	printf("\nStat Results:\n");
	printf("\tInteger array stats:\t\tMin:%f\tMax:%f\tMean:%f\tStandard Deviation:%f\n", array_stats[0], array_stats[1], 
																					   	 array_stats[2], array_stats[3]);
	printf("\tInteger linked list stats:\tMin:%f\tMax:%f\tMean:%f\tStandard Deviation:%f\n", linkedlist_stats[0], linkedlist_stats[1], 
																							 linkedlist_stats[2], linkedlist_stats[3]);
	printf("\n");
}

/*
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**#																		 #
**#								  PART 1								 #
**#																		 #
**########################################################################
*/


/*
**########################################################################
**#																		 #
**#								  PART 2								 #
**#																		 #
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


/*-------------------------------Generating Array and Linkedlist of Random Integers-------------------------------*/

/*
	get_random_int function returns a random integer within given range
*/
int get_random_int(int lower, int upper){
	return (lower + rand() % (upper - lower));
}

/*
	generate_random_linkedlist_array function generates both a linked list and an array
	these array and linked list are given 'len' long and they consist of integers that are within given range
	since integer array and linked list can't be returned together, array is returned normally
	and linked list's address is put to given linked list head/banner address variable 
*/
int *generate_random_linkedlist_array(int lower, int upper, int len, dataInteger **return_head){

	// Get first random integer, because we need to create a linked list head
	// which needs to have a value
	int random_int = get_random_int(lower, upper);
	
	// Create linked list head and set current node to it
	dataInteger *head = create_data_integer(random_int);
	dataInteger *curr_data_integer = head;

	// Allocate memory for the integer array then set it's firts element to random_int
	int *random_int_array = (int *) calloc(len+1, sizeof(int));	// len+1 not len because EOA has to be added last
	random_int_array[0] = random_int;

	int i;

	// Starting from second entries of array and linked list, until len number of entries are set
	for(i = 1; i < len; i++){

		random_int = get_random_int(lower, upper);	// Get a random integer within lower-upper range 
		
		// Add new node with random_int as value to the linked list
		curr_data_integer = append_data_integer_entry(curr_data_integer, random_int); 

		// Since it doesn't matter which entry is different between linked list or array
		// only the array entries are changed, an integer's change is selected randomly and the chance of change is 3/10
		// change magnitude is 1, but it has the same chance of being in negative or positive direction
		if(get_random_int(0, 10) < 3) random_int_array[i] = random_int + 2 * get_random_int(0, 1) - 1;
		else random_int_array[i] = random_int;
	}

	// Since there's a chance that no integer is changed, one randomly selected integer is changed
	// change magnitude is 2, that is because if it was 1 there would be a chance having -1 change before and +1 change now (or vice versa)
	// so it may cause array to be exact same with linked list
	random_int_array[get_random_int(0,len)] += 4 * get_random_int(0, 1) - 2;
	random_int_array[i] = EOA; // Setting end of array indicator

	*return_head = head;
	return random_int_array;
}


/*------------------------------Identifying Differences Between Array and Linkedlist------------------------------*/

/*
	differentIntegers struct holds the different integers that are at the same index but in linked list and array
*/
typedef struct different_integers
{
	int value1;
	int value2;
} differentIntegers;

/*
	find_different_integers function checks if the integers that are at the same index in given array and linked list
	are equal if not stores them in an array as differentIntegers struct then returns the array
*/
differentIntegers *find_different_integers(int *int_array, dataInteger *ll_head){

	dataInteger *curr_data_integer = ll_head;

	int i;						
	int curr_array_value;		// Variable that stores corresponding array integers in loops
	int number_of_differences;

	// Unless end is reached checks linked list and array integers if they are different
	// if so increases number_of_differences by one
	for(i = 0, number_of_differences = 0; (curr_array_value = int_array[i]) != EOA; i++){


		if(curr_array_value != curr_data_integer->value) number_of_differences++;

		curr_data_integer = curr_data_integer->next;
	}

	// Allocating memory for the result array consisting of struct that stores different numbers
	// plus one for the EOA indicator
	differentIntegers *differences_array = (differentIntegers *) calloc(number_of_differences+1, sizeof(differentIntegers));

	// Resetting 
	curr_data_integer = ll_head;

	// This time when different integers are encountered
	// differences_array result array is updated
	for(i = 0; (curr_array_value = int_array[i]) != EOA; i++){

		if(curr_array_value != curr_data_integer->value){

			differences_array->value1 = curr_array_value;
			differences_array->value2 = curr_data_integer->value;
			differences_array++;
		}

		curr_data_integer = curr_data_integer->next;
	}

	// To indicate result array's end it's last struct element values are set to EOA
	differences_array->value1 = EOA;
	differences_array->value2 = EOA;

	// Returns array's first element address
	return (differences_array - number_of_differences);
}


/*-------------------------Print Functions-------------------------*/

// This section functions are pretty straight forward
// all really done is printing every value from start of given list (linked or array) to end

void print_1D_array(int *int_array){

	int i;

	printf("Dynamic array:\t{");

	for(i = 0; int_array[i+1] != EOA; i++){
		printf("%d ", int_array[i]);
	}

	printf("%d}\n", int_array[i]);
}

void print_linkedlist(dataInteger *head){

	dataInteger *curr_data_integer = head;

	printf("Linked list:\t{");

	while(curr_data_integer->next != NULL){
		printf("%d ", curr_data_integer->value);
		curr_data_integer = curr_data_integer->next;
	}

	printf("%d}\n", curr_data_integer->value);
}

void print_differences(differentIntegers *differences_array){

	int i;

	printf("\nDifferent Integers:\n");

	for (i = 0; differences_array[i].value1 != -1; ++i)
	{
		printf("\t%d\t%d\n", differences_array[i].value1, differences_array[i].value2);
	}
}


/*-------------------------Main Part 2 Function------------------------*/

/*
	part2 function does mentioned things in the homework using functions above
*/
void part2(){

	// List (linked or array) variables
	int *int_array;
	dataInteger *head;
	differentIntegers *differences_array;

	// Getting lists (linked or array)
	int_array = generate_random_linkedlist_array(2, 100, 10, &head);
	differences_array = find_different_integers(int_array, head);

	// Informing the client from here on
	printf("\n----- Executing Part 2 -----\n");

	print_1D_array(int_array);	
	print_linkedlist(head);
	print_differences(differences_array);
}

/*
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**#																		 #
**#								  PART 2								 #
**#																		 #
**########################################################################
*/


/*-------------------------Main-------------------------*/
int main(){

	srand(time(NULL));

	part1();
	part2();

	return 0;
}