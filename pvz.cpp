#include <sys/time.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "mtrand.h"
#include "constants.h"

using namespace std;

char toSingleChar(int g){
	return (char)(((int)'0')+g);
}

char randPlant(int g){	
	int v = g % MAX_ELEMENT_TYPE;
	return toSingleChar(v < 0 ? -1 * v : v);
}

std::vector<string> generarPoblacion(
	MTRand_int32 gen,
	int h){
	
	int ml = h * MAX_FILAS;
	vector<string> v = vector<string>();
	for(int i = 0; i < INDIVIDUOS_POR_POBLACION; ++i ){
		cout<<i<<endl;
	}
}

int main(){
	struct timeval tim;  
	gettimeofday(&tim, NULL);
	MTRand_int32 gen(1000000 * tim.tv_sec + tim.tv_usec);
	
	char c;	
	for (int i = 0; i < 10; ++i)
	{
		c = randPlant(gen());		
		cout << c <<endl;
	}

	return EXIT_SUCCESS;
}