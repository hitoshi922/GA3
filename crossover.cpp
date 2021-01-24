#include"Header.h"


void parent_selection(individual* B, int* sel, int np);
void parent_selection2(individual* B, int* sel, int np);

void assign_parent(individual* B, double parent[][DIM_SEC], int chrome, int n_parent);
void assign_parent2(individual* B, double parent[][DIM_SEC], int cnt, int chrome);



void X_junction(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void single_point_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void two_point_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void uniform_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);

void BLX_a(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void SBX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void SPX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);

void UNDX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void STL_BLX_a(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void STL_2PX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
void STL_SX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome);
int keep_nextP(int cnt, double child[][DIM_SEC], int chrome, int n_child);
int recycle_select(individual* B, int cnt);

void search_space_restriction(double child[][DIM_SEC], int chrome, int n_child);
void SSR_pull_back(double child[][DIM_SEC], int chrome, int n_child);
void SSR_BM(double child[][DIM_SEC], int chrome, int n_child);

void generation_change(individual* B, int cnt);

#define OP_SEC DIM_SEC 
//グローバル変数-------------------
static int sel[DIM_SEC];
static int sel2[POP];
static int num_of_children;
static int num_of_recycle;
//---------------------------------


void crossover(individual* B) {
	int i;
	int cnt;
	int inc;
	double parent[OP_SEC][DIM_SEC];
	double child[OP_SEC][DIM_SEC];
	int n_parent;
	int n_child;

	if (CROSSOVER[0] == 12) { //SPX
		n_parent = DIM[0] + 1;
	}
	else {
		n_parent = 2;
	}
	if (CROSSOVER[0] == 12) { //SPX
		n_child = 1;
	}
	else {
		n_child = 2;
	}

	num_of_children = (int)(POP * CROSSOVER_RATE);
	num_of_recycle = POP - num_of_children;

	cnt = 0;

	//parent_selection2(B, sel2, num_of_children); //全親重複なし選択

	do {
		parent_selection(B, sel, n_parent);
		for (i = 0; i < CHROM; i++) {
			assign_parent(B, parent, i, n_parent);
			//assign_parent2(B, parent[0], parent[1], cnt, i);//全親重複なし選択ver
			X_junction(parent, child, i);
			search_space_restriction(child, i, n_child);
			inc = keep_nextP(cnt, child, i, n_child);
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
	int n_parent;

	if (CROSSOVER[0] == 12) {
		n_parent = DIM[0] + 1;
	}
	else {
		n_parent = 2;
	}

	double parent[DIM_SEC][DIM_SEC]; 
	double child[DIM_SEC][DIM_SEC];

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
			assign_parent(B, parent, i, n_parent);
			//X_junction(parent[0], parent[1], child[0], child[1], i);
			//inc = keep_nextP(cnt, child[0], child[1], i);
		}
		cnt = cnt + inc;
	} while (cnt < num_of_children);

	//mutation
	mutation(nextP, num_of_children);

	//全固体の評価
	evaluation(nextP, num_of_children);



}


void X_junction(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	switch (CROSSOVER[chrome])
	{
	case 0:
		single_point_crossover(parent, child, chrome);
		break;
	case 1:
		two_point_crossover(parent, child, chrome);
		break;
	case 2:
		uniform_crossover(parent, child, chrome);
		break;
	case 10:
		BLX_a(parent, child, chrome);
		break;
	case 11:
		SBX(parent, child, chrome);
		break;
	case 12:
		SPX(parent, child, chrome);
		break;
	case 100:
		STL_2PX(parent, child, chrome);
		break;
	case 101:
		STL_SX(parent, child, chrome);
		break;
	case 102:
		STL_BLX_a(parent, child, chrome);
		break;
	default:
		printf("CROSSOVERの値が不正です。");
		exit(1);
	}
}


int keep_nextP(int cnt, double child[][DIM_SEC], int chrome, int n_child) {
	int i;
	int inc = 0;
	for (int j = 0; j < n_child; j++) {
		for (i = 0; i < DIM[chrome]; i++) {
			nextP[cnt].X[i][chrome] = child[j][i];
		}
		cnt++;
		inc++;
		if (cnt == num_of_children) {
			return inc;
		}
	}
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
	//if (ALGO == 10) {
	//	crowding_tornament_select(B, POP, sel, np);
	//	return;
	//}
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
	default:
		printf("parent_selection: PSM is invalid value.\n");
		exit(1);
	}
}

void parent_selection2(individual* B, int* sel, int np) {
	non_restored_extract(POP, sel2, np);
}


void assign_parent(individual* B, double parent[][DIM_SEC], int chrome, int n_parent) {
	int i, j;
	for (j = 0; j < n_parent; j++) {
		for (i = 0; i < DIM[chrome]; i++) {
			parent[j][i] = B[sel[j]].X[i][chrome];
		}
	}



}

void assign_parent2(individual* B, double parent[][DIM_SEC], int cnt, int chrome) {
	int i;
	for (i = 0; i < DIM[chrome]; i++) {
		parent[0][i] = B[sel2[cnt]].X[i][chrome];
		parent[1][i] = B[sel2[cnt + 1]].X[i][chrome];
	}
}

void BLX_a(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {

	double alpha = 0.366;

	int i;
	double dx[DIM_SEC];
	double min_x[DIM_SEC];
	double max_x[DIM_SEC];
	double min_cx[DIM_SEC];
	double max_cx[DIM_SEC];
	double u1, u2;


	for (i = 0; i < DIM[chrome]; i++) {
		dx[i] = fabs(parent[0][i] - parent[1][i]);
		min_x[i] = fmin(parent[0][i], parent[1][i]);
		max_x[i] = fmax(parent[0][i], parent[1][i]);
		min_cx[i] = min_x[i] - alpha * dx[i];
		max_cx[i] = max_x[i] + alpha * dx[i];
	}

	for (i = 0; i < DIM[chrome]; i++) {
		u1 = (double)rand() / RAND_MAX;
		u2 = (double)rand() / RAND_MAX;
		child[0][i] = min_cx[i] + u1 * (max_cx[i] - min_cx[i]);
		child[1][i] = min_cx[i] + u2 * (max_cx[i] - min_cx[i]);
	}

}

void SBX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
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
			if (parent[0][i] < parent[1][i]) {
				p1 = parent[0][i];
				p2 = parent[1][i];
			}
			else {
				p1 = parent[1][i];
				p2 = parent[0][i];
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
			child[0][i] = 0.5 * ((p1 + p2) - beta_q1 * (p2 - p1));
			child[1][i] = 0.5 * ((p1 + p2) + beta_q2 * (p2 - p1));
		}
		//50%の確率で交叉しない
		else {
			child[0][i] = parent[0][i];
			child[1][i] = parent[1][i];
		}
	}
}

void SPX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	int i, j;
	double eps = sqrt(DIM[0] + 2);
	double G[DIM_SEC] = { 0 };
	double x[DIM_SEC][DIM_SEC] = { 0 };
	double C[DIM_SEC][DIM_SEC] = { 0 };
	double rk[DIM_SEC];
	int num_par = DIM[chrome] + 1;
	double temp;

	//あらかじめrkを計算
	for (i = 0; i < DIM[chrome]; i++) {
		rk[i] = (double)(rand()) / RAND_MAX;
		temp = 1 / ((double)i + 1);
		rk[i] = pow(rk[i], 1 / ((double)i + 1));
	}

	//重心
	for (i = 0; i < DIM[chrome]; i++) {
		for (j = 0; j < num_par; j++) {
			G[i] += parent[j][i] / ((double)DIM[0] + 1);
		}
	}
	//xを計算
	for (i = 0; i < num_par; i++) {
		for (j = 0; j < DIM[chrome]; j++) {
			x[i][j] = G[i] + eps * (parent[i][j] - G[i]);
		}
	}
	//Cに関して計算
	for (j = 0; j < DIM[chrome]; j++) {
		C[0][j] = 0;
	}
	for (i = 1; i < num_par; i++) {
		for (j = 0; j < DIM[chrome]; j++) {
			C[i][j] = rk[i - 1] * (x[i - 1][j] - x[i][j] + C[i - 1][j]);
		}
	}
	//子個体生成
	for (i = 0; i < DIM[0]; i++) {
		child[0][i] = x[DIM[0]][i] + C[DIM[0]][i];
	}

}


void UNDX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	int i;
	double p[DIM_SEC];
	double d[DIM_SEC];
	double D;
	//中心点pを求める
	for (i = 0; i < DIM[chrome]; i++) {
		p[i] = (parent[0][i] + parent[1][i]) / 2;
	}
	//差分ベクトルdを求める
	for (i = 0; i < DIM[chrome]; i++) {
		d[i] = parent[0][i] - parent[1][i];
	}
	//親3から主探索直線PSLまでの距離を求める


}


void single_point_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {

	int i;
	int x_point;

	x_point = rand() % (DIM[chrome] + 1);
	for (i = 0; i < x_point + 1; i++) {
		child[0][i] = parent[0][i];
		child[1][i] = parent[1][i];
	}
	for (i; i < DIM[chrome]; i++) {
		child[0][i] = parent[1][i];
		child[1][i] = parent[0][i];
	}

}

void two_point_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {

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
		child[0][i] = parent[0][i];
		child[1][i] = parent[1][i];
	}
	for (i; i < x_point[1]; i++) {
		child[0][i] = parent[1][i];
		child[1][i] = parent[0][i];
	}
	for (i; i < DIM[chrome]; i++) {
		child[0][i] = parent[0][i];
		child[1][i] = parent[1][i];
	}

}

void uniform_crossover(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	double p;
	int i;

	for (i = 0; i < DIM[chrome]; i++) {
		p = (double)rand() / RAND_MAX;
		if (p < 0.5) {
			child[0][i] = parent[0][i];
			child[1][i] = parent[1][i];
		}
		else {
			child[0][i] = parent[1][i];
			child[1][i] = parent[0][i];
		}
	}

}

void STL_BLX_a(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	
	const double length1 = SECTION_LENGTH[0];
	const double length2 = SECTION_LENGTH[1];
	const double length3 = SECTION_LENGTH[2];
	
	double alpha = 0.366;

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
	x1[0] = parent[0][0];
	x2[0] = parent[1][0];
	for (i = 1; i < DIM[chrome]; i++) {
		x1[i] = parent[0][i] + x1[i - 1];
		x2[i] = parent[1][i] + x2[i - 1];
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

	child[0][0] = a1[0];
	child[1][0] = a2[0];
	for (i = 1; i < DIM[chrome]; i++) {
		child[0][i] = a1[i] - a1[i - 1];
		child[1][i] = a2[i] - a2[i - 1];
	}


	//セグメント小さくなりすぎ問題への仮対処
	small_segment_handring(child[0]);
	small_segment_handring(child[1]);

	////extra operation for timestep problem
	//for (i = 0; i < SEGMENT; i++) {
	//	if (child[0][i] < 0.5) {
	//		child[0][i] = 0.5;
	//	}
	//	if (child[1][i] < 0.5) {
	//		child[1][i] = 0.5;
	//	}
	//}

	//正しいか表示用
	
		//double sum0 = 0;
		//double sum1 = 0;
		//double sum2 = 0;
		//for (i = 0; i < L_NODE[0] - 1; i++) {
		//	sum0 += child[0][i];
		//}
		//for (i; i < L_NODE[1] - 1; i++) {
		//	sum1 += child[0][i];
		//}
		//for (i; i < SEGMENT; i++) {
		//	sum2 += child[0][i];
		//}
		//printf("mutation\n%f\n%f\n%f\n", sum0, sum1, sum2);
		//sum0 = 0;
		//sum1 = 0;
		//sum2 = 0;
		//for (i = 0; i < L_NODE[0] - 1; i++) {
		//	sum0 += child[1][i];
		//}
		//for (i; i < L_NODE[1] - 1; i++) {
		//	sum1 += child[1][i];
		//}
		//for (i; i < SEGMENT; i++) {
		//	sum2 += child[1][i];
		//}
		//printf("mutation\n%f\n%f\n%f\n", sum0, sum1, sum2);

}

void STL_2PX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {

	int i;

	for (i = 0; i < L_NODE[0] - 1; i++) {
		child[0][i] = parent[0][i];
		child[1][i] = parent[1][i];
	}
	for (i; i < L_NODE[1] - 1; i++) {
		child[0][i] = parent[1][i];
		child[1][i] = parent[0][i];
	}
	for (i; i < SEGMENT; i++) {
		child[0][i] = parent[0][i];
		child[1][i] = parent[1][i];
	}

}

void STL_SX(double parent[][DIM_SEC], double child[][DIM_SEC], int chrome) {
	int i;
	int x_point;
	int temp;

	x_point = rand() % (DIM[chrome] + 1);
	temp = rand() % 2;
	if (temp == 0) {
		for (i = 0; i < x_point + 1; i++) {
			child[0][i] = parent[0][i];
			child[1][i] = parent[1][i];
		}
		for (i; i < DIM[chrome]; i++) {
			child[0][i] = parent[1][i];
			child[1][i] = parent[0][i];
		}
	}
	else if (temp == 1) {
		for (i = 0; i < x_point + 1; i++) {
			child[0][i] = parent[1][i];
			child[1][i] = parent[0][i];
		}
		for (i; i < DIM[chrome]; i++) {
			child[0][i] = parent[0][i];
			child[1][i] = parent[1][i];
		}
	}
	else {
		printf("STL_SX:default\n");
		exit(1);
	}

}

//実数値GAでの探索空間制限
void search_space_restriction(double child[][DIM_SEC], int chrome, int n_child) {
	SSR_BM(child, chrome, n_child);
	SSR_pull_back(child, chrome, n_child);

}

//空間制限 - 引き戻し
void SSR_pull_back(double child[][DIM_SEC], int chrome, int n_child) {

	for (int i = 0; i < n_child; i++) {
		for (int j = 0; j < DIM[chrome]; j++) {
			if (child[i][j] < lower_bound[j][chrome]){
				child[i][j] = lower_bound[j][chrome] + (double)rand() * 0.0001 / RAND_MAX;
				}
			else if (child[i][j] > upper_bound[j][chrome]) {
				child[i][j] = upper_bound[j][chrome] - (double)rand() * 0.0001 / RAND_MAX;
			}
		}
	}
}

//空間制限 - Boundary Mirroring
void SSR_BM(double child[][DIM_SEC], int chrome, int n_child) {
	for (int i = 0; i < n_child; i++) {
		for (int j = 0; j < DIM[chrome]; j++) {
			if (child[i][j] < lower_bound[j][chrome]) {
				child[i][j] = 2 * lower_bound[j][chrome] - child[i][j];
			}
			else if (child[i][j] > upper_bound[j][chrome]) {
				child[i][j] = 2 * upper_bound[j][chrome] - child[i][j];
			}
		}
	}
}



