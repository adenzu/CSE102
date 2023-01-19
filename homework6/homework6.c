#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>

//puzzle map define variables' names are pretty self explanatory
#define PUZZLE_MAP_WIDTH 20
#define PUZZLE_MAP_HEIGHT 20
#define PUZZLE_WORD_COUNT 10
#define PUZZLE_WORD_LENGTH 20

//total word count that is stored in words.txt file
#define TOTAL_WORD_COUNT 100

/*
since the puzzle map is a 20x20 array and the column indexes (id's in this case)
are stored as strings two digit numbers like 11 and 17 are going to be 2 characters long
*/
#define COLUMN_ID_LENGTH 2


/*
random_range function returns an integer value between given lower limit and upper limit

lower: parameter for the given lower limit argument 
upper: parameter for the given upper limit argument 

random: the result integer value
range: the difference between upper limit and lower limit

when assigning random, gets a random integer with rand() then gets mod range of it
so the number gets in between range then adds lower limit to reach desired range
*/
int random_range(int lower, int upper){
	int random;
	int range;

	range = upper - lower;
	random = lower + rand() % range;

	return random;
}


/*
get_random_char function gets a random integer between lower case character ascii codes
then turns it into char 

random_char: the random char variable that is returned 
*/
char get_random_char(){

	char random_char;
	random_char = random_range(97, 123);
	return random_char;
}


/*
randomise_puzzle_board fills the spaces in the puzzle map with random characters

puzzle_map: parameter that is used to access to puzzle_map

x: corresponding variable for puzzle_map array's columns
y: corresponding variable for puzzle_map array's rows
random_char: variable that stores randomly chosen char value to fill the space 
*/
void randomise_puzzle_board(char puzzle_map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH]){
	
	int x, y;
	char random_char;

	for(y = 0; y < PUZZLE_MAP_HEIGHT; ++y){
		for(x = 0; x < PUZZLE_MAP_WIDTH; ++x){

			//'0' is treated as blank/empty value
			//see that the first values assigned to puzzle_map array elements is '0' in fill_map function
			if(puzzle_map[y][x] == '0'){
				random_char = get_random_char();
				puzzle_map[y][x] = random_char;
			}
		}
	}
}


/*
print_map function is used to prettily print puzzle_map

puzzle_map: parameter to access to puzzle_map
rows: parameter to access to row value array

x: the variable that stores corresponding column index of the element
y: the variable that stores corresponding row index of the element 
*/
void print_map(char puzzle_map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH], char *rows){
	int x;
	int y;

	//not getting column ids like getting row ids via parameter
	//because printing it as a whole string like this is a lot easier
	printf("   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19\n");
	for(y = 0; y < PUZZLE_MAP_HEIGHT; ++y){

		/*
		printing corresponding row value
		printing characters for all column ids
		then printing new line
		*/
		printf("%c  ", rows[y]);
		for(x = 0; x < PUZZLE_MAP_WIDTH; ++x) printf("%c  ", puzzle_map[y][x]);
		printf("\n");
	}
}


/*
print_words function is used for printing the words that are in the puzzle

puzzle_words: parameter to access puzzle map

i: variable that keeps corresponding word index 
*/
void print_words(char puzzle_words[PUZZLE_WORD_COUNT][PUZZLE_WORD_LENGTH]){
	int i;

	printf("\nWords:\n");
	for(i = 0; i < PUZZLE_WORD_COUNT; ++i) printf("%s\n", puzzle_words[i]);
}


/*
put_words function is used for inserting randomly chosen words' characters to puzzle_map array

puzzle_map: parameter to access to puzzle map array
words: parameter to access the randomly chosen 10 words array
rows: parameter to access array of the row values of the puzzle map
columns: parameter to access array of the column values of the puzzle map

x_direction: variable that keeps which horizontal direction word is going to be written in
y_direction: variable that keeps which vertical direction word is going to be written in
x_range: array of 2 integers which are the lower and upper column value limits for word that shows where it can start getting written
y_range: array of 2 integers which are the lower and upper row value limits for word that shows where it can start getting written
x_range_index: this variable keeps the value that determines which integer will be changed in x_range array
y_range_index: this variable keeps the value that determines which integer will be changed in y_range array
curr_word: variable that stores the current word in the word comparison loop 
curr_letter: variable that stores the current letter in the character comparison and character assigning loops
word_len: length of the curr_word string
letter_i: variable to use in the comparison loop of letters/characters from the curr_word and corresponding word from words array
row_i: variable that stores row value to use in character/letter assigning to array elements in char assign for loop
column_i: variable that stores column value to use in character/letter assigning to array elements in char assign for loop
temp_row_i: temporary variable that basically does the same work what row_i does but instead this variable is used in char comparison not assignment
temp_column_i: temporary variable that basically does the same work what column_i does but instead this variable is used in char comparison not assignment
is_word_written: this is changed when the word is able to be written or not and checked afterwards if the word is writable so written
i: how many words from words string array is written
*/
void put_words(char puzzle_map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH], char words[PUZZLE_WORD_COUNT][PUZZLE_WORD_LENGTH], char rows[PUZZLE_MAP_HEIGHT], char columns[PUZZLE_MAP_WIDTH][COLUMN_ID_LENGTH]){

	int x_direction;
	int y_direction;

	int x_range[2];
	int y_range[2];

	int x_range_index;
	int y_range_index;

	char *curr_word;
	char curr_letter;
	int word_len;

	int letter_i;

	int row_i;
	int column_i;

	int temp_row_i;
	int temp_column_i;

	int is_word_written;

	int i = 0;

	//unless all 10 words are written
	while(i < PUZZLE_WORD_COUNT){

		//assuming word is writable
		is_word_written = 1;

		//resetting directions to 0 so that they can get new random values
		x_direction = 0;
		y_direction = 0;
		
		//resetting ranges
		x_range[0] = 0;
		x_range[1] = PUZZLE_MAP_WIDTH;
		y_range[0] = 0;
		y_range[1] = PUZZLE_MAP_HEIGHT;

		//setting new current word
		curr_word = words[i];
		word_len = strlen(curr_word);

		//unless both directions are not 0
		while(x_direction == 0 && y_direction == 0){
			x_direction = rand() % 3 - 1;
			y_direction = rand() % 3 - 1;
		}
		
		//if direction is 0 then there's no need for extra limitations for start of the word
		//if else then determine the start index limitations by the direction and length of the current word
		//this is for both x (horizontal) and y (vertical) directions of puzzle map array
		if(x_direction != 0){
			x_range_index = (1 + x_direction) / 2;
			x_range[x_range_index] -= x_direction * word_len + 1 - x_range_index;
		}

		if(y_direction != 0){
			y_range_index = (1 + y_direction) / 2;
			y_range[y_range_index] -= y_direction * word_len + 1 - y_range_index;
		}

		//after determining the x and y range then set random start row and column index
		row_i = random_range(y_range[0], y_range[1]);
		column_i = random_range(x_range[0], x_range[1]);

		//again assigning the values to temp variables to use them afterwards in comparison loops
		temp_row_i = row_i;
		temp_column_i = column_i;

		/*
		comparing corresponding characters in puzzle_map array if they match or the checked character is blank
		blank is '0' in this case
		if the conditions are not met then set is_word_written to 0 so that we know the word is not writable
		thus while loop go on again untill it finds right spot for the word
		*/
		for(letter_i = 0; letter_i < word_len; ++letter_i){

			curr_letter = curr_word[letter_i];

			if(puzzle_map[temp_row_i][temp_column_i] != '0' && puzzle_map[temp_row_i][temp_column_i] != curr_letter){
				is_word_written = 0;
				letter_i = word_len;
			}
			temp_row_i += y_direction;
			temp_column_i += x_direction;
		}

		/*
		if word is writable the proceed to assigning corresponding elements of puzzle_map array 
		*/
		if(is_word_written){
			for(letter_i = 0; letter_i < word_len; ++letter_i){

				curr_letter = curr_word[letter_i];				
				puzzle_map[row_i][column_i] = curr_letter;
				row_i += y_direction;
				column_i += x_direction;
			}

			//one word is written so add 1 to i
			++i;
		}
	}
}


/*
read_words function is used for getting/reading the words from words.txt file

filename: parameter that passes char pointer name of the word containing file
words: the words array that will store the read words

i: corresponding index value of the assigned element of the words array
file: FILE variable for words.txt file
word: the read word from the words.txt file
last_letter_index: actually the length of the word
*/
void read_words(char *filename, char words[TOTAL_WORD_COUNT][PUZZLE_WORD_LENGTH]){

	int i = 0;
	int last_letter_index;

	FILE *file;
	file = fopen(filename, "r");
	
	char word[PUZZLE_WORD_LENGTH];

	//untill the last line is read
	while(fgets(word, PUZZLE_WORD_LENGTH, file)){

		/*
		if read text's last character is new line character 
		then makte the one before it '\0' character so new line 
		character is not a part of the word anymore
		*/
		last_letter_index = strlen(word);
		if(word[last_letter_index-1] == '\n'){
			word[last_letter_index-2] = '\0';
		}
		
		//assign word to words array
		strcpy(words[i], word);
		i++;
	}
}


/*
choose_random_words function chooses random 10 words within all of the words that are read from the words.txt file

words: all 100 words that are read
chosen_words: the array that is going to be filled with 10 random words

chosen_word_count: integer variable to track how many words are chosen, keep going until it's 10
random_word_index: variable that stores a random integer value to get a random word from the words array using this variable
is_already_chosen: variable that stores the state of the word being in chosen_words array already
i: integer value to use in word comparison for loop
random_word: the random word that is randomlu chosen from the words array
*/
void choose_random_words(char words[TOTAL_WORD_COUNT][PUZZLE_WORD_LENGTH], char chosen_words[PUZZLE_WORD_COUNT][PUZZLE_WORD_LENGTH]){

	int chosen_word_count = 0;
	int random_word_index;

	int is_already_chosen;

	int i;
	
	char *random_word;

	//unless chosen_words array is full
	while(chosen_word_count < PUZZLE_WORD_COUNT){
		//setting is_already_chosen variable to 0 as default assuming current word is not already in chosen_words array
		is_already_chosen = 0;
		
		//randomly choosing a word																																																																																																																																																																																																																																																																																																																																																																																																																																																																									
		random_word_index = random_range(0, TOTAL_WORD_COUNT);
		random_word = words[random_word_index];

		//comparing if it's already chosen
		for(i = 0; i < chosen_word_count; ++i){
			if(strcmp(chosen_words[i], random_word) == 0){
				is_already_chosen = 1;
				i = chosen_word_count;
			}
		}

		//if it's not already chosen put it in chosen_words array
		if(!is_already_chosen){
			strcpy(chosen_words[i], words[random_word_index]);
			chosen_word_count++;
		}
	}
}

/*
fill_map function is used for filling puzzle_map array with '0' characters that are treated as blank characters

puzzle_map: parameter to access to puzzle_map array

x: corresponding column value of element
y: corresponding row value of element
*/
void fill_map(char puzzle_map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH]){
	int x, y;

	for(y = 0; y < PUZZLE_MAP_HEIGHT; ++y){
		for(x = 0; x < PUZZLE_MAP_WIDTH; ++x){
			puzzle_map[y][x] = '0';
		}
	}
}


/*
strupper functions takes source string and assigns it's uppercase version to destination string

destination: char pointer of the string variable that is going to be set to uppercase version of source string
source: char pointer of the string whose uppercase version is desired

i: integer value corresponding to char of the source
source_len: length of the source string
destination_len: length of the destination string
*/
void strupper(char *destination, char *source){
	int i;
	int source_len = strlen(source);
	int destination_len = strlen(destination);

	for(i = 0; (i < source_len) && (i < destination_len); ++i){
		destination[i] = toupper(source[i]);
	}
}


/*
find_word function is used for checkinf if user find the word they wrote in the position they wrote
return 1 or 0 if the word is guessed right or not, the result will be added total found word count later on in menu()

guess: total string that consist of row column and guessed word written by user
puzzle_map: parameter to access to puzzle map
puzzle_words: parameter to access to puzzle words, consists of 10 random words

guess_row: row value of the user's word location guess, will be used in character assignment for loop
guess_column: column value of the user's word location guess, will be used in character assignment for loop
guess_word: the word that user thinks is found
word_len: length of guess_word 
x_direction: horizontal direction value to use in character comparison and assignment loops
y_direction: vertical direction value to use in character comparison and assignment loops
word_found: if the word is found in puzzle_words array or not, 0 as default
found_word_index: corresponding index value of the word in puzzle_words, later will be used for assignment
temp_row: temporary variable of guess_row, will be used in for loop so that base value can be kept in guess_row
temp_column: temporary variable of guess_column, will be used in for loop so that base value can be kept in guess_column
*/
int find_word(char *guess, char puzzle_map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH], char puzzle_words[PUZZLE_WORD_COUNT][PUZZLE_WORD_LENGTH]){
	int i;

	int guess_row;
	int guess_column;
	char guess_word[PUZZLE_WORD_LENGTH];

	sscanf(guess, "%lc %d %s", &guess_row, &guess_column, guess_word);
	guess_row -= 97;

	int word_len = strlen(guess_word);

	int x_direction;
	int y_direction;

	int word_found = 0;
	int found_word_index;

	int temp_row;
	int temp_column;

	/*
	check if user's word guess starts from the guessed location in any direction
	and also check if entered guess word is actually one of words in puzzle_words array
	*/
	for(y_direction = -1; !word_found && (y_direction < 2); ++y_direction){
		for(x_direction = -1; !word_found && (x_direction < 2); ++x_direction){
			
			temp_row = guess_row;
			temp_column = guess_column;

			for(i = 0, word_found = 0; !word_found && (i < PUZZLE_WORD_COUNT); ++i){
				if(strcmp(puzzle_words[i], guess_word) == 0){
					word_found = 1;
					found_word_index = i;
				}			
			}

			if(word_found){
				for(i = 0; word_found && (i < word_len); ++i){
					if(toupper(guess_word[i]) != toupper(puzzle_map[temp_row][temp_column])){
						word_found = 0;
					}
					temp_row += y_direction;
					temp_column += x_direction;
				}
			}		
		}
	}

	/*
	just before exiting the for loops the direction values are added by 1 so 
	subtracting 1 to cancel it out
	*/
	x_direction -= 1;
	y_direction -= 1;

	/*
	if word is found then make corresponding characters uppercase in puzzle_map array
	and make corresponding word in puzzle_words uppercase too
	*/
	if(word_found){
		for(i = 0; i < word_len; ++i){
			puzzle_map[guess_row][guess_column] = toupper(puzzle_map[guess_row][guess_column]);
			guess_row += y_direction;
			guess_column += x_direction;
		}

		strupper(puzzle_words[found_word_index], guess_word);
	}

	return word_found;
}


/*
menu function is basically has everything in itself that changes what the user sees

ROWS: row values for puzzle map, is assigned in main for convenience
COLUMNS: column values for puzzle map, is assigned in main for convenience

words: consists of all 100 words written in words.txt
random_words: consists of 10 randomly selected words from words array 
map: the 2d array where all the random characters and also random words are kept
user_command: basically user command
found_word_count: total count of found words
*/
void menu(char ROWS[PUZZLE_MAP_HEIGHT], char COLUMNS[PUZZLE_MAP_WIDTH][COLUMN_ID_LENGTH]){

	char words[TOTAL_WORD_COUNT][PUZZLE_WORD_LENGTH];
	char random_words[PUZZLE_WORD_COUNT][PUZZLE_WORD_LENGTH];
	char map[PUZZLE_MAP_HEIGHT][PUZZLE_MAP_WIDTH];

	fill_map(map); 									//map is filled with '0's which is treated as blank
	read_words("words.txt", words); 				//100 words are read from words.txt file and put into words array
	choose_random_words(words, random_words); 		//10 words are randomly selected an put into random_words array
	put_words(map, random_words, ROWS, COLUMNS); 	//words in random_words are put into map
	randomise_puzzle_board(map);				 	//the rest of the map is filled with random characters

	char user_command[25];
	int found_word_count = 0;

	/*
	unless user enters 'Exit' to exit or finds all of 10 words
	inform user about the state of the map and the words by printing them
	getting users guess
	adding found_word_count 1 if user guessed right add 0 if else
	*/
	while((strcmp(user_command, "Exit") != 0) && found_word_count < 10){
		print_map(map, ROWS);
		print_words(random_words);
		printf("\nYour guess:\n");
		scanf(" %[^\n]s", user_command);
		found_word_count += find_word(user_command, map, random_words);
	}

	//lastly give the user a warm smile with a box of happiness by celebrating them if they found them all
	if(found_word_count == 10){
		printf("Congratulations! You found them all!\n");
	}
}


//assignments of COLUMNS and ROWS arrays and calling menu function afterwards
void main(){
	srand(time(NULL));
 	
 	char COLUMNS[PUZZLE_MAP_WIDTH][COLUMN_ID_LENGTH] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19"};
	char ROWS[PUZZLE_MAP_HEIGHT] = "abcdefghijklmnopqrst";
	
	menu(ROWS, COLUMNS);
}
