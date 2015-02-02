#include <sys/time.h> 
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <vector>
#include <fstream> 
#include <algorithm>
#include <math.h>  
#include <string>
#include <limits>
#include "mtrand.h"
#include "constants.h"

using namespace std;

MTRand_int32 mtrand;
MTRand_closed mtrand_closed;

class Zombie{
	private:
		int life;
		int posx;
		int posy;
	public:
		Zombie();
		Zombie(int l, int x, int y){
			this->life = l;
			this->posx = x;
			this->posy = y;
		}
		int get_life(){
			return this->life;
		}
		int get_x(){
			return this->posx;
		}
		int get_y(){
			return this->posy;
		}
		void set_life(int l){
			this->life = l;
		}
		void set_x(int x){
			this->posx = x;
		}
		void set_y(int y){
			this->posy = y;
		}
};

struct result_set{
	bool sobrevive;
	int turnos;
	int plantas;
	std::string individuo;

	result_set& operator =(const result_set& rhs)
	{
	    sobrevive = rhs.sobrevive;
	    turnos = rhs.turnos;
	    plantas = rhs.plantas;
	    individuo = rhs.individuo;
	    return *this;
	}
};
   
bool compara_aptitud(result_set lhs, result_set rhs){ 
	float ai_l = ((float)lhs.plantas / lhs.turnos) - ( lhs.sobrevive ? 0 : std::numeric_limits<double>::infinity() );
	float ai_r = ((float)rhs.plantas / rhs.turnos) - ( rhs.sobrevive ? 0 : std::numeric_limits<double>::infinity() );
	return  ai_r > ai_l;
}

void loadEntry(
	int &w, 
	int &h, 
	int &z, 
	vector<Zombie> &zombie_vector, 
	string filename
	){

	fstream f;
	f.open(filename.c_str(), std::fstream::in);
	int fila;	

	if(f){
		f >> w >> h >> z;

		if (h <= MAX_COLUMNAS)
		{
			f.close();
			cout<< "\tNo existe suficiente espacio"<<endl<<"\tpara colocar las plantas"<<endl<<"\tdentro de una matriz "<<w<<" x "<<h<<endl;
			cout<< "\tDebe existir espacio para una"<<endl<<"\tmatriz "<<w<<" x "<<MAX_COLUMNAS<<endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < z && !f.eof(); ++i)
		{			
			f>>fila;
			zombie_vector.push_back(Zombie(MAX_VIDA_ZOMBIE, fila-1, h-1));
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
	mtrand_closed.seed(1000000 * tim.tv_sec + tim.tv_usec);

}

void printMatrix(string x, int r, int c){	
	for(int i=0; i<r; i++){
		for(int j=0; j<c; j++){
			cout<<x[i*c+j];
			cout<< (j != c-1 ? " " : "" );
		}
		cout <<endl;
	}
}

char getPos(string x, int i, int j, int l){
		return x[i*l+j];
}

void setPos(string &x, int i, int j, int l, char v){
	x[i*l+j] = v;
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

vector<result_set> generarPoblacion(int h){	
	int ml = h * MAX_COLUMNAS;
	vector<result_set> v;
	result_set r;
	r.sobrevive = false;
	r.plantas = 0;
	r.turnos = 1;	

	for(int i = 0; i < INDIVIDUOS_POR_POBLACION; ++i ){
		string str(ml,0);
		generate_n (str.begin(), ml, randPlant);
		r.individuo = str;
		v.push_back(r);
	}
	return v;
}


set<int> getNrandDistinct(int n, int max){
	
	if ( n >= max ){
		cout << "(!) Se generaran todas las posiciones disponibles [0-"<<max<<"]"<<endl;
		n = max;
	}
	
	int p=0;
	set<int> s = set<int>();	
	while (s.size() < n){		
		p = n>= max ? p : mtrand() % max;
		s.insert(p < 0 ? -1 * p : p);
		++p;
	}
	return s;
}

int impactoFila(string individuo, int r){
	string sbstr = individuo.substr(r*MAX_COLUMNAS,MAX_COLUMNAS);
	int peashotter_impact = count(sbstr.begin(), sbstr.end(), (char)(((int)'0')+PEASHOTTER));
	int repeater_impact = count(sbstr.begin(), sbstr.end(), (char)(((int)'0')+REPEATER));
	return peashotter_impact * IMPACTO_PEASHOTTER + repeater_impact * IMPACTO_REPEATER;
}

result_set aptitud(string individuo, vector<Zombie> zv, int w, int h, int z){
	/*
	*
	*	Return Values :
	*		-1 	If zombies win
	*		0 	If a tie is detected
	*		1 	If plants win
	*		2	Means default/initial state
	*
	**/	
	int turn = 0;
	int mrz = 0;
	int winner = 2;	
	int ps = individuo.size() - count(individuo.begin(),individuo.end(),(char)(((int)'0')+VACIO));	
	PLANTA plant;
	string ind_res = individuo;
	vector<int> reset = vector<int>(w, -1);
	vector<int> vector_impacto;

	while ( winner == 2 )
	{		
		plant = VACIO;
		vector_impacto = reset;		
		for(
			int i = 0;
			i < zv.size(); ++i
		)
			if (i <= turn){
				if (zv[i].get_life())
					if(zv[i].get_y()){

						zv[i].set_y( zv[i].get_y() - MAX_PASO_ZOMBIE < 0 ? 0 : zv[i].get_y() - MAX_PASO_ZOMBIE );

						if ( zv[i].get_y() <= MAX_COLUMNAS-1 ){

							plant = (PLANTA)( getPos( individuo, zv[i].get_x(), zv[i].get_y(), w ) - '0' );							

							if( plant != VACIO ){

								if (plant == POTATO_MINE ){									
									zv[i].set_life(0);									
									z = !z ? 0:z-1;								

								}else if ( plant == PEASHOTTER ){									

									if(zv[i].get_life()- IMPACTO_PEASHOTTER <= 0)
									{
										z = !z ? 0:z-1;	
									}

									zv[i].set_life(zv[i].get_life()- IMPACTO_PEASHOTTER <= 0 ? 0 : zv[i].get_life() - IMPACTO_PEASHOTTER);									
								}else if ( plant == REPEATER ){									

									if(zv[i].get_life()- IMPACTO_REPEATER <= 0)
									{
										z = !z ? 0:z-1;	
									}
									
									zv[i].set_life(zv[i].get_life() - IMPACTO_REPEATER <= 0 ? 0 : zv[i].get_life() - IMPACTO_REPEATER);
								}

								ps = !ps ? 0: ps-1;
								setPos( 
									individuo, 
									zv[i].get_x(), 
									zv[i].get_y(), 
									w, 
									(char)(((int)'0')+VACIO)
								);								
							}
						}
					}else{						
						winner = -1;
					}

				if (zv[i].get_life()){
					
					if(vector_impacto[zv[i].get_x()] < 0)
						vector_impacto[zv[i].get_x()] = impactoFila(individuo, zv[i].get_x());					
					
					if ( zv[i].get_life() > vector_impacto[zv[i].get_x()]){
						zv[i].set_life( zv[i].get_life() - vector_impacto[zv[i].get_x()] );
						vector_impacto[zv[i].get_x()] = 0;
					}else if ( zv[i].get_life() <= vector_impacto[zv[i].get_x()] ){
						zv[i].set_life(0);
						vector_impacto[zv[i].get_x()] = vector_impacto[zv[i].get_x()] - zv[i].get_life();
						z = !z ? 0:z-1;
					}
				}

				if (!ps and !z)
					winner = 0;
				else if(!ps and z)
					winner = -1;
				else if (!z and ps)
					winner = 1;
			}
		++turn;
	}

	result_set r;
	r.sobrevive = (winner == 1 ? true:false); 
	r.turnos = turn+1;
	r.plantas = ps;
	r.individuo = ind_res;
	
	return r;
}

void seleccion_torneo(
	set<int> individuos,
	vector<result_set> &poblacion,
	vector<Zombie> zv, int w, int h, int z,
	vector<result_set> &poblacion_nueva){

	set<int>::iterator it;
	float max_aptitup = -1;
	int pos_final = -1;	
	result_set a_i;	
	
	for (it=individuos.begin(); it!=individuos.end(); ++it)
	{
		poblacion[*it] = aptitud((poblacion[*it]).individuo,zv, w, h, z);

		if ((float)poblacion[*it].plantas / poblacion[*it].turnos > max_aptitup)
		{
			max_aptitup = (float)poblacion[*it].plantas / poblacion[*it].turnos;
			pos_final = *it;
		}
	}	
	
	poblacion_nueva.push_back(poblacion[pos_final]);
	poblacion.erase(poblacion.begin()+pos_final);	
}

void cruce_clasico(
	vector<result_set> &poblacion,
	vector<Zombie> zv, int w, int h, int z
	){ 
	
	int crossing = 2 * ( ( mtrand() % (poblacion.size()/2)) + 1 );

	set<int> m = getNrandDistinct(crossing, poblacion.size());
	char plant;
	int pos_cruce;
	result_set r;
	string i1, i2;
	set<int>::iterator it=m.begin();
	r.sobrevive = false;
	r.plantas = 0;
	r.turnos = 1;

	while( it!= m.end() )
	{
		i1 = poblacion[*it].individuo;
		advance(it,1);
		i2 = poblacion[*it].individuo;
		advance(it,1);

		if( mtrand_closed() <= FACTOR_CRUCE )
		{	
			pos_cruce = 0;

			while ( pos_cruce ==  0 || pos_cruce == MAX_COLUMNAS-1 ){
				pos_cruce = mtrand() % MAX_COLUMNAS;
			}

			for(int i = 0; i < w; ++i)
				for (int j = pos_cruce; j< MAX_COLUMNAS; ++j){
					plant = i1[ i* MAX_COLUMNAS + j ];
					i1[ i* MAX_COLUMNAS + j ] = i2[ i* MAX_COLUMNAS + j ];;
					i2[ i* MAX_COLUMNAS + j ] = plant;
				}
			
			r.individuo = i1;
			r = aptitud(r.individuo, zv, w, h, z);
			poblacion.push_back(r);

			r.individuo = i2;
			r = aptitud(r.individuo, zv, w, h, z);
			poblacion.push_back(r);
		}		
	}
}

void mutacion_clasica(
	vector<result_set> &poblacion,
	vector<Zombie> zv, int w, int h, int z
	){
	int mutating = (mtrand() % (int)abs(poblacion.size() - MIN_INDIVIDUOS_MUTACION)) + MIN_INDIVIDUOS_MUTACION ;
	set<int> m = getNrandDistinct(mutating, poblacion.size());	
	char plant;
	int pos_mutacion;

	for(set<int>::iterator it=m.begin(); it!=m.end(); ++it){
		if( fmod(mtrand_closed(),MAX_FACTOR_MUTACION) + MIN_FACTOR_MUTACION  <= FACTOR_MUTACION)
		{				
			pos_mutacion = mtrand() % poblacion[*it].individuo.size();
			while( (plant = randPlant()) ==  poblacion[*it].individuo[pos_mutacion]);
			poblacion[*it].individuo[pos_mutacion] = plant;
			poblacion[*it] = aptitud((poblacion[*it]).individuo,zv, w, h, z);
		} 
	}
}

int main(){	
	
	init();
	int w, h, z, ps, t, g = 0;
	vector<Zombie> zv = vector<Zombie>();	
	loadEntry(w, h, z, zv, "input.in");	
	set<int> individuos = set<int>();
	vector<result_set> poblacion, poblacion_clone;
	result_set best;
	
	poblacion = generarPoblacion(w);
	best.sobrevive = false;	

	while(g++ < MAX_GENERACIONES and !best.sobrevive){
		ps = poblacion.size();
		if(ps == 1){
			cout<<"No ha sido posible conseguir una buena solucion, se presenta la combinacion encontrada con mejor heuristica encontrada."<<endl;
			break;
		}

		for(
			int i = 0; 
			i < ceil((float)ps / INDIVIDUOS_POR_TORNEO); 
			++i
		){			
			seleccion_torneo(
				getNrandDistinct(INDIVIDUOS_POR_TORNEO, poblacion.size()),
				poblacion,
				zv, w, h, z,
				poblacion_clone		
			);			
		}
		poblacion = poblacion_clone;		
		poblacion_clone.clear();		
		
		cruce_clasico(poblacion, zv, w, h ,z);
		mutacion_clasica(poblacion, zv, w, h ,z);

		sort(poblacion.begin(), poblacion.end(), compara_aptitud);
		best = poblacion[ poblacion.size() > 0 ? poblacion.size() -1 : 0 ];

		/*cout<<"-- "<<poblacion.size()<<" --"<<endl;
		for(int i =0;i<poblacion.size();++i){			
			cout<<"Individuo ["<<i<<"] "<<poblacion[i].individuo<<" "<<poblacion[i].sobrevive<<" "<<poblacion[i].plantas<<" "<<poblacion[i].turnos<<" "<<(float)poblacion[i].plantas/poblacion[i].turnos<<endl;
		}*/
	}
	printMatrix(best.individuo, w, MAX_COLUMNAS);
	cout<<"Heuristica: "<<(float)best.plantas / best.turnos <<endl;

	return EXIT_SUCCESS;
}
