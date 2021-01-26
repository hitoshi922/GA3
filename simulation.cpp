#include"Header.h"
//void make_SVsig(char* pulse);

void make_netlist(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i;

	double W[10];
	double L[10];

	for (i = 0; i < arr; i++) {

		W[0] = A[i].X[0][0];
		W[1] = A[i].X[1][0];
		W[2] = A[i].X[2][0];
		W[3] = A[i].X[3][0];
		W[4] = A[i].X[4][0];
		W[5] = A[i].X[5][0];
		W[6] = A[i].X[6][0];
		W[7] = A[i].X[7][0];
		W[8] = A[i].X[8][0];
		W[9] = A[i].X[9][0];
		//L[0] = A[i].X[9];
		//L[1] = A[i].X[10];
		//L[2] = A[i].X[11];
		//L[3] = A[i].X[12];
		//L[4] = A[i].X[13];
		//L[5] = A[i].X[14];
		//L[6] = A[i].X[15];
		//L[7] = A[i].X[16];
		//L[8] = A[i].X[17];


		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}
		else {
			fprintf(fp, "*TML%3d\n"
				"RT2 Vout1 0 76\n"
				"T1 N001 0 N002 0 Td=65p Z0 = %.2f\n"
				"T2 N002 0 N003 0 Td=65p Z0 = %.2f\n"
				"T3 N003 0 N004 0 Td=65p Z0 = %.2f\n"
				"T4 N004 0 N005 0 Td=65p Z0 = %.2f\n"
				"T5 N005 0 OBS1 0 Td=65p Z0 = %.2f\n"
				"T6 OBS1 0 N006 0 Td=65p Z0 = %.2f\n"
				"T7 N006 0 N007 0 Td=65p Z0 = %.2f\n"
				"T8 N007 0 N008 0 Td=65p Z0 = %.2f\n"
				"T9 N008 0 OBS2 0 Td=65p Z0 = %.2f\n"
				"T10 OBS2 0 Vout1 0 Td=65p Z0 = %.2f\n"
				"C1 OBS1 0 10p\n"
				"C2 OBS2 0 10p\n"
				"R1 N001 Vin1 76\n"
				"V1 Vin1 0 PULSE(0 6.6 2n 20p 20p 8n))\n"
				".tran 12n\n"
				".save V(OBS1)\n"
				".save V(OBS2)\n"
				".backanno\n"
				".end\n"
				, i, W[0], W[1], W[2], W[3], W[4], W[5], W[6], W[7], W[8], W[9]);
		}
		fclose(fp);

	}

}

void make_netlist2(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i;

	double W[9];
	double L[9];
	double Rd;
	double RT;
	for (i = 0; i < arr; i++) {
		W[0] = A[i].X[0][0];
		W[1] = A[i].X[1][0];
		W[2] = A[i].X[2][0];
		W[3] = A[i].X[3][0];
		W[4] = A[i].X[4][0];
		W[5] = A[i].X[5][0];
		W[6] = A[i].X[6][0];
		W[7] = A[i].X[7][0];
		W[8] = A[i].X[8][0];
		Rd = A[i].X[9][0];
		RT = A[i].X[10][0];
		L[0] = A[i].X[0][1];
		L[1] = A[i].X[1][1];
		L[2] = A[i].X[2][1];
		L[3] = A[i].X[3][1];
		L[4] = A[i].X[4][1];
		L[5] = A[i].X[5][1];
		L[6] = A[i].X[6][1];
		L[7] = A[i].X[7][1];
		L[8] = A[i].X[8][1];
		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}
		else {
			fprintf(fp, "*TML%3d\n"
				"RT2 Vout1 0 %.0f\n"
				"T1 N001 0 N002 0 Td = %.2fp Z0 = %.2f\n"
				"T2 N002 0 N003 0 Td = %.2fp Z0 = %.2f\n"
				"T3 N003 0 OBS1 0 Td = %.2fp Z0 = %.2f\n"
				"T4 OBS1 0 N004 0 Td = %.2fp Z0 = %.2f\n"
				"T5 N004 0 N005 0 Td = %.2fp Z0 = %.2f\n"
				"T6 N005 0 OBS2 0 Td = %.2fp Z0 = %.2f\n"
				"T7 OBS2 0 N006 0 Td = %.2fp Z0 = %.2f\n"
				"T8 N006 0 N007 0 Td = %.2fp Z0 = %.2f\n"
				"T9 N007 0 Vout1 0 Td = %.2fp Z0 = %.2f\n"
				"V1 Vin1 0 PULSE(0 6.6 2n 20p 20p 8n)\n"
				"C1 OBS1 0 10p\n"
				"C2 OBS2 0 10p\n"
				"R1 N001 Vin1 %.0f\n"
				".tran 14n\n"
				".save V(OBS1)\n"
				".save V(OBS2)\n"
				".backanno\n"
				".end\n"
				, i, RT, L[0], W[0], L[1], W[1], L[2], W[2], L[3], W[3], L[4], W[4], L[5], W[5], L[6], W[6], L[7], W[7], L[8], W[8], Rd);
		}
		fclose(fp);
	}


}

void make_netlist3(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];
	int OBS2 = L_NODE[1];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;
	
	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0]; 
		RT = A[i].X[j+1][0];
		for (j = 0; j < DIM[1]; j++) {
			L[j] = A[i].X[j][1];
		}

		//1p問題仮対処
		for (j = 0; j < SEGMENT; j++) {
			if (L[j] < 0.5) {
				L[j] = 0.5;
			}
		}


		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 6.6 2n 20p 20p 8n)\n"
				"C1 N%03d 0 10p\n"
				"C2 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, DIM[1] + 1, RT, OBS1, OBS2, Rd);
			//伝送線路
			for (j = 0; j < DIM[1]; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.1fp Z0 = %.2f\n", 
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}
			//シミュレーションコマンド
			fprintf(fp, ".tran 14n\n"
				".save V(N%03d)\n"
				".save V(N%03d)\n"
				".backanno\n"
				".end\n", OBS1, OBS2);

		}
		fclose(fp);
	}
}

//複数コード化向け
void make_netlist4(individual* A, int arr) {
	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];
	int OBS2 = L_NODE[1];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;

	/*DIM[0]:w section1
	  DIM[1];w section2
	  DIM[2]:w section3
	  DIM[3]:l section1
	  DIM[4]:l section2
	  DIM[5];l section3
	  DIM[6]:resistance
	  に仮定*/


	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0];
		RT = A[i].X[j + 1][0];
		for (j = 0; j < DIM[1]; j++) {
			L[j] = A[i].X[j][1];
		}

		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 6.6 2n 20p 20p 8n)\n"
				"C1 N%03d 0 10p\n"
				"C2 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, DIM[1] + 1, RT, OBS1, OBS2, Rd);
			//伝送線路
			for (j = 0; j < DIM[1]; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.2fp Z0 = %.2f\n",
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}
			//シミュレーションコマンド
			fprintf(fp, ".tran 14n\n"
				".save V(N%03d)\n"
				".save V(N%03d)\n"
				".backanno\n"
				".end\n", OBS1, OBS2);

		}
		fclose(fp);
	}
}

//measコマンドを利用したシミュレーション
void make_netlist5(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];
	int OBS2 = L_NODE[1];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;

	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0];
		RT = A[i].X[j][0] + 20;

		for (j = 0; j < SEGMENT; j++) {
			//L[j] = A[i].X[j][1];
			L[j] = 56; //fix

		}

		//1p問題仮対処
		for (j = 0; j < SEGMENT; j++) {
			if (L[j] < 0.5) {
				L[j] = 0.5;
			}
		}


		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 6.6 1n 20p 20p 8n) Rser=20\n"
				"C1 N%03d 0 10p\n"
				"C2 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, SEGMENT + 1, RT, OBS1, OBS2, Rd);
			//伝送線路
			for (j = 0; j < SEGMENT; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.2fp Z0 = %.2f\n",
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}

			//理想波形
			fprintf(fp, "Videal NI11 0 PULSE(0 6.6 1n 20p 20p 8n)\n"
				"RdI NI12 NI11 76\n"
				"RTI 0 NI13 76\n"
				"T1CONV NI12 0 ideal1 0 Td = %.0fp Z0 = 76\n"
				"T2CONV ideal1 0 ideal2 0 Td = %.0fp Z0 = 76\n"
				"T3CONV ideal2 0 NI13 0 Td = %.0fp Z0 = 76\n",
				SECTION_LENGTH[0], SECTION_LENGTH[1], SECTION_LENGTH[2]);

			//シミュレーションコマンド
			fprintf(fp, ".tran 18n\n"
				".save V(N%03d)\n"
				".save V(N%03d)\n"
				".save V(ideal1)\n"
				".save V(ideal2)\n"
				".meas TRAN DIFF1 INTEG ABS(V(N%03d,ideal1)) FROM 0 TO 16n\n"
				".meas TRAN DIFF2 INTEG ABS(V(N%03d,ideal2)) FROM 0 TO 16n\n"
				".end\n", OBS1, OBS2, OBS1, OBS2);

		}
		fclose(fp);
	}
}

void make_netlist_500MHz(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];
	int OBS2 = L_NODE[1];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;

	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0];
		RT = A[i].X[j][0] + 20;

		for (j = 0; j < DIM[0] - 2; j++) {
			L[j] = 28; //fix
		}

		//1p問題仮対処
		for (j = 0; j < SEGMENT; j++) {
			if (L[j] < 0.5) {
				L[j] = 0.5;
			}
		}


		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 6.6 1n 200p 200p 0.8n 2n) Rser=20\n"
				"C1 N%03d 0 10p\n"
				"C2 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, DIM[0] - 1, RT, OBS1, OBS2, Rd);
			//伝送線路
			for (j = 0; j < DIM[0] - 2; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.2fp Z0 = %.2f\n",
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}

			//理想波形
			fprintf(fp, "Videal NI11 0 PULSE(0 6.6 1n 1p 1p 1n 2n)\n"
				"RdI NI12 NI11 76\n"
				"RTI 0 NI13 76\n"
				"T1CONV NI12 0 ideal1 0 Td = %.0fp Z0 = 76\n"
				"T2CONV ideal1 0 ideal2 0 Td = %.0fp Z0 = 76\n"
				"T3CONV ideal2 0 NI13 0 Td = %.0fp Z0 = 76\n",
				SECTION_LENGTH[0], SECTION_LENGTH[1], SECTION_LENGTH[2]);

			//シミュレーションコマンド
			fprintf(fp, ".tran 16n\n"
				".save V(N%03d)\n"
				".save V(N%03d)\n"
				".save V(ideal1)\n"
				".save V(ideal2)\n"
				".meas TRAN DIFF1 INTEG ABS(V(N%03d,ideal1)) FROM 11.28n TO 13.28n\n"
				".meas TRAN DIFF2 INTEG ABS(V(N%03d,ideal2)) FROM 11.504n TO 13.504n\n"
				".end\n", OBS1, OBS2, OBS1, OBS2);

		}
		fclose(fp);
	}
}

void make_SSTL(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];
	int OBS2 = L_NODE[1];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;

	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0];
		RT = A[i].X[j][0] + 20;

		for (j = 0; j < DIM[1]; j++) {
			//L[j] = A[i].X[j][1];
			L[j] = 100; //fix

		}

		//1p問題仮対処
		for (j = 0; j < SEGMENT; j++) {
			if (L[j] < 0.5) {
				L[j] = 0.5;
			}
		}


		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 2 1n 20p 200p 1n 2n) Rser=20\n"
				"C1 N%03d 0 10p\n"
				"C2 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, DIM[1] + 1, RT, OBS1, OBS2, Rd);
			//伝送線路
			for (j = 0; j < DIM[1]; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.2fp Z0 = %.2f\n",
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}

			//理想波形
			fprintf(fp, "Videal NI11 0 PULSE(0 6.6 1n 200p 200p 1n 2n)\n"
				"RdI NI12 NI11 76\n"
				"RTI 0 NI13 76\n"
				"T1CONV NI12 0 ideal1 0 Td = %.0fp Z0 = 76\n"
				"T2CONV ideal1 0 ideal2 0 Td = %.0fp Z0 = 76\n"
				"T3CONV ideal2 0 NI13 0 Td = %.0fp Z0 = 76\n",
				SECTION_LENGTH[0], SECTION_LENGTH[1], SECTION_LENGTH[2]);

			//シミュレーションコマンド
			fprintf(fp, ".tran 18n\n"
				".save V(N%03d)\n"
				".save V(N%03d)\n"
				".save V(ideal1)\n"
				".save V(ideal2)\n"
				".meas TRAN DIFF1 INTEG ABS(V(N%03d,ideal1)) FROM 0 TO 16n\n"
				".meas TRAN DIFF2 INTEG ABS(V(N%03d,ideal2)) FROM 0 TO 16n\n"
				".end\n", OBS1, OBS2, OBS1, OBS2);

		}
		fclose(fp);
	}
}

void make_netlist_test1(individual* A, int arr) {

	FILE* fp;
	errno_t error;
	char filename[100];
	int i, j;
	int OBS1 = L_NODE[0];

	double W[DIM_SEC];
	double L[DIM_SEC];
	double Rd;
	double RT;

	for (i = 0; i < arr; i++) {
		//WL等に個体のパラメータを割り当て
		for (j = 0; j < DIM[0] - 2; j++) {
			W[j] = A[i].X[j][0];
		}
		//次2つはダンピング&終端抵抗
		Rd = A[i].X[j][0];
		RT = A[i].X[j + 1][0];
		for (j = 0; j < DIM[1]; j++) {
			L[j] = A[i].X[j][1];
		}

		sprintf_s(filename, "C:/LTspice_results/TML%03d.net", i);
		error = fopen_s(&fp, filename, "w");
		if (error != 0) {
			printf("生成ファイル,netを開けませんでした。");
			exit(1);
		}

		else {

			/*******ネットリスト生成***********************
			OBS点は変数宣言時に指定（上）
			電源・負荷等を変更したら.saveコマンドのチェック
			伝送線路部分は基本的にいじらない
			**********************************************/

			//電源・負荷等
			fprintf(fp, "*TML%03d\n"
				"RT2 N%03d 0 %.0f\n"
				"V1 Vin1 0 PULSE(0 6.6 2n 20p 20p 8n)\n"
				"C1 N%03d 0 10p\n"
				"R1 Vin1 N001 %.0f\n"
				, i, DIM[1] + 1, RT, OBS1, Rd);
			//伝送線路
			for (j = 0; j < DIM[1]; j++) {
				fprintf(fp, "T%d N%03d 0 N%03d 0 Td = %.2fp Z0 = %.2f\n",
					j + 1, j + 1, j + 2, L[j], W[j]);//N000 はGNDのためノード名に使わない
			}
			//シミュレーションコマンド
			fprintf(fp, ".tran 14n\n"
				".save V(N%03d)\n"
				".backanno\n"
				".end\n", OBS1);

		}
		fclose(fp);
	}
}


void sim_STL(individual* A, int arr) {
	//バッチファイル使用版　
	//char command[100] = "C:/Users/kuboh/source/repos/SOGA/SOGA/sim_ind10.bat";
	//system(command);

	int i;
	char cmd[100];
	for (i = 0; i < arr; i++) {
		sprintf_s(cmd, "XVIIx64 -b C:/LTspice_results/TML%03d.net", i);
		//sprintf_s(cmd, "XVIIx64 -b -ascii C:/LTspice_results/TML%03d.net", i);
		if (A[i].eval_tag == 0) { //未評価ならシミュレーション
			system(cmd);
		}
	}


}


double get_result(int num) {
	FILE* fp;
	errno_t error;
	char filename[100];
	double* t;
	double* OBS1;
	double* OBS2;
	double* SVsignal;
	double* SVsignal2;
	int number_of_point;
	char NOP[64] = "Points:";
	char start[64] = "Values:";
	int i, dammy;
	double diff;
	int cnt;

	sprintf_s(filename, "C:/LTspice_results/TML%03d.raw", num);


	error = fopen_s(&fp, filename, "r");
	//error = fopen_s(&fp, "C:/Users/kuboh/Documents/LTspice_results/TML.raw", "r");
	if (error != 0) {
		printf("TML%03d,rawを開けませんでした。", num);
		exit(1);
	}
	//rawファイルのnumber of pointを読み込み
	find_header2(fp, NOP);
	fscanf_s(fp, "%d", &number_of_point);

	if (number_of_point == 0) {
		printf("check result file.\n");
		exit(1);
	}

	t = (double*)malloc(sizeof(double) * number_of_point);
	OBS1 = (double*)malloc(sizeof(double) * number_of_point);
	OBS2 = (double*)malloc(sizeof(double) * number_of_point);
	SVsignal = (double*)malloc(sizeof(double) * number_of_point);
	SVsignal2 = (double*)malloc(sizeof(double) * number_of_point);

	find_header2(fp, start);
	for (i = 0; i < number_of_point; i++) {
		fscanf_s(fp, "%d %lf\n%lf\n%lf", &dammy, &t[i], &OBS1[i], &OBS2[i]);
	}

	//superviser_signal
	cnt = 0;
	do {
		if ((t[cnt] > 2.28e-9) && (t[cnt] < 10.28e-9))
			SVsignal[cnt] = 3.3;
		else
			SVsignal[cnt] = 0.0;
		if ((t[cnt] > 2.484e-9) && (t[cnt] < 10.484e-9))
			SVsignal2[cnt] = 3.3;
		else
			SVsignal2[cnt] = 0.0;
		cnt++;
	} while (cnt < number_of_point);

	diff = 0;
	cnt = 0;
	do {
		if ((t[cnt] > 2e-9) && (t[cnt] < 12e-9)) {
			diff += fabs(OBS1[cnt] - SVsignal[cnt]) * (t[cnt + 1] - t[cnt]);
			diff += fabs(OBS2[cnt] - SVsignal2[cnt]) * (t[cnt + 1] - t[cnt]);
		}
		cnt++;
		if (t[cnt] > 12e-9)break;
	} while (cnt < number_of_point);

	fclose(fp);


	free(t);
	free(OBS1);
	free(OBS2);
	free(SVsignal);
	free(SVsignal2);

	return diff;
}

//logファイルからdiffを読み取る方式
double get_result2(int num) {
	double diff1;
	double diff2;
	FILE* fp;
	errno_t error;
	char filename[100];
	int dammy;
	char buf1[64] = "diff1:";
	char buf2[64] = "diff2:";


	sprintf_s(filename, "C:/LTspice_results/TML%03d.log", num);

	error = fopen_s(&fp, filename, "r");
	if (error != 0) {
		printf("TML%03d,logを開けませんでした。", num);
		exit(1);
	}

	//diff1読み取り
	find_header2(fp, buf1);
	while ((dammy = fgetc(fp)) != 61) { //'='の値が61
	}
	fscanf_s(fp, "%lf", &diff1);
	//diff2読み取り
	find_header2(fp, buf2);
	while ((dammy = fgetc(fp)) != 61) { 
	}
	fscanf_s(fp, "%lf", &diff2);

	fclose(fp);

	return diff1 + diff2;
}

double getwidth(double X[][CHROM_SEC]) {
	int i;
	double min = 300;
	for (i = 0; i < SEGMENT; i++) {
		if (min > X[i][0]) {
			min = X[i][0];
		}
	}

	return min;
}

double get_w_range(double X[][CHROM_SEC]) {
	int i;
	double min, max;
	min = 1000;
	max = 0; //エラー避けのための適当初期値
	for (i = 0; i < SEGMENT; i++) {
		if (min > X[i][0]) {
			min = X[i][0];
		}
		if (max < X[i][0]) {
			max = X[i][0];
		}
	}
	return max - min;
}

double get_around75(double X[][CHROM_SEC]) {
	int i;
	double min, max;
	double cost;
	min = 1000;
	max = 0; //エラー避けのための適当初期値
	for (i = 0; i < SEGMENT; i++) {
		if (min > X[i][0]) {
			min = X[i][0];
		}
		if (max < X[i][0]) {
			max = X[i][0];
		}
	}
	cost = (max - 76) * (max - 76) + (76 - min) * (76 - min);
	return cost;
}

double getsegments(double X[][CHROM_SEC]) {
	int i;
	double count_segments = SEGMENT;
	for (i = 0; i < SEGMENT - 1; i++) {
		if (X[i][0] == X[i + 1][0]) {
			count_segments--;
		}
	}

	return count_segments;
}

//改修中
//void make_SVsig(char* pulse) {
//	int i;
//	char text[100] = "PULSE(0 6.6 SV 20p 20p 8n)";
//	double freq = 300e6;
//	double amp = 3.3;
//	double r = 1 / freq;
//	double endtm = 20;
//	double check;
//	int SV[6];
//
//	sprintf_s(pulse, 26, "PULSE(0 %f SV 20p 20p 8n)", amp);
//
//}