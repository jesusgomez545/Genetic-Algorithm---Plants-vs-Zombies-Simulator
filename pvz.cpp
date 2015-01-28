#include <sys/time.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream> 
#include <algorithm>
#include "mtrand.h"
#include "constants.h"

using namespace std;

MTRand_int32 mtrand;

void loadEntry(
	int &w, 
	int &h, 
	int &z, 
	vector<int> &zombie_vector, 
	string filename){

	fstream f;
	f.open(filename.c_str(), std::fstream::in);
	int zombie;
	if(f){
		f >> w >> h >> z;

		if (h < MAX_COLUMNAS || h == MAX_COLUMNAS)
		{
			f.close();
			cout<< "\tNo existe suficiente espacio"<<endl<<"\tpara colocar las plantas"<<endl<<"\tdentro de una matriz "<<w<<" x "<<h<<endl;
			cout<< "\tDebe existir espacio para una"<<endl<<"\tmatriz "<<w<<" x "<<MAX_COLUMNAS<<endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < z && !f.eof(); ++i)
		{
			f>>zombie;
			zombie_vector.push_back(zombie);
		}
		f.close();
	}else{
		cout<<"Problemas abriendo archivo de entrada \""<< filename <<"\""<<endl;
		exit(EXIT_FAILURE);
	}

}

void init(){
	struct timeval tim;  
	gettimeofday(&tim, NULL);
	mtrand.seed(1000000 * tim.tv_sec + tim.tv_usec);
}

void printMatrix(string x, int r, int c){	
	for(int i=0; i<r; i++){
		cout << "[";
		for(int j=0; j<c; j++){
			cout<<" "<<x[i*c+j];
		}
		cout<<" ]"<<endl;
	}
}

template <class T>
void printVector(std::vector<T> zv)
{
	for(int i=0;i<zv.size();++i)
		cout<<zv[i]<<" ";
	cout<<endl;

}

char toSingleChar(int g){
	return (char)(((int)'0')+g);
}

char randPlant(){	
	int v = mtrand() % MAX_ELEMENT_TYPE;
	return toSingleChar(v < 0 ? -1 * v : v);
}

vector<string> generarPoblacion(int h){
	int ml = h * MAX_COLUMNAS;
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
	int w, h, z, g = 0;
	vector<int> zv = vector<int>();
	loadEntry(w, h, z, zv, "input.in");
	vector<string> init_pob;

	while(g++ < MAX_GENERACIONES){
		init_pob = generarPoblacion(w);
		printMatrix(init_pob[0], w, MAX_COLUMNAS);
	}

	//printVector<int>(zv);
	//printMatrix(init_pob[0],4,4);
	//PLANTA p = VACIO;
	//cout << p <<endl;
	
	return EXIT_SUCCESS;

}