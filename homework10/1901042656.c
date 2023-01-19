#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/*--------------------Definitions--------------------*/
#define PI 				3.14159265
#define ARC_TO_ANGLE 	180.0 / PI 		// Used for converting arc to degrees

#define MAX_LINE_LEN 	500
#define MAX_LINE_COUNT 	200
#define MAX_WORD_LEN 	20
#define MAX_WORD_COUNT	50

#define MAX_OBJECT_COUNT 100

// Defining action and struct types with unimportant but different values between their own types
// so when determining an object or action is necessary these come in handy		NOTE: realized that this could be done with enum 
#define POINT_TYPE 		1
#define LINE_TYPE 		2
#define POLYGON_TYPE 	3

#define ACTION_DISTANCE 1
#define ACTION_ANGLE 	2
#define ACTION_LENGTH	3
#define ACTION_AREA		4


/*--------------------Object Structs--------------------*/

// Point struct consisting of two coordinate values and a label
typedef struct
{
	char label[MAX_WORD_LEN];
	float x;
	float y;
} Point;

// Line struct consisting of two points and a label 
typedef struct
{
	char label[MAX_WORD_LEN];
	Point P1;
	Point P2;
} Line;

// Polygon struct consisting of at max 20 lines, an integer variable that stores the length of the array of lines and a label
typedef struct
{
	char label[MAX_WORD_LEN];
	Line Sides[20];
	int side_count;
} Polygon;


/*--------------------Creation Functions--------------------*/

/*
createPoint function creates a point with given x, y values and a string for it's label
then returns new point's pointer
*/
Point *createPoint(float x, float y, char *label){
	
	Point *new_point = malloc(sizeof(Point));	// allocate memory for the point
	new_point->x = x;					// Set x
	new_point->y = y;					// Set y
	strcpy(new_point->label, label);	// Set label
	return new_point;
}

/*
createLine function creates a line with given points and a string for it's label
then returns new Line's pointer
*/
Line *createLine(Point *P1, Point *P2, char *label){

	Line *new_line = malloc(sizeof(Line));	// Allocate memory for the line
	new_line->P1 = *P1;				// Set first point
	new_line->P2 = *P2;				// Set second point
	strcpy(new_line->label, label);	// Set label
	return new_line;
}

/*
createPolygonLines function creates a polygon with given lines and a string for it's label, line_count is the length of line array
then returns new Polygon's pointer
*/
Polygon *createPolygonLines(Line *line_arr, int line_count, char *label){

	int i;	// for for loop
	Polygon *new_polygon = malloc(sizeof(Polygon));	// Allocate memory for the polygon

	for(i = 0; i < line_count; i++){		// Untill line_count number of lines is put in to sides array of the polygon
		new_polygon->Sides[i] = line_arr[i];
	}

	new_polygon->side_count = line_count;	// Set polygon's side count to given line count, this value is useful in following parts of the code
	strcpy(new_polygon->label, label);		// Set polygon's label

	return new_polygon;
}

/*
createPolygonLines function creates a polygon with given points and a string for it's label, point_count is the length of point array
actually since polygon struct only has line array in it, the given points are used for creating lines, then those lines are used for
creating the polygon
returns new Polygon's pointer
*/
Polygon *createPolygonPoints(Point *point_arr, int point_count, char *label){
	
	int i;	// for for loop
	Polygon *new_polygon = malloc(sizeof(Polygon));	// Allocate memory for polygon

	for(i = 0; i < point_count - 1; i++){	// Until i is 1 less then the limit, thus is able to get next point  
		new_polygon->Sides[i] = *createLine(point_arr+i, point_arr+i+1, "");	// Create a line with current point and with the just after it
																				// then put the line value in polygon's side array
	}

	// The last line from last point in given point array to first point in given point array
	new_polygon->Sides[i] = *createLine(point_arr+i, point_arr, "");
	new_polygon->side_count = point_count;	// Set it's side count to point count, this value is useful afterwards

	strcpy(new_polygon->label, label);	// Set label

	return new_polygon;
}


/*--------------------Print Functions--------------------*/

// These functions were used in development process, and since they are easy to understand
// they are not explained

void printPoint(Point P){
	printf("%s:\tx:%f\ty:%f\n", P.label, P.x, P.y);
}

void printLine(Line L){
	
	printf("%s:\n\t", L.label);
	printPoint(L.P1);
	printf("\t");
	printPoint(L.P2);
}

void printPolygon(Polygon P){

	int i;

	printf("%s:\n", P.label);
	for(i = 0; i < P.side_count; i++){
		printLine(P.Sides[i]);
	}
}


/*--------------------Action Functions--------------------*/

/*
Point2Point function calculates the distance between given two points then returns the result
*/
float Point2Point(Point P1, Point P2){

	float result;

	float dx = P1.x - P2.x;	// Delta x
	float dy = P1.y - P2.y;	// Delta y

	result = sqrt(dx*dx + dy*dy);	// Pythagorean theorem to calculate distance

	return result;
}

/*
Point2Line function calculates distance from given point to given line
it does this by checking if there's a perpendicular line from point to line if there is thus intersection point is nearest point on the line
then the distance measured between those two points
if that's not the case then one of the end points of the line should be the nearest point thus checks which one is it then calculates the
distance between those two
then returns the result
*/
float Point2Line(Point P, Line L){

	float result;

	// Already calculated the distances between given point and the end points of the line 
	float point_distance1 = Point2Point(P, L.P1);
	float point_distance2 = Point2Point(P, L.P2);

	// Declaring and assigning needed x and y values to calculate intersection point between perpendicular line from given point
	// to given line and the given line itself
	float x0 = P.x;
	float y0 = P.y;

	float x1 = L.P1.x;
	float y1 = L.P1.y;
	
	float x2 = L.P2.x;
	float y2 = L.P2.y;

	// Slope of given line
	float m = (y1-y2)/(x1-x2);

	// Intersection point's coordinates
	float xi = (x0/m + m*x1 + y0 - y1)/(m + 1/m);
	float yi = m * (xi - x1) + y1;

	// If xi is in the x1-x2 or x2-x1 interval and yi is in the y1-y2 or y2-y1 interval
	// if xi is in between of the x1 and x2 then it should be greater than one of them
	// and lesser than one of them, thus multiplication of both differences shall be less than 0
	// this is valid for yi to y1 and y2 too
	if((xi-x1)*(xi-x2) < 0 && (yi-y1)*(yi-y2) < 0){

		// Then create intersection point and set result to distance between two
		Point intersection_point = {xi, yi};	
		result = Point2Point(P, intersection_point);
	}
	else if(point_distance1 < point_distance2) result = point_distance1;	// else if distance from given point to end point one of line is less than distance to end point two
																			// set result to distance one
	else result = point_distance2;	// If not set result to distance 2
	
	return result;
}

/*
Angle function calculates the angle between given two lines
*/
float Angle(Line L1, Line L2){

	float result;

	// Declaring and assigning x and y values of end points of lines
	// this is done for simpler appeareance of the following calculation
	float x1, x2, x3, x4;
	float y1, y2, y3, y4;

	x1 = L1.P1.x;
	x2 = L1.P2.x;
	x3 = L2.P1.x;
	x4 = L2.P2.x;

	y1 = L1.P1.y;
	y2 = L1.P2.y;
	y3 = L2.P1.y;
	y4 = L2.P2.y;

	// Finds the angle of the lines with atan2 that gives angle of given delta y and delta x
	// then multiplying the arc results with ARC_TO_ANGLE to get angle results
	float angle1 = (atan2(y1-y2, x1-x2) * ARC_TO_ANGLE);
	float angle2 = (atan2(y3-y4, x3-x4) * ARC_TO_ANGLE);

	// Getting angles in 0-360 interval if they are not
	if(angle1 < 0) angle1 += 360.0;
	if(angle2 < 0) angle2 += 360.0;

	// Angle between two lines is basically the difference between angles of those two
	if(angle1 > angle2) result = angle1 - angle2;
	else result = angle2 - angle1;

	if(result > 90) result = 180.0 - result;	// This does not change correctness of the result actually
												// but I preferred the lesser angle between two lines to be the result

	return result;
}

/*
LineLength function calculates the length of given line by calculating the distance between it's end points
returns the result
*/
float LineLength(Line L){
	return Point2Point(L.P1, L.P2);
}

/*
PolygonLength function calculates each polygon line's length then returns the sum
*/
float PolygonLength(Polygon P){

	int i;
	float result;

	for(i = 0, result = 0; i < P.side_count; i++){	// Starting from 0 to side count of polygon P, measure line length and add to result
		result += LineLength(P.Sides[i]);
	}

	return result;
}

/*
PolygonArea function calculates given polygon's are by multiplying x values of each point to y values of next point
then adds it to result then does vice versa and substracts from the result and finally since are cannot be negative
gets it's absolute value just in case then divides by 2, I found this method on the internet
then returns the result 
*/
float PolygonArea(Polygon P){

	int i;
	float result;

	for(i = 0, result = 0; i < P.side_count - 1; i++){	// Untill points of the line one before the last line
		result += P.Sides[i].P1.x * P.Sides[i+1].P1.y;	// Multiply first line's first point's x with second line's first point's y then add to result
														// actually first point of second line and second point of first line are the same
														// but I decided to write it this way, it seemed easier
	}
	result += P.Sides[i].P1.x * P.Sides[0].P1.y;		// Multiplying x value of first point of last line with y value of first point of first line of given polygon

	// Here basically the same thing is done in loop above but this time it's y first and x next
	// and it's not added but subtracted from result, I don't know why it's just how it works (didn't look into mathmetics)
	for(i = 0; i < P.side_count - 1; i++){
		result -= P.Sides[i].P1.y * P.Sides[i+1].P1.x;
	}
	result -= P.Sides[i].P1.y * P.Sides[0].P1.x;

	result = abs(result) / 2.0;	// Lastly result should be a positive value, then divides by 2 to get correct result

	return result;
}


/*--------------------Run Commands--------------------*/

/*
splitLine function splits given string into parts by given another string
splitted parts are put in to given string array and function itself returns how many times is the string splitted
returns -1 if splitting string factor is not found in given main string
*/
int splitLine(char *line, 			// Main string which will be splitted
			  const char *splitter, // Split factor
			  char **str_arr){		// Result parts 

	int i = -1;	
	char *token; // Current split part

	if(strstr(line, splitter)){	// If main string contains split factor string

		token = strtok(line, splitter);		// First part of split

		for(i = 0; token != NULL; i++){		// Untill nothing left to split 
			str_arr[i] = token;				// Put split part in to string array
			token = strtok(NULL, splitter);	// Continue to process
		}
	}

	return i;
}

/*
getActualLine function gets rid of the comment part of the lines if there is one then returns the result
but since strtok altars the splitted string anyway you may see this function's result is not stored but given
string used after the func is called
*/
char *getActualLine(char *line){	// Given string that is read line from the commands file

	int i;

	char *actual_line = (char *) calloc(MAX_LINE_LEN, sizeof(char));	// Returned string
	char **line_parts = (char **) calloc(2, sizeof(char *));			// 2 strings array since there should be only one comment per line
																		// passed in to splitLine function

	if(splitLine(line, "//", line_parts) != -1){	// If result from splitLine is not -1, split is succesful that is
		strcpy(actual_line, line_parts[0]);			// set actual_line to first item of the line_parts array since comment comes after
	}
	else{
		strcpy(actual_line, line);	// Set given actual_line to given line since no split was done
	}

	/*
	this for loop gets rid of the space and tab and new line characters in the given line
	since every char's ascii that is mentioned above is less than 48, when when a character is encountered
	that it's ascii is greater or equal to 48 the one right char in the string is set to '\0' then loop gets broken
	also since spaces tabs and new lines are at the end of the line, i is set to length of the string when loop starts
	*/
	for(i = strlen(actual_line); i+1; i--){		// Stop when i+1 is 0 thus i is -1, this is i+1 insted of i because there's an item at 0 index in an array

		if(actual_line[i] > 47){
			actual_line[i+1] = '\0';
			break;
		}
	}

	return actual_line;
}

/*
determineObject function checks given read object name string from the commands file
then returns corresponding object type, such as point, line or polygon
*/
int determineObject(char *str){

	int result = LINE_TYPE;	// Set result to line as initial value

	if(str[1] == 'G') result = POLYGON_TYPE;	// If second char of the name of the object is G then this object is polygon
	else if(str[0] == 'P') result = POINT_TYPE;	// Else if first char of the name of the object is P then it has to be a point

	return result;
}

/*
findPoint function finds the point with given label in given array by searching through it until limit index is reached 
returns found point's pointer
*/
Point *findPoint(Point *point_arr, char *label, int limit){

	int i;

	for(i = 0; strcmp(point_arr[i].label, label) != 0 && i < limit; i++); // Checks until label of the item at index i in point_arr array 
																		  // is the same of given label or i is equal to limit

	return (point_arr+i);
}

/*
findPoints function basically does what findPoint function does but it finds every point by given labels in the label array
then returns the pointer of all the found points, basically array of points that is
also stops when point_count number of points are found, since point_count is basically length of the given label array
*/
Point *findPoints(Point *point_arr, char **label_arr, int point_count, int limit){

	int i;
	Point *found_points = (Point *) calloc(point_count, sizeof(Point));

	for(i = 0; i < point_count; i++){
		found_points[i] = *findPoint(point_arr, label_arr[i], limit);
	}

	return found_points;
}

/*
findLine function does exact same thing what findPoint but instead of point it is for line object
returns found line's pointer 
*/
Line *findLine(Line *line_arr, char *label, int limit){

	int i;

	for(i = 0; strcmp(line_arr[i].label, label) != 0 && i < limit; i++);

	return (line_arr+i);
}

/*
findLines function is the exact same of findPoints but instead of points it is for line objects
returns pointer of line array
*/
Line *findLines(Line *line_arr, char **label_arr, int line_count, int limit){

	int i;
	Line *found_lines = (Line *) calloc(line_count, sizeof(Line));

	for(i = 0; i < line_count; i++){
		found_lines[i] = *findLine(line_arr, label_arr[i], limit);
	}

	return found_lines;
}

/*
findPolygon function is the exact same of findPoint but instead of point it is for polygon object
returns pointer of found polygon
*/
Polygon *findPolygon(Polygon *polygon_arr, char *label, int limit){

	int i;

	for(i = 0; strcmp(polygon_arr[i].label, label) != 0 && i < limit; i++);

	return (polygon_arr+i);
}

/*
readData function reads each line in the commands file starting from one line under the 'data' line
then creates objects with given labels and properties, then stores them in given arrays
*/
void readData(FILE *file, Point *point_arr, Line *line_arr, Polygon *polygon_arr){

	char *curr_line = (char *) calloc(MAX_LINE_LEN, sizeof(char));	// Current read line in the while loop
	char *actual_line;	// Current read line without comment

	char **data_parts = (char **) calloc(MAX_WORD_COUNT, sizeof(char *));	// Parts of the line, when line is seperated by space characters 

	int number_of_objects;					// Number of objects that will be declared afterwards
	fgets(curr_line, MAX_LINE_LEN, file);	// Reads the line just after the "data" line, which contains number of objects
	number_of_objects = atoi(curr_line);	// Convert string to integer then store it in number_of_objects

	// Indexes corresponding to new objects for each object type
	int point_index = 0;
	int line_index = 0;
	int polygon_index = 0;

	// part_count is the returned integer value by the splitLine function it needed when part_count is -1 thus loop stops
	int part_count = 1;

	while(part_count+1 && !feof(file)){	// Untill part_count is -1 thus part_count+1 is 0 or end of file is reached

		fgets(curr_line, MAX_LINE_LEN, file);	// Read line
		actual_line = getActualLine(curr_line);	// Get rid of comments

		part_count = splitLine(actual_line, " ", data_parts); // Split line into parts

		if(part_count+1){	// If part count is not -1
			switch(determineObject(data_parts[part_count-1])){	// Determine the type of the object, part_count-1 since part_count is the length of the data_parts array
				case POINT_TYPE:	// If the written object is point

					/*
					then convert string parts of the line to float values with strtof and 
					create a point with given values then store it in point_arr at point_index
					add one to point_index to be ready to next time
					*/
					point_arr[point_index++] = *createPoint(strtof(data_parts[0], NULL),
																strtof(data_parts[1], NULL), 
																data_parts[2]);
					break;

				case LINE_TYPE:		// If object is line

					/*
					actually does the same thing but for line, but in this case since lines are defined with points
					needed points for the line creation are found with findPoint function then line is created and stored in the array
					adds one to index to be ready for the next time
					*/
					line_arr[line_index++] = *createLine(findPoint(point_arr, data_parts[0], point_index),
															 findPoint(point_arr, data_parts[1], point_index),
															 data_parts[2]);
					break;

				case POLYGON_TYPE:	// If object is polygon

					switch(determineObject(data_parts[0])){	// Checks how the polygon is defined, since it can be defined with points or lines
															// Checks only the first type since definition assumed as only one type of objects
						case POINT_TYPE:	// If defined with points

							/*
							finds right points with findPoints function then creates and stores the polygon in polygon_arr
							adds one to index to be ready for the next time
							*/
							polygon_arr[polygon_index++] = *createPolygonPoints(findPoints(point_arr, data_parts, part_count-1, point_index),
																					part_count-1,
																					data_parts[part_count-1]);
							break;

						case LINE_TYPE:		// If defined with lines
							
							/*
							basically does the same thing above but with lines
							*/
							polygon_arr[polygon_index++] = *createPolygonLines(findLines(line_arr, data_parts, part_count-1, line_index),
																				   part_count-1,
																				   data_parts[part_count-1]);
							break;
						}
					break;
			}
		}
	}
}

/*
determineAction function finds which action corresponds to the given action string
then returns it
*/
int determineAction(char *str){

	int result;

	if(		strcmp(str, "Distance") == 0) result = ACTION_DISTANCE;
	else if(strcmp(str, "Length") 	== 0) result = ACTION_LENGTH;
	else if(strcmp(str, "Angle") 	== 0) result = ACTION_ANGLE;
	else if(strcmp(str, "Area") 	== 0) result = ACTION_AREA;

	return result;
}

/*
writeActions function actually writes action results, it reads the given file where it was left after readData func
then executes those actions finally writes the results into given file 
*/
void writeActions(FILE *file, Point *point_arr, Line *line_arr, Polygon *polygon_arr){

	char *curr_line = (char *) calloc(MAX_LINE_LEN, sizeof(char));	// Current read line in while loop
	char *actual_line;	// Current read line without comment

	char **data_parts = (char **) calloc(MAX_WORD_COUNT, sizeof(char *));	// Line split result array

	char output_path[MAX_LINE_LEN];			// Output file path
	fgets(curr_line, MAX_LINE_LEN, file);	// Reads the line that is just after the "actions" line
	strcpy(output_path, curr_line);			// Stores the path

	FILE *output_file;
	output_file = fopen(output_path, "w");	// Open output file

	fgets(curr_line, MAX_LINE_LEN, file); 	// Reads how many actions will be executed, but it is not stored since I couldn't actually 
											// use this information usefully

	while(!feof(file)){	// Until end of file is reached
		
		fgets(curr_line, MAX_LINE_LEN, file);	// Read line
		actual_line = getActualLine(curr_line);	// Get rid of comments
		splitLine(actual_line, " ", data_parts);// Split line into parts


		switch(determineAction(data_parts[0])){	// Determine the action, checks the first item since actions are written first
			
			case ACTION_DISTANCE:	// If distance is measured
				
				if(determineObject(data_parts[1]) == POINT_TYPE && determineObject(data_parts[2]) == POINT_TYPE){	// If both of the objects after the action are points

					// Then calculate the distance between the points with Point2Point func and write it into output file
					// finds needed points by findPoint func
					fprintf(output_file, "Distance(%s, %s) = %f\n", data_parts[1], data_parts[2], 
																	Point2Point(*findPoint(point_arr, data_parts[1], MAX_OBJECT_COUNT), *findPoint(point_arr, data_parts[2], MAX_OBJECT_COUNT)));
				}
				else if(determineObject(data_parts[2]) == LINE_TYPE){	// Else if second object is a line

					// Then calculate the distance from point to line with Point2Line func and write the result 
					// finds needed point and line with findPoint and findLine functions
					fprintf(output_file, "Distance(%s, %s) = %f\n", data_parts[1], data_parts[2],
																	Point2Line(*findPoint(point_arr, data_parts[1], MAX_OBJECT_COUNT), *findLine(line_arr, data_parts[2], MAX_OBJECT_COUNT)));
				}
				else{	// Else if secon object is not line thus having the last possibility that is first object is a line

					// Do the same thing above but inversed
					fprintf(output_file, "Distance(%s, %s) = %f\n", data_parts[1], data_parts[2],
																	Point2Line(*findPoint(point_arr, data_parts[2], MAX_OBJECT_COUNT), *findLine(line_arr, data_parts[1], MAX_OBJECT_COUNT)));
				}
				break;

			case ACTION_LENGTH:	// If length is measured

				if(determineObject(data_parts[1]) == LINE_TYPE){	// If given object is a line

					// Then find the line and calculate it's length with LineLength, write the result afterwards
					fprintf(output_file, "Length(%s) = %f\n", data_parts[1], LineLength(*findLine(line_arr, data_parts[1], MAX_OBJECT_COUNT)));
				}
				else{	// If not then it is a polygon

					// Thus find the polygon with findPolygon func then calculate its length with PolygonLength
					// write the result into output file afterwards
					fprintf(output_file, "Length(%s) = %f\n", data_parts[1], PolygonLength(*findPolygon(polygon_arr, data_parts[1], MAX_OBJECT_COUNT)));
				}
				break;

			case ACTION_ANGLE:	// If angle between two lines is measured

				// Find the lines seperately then measure the angle between them with Angle func
				// write the result into output file afterwards
				fprintf(output_file, "Angle(%s, %s) = %f\n", data_parts[1], data_parts[2],
															 Angle(*findLine(line_arr, data_parts[1], MAX_OBJECT_COUNT), *findLine(line_arr, data_parts[2], MAX_OBJECT_COUNT)));
				break;

			case ACTION_AREA:	// If area of a polygon is measured

				// Find the polygon then calculate its area with PolygonArea function
				// write results into output_file afterwards
				fprintf(output_file, "Area(%s) = %f", data_parts[1], PolygonArea(*findPolygon(polygon_arr, data_parts[1], MAX_OBJECT_COUNT)));
				break;
		}
	}

	printf("\nOutputs are written to %s\n", output_path);
}

/*
readCommands function has point, line and polygon arrays in itself and reads and passes lines
until words "data" or "actions" are encountered, then proceeds to corresponding functions
*/
void readCommands(char *file_path){	// Given file path where the file that is going to be read

	// Open the file
	FILE *file;
	file = fopen(file_path, "r");	

	// Declare object arrays
	Point 	point_arr[MAX_OBJECT_COUNT]; 
	Line 	line_arr[MAX_OBJECT_COUNT]; 
	Polygon polygon_arr[MAX_OBJECT_COUNT];

	char *actual_line;	// Current read line without comments
	char *curr_line = (char *) calloc(MAX_LINE_LEN, sizeof(char));	// Current read line in while loop

	while(!feof(file)){	// Until end of file is reached

		fgets(curr_line, MAX_LINE_LEN, file);	// Read line
		actual_line = getActualLine(curr_line);	// Get rid of comment
		
		// If line is "data" then proceed to data reading process
		if(strcmp(actual_line, "data") == 0){	
			readData(file, point_arr, line_arr, polygon_arr);
		}
		// else if it is "actions" then proceed to executing and writing results of actions process
		else if(strcmp(actual_line, "actions") == 0){
			writeActions(file, point_arr, line_arr, polygon_arr);
		}
	}
}


/*--------------------Menu--------------------*/

/*
menu function gets the commands file path where will be commands read then proceeds to readCommands function with file path
*/
void menu(){
	char commands_path[MAX_LINE_LEN];

	printf("Welcome to the HOMEWORK 10\n");
	printf("Please enter file path for the commands: (enter '0' for 'commands.txt')\n");
	scanf(" %s", commands_path);

	if(commands_path[0] == '0'){
		strcpy(commands_path, "commands.txt");
	}

	readCommands(commands_path);
}


/*--------------------Main--------------------*/
int main(){
	menu();
	return 0;
}