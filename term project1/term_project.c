#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "term_project.h"


/*---------------------------Function Definitions---------------------------*/

/*
	EXPLANATION
		Creates a figure with given width and height sizes and returns it after setting the properties

	PARAMETERS
		width: Width of the created figure.
		height: Height of the created figure.

	RETURN
		new_figure_p: The pointer of the newly created figure.
*/
Figure * start_figure(double width, double height){

	Figure * new_figure_p = (Figure *) malloc(sizeof(Figure));

	new_figure_p->width = width;
	new_figure_p->height = height;

	//	Setting boundaries as if centre of the figure is (0, 0)
	new_figure_p->llx = -width/2;
	new_figure_p->lly = -height/2;
	new_figure_p->urx = width/2;
	new_figure_p->ury = height/2;

	//	Blank actions list
	new_figure_p->actions = NULL;

	return new_figure_p;
}

/*
	EXPLANATION
		Gets the last action in the action linkedlist of the figure. Then returns it.
	
	PARAMETERS
		fig: Pointer of the figure which is the action linkedlist owner.

	RETURN
		curr_action: The pointer of the last actual action in the action linkedlist of the figure fig.
*/
FigureActionNode * get_last_action(Figure * fig){

	FigureActionNode * curr_action = fig->actions;	//	Getting first action

	while(curr_action->next != NULL) curr_action = curr_action->next;	//	Getting to the next action unless there's no next action

	return curr_action;
}

/*
	EXPLANATION
		Creates an action node with the given action type then returns it.

	PARAMETERS
		action_type: Type of the action in enum figure_action data type. [see action constants]

	RETURN
		new_action_p: The pointer of the newly created action node. 
*/
FigureActionNode * create_action_node(enum figure_action action_type){

	FigureActionNode * new_action_p = (FigureActionNode *) malloc(sizeof(FigureActionNode));	//	Allocate memory for the new action node

	new_action_p->action_type = action_type;	//	Set it's type to given type
	new_action_p->next = NULL;	//	Let the node know there's no next action node to it yet

	return new_action_p;
}

/*
	EXPLANATION
		Creates a new tree node with given value. Then returns it.
	
	PARAMETERS
		value:	Integer value of the newly created tree node.
		left:	Given tree node to set newly created node's next left side tree node to.
		right:	Given tree node to set newly created node's next right side tree node to.

	RETURN
		new_tree_node_p: The pointer of the newly created tree node.
*/
Tree * create_tree_node(int value, Tree * left, Tree * right){

	Tree * new_tree_node_p = (Tree *) malloc(sizeof(Tree));	//	Allocate memory

	// Set node's properties to given arguments
	new_tree_node_p->value = value;
	new_tree_node_p->left = left;
	new_tree_node_p->right = right;

	return new_tree_node_p;
}

/*
	EXPLANATION
		Creates a new point with given values then returns it.

	PARAMETERS
		x: Double type value that corresponds to point's coordinate on x-axis.
		y: Double type value that corresponds to point's coordinate on y-axis.

	RETURN
		new_point_p: The pointer of the newly created point.		
*/
Point2D * create_point(double x, double y){

	Point2D * new_point_p = (Point2D *) malloc(sizeof(Point2D));	// Allocate memory

	// Set properties
	new_point_p->x = x;	
	new_point_p->y = y;

	return new_point_p;
}

/*
	EXPLANATION
		Creates a Color type data with given values then returns it.

	PARAMETERS
		r:	Double type value for red color density.
		g:	Double type value for green color density.
		b:	Double type value for blue color density.

	RETURN
		new_color_p: The pointer of the newly created color.
		
*/
Color * create_color(double r, double g, double b){

	Color * new_color_p = (Color *) malloc(sizeof(Color));

	new_color_p->r = r;
	new_color_p->g = g;
	new_color_p->b = b;

	return new_color_p;
}

/*
	EXPLANATION
		Returns a random integer in given range.

	PARAMETERS
		lower:	Lower limit for the random integer.
		upper:	Upper limit for the random integer.

	RETURN
		: A random integer in given range.
*/
int rand_range_int(int lower, int upper){
	return (rand() % (upper - lower) + lower);
}

/*
	EXPLANATION
		Starting from root of the tree append_tree function checks each node if given value is greater or lesser than it.
		If given value is greater than the checked tree node's value then moves to right node, if case is the opposite then moves to left one.
		This is continued untill there's no next node to move to thus our value takes it's place in the tree.
	
	PARAMETERS
		root: Given tree node that is checked on.
		value: Given integer value that is compared to current tree node's value.
*/
void append_tree(Tree * root, int value){

	if(value > root->value){
		if(root->right == NULL) root->right = create_tree_node(value, NULL, NULL);
		else append_tree(root->right, value);
	}
	else{
		if(root->left == NULL) root->left = create_tree_node(value, NULL, NULL);
		else append_tree(root->left, value);
	}
}

/*
	EXPLANATION
		Checks each node in a tree recursively, each time level of node increases, since only way to advance is to go one level down.
		Level of node is referred as step, when a node with both of it's left and right are full is encountered it checks if the level
		of the node is greater than the level where this was encountered before, if so new record gets stored. Thus says where last adjacent
		nodes are.  

	PARAMETERS
		root:		Tree node that is checked.
		step:		Taken step count to reach current tree node.
		counter:	Where maximum level where adjacent nodes are encountered is stored.
		
	RETURN
		*counter:	Maximum level where adjacent nodes are encountered. Returned through pointer.
*/
void get_tree_branch_count(Tree * root, int step, int * counter){

	int n = 0;	//	Is increased by one each time when left or right node is visited

	if(root->left != NULL){
		get_tree_branch_count(root->left, step + 1, counter);	//	Continue the loop by increasing step by one
		n++;
	}
	if(root->right != NULL){
		get_tree_branch_count(root->right, step + 1, counter);	//	Continue the loop by increasing step by one
		n++;
	}

	if(n == 2 && *counter < step) *counter = step;	//	If both left and right nodes are visited and the level is greater, then set the new record
}

/*
	EXPLANATION
		Basically does pythagorean theorem for given two values.

	PARAMETERS
		x:	First value for the pythagorean theorem. Referred as x, by assuming it corresponds to a point's horizontal location.
		y:	Second value for the pythagorean theorem. Referred as y, by assuming it corresponds to a point's vertical location.

	RETURN
		:	Pythagorean theorem's result for given values.
*/
double get_magnitude(double x, double y){
	return (sqrt(x*x+y*y));
}

/*
	EXPLANATION
		Adds given action node to the actions linkedlist in given figure.

	PARAMETERS
		fig:	Where actions linkedlist is where the given action node will be added
		action:	Given action node.
*/
void append_action(Figure * fig, FigureActionNode * action){

	if(fig->actions == NULL) fig->actions = action;
	else get_last_action(fig)->next = action;
}

/*
	EXPLANATION
		Sets given figure's current line thickness and resolution to given values. Also creates and stores action nodes 
		in actions linkedlist of the given figure for thickness and resolution change to have an effect on the next graphics 
		that will be drawn.

	PARAMETERS
		fig:		The figure whose line thickness and resolution is wanted to be changed.
		thickness:	Given value for the new lines' thickness.
		resolution:	Given value for the new lines' minimum length (resolution).
*/
void set_thickness_resolution(Figure * fig, double thickness, double resolution){

	//	Create action node for the thickness change and add it to actions linkedlist of given figure
	FigureActionNode * set_thickness_action = create_action_node(SET_THICKNESS);
	set_thickness_action->value.thickness = thickness;
	append_action(fig, set_thickness_action);

	fig->thickness = thickness;
	fig->resolution = resolution;
}

/*
	EXPLANATION
		Sets the current color of given figure to given color. Also creates an action node for the change.		

	PARAMETERS
		fig:	The given figure whose current color value will be changed.
		c:		Given color for current color value of the given figure to set to.
*/
void set_color(Figure * fig, Color * c){

	//	Create action node for the color change and add it to actions linkedlist of given figure
	FigureActionNode * set_color_action = create_action_node(SET_COLOR);
	set_color_action->value.color = c;
	append_action(fig, set_color_action);

	fig->color = *c;
}

/*
	EXPLANATION
		Draws the graph of the given function from given start to given end with given resolution. If a drawn line is shorter than 
		figure's resolution, it gets aborted and function moves to the next point.

	PARAMETERS
		fig:		The figure where graph is wanted to be drawn.
		f(x):		The function of the graph with x as input for the function.
		start_x:	First x value for the graph.
		end_x:		Last x value for the graph.
		step_size:	Increment between x values.
*/
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x, double step_size){

	double curr_x, curr_y;	// Current x and y=f(x) values

	double min_len = fig->resolution;	// Minimum line length

	//	Calculating and moving to start point of the graph
	double last_x = start_x;
	double last_y = f(last_x);

	FigureActionNode * move_action = create_action_node(MOVETO);
	move_action->value.point = create_point(last_x, last_y);
	append_action(fig, move_action);

	double line_len;	// Current line length

	for(curr_x = start_x + step_size; curr_x <= end_x; curr_x += step_size){	//	Continuing to draw the graph unless end point is reached

		curr_y = f(curr_x);

		// Get the length of the line between the current and last points
		line_len = get_magnitude(curr_x-last_x, curr_y-last_y);

		if(line_len >= min_len){	//	Unless minimum length is longer than current line's

			FigureActionNode * draw_action = create_action_node(LINETO);
			draw_action->value.point = create_point(curr_x, curr_y);
			append_action(fig, draw_action);

			//	Update			
			last_x = curr_x;
			last_y = curr_y;
		}		
	}

	//	Add action node to finish and draw the graph
	FigureActionNode * stroke_action = create_action_node(STROKE);
	append_action(fig, stroke_action);
}

/*
	EXPLANATION
		Draws lines between given set of points on to given figure.

	PARAMETERS
		fig:		The figure where the lines are wanted to be drawn.
		poly_line:	Set of points that will be used to draw from where to where.
		n:			How many numbers of points will be used from the start of the given set of points.	
*/
void draw_polyline(Figure * fig, Point2D * poly_line, int n){

	int i;	// For for loops

	double min_len = fig->resolution;
	double line_len;

	double curr_x, curr_y;
	double last_x = poly_line->x;
	double last_y = poly_line->y;

	FigureActionNode * move_action = create_action_node(MOVETO);
	move_action->value.point = create_point(last_x, last_y);
	append_action(fig, move_action);

	for(i = 1; i < n; i++){	// Unless n many points are checked (for lines to be drawn at)

		// Get and set coordinates
		curr_x = poly_line[i].x;
		curr_y = poly_line[i].y;

		// Get the length of the line between the current and last points
		line_len = get_magnitude(curr_x-last_x, curr_y-last_y);

		if(line_len >= min_len){

			FigureActionNode * draw_action = create_action_node(LINETO);
			draw_action->value.point = create_point(curr_x, curr_y);
			append_action(fig, draw_action);

			// Update
			last_x = curr_x;
			last_y = curr_y;
		}
	}

	//	Add action node to finish and draw the lines
	FigureActionNode * stroke_action = create_action_node(STROKE);
	append_action(fig, stroke_action);
}

/*
	EXPLANATION
		Basically does what draw_polyline function (the function above) does, but closes the line thus creates a polygon.

	PARAMETERS
		fig:		The figure where the lines are wanted to be drawn.
		poly_line:	Set of points that will be used to draw from where to where.
		n:			How many numbers of points will be used from the start of the given set of points.			
*/
void draw_polygon(Figure * fig, Point2D * poly_line, int n){

	int i;

	double min_len = fig->resolution;

	double line_len;

	double curr_x, curr_y;
	double last_x = poly_line->x;
	double last_y = poly_line->y;

	FigureActionNode * move_action = create_action_node(MOVETO);
	move_action->value.point = create_point(last_x, last_y);
	append_action(fig, move_action);

	for(i = 1; i < n; i++){

		curr_x = poly_line[i].x;
		curr_y = poly_line[i].y;

		line_len = get_magnitude(curr_x-last_x, curr_y-last_y);
		
		if(line_len >= min_len){

			FigureActionNode * draw_action = create_action_node(LINETO);
			draw_action->value.point = create_point(curr_x, curr_y);
			append_action(fig, draw_action);
		}

		last_x = curr_x;
		last_y = curr_y;
	}

	//	Create and add an action node to close the shape
	FigureActionNode * close_path_action = create_action_node(CLOSE_PATH);
	append_action(fig, close_path_action);

	FigureActionNode * stroke_action = create_action_node(STROKE);
	append_action(fig, stroke_action);
}

/*
	EXPLANATION
		Draws an ellipse with given width and height around given centre on to given figure.

	PARAMETERS
		fig:			The fig where ellipse is wanted to be drawn.
		centre:			The point that is the centre of the drawn ellipse.
		width_height:	The point whose x value corresponds to ellipse's width and y value to height.

	DISCLAIMER/

		This function actually does an inconsistent job for drawing an ellipse.	The line length calculation for the lines
		that form the ellipse is complex, instead a basic and non-pretty way is approached to draw the ellipses.
		Function draws a perfect circle, then stretches it to obtain an ellipse.

	/DISCLAIMER
*/
void draw_ellipse(Figure * fig, Point2D * centre, Point2D * width_height){

	int i, n;	// i is for for loops

	double angle_per_line;
	double point_angle_radian;

	double cx = centre->x;
	double cy = centre->y;

	double width = width_height->x;
	double height = width_height->y;

	double half_width = width/2;
	double half_height = height/2;

	double r;

	//	Setting r to shorter one
	if(half_width > half_height) r = half_height;
	else r = half_width;

	double min_len = fig->resolution;

	double perimeter = 2 * M_PI * r;
	double half_perimeter = perimeter / 2;

	// n is the number of the lines that will be drawn to form half of the ellipse
	n = half_perimeter / min_len;

	angle_per_line = 180.0f / n;	//	Point locations are calculated with cos and sin functions so angle for one line is needed
	n *= 2;	//	Actual total number of lines to form whole of the ellipse

	//	Allocating memory for the points (that will form the lines that will form the ellipse)
	Point2D * ellipse_point_list = (Point2D *) calloc(n, sizeof(Point2D));

	//	Calculation of the point locations and storing them in the array
	for(i = 0; i < n; i++){
		point_angle_radian = i * angle_per_line * M_PI / 180;					//	Converting degrees to radians
		ellipse_point_list[i].x = cx + half_width * cos(point_angle_radian);	//	Stretching the values by multiplying with given width value
		ellipse_point_list[i].y = cy + half_height * sin(point_angle_radian);	//	Stretching the values by multiplying with given height value
	}

	// Using draw_polygon function to draw the ellipse
	draw_polygon(fig, ellipse_point_list, n);
}

/*
	EXPLANATION
		Draws each tree node from given tree node recursively. Each time tweaks the next node's alignment respect to current one
		by dividing alignment by 2. Thus not-mention-worthy mathematical conditions are met so no node gets drawn on another one.

	PARAMETERS
		fig:		The figure where binary tree wants to be drawn.
		node:		The current tree node that will be/is drawn.
		centre:		Location of the current tree node.		
		branch:		Integer value that states if current node will be on the left or the right of the last one. '-1' for left '1' for right.
		radius:		Radius of the current node. Actually it is not changed through out the process but hardcoded values are avoided.
		alignment:	The horizontal distance between the current node and the last one.
*/
void draw_binary_tree_nodes(Figure * fig, Tree * node, Point2D * centre, int branch, double radius, double alignment){

	// Moving to next nodes if there are. And tweaking the position of the next node
	if(node->left != NULL){
		draw_binary_tree_nodes(fig, node->left, create_point(centre->x - alignment, centre->y - radius * 3), -1, radius, alignment/2);
	}

	if(node->right != NULL){
		draw_binary_tree_nodes(fig, node->right, create_point(centre->x + alignment, centre->y - radius * 3), 1, radius, alignment/2);
	}

	// The first drawn node's branch value is passed as 0. This condition is for that
	if(branch){

		Point2D * connection = (Point2D *) calloc(2, sizeof(Point2D));	// Allocate memory for the points for the 
																		// line between the current node and last node

		//	Setting the location for the start of the line
		connection[0].x = centre->x;
		connection[0].y = centre->y + radius;

		//	Setting the location for the end of the line
		connection[1].x = centre->x - branch * alignment * 2;
		connection[1].y = centre->y + radius * 2;

		//	Draw the line that connects the nodes with draw_polyline
		draw_polyline(fig, connection, 2);
	}

	//	Add move action to move to a bit left of the centre
	FigureActionNode * move_action = create_action_node(MOVETO);
	move_action->value.point = create_point(centre->x-2, centre->y);
	append_action(fig, move_action);

	//	Add write action to write the value of the current node
	FigureActionNode * write_action = create_action_node(WRITE);
	write_action->value.number = node->value;
	append_action(fig, write_action);

	//	Draw the written value
	FigureActionNode * stroke_action = create_action_node(STROKE);
	append_action(fig, stroke_action);

	//	Draw circle around centre to encircle current node's value 
	draw_ellipse(fig, centre, create_point(2*radius, 2*radius));
}

/*
	EXPLANATION
		Draws binary tree starting from given tree node on to given figure.

	PARAMETERS
		fig:	The figure where binary tree is wanted to be drawn.
		root:	Starting tree node. 
*/
void draw_binary_tree(Figure * fig, Tree * root){

	//	Gets maximum level where adjacent nodes are encountered
	int n = 0;	
	get_tree_branch_count(root, 0, &n);

	double radius = 5;	//	Could be function parameter but decided to follow given function declaration in the pdf 
	double alignment = radius * pow(2, n + 1);	//	Calculating first node alignment. n + 1 because 
												// 	right and left nodes can still be drawn on each other even after 1 level down 
												//	even though they are from different nodes, so this prevents that (not-mention-worthy stuff)

	//	Using draw_binary_tree_nodes recursive function to actually draw the tree
	draw_binary_tree_nodes(fig, root, create_point(0, fig->height/3), 0, radius, alignment);	//	Start point is chosen arbitrarily
}

/*
	EXPLANATION
		Scales given figure by given values. The greater the value the bigger the graphics get. 1 1 for normal size.

	PARAMETERS
		fig:		The figure which will be scaled.
		scale_x:	Scale factor for the given figure along the x-axis.
		scale_y:	Scale factor for the given figure along the y-axis.
*/
void scale_figure(Figure * fig, double scale_x, double scale_y){

	FigureActionNode * curr_action = fig->actions;

	while(curr_action != NULL){	//	Multiplying each value of the points through loop

		switch(curr_action->action_type){

			case LINETO:
			case MOVETO:
				curr_action->value.point->x *= scale_x;
				curr_action->value.point->y *= scale_y;
				break;
		}

		curr_action = curr_action->next;
	}
}

/*
	EXPLANATION
		Sets new boundaries to given values for the given figure.

	PARAMETERS
		fig:		The figure that is resized.
		start_roi:	One end of the boundaries. Is at opposite of the end_roi. 
		end_roi:	One end of the boundaries. Is at opposite of the start_roi. 
*/
void resize_figure(Figure * fig, Point2D * start_roi, Point2D * end_roi){

	fig->llx = start_roi->x;
	fig->lly = start_roi->y;
	fig->urx = end_roi->x;
	fig->ury = end_roi->y;

	fig->width = end_roi->x - start_roi->x;
	fig->height = end_roi->y - start_roi->y;
}

/*
	EXPLANATION
		Adds second given figure's actions linkedlist to the end of the first figure's actions linkedlist.
		Thus any action taking place in second figure will take place in the first too.

	PARAMETERS
		fig1:	First figure.
		fig2:	Second figure.
*/
void append_figures(Figure * fig1, Figure * fig2){
	get_last_action(fig1)->next = fig2->actions;
}

/*
	EXPLANATION
		Reads each action taking place in given figure then exports corresponding eps file to given file path.

	PARAMETERS
		fig:		The figure which is read and exported as an eps file.
		file_name:	Actually is the file path, but can be referred as file name. 
*/
void export_eps(Figure * fig, char * file_name){

	//	Open the file
	FILE * file;
	file = fopen(file_name, "w");

	//	Writing obligatory text
	fprintf(file, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(file, "%%%%BoundingBox: %lf %lf %lf %lf\n", fig->llx, fig->lly,	//	Specifying the boundaries of the figure
														fig->urx, fig->ury);
	fprintf(file, "/Times-Roman findfont\n3 scalefont\nsetfont\n");			//	For the texts when binary tree values are written

	FigureActionNode * curr_action = fig->actions;	//	Get actions linkedlist

	while(curr_action!=NULL){	//	Untill there's no action

		switch(curr_action->action_type){	//	Check the action type and write corresponding values and text to eps file
			case LINETO:
				fprintf(file, "%lf %lf lineto\n", curr_action->value.point->x, curr_action->value.point->y);
				break;
			case MOVETO:
				fprintf(file, "%lf %lf moveto\n", curr_action->value.point->x, curr_action->value.point->y);
				break;
			case CLOSE_PATH:
				fprintf(file, "closepath\n");
				break;
			case SET_THICKNESS:
				fprintf(file, "%lf setlinewidth\n", curr_action->value.thickness);
				break;
			case STROKE:
				fprintf(file, "stroke\n");
				break;
			case SET_COLOR:
				fprintf(file, "%lf %lf %lf setrgbcolor\n", curr_action->value.color->r, curr_action->value.color->g, curr_action->value.color->b);
				break;
			case WRITE:
				fprintf(file, "(%d) show\n", curr_action->value.number);
				break;
		}

		curr_action = curr_action->next;	// Get to next action
	}

	fclose(file);
}

