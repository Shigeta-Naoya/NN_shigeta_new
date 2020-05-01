#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>



int N = 50000;//�w�K��
double setE = 0.001;//�ڕW�덷
double ep = 0.7;//�w�K��

//�e�f�q�̒l�����\����
struct eldata {

	//���t�f�[�^
	double **Tin;
	double **Tout;

	double ***mid;
	double **out;
	
	//���w�K�f�[�^
	double **lin;
	double **lout;
};

//�덷�`�����Ɋe�f�q�ɓ��͂����l
struct errordata {
	double **in;
	double ***mid;
	double **out;
};

//�e�f�q�Ԃ̏d��
struct wdata {
	double **w1;
	double ***w2;
	double **w3;
};
eldata el;
errordata error;
wdata w;


//������
//�����͓��͂̐�in�A���ԑw�̑w���Ƒf�q��,�o�͐�,�f�[�^��
void colel(int in, int layer, int element, int out, int data, struct eldata *el, struct wdata *w);

void colel_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct wdata* w);
//�V�O���C�h�֐�
double sigmoid(double s);

//�덷�����߂�
double colerror(int out,struct eldata *el, int data, struct errordata *error);
double colerror_online(int out, struct eldata* el, int datanum, struct errordata* error);
//�덷�̓`���Əd�݂̍X�V
void spread(int in, int layer, int element, int out, int data, struct eldata *el, struct errordata *error, struct wdata *w);
void spread_online(int in, int layer, int element, int out, int datanum, struct eldata* el, struct errordata* error, struct wdata* w);

void lcolel(int in, int layer, int element, int out, int ldata, struct eldata *el, struct wdata *w);