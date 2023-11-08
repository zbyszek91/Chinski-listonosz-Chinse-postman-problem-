#pragma once

#include <list>
#include <vector>
using namespace std;

class Graf
{
public:
	//n to liczba wierzcholkojciecw
//krawedzie to lista par reprezentujacych krawedzie (domyslnie = pusta lista)
	Graf(int n, const list< pair<int, int> > & edges = list< pair<int, int> >());

	//Domyslny konstruktor tworzy pusty wykres
	Graf(): n(0), m(0) {};

	//Zwraca liczbe wierzcholkojciecw
	int WezNumWierzcholka() const { return n; };
	//Zwraca liczbe krawedzi
	int GetNumEdges() const { return m; };

	// Biorac pod uwage indeks krawedzi, zwraca jej punkty koñcowe jako pare
	pair<int, int> WezKrawedz(int e) const;
	// Biorac pod uwage punkty koñcowe, zwraca indeks
	int IndexKrawedzi(int u, int v) const;

	//Dodaje nowy wierzcholek do wykresu
	void AddVertex();
	//Dodaje nowa krawedz do wykresu
	void DodajKrawedz(int u, int v);

	//Zwraca liste sasiedztwa wierzcholka
	const list<int> & AdjList(int v) const;

	//Zwraca macierz sasiedztwa wykresu
	const vector< vector<bool> > & AdjMat() const;
private:
	//liczba wierzcholkow
	int n;
	//liczba krawedzi
	int m;

	//Macierz sasiedztwa
	vector< vector<bool> > adjMat;

	//Listy sasiedztwa
	vector< list<int> > adjList;

	//Tablica krawedzi
	vector< pair<int, int> > edges;

	//Indeksy krawedzi
	vector< vector<int> > krawedzIndex;
};
