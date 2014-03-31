#ifndef COMPLEX_H
#define COMPLEX_H
 
typedef struct complex {
	double real ;
	double img;
} t_complex;
 


t_complex* complexSum(t_complex* res, t_complex* add1, t_complex* add2);

t_complex* complexSub(t_complex* res, t_complex* sott1, t_complex* sott2);

t_complex* complexMul(t_complex* res, t_complex* mul1, t_complex* mul2);

t_complex* complexDiv(t_complex* res, t_complex* div1, t_complex* div2);

double complexMag(t_complex* comp);

//t_complex* complexdoubleMul(t_complex* res, t_complex* mul1, double mul2);

t_complex* iPow(t_complex* res, int exp);

t_complex* numballoc(double real, double img);
#endif