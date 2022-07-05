#include "Dispensador.h"
#include <iostream>
#include <string>

Dispensador::Dispensador() {
	cantidad200 = 0;
	cantidad100 = 0;
	cantidad50 = 0;
	cantidad20 = 0;
	cantidad10 = 0;
}

Dispensador::Dispensador(int _cantidad200, int _cantidad100, int _cantidad50, int _cantidad20, int _cantidad10) {
	cantidad200 = _cantidad200;
	cantidad100 = _cantidad100;
	cantidad50 = _cantidad50;
	cantidad20 = _cantidad20;
	cantidad10 = _cantidad10;
}