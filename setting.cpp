#include "Header.h"
void bound_preset();
void bound_check();
void secure_check();

void setting() {
	bound_preset(); //上下限仮設定

	if (EVALUATION == 1) {
		experiment_setting();
	}
	else {
		benchmark_setting();
	}

	bound_check();
	secure_check();
}


//*******実験用設定**********
void experiment_setting() {
	int i, j;
	if (NETLIST == 0) {
		SEGMENT = 10;
		CNT_SEGMENT[0] = 0;
		CNT_SEGMENT[1] = 5;
		CNT_SEGMENT[2] = 9;
		CNT_SEGMENT[3] = SEGMENT;
		CHROM = 1;
	}
	else if (NETLIST == 1) {
		SEGMENT = 30;
		CNT_SEGMENT[0] = 0;
		CNT_SEGMENT[1] = 15;
		CNT_SEGMENT[2] = 27;
		CNT_SEGMENT[3] = SEGMENT;
		CHROM = 1;
	}
	else if (NETLIST == 10) {
		SEGMENT = 10;
		CNT_SEGMENT[0] = 0;
		CNT_SEGMENT[1] = 5;
		CNT_SEGMENT[2] = 9;
		CNT_SEGMENT[3] = SEGMENT;
		DIM[1] = SEGMENT;
		CHROM = 2;
		for (i = 0; i < DIM[1]; i++) {
			upper_bound[i][1] = 100;
			lower_bound[i][1] = 50;
		}
	}
	DIM[0] = SEGMENT + 1; //セグメント数+抵抗数
	//DIM[1] = SEGMENT;
	if (ALGO == 10) {
		OBJ = 3;
	}
	else {
		OBJ = 1;
	}

	QTY_SECTION = 3;

	SECTION_LENGTH[0] = 280;
	SECTION_LENGTH[1] = 224;
	SECTION_LENGTH[2] = 56;
	L_NODE[0] = CNT_SEGMENT[1] + 1;
	L_NODE[1] = CNT_SEGMENT[2] + 1;
	
	for (i = 0; i < SEGMENT; i++) {
		upper_bound[i][0] = 120;
		lower_bound[i][0] = 20;
	}
	upper_bound[SEGMENT][0] = 100;
	lower_bound[SEGMENT][0] = 1;
	upper_bound[SEGMENT+1][0] = 100;
	lower_bound[SEGMENT+1][0] = 1;
	//for (i = 0; i < DIM[1]; i++) {
	//	upper_bound[i][1] = 100;
	//	lower_bound[i][1] = 1;
	//}
	
	MIN_OR_MAX[0] = 0;
}

//****ベンチマーク用設定（変更不要）******
void benchmark_setting() {
	DIM[0] = 10; //(n variable用)

	switch (EVALUATION) {
	case 10: //SCH
		DIM[0] = 1;
		OBJ = 2;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -1000;
			upper_bound[i][0] = 1000;
		}
		break;

	case 12: //POL
		DIM[0] = 2;
		OBJ = 2;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -M_PI;
			upper_bound[i][0] = M_PI;
		}
		break;
	case 13:
		DIM[0] = 3;
		OBJ = 2;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -5;
			upper_bound[i][0] = 5;
		}
		break;
	case 21: //ZDT1
		DIM[0] = 30;
		OBJ = 2;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = 0;
			upper_bound[i][0] = 1;
		}
		break;

	case 24: //ZDT4
		DIM[0] = 10;
		OBJ = 2;
		CHROM = 1;
		lower_bound[0][0] = 0;
		upper_bound[0][0] = 1;
		for (int i = 1; i < DIM[0]; i++) {
			lower_bound[i][0] = -5;
			upper_bound[i][0] = 5;
		}
		break;

	case 50: //rosenbrock(2 variables)
		DIM[0] = 2;
		OBJ = 1;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -2.048;
			upper_bound[i][0] = 2.048;
		}
		break;
	case 51: //sphare
		DIM[0] = 2;
		OBJ = 1;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -1000;
			upper_bound[i][0] = 1000;
		}
		break;
	case 60: //rosenbrock(n variables)
		if (DIM[0] <= 1) {
			printf("(DIM[0] = %d)rosenbrock function need 2 or more variables.\n", DIM[0]);
			exit(1);
		}
		OBJ = 1;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -2.048;
			upper_bound[i][0] = 2.048;
		}
		break;
	case 61:
		OBJ = 1;
		CHROM = 1;
		for (int i = 0; i < DIM[0]; i++) {
			lower_bound[i][0] = -1000;
			upper_bound[i][0] = 1000;
		}
		break;
	case 100:
		DIM[0] = 9;
		DIM[1] = 9;
		OBJ = 1;
		CHROM = 2;
		for (int i = 0; i < CHROM; i++) {
			for (int j = 0; j < DIM[i]; j++) {
				lower_bound[j][i] = 0;
				upper_bound[j][i] = 1000;
			}
		}
		break;
	default:
		printf("benchmark_setting: check EVALUATION value.\n");
		exit(1);
	}
}


//上限下限仮設定
void bound_preset() {

	for (int i = 0; i < CHROM; i++) {
		for (int j = 0; j < DIM[i]; j++) {
			lower_bound[j][i] = DBL_MIN;
			upper_bound[j][i] = DBL_MAX;
		}
	}

}

//上限下限の正常判断
void bound_check() {

	for (int i = 0; i < CHROM; i++) {
		for (int j = 0; j < DIM[i]; j++) {
			if (upper_bound[j][i] < lower_bound[j][i]) {
				printf("bound_check : boundary of X[%d][%d] is invalid.\n", j, i);
				exit(1);
			}
		}
	}
}


void secure_check() {
	for (int i = 0; i < CHROM; i++) {
		if (DIM[i] > DIM_SEC) {
			printf("secure_check : DIM[%d] is larger than DIM_SEC.\n", i);
			exit(1);
		}
	}
	if (OBJ > OBJ_SEC) {
		printf("secure_check : OBJ is larger than OBJ_SEC.\n");
		exit(1);
	}
}





//ファイルから設定を読み込む
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