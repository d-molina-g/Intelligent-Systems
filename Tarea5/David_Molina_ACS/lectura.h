#include "glob.h"


class City
{
	private:
			int city;
			double x;
			double y;
			double distancia;
	public:
			void newCity(int, double, double);
			void setCity(int);
			void setX(double);
			void setY(double);
			void setDistancia(double);
			int getCity();
			double getX();
			double getY();
			double getDistancia();
};

string Convert_Int_string(int date);
void ModifyInput(char *argv[]);
vector<City> SaveInput();

double CalcularTiempo(unsigned ti, unsigned tf);
