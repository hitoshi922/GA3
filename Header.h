#pragma once
#define _USE_MATH_DEFINES
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include<time.h>

#define ALGO 10 //0:simple  10:NSGA2

#define TYPE 0 //0:contenious  1:discrete
#define MODE 0//0:benchmark  1:experiment

constexpr int TRIAL = 1;

#define DIM_SEC 15
#define OBJ_SEC 5
#define CHROM_SEC 3
constexpr int POP = 50;
constexpr int GEN = 500;
constexpr double CROSSOVER_RATE = 1; // *100[%];
constexpr double MUTATION_RATE[CHROM_SEC] = { 0.05,0.1 }; // *100[%]

constexpr int discrete[] = { 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 };

#define EVALUATION 24
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

constexpr int CROSSOVER[CHROM_SEC] = { 11,102 };
/* CROSSOVER METHOD
OnePX:0  TwoPX:1  UniX:2
BLX_a:10  SBX:11
STL_2PX:100  STL_SX:101  STL_BLX_a:102
*/
constexpr int MUTATION[CHROM_SEC] = { 2,101 };
//�Ȃ�:0�@UNIM:1 PBM:2 100:STL_descrete 101:STL_continious;

#define TORNAMENT_SIZE 2

typedef struct INDIVIDUAL {
	double X[DIM_SEC][CHROM_SEC];
	double f[OBJ_SEC];
	double fitness[OBJ_SEC];
	double w[OBJ_SEC];
	double evaluation;
	int eval_tag; // 0:���]�� 1:�]���ς�

	int dominated;
	double crowding_distance;
	int dom_ind[POP + POP];
	int domination;
	int rank;
}individual;

typedef struct FRONT {
	individual F[POP + POP][POP + POP]; //[�����N][�̔z��]
	int num_arr[POP + POP]; //�����N���̐�
	int rank_arr; //�������N��
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

//algorithms
void basic_GA();
void NSGA2();

//crossover
void crossover(individual* B);

//evaluation
void evaluation(individual* A, int arr);

//main
void initialize3(individual* A, int arr);
void initialize2();
void initialize(individual* A, int arr);
void record(individual* ind, int arr, int cnt);
void init_ind(individual* ind, int arr);


//mutation
void mutation(individual* B);

//process
void random_seed();
void non_restored_extract(int max, int* x, int arr);
void tornament_select(individual* B, int max, int* x, int arr);
void roulette_select(individual* B, int limit, int* num, int arr);
void crowding_tornament_select(individual* B, int max, int* x, int arr);
int uniform_random(int a);
int find_header(FILE* fp, char* keyword);
int find_header2(FILE* fp, char* keyword);
void sort(individual* ind, int arr);
void d_sort(double* x, int arr);
void clear_array(double* x, int arr);
void clear_POP(individual* A, int arr);
void ind_cpy(individual* A, individual* B, int arr);
void individual_info(individual ind);


//setting
void setting();
void experiment_setting();
void benchmark_setting();

//simulation
void make_netlist(individual* A, int arr);
void make_netlist2(individual* A, int arr);
void sim_STL(individual* A, int arr);
double get_result(int i);
double getwidth(double X[][CHROM_SEC]);


