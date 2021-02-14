#include"Header.h"

void output_result(double ex_time, int gen) {
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
		fprintf(fp2, "ALGO,%d\n", ALGO);
		fprintf(fp2, "POPULATION,%d\n", POP);
		fprintf(fp2, "GEN,%d\n", gen);
		if (CROSSOVER[0] == 10) {
			fprintf(fp2, "CROSSOVER1,BLX_a\n");
		}
		else if (CROSSOVER[0] == 11) {
			fprintf(fp2, "CROSSOVER1,SBX\n");
		}
		else if (CROSSOVER[0] == 12) {
			fprintf(fp2, "CROSSOVER1,SPX\n");
		}
		else {
			fprintf(fp2, "CROSSOVER1,%d\n", CROSSOVER[0]);
		}
		fprintf(fp2, "MUTATION_RATE1,%f\n", MUTATION_RATE[0]);


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

}