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


void seperate_data_elements(char *data_line,
							char data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN])
{
	int i = 0;
	char *token;
	const char splitter[] = ",";

	token = strtok(data_line, splitter);

	while(token != NULL){
		strcpy(data_array[i], token);
		i++;
		token = strtok(NULL, splitter);
	}
}


int add_if_missing(char types[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
				   char *type)
{
	int i = 0;
	int found = 0;

	while(!found && (strcmp(types[i], EOA) != 0)){
		if(strcmp(types[i], type) == 0){ 
			found = 1;
			break;
		}
		i++;
	}

	if(!found){
		strcpy(types[i], type);
		strcpy(types[i+1], EOA);
	}
	return i;
}


int get_game_name_index(char  game_name[MAX_GAME_NAME_LEN],
	   				    char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN])
{
	int i;
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

void add_game_datas(char   data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN],
				   	char   game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],
				   	char   genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
				   	char   platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
				   	float  game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT],
				   	int    index)
{
	char  *char_pointer;

	strcpy(game_names[index], data_array[0]);
	game_datas[index][0] = add_if_missing(genre_names, data_array[1]);
	game_datas[index][1] = add_if_missing(platform_names, data_array[2]);
	game_datas[index][2] = strtod(data_array[3], &char_pointer);
	game_datas[index][3] = strtod(data_array[4], &char_pointer);
	game_datas[index][4] = strtod(data_array[5], &char_pointer);


	if(strcmp(data_array[6], "not_available") == 0){
		game_datas[index][5] = NOT_AVAILABLE;
	}
	else{
		game_datas[index][5] = strtod(data_array[6], &char_pointer);
	}


	game_datas[index][6] = strtod(data_array[7], &char_pointer);

	/*printf("%s %f %f %f %f %f %f %f\n", game_names[index],
										game_datas[index][0],
										game_datas[index][1],
										game_datas[index][2],
										game_datas[index][3],
										game_datas[index][4],
										game_datas[index][5],
										game_datas[index][6]);*/
}


void get_data(char  *file_path,
			  char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],
			  char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
			  char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
			  float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{
	FILE *data_file;

	char data_line[MAX_DATA_LINE_LEN];
	char data_array[DATA_LINE_DATA_COUNT][MAX_GAME_NAME_LEN];

	int i = 0;


	data_file = fopen(file_path, "r");

	fgets(data_line, MAX_DATA_LINE_LEN, data_file);

	while(fgets(data_line, MAX_DATA_LINE_LEN, data_file)){
		seperate_data_elements(data_line, data_array);
		add_game_datas(data_array, game_names, genre_names, platform_names, game_datas, i);
		i++;
	}

	fclose(data_file);
}


void list_genres_platform(char genres_platform[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN])
{
	int i = 0;

	while(strcmp(genres_platform[i], EOA) != 0){
		printf("%s\n", genres_platform[i]);
		i++;
	}
}


void list_by_year(char   game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],
				  float  game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{
	int i;
	int year;
	int until_since;
	int found = 0;


	while(!found){
		printf("Enter a year:\n");
		scanf("%d", &year);

		printf("Until (0) or since (1) %d\n", year);
		scanf("%d", &until_since);
		printf("\n");

		for (i = 0; i < MAX_GAME_COUNT; ++i){
			if(!until_since && (game_datas[i][2] < year)){
				printf("%s\n", game_names[i]);
				found = 1;
			}
			else if(until_since && (game_datas[i][2] >= year)){
				printf("%s\n", game_names[i]);
				found = 1;
			}
		}

		if(!found){
			printf("TRY AGAIN\n");
		}
	}
}


void show_game_data(char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],
					char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
					char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
					float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{
	int index = -1;
	int array_index;
	char wanted_game_name[MAX_GAME_NAME_LEN];

	while(index == -1){
		printf("\nPlease enter the name of the game:\n");
		scanf("%s", wanted_game_name);

		index = get_game_name_index(wanted_game_name, game_names);

		if(index == -1){
			printf("Game's not found please try again.\n");
		}
	}


	printf("\n");
	printf("Name:\t\t\t%s\n", game_names[index]);

	array_index = game_datas[index][0];
	printf("Genre:\t\t\t%s\n", genre_names[array_index]);

	array_index = game_datas[index][1];
	printf("Platform:\t\t%s\n", platform_names[array_index]);

	printf("Year:\t\t\t%f\n", game_datas[index][2]);
	printf("Sales in NA:\t\t%f\n", game_datas[index][3]);
	printf("Sales in EU:\t\t%f\n", game_datas[index][4]);

	if(game_datas[index][5] == NOT_AVAILABLE){
		printf("Total Sales:\t\tNot Available\n");
	}
	else{
		printf("Total Sales:\t\t%f\n", game_datas[index][5]);
	}

	printf("User Score:\t\t%f\n", game_datas[index][6]);
}


void get_user_score_avg(float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{
	int i;
	float sum;

	for(i = 0; i < MAX_GAME_COUNT; ++i){
		sum += game_datas[i][6];
	}

	printf("Average: %f\n", sum/MAX_GAME_COUNT);
}


void compare_sales(char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN],
				   float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT])
{
	int index = -1;
	char wanted_game_name[MAX_GAME_NAME_LEN];

	while(index == -1){
		printf("Please enter the name of the game:\n");
		scanf("%s", wanted_game_name);

		index = get_game_name_index(wanted_game_name, game_names);
		
		if(index == -1){
			printf("Game's not found please try again.\n");
		}
	}

	printf("\n");
	
	if(game_datas[index][3] < game_datas[index][4]){
		printf("This game was more popular in Europe\n");
	}
	else{
		printf("This game was more popular in North America\n");
	}
}


void print_frequency(char  platform_genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN],
					 float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT],
					 int   platform_genre)
{
	int i;
	int name_i;
	int platform_genre_name_index;
	int sum;
	char curr_name[MAX_GAME_NAME_LEN];

	for(sum = 0, name_i = 0; name_i < 12 - platform_genre * 2; ++name_i){
		strcpy(curr_name, platform_genre_names[name_i]);

		for(i = 0; i < MAX_GAME_COUNT; ++i){
			platform_genre_name_index = game_datas[i][0+platform_genre];
		
			if(strcmp(platform_genre_names[platform_genre_name_index], curr_name) == 0){
				sum++;
			}
		}
		printf("%s\t\t%d\n", curr_name, sum);
		sum = 0;
	}


}


void menu(){
	char  game_names[MAX_GAME_COUNT][MAX_GAME_NAME_LEN];
	char  genre_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN] = {EOA};
	char  platform_names[MAX_GENRE_PLATFORM_COUNT][MAX_GENRE_PLATFORM_LEN] = {EOA};
	float game_datas[MAX_GAME_COUNT][GAME_DATA_COUNT];

	get_data("Video_Games.txt", game_names, genre_names, platform_names, game_datas);

	int user_command;

	while(user_command != 8){
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
		scanf("%d", &user_command);
		printf("\n");

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


void main(){
	menu();
}