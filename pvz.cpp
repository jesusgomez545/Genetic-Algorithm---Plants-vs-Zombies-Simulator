#include <sys/time.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "mtrand.h"
#include "constants.h"
using namespace std;

int randPlant(int g){	
	int v = g % MAX_ELEMENT_TYPE;
	return v < 0 ? -1 * v : v;
}


int main(){
	struct timeval tim;  
	gettimeofday(&tim, NULL);
	MTRand_int32 gen(1000000 * tim.tv_sec + tim.tv_usec);

	for (int i = 0; i < 10; ++i)
	{
		cout << randPlant(gen()) <<endl;
	}
	
	return EXIT_SUCCESS;
}