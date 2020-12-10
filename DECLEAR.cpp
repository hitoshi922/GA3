#include"Header.h"

individual P[POP];
individual nextP[POP];
individual Q[POP];
individual R[POP + POP];
individual best_P[GEN];
individual elite[2] = { 0,0 };

front F;

double gen_ave[GEN];
double upper_bound[DIM_SEC][CHROM_SEC];
double lower_bound[DIM_SEC][CHROM_SEC];

int DIM[CHROM_SEC];
int OBJ;
int CHROM;
int MIN_OR_MAX[OBJ_SEC];
int SEGMENT;
int L_NODE[DIM_SEC];
int QTY_SECTION;
double SECTION_LENGTH[DIM_SEC];

double len_lim;
