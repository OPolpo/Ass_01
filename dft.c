#include "dft.h"
#define MAX_THREAD 8

struct arg_struct {
	t_complex** out;
	t_complex** matrix;
	int N;
	t_complex** fourier_transformation_matrix;
    int start;
    int stop;
};

//***************************************************
//                GESTIONE MEMORIA
//***************************************************
void freeMatComp(t_complex** to_free, int N){
	int i;
	for(i=0; i<N; i++)
		free(to_free[i]);
}

void freeMat(double** to_free, int N){
	int i;
	for(i=0; i<N; i++)
		free(to_free[i]);
}

t_complex** allocMatComp(int N){
	int i;
	t_complex** newmat = (t_complex**)malloc(sizeof(t_complex*)*N);
	for(i=0; i<N; i++)
		newmat[i]=(t_complex*)malloc(sizeof(t_complex)*N);
	return newmat;
}

double** allocMat(int N){
	int i;
	double** newmat = (double**)malloc(sizeof(double*)*N);
	for(i=0; i<N; i++)
		newmat[i]=(double*)malloc(sizeof(double)*N);
	return newmat;
}

//***************************************************
//          VISUALIZZAZIONE DELLA MATRICE
//***************************************************

void dft_visual_ordering(double** matrix, int N){
	
	int i=0, j=0;

	double** tmp=allocMat(N);
	
	//first quadrant
	for(i=0; i<(N/2); i++)
		for(j=0; j<(N/2); j++)
			tmp[i + (N/2)][j + (N/2)] = matrix[i][j];

	//third quadrant
	for(i=(N/2); i<N; i++)
		for(j=0; j<(N/2); j++)
			tmp[i-(N/2)][j + (N/2)] = matrix[i][j];

	//second quadrant
	for(i=0; i<(N/2); i++)
		for(j=(N/2); j<N; j++)
			tmp[i+(N/2)][j - (N/2)] = matrix[i][j];

	for(i=(N/2); i<N; i++)
		for(j=(N/2); j<N; j++)
			tmp[i-(N/2)][j-(N/2)]=matrix[i][j];

	//ricopio temp nella matrice originale
	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			matrix[i][j] = tmp[i][j];//Error In PDF Assignment!!!!
	free(tmp);

}

double matrix_max_abs(double** matrix, int N){
	int i,j;
	double max=abs(matrix[0][0]);
	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			if(abs(matrix[i][j])>max)
				max=abs(matrix[i][j]);
	return max;
}

void scale(double** matrix, int N){
	double C;
	double MAX=matrix_max_abs(matrix, N);
	C=255/log(MAX);
	int i,j;
	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			matrix[i][j]=C*log(1+matrix[i][j]);
}

double** matrixMag(double** out, t_complex** matrix,int N){
	int i,j;
	for(j=0; j<N; j++)
		for(i=0; i<N; i++)
			out[i][j]=complexMag(&(matrix[i][j]));
	return out;
}

double** display_DFT(double** out, t_complex** input_matrix, int N){
	matrixMag(out, input_matrix, N);
	dft_visual_ordering(out, N);
	scale(out, N);
	return out;
}





//***************************************************
//       CALCOLO EFFETTIVO DELLA TRASFORMATA
//***************************************************
t_complex** dftmtx(t_complex** matrix, int N){
	int i,j;
	t_complex* temp = numballoc(0,0);
	double teta;
	for(i=0; i<N; i++){
		for(j=i; j<N; j++){
			teta=(-2*M_PI/N)*i*j;
			temp->real=cos(teta);
			temp->img=sin(teta);
			matrix[i][j].real=temp->real;
			matrix[i][j].img=temp->img;
			matrix[j][i].real=temp->real;
			matrix[j][i].img=temp->img;
		}
	}
	free(temp);
	return matrix;
}	

t_complex* mulMatrixVect(t_complex* res, int N, t_complex** matrix, t_complex* vect){
	int i,im;
	t_complex* temp = numballoc(0,0);
	for(im=0; im<N; im++)//righe matrice
		for(i=0; i<N; i++){//elementi vettore e singoli elementi di ogni riga della matrice (idice colonna)
			complexMul(temp,&(matrix[im][i]),&(vect[i]));
			res[im].real+=temp->real;
			res[im].img+=temp->img;
		}
	free(temp);
	return res;
}

void *singlethread(void *arguments){
	int i;
	struct arg_struct *args = (struct arg_struct *)arguments;

	for(i=args->start; i<args->stop; i++)//righe matrice da trasformare (elementi presi uno per volta sono i vettori)
		mulMatrixVect((args->out)[i], args->N, args->fourier_transformation_matrix, (args->matrix)[i]);
	pthread_exit(NULL);
}

t_complex** oneD_DFT_row(t_complex** out, t_complex** matrix, int N, t_complex** fourier_transformation_matrix){
	int i;


	if (N > 40){//per valori bassi non credo abbia troppo senso buttarsi sui thread

		pthread_t threadVect[MAX_THREAD];
		struct arg_struct* args_vect=(struct arg_struct*)malloc(sizeof(struct arg_struct)*MAX_THREAD);
		int limit=floor(N/MAX_THREAD);
		int t=0;
		for(t=0;t<MAX_THREAD;t++){
    		args_vect[t].out = out;
    		args_vect[t].matrix = matrix;
    		args_vect[t].N = N;
    		args_vect[t].fourier_transformation_matrix = fourier_transformation_matrix;
    		args_vect[t].start = t*limit;
    		if(t==MAX_THREAD-1)
    			args_vect[t].stop = N;
    		else
    			args_vect[t].stop = (t+1)*limit;

			if (pthread_create(&(threadVect[t]), NULL, &singlethread, (void *)&(args_vect[t]))){
    			printf("Error creating Thread!\n");
    		}
    	}
    	for(t=0;t<8;t++){
			if(pthread_join(threadVect[t], NULL))
				fprintf(stderr, "Error joining thread\n");
    	}

		return out;
	}
	else{
		for(i=0; i<N; i++)
			mulMatrixVect(out[i], N, fourier_transformation_matrix, matrix[i]);
		return out;
	}
}

t_complex** transposeMatrix(t_complex** out, t_complex** matrix, int N){
	int i,j;
	for(j=0; j<N; j++)
		for(i=0; i<N; i++){
			out[i][j].real=matrix[j][i].real;
			out[i][j].img=matrix[j][i].img;
			out[j][i].real=matrix[i][j].real;
			out[j][i].img=matrix[i][j].img;
		}
	return out;
}

t_complex** oneD_DFT_col(t_complex** out, t_complex** matrix, int N, t_complex** fourier_transformation_matrix){
	//traslo la matrice in modo da operare sulle colonne anzichè le righe
	//trasformo con oneD_DFT_row
	//traslo nouvamente il risultato per avere nuovamente le righe traslate sulle colonne
	t_complex** first_trasled = allocMatComp(N);
	t_complex** intermediate = allocMatComp(N);
	transposeMatrix(first_trasled, matrix, N);
	oneD_DFT_row(intermediate,first_trasled,N,fourier_transformation_matrix);
	transposeMatrix(out, intermediate, N);
	freeMatComp(first_trasled,N);
	freeMatComp(intermediate,N);

	return out;
}

t_complex** twoD_DFT(t_complex** out, double** input_matrix, int N){
	//trasformo la matrice da double a complessa
	t_complex** input_matrix_c=allocMatComp(N);
	int i,j;
	for(i=0; i<N; i++)
		for(j=0; j<N; j++){
			input_matrix_c[i][j].real=input_matrix[i][j];
			input_matrix_c[i][j].img=0;
		}
	//creo la matrice di trasformazione di fourie
	t_complex** fourier_transformation_matrix = allocMatComp(N);
	dftmtx(fourier_transformation_matrix,N);

	//trasformo per colonne
	t_complex** intermediate_out = allocMatComp(N);
	oneD_DFT_col(intermediate_out,input_matrix_c,N,fourier_transformation_matrix);
	freeMatComp(input_matrix_c,N);

	//trasformo per righe
	oneD_DFT_row(out,intermediate_out,N,fourier_transformation_matrix);
	freeMatComp(intermediate_out, N);
	freeMatComp(fourier_transformation_matrix, N);



	return out;
}


