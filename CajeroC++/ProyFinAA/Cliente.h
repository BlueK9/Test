#include <iostream>
#include <time.h>
using namespace std;

class Cliente
{
	public:
		string numeroCuenta;
		string nombre;
		struct tm fechaNac;
		float monto;
		Cliente(string, string, struct tm, float);
		~Cliente();
		int getContrasenia();
		void setContrasenia(int _contrasenia);
	private:
		int contrasenia;
};

