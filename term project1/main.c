#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "term_project.h"

double ff(double x){
	return x*x;
}

int main(){
	int i, j;
	srand(time(0));

	Figure * fig1 = start_figure(200, 200);
	Figure * fig2 = start_figure(500, 200);
	Figure * fig3 = start_figure(1360, 200);

	set_thickness_resolution(fig1, 1, 1);
	draw_ellipse(fig1, create_point(-31, 69), create_point(50, 10));

	Color * ec = create_color(255/255.0f,192/255.0f,203/255.0f);
	set_thickness_resolution(fig2, 1, 1);
	set_color(fig2, ec);
	double r = 10;

	Point2D * centrep = create_point(2*r-100, r-100);
	Point2D * wh = create_point(4*r, 2*r);

	for(j = 0; j < 4; j++){
		for(i = 0; i < 5; i++){

			draw_ellipse(fig2, centrep, wh);
			centrep->x += wh->x;
			fig2->resolution++;
		}
		centrep->y += wh->y;
		centrep->x = 2*r-100;
	}


	set_thickness_resolution(fig1, 0.5, 0.4);
	draw_fx(fig1, ff, -69, 31, 0.5);

	Tree * root = create_tree_node(500, NULL, NULL);
	int intl[5];

	for (i = 0; i < 10; ++i)
	{
		append_tree(root, rand_range_int(0, 999));
	}

	set_thickness_resolution(fig3, 0.5, 1);
	draw_binary_tree(fig3, root);

	export_eps(fig1, "1) elips ve x^2 grafiği.eps");
	export_eps(fig2, "2) çözünürlük değişimi ile elipsler.eps");
	export_eps(fig3, "3) rastgele sayıların eşitsizliği binary tree.eps");

	scale_figure(fig1, 5, 1);
	append_figures(fig1, fig2);
	export_eps(fig1, "4) ilk iki figürin birleşmişi ilk figür yatay genişletilmiş.eps");

	append_figures(fig3, fig1);
	resize_figure(fig3, create_point(-150, -150), create_point(150, 150));
	export_eps(fig3, "5) bütün figürlerin en son hali birleşmişi ama sınırlar farklı");

	return 0;
}