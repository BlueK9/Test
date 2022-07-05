#include "Cliente.h"
#include <time.h>
#include <iostream>

Cliente::Cliente(string _numeroCuenta, string _nombre, struct tm _fechaNac, float _monto) {
	numeroCuenta = _numeroCuenta;
	nombre = _nombre;
	fechaNac = _fechaNac;
	monto = _monto;
}
Cliente::~Cliente() {
}
int Cliente::getContrasenia() {
	return contrasenia;
}
void Cliente::setContrasenia(int _contrasenia) {
	contrasenia = _contrasenia;
}