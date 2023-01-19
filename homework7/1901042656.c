#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_GAME_NAME_LEN 100
#define MAX_GENRE_PLATFORM_LEN 20
#define MAX_DATA_LINE_LEN 200
#define GAME_DATA_COUNT 7
#define DATA_LINE_DATA_COUNT 8
#define EOA "END OF ARRAY"
#define NOT_AVAILABLE -1
#define MAX_GAME_COUNT 2000
#define MAX_GENRE_PLATFORM_COUNT 20


/*
this function seperates the data string by ',' to data array
*/
void seperate_data_elements(char *data_line,										 /*given data line 						   */
							char data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN])/*where seperated strings will be added to*/
{
	int i = 0;

	/*
	token is each string that is seperated from data line
	splitter is the separation indicator
	*/
	char *token;
	const char splitter[] = ",";

	/*get firs seperated string*/
	token = strtok(data_line, splitter);

	/*
	untill there's nothing left to seperate
	add string to data_array
	then proceed to seperate again
	*/
	while(token != NULL){
		strcpy(data_array[i], token);
		i++;
		token = strtok(NULL, splitter);
	}
}


/*
this function is used for filling the genre and platform name arrays
but at the same time when a new/not added type of platform or genre is given
it is added to the array and the function returns the index where the name is added
to the array, thus game_datas array can be filled with platform/genre names arrays at
the same time
*/
int add_if_missing(char types[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],/*platform_names or genre_names array*/
				   char *type)													/*given platform or genre name 		 */
{
	int i = 0;
	/*
	found is state of if given name is already added to given array
	*/
	int found = 0;

	/*until EOA is reached (end of array) check if the name exist in the array*/
	while(!found && (strcmp(types[i], EOA) != 0)){
		if(strcmp(types[i], type) == 0){ 
			found = 1;
			break;
		}
		i++;
	}

	/*if name is not found in array then add it to array*/
	if(!found){
		strcpy(types[i], type);	  /*adding the platform/genre name to array*/
		strcpy(types[i+1], EOA);  /*the new EOA is one after			   */
	}
	return i;
}

/*
this function returns the index of the game name respect to game_names array
*/
int get_game_name_index(char  game_name[MAX_GAME_NAME_LEN], 				/*given game name */
	   				    char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN])/*game_names array*/
{
	int i;
	/*
	found is the state of if the game is found in the array or not
	result is the index corresponding to where the name of the game found in the game_names array
	initial value is set to -1 if the name is not found 
	*/
	int found = 0;
	int result = -1;

	for(i = 0; !found && (i < MAX_GAME_COUNT); ++i){
		if(strcmp(game_names[i], game_name) == 0){
			result = i;
			found = 1;
		}
	}

	return result;
}


/*
this function gets data about a game and classes data values by specific arrays
like putting name of the game to game_names etc 
*/
void add_game_datas(char   data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN],				/*all of the data about the game 						   */
				   	char   game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],					/*where the name of the game will be added 				   */
				   	char   genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],	/*where the name of the game's genre will be added 		   */
				   	char   platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],/*where the name of the platform where the game released to*/
				   	float  game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT],						/*the rest of the data of the game that has float values   */
				   	int    index)															/*index of where all these datas will be added in arraus   */
{
	/*
	pointer to the char one after where the converted numerical value is
	but in this process it is not needed but for strtod function
	*/
	char  *char_pointer;


	strcpy(game_names[index], data_array[0]); 							 /*copy name of the game to game_names array at index*/
	game_datas[index][0] = add_if_missing(genre_names, data_array[1]);	 /*set game_datas value at index at 0 to game genre index corresponding to genre_names array 
																		   see add_if_missing function to see how actually it works*/
	game_datas[index][1] = add_if_missing(platform_names, data_array[2]);/*set game_datas value at index at 1 to game platform index corresponding to platform_names array 
																		   see add_if_missing function to see how actually it works*/
	
	/*convert string numerical values to float and add to game_datas at 'index' at corresponding index*/
	game_datas[index][2] = strtod(data_array[3], &char_pointer);		 
	game_datas[index][3] = strtod(data_array[4], &char_pointer);
	game_datas[index][4] = strtod(data_array[5], &char_pointer);

	/*check if 'not_available' is read instead of numerical global sale value*/
	if(strcmp(data_array[6], "not_available") == 0){
		game_datas[index][5] = NOT_AVAILABLE; /*then set to NOT_AVAILABLE*/
	}
	else{
		game_datas[index][5] = strtod(data_array[6], &char_pointer); /*if not convert to float and set corresponding value*/
	}

	/*convert to float value then set value in game_datas at index at 6*/
	game_datas[index][6] = strtod(data_array[7], &char_pointer);

}


/*
this function reads all the data from the file Video_Games.txt and puts them into arrays
*/
void get_data(char  *file_path,														  /*the path of the file that's going to be read 								*/
			  char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],					  /*needed game_names array to store the read game names from the file 			*/
			  char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],	  /*needed genre_names array to store the read genre names from the file 		*/
			  char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN], /*needed platform_names array to store the read platform names from the file  */
			  float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])					  /*needed game_datas array to store the rest of the game data that is in float */
{
	/*the Video_Games.txt file*/
	FILE *data_file;

	/*
	data_line is the read line from the data file
	data_array is a one dimensional string array that stores data_line as splitted anywhere ',' is encountered
	*/
	char data_line[MAX_DATA_LINE_LEN];
	char data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN];

	/*
	i is a counter for lines in the file
	*/
	int i = 0;

	/*open the file*/
	data_file = fopen(file_path, "r");

	/*read the first line which is not actually a data, therefore unneeded*/
	fgets(data_line, MAX_DATA_LINE_LEN, data_file);

	while(fgets(data_line, MAX_DATA_LINE_LEN, data_file)){ 									/*as long as file can be read*/
		seperate_data_elements(data_line, data_array); 										/*seperate data line to strings and store in data_array*/
		add_game_datas(data_array, game_names, genre_names, platform_names, game_datas, i); /*then proceed to classify all the data by name of the game, genre of the name etc.
																							  also the data will be put to arrays by the index i*/
		i++;
	}

	fclose(data_file); /*don't forget to close the data after finishing the process*/
}

/*
lists all the genres or the platforms depending on the user
*/
void list_genres_platform(char genres_platform[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN]) /*array of genre names or platform names again depending on the user*/
{
	int i = 0;

	while(strcmp(genres_platform[i], EOA) != 0){
		printf("%s\n", genres_platform[i]);
		i++;
	}
}


/*
this function lists released games until the given year or since the given year depending on the user's choice
*/
void list_by_year(char   game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN], /*needed game_names array to get game's index*/
				  float  game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])	/*needed to check games's release dates 	 */
{
	/*
	i is used for getting all games's release dates to check in the for loop
	year is the given year by the user
	until_since is the choice made by the user if they want to see until the year or since the year
	found is the state of if any game is found in given interval 
	*/
	int i;
	int year;
	int until_since;
	int found = 0;

	/*unless any game is found succesfully*/
	while(!found){

		/*getting the year*/
		printf("Enter a year:\n");
		scanf("%d", &year);

		/*getting if since or until the year is wanted*/
		printf("Until (0) or since (1) %d\n", year);
		scanf("%d", &until_since);
		printf("\n");

		for (i = 0; i < MAX_GAME_COUNT; ++i){
			if(!until_since && (game_datas[i][2] < year)){ /*if "since the year" is wanted and the current game in game_datas has a release date is lesser than given year*/
				printf("%s\n", game_names[i]);
				found = 1;
			}
			else if(until_since && (game_datas[i][2] >= year)){ /*else if "until the year" is wanted and the current game in game_datas has a release date is greater than given year*/
				printf("%s\n", game_names[i]);
				found = 1;
			}
		}

		if(!found){
			printf("TRY AGAIN\n");
		}
	}
}


/*
this function gets a game name from the user then prints the game's data values
*/
void show_game_data(char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],					/*needed game_names array to find index of the game 					 */
					char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],	/*needed genre_names array to print game's genre 						 */
					char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],	/*needed platform_names array to print which platform is game released to*/
					float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])						/*obviously needed to print other data values of the game 				 */
{
	/*
	index of game name related to game_names array, -1 as initial value due to get_game_name_index base return value if the game is not found
	array_index is the variable that stores the value which leads to game's genre or to which platform it's released to
	wanted_game_name is the name entered by the user 
	*/
	int index = -1;
	int array_index;
	char wanted_game_name[MAX_GAME_NAME_LEN];

	/*unless the game is found succesfully*/
	while(index == -1){
		printf("\nPlease enter the name of the game:\n");
		scanf("%s", wanted_game_name); /*get the name of desired game*/

		index = get_game_name_index(wanted_game_name, game_names); /*find it's index, -1 if not found*/

		if(index == -1){
			printf("Game's not found please try again.\n");
		}
	}

	/*inform the user about the game's data values*/
	printf("\n");
	printf("Name:\t\t\t%s\n", game_names[index]);

	array_index = game_datas[index][0];
	printf("Genre:\t\t\t%s\n", genre_names[array_index]);

	array_index = game_datas[index][1];
	printf("Platform:\t\t%s\n", platform_names[array_index]);

	printf("Year:\t\t\t%f\n", game_datas[index][2]);
	printf("Sales in NA:\t\t%f\n", game_datas[index][3]);
	printf("Sales in EU:\t\t%f\n", game_datas[index][4]);

	/*if game's total sale value is NOT_AVAILABLE, -1 in this case, then print so*/
	if(game_datas[index][5] == NOT_AVAILABLE){
		printf("Total Sales:\t\tNot Available\n");
	}
	else{
		printf("Total Sales:\t\t%f\n", game_datas[index][5]);
	}

	printf("User Score:\t\t%f\n", game_datas[index][6]);
}


/*
this function adds all the user scores the proceeds to divide the result by game count to find average
then prints the final result
*/
void get_user_score_avg(float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{	
	int i;
	float sum;

	for(i = 0; i < MAX_GAME_COUNT; ++i){
		sum += game_datas[i][6];
	}

	printf("Average: %f\n", sum/MAX_GAME_COUNT);
}


/*
this function compares the regional sales of the game and prints where the game is sold more
*/
void compare_sales(char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN], /*game_names array to find corresponding index of the game name*/
				   float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])	/*game_datas array to get sale datas and compare them		   */
{
	/*
	index is where name of the game is stored in game_names array, 
	it's initial value is -1 since get_game_name_index function returns -1 if game name is not found in game_names array
	wanted_game_name is the name user entered
	*/
	int index = -1;
	char wanted_game_name[MAX_GAME_NAME_LEN];

	/*
	as long as the task is not finished succesfully
	*/
	while(index == -1){
		
		printf("Please enter the name of the game:\n");
		scanf("%s", wanted_game_name); /*getting desired game name*/

		index = get_game_name_index(wanted_game_name, game_names); /*finding index of the name in game_names*/
		
		if(index == -1){
			printf("Game's not found please try again.\n");
		}
	}

	printf("\n");
	
	/*3rd data of game is sale amount for North America and 4th is for Europe*/
	if(game_datas[index][3] < game_datas[index][4]){
		printf("This game was more popular in Europe\n");
	}
	else{
		printf("This game was more popular in North America\n");
	}
}


/*
print_frequency function prints what genre is encountered how many times through all the data
or platforms instead of genres depending on which user wants to see

this function is used for printing both genres and platforms since the overall process is nearly the same 
*/
void print_frequency(char  platform_genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],  /*the platform or the genre names array depending on the user 				   */
					 float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT], 							/*game datas array to count what platforms/genres encountered how many times   */
					 int   platform_genre)															/*indicates if platforms or genres will be printed, 0 for genre 1 for platform */
{
	/*
	i is used as game_datas array index   
	name_i is used as platform_genre_names index 
	platform_genre_name_index is the value of the current game in the game_datas that 
	corresponds to the index of the platform/genre name in platform_genre_names array
	sum is how many times the given platform/genre name is encountered
	curr_name is the current platform/genre name that is counted
	*/
	int i;
	int name_i;
	int platform_genre_name_index;
	int sum;
	char curr_name[MAX_GAME_NAME_LEN];

	/*
	resetting sum and name_i to 0
	loop will continue on as long as name_i is less than 12 - platform_genre * 2
	now there are 12 different game genres but 10 different platforms so the formula above is needed for handling the for loop
	*/
	for(sum = 0, name_i = 0; name_i < 12 - platform_genre * 2; ++name_i){

		/*
		copying corresponding name in the array to curr_name so we won't have to call it from game_datas array again
		*/
		strcpy(curr_name, platform_genre_names[name_i]);

		/*till every platform/genre game data is checked*/
		for(i = 0; i < MAX_GAME_COUNT; ++i){

			/*needed index to get platform/genre name*/
			platform_genre_name_index = game_datas[i][0+platform_genre];
		
			/*if curr_name and platform/genre name of current game is identical*/
			if(strcmp(platform_genre_names[platform_genre_name_index], curr_name) == 0){
				sum++; /*then add 1 to sum*/
			}
		}

		/*after checking every game print the result*/
		printf("%s\t\t%d\n", curr_name, sum);
		sum = 0;
	}


}


/*
Basically everything that client sees or doesn't is done in menu function, both user interface and the other processes.
*/
void menu(){

	/*
	game_names array stores the names of the games that are read from file Video_Games.txt
	genre_names array stores the genres that are read same way
	platform_names array stores rhe platform names
	game_datas is a float array that basically stores all the numerical datas about the games 
	in the same order which game_names array stores name of games 
	*/
	char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN];
	char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN] = {EOA};
	char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN] = {EOA};
	float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT];

	/*getting the data from file Video_Games.txt to mentioned arrays*/
	get_data("Video_Games.txt", game_names, genre_names, platform_names, game_datas);

	/*variable that keeps entered user command*/
	int user_command;

	/*runs until user enters 8 which is exit command number*/
	while(user_command != 8){
		/*informing the user what are their options*/
		printf("0: List of Genres\n");
		printf("1: List of the Platforms\n");
		printf("2: List of the Games through the years\n");
		printf("3: All informations of a Single Game\n");
		printf("4: Average of the User Scores\n");
		printf("5: Geographical Information of a Single Game\n");
		printf("6: Frequence of the Genres\n");
		printf("7: Frequence of the Platforms\n");
		printf("8: Exit\n");
		printf("\nPlease select an operation: ");

		/*getting the command*/
		scanf("%d", &user_command);
		printf("\n");

		/*
		handling the entered command
		initiates the function which does what the user wanted
		*/
		switch(user_command){
			case 0:
				list_genres_platform(genre_names);
				break;
			case 1:
				list_genres_platform(platform_names);
				break;
			case 2:
				list_by_year(game_names, game_datas);
				break;
			case 3:
				show_game_data(game_names, genre_names, platform_names, game_datas);
				break;
			case 4:
				get_user_score_avg(game_datas);
				break;
			case 5:
				compare_sales(game_names, game_datas);
				break;
			case 6:
				print_frequency(genre_names, game_datas, 0);
				break;
			case 7:
				print_frequency(platform_names, game_datas, 1);
				break;
			case 8:
				break;
			default:
				printf("\nTRY AGAIN\n");
		}
		printf("\n");


	}
}


/*
Actually everything is done in menu function. Menu function is called in main.
*/
void main(){
	menu();
}