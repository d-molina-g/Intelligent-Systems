#include "lectura.h"
//metodos que utiliza la clase CIty para la lectura del archivo de entrada
void City::newCity(int val_city, double val_x, double val_y)
{
	city = val_city;
	x = val_x;
	y = val_y;
}
void City::setCity(int val_city){city = val_city;}
void City::setX(double val_x){x = val_x;}
void City::setY(double val_y){y = val_y;}
void City::setDistancia(double dist){distancia = dist;}
int City::getCity(){return city;}
double City::getX(){return x;}
double City::getY(){return y;}
double City::getDistancia(){ return distancia;}

//Funcion que convierte un valor Int a String
string Convert_Int_string(int date)
{
	stringstream convert;
	convert <<date;
	string newdate = convert.str();
	return newdate;
}
//Funcion que convierte un valor Double a string
string Convert_double_string(double time)
{
	stringstream convert;
	convert<<time;
	string t = convert.str();
	return t;
}
//Funcion que modifica el archivo de entrada orignal
//y crea un archivo valido para la siguiente lectura
void ModifyInput(char *argv[])
{
	ifstream input(argv[1]);
	ofstream output("Entrada/input2.tsp");
	if(!input)
	{
		cout<<"Error lectura input"<<endl;
		exit(-1);
	}
	bool startCities = false;
	while(!input.eof())
	{
		char line[128];
		input.getline(line,128);

		char *token = strtok (line," ");
		string str = token;
		//para que no lea la ultima linea
		if(input.eof() || str.compare("EOF")==0){break;}

		if(str.compare("1") == 0){startCities = true;}
		if(startCities)
		{
			//cout<<"Start ModifyInput"<<endl;
			int val_city;
			double x, y;
			
			int select = 0;
			while(token != NULL)
			{
				if(select == 0)
				{
					val_city = atoi(token)-1;
					select+=1;
				}
				else if(select == 1)
				{
					x = atof(token);
					select+=1;
				}
				else if(select == 2)
				{
					y = atof(token);
				}
				token = strtok (NULL," ");
			}
		
			cout<<"Ciudad : "<<val_city<<" ----x: "<<x<<"	----y: "<<y<<endl;
			string c = Convert_Int_string(val_city);
			string sx = Convert_double_string(x);
			string sy = Convert_double_string(y);
			output<<c<<" "<<sx<<" "<<y<<endl;
		}
	}
	output<<"EOF";
	input.close();
	output.close();
}


//Funcion que calcula el tiempo
double CalcularTiempo(unsigned ti, unsigned tf)
{
	return (double(tf- ti)/CLOCKS_PER_SEC);
}



//Lee el archivo creado por la funcion ModifyInput
//y guarda los puntos utilizando la clase City
vector<City> SaveInput()
{
	ifstream input("Entrada/input2.tsp");

	if (!input)
	{
		cout<<"Error read input."<<endl;
		exit(-1);
	}

	vector<City> citys;
	while(!input.eof())
	{
		char line[128];
		input.getline(line,128);

		char *token = strtok (line," ");
		//para que no lea la ultima linea
		if(input.eof()){break;}

		int val_city;
		double x, y;
		
		int select = 0;
		while(token != NULL)
		{
			if(select == 0)
			{
				val_city = atoi(token);
				select+=1;
			}
			else if(select == 1)
			{
				x = atof(token);
				select+=1;
			}
			else if(select == 2)
			{
				y = atof(token);
			}
			token = strtok (NULL," ");
		}
		City city;
		//cout<<"Ciudad : "<<val_city<<" ----x: "<<x<<"	----y: "<<y<<endl;
		city.newCity(val_city,x,y);
		citys.push_back(city);
		//getchar();
	}
	input.close();
	cout<<"total de ciudades: "<<citys.size()<<endl;
	return citys;
}

