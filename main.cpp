#include"Header.h"

int main(void) {

	int it = 0; //繰り返し
	clock_t start, end;
	double ex_time;

	do {
		Allclear_POP(P, POP);
		Allclear_POP(Q, POP);
		Allclear_POP(R, POP + POP);
		start = clock();
		setting(); //問題設定の読み込み
		random_seed(); //乱数初期値
		algorithm_junction(); //実行部分
		end = clock();
		ex_time = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.2f秒かかりました\n", ex_time);
		output_result(ex_time, GEN);
		it++;
	} while (it < TRIAL);

	return 0;
}



