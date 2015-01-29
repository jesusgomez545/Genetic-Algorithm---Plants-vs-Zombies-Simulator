#include <sys/time.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm> 
#include "mtrand.h"
#include "constants.h"

using namespace std;

MTRand_int32 mtrand;

void init(){
	struct timeval tim;  
	gettimeofday(&tim, NULL);
	mtrand.seed(1000000 * tim.tv_sec + tim.tv_usec);
}

void printMatrix(string x, int r, int c){
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			cout<<" "<<x[i*c+j];
		}
		cout<<endl;
	}
}

char toSingleChar(int g){
	return (char)(((int)'0')+g);
}

char randPlant(){	
	int v = mtrand() % MAX_ELEMENT_TYPE;
	return toSingleChar(v < 0 ? -1 * v : v);
}

std::vector<string> generarPoblacion(int h){
	int ml = h * MAX_FILAS;
	vector<string> v = vector<string>();
	char s[ml];
	
	for(int i = 0; i < INDIVIDUOS_POR_POBLACION; ++i ){
		s[ml] = '\0';
		generate_n (s, ml, randPlant);
		v.push_back(s);
	}
	return v;
}

int main(){	
	
	init();

	vector<string> init_pob = generarPoblacion(2);

	for (vector<string>::iterator it = init_pob.begin(); it != init_pob.end(); ++it)
	    cout << '\t' << *it;
	cout << '\n';

	printMatrix(init_pob[0],4,2);
	cout<< init_pob.size() <<endl;

	return EXIT_SUCCESS;
}