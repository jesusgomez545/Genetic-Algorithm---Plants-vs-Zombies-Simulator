#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
*
* Conjunto de constantes necesarios para la
* simulacion del juego
*
*/
extern const int MAX_COLUMNAS = 4;
extern const int MAX_VIDA_ZOMBIE = 9;
extern const int MAX_VIDA_PLANTA = 1;
extern const int MAX_PASO_ZOMBIE = 1;
extern const int MAX_ELEMENT_TYPE = 4;
extern const int IMPACTO_PEASHOTTER = 1;
extern const int IMPACTO_REPEATER = 2;

/*
*
* Conjunto de constantes necesarios para la
* construccion del Algoritmo Genertico
*
*/

extern const int INDIVIDUOS_POR_POBLACION = 300;
extern const int MAX_GENERACIONES = 50;
extern const int INDIVIDUOS_POR_TORNEO = 2;
extern const float FACTOR_MUTACION = 0.0055;
extern const float MIN_FACTOR_MUTACION = 0.001;
extern const float MAX_FACTOR_MUTACION = 0.01;
extern const float FACTOR_CRUCE = 0.5;
extern const float MIN_FACTOR_CRUCE = 0;
extern const float MAX_FACTOR_CRUCE = 1;

enum PLANTA { VACIO, PEASHOTTER, REPEATER, POTATO_MINE };

#endif // CONSTANTS_H