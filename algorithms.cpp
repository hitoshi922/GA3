#include "Header.h"

//NSGA2
void create_R(individual* P, individual* Q, individual* R);
void non_dominated_sort(int arr); //- front F
void count_domination(individual* R, int arr);
void create_Front(individual* R, int arr);//- front F
void create_new_P(individual* P);//-F  アーカイブには元の情報アリ
void crowding_distance(individual* P); //-front F
void crowding_tornament(individual* P, individual* Q);
individual create_new_Q(individual* P);
void format_front();
void format_pop(individual* ind, int arr);


//MGG
void select_new_comer(individual* A, int* new_comer_selector, int arr);
void MGG_generation_change(individual* P, individual* nextP, int* parent_selector, int* new_comer_selector);;


void algorithm_junction() {
	if (ALGO == 0) {
		simple_GA();
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
}



void simple_GA() {
	int cnt = 0;
	bool convergence;

	init_ind(P, POP);
	evaluation(P, POP);
	do {
		//世代数表示
		printf("gen%d\n", cnt); 
		record(P, POP, cnt);
		ind_cpy(Q, P, POP);
		crossover(Q);
		mutation(Q, POP);
		ind_cpy(P, Q, POP);
		evaluation(P, POP);
		cnt++;
		convergence = halt_condition(P, POP);
	} while ((cnt < GEN) && (convergence == false));


}

void MGG() {
	double best = 0;
	double range = 0;

	int num_chi = 20;
	int qty_family;
	int parent_selecter[DIM_SEC];
	int new_comer_selector[DIM_SEC];
	int cnt = 0;
	int num_par;
	bool convergence;
	
	//多親交叉用
	if (CROSSOVER[0] == 12) {
		num_par = DIM[0] + 1;
	}
	else {
		num_par = 2;
	}
	qty_family = num_par + num_chi;

	init_ind(P, POP);
	evaluation(P, POP);
	do {
		printf("gen%d\n", cnt);
		non_restored_extract(POP, parent_selecter, num_par);
		MGG_crossover(P, parent_selecter,num_par, qty_family);//mutationも一体です
		select_new_comer(nextP, new_comer_selector, num_chi + 2);
		MGG_generation_change(P, nextP, parent_selecter, new_comer_selector);
		record(P, POP, cnt);
		convergence = halt_condition(P, POP);
		cnt++;

		best = 0;
		range = 0;
		for (int j = 0; j < POP; j++) {
			if (best < P[j].evaluation) {
				best = P[j].evaluation;
			}
		}
		printf("best = %f\n", best);

	} while ((cnt < GEN) && (convergence == false));
	//もしかして一度も選ばれていない個体を評価するため
	evaluation(P, POP);
}

void NSGA2() {//- front F
	int cnt = 0;
	int ranks;
	double best[2] = { 0 };
	double range;
	bool convergence;
	init_ind(Q, POP);
	evaluation(Q, POP);
	ind_cpy(R, Q, POP); //探索集団Qを全集団Rにコピー
	ind_cpy(P, R, POP); //全集団Rから初回アーカイブ
	record(P, POP, cnt);
	crossover(Q);
	
	cnt++;
	do {
		//for (int j = 0; j < POP; j++) {
		//	if (best[0] < P[j].evaluation) {
		//		best[0] = P[j].evaluation;
		//		range = P[j].f[1];
		//	}
		//}
		//printf("best = %f range = %.1f\n", best[0], range);
		printf("gen%d\n", cnt); //世代数表示
		format_pop(P, POP);
		format_pop(Q, POP);
		format_front();
		evaluation(Q, POP);
		create_R(P, Q, R);
		//int temp = check_clone(R, POP + POP);
		non_dominated_sort(POP + POP); //-F,R  支配数計測　パレートフロント作成

		for (int i = 0; i < F.rank_arr; i++) {
			printf("rank[%d]=%d ", i, F.num_arr[i]);
		}
		printf("\n\n");

		Allclear_POP(P, POP);
		//create_new_P(P); //-F  アーカイブには元の情報アリ
		crowding_distance(P);//-F  混雑距離を計算 + Pにアーカイブ
		//crowding_tornament(P, Q);
		ind_cpy(Q, P, POP); //上のcrowdingの代わり
		record(P, POP, cnt);
		convergence = halt_condition(P, POP);
		crossover(Q);
		mutation(Q, POP);
		cnt++;
		if (cnt % 50 == 0) {
			output_result(0, cnt);
		}
	} while ((cnt < GEN) && (convergence == false));

}

void FWGA() {
	//crossover();
	//mutation();
	//evaluation();
}

void create_R(individual* P, individual* Q, individual* R) {
	int i, j;

	for (i = 0; i < POP; i++) {
		R[i] = P[i];
	}
	for (j = 0; j < POP; j++) {
		R[j + i] = Q[j];
	}
}

void non_dominated_sort(int arr) {//- front F
	count_domination(R, arr);
	create_Front(R, arr);
}

void count_domination(individual* R, int arr) {
	int i;
	int j;
	int k;
	int cnt;
	int cnt_dominated_fitness;
	int cnt_dominated_number[POP + POP]; //被支配数カウンタ

	//初期化
	for (i = 0; i < arr; i++) {
		cnt_dominated_number[i] = 0;
	}

	//デバッグ用
	//R[0].fitness[0] = 1;
	//R[1].fitness[0] = 1;
	//R[0].fitness[1] = 1;
	//R[1].fitness[1] = 1;

	//被支配数計測
	//for (i = 0; i < arr; i++) {
	//	for (j = (i + 1); j < arr; j++) {
	//		cnt_dominated_fitness = 0;

	//		for (k = 0; k < OBJ; k++) {
	//			if (R[i].fitness[k] >= R[j].fitness[k]) //R[i]のほうが目的関数が大きいとき
	//				cnt_dominated_fitness = cnt_dominated_fitness + 1;
	//		}
	//		if (cnt_dominated_fitness == (OBJ)) {
	//			cnt_dominated_number[j] = cnt_dominated_number[j] + 1;
	//			R[i].dom_ind[R[i].domination] = j;
	//			R[i].domination++;
	//		}
	//		if (cnt_dominated_fitness == 0) {
	//			cnt_dominated_number[i] = cnt_dominated_number[i] + 1;
	//			R[j].dom_ind[R[j].domination] = i;
	//			R[j].domination++;
	//		}
	//	}
	//}


	//確実な方
	for (i = 0; i < arr; i++) {
		for (j = 0; j < arr; j++) {
			//自分同士での比較を避ける
			if (i != j) {
				cnt_dominated_fitness = 0;

				for (k = 0; k < OBJ; k++) {
					if (R[i].fitness[k] >= R[j].fitness[k]) //R[i]のほうが目的関数が大きいとき
						cnt_dominated_fitness = cnt_dominated_fitness + 1;
				}

				if (cnt_dominated_fitness == (OBJ)) {
					cnt_dominated_number[j] = cnt_dominated_number[j] + 1;
					R[i].dom_ind[R[i].domination] = j;
					R[i].domination++;
				}
			}
		}
	}

	//配列移し替え
	for (i = 0; i < arr; i++) {
		R[i].dominated = cnt_dominated_number[i];
	}
}

void create_Front(individual* R, int arr) {//- front F
	int i;
	int j;
	int k;
	int cnt; //個体数のカウント
	int temp;
	int num_cnt; //同ランク内での順番用変数
	int rank_cnt;

	cnt = 0;

	for (rank_cnt = 0; rank_cnt < arr; rank_cnt++) {
		num_cnt = 0;


		//dominated 0 を移動
		for (i = 0; i < arr; i++) {
			if (R[i].dominated == 0) {

				R[i].rank = rank_cnt;
				R[i].dominated = -1;
				F.F[rank_cnt][num_cnt] = R[i];
				F.num_arr[rank_cnt]++;
				num_cnt++;
				cnt++;
			}
		}
		F.rank_arr++;

		//被支配分を減算
		for (j = 0; j < arr; j++) {
			if (R[j].rank == rank_cnt) {
				for (k = 0; k < R[j].domination; k++) {
					temp = R[j].dom_ind[k];
					R[temp].dominated = R[temp].dominated - 1;
				}
			}
		}

		if (cnt == arr) {
			break;
		}
	}


}

// |P| + |F| < N 
//新しいアーカイブ母集団Pの作成
void create_new_P(individual* P) {//- front F
	int i, j;
	int total = 0;//個体数管理
	int fcnt = 0;//ランク数管理
	int pcnt = 0;
	int temp = 0;

	//Pに入れる上位個体を数える
	while (total < POP) {
		temp += F.num_arr[fcnt];
		if (temp > POP) {
			break;
		}
		else {
			total += F.num_arr[fcnt];
			fcnt++;
		}
	};
	//Pへ書き込み
	for (i = 0; i < fcnt; i++) {
		for (j = 0; j < F.num_arr[i]; j++) {
			P[pcnt] = F.F[i][j];
			pcnt++;
		}
	}

}

void crowding_distance(individual* P) { //-front F
	int i;
	int j;
	int k;
	int l;
	int M; //ランク内総数
	int total;
	int terget_rank;
	individual max;
	individual min;
	individual temp;

	for (i = 0; i < F.rank_arr; i++) {
		for (j = 0; j < OBJ; j++) {

			//fitness順に並び替え
			for (k = 0; k < F.num_arr[i]; k++) {
				for (l = k; l < F.num_arr[i]; l++) {
					if (F.F[i][k].fitness[j] > F.F[i][l].fitness[j]) {
						temp = F.F[i][k];
						F.F[i][k] = F.F[i][l];
						F.F[i][l] = temp;
					}
				}
			}


			M = F.num_arr[i];
			min = F.F[i][0];
			max = F.F[i][M - 1];
			F.F[i][0].crowding_distance = DBL_MAX; //目的関数最大・最小に最大距離を与える
			F.F[i][M - 1].crowding_distance = DBL_MAX;
			for (k = 1; k < F.num_arr[i] - 1; k++) {
				if (F.F[i][k].crowding_distance != DBL_MAX) { //既に最大距離なら何もしない //本当はこの記述いらない
					F.F[i][k].crowding_distance += (F.F[i][k + 1].f[j] - F.F[i][k - 1].f[j]) / (max.f[j] - min.f[j]);
				}
			}
		}
	}
	//次のcrowding_distanceで個体群Pが混雑距離を持つために必須
	create_new_P(P);
	//*************

	total = 0;
	for (i = 0; i < POP; i++) {
		total = total + F.num_arr[i];
		if (total > POP) {
			total = total - F.num_arr[i];
			break;
		}
	} // この時iはsortするrank
	terget_rank = i;

	//crowding_sort
	for (i = 0; i < F.num_arr[terget_rank]; i++) {
		for (j = i; j < F.num_arr[terget_rank]; j++) {
			if (F.F[terget_rank][i].crowding_distance < F.F[terget_rank][j].crowding_distance) {
				temp = F.F[terget_rank][i];
				F.F[terget_rank][i] = F.F[terget_rank][j];
				F.F[terget_rank][j] = temp;
			}
		}
	}
	for (i = 0; i < (POP - total); i++) {
		P[total + i] = F.F[terget_rank][i];
	}
}

void crowding_tornament(individual* P, individual* Q) {
	int i;
	for (i = 0; i < POP; i++) {
		Q[i] = create_new_Q(P);
	}
}

//探索母集団Q 生成
individual create_new_Q(individual* P) {
	int i, j;
	int select_operator[TORNAMENT_SIZE];
	individual participant[TORNAMENT_SIZE];
	individual temp;

	non_restored_extract(POP, select_operator, TORNAMENT_SIZE);
	for (i = 0; i < TORNAMENT_SIZE; i++) {
		participant[i] = P[select_operator[i]];
	}
	//ここからトーナメント
	for (i = 0; i < TORNAMENT_SIZE; i++) {
		for (j = i; j < TORNAMENT_SIZE; j++) {
			if (participant[i].rank > participant[j].rank)
			{
				temp = participant[i];
				participant[i] = participant[j];
				participant[j] = temp;
			}
			else if ((participant[i].crowding_distance > participant[j].crowding_distance)
				&& (participant[i].rank == participant[j].rank))
			{
				temp = participant[i];
				participant[i] = participant[j];
				participant[j] = temp;
			}
		}
	}
	return participant[0];
}

//front初期化
void format_front() {//- front F
	int i;
	for (i = 0; i < POP + POP; i++) {
		Allclear_POP(F.F[i], POP + POP);
	}
	for (i = 0; i < POP + POP; i++) {
		F.num_arr[i] = 0;
		F.rank_arr = 0;
	}
}

//パラメータセット以外の個体データ消去
void format_pop(individual* ind, int arr) {
	int i;
	int j;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < POP + POP; j++) {
			ind[i].dom_ind;
		}
		ind[i].rank = -1;
		ind[i].crowding_distance = 0;
		ind[i].domination = 0;
		ind[i].dominated = 0;
		//ind[i].evaluation = 0;
		for (j = 0; j < POP + POP; j++) {
			ind[i].dom_ind[j] = -1;
		}
	}
}

//MGG
void select_new_comer(individual* A, int* new_comer_selector, int arr) {
	//戻す個体の選択
	int i;
	int cnd_elite = 0;
	int cnd_rand = 0;
	cnd_elite = 0;
	for (i = 0; i < arr; i++) {
		if (nextP[i].fitness[0] > nextP[cnd_elite].fitness[0]) {
			cnd_elite = i;
		}
	}
	do {
		cnd_rand = MGG_roulette_select(nextP, arr);
	} while (cnd_rand == cnd_elite);
	new_comer_selector[0] = cnd_elite;
	new_comer_selector[1] = cnd_rand;
}

void MGG_generation_change(individual* P, individual* nextP, int* parent_selector, int* new_comer_selector) {
	P[parent_selector[0]] = nextP[new_comer_selector[0]];
	P[parent_selector[1]] = nextP[new_comer_selector[1]];
}
