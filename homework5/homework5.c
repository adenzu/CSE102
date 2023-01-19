#include <stdio.h>


double f_func(int x){
	double result;

	result = x*x*(x-1)+2;

	return result;
}

double g_func(double f(int x), int a){
	double result;

	result = f(a);
	result *= result;

	return result;
}

void read_magic_numbers(char buffer_magic[10], char buffer_news[500]){
	int result = 0; //Declaring and assigning result variable
	FILE *file; 

	char read_char; //The variable that stores read characters
	int read_int; //The variable that stores read numbers

	file = fopen(buffer_magic, "r"); //Opening the file in read mode
	read_char = fgetc(file); //Reading the firs character

	while(read_char != EOF){ //Untill end of file is reached

		printf("%c", read_char);						//Print the read character so that user can see the text

		if(read_char == '#'){ 							//If read character is # thus the next one will be the magic number
			read_char = fgetc(file); 					//Get magic number in char
			read_int = read_char - '0'; 				//Getting it as int
			printf("%d", read_int); 					//Printing yet again to continue to inform user what is read
			result += g_func(f_func, read_int); 		//Calculating the value and adding it to result
		}
		read_char = fgetc(file); //Reading next character
	}

	printf("\n");

	//Printing specially for the different news
	switch(buffer_magic[5]){
		case '1':
			printf("number of tests performed");
			break;
		case '2':
			printf("number of sick people");
			break;
		case '3':
			printf("number of deaths");
			break;
		default:
			printf("expected number of sick people");
	}
	
	//Printing the result 
	printf(" = %d\n", result);

}

void append_file(char *file_path, char c){
	FILE *file; //File variable

	file = fopen(file_path, "a"); //Opening file in append mode

	fputc(c, file); //Writing the new read new number to the end of the file
}

void read_news(char buffer[500], char file_path[10], int isOnlyTitle){
	FILE *file; //The variable that is going to be used to store the file

	file = fopen(file_path, "r"); //Opening file


	//Checking for if it's only the title or the whole text to print
	if(isOnlyTitle == 1){ 
		fgets(buffer, 500, file); //Reading only one line since it's only the title 
		printf("%s", buffer); //Printing read title
	}
	else if(isOnlyTitle == 0){
		printf("\n");
		while (fgets(buffer, 500, file) != NULL)	printf("%s", buffer); //Untill all the lines are read, read lines and print them
		printf("\n\n");
	}


}

void menu(){
	int i; //Variable for for loops
	int max_line_size = 500; //Maximum file line size

	char curr_file_path[15] = "news/ .txt"; //News file path variable to open news files
	FILE *curr_file; //News file
	
	char read_news_file_path[25] = "readed_news_id.txt"; //Path of the file that store read news
	FILE *read_news_file; //Read news stored file

	char read_line[max_line_size]; //String variable to store read lines

	char command = '0'; //Variable to store user menu commands
	char reread; //Variable to store user's choice of reareading a new
	char decrypt; //Variable to store user's choice of news to decrypt

	int read_newsa[4] = {0, 0, 0, 0}; //The array that stores read status of news
	int read_new_index_i; //Variable to reach array read_news' elements 
	char read_new_index; //Variable to reach array read_news' elements but in char type



	//Opening the file that stores already read news
	//to update read_newsa array
	read_news_file = fopen(read_news_file_path, "r");
	read_new_index = fgetc(read_news_file);

	while(read_new_index != EOF){ //Till end of file is reached

		if (read_new_index != '\n') //If the read char is not new line character
		{
			//Turning the read char to int by subtracting '1' then to show it is read 
			//changing corresponding element to 1
			read_newsa[read_new_index - '1'] = 1; 
		}
	
		read_new_index = fgetc(read_news_file); //Getting the next char
	}

	//Printing
	printf("******Daily Press******\n");
	printf("\nToday's news are listed for you:\n\n");


	while(command != 'n'){ //Untill user enters 'n' as command

		for (i = 1; i < 5; ++i)
		{	
			//Changing the space char in curr_file_path to related news number
			//then printing corresponding title with read_news function
			curr_file_path[5] = '0' + i;
			printf("Title of %d. news: ", i);
			read_news(read_line, curr_file_path, 1);
		}

		//Informing the user of the options they have
		printf("\nWhat do you want to do?\n");
		printf("a.Read a new\n");
		printf("b.List the read news\n");
		printf("c.Get decrypted information from the news\n");

		scanf(" %c", &command); //Getting the command

		switch(command){
			case 'a': //If user chose to read a new

				//Getting desired new
				printf("\nWhich new do you want to read?\n");
				scanf(" %c", &read_new_index);

				read_new_index_i = read_new_index - '1'; //Getting the integer value that corresponds to the array

				if(!read_newsa[read_new_index_i]){ 						//If chosen new is not read
					read_newsa[read_new_index_i] = 1; 					//Set it to 1 to show it's read now
					append_file(read_news_file_path, read_new_index); 	//Saving read new to file
					curr_file_path[5] = read_new_index; 				//Updating curr_file_path for the chosen new
					read_news(read_line, curr_file_path, 0); 			//Reading new with read_news
				}
				else{ //Else if chosen new is already read
					//Making sure the user wants to read this new again
					printf("This new is read. Do you want to read again? y/n\n"); 
					scanf(" %c", &reread);
				}

				switch(reread){

					//If user wants to read again
					case 'y':

						curr_file_path[5] = read_new_index; 		//Updating curr_file_path for the desired new
						read_news(read_line, curr_file_path, 0); 	//Reading the news with read_news function
						break;
				}
				break;

			case 'b': //If user chose to see list of read news


				printf("\nRead news are listed below:\n"); //Printing
				for (i = 0; i < 4; ++i)
				{
					//If corresponding element in the array is 1 (true)
					if(read_newsa[i]){ 
						printf("%d. new is read\n", i+1); //Print read news number
					}
				}
				break;

			default: //Default for command 'c'

				//Getting the desired news number
				printf("Which news would you like to decrypt?:\n");
				scanf(" %c", &decrypt); //Storing it ing decrypt variable

				curr_file_path[5] = decrypt; 					//Updating the curr_file_path for chosen news
				read_magic_numbers(curr_file_path, read_line); 	//Decrypting the chosen news

				printf("\n");
		}

		//Checking if user wants to continue on
		printf("Do you want to continue? y/n\n");
		scanf(" %c", &command);
	}
	printf("Goodbye!\n"); //A final heartwarming message to user to make their day :)
}

void main(){
	menu(); //Calling the menu function
}