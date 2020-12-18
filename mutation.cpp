#include "Header.h"

void uniform_mutation(individual* B,int arr, int chrome);
void PBM(individual* B,int arr, int chrome);
void STL_mutation1(individual* B,int arr, int chrome);
void STL_mutation2(individual* B,int arr, int chrome);


void mutation(individual* B, int arr) {
	int i;
	for (i = 0; i < CHROM; i++) {
		switch (MUTATION[i]) {
		case 0:
			break;
		case 1:
			uniform_mutation(B, arr, i);
			break;
		case 2:
			PBM(B, arr, i);
			break;
		case 100:
			STL_mutation1(B, arr, i);
			break;
		case 101:
			STL_mutation2(B, arr, i);
			break;
		default:
			printf("mutation : check MUTATION value.\n");
			exit(1);
		}
	}

	//—£ŽU’lˆ—
	if (TYPE == 1) {
		for (int j = 0; j < arr; j++) {
			for (int k = 0; k < DIM[0]; k++) {
				B[j].X[k][0] = round(B[j].X[k][0]);
			}
		}
	}

}

void uniform_mutation(individual* B, int arr, int chrome) {
	int i, j;
	double n;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < DIM[chrome]; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				B[i].X[j][chrome] = lower_bound[j][chrome] + (double)rand() / RAND_MAX * (upper_bound[j][chrome] - lower_bound[j][chrome]);
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}
	}
}

//parameter-based mutation
void PBM(individual* B,int arr, int chrome) {
	int i, j;
	double nm = 20;
	double temp;
	double delta_l;
	double delta_u;
	double delta_q;
	double u;
	double uc;
	double ud;

	for (j = 0; j < arr; j++) {
		//“Ë‘R•ÏˆÙ—¦‚ÉŠî‚Ã‚¢‚Ä•ÏˆÙ‚·‚é‚©”Û‚©
		ud = (double)rand() / RAND_MAX;
		if (ud < MUTATION_RATE[chrome]) {
			for (i = 0; i < DIM[chrome]; i++) {
				//ƒxƒNƒgƒ‹—v‘f‚²‚Æ‚É1/n‚ÌŠm—¦‚Å•ÏˆÙ
				uc = (double)rand() / RAND_MAX;
				if (uc < 1.0 / DIM[chrome]) {
					u = (double)rand() / RAND_MAX;
					//ˆÈ‰º—Ç‚­‚í‚©‚ç‚È‚¢Šm—¦–§“x•ª•z‚É]‚Á‚Ä—v‘f‚Ì’l‚ª•Ï‰»
					delta_l = (B[j].X[i][chrome] - lower_bound[i][chrome]) / (upper_bound[i][chrome] - lower_bound[i][chrome]);
					delta_u = (upper_bound[i][chrome] - B[j].X[i][chrome]) / (upper_bound[i][chrome] - lower_bound[i][chrome]);
					if (u <= 0.5) {
						temp = 2.0 * u + (1.0 - 2.0 * u) * pow((1.0 - delta_l), nm + 1.0);
						delta_q = pow(temp, 1.0 / (nm + 1.0)) - 1.0;
					}
					else {
						temp = 2.0 * (1.0 - u) + (2.0 * u - 1.0) * pow(1.0 - delta_u, nm + 1.0);
						delta_q = 1.0 - pow(temp, 1.0 / (nm + 1.0));
					}
					B[j].X[i][chrome] = B[j].X[i][chrome] + delta_q * (upper_bound[i][chrome] - lower_bound[i][chrome]);
				}
			}
		}
	}

}


void STL_mutation1(individual* B,int arr, int chrome) {
	int i, j;
	double n;
	int sel;
	int temp;
	int arrnum;
	arrnum = sizeof designed / sizeof(int);
	for (i = 0; i < arr; i++) {
		for (j = 0; j < 9; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				do {
					sel = rand() % arrnum;
					temp = designed[sel];
				} while (temp == B[i].X[j][chrome]);
				B[i].X[j][chrome] = temp;


				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}
		for (j; j < 11; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				B[i].X[j][chrome] = (double)(rand() % 99 + 1);
			}
		}
	}
}


void STL_mutation2(individual* B,int arr, int chrome) {
	int i, j, k;
	double delta_L;
	double delta_ot;
	double n;
	double temp;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < CNT_SEGMENT[1] - CNT_SEGMENT[0]; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j + CNT_SEGMENT[0]][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				delta_ot = delta_L / ((double)CNT_SEGMENT[1] - (double)CNT_SEGMENT[0] - 1);
				for (k = 0; k < CNT_SEGMENT[1] - CNT_SEGMENT[0]; k++) {
					B[i].X[k + CNT_SEGMENT[0]][chrome] = B[i].X[k + CNT_SEGMENT[0]][chrome] + delta_ot;
				}
				B[i].X[j + CNT_SEGMENT[0]][chrome] = B[i].X[j + CNT_SEGMENT[0]][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}

		for (j; j < L_NODE[1] - 1; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				delta_ot = delta_L / (L_NODE[1] - L_NODE[0] - 1);
				for (k = L_NODE[0] - 1; k < L_NODE[1] - 1; k++) {
					B[i].X[k][chrome] = B[i].X[k][chrome] + delta_ot;
				}
				B[i].X[j][chrome] = B[i].X[j][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}

		for (j; j < DIM[chrome]; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				//—ëœŽZ‰ñ”ð
				if (SEGMENT != L_NODE[1]) {
					delta_ot = delta_L / (SEGMENT - L_NODE[1]);
				}
				else {
					delta_ot = 0;
					delta_L = 0;
				}

				for (k = L_NODE[1] - 1; k < SEGMENT; k++) {
					B[i].X[k][chrome] = B[i].X[k][chrome] + delta_ot;
				}
				B[i].X[j][chrome] = B[i].X[j][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}


		//”O‚Ì‚½‚ß’·‚³‚ª¬‚³‚­‚È‚è‚·‚¬‚Ä‚È‚¢‚©ƒ`ƒFƒbƒN
		double tempX[DIM_SEC];
		for (int n = 0; n < DIM[1]; n++) {
			tempX[n] = B[i].X[n][1];
		}
		small_segment_handring(tempX);
		for (int n = 0; n < DIM[1]; n++) {
			B[i].X[n][1] = tempX[n];
		}
	}



}
