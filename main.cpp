#include"Header.h"
void read_setting();
void random_weight();
void fix_weight();
void output_result(double ex_time);

void make_SVsig(char* pulse) {
	int i;
	char text[100] = "PULSE(0 6.6 SV 20p 20p 8n)";
	double freq = 300e6;
	double amp = 3.3;
	double Tdelay = 2;
	double r = 1 / freq;
	double endtm = 20;
	double check;
	int SV[6];

	sprintf_s(pulse, 100, "PULSE(0 %.1f %.1fn 20p 20p 8n)", amp, Tdelay);

}



int main(void) {
	//****
	//char pwsp[100];
	//make_SVsig(pwsp);
	//****


	int it = 0;
	clock_t start, end;
	double ex_time;

	do {
		clear_POP(P, POP);
		clear_POP(Q, POP);
		clear_POP(R, POP + POP);
		elite[0].f[0] = DBL_MAX;
		elite[1].f[0] = DBL_MAX;
		start = clock();
		setting(); //問題設定の読み込み
		random_seed(); //乱数初期値


		if (ALGO == 0) {
			basic_GA();
		}
		else if (ALGO == 1) {
			MGG();
		}
		else if (ALGO == 10) {
			NSGA2();
		}
		else {
			printf("check ALGORITHM\n");
			exit(1);
		}
		end = clock();
		ex_time = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.2f秒かかりました\n", ex_time);
		output_result(ex_time);
		it++;
	} while (it < TRIAL);
	//*******************
	for (int i = 0; i < POP; i++) {
		printf("ind[%d]\n", i);
		individual_info(P[i]);
	}
	//*******************

	return 0;
}

void read_setting() {
	FILE* fp;
	errno_t error;
	//int check;

	//test
	//char objective[64] = "#objective";
	/*char min_or_max[64] = "#min_or_max";*/

	error = fopen_s(&fp, "setting.txt", "r");
	if (error != 0) {
		printf("設定ファイルを開けませんでした");
		exit(1);
	}

	//find_header(fp, objective);
	//fscanf_s(fp, "%d", &OBJ);

	//find_header(fp, min_or_max);
	//for (int i = 0; i < OBJ; i++) {
	//	fscanf_s(fp, "%d", &MIN_OR_MAX[i]);
	//}

	fclose(fp);
}


void FWGA() {
	//crossover();
	//mutation();
	//evaluation();
}
//
//
//void SOGA() {
//	
//	innerGA();
//	//outerGA
//	crossover();
//	mutation();
//	evaluation();
//
//}
//
//void innerGA() {
//	double w[pop_inner][num_objective];
//	for (i = 0; i < 20; i++) {
//		w_evaluate(w);
//		crossover(w);
//		mutation(w);
//	}
//	choose_best_w();
//}
//
//void crossover() {
//	roulette();
//}

void initialize(individual* A, int arr) {
	int i, j, k;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < CHROM; j++) {
			for (k = 0; k < DIM[j]; k++) {
				A[i].X[k][j] = lower_bound[k][j] + (double)rand() / RAND_MAX * (upper_bound[k][j] - lower_bound[k][j]);
			}
		}
	}

}

void initialize2() {
	int i, j;
	int temp;
	double sum;

	for (i = 0; i < POP; i++) {
		for (j = 0; j < 9; j++) {
			temp = rand() % 3;
			if (temp == 0) {
				P[i].X[j][0] = 5;
			}
			else if (temp == 1) {
				P[i].X[j][0] = 25;
			}
			else if (temp == 2) {
				P[i].X[j][0] = 76;
			}
			else {
				exit(1);
			}
		}
		for (j; j < 11; j++) {
			P[i].X[j][0] = (double)(rand() % 99 + 1);
		}
		for (j = 0; j < DIM[1]; j++) {
			P[i].X[j][1] = lower_bound[j][1] + (double)rand() / RAND_MAX * (upper_bound[j][1] - lower_bound[j][1]);
		}
		sum = P[i].X[0][1] + P[i].X[1][1] + P[i].X[2][1];
		P[i].X[0][1] = P[i].X[0][1] / sum * 280;
		P[i].X[1][1] = P[i].X[1][1] / sum * 280;
		P[i].X[2][1] = P[i].X[2][1] / sum * 280;
		sum = P[i].X[3][1] + P[i].X[4][1] + P[i].X[5][1];
		P[i].X[3][1] = P[i].X[3][1] / sum * 224;
		P[i].X[4][1] = P[i].X[4][1] / sum * 224;
		P[i].X[5][1] = P[i].X[5][1] / sum * 224;
		sum = P[i].X[6][1] + P[i].X[7][1] + P[i].X[8][1];
		P[i].X[6][1] = P[i].X[6][1] / sum * 56;
		P[i].X[7][1] = P[i].X[7][1] / sum * 56;
		P[i].X[8][1] = P[i].X[8][1] / sum * 56;
	}
}

void initialize3(individual* A, int arr) {
	int i, j;
	int temp;
	double sum;

	int arrnum = sizeof discrete / sizeof(int);
	for (i = 0; i < arr; i++) {
		///Zの初期化
		//Zを連続値で扱う場合
		//if (TYPE == 0) {
		for (j = 0; j < DIM[0]; j++) {
			A[i].X[j][0] = lower_bound[j][0] + (double)rand() / RAND_MAX * (upper_bound[j][0] - lower_bound[j][0]);
			/////
			if (TYPE == 1) {
				A[i].X[j][0] = round(A[i].X[j][0]);
			}
			//////
		}

		//}
		//Zを離散値で扱う場合
		//else if (TYPE == 1) {
		//	for (j = 0; j < SEGMENT; j++) {
		//		temp = rand() % arrnum;
		//		A[i].X[j][0] = discrete[temp];
		//	}
		//	for (j; j < SEGMENT + 2; j++) {
		//		A[i].X[j][0] = (double)(rand() % 99 + 1);
		//	}
		//}
	/*	else {
			printf("TYPE value is invalid.\n");
			exit(1);
		}*/



		///Tdの初期化
		int sub_seg0 = SECTION_LENGTH[0] / 0.5;
		int sub_seg1 = SECTION_LENGTH[1] / 0.5;
		int sub_seg2 = SECTION_LENGTH[2] / 0.5;
	
		
		for (j = 0; j < DIM[1]; j++) {
			A[i].X[j][1] = lower_bound[j][1] + (double)rand() / RAND_MAX * (upper_bound[j][1] - lower_bound[j][1]);
		}
		//セクション1
		sum = 0;
		for (j = 0; j < L_NODE[0] - 1; j++) {
			sum += A[i].X[j][1];
		}
		for (j = 0; j < L_NODE[0] - 1; j++) {
			A[i].X[j][1] = A[i].X[j][1] / sum * SECTION_LENGTH[0];
		}
		//int arr[5];
		//divide_num(sub_seg0, arr, 5);

		//セクション2
		sum = 0;
		for (j = L_NODE[0] - 1; j < L_NODE[1] - 1; j++) {
			sum += A[i].X[j][1];
		}
		for (j = L_NODE[0] - 1; j < L_NODE[1] - 1; j++) {
			A[i].X[j][1] = A[i].X[j][1] / sum * SECTION_LENGTH[1];
		}
		//セクション3
		sum = 0;
		for (j = L_NODE[1] - 1; j < SEGMENT; j++) {
			sum += A[i].X[j][1];
		}
		for (j = L_NODE[1] - 1; j < SEGMENT; j++) {
			A[i].X[j][1] = A[i].X[j][1] / sum * SECTION_LENGTH[2];
		}

	}

}


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



void record(individual* ind, int arr, int cnt) {
	int i, j;
	double keep = 0;
	double ave = 0;
	for (i = 0; i < arr; i++) {
		if (keep < ind[i].evaluation) {
			keep = ind[i].evaluation;
			best_P[cnt] = ind[i];
		}
	}
	for (i = 0; i < arr; i++) {
		ave += ind[i].f[0];
	}
	ave = ave / (double)arr;
	gen_ave[cnt] = ave;
	sort(ind, arr);
	if (elite[0].f[0] > ind[0].f[0]) {
		elite[1] = elite[0];
		elite[0] = ind[0];
	}
	if (elite[1].f[0] > ind[1].f[0]) {
		elite[1] = ind[1];
	}

}

void output_result(double ex_time) {
	time_t timer;
	struct tm local;
	int i, j, k;
	char filename[100];
	char filename2[100];
	FILE* fp, * fp2;
	errno_t error, error2;
	errno_t errort;

	timer = time(NULL);
	errort = localtime_s(&local, &timer);

	if (errort != 0) {
		sprintf_s(filename, "result.txt");
	}
	else {
		sprintf_s(filename, "C:/ex_result/%02d%02d-%02d%02d[p]%d[g]%d.txt", local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, POP, GEN);
	}

	error = fopen_s(&fp, filename, "w");
	if (error != 0) {
		printf("結果ファイル.txtを開けませんでした。");
		exit(1);
	}
	else {
		if (errort != 0) {
			fprintf(fp, "TIME ERROR\n");
		}
		else {
			fprintf(fp, "%4d/", local.tm_year + 1900);
			fprintf(fp, "%2d/", local.tm_mon + 1);
			fprintf(fp, "%2d ", local.tm_mday);
			fprintf(fp, "%2d:", local.tm_hour);
			fprintf(fp, "%2d:", local.tm_min);
			fprintf(fp, "%2d\n", local.tm_sec);
		}
		fprintf(fp, "実行時間 : %.2fs\n", ex_time);
		fprintf(fp, "*******************************\n");
		if (ALGO == 0) {
			fprintf(fp, "ALGO : simpleGA\n");
		}
		else if (ALGO == 1) {
			fprintf(fp, "ALGO : MGG\n");
		}
		else if (ALGO == 10) {
			fprintf(fp, "ALGO : NSGA2\n");
		}
		else {
			fprintf(fp, "ALGO : %d\n", ALGO);
		}
		fprintf(fp, "MODE : %d\n", MODE);
		fprintf(fp, "EVALUATION : %d\n", EVALUATION);
		fprintf(fp, "POPULATION : %d\n", POP);
		fprintf(fp, "GENERATION : %d\n", GEN);
		for (i = 0; i < CHROM; i++) {
			fprintf(fp, "CROSSOVER_METHOD[%d] : %d\n", i, CROSSOVER[i]);
		}
		fprintf(fp, "CROSSOVER_RATE : %.3f\n", CROSSOVER_RATE);
		for (i = 0; i < CHROM; i++) {
			fprintf(fp, "MUTATION_METHOD[%d] : %d\n", i, MUTATION[i]);
		}
		for (i = 0; i < CHROM; i++) {
			fprintf(fp, "MUTATION_RATE[%d] : %.3f\n", i, MUTATION_RATE[i]);
		}
		fprintf(fp, "*******************************\n");



		for (j = 0; j < GEN; j++) {
			fprintf(fp, "gen[%d] ", j);
			for (k = 0; k < CHROM; k++) {
				for (i = 0; i < DIM[k]; i++) {
					fprintf(fp, "%f ", best_P[j].X[i][k]);
				}
			}
			for (i = 0; i < OBJ; i++) {
				fprintf(fp, "f[%d]=%f ", i, best_P[j].f[i]);
			}
			fprintf(fp, "eval=%f ave=%f\n", best_P[j].evaluation, gen_ave[j]);
		}
	}
	fclose(fp);

	//ここからcsvファイル出力
	sprintf_s(filename2, "C:/ex_result/final%02d%02d-%02d%02d[p]%d[g]%d.csv", local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, POP, GEN);
	error2 = fopen_s(&fp2, filename2, "w");
	if (error2 != 0) {
		printf("結果ファイル.csvを開けませんでした。");
		exit(1);
	}
	else {
		//見出し
		fprintf(fp2, "result,");
		for (i = 0; i < CHROM; i++) {
			for (j = 0; j < DIM[i]; j++) {
				fprintf(fp2, "X[%d][%d],", j, i);
			}
		}
		for (j = 0; j < OBJ; j++) {
			fprintf(fp2, "f[%d],", j);
		}
		fprintf(fp2, "eval\n");
		//データ
		for (j = 0; j < POP; j++) {
			fprintf(fp2, "P[%d],", j);
			for (k = 0; k < CHROM; k++) {
				for (i = 0; i < DIM[k]; i++) {
					fprintf(fp2, "%f,", P[j].X[i][k]);
				}
			}
			for (i = 0; i < OBJ; i++) {
				fprintf(fp2, "%f,", P[j].f[i]);
			}
			fprintf(fp2, "%f\n", P[j].evaluation);
		}
	}
	fclose(fp2);


}

void init_ind(individual* ind, int arr) {
	if (MODE == 0) {
		initialize(ind, arr);
	}
	if (MODE == 1) {
		initialize3(ind, arr);
	}
}


