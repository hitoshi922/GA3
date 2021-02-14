#include"Header.h"

//分岐
void init_ind(individual* ind, int arr) {
	if (EVALUATION == 1) {
		initialize3(ind, arr);
	}
	else {
		initialize(ind, arr);
	}

}

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

	int arrnum = sizeof designed / sizeof(int);
	for (i = 0; i < arr; i++) {
		///Zの初期化
		//Zを連続値で扱う場合
		if (TYPE == 0) {
			for (j = 0; j < DIM[0]; j++) {
				A[i].X[j][0] = lower_bound[j][0] + (double)rand() / RAND_MAX * (upper_bound[j][0] - lower_bound[j][0]);
			}
		}
		//離散値
		else if (TYPE == 1) {
			for (j = 0; j < DIM[0]; j++) {
				A[i].X[j][0] = lower_bound[j][0] + (double)rand() / RAND_MAX * (upper_bound[j][0] - lower_bound[j][0]);
				if (TYPE == 1) {
					A[i].X[j][0] = round(A[i].X[j][0]); //四捨五入
				}
			}
		}
		//指定された値
		else if (TYPE == 2) {
			for (j = 0; j < SEGMENT; j++) {
				temp = rand() % arrnum;
				A[i].X[j][0] = designed[temp];
			}
			for (j; j < SEGMENT + 2; j++) {
				A[i].X[j][0] = (double)(rand() % 99 + 1);
			}
		}
		else {
			printf("TYPE value is invalid.\n");
			exit(1);
		}



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


//kari
void initialize_read(individual* ind, int arr) {
	FILE* fp1;
	errno_t error;
	int qty_ind = 4;
	int i;
	double y;

	if (qty_ind > POP) {
		printf("error check initialize_read\n");
		exit(1);
	}

	error = fopen_s(&fp1, "D:/HITOSHI/Documents/final(p)100(g)100.csv", "r");
	if (error != 0) {
		exit(1);
	}

	for (i = 0; i < qty_ind; i++) {
		fscanf_s(fp1, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &ind[i].X[0][0], &ind[i].X[1][0], &ind[i].X[2][0], &ind[i].X[3][0], &ind[i].X[4][0], &ind[i].X[5][0], &ind[i].X[6][0], &ind[i].X[7][0], &ind[i].X[8][0], &ind[i].X[9][0], &ind[i].X[10][0], &ind[i].X[11][0], &ind[i].X[0][1], &ind[i].X[1][1], &ind[i].X[2][1], &ind[i].X[3][1], &ind[i].X[4][1], &ind[i].X[5][1], &ind[i].X[6][1], &ind[i].X[7][1], &ind[i].X[8][1], &ind[i].X[9][1]);
	}
	int j;
	int temp;
	double sum;

	int arrnum = sizeof designed / sizeof(int);
	for (i = qty_ind; i < arr; i++) {
		///Zの初期化
		//Zを連続値で扱う場合
		if (TYPE == 0) {
			for (j = 0; j < DIM[0]; j++) {
				ind[i].X[j][0] = lower_bound[j][0] + (double)rand() / RAND_MAX * (upper_bound[j][0] - lower_bound[j][0]);
			}
		}
		//Zを離散値で扱う場合
		else if (TYPE == 1) {
			for (j = 0; j < SEGMENT; j++) {
				temp = rand() % arrnum;
				ind[i].X[j][0] = designed[temp];
			}
			for (j; j < SEGMENT + 2; j++) {
				ind[i].X[j][0] = (double)(rand() % 99 + 1);
			}
		}
		else {
			printf("TYPE value is invalid.\n");
			exit(1);
		}

		///Tdの初期化
		for (j = 0; j < DIM[1]; j++) {
			ind[i].X[j][1] = lower_bound[j][1] + (double)rand() / RAND_MAX * (upper_bound[j][1] - lower_bound[j][1]);
		}
		//セクション1
		sum = 0;
		for (j = 0; j < L_NODE[0] - 1; j++) {
			sum += ind[i].X[j][1];
		}
		for (j = 0; j < L_NODE[0] - 1; j++) {
			ind[i].X[j][1] = ind[i].X[j][1] / sum * SECTION_LENGTH[0];
		}
		//セクション2
		sum = 0;
		for (j = L_NODE[0] - 1; j < L_NODE[1] - 1; j++) {
			sum += ind[i].X[j][1];
		}
		for (j = L_NODE[0] - 1; j < L_NODE[1] - 1; j++) {
			ind[i].X[j][1] = ind[i].X[j][1] / sum * SECTION_LENGTH[1];
		}
		//セクション3
		sum = 0;
		for (j = L_NODE[1] - 1; j < SEGMENT; j++) {
			sum += ind[i].X[j][1];
		}
		for (j = L_NODE[1] - 1; j < SEGMENT; j++) {
			ind[i].X[j][1] = ind[i].X[j][1] / sum * SECTION_LENGTH[2];
		}

	}

	fclose(fp1);

}





