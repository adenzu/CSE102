#ifndef	SECOND_PART_OF_TERM_PROJECT
#define SECOND_PART_OF_TERM_PROJECT


#define DEG_RAD M_PI / 180
#define RAD_DEG 180 / M_PI


enum archetype{SQUARE, HEXAGON, NONE_SHAPE};


typedef struct point_s
{
	double x, y;
} Point;

typedef struct color_s
{
	int r, g, b;
} Color;

typedef struct int_ll_s
{
	int value;
	struct int_ll_s * next;
} IntegerLinkedlistNode;

typedef struct shape_s
{
	int side_count;
	Point * centre;
	Point * vertices;
} Shape;

typedef struct canvas_s
{
	Point * canvas_size;
	Color * background_color;
	Color * foreground_color;
	Color * grid_color;
	int grid_thickness;
	int tile_size;
	int show_grid;
	int pattern_thickness;
	Point * angle_offset;
	char * output_file_name;
	enum archetype pattern_shape;
} Canvas;


IntegerLinkedlistNode * get_last_node(IntegerLinkedlistNode * root);
IntegerLinkedlistNode * create_int_node(int value);
Canvas * create_canvas();
Canvas ** run_commands(int * canvas_count);
Point * reposition_point(Point * given_point, double x, double y);
Point * rotate_point_deg(Point * given_point, double angle_deg);
Point * rotate_point_rad(Point * given_point, double angle_rad);
Point * shift_point(Point * given_point, double x, double y);
Point * sum_points(Point * p1, Point * p2);
Point * copy_point(Point * given_point);
Shape * create_equilateral_shape_side_length(Point * centre, double side_length, int side_count, double point_angle_offset);
Shape * create_equilateral_shape_radius(Point * centre, double radius, int side_count, double point_angle_offset);
Shape * create_hexagon_pattern_shape(Point * centre, double side_length, Point * angle_offset);
Shape * create_square_pattern_shape(Point * centre, double side_length, Point * angle_offset);
Color * create_color(int r, int g, int b);
Point * create_point(int x, int y);
long int get_file_size(FILE * file);
int * read_n_integers(char * text, int n);
int find_char(char * text, char wanted);
int get_text_line_count(char * text);
char * read_n_chars(char * text, int n);
char * read_file(char * file_name);
char ** seperate_command_line(char * text, int * field_count);
char ** get_file_lines(char * file_name, int * lc);
void draw_grid_in_pattern(FILE * file, Shape * given_shape, int row_count, int column_count, Point * start_position, Point * horizontal_shift, Point * vertical_shift);
void seperate_string(char * dest, char seperation_char, char ** first_half, char ** second_half);
void set_canvas_field(Canvas * given_canvas, char * field_name, char * field_value_string);
void set_canvas_fields(Canvas * given_canvas, char ** fields_and_values, int field_count);
void draw_canvas_background(FILE * file, Canvas * given_canvas);
void append_int_node(IntegerLinkedlistNode * root, int value);
void draw_canvas_pattern(FILE * file, Canvas * given_canvas);
void draw_shape(FILE * file, Shape * given_shape, int fill);
void draw_canvas_grid(FILE * file, Canvas * given_canvas);
void draw_line(FILE * file, Point * p1, Point * p2);
void change_file_color(FILE * file, Color * color);
void set_canvas_defaults(Canvas * given_canvas);
void draw_canvas(Canvas * given_canvas);


#endif