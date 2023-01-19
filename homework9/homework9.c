#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/*----------Part 1----------*/

/*
numPathsHome function calculates how many optimal paths there are from (street, avenue) to (1,1) then returns it
*/
int numPathsHome(int street,	// Street number
				 int avenue){	// Avenue number
	
	int result = 0; // The number of optimal paths from (street, avenue) to (1, 1)

	if(street-1){	// If street 1 is not reached, statement is true for any number greater than 1
		result += numPathsHome(street-1, avenue);	// Then take a step towards street 1, and add returned value which is
													// how many times is goal reached through this path
	}
	if(avenue-1){	// If avenue 1 is not reached, statement is true for any number greater than 1
		result += numPathsHome(street, avenue-1);	// Then take a step towards avenue 1, and add returned value which is
													// how many times is goal reached through this path
	}

	if((street == 1) && (avenue == 1)){	// If finally (1, 1) is reached
		result = 1;						// Set result to 1 since goal (1, 1) is reached
	}

	return result;
}

/*----------Part 2----------*/

/*
Hospital struct holding it's name and cities it serves to
*/
typedef struct 
{
	char *name;				// Name of hospital
	char citiesServed[3];	// Cities served by this hospital
} Hospital;

/*
setServedCities function sets given hospital's served cities to given location
this is used instead of strcpy() since locations don't have '\0'
*/
void setServedCities(Hospital results[4],	// The array hospitals are stored 
					 char _locations[4][3], // All possible city serve schemes
					 int resultIndex, 		// Index of the hospital in results array
					 int locationIndex){	// Index of the serve scheme in _locations array

	results[resultIndex].citiesServed[0] = _locations[locationIndex][0];
	results[resultIndex].citiesServed[1] = _locations[locationIndex][1];
	results[resultIndex].citiesServed[2] = _locations[locationIndex][2];
	results[resultIndex].citiesServed[3] = '\0';
}

/*
printResults function prints the final states of the hospitals after finding the first
right locations are found for covid coverage
*/
void printResults(Hospital results[4]){	// Hospital array that holds what cities are served by them
	int i;

	/*
	print until maximum index 4 is reached or unnecessary hospital is reached
	which doesn't serve to any locations thus having citiesServed as '\0'
	*/
	for(i = 0; i < 4 && results[i].citiesServed[0] != '\0'; i++){
		printf("%s\n", results[i].name);
		printf("Hospital locations: %s\n", results[i].citiesServed);
	}
}

/*
resetResults function resets the hospital array results by setting each hopitals citiesServed to '\0'
*/
void resetResults(Hospital results[4]){
	int i;

	for(i = 0; i < 4; i++){
		strcpy(results[i].citiesServed, "\0");
	}
}

/*
canOfferCovidCoverage function finds the right locations for hospitals to cover every city
*/
int canOfferCovidCoverage(char _cities[6], 			// Every city there is
						  char _locations[4][3], 	// Possible locations to serve by hospitals
						  int _numHospitals, 		// Maximum number of hospital that can be built
						  Hospital results[4]){		// Hospital array that stores hospitals with right locations for coverage

	// Variables that are used in for loops 
	int i;
	int j;
	int k;

	// Integer corresponding to hospital in results array
	int resultIndex;

	// City char variable used for storing current city char in the loop
	char currServedCity;

	// Variable that shows if there's a possible solution with given locations and number of hospitals
	int result;

	// For loop used for finding corresponding index to next blank hospital
	for(resultIndex = 0; (resultIndex < 4) && (results[resultIndex].citiesServed[0] != '\0'); resultIndex++);

	// If given number for hospitals is greater than 0
	if(_numHospitals){

		for(i = 0, result = 0;	// Set result to 0 since loop will go on till result is 1
		   (i < 4) && !result;	// Unless maximum index 4 is not reached and result is not 1
		    i++)				
		{
			setServedCities(results, _locations, resultIndex, i);	// Set citiesServed of current hospital			
			result = canOfferCovidCoverage(_cities, _locations, _numHospitals-1, results);	// Proceed to next hospitals
		}

		// If the right locations are not found then set current hospital's citiesServed back to "\0"
		if(!result)	strcpy(results[resultIndex].citiesServed, "\0");
	}
	else{	// If given numHospitals is 0

		for(k = 0; (k < 6) && result; k++){	// k is representing index of the current city here

			result = 0;	// Setting to 0 if city is not served at all

			for(i = 0; (i < resultIndex) && !result; i++){	// i is representing hospital index in results array here
															// resultIndex is max since it correspondes to first hospital
															// with empty citiesServed, also stops when city is found thus result is 1

				for(j = 0; (j < 3) && !result; j++){	// j is representing index of current served city by current hospital
														// stops when j is no longer lesser than 3 since 3 is the maximum number of
														// locations that a hospital can serve to, also stops when city is found in
														// hospital's served cities
					
					// Curren served city citiesServed[j] of current hospital results[i]
					currServedCity = results[i].citiesServed[j];

					if(currServedCity == _cities[k]){	// If current city of all cities is found in this hospitals served cities
						result = 1;						// Then result is true
					}
				}
			}
		}
	}

	return result;
}


/*----------Part 3----------*/

/*
card struct consisting of face name and suit name of a card
*/
typedef struct
{
	const char *face;	// Card's face (Ace, Queen etc.)
	const char *suit;	// Card's suit (Hearts, Diamonds etc.)
} card;

/*
printDeck functions prints an array of card variables
*/
void printDeck(card *deck){	// Given array of cards
	int i;

	for(i = 0; i < 26; i++){
		printf("%s of %s\t\t%s of %s\n", deck[i].face, deck[i].suit,
										 deck[i+26].face, deck[i+26].suit);
	}
}

/*
swap function swaps two cards with given index in given card array 
*/
void swap(card *deck, int index1, int index2){
	
	// Temporary variables for the exchange
	const char *temp_face;
	const char *temp_suit;

	temp_face = deck[index1].face;
	temp_suit = deck[index1].suit;

	deck[index1].face = deck[index2].face;
	deck[index1].suit = deck[index2].suit;

	deck[index2].face = temp_face;
	deck[index2].suit = temp_suit;
}

/*
shuffle function swaps every card with another random card in given array of cards
starts from start of the array and swaps every card one at a time
*/
void shuffle(card *deck){
	int i;

	for(i = 0; i < 52; i++){
		swap(deck, i, rand() % 52);
	}
}

/*
readyDeck function creates a card array then shuffles it then returns the pointer of it
*/
card *readyDeck(){

	// Allocating memory for the so called array of cards
	card *deck = (card *) calloc(52, sizeof(card));
 
	// Suits and faces, used pointer since we don't want their strings to perish when the scope is left
	char *suits[4]  = {"Hearts", "Diamonds", "Clubs", "Spades"};
	char *faces[13] = {"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};

	// Used variables in the next for loop
	int suitIndex;
	int faceIndex;
	int cardIndex = 0;

	// Firstly fills the array with all possible cards in order
	for(suitIndex = 0; suitIndex < 4; suitIndex++){
		for(faceIndex = 0; faceIndex < 13; faceIndex++){
			deck[cardIndex].face = faces[faceIndex];
			deck[cardIndex].suit = suits[suitIndex];
			cardIndex++;
		}
	}

	// Then shuffles it
	shuffle(deck);

	return deck;
}


/*-----------Menu-----------*/
void menu(){

	// Variables for the numPathsHome function
	int street;
	int avenue;

	// Variables for the numHospitals function
	char cities[6] = { 'A', 'B', 'C', 'D', 'E', 'F' };
	char locations[4][3] = { {'A', 'B', 'C'}, {'A', 'C', 'D'}, {'B', 'F'}, {'C', 'E', 'F'} };
	int  numHospitals;

	Hospital results[4] = {{"Hospital - 1", "\0"},
						   {"Hospital - 2", "\0"},
						   {"Hospital - 3", "\0"},
						   {"Hospital - 4", "\0"}};

	// Card pointer (array) to store return of readyDeck function
	card *deck;

	// User's command, 0 as initial value
	int command = 0;

	printf("Welcome to the Homework 9			\n");

	while(command != 4){	// Unless user enter exit command 4

		printf("\n\n");
		printf("-------------Select part------------\n");
		printf("1) Evaluate Number of Optimal Paths \n");
		printf("2) Evaluate Covid Coverage 			\n");
		printf("3) Shuffle and Print Deck of Cards	\n");
		printf("4) Exit								\n");
		
		// Get command from user
		scanf("%d", &command);

		switch(command){
			case 1:
				/*
				If user wants to see part 1
				get street and avenue numbers then calculate the result
				show result to the user
				*/
				printf("Enter the street number:\n");
				scanf("%d", &street);

				printf("Enter the avenue number:\n");
				scanf("%d", &avenue);

				printf("Number of optimal paths to take back home: %d\n", numPathsHome(street, avenue));
				break;
			case 2:

				printf("Enter the maximum number of hospitals that can be constructed:\n");
				scanf("%d", &numHospitals);	// Get number of hospitals from user
				
				switch(canOfferCovidCoverage(cities, locations, numHospitals, results)){	
					
					// If result is 1 thus it's possible with given number of hospitals
					case 1:

						// Then proceed to printing results then reset them for the next time
						printf("\nYes, can offer health care for all!\n\n");
						printResults(results);
						resetResults(results);
						break;
					default:
						printf("No, some cities are not covered.\n");
				}
				break;
			case 3:

				// If user wants shuffled deck of cards, print them
				printf("\nYour cards:\n\n");
				deck = readyDeck();
				printDeck(deck);	
		}
	}
}


int main(){	
	srand(time(NULL));
	menu();
	return 0;
}