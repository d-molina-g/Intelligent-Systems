#include "glob.h"
#include "ACO.h"


//Función que inicializa los valores para que sean utilizados
//en este archivo
ACO::ACO (int nAnts, int nCities, 
		double alpha, double beta, double q, double ro, double taumax,
		int initCity) {
	NUMBEROFANTS 	= nAnts;
	NUMBEROFCITIES 	= nCities;
	ALPHA 			= alpha;
	BETA 			= beta;
	Q 				= q;
	RO 				= ro;
	TAUMAX 			= taumax;
	INITIALCITY		= initCity;

	randoms = new Randoms (21);	
}
//Función que elimina los datos de memoria
ACO::~ACO () {
	for(int i=0; i<NUMBEROFCITIES; i++) {
		delete [] GRAPH[i];
		delete [] CITIES[i];
		delete [] PHEROMONES[i];
		delete [] DELTAPHEROMONES[i];
		if(i < NUMBEROFCITIES - 1) {
			delete [] PROBS[i];
		}
	}
	delete [] GRAPH;
	delete [] CITIES;
	delete [] PHEROMONES;
	delete [] DELTAPHEROMONES;
	delete [] PROBS;
}
//Se crean los arreglos y se le asignan los valores predeterminados
void ACO::init () {
	GRAPH 			= new int*[NUMBEROFCITIES];
	CITIES 			= new double*[NUMBEROFCITIES];
	PHEROMONES 		= new double*[NUMBEROFCITIES];
	DELTAPHEROMONES = new double*[NUMBEROFCITIES];
	PROBS 			= new double*[NUMBEROFCITIES-1];
	for(int i=0; i<NUMBEROFCITIES; i++) {
		GRAPH[i] 			= new int[NUMBEROFCITIES];
		CITIES[i] 			= new double[2];
		PHEROMONES[i] 		= new double[NUMBEROFCITIES];
		DELTAPHEROMONES[i] 	= new double[NUMBEROFCITIES];
		PROBS[i] 			= new double[2];
		for (int j=0; j<2; j++) {
			CITIES[i][j] = -1.0;
			PROBS[i][j]  = -1.0;
		}
		for (int j=0; j<NUMBEROFCITIES; j++) {
			GRAPH[i][j] 			= 0;
			PHEROMONES[i][j] 		= 0.0;
			DELTAPHEROMONES[i][j] 	= 0.0;
		}
	}	

	ROUTES = new int*[NUMBEROFANTS];
	for (int i=0; i<NUMBEROFANTS; i++) {
		ROUTES[i] = new int[NUMBEROFCITIES];
		for (int j=0; j<NUMBEROFCITIES; j++) {
			ROUTES[i][j] = -1;
		}
	}
	
	BESTLENGTH = (double) INT_MAX;
	BESTROUTE  = new int[NUMBEROFCITIES];
	for (int i=0; i<NUMBEROFCITIES; i++) {
		BESTROUTE[i] = -1;	
	}
}

/*
Funcion utilizada para agregar las conecciones que tiene 
cada ciudad, mientras se agrega la coneccion se ingresan 
los valores iniciales en los arreglos correspondientes   
*/
void ACO::connectCITIES (int cityi, int cityj) {
	GRAPH[cityi][cityj] = 1;
	PHEROMONES[cityi][cityj] = randoms -> Uniforme() * TAUMAX;
	GRAPH[cityj][cityi] = 1;
	PHEROMONES[cityj][cityi] = PHEROMONES[cityi][cityj];
}
//Se asignan la posición de eje x e i de cada ciudad
void ACO::setCITYPOSITION (int city, double x, double y) {
	CITIES[city][0] = x;
	CITIES[city][1] = y;
}
//Esta funcion imprime las feromonas que tiene cada ciudad
//en forma de matriz.
void ACO::printPHEROMONES () {	
	cout << " PHEROMONES: " << endl;
	cout << "  | ";
	for (int i=0; i<NUMBEROFCITIES; i++) {
		printf("%5d   ", i);
	}
	cout << endl << "- | ";
	for (int i=0; i<NUMBEROFCITIES; i++) {
		cout << "--------";
	}
	cout << endl;
	for (int i=0; i<NUMBEROFCITIES; i++) {
		cout << i << " | ";
		for (int j=0; j<NUMBEROFCITIES; j++) {
			if (i == j) {
				printf ("%5s   ", "x");
				continue;
			}
			if (exists(i, j)) {
				printf ("%7.3f ", PHEROMONES[i][j]);
			}
			else {
				if(PHEROMONES[i][j] == 0.0) {
					printf ("%5.0f   ", PHEROMONES[i][j]);
				}
				else {
					printf ("%7.3f ", PHEROMONES[i][j]);
				}
			}
		}
		cout << endl;
	}
	cout << endl;
}

//Funcion que calcula la distancia que hay entre
//entre las ciudades pasadas coo parametro
double ACO::distance (int cityi, int cityj) {
	return (double) 
		sqrt (pow (CITIES[cityi][0] - CITIES[cityj][0], 2) + 
 			  pow (CITIES[cityi][1] - CITIES[cityj][1], 2));
}
bool ACO::exists (int cityi, int cityc) {
	return (GRAPH[cityi][cityc] == 1);
}
//funcion que valida si a sido visitada la ciudad
bool ACO::vizited (int antk, int c) {
	for (int l=0; l<NUMBEROFCITIES; l++) {
		if (ROUTES[antk][l] == -1) {
			break;
		}
		if (ROUTES[antk][l] == c) {
			return true;
		}
	}
	return false;
}
double ACO::PHI (int cityi, int cityj, int antk) {
	double ETAij = (double) pow (1 / distance (cityi, cityj), BETA);
	double TAUij = (double) pow (PHEROMONES[cityi][cityj],   ALPHA);

	double sum = 0.0;
	for (int c=0; c<NUMBEROFCITIES; c++) {
		if (exists(cityi, c)) {
			if (!vizited(antk, c)) {
				double ETA = (double) pow (1 / distance (cityi, c), BETA);
				double TAU = (double) pow (PHEROMONES[cityi][c],   ALPHA);
				sum += ETA * TAU;
			}	
		}	
	}
	return (ETAij * TAUij) / sum;
}

double ACO::length (int antk) {
	double sum = 0.0;
	for (int j=0; j<NUMBEROFCITIES-1; j++) {
		sum += distance (ROUTES[antk][j], ROUTES[antk][j+1]);	
	}
	return sum;
}

int ACO::city () {
	double xi = randoms -> Uniforme();
	int i = 0;
	double sum = PROBS[i][0];
	while (sum < xi) {
		i++;
		sum += PROBS[i][0];
	}
	return (int) PROBS[i][1];
}
//Va guardando la ruta tomada por la hormiga
void ACO::route (int antk) {
	ROUTES[antk][0] = INITIALCITY;
	for (int i=0; i<NUMBEROFCITIES-1; i++) {		
		int cityi = ROUTES[antk][i];
		int count = 0;
		for (int c=0; c<NUMBEROFCITIES; c++) {
			if (cityi == c) {
				continue;	
			}
			if (exists (cityi, c)) {
				if (!vizited (antk, c)) {
					PROBS[count][0] = PHI (cityi, c, antk);
					PROBS[count][1] = (double) c;
					count++;
				}

			}
		}
		
		// deadlock
		if (0 == count) {
			return;
		}
		
		ROUTES[antk][i+1] = city();
	}
}
//Valida la ruta tomada por la hormiga
int ACO::valid (int antk, int iteration) {
	for(int i=0; i<NUMBEROFCITIES-1; i++) {
		int cityi = ROUTES[antk][i];
		int cityj = ROUTES[antk][i+1];
		if (cityi < 0 || cityj < 0) {
			return -1;	
		}
		if (!exists(cityi, cityj)) {
			return -2;	
		}
		for (int j=0; j<i-1; j++) {
			if (ROUTES[antk][i] == ROUTES[antk][j]) {
				return -3;
			}	
		}
	}
	
	if (!exists (INITIALCITY, ROUTES[antk][NUMBEROFCITIES-1])) {
		return -4;
	}
	
	return 0;
}
//Se imprime la matriz del grafo
void ACO::printGRAPH () {
	cout << " GRAPH: " << endl;
	cout << "  | ";
	for( int i=0; i<NUMBEROFCITIES; i++) {
		cout << i << " ";
	}
	cout << endl << "- | ";
	for (int i=0; i<NUMBEROFCITIES; i++) {
		cout << "- ";
	}
	cout << endl;
	int count = 0;
	for (int i=0; i<NUMBEROFCITIES; i++) {
		cout << i << " | ";
		for (int j=0; j<NUMBEROFCITIES; j++) {
			if(i == j) {
				cout << "x ";	
			}
			else {
				cout << GRAPH[i][j] << " ";	
			}
			if (GRAPH[i][j] == 1) {
				count++;	
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "Number of connections: " << count << endl << endl;
}
//se imprime los resultados del mejor tour encontrado
//la salida del archivo se encuentra en la carpeta Salida
void ACO::printRESULTS (char *argv[], string time_ejecution, string time_optimize, string SNUMBEROFCITIES, string SNUMBEROFANTS, string SINITIALCITY,string SALPHA,string SBETA, string SQ, string SRO) {
	ofstream output(argv[2]);

	output<<"Tiempo de Ejecucion:	"<<time_ejecution<<"	seg."<<endl;
	output<<"Tiempo de Optimize:	"<<time_optimize<<"	seg."<<endl;
	output<<"Total de Ciudades:	"<<SNUMBEROFCITIES<<endl;
	output<<"Total de Hormigas:	"<<SNUMBEROFANTS<<endl;
	output<<"Ciudad Inicial:	"<<SINITIALCITY<<endl;
	output<<"ALPHA:	"<<SALPHA<<endl;
	output<<"BETA:	"<<SBETA<<endl;
	output<<"Q:	"<<SQ<<endl;
	output<<"RO:	"<<SRO<<endl;
	BESTLENGTH += distance (BESTROUTE[NUMBEROFCITIES-1], INITIALCITY);
	output << "Distancia:	" << BESTLENGTH << endl;
	cout << " BEST ROUTE:" << endl;

	for (int i=0; i<NUMBEROFCITIES; i++) {
		cout << BESTROUTE[i] +1<< " ";
		output<< BESTROUTE[i]+1<<endl;
	}
	output.close();
	cout << endl << "length: " << BESTLENGTH << endl;
}
//Actualiza las feromonas
void ACO::updatePHEROMONES () {
	for (int k=0; k<NUMBEROFANTS; k++) {
		double rlength = length(k);
		for (int r=0; r<NUMBEROFCITIES-1; r++) {
			int cityi = ROUTES[k][r];
			int cityj = ROUTES[k][r+1];
			DELTAPHEROMONES[cityi][cityj] += Q / rlength;
			DELTAPHEROMONES[cityj][cityi] += Q / rlength;
		}
	}
	for (int i=0; i<NUMBEROFCITIES; i++) {
		for (int j=0; j<NUMBEROFCITIES; j++) {
			PHEROMONES[i][j] = (1 - RO) * PHEROMONES[i][j] + DELTAPHEROMONES[i][j];
			DELTAPHEROMONES[i][j] = 0.0;
		}	
	}
}


void ACO::optimize (int ITERATIONS) {
	for (int iterations=1; iterations<=ITERATIONS; iterations++) {
		cout << flush;
		cout << "ITERATION " << iterations << " HAS STARTED!" << endl << endl;
		/*
		 se pocede con la busqueda que toma cada hormiga  
		 se va iterando hasta las K hormigas              
		 */

		
		for (int k=0; k<NUMBEROFANTS; k++) {
			cout << " : ant " << k << " has been released!" << endl;
			//Va validando la ruta que toma la ormiga
			while (0 != valid(k, iterations)) {
				cout << "  :: releasing ant " << k << " again!" << endl;
				for (int i=0; i<NUMBEROFCITIES; i++) {
					ROUTES[k][i] = -1;	
				}
				//se acualiza mejor ruta tomada por la hormiga
				route(k);
			}
			//se imprime la ruta tomada por la hormiga
			for (int i=0; i<NUMBEROFCITIES; i++) {
				cout << ROUTES[k][i] << " ";	
			}
			cout << endl;
			
			cout << "  :: route done" << endl;
			double rlength = length(k);
			//se compara las rutas y se actualiza la mejor ruta encontrada
			if (rlength < BESTLENGTH) {
				BESTLENGTH = rlength;
				for (int i=0; i<NUMBEROFCITIES; i++) {
					BESTROUTE[i] = ROUTES[k][i];
				}
			}
			cout << " : ant " << k << " has ended!" << endl;				
		}		
		//Se procede a actualizar las feromonas
		cout << endl << "updating PHEROMONES . . .";
		updatePHEROMONES ();
		cout << " done!" << endl << endl;
		printPHEROMONES ();
		//se resetea la ruta para la siguiente iteracion
		for (int i=0; i<NUMBEROFANTS; i++) {
			for (int j=0; j<NUMBEROFCITIES; j++) {
				ROUTES[i][j] = -1;
			}
		}

		cout << endl << "ITERATION " << iterations << " HAS ENDED!" << endl << endl;
	}
}
