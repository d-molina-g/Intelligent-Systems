#include <cstdlib>

#include <cmath>
#include <limits>

class Randoms {

private:
	long xpto;
public:
	//Generador de semilla
	Randoms (long x) {xpto = -x;}
	//retorna un numero gaussiano randoom
	double Normal (double avg, double sigma)
	{
		return (avg+sigma*gaussdev(&xpto)) ;
	}
	//Retorna un numero uniforme random entre 0 y 1
	double Uniforme()	
	{
		return ran1(&xpto);
	}
	//Retorna un numero random entre -m y m
	double sorte(int m)
	{ 
		return (1.0*rand())/(1.0*RAND_MAX)*2.0*m-m;
	}

#define   IA 16807
#define   IM 2147483647
#define   AM (1.0/IM)
#define   IQ 127773
#define   IR 2836
#define   NTAB 32
#define   NDIV (1+(IM-1)/NTAB)
#define   EPS 1.2e-7
#define   RNMX (1.0-EPS)

float ran1(long *idum)
/***********************************************************
 *Generador de numeros aleatorios de Park y Miller con     *
 *Bays-Durham Shuffle. Devuelve un desvio random uniforme  *
 *entre 0.0 y 1.0 (excluye los valores del ultimo punto).  *
 *Llama con idam un valor entero negativo para inicializar *
 *despues, no se altera idum entre valores secuenciales    * 
 *RNMX aproxima el valor flotante más grande menor a 1     *
 ***********************************************************/
{
	 int j;
	 long k;
	 static long iy=0;
	 static long iv[NTAB];
	 float temp;
	 if (*idum <= 0 || !iy) {        
		 if (-(*idum) < 1) *idum=1;    
		 else *idum = -(*idum);
		 for (j=NTAB+7;j>=0;j--) {     
			  k=(*idum)/IQ;
			  *idum=IA*(*idum-k*IQ)-IR*k;
			  if (*idum < 0) *idum += IM;
			  if (j < NTAB) iv[j] = *idum;
		 }
		 iy=iv[0];
	 }
	 k=(*idum)/IQ;                     // Comienza aquí cuando no se a inicializado
	 *idum=IA*(*idum-k*IQ)-IR*k;       // calcula idum=(IA*idum) % IM 
	 if (*idum < 0) *idum += IM;       
	 j=iy/NDIV;                     
	 iy=iv[j];                        
	 iv[j] = *idum;                   
	 if ((temp=AM*iy) > RNMX) 
		return RNMX; 				   
	 else 
		return temp;
}

float gaussdev(long *idum)
// Returns a normally distributed deviate with zero mean and unit variance, 
// using ran1(idum) as the source of uniform deviates.
{
//    float ran1(long *idum);

	static int iset=0;
	static float gset;
	float fac,rsq,v1,v2;
	if (*idum < 0) iset=0;      //     Reinicializa.
	if (iset == 0) {            //     si no se tiene un desvio adicioanl
	  do {
			 v1=2.0*ran1(idum)-1.0;    // Eligen dos numeros uniformes al cuadrado 
			 v2=2.0*ran1(idum)-1.0;    // que se extienden de -1 a +1 en cada direccion,
			 rsq=v1*v1+v2*v2;          // se verifica si esta entre 1.0 y 0.0

	  } while (rsq >= 1.0 || rsq == 0.0);  // si no esta entre los valores se vuelve a calcular.
	  fac=sqrt(-2.0*log(rsq)/rsq);
	  //Se hace la trandormacion Box-Muller para obtener dos desiaciones normales.
	  //se retorna uno y el otro se guarda para la próxima vez
	  gset=v1*fac;
	  iset=1;                
	  return v2*fac;
  } else {                    
	  iset=0;                
	  return gset;
  }
}

};
