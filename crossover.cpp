#include"Header.h"


void parent_selection(individual* B, int* sel, int np);
void parent_selection2(individual* B, int* sel, int np);

void assign_parent(individual* B, double* parent1, double* parent2, int chrome);
void assign_parent2(individual* B, double* parent1, double* parent2, int cnt, int chrome);

void X_junction(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void single_point_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void two_point_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void uniform_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome);

void BLX_a(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void SBX(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void STL_BLX_a(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void STL_2PX(double* parent1, double* parent2, double* child1, double* child2, int chrome);
void STL_SX(double* parent1, double* parent2, double* child1, double* child2, int chrome);
int keep_nextP(int cnt, double* child1, double* child2, int chrome);
int recycle_select(individual* B, int cnt);

void generation_change(individual* B, int cnt);

//グローバル変数-------------------
static int n_parent = 2;
static int sel[2];
static int sel2[POP];
static int num_of_children;
static int num_of_recycle;
//---------------------------------


void crossover(individual* B) {
	int i;
	int cnt;
	int inc;
	double parent1[DIM_SEC];
	double parent2[DIM_SEC];
	double child1[DIM_SEC];
	double child2[DIM_SEC];

	num_of_children = (int)(POP * CROSSOVER_RATE);
	num_of_recycle = POP - num_of_children;

	cnt = 0;

	//parent_selection2(B, sel2, num_of_children); //全親重複なし選択

	do {
		parent_selection(B, sel, n_parent);
		for (i = 0; i < CHROM; i++) {
			assign_parent(B, parent1, parent2, i);
			//assign_parent2(B, parent1, parent2, cnt, i);//全親重複なし選択ver
			X_junction(parent1, parent2, child1, child2, i);
			inc = keep_nextP(cnt, child1, child2, i);
		}
		cnt = cnt + inc; //子個体分インクリメント
	} while (cnt < num_of_children);

	cnt = recycle_select(B, cnt);

	//crear_info()

	generation_change(B, cnt);
	//離散値のみ
	if (TYPE == 1) {
		for (int j = 0; j < cnt; j++) {
			for (int k = 0; k < DIM[0]; k++) {
				B[j].X[k][0] = round(B[j].X[k][0]);
			}
		}
	}

}

void MGG_crossover(individual* B, int* parent_select, int qty_family) {
	int i, j;
	int cnt;
	int inc = 0;

	double parent1[DIM_SEC];
	double parent2[DIM_SEC];
	double child1[DIM_SEC];
	double child2[DIM_SEC];

	cnt = 0;
	sel[0] = parent_select[0];
	sel[1] = parent_select[1];

	num_of_children = qty_family;

	//親個体のファミリーへの移し替え
	//nextP[POP]<number_of_childrenだとバグる、そのための強制停止
	if (num_of_children > POP) {
		printf("num_of_children > POP, check MGG_crossover\n");
		exit(1);
	}

	nextP[0] = B[sel[0]];
	nextP[1] = B[sel[1]];
	cnt = cnt + 2;
	//子個体の生成
	do {
		for (i = 0; i < CHROM; i++) {
			assign_parent(B, parent1, parent2, i);
			X_junction(parent1, parent2, child1, child2, i);
			inc = keep_nextP(cnt, child1, child2, i);
		}
		cnt = cnt + inc;
	} while (cnt < num_of_children);

	//mutation
	mutation(nextP, num_of_children);

	//全固体の評価
	evaluation(nextP, num_of_children);



}


void X_junction(double* parent1, double* parent2, double* child1, double* child2, int chrome) {
	switch (CROSSOVER[chrome])
	{
	case 0:
		single_point_crossover(parent1, parent2, child1, child2, chrome);
		break;
	case 1:
		two_point_crossover(parent1, parent2, child1, child2, chrome);
		break;
	case 2:
		uniform_crossover(parent1, parent2, child1, child2, chrome);
		break;
	case 10:
		BLX_a(parent1, parent2, child1, child2, chrome);
		break;
	case 11:
		SBX(parent1, parent2, child1, child2, chrome);
		break;
	case 100:
		STL_2PX(parent1, parent2, child1, child2, chrome);
		break;
	case 101:
		STL_SX(parent1, parent2, child1, child2, chrome);
		break;
	case 102:
		STL_BLX_a(parent1, parent2, child1, child2, chrome);
		break;
	default:
		printf("CROSSOVERの値が不正です。");
		exit(1);
	}
}


int keep_nextP(int cnt, double* child1, double* child2, int chrome) {
	int i;
	int inc = 0;
	for (i = 0; i < DIM[chrome]; i++) {
		nextP[cnt].X[i][chrome] = child1[i];
	}
	cnt++;
	inc++;
	if (cnt == num_of_children) {
		return inc;
	}
	for (i = 0; i < DIM[chrome]; i++) {
		nextP[cnt].X[i][chrome] = child2[i];
	}
	cnt++;
	inc++;
	return inc;
}


int recycle_select(individual* B, int cnt) {
	int i;
	int x[POP];
	int temp;
	non_restored_extract(POP, x, num_of_recycle);
	for (i = 0; i < num_of_recycle; i++) {
		temp = x[i];
		nextP[cnt] = B[temp];
		cnt++;
	}
	return cnt;
}

void generation_change(individual* B, int cnt) {
	int i;
	for (i = 0; i < cnt; i++) {
		B[i] = nextP[i];
	}
}

void parent_selection(individual* B, int* sel, int np) {
	switch (PSM) {
	case 0:
		non_restored_extract(POP, sel, np);
		break;
	case 1:
		tornament_select(B, POP, sel, np);
		break;
	case 2:
		roulette_select(B, POP, sel, np);
		break;
	case 10:
		crowding_tornament_select(B, POP, sel, np);
		break;
	default:
		printf("parent_selection: PSM is invalid value.\n");
		exit(1);
	}
}

void parent_selection2(individual* B, int* sel, int np) {
	non_restored_extract(POP, sel2, np);
}


void assign_parent(individual* B, double* parent1, double* parent2, int chrome) {
	int i;
	for (i = 0; i < DIM[chrome]; i++) {
		parent1[i] = B[sel[0]].X[i][chrome];
		parent2[i] = B[sel[1]].X[i][chrome];
	}
}

void assign_parent2(individual* B, double* parent1, double* parent2, int cnt, int chrome) {
	int i;
	for (i = 0; i < DIM[chrome]; i++) {
		parent1[i] = B[sel2[cnt]].X[i][chrome];
		parent2[i] = B[sel2[cnt + 1]].X[i][chrome];
	}
}

void BLX_a(double* parent1, double* parent2, double* child1, double* child2, int chrome) {

	double alpha = 0.3;

	int i;
	double dx[DIM_SEC];
	double min_x[DIM_SEC];
	double max_x[DIM_SEC];
	double min_cx[DIM_SEC];
	double max_cx[DIM_SEC];
	double u1, u2;


	for (i = 0; i < DIM[chrome]; i++) {
		dx[i] = fabs(parent1[i] - parent2[i]);
		min_x[i] = fmin(parent1[i], parent2[i]);
		max_x[i] = fmax(parent1[i], parent2[i]);
		min_cx[i] = min_x[i] - alpha * dx[i];
		max_cx[i] = max_x[i] + alpha * dx[i];
	}
	//制限
	for (i = 0; i < DIM[chrome]; i++) {
		if (min_cx[i] < lower_bound[i][chrome]) {
			min_cx[i] = lower_bound[i][chrome];
		}
		if (max_cx[i] > upper_bound[i][chrome]) {
			max_cx[i] = upper_bound[i][chrome];
		}
	}

	for (i = 0; i < DIM[chrome]; i++) {
		u1 = (double)rand() / RAND_MAX;
		u2 = (double)rand() / RAND_MAX;
		child1[i] = min_cx[i] + u1 * (max_cx[i] - min_cx[i]);
		child2[i] = min_cx[i] + u2 * (max_cx[i] - min_cx[i]);
	}

}

void SBX(double* parent1, double* parent2, double* child1, double* child2, int chrome) {
	//パラメータ：2~5を設定
	double nc = 2;

	int i;
	double u, ud;
	double alpha1, alpha2;
	double beta1, beta2;
	double beta_q1, beta_q2;
	double p1, p2;

	for (i = 0; i < DIM[chrome]; i++) {
		ud = (double)rand() / RAND_MAX;
		//各遺伝子コードに対して50%で交叉
		if (ud <= 0.5) {
			//p2 > p1 にする
			if (parent1[i] < parent2[i]) {
				p1 = parent1[i];
				p2 = parent2[i];
			}
			else {
				p1 = parent2[i];
				p2 = parent1[i];
			}
			//ここからの意味はわからないが、確率密度分布に従ってbetaを作成できる
			u = (double)rand() / RAND_MAX;
			//if (x2 == x1) {
			//	beta1 = DBL_MAX;
			//	beta2 = DBL_MAX;
			//}
			//else {
			beta1 = 1.0 + 2.0 * (p1 - lower_bound[i][chrome]) / (p2 - p1);
			beta2 = 1.0 + 2.0 * (upper_bound[i][chrome] - p2) / (p2 - p1);
			//}

			alpha1 = 2.0 - pow(beta1, -(nc + 1));
			alpha2 = 2.0 - pow(beta2, -(nc + 1));

			if (u <= (1 / alpha1)) {
				beta_q1 = pow((u * alpha1), 1.0 / (nc + 1.0));
			}
			else {
				beta_q1 = pow(1.0 / (2.0 - u * alpha1), 1.0 / (nc + 1.0));
			}
			if (u <= (1 / alpha2)) {
				beta_q2 = pow((u * alpha2), 1.0 / (nc + 1.0));
			}
			else {
				beta_q2 = pow(1.0 / (2.0 - u * alpha2), 1.0 / (nc + 1.0));
			}
			child1[i] = 0.5 * ((p1 + p2) - beta_q1 * (p2 - p1));
			child2[i] = 0.5 * ((p1 + p2) + beta_q2 * (p2 - p1));
		}
		//50%の確率で交叉しない
		else {
			child1[i] = parent1[i];
			child2[i] = parent2[i];
		}
	}
}


void single_point_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome) {

	int i;
	int x_point;

	x_point = rand() % (DIM[chrome] + 1);
	for (i = 0; i < x_point + 1; i++) {
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}
	for (i; i < DIM[chrome]; i++) {
		child1[i] = parent2[i];
		child2[i] = parent1[i];
	}

}

void two_point_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome) {

	int i;
	int x_point[2];
	int temp;

	non_restored_extract(DIM[chrome], x_point, 2);
	if (x_point[0] > x_point[1]) {
		temp = x_point[0];
		x_point[0] = x_point[1];
		x_point[1] = temp;
	}
	for (i = 0; i < x_point[0]; i++) {
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}
	for (i; i < x_point[1]; i++) {
		child1[i] = parent2[i];
		child2[i] = parent1[i];
	}
	for (i; i < DIM[chrome]; i++) {
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}

}

void uniform_crossover(double* parent1, double* parent2, double* child1, double* child2, int chrome) {
	double p;
	int i;

	for (i = 0; i < DIM[chrome]; i++) {
		p = (double)rand() / RAND_MAX;
		if (p < 0.5) {
			child1[i] = parent1[i];
			child2[i] = parent2[i];
		}
		else {
			child1[i] = parent2[i];
			child2[i] = parent1[i];
		}
	}

}

void STL_BLX_a(double* parent1, double* parent2, double* child1, double* child2, int chrome) {
	
	const double length1 = SECTION_LENGTH[0];
	const double length2 = SECTION_LENGTH[1];
	const double length3 = SECTION_LENGTH[2];
	
	double alpha = 0.3;

	int i, j;
	double dx[DIM_SEC];
	double min_x[DIM_SEC];
	double max_x[DIM_SEC];
	double min_cx[DIM_SEC];
	double max_cx[DIM_SEC];
	double u1, u2;

	double x1[DIM_SEC];
	double x2[DIM_SEC];
	double a1[DIM_SEC];
	double a2[DIM_SEC];
	x1[0] = parent1[0];
	x2[0] = parent2[0];
	for (i = 1; i < DIM[chrome]; i++) {
		x1[i] = parent1[i] + x1[i - 1];
		x2[i] = parent2[i] + x2[i - 1];
	}


	for (i = 0; i < DIM[chrome]; i++) {
		dx[i] = fabs(x1[i] - x2[i]);
		min_x[i] = fmin(x1[i], x2[i]);
		max_x[i] = fmax(x1[i], x2[i]);
		min_cx[i] = min_x[i] - alpha * dx[i];
		max_cx[i] = max_x[i] + alpha * dx[i];
	}

	//制限
	for (i = 0; i < L_NODE[0] - 1; i++) {
		if (min_cx[i] < 0) {
			min_cx[i] = 0;
		}
		if (max_cx[i] > length1) {
			max_cx[i] = length1;
		}
	}
	for (i = L_NODE[0] - 1; i < L_NODE[1] - 1; i++) {
		if (min_cx[i] < length1) {
			min_cx[i] = length1;
		}
		if (max_cx[i] > length1 + length2) {
			max_cx[i] = length1 + length2;
		}
	}
	for (i = L_NODE[1] - 1; i < SEGMENT; i++) {
		if (min_cx[i] < length1 + length2) {
			min_cx[i] = length1 + length2;
		}
		if (max_cx[i] > length1 + length2 + length3) {
			max_cx[i] = length1 + length2 + length3;
		}
	}

	for (i = 0; i < DIM[chrome]; i++) {
		u1 = (double)rand() / RAND_MAX;
		u2 = (double)rand() / RAND_MAX;
		a1[i] = min_cx[i] + u1 * (max_cx[i] - min_cx[i]);
		a2[i] = min_cx[i] + u2 * (max_cx[i] - min_cx[i]);
	}
	d_sort(a1, DIM[chrome]);
	d_sort(a2, DIM[chrome]);

	child1[0] = a1[0];
	child2[0] = a2[0];
	for (i = 1; i < DIM[chrome]; i++) {
		child1[i] = a1[i] - a1[i - 1];
		child2[i] = a2[i] - a2[i - 1];
	}


	//セグメント小さくなりすぎ問題への仮対処
	small_segment_handring(child1);
	small_segment_handring(child2);

	////extra operation for timestep problem
	//for (i = 0; i < SEGMENT; i++) {
	//	if (child1[i] < 0.5) {
	//		child1[i] = 0.5;
	//	}
	//	if (child2[i] < 0.5) {
	//		child2[i] = 0.5;
	//	}
	//}

	//正しいか表示用
	
		//double sum0 = 0;
		//double sum1 = 0;
		//double sum2 = 0;
		//for (i = 0; i < L_NODE[0] - 1; i++) {
		//	sum0 += child1[i];
		//}
		//for (i; i < L_NODE[1] - 1; i++) {
		//	sum1 += child1[i];
		//}
		//for (i; i < SEGMENT; i++) {
		//	sum2 += child1[i];
		//}
		//printf("mutation\n%f\n%f\n%f\n", sum0, sum1, sum2);
		//sum0 = 0;
		//sum1 = 0;
		//sum2 = 0;
		//for (i = 0; i < L_NODE[0] - 1; i++) {
		//	sum0 += child2[i];
		//}
		//for (i; i < L_NODE[1] - 1; i++) {
		//	sum1 += child2[i];
		//}
		//for (i; i < SEGMENT; i++) {
		//	sum2 += child2[i];
		//}
		//printf("mutation\n%f\n%f\n%f\n", sum0, sum1, sum2);

}

void STL_2PX(double* parent1, double* parent2, double* child1, double* child2, int chrome) {

	int i;

	for (i = 0; i < L_NODE[0] - 1; i++) {
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}
	for (i; i < L_NODE[1] - 1; i++) {
		child1[i] = parent2[i];
		child2[i] = parent1[i];
	}
	for (i; i < SEGMENT; i++) {
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}

}

void STL_SX(double* parent1, double* parent2, double* child1, double* child2, int chrome) {
	int i;
	int x_point;
	int temp;

	x_point = rand() % (DIM[chrome] + 1);
	temp = rand() % 2;
	if (temp == 0) {
		for (i = 0; i < x_point + 1; i++) {
			child1[i] = parent1[i];
			child2[i] = parent2[i];
		}
		for (i; i < DIM[chrome]; i++) {
			child1[i] = parent2[i];
			child2[i] = parent1[i];
		}
	}
	else if (temp == 1) {
		for (i = 0; i < x_point + 1; i++) {
			child1[i] = parent2[i];
			child2[i] = parent1[i];
		}
		for (i; i < DIM[chrome]; i++) {
			child1[i] = parent1[i];
			child2[i] = parent2[i];
		}
	}
	else {
		printf("STL_SX:default\n");
		exit(1);
	}

}





