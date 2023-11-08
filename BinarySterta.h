#pragma once

#include "Globals.h"
#include <vector>
using namespace std;

/*
To jest sterta binarna dla par typu (podwojciecjny klucz, int satelita)
Zaklada sie, ze satelity sa unikalnymi liczbami calkowitymi
Dzieje sie tak w przypadku algorytmojciecw grafowych, w ktojciecrych satelity sa indeksami wierzcholkojciecw lub krawedzi
  */
class BinarySterta
{
public:
	BinarySterta(): satelita(1), size(0) {};

	//Wstawia (klawisz k, satelity s) na stercie
	void Wstaw(double k, int s);
	//Usuwa element z kluczem minimalnym i zwraca informacje o satelitach
	int UsunMin();
	//Zmienia klucz elementu za pomoca satelity
	void ZmienKlucz(double k, int s);
	//Usuwa element z satelitami
	void Usun(int s);
	//Zwraca element elementu na stercie
	int Size();
	//Resetuje strukture
	void Clear();

private:
	vector<double> klucz;//Biorac pod uwage satelite, to jest jego klucz
	vector<int> pozycja;//Biorac pod uwage satelite, oto jego pozycja na stercie
	vector<int> satelita;//This is the sterta!

	//Liczba elementojciecw na stercie
	int size;
};



