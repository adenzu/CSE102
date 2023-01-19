#ifndef TERM_PROJECT_H
#define TERM_PROJECT_H


/*
	DISCLAIMER/

	These declarations are for the user to see every function, structure and constants and to know where they are used!
	Further explanation of how they work and why such parameters are used is mentioned afterwards, in the library file.
	Please see definition sections in the library file to fully grasp.

	/DISCLAIMER
*/

enum figure_action{SET_THICKNESS, SET_COLOR, MOVETO, LINETO, CLOSE_PATH, STROKE, WRITE};


typedef struct color_s
{
	double r, g, b;
} Color;

typedef struct point2d_s
{
	double x, y;
} Point2D;

typedef struct tree_s
{
	int value;
	struct tree_s * left;
	struct tree_s * right;
} Tree;

typedef union figure_action_value_u
{
	Color * color;		// Color value
	Point2D * point;	// Point value
	int number;			// An integer value to write (show in postscript) afterwards
	double thickness;	// Double type value to set line thickness with it
} FigureActionValue;

typedef struct figure_action_ll_s
{
	enum figure_action action_type;	// Type of the action [see action constants]
	FigureActionValue value;		// Value of the action, this is a union structure since value data type varies

	struct figure_action_ll_s * next;	// Next action node
} FigureActionNode;

typedef struct figure_s
{
	double width, height;			
	double llx, lly, urx, ury;		// Boundary coordinates of the seen graphics, lower left x, upper right x etc.
	double thickness, resolution;	// Line thickness and minimum line length as resolution
	Color color;					// Current color

	FigureActionNode * actions;		// Action linkedlist
} Figure;


Figure * start_figure(double width, double height);		// Use to initate blank canvas to draw on 
FigureActionNode * get_last_action(Figure * fig);		// Use to get last actual action
FigureActionNode * create_action_node(enum figure_action action_type);	// Use to create new action
Tree * create_tree_node(int value, Tree * left, Tree * right);			// Use to create new tree node
Color * create_color(double r, double g, double b);	// Use to create new color
Point2D * create_point(double x, double y);		// Use to create new point
int rand_range_int(int lower, int upper);		// Use to get a random integer between given integers
double get_magnitude(double x, double y);		// Use to get magnitude of a vector consisting of x for horizontal length and y for vertical 
void append_tree(Tree * root, int value);		// Use to append new tree node with given value to given root.
void get_tree_branch_count(Tree * root, int step, int * counter);	// Use to get last (in deepest level in the tree) two adjacent nodes's (having same parent) level number 
void draw_binary_tree_nodes(Figure * fig, Tree * node, Point2D * centre, int branch, double radius, double alignment);	// Use to draw binary tree [WARNING: DON'T ACTUALLY USE THIS INSTEAD SEE draw_binary_tree FUNCTION]
void append_action(Figure * fig, FigureActionNode * action);	// Use to add a new action
void set_thickness_resolution(Figure * fig, double thickness, double resolution);	// Use to set thickness and resolution (min length) of new lines
void set_color(Figure * fig, Color * c);	// Use to set color for new lines
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x, double step_size);	// Use to visualize given function
void draw_polyline(Figure * fig, Point2D * poly_line, int n);	// Use to draw a line consisting of given points
void draw_polygon(Figure * fig, Point2D * poly_line, int n);	// Use to draw a closed polygon shape consisting of given points
void draw_ellipse(Figure * fig, Point2D * centre, Point2D * width_height);	// Use to draw an ellipse
void draw_binary_tree(Figure * fig, Tree * root);							// Use to draw binary tree
void scale_figure(Figure * fig, double scale_x, double scale_y);			// Use to scale figure
void resize_figure(Figure * fig, Point2D * start_roi, Point2D * end_roi);	// Use to set boundaries
void append_figures(Figure * fig1, Figure * fig2);	// Use to merge second figure to first
void export_eps(Figure * fig, char * file_name);	// Use to export .eps file

#endif