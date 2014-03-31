#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "complex.h"

t_complex* complexSum(t_complex* res, t_complex* add1, t_complex* add2){
	res->real=add1->real+add2->real;
	res->img=add1->img+add2->img;
	return res;
}

t_complex* complexSub(t_complex* res, t_complex* sott1, t_complex* sott2){
	res->real=sott1->real-sott2->real;
	res->img=sott1->img-sott2->img;
	return res;
}

t_complex* complexMul(t_complex* res, t_complex* mul1, t_complex* mul2){
	res->real=mul1->real*mul2->real-mul1->img*mul2->img;
	res->img=mul1->real*mul2->img+mul1->img*mul2->real;
	return res;
}
// t_complex* complexdoubleMul(t_complex* res, t_complex* mul1, double mul2){
// 	res->real=mul1->real*mul2;
// 	res->img=mul1->img*mul2;
// 	return res;
// }

t_complex* complexDiv(t_complex* res, t_complex* div1, t_complex* div2){
	res->real=(div1->real*div2->real-div1->img*(-1)*div2->img)/(div2->real*div2->real+div2->img*div2->img);
	res->img=(div1->real*(-1)*div2->img+div1->img*div2->real)/(div2->real*div2->real+div2->img*div2->img);
	return res;
}


double complexMag(t_complex* comp){
	return sqrt(pow(comp->real,2)+pow(comp->img,2));
}

t_complex* iPow(t_complex* res, int exp){
	switch (exp%4){
		case 0:
			res->real=1;
			res->img=0;
			break;
		case 1:
			res->real=0;
			res->img=1;
			break;
		case 2:
			res->real=-1;
			res->img=0;
			break;
		case 3:
			res->real=0;
			res->img=-1;
			break;
		}
		return res;
}

t_complex* numballoc(double real, double img){
	t_complex* numb = malloc(sizeof(t_complex));
	if(numb!=NULL){
		numb->real=real;
		numb->img=img;
		return numb;
	}
	return NULL;
}

//TEST CASE
// int main(){
// 	t_complex* testnumb1 = numballoc(3.,-2.);
// 	t_complex* testnumb2 = numballoc(2.,1.);
// 	t_complex* res = numballoc(0.,0.);
// 	complexSum(res,testnumb1,testnumb2);
// 	printf("la somma vale: Real-> %f Imag-> %f\n",res->real,res->img);
// 	complexSub(res,testnumb1,testnumb2);
// 	printf("la differenza vale: Real-> %f Imag-> %f\n",res->real,res->img);
// 	complexMul(res,testnumb1,testnumb2);
// 	printf("il prodotto vale: Real-> %f Imag-> %f\n",res->real,res->img);
// 	complexDiv(res,testnumb1,testnumb2);
// 	printf("la divisione vale: Real-> %f Imag-> %f\n",res->real,res->img);

// 	return 0;
// }