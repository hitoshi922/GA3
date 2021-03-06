#include"Header.h"

void eval_junction(double X[][CHROM_SEC], double* f);
void test_sphare(double X[][CHROM_SEC], double* f);

void rosenbrock(double X[][CHROM_SEC], double* f);
void rosenbrock_n(double X[][CHROM_SEC], double* f);
void sphare(double X[][CHROM_SEC], double* f);
void SCH(double X[][CHROM_SEC], double* f);
void ZDT1(double X[][CHROM_SEC], double* f);
void ZDT4(double X[][CHROM_SEC], double* f);
void POL(double X[][CHROM_SEC], double* f);
void KUR(double X[][CHROM_SEC], double* f);
double assign_eval(double* fitness);
void normalize();
void normalize2();
void copy_f_to_fitness(individual* A, int arr);
void negative_fitness_translation(individual* A, int arr);

void culc_fitness(double* f, double* fitness);
void culc_improvant_rate(double* f, double* fitness);


void sort2(individual* ind, int arr) { //fitness専用降順ソート
	int i;
	int j;
	individual temp;

	for (i = 0; i < arr; i++) {
		for (j = i; j < arr; j++) {
			if (ind[i].fitness[0] < ind[j].fitness[0]) {
				temp = ind[i];
				ind[i] = ind[j];
				ind[j] = temp;
			}
		}
	}
}


void evaluation(individual* A, int arr) {
	int i, j;

	//実験モード
	if (EVALUATION == 1) {
		//make_netlist_test1(A, arr);
		//make_netlist3(A, arr);//安定版
		if (NETLIST == 0) {
			make_netlist5(A, arr);
		}
		else if (NETLIST == 1) {
			make_netlist_500MHz(A, arr);
		}
		else if (NETLIST == 10) {
			make_netlist_pulseVL(A, arr);
		}
		else {
			printf("chech NETLIST\n");
			exit(1);
		}
		sim_STL(A, arr);
		//system("timeout 10");
	}


	for (i = 0; i < arr; i++) {
		if (A[i].eval_tag == 0) {
			if (EVALUATION == 1) {
				//A[i].f[0] = get_result(i);
				A[i].f[0] = get_result2(i) * 10e9; //make_netlist

				if (ALGO == 10) {
					A[i].f[1] = getwidth(A[i].X);
					//A[i].f[1] = get_w_range(A[i].X);
					//A[i].f[1] = getsegments(A[i].X);
					//A[i].f[1] = get_around75(A[i].X);
				}

			}
			else {
				eval_junction(A[i].X, A[i].f);
			}
			A[i].eval_tag = 1; //評価済みに更新
		}
	}

	copy_f_to_fitness(A, arr);
	negative_fitness_translation(A, arr);


	for (i = 0; i < arr; i++) {
		if (EVALUATION == 1) {
			for (i = 0; i < POP; i++) {
				culc_improvant_rate(A[i].f, A[i].fitness);
			}
		}
		else {
			for (i = 0; i < POP; i++) {
				culc_fitness(A[i].f, A[i].fitness);
			}
		}
	}



	//normalize();
	//normalize2();
	for (i = 0; i < POP; i++) {
		A[i].evaluation = assign_eval(A[i].fitness);
	}

}

void eval_junction(double X[][CHROM_SEC], double* f) {
	switch (EVALUATION)
	{
	case 10:
		SCH(X, f);
		break;
	case 12:
		POL(X, f);
		break;
	case 13:
		KUR(X, f);
		break;
	case 21:
		ZDT1(X, f);
		break;
	case 24:
		ZDT4(X, f);
		break;
	case 50:
		rosenbrock(X, f);
		break;
	case 51:
	case 61:
		sphare(X, f);
		break;
	case 60:
		rosenbrock_n(X, f);
		break;
	case 100://test
		test_sphare(X, f);
		break;
	default:
		printf("eval_junction : EVALUATION is invalid value.\n");
		exit(1);
	}



}



double assign_eval(double* fitness) {
	int i;
	double evaluation;
	evaluation = 0;
	if (EVALUATION == 1) {
		evaluation = fitness[0];
	}
	else {
		if (OBJ == 1) {
			evaluation = fitness[0];
		}
		else if (OBJ >= 2) {
			for (i = 0; i < OBJ; i++) {
				//evaluation += w[i] * fitness[i];
			}
		}
		else {
			printf("assign_value: OBJ is invalid value.\n");
			exit(1);
		}
	}

	return evaluation;
}

void test_sphare(double X[][CHROM_SEC], double* f) {
	int i, j;
	f[0] = 0;

	for (i = 0; i < CHROM; i++) {
		for (j = 0; j < DIM[i]; j++) {
			f[0] += X[j][i] * X[j][i];
		}
	}
}


void rosenbrock(double X[][CHROM_SEC], double* f) {
	f[0] = (1.0 - X[0][0]) * (1.0 - X[0][0]) + 100.0 * (X[1][0] - X[0][0] * X[0][0]) * (X[1][0] - X[0][0] * X[0][0]);
}

void rosenbrock_n(double X[][CHROM_SEC], double* f) {
	int i;
	f[0] = 0;
	for (i = 0; i < DIM[0] - 1; i++) {
		f[0] += 100 * pow((X[i + 1][0] - pow(X[i][0], 2)), 2) + pow((X[i][0] - 1.0), 2);
	}
}

void sphare(double X[][CHROM_SEC], double* f) {
	int i;
	f[0] = 0;
	for (i = 0; i < DIM[0]; i++) {
		f[0] += X[i][0] * X[i][0];
	}
}


void SCH(double X[][CHROM_SEC], double* f) {
	f[0] = X[0][0] * X[0][0];
	f[1] = (X[0][0] - 2.0) * (X[0][0] - 2.0);
}

void POL(double X[][CHROM_SEC], double* f) {
	double f1, f2;
	double A1, A2, B1, B2;
	A1 = 0.5 * sin(1.0 * M_PI / 180) - 2.0 * cos(1.0 * M_PI / 180) + sin(2.0 * M_PI / 180) - 1.5 * cos(2.0 * M_PI / 180);
	A2 = 1.5 * sin(1.0 * M_PI / 180) - cos(1.0 * M_PI / 180) + 2.0 * sin(2.0 * M_PI / 180) - 0.5 * cos(2.0 * M_PI / 180);
	B1 = 0.5 * sin(X[0][0] * M_PI / 180) - 2.0 * cos(X[0][0] * M_PI / 180) + sin(X[1][0] * M_PI / 180) - 1.5 * cos(X[1][0] * M_PI / 180);
	B2 = 1.5 * sin(X[0][0] * M_PI / 180) - cos(X[0][0] * M_PI / 180) + 2.0 * sin(X[1][0] * M_PI / 180) - 0.5 * cos(X[1][0] * M_PI / 180);
	f1 = 1.0 + (A1 - B1) * (A1 - B1) + (A2 - B2) * (A2 - B2);
	f2 = (X[0][0] + 3.0) * (X[0][0] + 3.0) + (X[1][0] + 1.0) * (X[1][0] + 1.0);

	f[0] = f1;
	f[1] = f2;
}

void KUR(double X[][CHROM_SEC], double* f) {
	f[0] = 0;
	f[1] = 0;
	for (int i = 0; i < DIM[0] - 1; i++) {
		f[0] = f[0] - 10 * exp(-0.2 * sqrt(X[i][0] * X[i][0] + X[i + 1][0] * X[i + 1][0]));
	}
	for (int i = 0; i < DIM[0]; i++) {
		f[1] = f[1] + pow(fabs(X[i][0]), 0.8) + 5 * sin(pow(X[i][0], 3));
	}
}


void ZDT1(double X[][CHROM_SEC], double* f) {
	double g;
	double sigma = 0;
	f[0] = X[0][0];
	for (int i = 1; i < DIM[0]; i++) {
		sigma = sigma + X[i][0];
	}
	g = 1 + 9 * sigma / (DIM[0] - 1);
	f[1] = g * (1 - sqrt(X[0][0] / g));
}



void ZDT4(double X[][CHROM_SEC], double* f) {
	int i;
	double g, h;
	double temp = 0;
	for (i = 1; i < DIM[0]; i++) {
		temp += X[i][0] * X[i][0] - 10.0 * cos(4 * M_PI + X[i][0]);
	}
	f[0] = X[0][0];
	g = 1.0 + 10.0 * (DIM[0] - 1.0) + temp;
	h = 1.0 - sqrt(f[0] / g);
	f[1] = g * h;

}

void normalize() {
	int i, j;
	double total[OBJ_SEC];
	for (i = 0; i < OBJ; i++) {
		total[i] = 0;
		for (j = 0; j < POP; j++) {
			total[i] += P[j].fitness[i];
		}
		for (j = 0; j < POP; j++) {
			P[j].fitness[i] = P[j].fitness[i] / total[i];
		}
	}
}

void normalize2() {
	int i, j;
	double temp;
	for (i = 0; i < OBJ; i++) {
		temp = 0;
		for (j = 0; j < POP; j++) {
			if (temp < P[j].fitness[i]) {
				temp = P[j].fitness[i];
			}
		}
		for (j = 0; j < POP; j++) {
			P[j].fitness[i] = P[j].fitness[i] / temp;
		}
	}
}
void normalize3() {
	int i, j;
	double temp;
	for (i = 0; i < OBJ; i++) {
		temp = 0;
		for (j = 0; j < POP; j++) {
			if (temp > P[j].fitness[i]) {
				temp = P[j].fitness[i];
			}//負の値がないかのチェック
		}
	}
}

void copy_f_to_fitness(individual* A, int arr) {
	for (int i = 0; i < OBJ; i++) {
		for (int j = 0; j < arr; j++) {
			A[j].fitness[i] = A[j].f[i];
		}
	}
}


void negative_fitness_translation(individual* A, int arr) {
	double min;
	for (int j = 0; j < OBJ; j++) {
		min = 0;
		for (int i = 0; i < arr; i++) {
			if (min > A[i].fitness[j]) {
				min = A[i].fitness[j];
			}
		}
		if (min < 0) {
			for (int i = 0; i < arr; i++) {
				A[i].fitness[j] = A[i].fitness[j] - min;
			}
		}
	}
}


void culc_fitness(double* f, double* fitness) {
	for (int i = 0; i < OBJ; i++)
	{
		if (MIN_OR_MAX[i] == 0) { //最小化なら適応度は逆数
			fitness[i] = 1.0 / (fitness[i] + DBL_EPSILON);
		}
		else {
			fitness[i] = fitness[i];
		}
	}
}

void culc_improvant_rate(double* f, double* fitness) {
	//fitness[0] = 3.53819e-8  / f[0]; //make_netlist5
	if (NETLIST == 0) {
		fitness[0] = (5.29322e-9 + 5.30237e-9) * 10e9 / fitness[0];
	}
	else if (NETLIST == 1) {
		fitness[0] = (1.82903e-9 + 5.16146e-9) * 10e9 / fitness[0];
	}
	else if (NETLIST == 10) {
		fitness[0] = (5.29322e-9 + 5.30237e-9) * 10e9 / fitness[0];
	}
	fitness[1] = fitness[1];
}



