#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Header.h"
#pragma warning(disable: 4996)

//NN(�w���A�f�q�����w��)
//�w��2�A��w�̑f�q��4
//���͐�3



int main(void) {

	//���͎������C�o�͎������C�f�[�^���C���w�K�f�[�^��(�e�X�g�f�[�^)
	//exor
	int in = 3, out = 1, data = 6, ldata = 2;
	//�w���A�f�q��
	int layer = 2, element = 4;

	int i, j, k, count = 0;		//��Ɨp

	double E;							//���덷

	int learn = 1;						//�w�K���@(0�E�E�E����,1�E�E�E�ꊇ)

	FILE* fp;//���t�f�[�^�Ɩ��w�K�f�[�^�p


	printf("NN�̑w�����w��E�E�E");
	scanf_s("%d", &layer);
	printf("NN��1�w������̑f�q�����w��E�E�E");
	scanf_s("%d", &element);
	do {
		printf("�w�K���@�̎w��(0�E�E�E����,�@�@1�E�E�E�ꊇ)");
		scanf_s("%d", &learn);
	} while (learn != 0 && learn != 1);




	printf("NN\n");
	//***********�e�l�̂��߂̗̈�m�ۂƏ�����***************
	eldata el;
	//���t�f�[�^
	el.Tin = (double**)malloc(sizeof(double*) * data);
	el.Tin[0] = (double*)malloc(sizeof(double) * data * in);				//2����
	for (i = 1; i < data; i++) {
		el.Tin[i] = el.Tin[0] + i * in;
	}
	//���t�o�̓f�[�^
	el.Tout = (double**)malloc(sizeof(double) * data);
	el.Tout[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		el.Tout[i] = el.Tout[0] + i * out;
	}
	//exor
	//if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
	//EMG
	//if ((fp = fopen("EMGdata.txt", "r")) == NULL) {
	if ((fp = fopen("EXOR.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}

	printf("���t�f�[�^�ǂݍ���\n");
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf %lf", &el.Tin[i][0], &el.Tin[i][1], &el.Tin[i][2], &el.Tout[i])) != EOF) {
		printf("%f %f %f %f\n", el.Tin[i][0], el.Tin[i][1], el.Tin[i][2], el.Tout[i]);
		i++;
	}*/
	for (i = 0; i < data; i++) {
		for (j = 0; j < in; j++) {
			//���t���̓f�[�^�ǂݍ���
			if (fscanf_s(fp, "%lf", &el.Tin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
			}
		}
		for (k = 0; k < out; k++) {
			//���t�o�̓f�[�^�ǂݍ���
			if (fscanf_s(fp, "%lf", &el.Tout[i][k]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
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

	//�o�͑w�񎟌��ɑΉ�
	el.out = (double**)malloc(sizeof(double) * data);
	el.out[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		el.out[i] = el.out[0] + i * out;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < out; j++) {
			el.out[i][j] = 0;
		}
	}

	//���ԑw
	el.mid = (double***)malloc(sizeof(double**) * data);
	el.mid[0] = (double**)malloc(sizeof(double*) * data * layer);
	el.mid[0][0] = (double*)malloc(sizeof(double) * data * layer * element);	//3����
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
	error.in[0] = (double*)malloc(sizeof(double) * data * in);				//2����
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
	error.mid[0][0] = (double*)malloc(sizeof(double) * data * layer * (element + 1));	//3����
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

	//�o�͑w�̎������ɑΉ�
	error.out = (double**)malloc(sizeof(double) * data);
	error.out[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < data; i++) {
		error.out[i] = error.out[0] + i * out;
	}
	for (i = 0; i < data; i++) {
		for (j = 0; j < out; j++) {
			error.out[i][j] = 0;
		}
	}


	//�d�݂̗̈�m�ۂƏ�����
	wdata w;
	w.w1 = (double**)malloc(sizeof(double*) * in + 1);
	w.w1[0] = (double*)malloc(sizeof(double) * (in + 1) * element);						//2����
	for (i = 1; i < (in + 1); i++) {
		w.w1[i] = w.w1[0] + i * element;
	}

	w.w2 = (double***)malloc(sizeof(double**) * (layer - 1));
	w.w2[0] = (double**)malloc(sizeof(double*) * (layer - 1) * (element + 1));
	w.w2[0][0] = (double*)malloc(sizeof(double) * (layer - 1) * (element + 1) * element);	//3����
	for (i = 0; i < (layer - 1); i++) {
		w.w2[i] = w.w2[0] + i * (element + 1);
		for (j = 0; j < (element + 1); j++) {
			w.w2[i][j] = w.w2[0][0] + i * (element + 1) * element + j * element;
		}
	}

	//�o�͑w�̎������̕ω��ɑΉ�
	w.w3 = (double**)malloc(sizeof(double*) * (element + 1));
	w.w3[0] = (double*)malloc(sizeof(double) * (element + 1) * out);						//2����
	for (i = 1; i < (element + 1); i++) {
		w.w3[i] = w.w3[0] + i * out;
	}

	//�����̏�����
	srand((unsigned int)time(NULL));

	printf("�d�݂̏����l\n");
	printf("w1\n");
	for (i = 0; i < in + 1; i++) {
		for (j = 0; j < element; j++) {
			w.w1[i][j] = rand() / double(RAND_MAX) * 2 - 1;
			//w.w1[i][j] = 0.5;
			printf("%f ", w.w1[i][j]);
		}
		printf("\n");
	}
	//1����2
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
	//2����o��
	//�o�͂̐��̕ω��ɑΉ�
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
		printf("�����w�K\n");
	}
	else {
		printf("�ꊇ�w�K\n");
	}


	//********************�g���f�[�^�̐ݒ肪����*****************************************


	if (learn == 1) {
		//*******************************�ꊇ�w�K********************************************
		//�ȉ������[�v���A�������A�t���������肩�����Ċw�K
		do {
			//�f�[�^�����������ɓ`��������
			//printf("colel\n");
			colel(in, layer, element, out, data, &el, &w);

			//�덷�̌v�Z���덷error�ƕ��ʂ̌덷error.out
			E = colerror(out, &el, data, &error);
			//if (count % 100 == 0) {
			printf("\ncount = %d	E = %lf\n", count, E);
			//}
			//�t����
			spread(in, layer, element, out, data, &el, &error, &w);


			count++;
		} while (count < N && E > setE);
		//********************************************************************************************************************
	}

	//*******************************�����w�K********************************************
	else {
		do {
			//�f�[�^������ǂ�ŏ������C�덷�C�t�`���C�d�ݍX�V���J��Ԃ�
			//�f�[�^�����̃��[�v
			int i, datanum;
			for (i = 0; i < data; i++) {
				//�����_���ȏ��ԂŊw�K������
				datanum = rand() % data;
				//������
				colel_online(in, layer, element, out, datanum, &el, &w);
				/*for (int i = 0; i < out; i++) {
					printf("%f ",el.out[datanum][i]);
				}
				printf("\n");*/

				//�덷�v�Z
				E = colerror_online(out, &el, datanum, &error);
				//if (count % 100 == 0) {
					//printf("\ncount = %d	E = %lf\n", count, E);
				//}
				//�t�`���Əd�ݍX�V
				spread_online(in, layer, element, out, datanum, &el, &error, &w);
			}
			E = colerror(out, &el, data, &error);
			printf("\ncount = %d	E = %lf\n", count, E);
			count++;
		} while (count < N && E > setE);
	}
	//********************************************************************************************************************

	printf("�w�K�̏I��\n");
	printf("count = %d  error = %f\n", count, E);
	if (count == N) {
		printf("count = %d\n", count);
	}
	else if (E <= setE) {
		printf("�w�K���I���܂���\n");
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


	//�̈�̉��

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


	//�G���^�[�҂�
	do {
		//printf("�G���^�[�L�[�������ĉ������B");
	} while (getchar() != '\n');


	//���w�K�f�[�^����荞��
	//*********************************************************************************
	el.lin = (double**)malloc(sizeof(double*) * ldata);
	el.lin[0] = (double*)malloc(sizeof(double) * data * in);				//2����
	for (i = 1; i < ldata; i++) {
		el.lin[i] = el.lin[0] + i * in;
	}
	el.lout = (double**)malloc(sizeof(double) * ldata);
	el.lout[0] = (double*)malloc(sizeof(double) * data * out);				//2����
	for (i = 1; i < ldata; i++) {
		el.lout[i] = el.lout[0] + i * out;
	}

	//if ((fp = fopen("Traning_Data.txt", "r")) == NULL) {
	//	printf("file open error!!\n");
	//	exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	//}

	printf("���w�K�f�[�^\n");
	//exor
	//if ((fp = fopen("Test_Data.txt", "r")) == NULL) {
	//EMG
	//if ((fp = fopen("EMGdatatest.txt", "r")) == NULL) {
	if ((fp = fopen("unlearn.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}
	i = 0;
	/*while ((fscanf_s(fp, "%lf %lf %lf", &el.lin[i][0], &el.lin[i][1], &el.lin[i][2])) != EOF) {
		printf("%f %f %f\n", el.lin[i][0], el.lin[i][1], el.lin[i][2]);
		i++;
	}*/

	for (i = 0; i < ldata; i++) {
		for (j = 0; j < in; j++) {
			//���t���̓f�[�^�ǂݍ���
			if (fscanf_s(fp, "%lf", &el.lin[i][j]) == EOF) {
				printf("EOF!\n");
				fclose(fp);
				break;
			}
		}
	}

	fclose(fp);


	//���w�K�f�[�^�ɑ΂���v�Z
	lcolel(in, layer, element, out, ldata, &el, &w);

	printf("���w�K�f�[�^�ɑ΂���o��\n");
	for (i = 0; i < ldata; i++) {//�f�[�^��
		for (j = 0; j < out; j++) {//�o�͎�����
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

	//�G���^�[�҂�
	do {
		printf("�I�����܂��B�G���^�[�L�[�������ĉ������B");
	} while (getchar() != '\n');

	return 0;
}



//������//�ꊇ
//	int in						���͎�����
//	int layer,					�w��
//	int element,				�f�q��
//	int out,					�o�͎�����
//	int data,					�w�K�f�[�^��
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct wdata* w,			�e�w�̊Ԃ̏d��
void colel(int in, int layer, int element, int out, int data, struct eldata* el, struct wdata* w) {

	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//��Ɨp


	//�e�l�̏�����������
	for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
		for (layerNum = 0; layerNum < layer; layerNum++) {//�w����
			for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����
				el->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
		for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
			el->out[dataNum][outNum] = 0;
		}
	}

	//�f�o�b�O�p
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("�d�݂̌o��\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1����2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2����o��
	////�o�͂̐��̕ω��ɑΉ�
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");

	for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
		
		//******************************���͂��璆�ԑw******************************
		for (elementNum = 0; elementNum < element; elementNum++) {//���ԑw�̑f�q��
			for (inNum = 0; inNum < in + 1; inNum++) {//���͎�����+�o�C�A�X��
				if (inNum == in) {
					el->mid[dataNum][0][elementNum] += w->w1[inNum][elementNum];
				}
				else {
					el->mid[dataNum][0][elementNum] += el->Tin[dataNum][inNum] * w->w1[inNum][elementNum];
				}
			}
			//�V�O���C�h�őf�q�̏o�͒l
			el->mid[dataNum][0][elementNum] = sigmoid(el->mid[dataNum][0][elementNum]);
		}
		//******************************���ԑw******************************
		for (layerNum = 0; layerNum < layer-1; layerNum++) {//���ԑw�w��-1
			for (nextNum = 0; nextNum < element; nextNum++) {//�����̒��ԑw�̑f�q��
				for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
					if (elementNum == element) {
						el->mid[dataNum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
					}
					else {
						el->mid[dataNum][layerNum + 1][nextNum] += el->mid[dataNum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
					}
				}
				//�V�O���C�h�őf�q�̏o�͒l
				el->mid[dataNum][layerNum + 1][nextNum] = sigmoid(el->mid[dataNum][layerNum + 1][nextNum]);
			}
		}
		//******************************���ԑw����o��******************************
		for (outNum = 0; outNum < out; outNum++) {//�o�͑w�̎�����
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
				if (elementNum == element) {
					el->out[dataNum][outNum] += w->w3[elementNum][outNum];
				}
				else {
					el->out[dataNum][outNum] += el->mid[dataNum][layer - 1][elementNum] * w->w3[elementNum][outNum];
				}
			}
			//�V�O���C�h�őf�q�̏o�͒l
			el->out[dataNum][outNum] = sigmoid(el->out[dataNum][outNum]);
		}
	}

	//******************************�������̌v�Z�̏I��******************************

	//�f�o�b�O�p
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


//�V�O���C�h�֐��̒l��Ԃ�
double sigmoid(double s) {
	return 1 / (1 + exp(-s));
}

//�ꊇ�w�K���̌덷
//	int out,					�o�͎�����
//	int data,					�w�K�f�[�^��
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct errordata* error,	�e�f�q�ɓ`�������덷
double colerror(int out,struct eldata *el,int data,struct errordata *error) {
	int dataNum, outNum;//��Ɨp
	double E = 0;

	for (dataNum = 0; dataNum < data;dataNum++) {//�f�[�^����
		for (outNum = 0; outNum < out; outNum++) {//�o�͎�������
			error->out[dataNum][outNum] = el->out[dataNum][outNum] -el->Tout[dataNum][outNum];
			E += pow(error->out[dataNum][outNum], 2);//�����֐�
		}
	}
	return E;
}


//�t�`��-�ꊇ�w�K
//	int in						���͎�����
//	int layer,					�w��
//	int element,				�f�q��
//	int out,					�o�͎�����
//	int data,					�w�K�f�[�^��
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct errordata* error,	�e�f�q�ɓ`�������덷
//	struct wdata* w,			�e�w�̊Ԃ̏d��
void spread(int in, int layer, int element, int out, int data, struct eldata* el, struct errordata* error, struct wdata* w) {
	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//��Ɨp
	double totalerror;
	//����
	for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
		for (layerNum = 0; layerNum < layer; layerNum++) {//�w����
			for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����
				error->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
	}
	//�`���ƍX�V���s���Ă���
	//******************************�o�͂��璆�ԑw******************************
	//�덷�̓`��
	for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����	
		for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
			for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�ւ̓`���͕K�v�Ȃ�))
				error->mid[dataNum][layerNum-1][elementNum] += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * w->w3[elementNum][outNum];
			}
		}
	}

	//�d�݂̍X�V
	layerNum = layer;
	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//�f�q��+�o�C�A�X
			if (elementNum == element) {//�o�C�A�X�̍X�V
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����	
					totalerror += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * 1;
				}
				//�����ōX�V
				w->w3[elementNum][outNum] += -2*ep * totalerror / data;
			}
			else {//�o�C�A�X�ȊO�̍X�V
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����	
					totalerror += error->out[dataNum][outNum] * el->out[dataNum][outNum] * (1 - el->out[dataNum][outNum]) * el->mid[dataNum][layerNum - 1][elementNum];
				}
				//�����ōX�V
				w->w3[elementNum][outNum] += -2*ep * totalerror / data;
			}
		}
	}

	//******************************���ԑw******************************
	for (layerNum = layer - 1; layerNum > 0; layerNum -- ) {//�w����(�X�^�[�g�͍Ō�̒��ԑw�C���f�b�N�X��layerNum-1 w2�̃C���f�b�N�X�΂����-1)
		//�덷�̓`��
		for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
			for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�͊܂܂Ȃ�)
				for (nextNum = 0; nextNum < element; nextNum++) {//�����̑f�q����(�o�C�A�X�͊܂܂Ȃ�)
					error->mid[dataNum][layerNum - 1][nextNum] 
						+= error->mid[dataNum][layerNum][elementNum] 
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * w->w2[layerNum - 1][nextNum][elementNum];
				}
			}
		}

		//�d�݂̍X�V
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�͊܂܂Ȃ�)
			for (nextNum = 0; nextNum < element + 1; nextNum++) {//�����̑f�q����+�o�C�A�X
				if (nextNum == element) {//�o�C�A�X�̍X�V
					totalerror = 0;
					for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
						totalerror += error->mid[dataNum][layerNum][elementNum]
							* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * 1;
					}
					//�����ōX�V
					w->w2[layerNum-1][nextNum][elementNum]+= -ep * totalerror / data;
				}
				else {//�o�C�A�X�ȊO�̍X�V
					totalerror = 0;
					for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
						totalerror += error->mid[dataNum][layerNum][elementNum]
							* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * el->mid[dataNum][layerNum - 1][nextNum];
					}
					//�����ōX�V
					w->w2[layerNum-1][nextNum][elementNum] += -ep * totalerror / data;
				}
			}
		}
	}

	//******************************���ԑw�������******************************
	layerNum = 0;//�ꉞ
	//�덷�̓`���͕K�v�Ȃ�
	//�d�݂̍X�V
	for (elementNum = 0; elementNum < element; elementNum++) {//���ԑw�̑f�q��(�o�C�A�X�Ȃ�)
		for (inNum = 0; inNum < in + 1; inNum++) {//���͎�����+1(�o�C�A�X)
			if (inNum == in) {//�o�C�A�X�̍X�V
				totalerror = 0;
				for (dataNum = 0; dataNum < data;dataNum++) {//�f�[�^����
					totalerror += error->mid[dataNum][layerNum][elementNum]
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * 1;
				}
				//�����ōX�V
				w->w1[inNum][elementNum] += -ep * totalerror / data;
			}
			else {//�o�C�A�X�ȊO�̍X�V
				totalerror = 0;
				for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
					totalerror += error->mid[dataNum][layerNum][elementNum]
						* el->mid[dataNum][layerNum][elementNum] * (1 - el->mid[dataNum][layerNum][elementNum]) * el->Tin[dataNum][inNum];
				}
				//�����ōX�V
				w->w1[inNum][elementNum] += -ep * totalerror / data;
			}
		}
	}

	//�d�ݍX�V�̊���
}




//�����w�K�̂��߁@1�f�[�^����
//	int in						���͎�����
//	int layer,					�w��
//	int element,				�f�q��
//	int out,					�o�͎�����
//	int data,					�g�p����w�K�f�[�^�̃C���f�b�N�X
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct wdata* w,			�e�w�̊Ԃ̏d��
void colel_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct wdata* w) {
	//int dataNum, 
	int inNum, layerNum, elementNum, nextNum, outNum;	//��Ɨp


	//�e�l�̏�����������
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
	for (layerNum = 0; layerNum < layer; layerNum++) {//�w����
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����
			el->mid[datanum][layerNum][elementNum] = 0;
		}
	}
	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
		el->out[datanum][outNum] = 0;
	}
	//}

	//�f�o�b�O�p
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("�d�݂̌o��\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1����2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2����o��
	////�o�͂̐��̕ω��ɑΉ�
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");


	//�����ł̓f�[�^�����̃��[�v�����Odatanum������
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����

	//******************************���͂��璆�ԑw******************************
	for (elementNum = 0; elementNum < element; elementNum++) {//���ԑw�̑f�q��
		for (inNum = 0; inNum < in + 1; inNum++) {//���͎�����+�o�C�A�X��
			if (inNum == in) {
				el->mid[datanum][0][elementNum] += w->w1[inNum][elementNum];
			}
			else {
				el->mid[datanum][0][elementNum] += el->Tin[datanum][inNum] * w->w1[inNum][elementNum];
			}
		}
		//�V�O���C�h�őf�q�̏o�͒l
		el->mid[datanum][0][elementNum] = sigmoid(el->mid[datanum][0][elementNum]);
	}
	//******************************���ԑw******************************
	for (layerNum = 0; layerNum < layer - 1; layerNum++) {//���ԑw�w��-1
		for (nextNum = 0; nextNum < element; nextNum++) {//�����̒��ԑw�̑f�q��
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
				if (elementNum == element) {
					el->mid[datanum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
				}
				else {
					el->mid[datanum][layerNum + 1][nextNum] += el->mid[datanum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
				}
			}
			//�V�O���C�h�őf�q�̏o�͒l
			el->mid[datanum][layerNum + 1][nextNum] = sigmoid(el->mid[datanum][layerNum + 1][nextNum]);
		}
	}
	//******************************���ԑw����o��******************************
	for (outNum = 0; outNum < out; outNum++) {//�o�͑w�̎�����
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
			if (elementNum == element) {
				el->out[datanum][outNum] += w->w3[elementNum][outNum];
			}
			else {
				el->out[datanum][outNum] += el->mid[datanum][layer - 1][elementNum] * w->w3[elementNum][outNum];
			}
		}
		//�V�O���C�h�őf�q�̏o�͒l
		el->out[datanum][outNum] = sigmoid(el->out[datanum][outNum]);
	}
	//}

	//******************************�������̌v�Z�̏I��******************************

	//�f�o�b�O�p
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

//�����w�K�@�`���Əd�ݍX�V
//�����w�K�̍ۂ̌덷(�e�f�[�^����)
//	int in						���͎�����
//	int layer,					�w��
//	int element,				�f�q��
//	int out,					�o�͎�����
//	int data,					�g�p����w�K�f�[�^�̃C���f�b�N�X
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct errordata* error,	�e�f�q�ɓ`�������덷
//	struct wdata* w,			�e�w�̊Ԃ̏d��
void spread_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct errordata* error, struct wdata* w) {
	//int dataNum,
	int	inNum, layerNum, elementNum, nextNum, outNum;	//��Ɨp
	double totalerror;
	//����
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
	for (layerNum = 0; layerNum < layer; layerNum++) {//�w����
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����
			error->mid[datanum][layerNum][elementNum] = 0;
		}
	}
	//}
	//�����ł̓f�[�^�����̃��[�v�����Odatanum������
	//�`���ƍX�V���s���Ă���
	//******************************�o�͂��璆�ԑw******************************
	//�덷�̓`��
	//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����	
	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�ւ̓`���͕K�v�Ȃ�))
			error->mid[datanum][layerNum - 1][elementNum] += error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * w->w3[elementNum][outNum];
		}
	}
	//}

	//�d�݂̍X�V
	layerNum = layer;
	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
		for (elementNum = 0; elementNum < element + 1; elementNum++) {//�f�q��+�o�C�A�X
			if (elementNum == element) {//�o�C�A�X�̍X�V
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����	
				totalerror = error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * 1;
				//}
				//�����ōX�V
				w->w3[elementNum][outNum] += -2 * ep * totalerror;
			}
			else {//�o�C�A�X�ȊO�̍X�V
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����	
				totalerror = error->out[datanum][outNum] * el->out[datanum][outNum] * (1 - el->out[datanum][outNum]) * el->mid[datanum][layerNum - 1][elementNum];
				//}
				//�����ōX�V
				w->w3[elementNum][outNum] += -2 * ep * totalerror;
			}
		}
	}

	//******************************���ԑw******************************
	for (layerNum = layer - 1; layerNum > 0; layerNum--) {//�w����(�X�^�[�g�͍Ō�̒��ԑw�C���f�b�N�X��layerNum-1 w2�̃C���f�b�N�X�΂����-1)
		//�덷�̓`��
		//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�͊܂܂Ȃ�)
			for (nextNum = 0; nextNum < element; nextNum++) {//�����̑f�q����(�o�C�A�X�͊܂܂Ȃ�)
				error->mid[datanum][layerNum - 1][nextNum]
					+= error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * w->w2[layerNum - 1][nextNum][elementNum];
			}
		}
		//}

		//�d�݂̍X�V
		for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����(�o�C�A�X�͊܂܂Ȃ�)
			for (nextNum = 0; nextNum < element + 1; nextNum++) {//�����̑f�q����+�o�C�A�X
				if (nextNum == element) {//�o�C�A�X�̍X�V
					//totalerror = 0;
					//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
					totalerror = error->mid[datanum][layerNum][elementNum]
						* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * 1;
					//}
					//�����ōX�V
					w->w2[layerNum - 1][nextNum][elementNum] += -ep * totalerror;
				}
				else {//�o�C�A�X�ȊO�̍X�V
					//totalerror = 0;
					//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
					totalerror = error->mid[datanum][layerNum][elementNum]
						* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * el->mid[datanum][layerNum - 1][nextNum];
					//}
					//�����ōX�V
					w->w2[layerNum - 1][nextNum][elementNum] += -ep * totalerror;
				}
			}
		}
	}

	//******************************���ԑw�������******************************
	layerNum = 0;//�ꉞ
	//�덷�̓`���͕K�v�Ȃ�
	//�d�݂̍X�V
	for (elementNum = 0; elementNum < element; elementNum++) {//���ԑw�̑f�q��(�o�C�A�X�Ȃ�)
		for (inNum = 0; inNum < in + 1; inNum++) {//���͎�����+1(�o�C�A�X)
			if (inNum == in) {//�o�C�A�X�̍X�V
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
				totalerror = error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * 1;
				//}
				//�����ōX�V
				w->w1[inNum][elementNum] += -ep * totalerror;
			}
			else {//�o�C�A�X�ȊO�̍X�V
				//totalerror = 0;
				//for (dataNum = 0; dataNum < datanum; dataNum++) {//�f�[�^����
				totalerror = error->mid[datanum][layerNum][elementNum]
					* el->mid[datanum][layerNum][elementNum] * (1 - el->mid[datanum][layerNum][elementNum]) * el->Tin[datanum][inNum];
				//}
				//�����ōX�V
				w->w1[inNum][elementNum] += -ep * totalerror;
			}
		}
	}

	//�d�ݍX�V�̊���
}

//���w�K�f�[�^�ɑ΂��邵�o�͂����߂�
//�ꊇ�̏������̃f�[�^���Ȃǂ�ς���΂悢
//	int in						���͎�����
//	int layer,					�w��
//	int element,				�f�q��
//	int out,					�o�͎�����
//	int data,					���w�K�w�K�f�[�^��
//	struct eldata* el,			�l�b�g���[�N�̊e�f�q�̏o�͂�ێ�����\���̂ւ̃|�C���^
//	struct wdata* w,			�e�w�̊Ԃ̏d��
void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w) {
	int dataNum, inNum, layerNum, elementNum, nextNum, outNum;	//��Ɨp


	//�e�l�̏�����������
	for (dataNum = 0; dataNum < ldata; dataNum++) {//�f�[�^����
		for (layerNum = 0; layerNum < layer; layerNum++) {//�w����
			for (elementNum = 0; elementNum < element; elementNum++) {//�f�q����
				el->mid[dataNum][layerNum][elementNum] = 0;
			}
		}
		for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
			el->lout[dataNum][outNum] = 0;
		}
	}

	//�f�o�b�O�p
	//printf("Tin\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (inNum = 0; inNum < in; inNum++) {
	//		printf("%f ", el->Tin[dataNum][inNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("out\n");
	//for (dataNum = 0; dataNum < data; dataNum++) {//�f�[�^����
	//	for (outNum = 0; outNum < out; outNum++) {//�o�͎�����
	//		printf("%f ", el->Tout[dataNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");
	//printf("�d�݂̌o��\n");
	//printf("w1\n");
	//for (inNum = 0; inNum < in + 1; inNum++) {
	//	for (elementNum = 0; elementNum < element; elementNum++) {
	//		printf("%f ", w->w1[inNum][elementNum]);
	//	}
	//	printf("\n");
	//}
	////1����2
	//printf("w2\n");
	//for (layerNum = 0; layerNum< layer - 1; layerNum++) {
	//	for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//		for (nextNum = 0; nextNum < element; nextNum++) {
	//			printf("%f ", w->w2[layerNum][elementNum][nextNum]);
	//		}
	//		printf("\n");
	//	}
	//}
	////2����o��
	////�o�͂̐��̕ω��ɑΉ�
	//printf("w3\n");
	//for (elementNum = 0; elementNum < element + 1; elementNum++) {
	//	for (outNum = 0; outNum < out; outNum++) {
	//		printf("%f ", w->w3[elementNum][outNum]);
	//	}
	//	printf("\n");
	//}
	//printf("\n\n");



	for (dataNum = 0; dataNum < ldata; dataNum++) {//�f�[�^����

		//******************************���͂��璆�ԑw******************************
		for (elementNum = 0; elementNum < element; elementNum++) {//���ԑw�̑f�q��
			for (inNum = 0; inNum < in + 1; inNum++) {//���͎�����+�o�C�A�X��
				if (inNum == in) {
					el->mid[dataNum][0][elementNum] += w->w1[inNum][elementNum];
				}
				else {
					el->mid[dataNum][0][elementNum] += el->lin[dataNum][inNum] * w->w1[inNum][elementNum];
				}
			}
			//�V�O���C�h�őf�q�̏o�͒l
			el->mid[dataNum][0][elementNum] = sigmoid(el->mid[dataNum][0][elementNum]);
		}
		//******************************���ԑw******************************
		for (layerNum = 0; layerNum < layer - 1; layerNum++) {//���ԑw�w��-1
			for (nextNum = 0; nextNum < element; nextNum++) {//�����̒��ԑw�̑f�q��
				for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
					if (elementNum == element) {
						el->mid[dataNum][layerNum + 1][nextNum] += w->w2[layerNum][elementNum][nextNum];
					}
					else {
						el->mid[dataNum][layerNum + 1][nextNum] += el->mid[dataNum][layerNum][elementNum] * w->w2[layerNum][elementNum][nextNum];
					}
				}
				//�V�O���C�h�őf�q�̏o�͒l
				el->mid[dataNum][layerNum + 1][nextNum] = sigmoid(el->mid[dataNum][layerNum + 1][nextNum]);
			}
		}
		//******************************���ԑw����o��******************************
		for (outNum = 0; outNum < out; outNum++) {//�o�͑w�̎�����
			for (elementNum = 0; elementNum < element + 1; elementNum++) {//���ԑw�̑f�q��+�o�C�A�X��
				if (elementNum == element) {
					el->lout[dataNum][outNum] += w->w3[elementNum][outNum];
				}
				else {
					el->lout[dataNum][outNum] += el->mid[dataNum][layer - 1][elementNum] * w->w3[elementNum][outNum];
				}
			}
			//�V�O���C�h�őf�q�̏o�͒l
			el->lout[dataNum][outNum] = sigmoid(el->lout[dataNum][outNum]);
		}
	}

	//******************************�������̌v�Z�̏I��******************************

	//�f�o�b�O�p
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