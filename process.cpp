#include"Header.h"

void rm_right_space(char* str);

//乱数初期値
void random_seed() {
	unsigned int now;
	now = (unsigned int)time(0);
	srand(now);
}

//非復元抽出   
void non_restored_extract(int max, int* x, int arr) {
	int i, j;
	for (i = 0; i < arr; i++) {
		x[i] = uniform_random(max);
		for (j = 0; j < i; j++) {
			while (x[i] == x[j]) {
				x[i] = uniform_random(max);
			}
		}
	}
}

void non_restored_extract2(int max, int min, int* x, int arr) {
	int i, j;
	for (i = 0; i < arr; i++) {
		x[i] = uniform_random2(max, min);
		for (j = 0; j < i; j++) {
			while (x[i] == x[j]) {
				x[i] = uniform_random2(max, min);
			}
		}
	}
}

//トーナメント選択　//評価がプラスだとおかしくなる
void tornament_select(individual* B, int max, int* x, int arr) {
	int i, j;
	int win;
	int check = 0;
	int cand[TORNAMENT_SIZE];

	double keep = 0;
	int cnt = 0;

	if (arr * TORNAMENT_SIZE > max) {
		printf("(選択数)* TORNAMENT_SIZE < (母集団数) を満たす値をtornament_selectに渡してください\n");
		exit(1);
	}

	do {
		non_restored_extract(max, cand, TORNAMENT_SIZE);
		keep = 0;
		for (i = 0; i < TORNAMENT_SIZE; i++) {
			if (keep <= B[cand[i]].evaluation) {
				win = cand[i];
				keep = B[win].evaluation;
			}
		}
		x[cnt] = win;

		for (j = 0; j < cnt; j++) {
			if (x[cnt] == x[j]) {
				check = 1;
			}
		}
		if (check == 0) {
			cnt++;
		}
		check = 0; //リセット
	} while (cnt < arr);

}


//ルーレット選択 //評価値がマイナスだとおかしくなる
void roulette_select(individual* B, int max, int* x, int arr) {
	int i, j;
	double n;
	double total = 0;
	double sum = 0;
	for (i = 0; i < max; i++) {
		total += B[i].evaluation;
	}
	for (j = 0; j < arr; j++) {
		sum = 0;
		n = (double)rand() / RAND_MAX;
		for (i = 0; i < max; i++) {
			sum += B[i].evaluation / total;
			if (sum > n) {
				x[j] = i;
				break;
			}
		}
	}
}

//混雑度トーナメント親選択
void crowding_tornament_select(individual* B, int max, int* x, int arr) {
	int i, j;
	int select_operator[TORNAMENT_SIZE * 2];
	individual participant[TORNAMENT_SIZE];
	individual temp;

	non_restored_extract(max, select_operator, TORNAMENT_SIZE * 2);
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

}




//一様ランダムセレクト
int uniform_random(int max) {
	int num;
	num = rand() % max;
	return num;
}

//そのうち上と統合する
int uniform_random2(int max, int min) {
	int num;
	num = min + rand() % (max - min);
	return num;
}

int find_header(FILE* fp, char* keyword)
/*
	入力	fp	ファイルポインタ
			keyword	キーワード
	戻り値	1：ヘッダを見つけた	0：ヘッダが見つからなかった
*/
{
	char char_buf[256];
	char keyword_buf[256];
	char* condition;
	int len, i;

	strcpy_s(keyword_buf, keyword);
	rm_right_space(keyword_buf); //右側の空白を取り除く

	rewind(fp);
	for (;;) {
		condition = fgets(char_buf, BUFSIZ, fp);
		if (condition == NULL)break;
		rm_right_space(char_buf);
		len = strlen(char_buf);
		if (len == 0)continue;
		for (i = 0; i < len; i++) {
			if (char_buf[i] != ' ')break;
		}
		if (strcmp(&char_buf[i], keyword_buf) == 0) {
			return(1);
			break;
		}
	}
	fprintf(stderr, "cannot find header. keyword ; |%s|\n,", keyword);
	return(0);
}

int find_header2(FILE* fp, char* keyword) {
	char char_buf[256];
	char keyword_buf[256];
	int len, i;
	int condition;

	strcpy_s(keyword_buf, keyword);
	rewind(fp);
	for (;;) {
		condition = fscanf_s(fp, "%s\n", char_buf, 256);
		if (strcmp(char_buf, keyword_buf) == 0) {
			return(1);
			break;
		}
		if (condition == -1)break;
	}
	fprintf(stderr, "cannot find header. keyword ; |%s|\n,", keyword);
	return(0);
}

void rm_right_space(char* str)
{
	int i;

	i = strlen(str) - 1;
	if (str[i] == '\n')i--;

	while (i > 0) {
		if (str[i] != ' ')break;
		i--;
	}
	str[i + 1] = '\0';
}

//降順ソート
void sort(individual* ind, int arr) {
	int i;
	int j;
	individual temp;

	for (i = 0; i < arr; i++) {
		for (j = i; j < arr; j++) {
			if (ind[i].evaluation < ind[j].evaluation) {
				temp = ind[i];
				ind[i] = ind[j];
				ind[j] = temp;
			}
		}
	}
}

void d_sort(double* x, int arr) {
	int i, j;
	double temp;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < arr; j++) {
			if (x[i] < x[j]) {
				temp = x[i];
				x[i] = x[j];
				x[j] = temp;
			}
		}
	}
}

//配列初期化
void clear_array(double* x, int arr) {
	for (int i = 0; i < arr; i++) {
		x = 0;
	}
}

//個体群の完全初期化
void clear_POP(individual* A, int arr) {
	int i, j, k;
	for (i = 0; i < arr; i++) {
		for (j = 0; j < CHROM; j++) {
			for (k = 0; k < DIM[j]; k++) {
				A[i].X[k][j] = 0;
			}
		}
		for (j = 0; j < OBJ; j++) {
			A[i].f[j] = 0;
			A[i].fitness[j] = 0;
			A[i].w[j] = 0;
		}
		A[i].evaluation = 0;
		A[i].crowding_distance = 0;
		A[i].dominated = 0;
		A[i].domination = 0;
		A[i].rank = -1;
		A[i].eval_tag = 0;
		for (j = 0; j < POP + POP; j++) {
			A[i].dom_ind[j] = -1;
		}
	}
}

void ind_cpy(individual* A, individual* B, int arr) {
	for (int i = 0; i < arr; i++) {
		A[i] = B[i];
	}
}




//デバッグ用　個体表示
void individual_info(individual ind) {
	int i, j, k;
	for (i = 0; i < CHROM; i++) {
		for (j = 0; j < DIM[i]; j++) {
			printf("X[%d]=%f ", j, ind.X[j][i]);
		}
	}

	printf("\n");
	for (j = 0; j < OBJ; j++) {
		printf("f[%d]=%f ", j, ind.f[j]);
	}
	printf("\n");
	for (j = 0; j < OBJ; j++) {
		printf("fitness[%d]=%f ", j, ind.fitness[j]);
	}
	printf("\n");
	printf("rank=%d\n", ind.rank);
	printf("evaluation=%f\n", ind.evaluation);
	if (ind.crowding_distance == DBL_MAX) {
		printf("d=MAX\n");
	}
	else {
		printf("d=%f\n", ind.crowding_distance);
	}
	printf("dominated=(%d)\n", ind.dominated);
	printf("dominate=(%d)\n", ind.domination);


	for (j = 0; j < ind.domination; j++) {
		printf("%d ", ind.dom_ind[j]);
	}
	printf("\n\n");

}

void divide_num(int qty, int* divided, int arr) {
	int cnt = 0;
	int i;
	int t[DIM_SEC];
	t[arr] = qty;
	non_restored_extract2(qty - 1, 1, t, arr);
	t[0] = 0;
	t[arr] = qty;

	ascending_array_sort(t, arr + 1);
	divided[0] = t[1] - t[0];
	for (i = 1; i < arr; i++) {
		divided[i] = t[i+1] - t[i];
	}
	
}

void ascending_array_sort(int* array, int arr) {
	int temp;
	int i, j;
	for (i = 0; i < arr; i++) {
		for (j = i + 1; j < arr; j++) {
			if (array[i] > array[j]) {
				temp = array[j];
				array[j] = array[i];
				array[i] = temp;
			}
		}
	}
}
