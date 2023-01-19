#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "termp2.h"


/*------------------------------Function Definitions------------------------------*/

long int get_file_size(FILE * file){

	long int current_position = ftell(file);

	fseek(file, 0, SEEK_END);

	long int file_size = ftell(file);

	fseek(file, current_position, SEEK_SET);

	return file_size;
}

char * read_file(char * file_name){

	FILE * file;

	file = fopen(file_name, "r");

	long int file_size = get_file_size(file);

	char * file_text = (char *) calloc(file_size+1, sizeof(char));

	fread(file_text, file_size + 1, 1, file);

	fclose(file);

	return file_text;
}

int get_text_line_count(char * text){

	int i;
	int line_count;
	char curr_char;

	for(i = 0, line_count = 0; (curr_char = text[i]) != '\0'; i++){
		if(curr_char == '\n') line_count++;	
	}

	return line_count;
}

char ** get_file_lines(char * file_name, int * lc){

	int i, j, k;

	char * file_text = read_file(file_name);

	int line_count = get_text_line_count(file_text);
	char ** line_pointer_array = (char **) calloc(line_count, sizeof(char *));

	int line_length;
	char curr_char;

	for(j = 0, i = 0; j < line_count; j++){

		for(line_length = 0; (curr_char = file_text[i]) != '\0'; i++){

				if(curr_char == '/'){
					while((curr_char = file_text[i]) != '\n') i++;
					break;
				}
				else if(curr_char == '\n') break;
				else if(curr_char != ' ' && curr_char != '\t') line_length++;
		}
		i++;
		line_pointer_array[j] = (char *) calloc(line_length, sizeof(char));
	}

	for(j = 0, i = 0; j < line_count; j++){

		for(k = 0; (curr_char = file_text[i]) != '\0'; i++){

				if(curr_char == '/'){
					while((curr_char = file_text[i]) != '\n') i++;
					break;
				}
				else if(curr_char == '\n') break;
				else if(curr_char != ' ' && curr_char != '\t') line_pointer_array[j][k++] = curr_char;
		}
		i++;
	}

	*lc = line_count;
	return line_pointer_array;
}

IntegerLinkedlistNode * create_int_node(int value){

	IntegerLinkedlistNode * new_int_node = (IntegerLinkedlistNode *) malloc(sizeof(IntegerLinkedlistNode));

	new_int_node->value = value;
	new_int_node->next = NULL;

	return new_int_node;
}

IntegerLinkedlistNode * get_last_node(IntegerLinkedlistNode * root){

	IntegerLinkedlistNode * curr_node = root;

	while(curr_node->next != NULL) curr_node = curr_node->next;

	return curr_node;
}

void append_int_node(IntegerLinkedlistNode * root, int value){

	get_last_node(root)->next = create_int_node(value);
}

char ** seperate_command_line(char * text, int * field_count){

	int i;
	char curr_char;

	IntegerLinkedlistNode * seperation_indexes = create_int_node(-1);
	IntegerLinkedlistNode * curr_node = seperation_indexes;
	
	int seperation_count;

	for(i = 0, seperation_count = 0; (curr_char = text[i]) != '\0'; i++){
		
		if(curr_char == ',' && (text[i+1] > 57 || text[i+1] < 48)){
			seperation_count++;
			append_int_node(seperation_indexes, i);
		}
	}
	
	char ** fields_and_values = (char **) calloc(seperation_count + 1, sizeof(char *));
	int string_len;

	for(i = 0; curr_node->next != NULL; i++){

		string_len = curr_node->next->value - curr_node->value - 1;

		fields_and_values[i] = (char *) calloc(string_len + 1, sizeof(char));

		strncpy(fields_and_values[i], text + curr_node->value + 1, string_len);

		curr_node = curr_node->next;
	}

	fields_and_values[i] = text + curr_node->value + 1;

	*field_count = seperation_count + 1;
	return fields_and_values;
}

int * read_n_integers(char * text, int n){

	int * int_array = (int *) calloc(n, sizeof(int));
	
	int i, index, int_encountered;
	char curr_char;

	for(i = 0, index = 0, int_encountered = 0; index <= n && (curr_char = text[i]) != '\0'; i++){

		if(!int_encountered && (curr_char >= 48 && curr_char <= 57)){
			sscanf(text + i, "%d", int_array + index++);
			int_encountered = 1;
		}
		else if(curr_char < 48 || curr_char > 57) int_encountered = 0;
	}

	return int_array;
}

int find_char(char * text, char wanted){

	int i;
	char curr_char;

	for(i = 0; text[i] != wanted && text[i] != '\0'; i++);

	if(text[i] == '\0') i = -1;

	return i;
}

char * read_n_chars(char * text, int n){

	char * read_chars = (char *) calloc(n + 1, sizeof(char));

	int i;

	for(i = 0; i < n && text[i] != '\0'; i++) read_chars[i] = text[i];

	return read_chars;
}

Point * create_point(int x, int y){

	Point * new_point = (Point *) malloc(sizeof(Point));

	new_point->x = x;
	new_point->y = y;

	return new_point;
}

Color * create_color(int r, int g, int b){

	Color * new_color = (Color *) malloc(sizeof(Color));

	new_color->r = r;
	new_color->g = g;
	new_color->b = b;

	return new_color;
}

void set_canvas_field(Canvas * given_canvas, char * field_name, char * field_value_string){

	int * field_value;

	if(!strcmp(field_name, "canvas_size")){
		field_value = read_n_integers(field_value_string, 2);
		given_canvas->canvas_size = create_point(field_value[0], field_value[1]);
	}
	else if(!strcmp(field_name, "background_color")){
		field_value = read_n_integers(field_value_string, 3);
		given_canvas->background_color = create_color(field_value[0], field_value[1], field_value[2]);
	}
	else if(!strcmp(field_name, "foreground_color")){
		field_value = read_n_integers(field_value_string, 3);
		given_canvas->foreground_color = create_color(field_value[0], field_value[1], field_value[2]);
	}
	else if(!strcmp(field_name, "grid_color")){
		field_value = read_n_integers(field_value_string, 3);
		given_canvas->grid_color = create_color(field_value[0], field_value[1], field_value[2]);
	}
	else if(!strcmp(field_name, "grid_thickness")){
		field_value = read_n_integers(field_value_string, 1);
		given_canvas->grid_thickness = *field_value;
	}
	else if(!strcmp(field_name, "tile_size")){
		field_value = read_n_integers(field_value_string, 1);
		given_canvas->tile_size = *field_value;
	}
	else if(!strcmp(field_name, "show_grid")){

		if(!strcmp(field_value_string, "False")) given_canvas->show_grid = 0;
		else if(!strcmp(field_value_string, "True")) given_canvas->show_grid = 1;
	}
	else if(!strcmp(field_name, "pattern_thickness")){
		field_value = read_n_integers(field_value_string, 1);
		given_canvas->pattern_thickness = *field_value;
	}
	else if(!strcmp(field_name, "angle_offset")){
		field_value = read_n_integers(field_value_string, 2);
		given_canvas->angle_offset = create_point(field_value[0], field_value[1]);
	}
	else if(!strcmp(field_name, "archetype")){

		if(!strcmp(field_value_string, "Square")) given_canvas->pattern_shape = SQUARE;
		else if(!strcmp(field_value_string, "Hexagon")) given_canvas->pattern_shape = HEXAGON;
	}
	else if(!strcmp(field_name, "file_name")){
		given_canvas->output_file_name = field_value_string;
	}
}

void seperate_string(char * dest, char seperation_char, char ** first_half, char ** second_half){

	int seperation_index = find_char(dest, seperation_char);

	*first_half = read_n_chars(dest, seperation_index);
	*second_half = read_n_chars(dest + seperation_index + 1, strlen(dest) - seperation_index - 1);
}

void set_canvas_fields(Canvas * given_canvas, char ** fields_and_values, int field_count){

	int i;

	char * curr_field;
	char * curr_value;

	for(i = 0; i < field_count; i++){
		seperate_string(fields_and_values[i], ':', &curr_field, &curr_value);
		set_canvas_field(given_canvas, curr_field, curr_value);
	}
}

void set_canvas_defaults(Canvas * given_canvas){

	int line_count;
	char ** default_lines = get_file_lines("defaults.txt", &line_count);

	set_canvas_fields(given_canvas, default_lines, line_count);
}

Canvas * create_canvas(){

	Canvas * new_canvas = (Canvas *) malloc(sizeof(Canvas));

	set_canvas_defaults(new_canvas);

	new_canvas->output_file_name = NULL;
	new_canvas->pattern_shape = NONE_SHAPE;

	return new_canvas;
}

Canvas ** run_commands(int * canvas_count){

	int i, j;

	int line_count;
	char ** command_lines = get_file_lines("commands.txt", &line_count);
	char ** command_line_fields_and_values;

	Canvas ** canvas_array = (Canvas **) calloc(line_count, sizeof(Canvas *));

	int line_field_count;

	for(i = 0; i < line_count; i++){

		canvas_array[i] = create_canvas();

		command_line_fields_and_values = seperate_command_line(command_lines[i], &line_field_count);

		set_canvas_fields(canvas_array[i], command_line_fields_and_values, line_field_count);

		if(canvas_array[i]->output_file_name == NULL){
			printf("%d. line in commands.txt won't be executed! File name is not provided!\n", i+1);
			free(canvas_array[i]);
			canvas_array[i] = NULL;
		}
		else if(canvas_array[i]->pattern_shape == NONE_SHAPE){
			printf("%d. line in commands.txt won't be executed! Archetype is not provided!\n", i+1);
			free(canvas_array[i]);
			canvas_array[i] = NULL;			
		}
	}

	*canvas_count = line_count;
	return canvas_array;
}

Shape * create_equilateral_shape_radius(Point * centre, double radius, int side_count, double point_angle_offset){

	int i;

	double angle_per_side = 360.0f / side_count;
	double curr_point_angle;

	Shape * new_shape = (Shape *) malloc(sizeof(Shape));
	new_shape->side_count = side_count;
	new_shape->centre = centre;

	Point * vertices = (Point *) calloc(side_count, sizeof(Point));

	for(i = 0; i < side_count; i++){
		
		curr_point_angle = angle_per_side * i + point_angle_offset - 90;
		curr_point_angle *= DEG_RAD;
		vertices[i].x = radius * sin(curr_point_angle);
		vertices[i].y = radius * cos(curr_point_angle);
	}

	new_shape->vertices = vertices;
	return new_shape;
}

Shape * create_equilateral_shape_side_length(Point * centre, double side_length, int side_count, double point_angle_offset){

	double radius = side_length / (2 * sin(180.0f / side_count * DEG_RAD));

	return create_equilateral_shape_radius(centre, radius, side_count, point_angle_offset);
}

Point * reposition_point(Point * given_point, double x, double y){

	given_point->x = x;
	given_point->y = y;

	return given_point;
}

Point * shift_point(Point * given_point, double x, double y){

	given_point->x += x;
	given_point->y += y;

	return given_point;
}

Shape * reposition_shape(Shape * given_shape, double x, double y){

	reposition_point(given_shape->centre, x, y);

	return given_shape;
}

Shape * shift_shape(Shape * given_shape, double x, double y){

	shift_point(given_shape->centre, x, y);

	return given_shape;
}

Point * sum_points(Point * p1, Point * p2){

	Point * result_point = (Point *) malloc(sizeof(Point));

	return reposition_point(result_point, p1->x + p2->x, p1->y + p2->y);
}

void change_file_color(FILE * file, Color * color){
	fprintf(file, "%lf %lf %lf setrgbcolor\n", color->r / 255.0f, color->g / 255.0f, color->b / 255.0f);
}

void draw_shape(FILE * file, Shape * given_shape, int fill){

	int shape_side_count = given_shape->side_count;
	Point * shape_centre = given_shape->centre;
	Point * shape_vertices = given_shape->vertices;

	int i;

	fprintf(file, "%lf %lf moveto\n", shape_centre->x + shape_vertices[0].x, shape_centre->y + shape_vertices[0].y);

	for(i = 1; i < shape_side_count; i++) fprintf(file, "%lf %lf lineto\n", shape_centre->x + shape_vertices[i].x, shape_centre->y + shape_vertices[i].y);

	fprintf(file, "closepath\n");

	if(fill) fprintf(file, "fill\n");

	fprintf(file, "stroke\n");
}

void draw_canvas_background(FILE * file, Canvas * given_canvas){

	double background_filler_side_length;

	if(given_canvas->canvas_size->x > given_canvas->canvas_size->y) background_filler_side_length = given_canvas->canvas_size->x; 
	else background_filler_side_length = given_canvas->canvas_size->y;

	Shape * background_filler = create_equilateral_shape_side_length(create_point(0, 0), background_filler_side_length, 4, 45);

	change_file_color(file, given_canvas->background_color);
	draw_shape(file, background_filler, 1);
}

Point * copy_point(Point * given_point){

	Point * copy = (Point *) malloc(sizeof(Point));

	copy->x = given_point->x;
	copy->y = given_point->y;

	return copy;
}

void draw_grid_in_pattern(FILE * file, Shape * given_shape, int row_count, int column_count, Point * start_position, Point * horizontal_shift, Point * vertical_shift){
	
	int i, j;

	for(j = 0; j < row_count + 1; j++){

		for(i = 0; i < column_count; i++){
			draw_shape(file, given_shape, 0);
			shift_shape(given_shape, horizontal_shift->x, horizontal_shift->y);
		}
		
		draw_shape(file, given_shape, 0);
		reposition_shape(given_shape, start_position->x, start_position->y);
		shift_shape(given_shape, ((1 + j) % 2) * vertical_shift->x, (j + 1) * vertical_shift->y);
	}
}

void draw_line(FILE * file, Point * p1, Point * p2){

	fprintf(file, "%lf %lf moveto\n", p1->x, p1->y);
	fprintf(file, "%lf %lf lineto\n", p2->x, p2->y);
}

void draw_pattern_shape(FILE * file, Shape * given_shape){

	int shape_side_count = given_shape->side_count;
	Point * shape_centre = given_shape->centre;
	Point * shape_vertices = given_shape->vertices;

	int i, index;

	for(i = 0; i < shape_side_count * 2; i++){

		index = 2*i;
		draw_line(file, sum_points(shape_centre, shape_vertices + index), sum_points(shape_centre, shape_vertices + index + 1));
		fprintf(file, "stroke\n");
	}
}

void draw_pattern_shape_in_pattern(FILE * file, Shape * given_shape, int row_count, int column_count, Point * start_position, Point * horizontal_shift, Point * vertical_shift){
	
	int i, j;

	for(j = 0; j < row_count + 1; j++){

		for(i = 0; i < column_count; i++){
			draw_pattern_shape(file, given_shape);
			shift_shape(given_shape, horizontal_shift->x, horizontal_shift->y);
		}
		
		draw_pattern_shape(file, given_shape);
		reposition_shape(given_shape, start_position->x, start_position->y);
		shift_shape(given_shape, ((1 + j) % 2) * vertical_shift->x, (j + 1) * vertical_shift->y);
	}
}

Point * rotate_point_rad(Point * given_point, double angle_rad){

	double old_x = given_point->x;
	double old_y = given_point->y;

	given_point->x = old_x * cos(angle_rad) - old_y * sin(angle_rad);
	given_point->y = old_x * sin(angle_rad) + old_y * cos(angle_rad);

	return given_point;
}

Point * rotate_point_deg(Point * given_point, double angle_deg){
	return rotate_point_rad(given_point, angle_deg * DEG_RAD);
}

Shape * create_square_pattern_shape(Point * centre, double side_length, Point * angle_offset){

	double angle = angle_offset->x * DEG_RAD;
	double offset  = angle_offset->y;

	Shape * square_pattern_shape = (Shape *) malloc(sizeof(Shape));
	square_pattern_shape->centre = centre;
	square_pattern_shape->side_count = 4;
	square_pattern_shape->vertices = (Point *) calloc(2 * 2 * 4, sizeof(Point));

	Point * line = (Point *) calloc(2, sizeof(Point));
	Point * reverse_line = (Point *) calloc(2, sizeof(Point));

	double end_x = (side_length + offset) / (2 + 2 * tan(angle));
	double end_y = tan(angle) * end_x;

	reposition_point(line, -offset / 2, -side_length / 2);
	reposition_point(line + 1, end_x - offset / 2, end_y - side_length / 2);

	reposition_point(reverse_line, -line[0].x, line[0].y);
	reposition_point(reverse_line + 1, -line[1].x, line[1].y);

	int i, index;

	for(i = 0; i < 4; i++){

		index = 4*i;

		square_pattern_shape->vertices[index].x = line[0].x;
		square_pattern_shape->vertices[index].y = line[0].y;
		square_pattern_shape->vertices[index+1].x = line[1].x;
		square_pattern_shape->vertices[index+1].y = line[1].y;
		square_pattern_shape->vertices[index+2].x = reverse_line[0].x;
		square_pattern_shape->vertices[index+2].y = reverse_line[0].y;
		square_pattern_shape->vertices[index+3].x = reverse_line[1].x;
		square_pattern_shape->vertices[index+3].y = reverse_line[1].y;

		rotate_point_deg(line, 90);
		rotate_point_deg(line + 1, 90);
		rotate_point_deg(reverse_line, 90);
		rotate_point_deg(reverse_line + 1, 90);
	}

	return square_pattern_shape;
}

Shape * create_hexagon_pattern_shape(Point * centre, double side_length, Point * angle_offset){

	double angle = angle_offset->x * DEG_RAD;
	double offset  = angle_offset->y;

	Shape * hexagon_pattern_shape = (Shape *) malloc(sizeof(Shape));
	hexagon_pattern_shape->centre = centre;
	hexagon_pattern_shape->side_count = 6;
	hexagon_pattern_shape->vertices = (Point *) calloc(2 * 2 * 6, sizeof(Point));

	Point * line = (Point *) calloc(2, sizeof(Point));
	Point * reverse_line = (Point *) calloc(2, sizeof(Point));

	double end_x = sqrt(3) * (side_length + 2 * offset) / (2 * sqrt(3) + 2 * tan(angle));
	double end_y = tan(angle) * end_x;

	reposition_point(line, -offset / 2, -sqrt(3) * side_length / 2);
	reposition_point(line + 1, end_x - offset / 2, end_y - sqrt(3) * side_length / 2);

	reposition_point(reverse_line, -line[0].x, line[0].y);
	reposition_point(reverse_line + 1, -line[1].x, line[1].y);

	int i, index;

	rotate_point_deg(line, 30);
	rotate_point_deg(line + 1, 30);
	rotate_point_deg(reverse_line, 30);
	rotate_point_deg(reverse_line + 1, 30);

	for(i = 0; i < 6; i++){

		index = 4*i;
		
		rotate_point_deg(line, 60 );
		rotate_point_deg(line + 1, 60);
		rotate_point_deg(reverse_line, 60);
		rotate_point_deg(reverse_line + 1, 60);
	
		hexagon_pattern_shape->vertices[index].x = line[0].x;
		hexagon_pattern_shape->vertices[index].y = line[0].y;
		hexagon_pattern_shape->vertices[index+1].x = line[1].x;
		hexagon_pattern_shape->vertices[index+1].y = line[1].y;
		hexagon_pattern_shape->vertices[index+2].x = reverse_line[0].x;
		hexagon_pattern_shape->vertices[index+2].y = reverse_line[0].y;
		hexagon_pattern_shape->vertices[index+3].x = reverse_line[1].x;
		hexagon_pattern_shape->vertices[index+3].y = reverse_line[1].y;
	}

	return hexagon_pattern_shape;
}

void draw_canvas_grid(FILE * file, Canvas * given_canvas){

	double half_width = given_canvas->canvas_size->x / 2;
	double half_height = given_canvas->canvas_size->y / 2;

	Shape * grid_shape;
	double side_length = given_canvas->tile_size;
	enum archetype pattern_shape = given_canvas->pattern_shape;

	Point * start_position;
	Point * horizontal_shift;
	Point * vertical_shift;

	if(pattern_shape == SQUARE){
		start_position = create_point(side_length / 2 - half_width, side_length / 2 - half_height);
		grid_shape = create_equilateral_shape_side_length(copy_point(start_position), side_length, 4, 45);
		horizontal_shift = create_point(side_length, 0);
		vertical_shift = create_point(0, side_length);
	}
	else{
		start_position = create_point(-half_width, -half_height);
		grid_shape = create_equilateral_shape_side_length(copy_point(start_position), side_length, 6, 30);
		horizontal_shift = create_point(sqrt(3) * side_length, 0);
		vertical_shift = create_point(sqrt(3) * side_length / 2, 3 * side_length / 2);
	}

	change_file_color(file, given_canvas->grid_color);

	int row_count = 2 * half_height / vertical_shift->y + 1;
	int column_count =  2 * half_width / horizontal_shift->x + 1;

	draw_grid_in_pattern(file, grid_shape, row_count, column_count, start_position, horizontal_shift, vertical_shift);
}

void draw_canvas_pattern(FILE * file, Canvas * given_canvas){

	double half_width = given_canvas->canvas_size->x / 2;
	double half_height = given_canvas->canvas_size->y / 2;

	Shape * pattern_shape_lines;
	double side_length = given_canvas->tile_size;
	enum archetype pattern_shape = given_canvas->pattern_shape;

	Point * start_position;
	Point * horizontal_shift;
	Point * vertical_shift;

	if(pattern_shape == SQUARE){
		start_position = create_point(side_length / 2 - half_width, side_length / 2 - half_height);
		pattern_shape_lines = create_square_pattern_shape(copy_point(start_position), side_length, given_canvas->angle_offset);
		horizontal_shift = create_point(side_length, 0);
		vertical_shift = create_point(0, side_length);
	}
	else{
		start_position = create_point(-half_width, -half_height);
		pattern_shape_lines = create_hexagon_pattern_shape(copy_point(start_position), side_length, given_canvas->angle_offset);
		horizontal_shift = create_point(sqrt(3) * side_length, 0);
		vertical_shift = create_point(sqrt(3) * side_length / 2, 3 * side_length / 2);
	}

	change_file_color(file, given_canvas->foreground_color);

	int row_count = 2 * half_height / vertical_shift->y + 1;
	int column_count =  2 * half_width / horizontal_shift->x + 1;

	draw_pattern_shape_in_pattern(file, pattern_shape_lines, row_count, column_count, start_position, horizontal_shift, vertical_shift);
}

void draw_canvas(Canvas * given_canvas){

	FILE * file;

	file = fopen(given_canvas->output_file_name, "w");

	double half_width = given_canvas->canvas_size->x / 2;
	double half_height = given_canvas->canvas_size->y / 2;

	fprintf(file, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(file, "%%%%BoundingBox: %lf %lf %lf %lf\n", -half_width, -half_height,	//	Specifying the boundaries of the figure
														half_width, half_height);

	draw_canvas_background(file, given_canvas);
	if(given_canvas->show_grid) draw_canvas_grid(file, given_canvas);
	draw_canvas_pattern(file, given_canvas);

	fclose(file);
}





