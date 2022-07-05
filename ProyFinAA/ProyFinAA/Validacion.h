#include<iostream>
using namespace std;

bool validarTexto(string texto, string error) {
	if (texto == "") {
		cout << "Ingrese un(a) " << error << " valido(a).";
		return false;
	}
	return true;
}

bool validarDatos() {
	if (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');
		return false;
	}
	return true;
}

bool validarDatos(string texto) {
	if (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << texto << endl;
		return false;
	}
	return true;
}

bool validarNumeros(int menor, int mayor, int m, string texto) {
	if (m<menor || m>mayor) {
		cout << texto << endl;
		return false;
	}
	return true;
}

bool validarRango(float menor, float mayor, float m, string texto) {
	if (m<menor || m>mayor) {
		cout << texto << endl;
		return false;
	}
	return true;
}