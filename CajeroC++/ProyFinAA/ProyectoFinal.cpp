#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <time.h>

//Librerias creadas
#include "Cliente.h"
#include "Dispensador.h"
#include "Movimiento.h"
#include "Validacion.h"

using namespace std;

//Fechas y horas
time_t t = time(NULL);
struct tm tiempoLocal = *localtime(&t);
char fechaHora[70];
char* formatoYMDHMS = "%Y-%m-%d %H:%M:%S";
char* formatoDMY = "%d-%m-%Y";
int bytesEscritos = strftime(fechaHora, sizeof fechaHora, formatoYMDHMS, &tiempoLocal);


// Lectura de archivos
void leerArchivoDispensador();
void leerArchivoClientes();
void leerArchivoMovimientos();

// Escritura de archivos
void escribirArchivoCliente();
void escribirArchivoDispensador();
void registrarMovimientos(Cliente cliente, string tipo, float monto);

// Sub-Menús
void menuAdministrador();
void menuActualizarCliente();
void menuActualizarDispensador();
void menuCliente(Cliente* cliente);
void PagarServicio(Cliente* cliente);

// Requerimientos
void guardarCliente();
Cliente* existeCliente(string num);
void eliminarCliente(string nro);
void mostrarClientes();
void actualizarDinero(int cantidad, int tipo);
void retirarDinero(float* dinero, float suma);
void depositarDinero(float* dinero, float suma);
void transferir(float* dineroCliente1, float* dineroCliente2, float suma);
void pagoServicio(float* dinero, float suma);
void imprimirMovimientos(string nro);
void cambioContrasenia(Cliente* cliente, int nuevaC);

// Funciones adicionales
void loginCliente();
float leerMonto(string msg);
bool GiveMoney(float monto);
void getMoney(float monto);
int dineroServicio(Cliente* cliente);

// Archivos
const string archivoDispensador = "Dispensaador.txt";
const string archivoClientes = "Clientes.txt";
const string archivoMovimientos = "Movimientos.txt";
const string temporal = "temporal.txt";

// Listas
vector<Cliente> listaCliente;
vector<Movimiento> listaMovimientos;

// Dispensador
Dispensador d1 = Dispensador(100, 100, 100, 200, 200);

int main() {
	cout << "            ####   ######   ######            ######     ###    ##   ##  ### ###" << endl;
	cout << "           ##  ##   ##  ##   ##  ##            ##  ##   ## ##   ###  ##   ## ##" << endl;
	cout << "          ##        ##  ##   ##  ##            ##  ##  ##   ##  #### ##   ####" << endl;
	cout << "          ##        #####    #####             #####   ##   ##  #######   ###" << endl;
	cout << "          ##        ##       ##                ##  ##  #######  ## ####   ####" << endl;
	cout << "           ##  ##   ##       ##                ##  ##  ##   ##  ##  ###   ## ##" << endl;
	cout << "            ####   ####     ####              ######   ##   ##  ##   ##  ### ###" << endl;
	setlocale(LC_ALL, "spanish");
	system("Color 0A");
	
	
	leerArchivoDispensador();
	leerArchivoClientes();
	leerArchivoMovimientos();

	cout << endl;
	char opcS = '0';
	do {
		cout << endl << "Iniciar sesión como:" << endl;
		cout << "1. Administrador" << endl;
		cout << "2. Cliente o usuario" << endl;
		cout << "3. Salir" << endl << endl;
		cout << "Eliga una opción: ";
		opcS = _getche();
		system("cls");
		cout << endl;
		switch (opcS)
		{
			case '1': {
				menuAdministrador();
				
				break;
			}
			case '2': {
				loginCliente();
				break;
			}
			case '3': {
				return 0;
			}
		}
	} while (opcS != '3');
	return 0;
}

void leerArchivoDispensador() {
	ifstream leer;
	leer.open(archivoDispensador.c_str(), ios::in);
	if (leer.fail()) {
		cout << "Ocurrio un error en la captura de datos" << endl;
		return;
	}
	string _200, _100, _50, _20, _10;
	getline(leer, _200, ';');
	getline(leer, _100, ';');
	getline(leer, _50, ';');
	getline(leer, _20, ';');
	getline(leer, _10, '\n');

	d1 = Dispensador(stoi(_200), stoi(_100), stoi(_50), stoi(_20), stoi(_10));
	leer.close();
}

void leerArchivoClientes() {
	ifstream leer;
	leer.open(archivoClientes.c_str(), ios::in);
	if (leer.fail()) {
		cout << "Ocurrio un error en la captura de datos" << endl;
		return;
	}
	string nroCuenta, nombre, monto, contrasenia;
	string d, m, a;
	while (!leer.eof()) {
		getline(leer, nroCuenta, ';');
		getline(leer, nombre, ';');
		getline(leer, d, '-');
		getline(leer, m, '-');
		getline(leer, a, ';');
		getline(leer, monto, ';');
		getline(leer, contrasenia, '\n');
		if (nroCuenta.length() <= 0) break;
		struct tm fechaN;
		fechaN.tm_year = stoi(a) - 1900;
		fechaN.tm_mon = stoi(m) - 1;
		fechaN.tm_mday = stoi(d);

		Cliente cliente = Cliente(nroCuenta, nombre, fechaN, stof(monto));
		cliente.setContrasenia(stoi(contrasenia));
		listaCliente.push_back(cliente);
	}
	leer.close();
}

void leerArchivoMovimientos() {
	ifstream leer;
	leer.open(archivoMovimientos.c_str(), ios::in);
	if (leer.fail()) {
		cout << "Ocurrio un error en la captura de datos" << endl;
		return;
	}
	string nroCuenta, tipo, monto, fecha;
	while (!leer.eof()) {
		getline(leer, nroCuenta, ';');
		getline(leer, tipo, ';');
		getline(leer, monto, ';');
		getline(leer, fecha, '\n');
		if (nroCuenta.length() <= 0) break;

		Movimiento movimiento = Movimiento(nroCuenta, tipo, stof(monto), fecha);
		listaMovimientos.push_back(movimiento);
	}
	leer.close();
}


void escribirArchivoCliente() {
	ofstream escribir;
	escribir.open(temporal.c_str(), ios::out | ios::app);
	int l = listaCliente.size();
	for (int i = 0; i < l; i++)
	{
		escribir << listaCliente[i].numeroCuenta << ";";
		escribir << listaCliente[i].nombre << ";";
		escribir << listaCliente[i].fechaNac.tm_mday << "-";
		escribir << listaCliente[i].fechaNac.tm_mon + 1 << "-";
		escribir << listaCliente[i].fechaNac.tm_year + 1900 << ";";
		escribir << listaCliente[i].monto << ";";
		escribir << listaCliente[i].getContrasenia() << "\n";
	}
	escribir.close();
	remove(archivoClientes.c_str());
	rename(temporal.c_str(), archivoClientes.c_str());
}

void escribirArchivoDispensador() {
	ofstream escribir;
	escribir.open(temporal.c_str(), ios::out | ios::app);
	escribir << d1.cantidad200 << ";";
	escribir << d1.cantidad100 << ";";
	escribir << d1.cantidad50 << ";";
	escribir << d1.cantidad20 << ";";
	escribir << d1.cantidad10 << "\n";
	escribir.close();
	remove(archivoDispensador.c_str());
	rename(temporal.c_str(), archivoDispensador.c_str());
}

void registrarMovimientos(Cliente cliente, string tipo, float monto) {
	t = time(NULL);
	tiempoLocal = *localtime(&t);
	int bytesEscritos = strftime(fechaHora, sizeof fechaHora, formatoYMDHMS, &tiempoLocal);
	Movimiento m = Movimiento(cliente.numeroCuenta, tipo, monto, fechaHora);
	listaMovimientos.push_back(m);
	ofstream escribir;
	escribir.open(archivoMovimientos.c_str(), ios::out | ios::app);
	escribir << m.numCuenta << ";";
	escribir << m.tipo << ";";
	escribir << m.monto << ";";
	escribir << m.fechaM << "\n";
	escribir.close();
}


void menuAdministrador() {
	system("cls");
	char opc = '0';
	do {
		cout << endl << "Actualizar informacion de: " << endl;
		cout << "1. Cliente" << endl;
		cout << "2. Dispensador" << endl;
		cout << "3. Salir" << endl << endl;
		cout << "Eliga una opción: ";
		opc = _getche();
		cout << endl;
		switch (opc)
		{
			case '1': {
				menuActualizarCliente();
				break;
			}
			case '2': {
				menuActualizarDispensador();
				break;
			}
		}
	} while (opc != '3');
}

void menuActualizarCliente() {
	system("cls");
	char opc = '0';
	do {
		cout << endl;
		cout << "ACTUALIZAR REGISTRO DE CLIENTES" << endl;
		cout << "1. Agregar cliente" << endl;
		cout << "2. Buscar cliente" << endl;
		cout << "3. Eliminar cliente" << endl;
		cout << "4. Ver lista de clientes" << endl;
		cout << "5. Salir" << endl << endl;
		cout << "Eliga una opción: ";
		opc = _getche();
		cout << endl;
		switch (opc)
		{
			case '1': {
				guardarCliente();
				break;
			}
			case '2': {
				string nro;
				cout << "Ingrese un numero de cuenta: ";
				cin >> nro;
				if (!validarTexto(nro, "numero de cuenta")) break;
				if (existeCliente(nro) != NULL) {
					Cliente* buscado = existeCliente(nro);
					int bytesEscritos = strftime(fechaHora, sizeof fechaHora, formatoDMY, &(buscado->fechaNac));
					cout << endl;
					cout << "Nombres: " << buscado->nombre << endl;
					cout << "Fecha nacimiento: " << fechaHora << endl;
					cout << "Monto: " << " S/. " << buscado->monto << endl << endl;
				}
				else cout << "Cliente no encontrado, ingrese otro numero de cuenta." << endl;
				break;
			}
			case '3': {
				string nro;
				cout << "Ingrese un numero de cuenta: ";
				cin >> nro;
				if (!validarTexto(nro, "numero de cuenta")) break;
				if (existeCliente(nro) != NULL) {
					eliminarCliente(nro);
					escribirArchivoCliente();
					cout << "El cliente fue eliminado correctamente." << endl << endl;
				}
				else cout << "Cliente no encontrado, ingrese otro numero de cuenta." << endl;
				break;
			}
			case '4': {
				mostrarClientes();
				break;
			}
		}
	} while (opc != '5');
}

void menuActualizarDispensador() {
	system("cls");
	char opc = '0';
	do {
		cout << endl;
		cout << "ACTUALIZAR DINERO DEL DISPENSADOR" << endl;
		cout << "1. Agregar dinero del dispensador" << endl;
		cout << "2. Retirar dinero del dispensador" << endl;
		cout << "3. Ver lista de dinero" << endl;
		cout << "4. Salir" << endl << endl;
		cout << "Eliga una opción: ";
		opc = _getche();
		cout << endl;
		switch (opc)
		{
		case '1': {
			int cantidad, tipo;
			cout << "Ingrese el tipo de cantidad de dinero a depositar: ";
			cin >> tipo;
			if (!validarDatos("Ingrese numeros!")) break;
			if (tipo != 200 && tipo != 100 && tipo != 50 && tipo != 20 && tipo != 10) {
				cout << "Los billetes deben ser de 200, 100, 50, 20 o 10" << endl;
				break;
			}
			cout << "Ingrese el numero de billetes: ";
			cin >> cantidad;
			if (!validarDatos("Ingrese numeros!")) break;
			if (!validarNumeros(1, 100, cantidad, "Ingrese una cantidad valida")) break;
			actualizarDinero(cantidad, tipo);
			escribirArchivoDispensador();
			break;
		}
		case '2': {
			int cantidad, tipo;
			cout << "Ingrese el tipo de cantidad de dinero a retirar: ";
			cin >> tipo;
			if (!validarDatos("Ingrese numeros!")) break;
			if (tipo != 200 && tipo != 100 && tipo != 50 && tipo != 20 && tipo != 10) {
				cout << "Los billetes deben ser de 200, 100, 50, 20 o 10." << endl;
				break;
			}
			cout << "Ingrese el numero de billetes: ";
			cin >> cantidad;
			if (!validarDatos("Ingrese numeros!")) break;
			if (!validarNumeros(1, 100, cantidad, "Ingrese una cantidad valida")) break;
			actualizarDinero((cantidad * -1), tipo);
			escribirArchivoDispensador();
			break;
		}
		case '3': {
			cout << endl << "Billetes de 200: " << d1.cantidad200 << endl;
			cout << "Billetes de 100: " << d1.cantidad100 << endl;
			cout << "Billetes de 50: " << d1.cantidad50 << endl;
			cout << "Billetes de 20: " << d1.cantidad20 << endl;
			cout << "Billetes de 10: " << d1.cantidad10 << endl;
			break;
		}
		}
	} while (opc != '4');
}

void menuCliente(Cliente* cliente) {
	system("cls");
	char opc = '0';
	do {
		cout << endl;
		cout << "MENÚ CLIENTE" << endl;
		cout << "1. Retirar Dinero" << endl;
		cout << "2. Depositar Dinero " << endl;
		cout << "3. Transferir Dinero " << endl;
		cout << "4. Pagar Servicio " << endl;
		cout << "5. Consultar saldo " << endl;
		cout << "6. Cambiar contraseña " << endl;
		cout << "7. Consular movimientos " << endl;
		cout << "8. Cerrar sesión" << endl << endl;

		cout << "Eliga opcion a requerir: ";
		opc = _getche();
		cout << endl;
		switch (opc)
		{
		case '1': {
			float retiro = leerMonto("Ingrese el monto de retiro: ");
			if (!(validarRango(0, cliente->monto, retiro, "El retiro de efectivo debe ser menor o igual al monto."))) break;
			if ((int)retiro % 10 != 0) {
				cout << "El retiro debe ser multiplo de 10." << endl;
				break;
			}
			if (!GiveMoney(retiro)) break;
			retirarDinero(&(cliente->monto), retiro);
			escribirArchivoCliente();
			registrarMovimientos(*(cliente), "Retiro", retiro);
			escribirArchivoDispensador();
			cout << "Consulte su monto para visualizarlo." << endl << endl;
			break;
		}
		case '2': {
			float deposito = leerMonto("Ingrese el monto de depósito: ");
			if (!(validarRango(10, 9000, deposito, "El depósito de efectivo debe estar entre 10 y 9000"))) break;
			if ((int)deposito % 10 != 0) {
				cout << "El depósito debe ser multiplo de 10." << endl;
				break;
			}
			getMoney(deposito);
			depositarDinero(&(cliente->monto), deposito);
			escribirArchivoCliente();
			registrarMovimientos(*cliente, "Depósito", deposito);
			escribirArchivoDispensador();
			cout << "Consulte su monto para visualizarlo." << endl << endl;
			break;
		}
		case '3': {
			string numCuenta;
			cout << "Ingrese un numero de cuenta para transferir: ";
			cin >> numCuenta;
			if (!validarDatos("Dato erróneo, ingrese números.")) break;
			if (existeCliente(numCuenta) == NULL) {
				cout << "El numero de cuenta no existe, ingrese otro." << endl;
				break;
			}
			if (cliente->numeroCuenta == numCuenta) {
				cout << "El numero de cuenta debe ser distinto al suyo." << endl;
				break;
			}
			float transferencia = leerMonto("Ingrese el monto de transferencia: ");
			if (!validarRango(1, cliente->monto, transferencia, "El monto de transferencia debe estar entre 1 y su monto total")) break;
			transferir(&(cliente->monto), &(existeCliente(numCuenta)->monto), transferencia);
			escribirArchivoCliente();
			registrarMovimientos(*(cliente), "Transferencia a " + numCuenta, transferencia);
			cout << "Se transfirió S/. " << transferencia << " a la cuenta " << numCuenta << " correctamente." << endl << endl;
			break;
		}
		case '4': {
			PagarServicio(cliente);
			escribirArchivoCliente();
			break;
		}
		case '5': {
			cout << "Su monto actual es de: " << "S/. " << cliente->monto << endl << endl;
			break;
		}
		case '6': {
			int contrasenia;
			cout << "Ingrese su nueva contrasenia: ";
			cin >> contrasenia;
			if (!validarDatos("Ingrese numeros en la seccion contraseña.")) break;
			if (to_string(contrasenia).size() != 4) {
				cout << "La contraseña o PIN secreto debe tener 4 digitos." << endl;
				break;
			}
			cambioContrasenia(cliente, contrasenia);
			escribirArchivoCliente();
			cout << "Se actualizó la contraseña conrrectamente." << endl << endl;;
			break;
		}
		case '7': {
			imprimirMovimientos(cliente->numeroCuenta);
			break;
		}
		}
	} while (opc != '8');
}

void PagarServicio(Cliente* cliente) {
	system("cls");
	char opc = '0';
	int num = 0;
	do
	{
		cout << endl;
		cout << "MENÚ PAGAR SERVICIO" << endl;
		cout << "1. Pagar recibo de luz" << endl;
		cout << "2. Pagar recibo de agua" << endl;
		cout << "3. Pagar recibo de internet" << endl;
		cout << "4. Pagar recibo de cable" << endl;
		cout << "5. Pagar recibo de gas" << endl;
		cout << "6. Salir" << endl;
		cout << "Eliga opción: ";
		opc = _getche();
		switch (opc)
		{
		case '1':
			cout << endl << "\tBienvenido a Edelnor" << endl;
			dineroServicio(cliente);
			break;
		case '2':
			cout << endl << "\tBienvenido a Sedapal" << endl;
			dineroServicio(cliente);
			break;
		case '3':
			cout << endl << "\tBuenvenido a Oscuro" << endl;
			dineroServicio(cliente);
			break;
		case '4':
			cout << endl << "\tBienvenido a CablePeru" << endl;
			dineroServicio(cliente);
			break;
		case '5':
			cout << endl << "\tBienvenido a LapizGas" << endl;
			dineroServicio(cliente);
			break;
		}
	} while (opc != '6');
	cout << endl << endl;
}


void guardarCliente() {
	cout << endl;

	string nro, nombre, fecha;
	float monto;
	int dia, mes, anio;
	cout << "Ingrese el numero de cuenta del nuevo cliente: ";
	cin >> nro;
	if (!validarTexto(nro, "numero de cuenta")) return;
	if (existeCliente(nro) != NULL) {
		cout << "El cliente ya existe, ingrese otro numero de cuenta." << endl;
		return;
	}
	cout << "Ingrese el nombre del cliente: ";
	cin >> nombre;
	if (!validarTexto(nombre, "nombre")) return;
	cout << "Fecha de nacimiento:" << endl;
	struct tm fechaNac;
	try {
		cout << "Ingrese el año: ";
		cin >> anio;
		fechaNac.tm_year = anio - 1900;
		cout << "Ingrese el mes: ";
		cin >> mes;
		fechaNac.tm_mon = mes - 1;
		cout << "Ingrese el dia: ";
		cin >> dia;
		fechaNac.tm_mday = dia;
	}
	catch (...) {
		cout << "Ingrese una fecha adecuada." << endl;
		return;
	}
	cout << "Ingrese el monto inicial del cliente: ";
	cin >> monto;
	if (!validarDatos("Ingrese numeros en el campo monto.")) return;
	if (!validarRango(1, 100000, monto, "El monto incial debe estar entre 1 y 100000")) return;
	
	Cliente cliente = Cliente(nro, nombre, fechaNac, monto);
	cliente.setContrasenia(1234);
	listaCliente.push_back(cliente);
	escribirArchivoCliente();
	cout << "El cliente se registró correctamente." << endl << endl;
}

Cliente* existeCliente(string num) {
	int n = listaCliente.size();
	for (int i = 0; i < n; i++)
	{
		if (listaCliente[i].numeroCuenta == num) {
			return &listaCliente[i];
		}
	}
	return NULL;
}

void eliminarCliente(string nro) {
	int l = listaCliente.size();
	for (int i = 0; i < l; i++)
	{
		if (listaCliente[i].numeroCuenta == nro) {
			auto idx = listaCliente.begin() + i;
			if (idx != listaCliente.end()) {
				listaCliente.erase(idx);
			}
			else {
				cout << "No se pudo eliminar el cliente" << endl;
			}
		}
	}
}

void mostrarClientes() {
	cout << endl;
	int n = listaCliente.size();
	int bytesEscritos;
	for (int i = 0; i < n; i++)
	{
		bytesEscritos = strftime(fechaHora, sizeof fechaHora, formatoDMY, &listaCliente[i].fechaNac);
		cout << "CLIENTE " << i + 1 << endl;

		cout << "Numero de cuenta: " << listaCliente[i].numeroCuenta << endl;
		cout << "Nombres: " << listaCliente[i].nombre << endl;
		cout << "Fecha nacimiento: " << fechaHora << endl;
		cout << "Monto: " << " S/. " << listaCliente[i].monto << endl << endl;
	}
}

void actualizarDinero(int cantidad, int tipo) {
	switch (tipo)
	{
	case 200: {
		d1.cantidad200 += cantidad;
		break;
	}
	case 100: {
		d1.cantidad100 += cantidad;
		break;
	}
	case 50: {
		d1.cantidad50 += cantidad;
		break;
	}
	case 20: {
		d1.cantidad20 += cantidad;
		break;
	}
	case 10: {
		d1.cantidad10 += cantidad;
		break;
	}
	}
	if (cantidad < 0)cout << "Se retiraron " << (cantidad * -1) << " billetes de " << tipo << "." << endl;
	else cout << "Se agregaron " << cantidad << " billetes de " << tipo << "." << endl;
}

void retirarDinero(float* dinero, float suma) {
	*dinero -= suma;
}

void depositarDinero(float* dinero, float suma) {
	*dinero += suma;
}

void transferir(float* dineroCliente1, float* dineroCliente2, float suma) {
	*dineroCliente1 -= suma;
	*dineroCliente2 += suma;
}

void pagoServicio(float* dinero, float suma) {
	*dinero -= suma;
}

void imprimirMovimientos(string nro) {
	cout << endl;
	int l = listaMovimientos.size();
	for (int i = 0; i < l; i++)
	{
		if (listaMovimientos[i].numCuenta == nro) {
			cout << listaMovimientos[i].tipo << " el dia " <<
				listaMovimientos[i].fechaM << " de monto: S/. " << listaMovimientos[i].monto << endl;
		}
	}
}

void cambioContrasenia(Cliente* cliente, int nuevaC) {
	cliente->setContrasenia(nuevaC);
}


void loginCliente() {
	string numCuenta;
	int contra = 0;
	do {
		cout << endl << "X. Volver al menu de administradores y clientes." << endl;
		cout << "Bienvenido, ingrese su numero de cuenta: ";
		cin >> numCuenta;
		if (numCuenta == "X") {
			break;
		}
		if (existeCliente(numCuenta) == NULL) {
			cout << "\nEl numero de cuenta no existe, ingrese otro." << endl << endl;
			continue;
		}
		cout << "Ingrese su contraseña o PIN: ";
		cin >> contra;
		if (existeCliente(numCuenta)->getContrasenia() != contra) {
			cout << "\nLa contraseña es incorrecta, ingrese otra." << endl << endl;
			continue;
		}
		menuCliente(existeCliente(numCuenta));
	} while (numCuenta != "X");
}

float leerMonto(string msg) {
	float dinero;
	cout << msg;
	cin >> dinero;
	if (!validarDatos("Dato erróneo, ingrese números.")) return 0;
	return dinero;
}

bool GiveMoney(float monto) {

	int billetes[] = { 200,100,50,20,10 };
	int cant[] = { 0,0,0,0,0 };
	int tam = sizeof(billetes) / sizeof(*billetes);
	int i = 0;
	int c = 0;
	float m = monto;
	while (i < tam && m >0) {
		c = 0;
		while (m >= billetes[i]) {
			c = c + 1;
			m = m - billetes[i];
			cant[i]++;
		}
		i = i + 1;
	}
	if (d1.cantidad200 < cant[0] || d1.cantidad100 < cant[1] || d1.cantidad50 < cant[2]
		|| d1.cantidad20 < cant[3] || d1.cantidad10 < cant[4]) {
		cout << "Billetes insuficietes" << endl;
		return false;
	}
	cout << endl;
	d1.cantidad200 -= cant[0];
	d1.cantidad100 -= cant[1];
	d1.cantidad50 -= cant[2];
	d1.cantidad20 -= cant[3];
	d1.cantidad10 -= cant[4];
	cant[0] = cant[1] = cant[2] = cant[3] = cant[4] = 0;
	i = 0;
	c = 0;
	m = monto;
	while (i < tam && m >0) {
		c = 0;
		while (m >= billetes[i]) {
			c = c + 1;
			cout << "Se retiro: " << " S/" << billetes[i] << endl;
			m = m - billetes[i];
		}
		i = i + 1;
	}
	return true;
}

void getMoney(float monto) {
	int billetes[] = { 200,100,50,20,10 };
	int cant[] = { 0,0,0,0,0 };
	int tam = sizeof(billetes) / sizeof(*billetes);
	int i = 0;
	int c = 0;
	float m = monto;
	while (i < tam && m >0) {
		c = 0;
		while (m >= billetes[i]) {
			c = c + 1;
			m = m - billetes[i];
			cant[i]++;
		}
		i = i + 1;
	}
	cout << endl;
	d1.cantidad200 += cant[0];
	d1.cantidad100 += cant[1];
	d1.cantidad50 += cant[2];
	d1.cantidad20 += cant[3];
	d1.cantidad10 += cant[4];
	cant[0] = cant[1] = cant[2] = cant[3] = cant[4] = 0;
	i = 0;
	c = 0;
	m = monto;
	while (i < tam && m >0) {
		c = 0;
		while (m >= billetes[i]) {
			c = c + 1;
			cout << "Se deposito: " << " S/" << billetes[i] << endl;
			m = m - billetes[i];
		}
		i = i + 1;
	}
}

int dineroServicio(Cliente* cliente){
	float pago = leerMonto("Ingrese el monto de su recibo: ");
	if (!(validarRango(0, cliente->monto, pago, "El pago del servicio debe ser menor o igual al monto.")))return 0;
	pagoServicio(&(cliente->monto), pago);
	registrarMovimientos(*(cliente), "Pago de servicio", pago);
	cout << "Su pago de S/. " << pago << " se realizó con éxito." << endl << endl;
	return 0;
}