#include "glob.h"
#include "lectura.h"

#include "ACO.h"

/*
David Molina GArrido
*/


/*
Las funciones Convert_doubleString y Convert_IntString    
son utilizadas para la conversión de valores Double e Int 
a String.                                                 
*/
string Convert_doubleString(double time)
{
	stringstream convert;
	convert<<time;
	string t = convert.str();
	return t;
}
string Convert_IntString(int date)
{
	stringstream convert;
	convert <<date;
	string newdate = convert.str();
	return newdate;
}


int main(int argc, char *argv[]) {

	//Variable contabilizadoras de tiempo
	unsigned timeI_Ejecution, timeF_Ejecution;
	unsigned timeI_Optimize, timeF_Optimize;

	
	timeI_Ejecution = clock();//Inicio Tiempo de Ejecución

	/*
	La funcion Mofify lee el archivo de entrada y lo     
	modifica y crea uno nuevo para que sea leido por la  
	funcion SaveInput, la cual se encarga de hacer la    
	lectura del nuevo archivo creado guardar las         
	ciudades y su posicion                               
	*/

	ModifyInput(argv);
	vector<City>Citys = SaveInput();
	
	/*
	Se definen las variable para ser utilizadas en la    
	función ACO                                          
	*/

	int NUMBEROFCITIES = Citys.size();
	int ITERATIONS = 10;
	int NUMBEROFANTS = 10;
	//Se puede hacer que la ciudad inicial sea fija o que sea aleatoria
	//Fija
	int INITIALCITY = 0;
	// si ALPHA == 0  (busqueda estcastiva & ruta sub-optima)
	double ALPHA = 0.1;
	// si BETA  == 0 ruta sub-optima
	double BETA	= 2;
	//estimacion de la mejor ruta
	double Q = 0.9;
	//Evaporacion de feromonas
	double RO = 0.1;
	//Maxima número random de peromona
	int TAUMAX = 2;

	//Se inicializa la función ACO con los parametros anteriores
	ACO *ANTS = new ACO (NUMBEROFANTS, NUMBEROFCITIES, 
			 			ALPHA, BETA, Q, RO, TAUMAX,
			 			INITIALCITY);
	/*
	Se llama a la función init para que cree todos los  
	arreglos a utilizar para la búsqueda del mejor tour 
	*/

	ANTS -> init();

	//Inicio ingreso de Conecciones de las ciudades a través
	//de la funcion connectCITIES
	vector<City>::iterator it1;
	vector<City>::iterator it2;
	for(it1 = Citys.begin() ; it1 != Citys.end() ; it1++)
	{
		City city1 = *it1;
		int citie1 = city1.getCity();
		for(it2 = Citys.begin() ; it2 != Citys.end() ; it2++)
		{
			City city2 = *it2;
			int citie2 = city2.getCity();
			if( citie1 != citie2)
			{
				ANTS -> connectCITIES (citie1, citie2);
			}
		}
	}
	//Inicio ingreso de posiciones de cada ciudad
	// a través de la función SetCITYPOSITION
	vector<City>::iterator it;
	for(it = Citys.begin() ; it != Citys.end() ; it++)
	{
		City city = *it;

		int citie = city.getCity();
		double x = city.getX();
		double y = city.getY();
		ANTS -> setCITYPOSITION ( citie, x, y);
	}

	//Se imprime el grafo en forma de matriz de las ciudades
	//indicando la direccion que tiene cada ciudad
	ANTS -> printGRAPH ();

	//Se imprime la matriz con las feromonas que tiene cada ciudad
	ANTS -> printPHEROMONES ();

	/*
	las variables timeI_Optimize y timeF_Optimize son     
	utilizadas para calcular el tiempo que se demora en   
	buscar el mejor tour.                                 
	Se llama a la función optimize para comenzar con la   
	búsqueda del mejor tour, y se le pasa la cantidad de  
	iteraciones que se deben de realizar                  
	*/
	timeI_Optimize = clock();
	ANTS -> optimize (ITERATIONS);
	timeF_Optimize = clock();

	//Fin Tiempo de Ejecicion
	timeF_Ejecution = clock();

	/*
	Se procede a calcular los tiempos y aconvertir los    
	tiempos a string para ser pasados como parametros a la
	función printRESULTS                                  
	*/

	double timeT_Optimize = CalcularTiempo(timeI_Optimize,timeF_Optimize);
	double timeT_Ejecution = CalcularTiempo(timeI_Ejecution, timeF_Ejecution);
	string sto = Convert_doubleString(timeT_Optimize);
	string ste = Convert_doubleString(timeT_Ejecution);
	string SNUMBEROFCITIES = Convert_IntString(NUMBEROFCITIES);
	string SNUMBEROFANTS = Convert_IntString(NUMBEROFANTS);
	string SINITIALCITY = Convert_IntString(INITIALCITY+1);
	string SBETA = Convert_doubleString(BETA);
	string SALPHA = Convert_doubleString(ALPHA);
	string SQ = Convert_doubleString(Q);
	string SRO = Convert_doubleString(RO);
	
	//Se guarda los resultados en un archivo .tsp ubicado en la carpeta Salida
	ANTS -> printRESULTS (argv, ste, sto, SNUMBEROFCITIES, SNUMBEROFANTS, SINITIALCITY, SALPHA, SBETA, SQ, SRO);

	return 0;
}
