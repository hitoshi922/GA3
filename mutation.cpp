#include "Header.h"

void uniform_mutation(individual* B, int chrome);
void PBM(individual* B, int chrome);
void STL_mutation1(individual* B, int chrome);
void STL_mutation2(individual* B, int chrome);


void mutation(individual* B) {
	int i;
	for (i = 0; i < CHROM; i++) {
		switch (MUTATION[i]) {
		case 0:
			break;
		case 1:
			uniform_mutation(B, i);
			break;
		case 2:
			PBM(B, i);
			break;
		case 100:
			STL_mutation1(B, i);
			break;
		case 101:
			STL_mutation2(B, i);
			break;
		default:
			printf("mutation : check MUTATION value.\n");
			exit(1);
		}
	}


}

void uniform_mutation(individual* B, int chrome) {
	int i, j;
	double n;
	for (i = 0; i < POP; i++) {
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
void PBM(individual* B, int chrome) {
	int i, j;
	double nm = 20;
	double temp;
	double delta_l;
	double delta_u;
	double delta_q;
	double u;
	double uc;
	double ud;

	for (j = 0; j < POP; j++) {
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


void STL_mutation1(individual* B, int chrome) {
	int i, j;
	double n;
	int sel;
	int temp;
	int arrnum;
	arrnum = sizeof discrete / sizeof(int);
	for (i = 0; i < POP; i++) {
		for (j = 0; j < 9; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				do {
					sel = rand() % arrnum;
					temp = discrete[sel];
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


void STL_mutation2(individual* B, int chrome) {
	int i, j;
	double delta_L;
	double delta_ot;
	double n;
	double temp;
	for (i = 0; i < POP; i++) {
		for (j = 0; j < 3; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				delta_ot = delta_L / 2;
				B[i].X[0][chrome] = B[i].X[0][chrome] + delta_ot;
				B[i].X[1][chrome] = B[i].X[1][chrome] + delta_ot;
				B[i].X[2][chrome] = B[i].X[2][chrome] + delta_ot;
				B[i].X[j][chrome] = B[i].X[j][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}
		for (j; j < 6; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				delta_ot = delta_L / 2;
				B[i].X[3][chrome] = B[i].X[3][chrome] + delta_ot;
				B[i].X[4][chrome] = B[i].X[4][chrome] + delta_ot;
				B[i].X[5][chrome] = B[i].X[5][chrome] + delta_ot;
				B[i].X[j][chrome] = B[i].X[j][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}

		for (j; j < DIM[chrome]; j++) {
			n = (double)rand() / RAND_MAX;
			if (n < MUTATION_RATE[chrome]) {
				temp = 1.0 + (double)rand() / RAND_MAX * (B[i].X[j][chrome] - 1.0);
				delta_L = B[i].X[j][chrome] - temp;
				delta_ot = delta_L / 2;
				B[i].X[6][chrome] = B[i].X[6][chrome] + delta_ot;
				B[i].X[7][chrome] = B[i].X[7][chrome] + delta_ot;
				B[i].X[8][chrome] = B[i].X[8][chrome] + delta_ot;
				B[i].X[j][chrome] = B[i].X[j][chrome] - delta_ot - delta_L;
				B[i].eval_tag = 0; //–¢•]‰¿ˆµ‚¢
			}
		}
	}
}
