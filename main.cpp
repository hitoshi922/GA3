#include"Header.h"

int main(void) {

	int it = 0; //�J��Ԃ�
	clock_t start, end;
	double ex_time;

	do {
		Allclear_POP(P, POP);
		Allclear_POP(Q, POP);
		Allclear_POP(R, POP + POP);
		start = clock();
		setting(); //���ݒ�̓ǂݍ���
		random_seed(); //���������l
		algorithm_junction(); //���s����
		end = clock();
		ex_time = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.2f�b������܂���\n", ex_time);
		output_result(ex_time, GEN);
		it++;
	} while (it < TRIAL);

	return 0;
}



