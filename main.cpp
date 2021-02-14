#include"Header.h"

int main(void) {

	int it = 0; //ŒJ‚è•Ô‚µ
	clock_t start, end;
	double ex_time;

	do {
		Allclear_POP(P, POP);
		Allclear_POP(Q, POP);
		Allclear_POP(R, POP + POP);
		start = clock();
		setting(); //–â‘èİ’è‚Ì“Ç‚İ‚İ
		random_seed(); //—”‰Šú’l
		algorithm_junction(); //Às•”•ª
		end = clock();
		ex_time = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.2f•b‚©‚©‚è‚Ü‚µ‚½\n", ex_time);
		output_result(ex_time, GEN);
		it++;
	} while (it < TRIAL);

	return 0;
}



