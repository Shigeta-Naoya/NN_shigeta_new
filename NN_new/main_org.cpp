#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Header.h"
#pragma warning(disable: 4996)

//NN(層数、素子数を指定)
//層数2、一層の素子数4
//入力数3



int main(void) {

	//入力次元数，出力次元数，データ数，未学習データ数(テストデータ)
	//exor
	int in = 3, out = 1, data = 6, ldata = 2;
	//層数、素子数
	int layer = 2, element = 4;

	int i, j, k, count = 0;		//作業用

	double E;							//二乗誤差

	int learn = 1;						//学習方法(0・・・逐次,1・・・一括)

	FILE* fp;//教師データと未学習データ用


	printf("NNの層数を指定・・・");
	scanf_s("%d", &layer);
	printf("NNの1層当たりの素子数を指定・・・");
	scanf_s("%d", &element);
	do {
		printf("学習方法の指定(0・・・逐次,　　1・・・一括)");
		scanf_s("%d", &learn);
	} while (learn != 0 && learn != 1);




	printf("NN\n");
	//***********各値のための領域確保と初期化***************
	eldata el;
	//教師データ
	el.Tin = (double**)malloc(sizeof(double*) * data);
	el.Tin[0] = (double*)malloc(sizeof(double) * data * in);				//2次元
	for (i = 1; i < data; i++) {
		el.Tin[i] = el.Tin[0] + i * in;
	}
	//教師出力データ
	el.Tout = (double**)malloc(sizeof(double) * data);
	el.Tout[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		el.Tout[i] = el.Tout[0] + i * out;
	}
	//exor
	//if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
	//EMG
	//if ((fp = fopen("EMGdata.txt", "r")) == NULL) {
	if ((fp = fopen("EXOR.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}

	printf("教師データ読み込み\n");
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf %lf", &el.Tin[i][0], &el.Tin[i][1], &el.Tin[i][2], &el.Tout[i])) != EOF) {
		printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i]);
		i++;
	}*/
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++) {
			//教師入力データ読み込み
			if (fscanf_s(fp, "%lf", &el.Tin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
			}
		}
		for (k = 0; k < out; k++) {
			//教師出力データ読み込み
			if (fscanf_s(fp, "%lf", &el.Tout[i][k]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
			}
		}

	}

	//}
	fclose(fp);


	//int count = 0;
	for (i = 0; i < data; i++) {
		//for (j = 0; j < in; j++) {
			//el.Tin[i][j] = count;
		printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i][0]);
		count++;
		//}
	}

	//出力層二次元に対応
	el.out = (double**)malloc(sizeof(double) * data);
	el.out[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		el.out[i] = el.out[0] + i * out;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < out; j++) {
			el.out[i][j] = 0;
		}
	}

	//中間層
	el.mid = (double***)malloc(sizeof(double**) * data);
	el.mid[0] = (double**)malloc(sizeof(double*) * data * layer);
	el.mid[0][0] = (double*)malloc(sizeof(double) * data * layer * element);	//3次元
	for (i = 0; i < data; i++) {
		el.mid[i] = el.mid[0] + i * layer;
		for (j = 0; j < layer; j++) {
			el.mid[i][j] = el.mid[0][0] + i * layer * element + j * element;
		}
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < layer; j++) {
			for (k = 0; k < element; k++) {
				el.mid[i][j][k] = 0;
				//printf("%f", el.mid[i][j][k]);
			}
		}
	}
	//printf("NN\n");



	errordata error;
	error.in = (double**)malloc(sizeof(double*) * data);
	error.in[0] = (double*)malloc(sizeof(double) * data * in);				//2次元
	for (i = 1; i < data; i++) {
		error.in[i] = error.in[0] + i * in;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++) {
			error.in[i][j] = 0;
			//printf("error.in %f", error.in[i][j]);
		}
	}

	error.mid = (double***)malloc(sizeof(double**) * data);
	error.mid[0] = (double**)malloc(sizeof(double*) * data * layer);
	error.mid[0][0] = (double*)malloc(sizeof(double) * data * layer * (element + 1));	//3次元
	for (i = 0; i < data; i++) {
		error.mid[i] = error.mid[0] + i * layer;
		for (j = 0; j < layer; j++) {
			error.mid[i][j] = error.mid[0][0] + i * layer * element + j * (element + 1);
		}
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < layer; j++) {
			for (k = 0; k < element + 1; k++) {
				error.mid[i][j][k] = 0;
				//printf("errormid %f  ", error.mid[i][j][k]);
			}
			//printf("\n");
		}
		//printf("\n");
	}
	//printf("\n");

	//出力層の次元数に対応
	error.out = (double**)malloc(sizeof(double) * data);
	error.out[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < data; i++) {
		error.out[i] = error.out[0] + i * out;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < out; j++) {
			error.out[i][j] = 0;
		}
	}


	//重みの領域確保と初期化
	wdata w;
	w.w1 = (double**)malloc(sizeof(double*) * in + 1);
	w.w1[0] = (double*)malloc(sizeof(double) * (in + 1) * element);						//2次元
	for (i = 1; i < (in + 1); i++) {
		w.w1[i] = w.w1[0] + i * element;
	}

	w.w2 = (double***)malloc(sizeof(double**) * (layer - 1));
	w.w2[0] = (double**)malloc(sizeof(double*) * (layer - 1) * (element + 1));
	w.w2[0][0] = (double*)malloc(sizeof(double) * (layer - 1) * (element + 1) * element);	//3次元
	for (i = 0; i < (layer - 1); i++) {
		w.w2[i] = w.w2[0] + i * (element + 1);
		for (j = 0; j < (element + 1); j++) {
			w.w2[i][j] = w.w2[0][0] + i * (element + 1) * element + j * element;
		}
	}

	//出力層の次元数の変化に対応
	w.w3 = (double**)malloc(sizeof(double*) * (element + 1));
	w.w3[0] = (double*)malloc(sizeof(double) * (element + 1) * out);						//2次元
	for (i = 1; i < (element + 1); i++) {
		w.w3[i] = w.w3[0] + i * out;
	}

	//乱数の初期化
	srand((unsigned int)time(NULL));

	printf("重みの初期値\n");
	printf("w1\n");
	for (i = 0; i < in + 1; i++) {
		for (j = 0; j < element; j++) {
			w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w1[i][j] = 0.5;
			printf("%f ", w.w1[i][j]);
		}
		printf("\n");
	}
	//1から2
	printf("w2\n");
	for (i = 0; i < layer - 1; i++) {
		for (j = 0; j < element + 1; j++) {
			for (k = 0; k < element; k++) {
				w.w2[i][j][k] = rand() / double(RAND_MAX) * 2 - 1;
				//w.w2[i][j][k] = 0.5;
				printf("%f ", w.w2[i][j][k]);
			}
			printf("\n");
		}
	}
	//2から出力
	//出力の数の変化に対応
	printf("w3\n");
	for (i = 0; i < element + 1; i++) {
		for (j = 0; j < out; j++) {
			w.w3[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w3[i] = 0.5;
			printf("%f ", w.w3[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");


	if (learn == 0) {
		printf("逐次学習\n");
	}
	else {
		printf("一括学習\n");
	}


	//********************使うデータの設定が完了*****************************************


	if (learn == 1) {
		//*******************************一括学習********************************************
		//以下をループし、順方向、逆方向をくりかえして学習
		do {
			//データ数分順方向に伝搬させる
			//printf("colel\n");
			colel(in, layer, element, out, data, &el, &w);

			//誤差の計算二乗誤差errorと普通の誤差error.out
			E = colerror(out, &el, data, &error);
			//if (count % 100 == 0) {
			printf("\ncount = %d	E = %lf\n", count, E);
			//}
			//逆方向
			spread(in, layer, element, out, data, &el, &error, &w);


			count++;
		} while (count < N && E > setE);
		//********************************************************************************************************************
	}

	//*******************************逐次学習********************************************
	else {
		do {
			//データを一個ずつ読んで順方向，誤差，逆伝搬，重み更新を繰り返す
			//データ数分のループ
			int i, datanum;
			for (i = 0; i < data; i++) {
				//ランダムな順番で学習させる
				datanum = rand() % data;
				//順方向
				colel_online(in, layer, element, out, datanum, &el, &w);
				/*for (int i = 0; i < out; i++) {
					printf("%f ",el.out[datanum][i]);
				}
				printf("\n");*/

				//誤差計算
				E = colerror_online(out, &el, datanum, &error);
				//if (count % 100 == 0) {
					//printf("\ncount = %d	E = %lf\n", count, E);
				//}
				//逆伝搬と重み更新
				spread_online(in, layer, element, out, datanum, &el, &error, &w);
			}
			E = colerror(out, &el, data, &error);
			printf("\ncount = %d	E = %lf\n", count, E);
			count++;
		} while (count < N && E > setE);
	}
	//********************************************************************************************************************

	printf("学習の終了\n");
	printf("count = %d  error = %f\n", count, E);
	if (count == N) {
		printf("count = %d\n", count);
	}
	else if (E <= setE) {
		printf("学習を終えました\n");
	}
	for (i = 0; i < data; i++) {
		printf("Tout  ");
		for (j = 0; j < out; j++) {
			printf("%f  ", el.Tout[i][j]);
		}
		printf("\n");
		printf("out   ");
		for (j = 0; j < out; j++) {
			printf("%f  ", el.out[i][j]);
		}
		printf("\n");
	}
	printf("error = %f\n", E);


	//領域の解放

	free(error.out);
	free(error.mid[0][0]);
	free(error.mid[0]);
	free(error.mid);
	free(error.in[0]);
	free(error.in);
	//printf("cerror = %f\n", E);



	free(el.Tout[0]);
	free(el.Tout);
	free(el.Tin[0]);
	free(el.Tin);
	//printf("derror = %f\n", E);


	//エンター待ち
	do {
		//printf("エンターキーを押して下さい。");
	} while (getchar() != '\n');


	//未学習データを取り込む
	//*********************************************************************************
	el.lin = (double**)malloc(sizeof(double*) * ldata);
	el.lin[0] = (double*)malloc(sizeof(double) * data * in);				//2次元
	for (i = 1; i < ldata; i++) {
		el.lin[i] = el.lin[0] + i * in;
	}
	el.lout = (double**)malloc(sizeof(double) * ldata);
	el.lout[0] = (double*)malloc(sizeof(double) * data * out);				//2次元
	for (i = 1; i < ldata; i++) {
		el.lout[i] = el.lout[0] + i * out;
	}

	//if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
	//	printf("file open error!!\n");
	//	exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	//}

	printf("未学習データ\n");
	//exor
	//if ((fp = fopen("Test_Data.txt", "r")) == NULL) {
	//EMG
	//if ((fp = fopen("EMGdatatest.txt", "r")) == NULL) {
	if ((fp = fopen("unlearn.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf", &el.lin[i][0], &el.lin[i][1], &el.lin[i][2])) != EOF) {
		printf("%f %f %f\n", el.lin[i][0], el.lin[i][1], el.lin[i][2]);
		i++;
	}*/

	for (i = 0; i < ldata; i++) {
		for (j = 0; j < in; j++) {
			//教師入力データ読み込み
			if (fscanf_s(fp, "%lf", &el.lin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				break;
			}
		}
	}

	fclose(fp);


	//未学習データに対する計算
	lcolel(in, layer, element, out, ldata, &el, &w);

	printf("未学習データに対する出力\n");
	for (i = 0; i < ldata; i++) {//データ数
		for (j = 0; j < out; j++) {//出力次元数
			printf("%f ", el.lout[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	//printf("aerror = %f\n", E);
	free(w.w3[0]);
	free(w.w3);
	//printf("aerror = %f\n", E);
	free(w.w2[0][0]);
	free(w.w2[0]);
	free(w.w2);
	//printf("aerror = %f\n", E);
	free(w.w1[0]);
	//free(w.w1);
	//printf("berror = %f\n", E);

	free(el.lin[0]);
	free(el.lin);
	free(el.mid[0][0]);
	free(el.mid[0]);
	free(el.mid);
	free(el.out[0]);
	free(el.out);

	//エンター待ち
	do {
		printf("終了します。エンターキーを押して下さい。");
	} while (getchar() != '\n');

	return 0;
}



//順方向//一括
//	int in						入力次元数
//	int layer,					層数
//	int element,				素子数
//	int out,					出力次元数
//	int data,					学習データ数
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct wdata* w,			各層の間の重み
void colel(int in, int layer, int element, int out, int data, struct eldata* el, struct wdata* w) {

	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//作業用


	//各値の初期化をする
	for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
		for (layerNum = 0; layerNum < layer; layerNum++) {//層数分
			for (elementNum = 0; elementNum < element; elementNum++) {//素子数分
				el->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
		for (outNum = 0; outNum < out; outNum++) {//出力次元分
			el->out[dataNum][outNum] = 0;
		}
	}

	//デバッグ用
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (outNum = 0; outNum < out; outNum++) {//出力次元分
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("重みの経過\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1から2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2から出力
	////出力の数の変化に対応
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");

	for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
		
		//******************************入力から中間層******************************
		for (elementNum = 0; elementNum < element; elementNum++) {//中間層の素子数
			for (inNum = 0; inNum < in + 1; inNum++) {//入力次元数+バイアス分
				if (inNum == in) {
					el->mid[dataNum][0][elementNum] += w->w1[inNum][elementNum];
				}
				else {
					el->mid[dataNum][0][elementNum] += el->Tin[dataNum][inNum] * w->w1[inNum][elementNum];
				}
			}
			//シグモイドで素子の出力値
			el->mid[dataNum][0][elementNum] = sigmoid(el->mid[dataNum][0][elementNum]);
		}
		//******************************中間層******************************
		for (layerNum = 0; layerNum < layer-1; layerNum++) {//中間層層数-1
			for (nextNum = 0; nextNum < element; nextNum++) {//送る先の中間層の素子数
				for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
					if (elementNum == element) {
						el->mid[dataNum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
					}
					else {
						el->mid[dataNum][layerNum + 1][nextNum] += el->mid[dataNum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
					}
				}
				//シグモイドで素子の出力値
				el->mid[dataNum][layerNum + 1][nextNum] = sigmoid(el->mid[dataNum][layerNum + 1][nextNum]);
			}
		}
		//******************************中間層から出力******************************
		for (outNum = 0; outNum < out; outNum++) {//出力層の次元数
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
				if (elementNum == element) {
					el->out[dataNum][outNum] += w->w3[elementNum][outNum];
				}
				else {
					el->out[dataNum][outNum] += el->mid[dataNum][layer - 1][elementNum] * w->w3[elementNum][outNum];
				}
			}
			//シグモイドで素子の出力値
			el->out[dataNum][outNum] = sigmoid(el->out[dataNum][outNum]);
		}
	}

	//******************************順方向の計算の終了******************************

	//デバッグ用
	/*for (dataNum = 0; dataNum < data; dataNum++) {
		for (layerNum = 0; layerNum < layer; layerNum++) {
			for (elementNum = 0; elementNum < element; elementNum++) {
				printf("%f ",el->mid[dataNum][layerNum][elementNum] );
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");

	for (dataNum = 0; dataNum < data; dataNum++) {
		for (outNum = 0; outNum < out; outNum++) {
			printf("%f  ", el->out[dataNum][outNum]);
		}
	}

	printf("\n");*/

	//	for (dataNum = 0; dataNum < data; dataNum++) {
	//		//for (outNum = 0; outNum < out; outNum++) {
	//			printf("%f  ", el->out[dataNum][0]);
	//		//}
	//	}
	//	printf("\n");]
}


//シグモイド関数の値を返す
double sigmoid(double s) {
	return 1 / (1 + exp(-s));
}

//一括学習時の誤差
//	int out,					出力次元数
//	int data,					学習データ数
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct errordata* error,	各素子に伝搬される誤差
double colerror(int out,struct eldata *el,int data,struct errordata *error) {
	int dataNum, outNum;//作業用
	double E = 0;

	for (dataNum = 0; dataNum < data;dataNum++) {//データ数分
		for (outNum = 0; outNum < out; outNum++) {//出力次元数分
			error->out[dataNum][outNum] = el->out[dataNum][outNum] -el->Tout[dataNum][outNum];
			E += pow(error->out[dataNum][outNum], 2);//損失関数
		}
	}
	return E;
}


//逆伝搬-一括学習
//	int in						入力次元数
//	int layer,					層数
//	int element,				素子数
//	int out,					出力次元数
//	int data,					学習データ数
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct errordata* error,	各素子に伝搬される誤差
//	struct wdata* w,			各層の間の重み
void spread(int in, int layer, int element, int out, int data, struct eldata* el, struct errordata* error, struct wdata* w) {
	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//作業用
	double totalerror;
	//初期
	for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
		for (layerNum = 0; layerNum < layer; layerNum++) {//層数分
			for (elementNum = 0; elementNum < element; elementNum++) {//素子数分
				error->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
	}
	//伝搬と更新を行っていく
	//******************************出力から中間層******************************
	//誤差の伝搬
	for (dataNum = 0; dataNum < data; dataNum++) {//データ数分	
		for (outNum = 0; outNum < out; outNum++) {//出力次元数
			for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスへの伝搬は必要ない))
				error->mid[dataNum][layerNum-1][elementNum] += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * w->w3[elementNum][outNum];
			}
		}
	}

	//重みの更新
	layerNum = layer;
	for (outNum = 0; outNum < out; outNum++) {//出力次元数
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//素子数+バイアス
			if (elementNum == element) {//バイアスの更新
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//データ数分	
					totalerror += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * 1;
				}
				//ここで更新
				w->w3[elementNum][outNum] += -2*ep * totalerror / data;
			}
			else {//バイアス以外の更新
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//データ数分	
					totalerror += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * el->mid[dataNum][layerNum - 1][elementNum];
				}
				//ここで更新
				w->w3[elementNum][outNum] += -2*ep * totalerror / data;
			}
		}
	}

	//******************************中間層******************************
	for (layerNum = layer - 1; layerNum > 0; layerNum -- ) {//層数分(スタートは最後の中間層インデックスはlayerNum-1 w2のインデックスばさらに-1)
		//誤差の伝搬
		for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
			for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスは含まない)
				for (nextNum = 0; nextNum < element; nextNum++) {//左側の素子数分(バイアスは含まない)
					error->mid[dataNum][layerNum - 1][nextNum] 
						+= error->mid[dataNum][layerNum][elementNum] 
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * w->w2[layerNum - 1][nextNum][elementNum];
				}
			}
		}

		//重みの更新
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスは含まない)
			for (nextNum = 0; nextNum < element + 1; nextNum++) {//左側の素子数分+バイアス
				if (nextNum == element) {//バイアスの更新
					totalerror = 0;
					for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
						totalerror += error->mid[dataNum][layerNum][elementNum]
							* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * 1;
					}
					//ここで更新
					w->w2[layerNum-1][nextNum][elementNum]+= -ep * totalerror / data;
				}
				else {//バイアス以外の更新
					totalerror = 0;
					for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
						totalerror += error->mid[dataNum][layerNum][elementNum]
							* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * el->mid[dataNum][layerNum - 1][nextNum];
					}
					//ここで更新
					w->w2[layerNum-1][nextNum][elementNum] += -ep * totalerror / data;
				}
			}
		}
	}

	//******************************中間層から入力******************************
	layerNum = 0;//一応
	//誤差の伝搬は必要ない
	//重みの更新
	for (elementNum = 0; elementNum < element; elementNum++) {//中間層の素子数(バイアスなし)
		for (inNum = 0; inNum < in + 1; inNum++) {//入力次元数+1(バイアス)
			if (inNum == in) {//バイアスの更新
				totalerror = 0;
				for (dataNum = 0; dataNum < data;dataNum++) {//データ数分
					totalerror += error->mid[dataNum][layerNum][elementNum]
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * 1;
				}
				//ここで更新
				w->w1[inNum][elementNum] += -ep * totalerror / data;
			}
			else {//バイアス以外の更新
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
					totalerror += error->mid[dataNum][layerNum][elementNum]
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * el->Tin[dataNum][inNum];
				}
				//ここで更新
				w->w1[inNum][elementNum] += -ep * totalerror / data;
			}
		}
	}

	//重み更新の完了
}




//逐次学習のため　1データごと
//	int in						入力次元数
//	int layer,					層数
//	int element,				素子数
//	int out,					出力次元数
//	int data,					使用する学習データのインデックス
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct wdata* w,			各層の間の重み
void colel_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct wdata* w) {
	//int dataNum, 
	int inNum, layerNum, elementNum, nextNum, outNum;	//作業用


	//各値の初期化をする
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
	for (layerNum = 0; layerNum < layer; layerNum++) {//層数分
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分
			el->mid[datanum][layerNum][elementNum] = 0;
		}
	}
	for (outNum = 0; outNum < out; outNum++) {//出力次元分
		el->out[datanum][outNum] = 0;
	}
	//}

	//デバッグ用
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (outNum = 0; outNum < out; outNum++) {//出力次元分
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("重みの経過\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1から2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2から出力
	////出力の数の変化に対応
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");


	//逐次ではデータ数分のループを除外datanumをつかう
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分

	//******************************入力から中間層******************************
	for (elementNum = 0; elementNum < element; elementNum++) {//中間層の素子数
		for (inNum = 0; inNum < in + 1; inNum++) {//入力次元数+バイアス分
			if (inNum == in) {
				el->mid[datanum][0][elementNum] += w->w1[inNum][elementNum];
			}
			else {
				el->mid[datanum][0][elementNum] += el->Tin[datanum][inNum] * w->w1[inNum][elementNum];
			}
		}
		//シグモイドで素子の出力値
		el->mid[datanum][0][elementNum] = sigmoid(el->mid[datanum][0][elementNum]);
	}
	//******************************中間層******************************
	for (layerNum = 0; layerNum < layer - 1; layerNum++) {//中間層層数-1
		for (nextNum = 0; nextNum < element; nextNum++) {//送る先の中間層の素子数
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
				if (elementNum == element) {
					el->mid[datanum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
				}
				else {
					el->mid[datanum][layerNum + 1][nextNum] += el->mid[datanum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
				}
			}
			//シグモイドで素子の出力値
			el->mid[datanum][layerNum + 1][nextNum] = sigmoid(el->mid[datanum][layerNum + 1][nextNum]);
		}
	}
	//******************************中間層から出力******************************
	for (outNum = 0; outNum < out; outNum++) {//出力層の次元数
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
			if (elementNum == element) {
				el->out[datanum][outNum] += w->w3[elementNum][outNum];
			}
			else {
				el->out[datanum][outNum] += el->mid[datanum][layer - 1][elementNum] * w->w3[elementNum][outNum];
			}
		}
		//シグモイドで素子の出力値
		el->out[datanum][outNum] = sigmoid(el->out[datanum][outNum]);
	}
	//}

	//******************************順方向の計算の終了******************************

	//デバッグ用
	/*for (dataNum = 0; dataNum < data; dataNum++) {
		for (layerNum = 0; layerNum < layer; layerNum++) {
			for (elementNum = 0; elementNum < element; elementNum++) {
				printf("%f ",el->mid[dataNum][layerNum][elementNum] );
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");

	for (dataNum = 0; dataNum < data; dataNum++) {
		for (outNum = 0; outNum < out; outNum++) {
			printf("%f  ", el->out[dataNum][outNum]);
		}
	}

	printf("\n");*/

	//	for (dataNum = 0; dataNum < data; dataNum++) {
	//		//for (outNum = 0; outNum < out; outNum++) {
	//			printf("%f  ", el->out[dataNum][0]);
	//		//}
	//	}
	//	printf("\n");]
}



double colerror_online(int out, struct eldata* el, int datanum, struct errordata* error) {
	
	int outNum;
	double E = 0;

	for (outNum = 0; outNum < out; outNum++) {
		error->out[datanum][outNum] = el->out[datanum][outNum]-el->Tout[datanum][outNum];
		E += pow(error->out[datanum][outNum], 2);
	}
	return E;
}

//逐次学習　伝搬と重み更新
//逐次学習の際の誤差(各データごと)
//	int in						入力次元数
//	int layer,					層数
//	int element,				素子数
//	int out,					出力次元数
//	int data,					使用する学習データのインデックス
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct errordata* error,	各素子に伝搬される誤差
//	struct wdata* w,			各層の間の重み
void spread_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct errordata* error, struct wdata* w) {
	//int dataNum,
	int	inNum, layerNum, elementNum, nextNum, outNum;	//作業用
	double totalerror;
	//初期
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
	for (layerNum = 0; layerNum < layer; layerNum++) {//層数分
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分
			error->mid[datanum][layerNum][elementNum] = 0;
		}
	}
	//}
	//逐次ではデータ数分のループを除外datanumをつかう
	//伝搬と更新を行っていく
	//******************************出力から中間層******************************
	//誤差の伝搬
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分	
	for (outNum = 0; outNum < out; outNum++) {//出力次元数
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスへの伝搬は必要ない))
			error->mid[datanum][layerNum - 1][elementNum] += error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * w->w3[elementNum][outNum];
		}
	}
	//}

	//重みの更新
	layerNum = layer;
	for (outNum = 0; outNum < out; outNum++) {//出力次元数
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//素子数+バイアス
			if (elementNum == element) {//バイアスの更新
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分	
				totalerror = error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * 1;
				//}
				//ここで更新
				w->w3[elementNum][outNum] += -2 * ep * totalerror;
			}
			else {//バイアス以外の更新
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分	
				totalerror = error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * el->mid[datanum][layerNum - 1][elementNum];
				//}
				//ここで更新
				w->w3[elementNum][outNum] += -2 * ep * totalerror;
			}
		}
	}

	//******************************中間層******************************
	for (layerNum = layer - 1; layerNum > 0; layerNum--) {//層数分(スタートは最後の中間層インデックスはlayerNum-1 w2のインデックスばさらに-1)
		//誤差の伝搬
		//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスは含まない)
			for (nextNum = 0; nextNum < element; nextNum++) {//左側の素子数分(バイアスは含まない)
				error->mid[datanum][layerNum - 1][nextNum]
					+= error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * w->w2[layerNum - 1][nextNum][elementNum];
			}
		}
		//}

		//重みの更新
		for (elementNum = 0; elementNum < element; elementNum++) {//素子数分(バイアスは含まない)
			for (nextNum = 0; nextNum < element + 1; nextNum++) {//左側の素子数分+バイアス
				if (nextNum == element) {//バイアスの更新
					//totalerror = 0;
					//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
					totalerror = error->mid[datanum][layerNum][elementNum]
						* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * 1;
					//}
					//ここで更新
					w->w2[layerNum - 1][nextNum][elementNum] += -ep * totalerror;
				}
				else {//バイアス以外の更新
					//totalerror = 0;
					//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
					totalerror = error->mid[datanum][layerNum][elementNum]
						* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * el->mid[datanum][layerNum - 1][nextNum];
					//}
					//ここで更新
					w->w2[layerNum - 1][nextNum][elementNum] += -ep * totalerror;
				}
			}
		}
	}

	//******************************中間層から入力******************************
	layerNum = 0;//一応
	//誤差の伝搬は必要ない
	//重みの更新
	for (elementNum = 0; elementNum < element; elementNum++) {//中間層の素子数(バイアスなし)
		for (inNum = 0; inNum < in + 1; inNum++) {//入力次元数+1(バイアス)
			if (inNum == in) {//バイアスの更新
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
				totalerror = error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * 1;
				//}
				//ここで更新
				w->w1[inNum][elementNum] += -ep * totalerror;
			}
			else {//バイアス以外の更新
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//データ数分
				totalerror = error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * el->Tin[datanum][inNum];
				//}
				//ここで更新
				w->w1[inNum][elementNum] += -ep * totalerror;
			}
		}
	}

	//重み更新の完了
}

//未学習データに対するし出力を求める
//一括の順方向のデータ数などを変えればよい
//	int in						入力次元数
//	int layer,					層数
//	int element,				素子数
//	int out,					出力次元数
//	int data,					未学習学習データ数
//	struct eldata* el,			ネットワークの各素子の出力を保持する構造体へのポインタ
//	struct wdata* w,			各層の間の重み
void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w) {
	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//作業用


	//各値の初期化をする
	for (dataNum = 0; dataNum < ldata; dataNum++) {//データ数分
		for (layerNum = 0; layerNum < layer; layerNum++) {//層数分
			for (elementNum = 0; elementNum < element; elementNum++) {//素子数分
				el->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
		for (outNum = 0; outNum < out; outNum++) {//出力次元分
			el->lout[dataNum][outNum] = 0;
		}
	}

	//デバッグ用
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//データ数分
	//	for (outNum = 0; outNum < out; outNum++) {//出力次元分
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("重みの経過\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1から2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2から出力
	////出力の数の変化に対応
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");



	for (dataNum = 0; dataNum < ldata; dataNum++) {//データ数分

		//******************************入力から中間層******************************
		for (elementNum = 0; elementNum < element; elementNum++) {//中間層の素子数
			for (inNum = 0; inNum < in + 1; inNum++) {//入力次元数+バイアス分
				if (inNum == in) {
					el->mid[dataNum][0][elementNum] += w->w1[inNum][elementNum];
				}
				else {
					el->mid[dataNum][0][elementNum] += el->lin[dataNum][inNum] * w->w1[inNum][elementNum];
				}
			}
			//シグモイドで素子の出力値
			el->mid[dataNum][0][elementNum] = sigmoid(el->mid[dataNum][0][elementNum]);
		}
		//******************************中間層******************************
		for (layerNum = 0; layerNum < layer - 1; layerNum++) {//中間層層数-1
			for (nextNum = 0; nextNum < element; nextNum++) {//送る先の中間層の素子数
				for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
					if (elementNum == element) {
						el->mid[dataNum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
					}
					else {
						el->mid[dataNum][layerNum + 1][nextNum] += el->mid[dataNum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
					}
				}
				//シグモイドで素子の出力値
				el->mid[dataNum][layerNum + 1][nextNum] = sigmoid(el->mid[dataNum][layerNum + 1][nextNum]);
			}
		}
		//******************************中間層から出力******************************
		for (outNum = 0; outNum < out; outNum++) {//出力層の次元数
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//中間層の素子数+バイアス分
				if (elementNum == element) {
					el->lout[dataNum][outNum] += w->w3[elementNum][outNum];
				}
				else {
					el->lout[dataNum][outNum] += el->mid[dataNum][layer - 1][elementNum] * w->w3[elementNum][outNum];
				}
			}
			//シグモイドで素子の出力値
			el->lout[dataNum][outNum] = sigmoid(el->lout[dataNum][outNum]);
		}
	}

	//******************************順方向の計算の終了******************************

	//デバッグ用
	/*for (dataNum = 0; dataNum < data; dataNum++) {
		for (layerNum = 0; layerNum < layer; layerNum++) {
			for (elementNum = 0; elementNum < element; elementNum++) {
				printf("%f ",el->mid[dataNum][layerNum][elementNum] );
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");

	for (dataNum = 0; dataNum < data; dataNum++) {
		for (outNum = 0; outNum < out; outNum++) {
			printf("%f  ", el->out[dataNum][outNum]);
		}
	}

	printf("\n");*/

	//	for (dataNum = 0; dataNum < data; dataNum++) {
	//		//for (outNum = 0; outNum < out; outNum++) {
	//			printf("%f  ", el->out[dataNum][0]);
	//		//}
	//	}
	//	printf("\n");]
}