#include "Movimiento.h"
#include <iostream>
using namespace std;

Movimiento::Movimiento(string _nro, string _tipo, float _monto, string _fechaM) {
	numCuenta = _nro;
	tipo = _tipo;
	monto = _monto;
	fechaM = _fechaM;
}