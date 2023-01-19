#include <stdio.h>
#include "termp2.h"

int main(){

	int canvas_count;
	Canvas ** canvasses = run_commands(&canvas_count);

	int i;

	for(i = 0; i < canvas_count; i++){

		if(canvasses[i] != NULL) draw_canvas(canvasses[i]);
	}


	return 0;
}