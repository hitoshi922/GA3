#pragma once
#define _USE_MATH_DEFINES
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include<time.h>

//information
//負荷容量を20pにしています

#define NETLIST 10 //0:single pulse  1:500MHz clock signal 10:single pulse(variable length)


//NSGA2を使うときは交叉率を100%にすること
#define ALGO 10 //0:simple  1:MGG  10:NSGA2
constexpr double CROSSOVER_RATE = 1; // *100[%];


//離散・連続を変更したら、交叉・突然変異を変更
#define TYPE 0 //0:contenious  1:discrete  2:designed
#define SUB_TYPE 1 //0:contenious 1:discrete

constexpr int TRIAL = 1;

#define DIM_SEC 50
#define OBJ_SEC 5
#define CHROM_SEC 3
constexpr int POP = 30;
constexpr int GEN = 50;
constexpr double MUTATION_RATE[CHROM_SEC] = { 0.01,0.2 }; // *100[%]

constexpr int designed[] = { 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 };

#define EVALUATION 1
/* EVALUATION FUNCTION
0:equation_base
1:simulation_base
10:SCH
11:FON
12:POL
13:KUR
24:ZDT4
50:rosenbrock(2 variables)
51:sphare(2 variables)
60:rosenbrock(n variables)
61:sphare(n variables)
100:test
*/


#define PSM 0
/*PARENT SELECTION METHOD
UNIFORM:0  TORNAMENT:1  ROULETTE:2
crowding:10
*/

constexpr int CROSSOVER[CHROM_SEC] = { 12,100 };
/* CROSSOVER METHOD
OnePX:0  TwoPX:1  UniX:2
BLX_a:10  SBX:11  SPX:12
//L専用
STL_2PX:100  STL_BLX_a:102
//Z専用
STL_SX:101
*/


//100を使ってはならない
constexpr int MUTATION[CHROM_SEC] = { 1,101 };
//なし:0　UNIM:1 PBM:2 100:STL_descrete 101:STL_continious;

#define TORNAMENT_SIZE 2

typedef struct INDIVIDUAL {
	double X[DIM_SEC][CHROM_SEC];
	double f[OBJ_SEC];
	double fitness[OBJ_SEC];
	double w[OBJ_SEC];
	double evaluation;
	int eval_tag; // 0:未評価 1:評価済み

	int dominated;
	double crowding_distance;
	int dom_ind[POP + POP];
	int domination;
	int rank;
}individual;

typedef struct FRONT {
	individual F[POP + POP][POP + POP]; //[ランク][個体配列]
	int num_arr[POP + POP]; //ランク内個体数
	int rank_arr; //総ランク数
}front;

//grobal variance
extern front F;
extern individual P[POP];
extern individual nextP[POP];
extern individual Q[POP];
extern individual R[POP + POP];
extern individual best_P[GEN];
extern individual elite[2];
extern double gen_ave[GEN];
extern double upper_bound[DIM_SEC][CHROM_SEC];
extern double lower_bound[DIM_SEC][CHROM_SEC];
extern int DIM[CHROM_SEC];
extern int OBJ;
extern int CHROM;
extern int max_min[OBJ_SEC];
extern int MIN_OR_MAX[OBJ_SEC];
extern int SEGMENT;
extern int L_NODE[DIM_SEC]; //負荷容量を接続するノード
extern int QTY_SECTION;
extern double SECTION_LENGTH[DIM_SEC]; //負荷容量までの距離
extern int CNT_SEGMENT[DIM_SEC]; 

//algorithms
void basic_GA();
void MGG();
void NSGA2();

//crossover
void crossover(individual* B);
void MGG_crossover(individual* B, int* parent_select, int num_par, int qty_family);

//evaluation
void evaluation(individual* A, int arr);

//main
void initialize3(individual* A, int arr);
void initialize2();
void initialize(individual* A, int arr);
void record(individual* ind, int arr, int cnt);
void init_ind(individual* ind, int arr);
void small_segment_handring(double* seg);
void output_result(double ex_time, int gen);

//mutation
void mutation(individual* B, int arr);

//process
void random_seed();
void non_restored_extract(int max, int* x, int arr);
void non_restored_extract2(int max, int min, int* x, int arr);
void tornament_select(individual* B, int max, int* x, int arr);
void roulette_select(individual* B, int limit, int* num, int arr);
int MGG_roulette_select(individual* B, int max);
void crowding_tornament_select(individual* B, int max, int* x, int arr);
int uniform_random(int a);
int uniform_random2(int max, int min);
int find_header(FILE* fp, char* keyword);
int find_header2(FILE* fp, char* keyword);
void sort(individual* ind, int arr);
void d_sort(double* x, int arr);
void clear_array(double* x, int arr);
void clear_POP(individual* A, int arr);
void ind_cpy(individual* A, individual* B, int arr);
void individual_info(individual ind);
void divide_num(int qty, int* divided, int arr);
void ascending_array_sort(int* array, int arr);
void gram_schmidt(double* x, double* e, int arr_x, int arr_v);
double inner_product(double* x, double* y, int arr);
void vector_plus(double* x1, double* x2, int arr, double* y);
void vector_minus(double* x1, double* x2, int arr, double* y);
int check_clone(individual* ind, int arr);





//setting
void setting();
void experiment_setting();
void benchmark_setting();

//simulation
void make_netlist(individual* A, int arr);
void make_netlist2(individual* A, int arr);
void make_netlist3(individual* A, int arr);
void make_netlist5(individual* A, int arr);
void make_netlist_500MHz(individual* A, int arr);
void make_netlist_pulseVL(individual* A, int arr);
void make_netlist_test1(individual* A, int arr);
void sim_STL(individual* A, int arr);
double get_result(int i);
double get_result2(int i);
double getwidth(double X[][CHROM_SEC]);
double get_w_range(double X[][CHROM_SEC]);
double getsegments(double X[][CHROM_SEC]);
double get_around75(double X[][CHROM_SEC]);





