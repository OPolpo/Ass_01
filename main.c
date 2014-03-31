#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include "dft.h"
#include "complex.h"


void printMat(double** matrix, int N){
	int i,j;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++)
			printf("%1.f ",matrix[i][j]);
		printf("\n");
	}
}

void printMatComp(t_complex** matrix, int N){
	int i,j;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			printf("(%1.2f ",matrix[i][j].real);
			printf("%1.2f)     ",matrix[i][j].img);
		}
		printf("\n\n\n");
	}
}


double** readFile(double** out, char* file_name, int N){
	FILE *fp;
	int i, j;
	double x;
	fp = fopen(file_name, "r");
	if (fp == NULL ){
		printf("Error\n");
    exit(1);
	}

    for (i=0;i<N;i++)
    	for(j=0;j<N;j++){
        	fscanf(fp,"%lf",&x);
        	out[i][j] = x;
    	}
    return out;
}

double squareEuclideanDistance(double** mat1, double** mat2, int N){
	int i,j;
	double distance=0;
	for (i=0;i<N;i++)
    	for(j=0;j<N;j++)
    		distance+=pow(mat1[i][j]-mat2[i][j],2);
    return distance;
}

//TEST CASE
int main(int argc, char *argv[]){
	if(argc<2){
		printf("\nUsage: %s inputimagepath.txt\nThe image Must be 512x512\ndatabase must be As_01_material_04-03-14-2\n",argv[0]);
		return 0;
	}
	


	//leggo il file
	int N=512;
	double** readed_matrix = allocMat(N);
	readFile(readed_matrix,argv[1],N);

	//creo la matrice trasformata di fourier
	t_complex** transformed_matrix = allocMatComp(N);
	twoD_DFT(transformed_matrix,readed_matrix,N);

	//creo la matrice stampabile
	double** displayable = allocMat(N);
	display_DFT(displayable,transformed_matrix,N);




	double** to_compare = allocMat(N);
	DIR *dir;
	char* path_db = "As_01_material_04-03-14-2/";
	char filepath[255];

	struct dirent *ent;
	dir = opendir(path_db);
	if (!dir)
        return 1;
	while ((ent = readdir(dir))) {
		filepath[0]=0;
		strcat(filepath,path_db);
		strcat(filepath,ent->d_name);
		if(strstr(ent->d_name,".txt")){
			//printf("\n\n%s\n\n",filepath);
			readFile(to_compare,filepath,N);
			printf("\ndistance to %s: %f\n",ent->d_name,squareEuclideanDistance(to_compare,displayable,N));
		}
	}
	closedir(dir);

	freeMat(to_compare,N);
	freeMat(displayable,N);
	freeMat(readed_matrix,N);
	freeMatComp(transformed_matrix,N);


	return 0;
}