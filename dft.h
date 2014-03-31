#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "complex.h"

t_complex** twoD_DFT(t_complex** out, double** input_matrix, int N);

double** display_DFT(double** out, t_complex** input_matrix, int N);

void freeMatComp(t_complex** to_free, int N);

void freeMat(double** to_free, int N);

t_complex** allocMatComp(int N);

double** allocMat(int N);
