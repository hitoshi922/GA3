#include"Header.h"

void random_weight() {
	double total;
	int i, j;
	for (i = 0; i < POP; i++) {
		total = 0;
		for (j = 0; j < OBJ; j++) {
			P[i].w[j] = (double)rand();
			total += P[i].w[j];
		}
		for (j = 0; j < OBJ; j++) {
			P[i].w[j] = P[i].w[j] / total;
		}
	}
}

void fix_weight() {
	int i, j;
	for (i = 0; i < POP; i++) {
		P[i].w[0] = 0.1;
		P[i].w[1] = 0.9;
	}
}