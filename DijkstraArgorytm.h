#pragma once

#include "Graf1.h"
#include "BinarySterta.h"
#include "Globals.h"
#include <limits>
using namespace std;

//Algorytm-Dijkstry wykorzystujacy sterte binarna
//Zwraca pare (vector<int>, wektor<double>)
//vector<double> podaje koszt optymalnej sciezki do kazdego wierzcholka
//vector<int> podaje rodzica kazdego wierzcholka w drzewie optymalnych sciezek
pair< vector<int>, vector<double> > Dijkstra(const Graf & G, int origin, const vector<double> & koszt)
{
	BinarySterta B;

	int n = G.WezNumWierzcholka();

	//Ojciec kazdego wierzcholka w optymalnym drzewie sciezek
	vector<int> father(n, -1);

	//Sluzy do wskazania, czy wierzcholek jest trwale oznaczony etykieta
	vector<bool> permanent(n, false);

	vector<double> pathCost(n, numeric_limits<double>::infinity());

	//Umiesc s na stercie
	B.Wstaw(0, origin);
	pathCost[origin] = 0;

	for(int i = 0; i < n; i++)
	{
		//Wybierz wierzcholek, do ktojciecrego mozna dotrzec najmniejszym kosztem
		int u = B.UsunMin();

		permanent[u] = true;

		//Aktualizuj sterte wierzcholkami sasiadujacymi z u
		for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
		{
			int v = *it;

			if(permanent[v])
				continue;

			double c = pathCost[u] + koszt[G.IndexKrawedzi(u,v)];

			//v nie zostal jeszcze odkryty
			if(father[v] == -1)
			{
				father[v] = u;
				pathCost[v] = c;
				B.Wstaw(c, v);
			}
			//znalezlismy tansze polaczenie do v
			else if( LESS(c, pathCost[v]) )
			{
				father[v] = u;
				pathCost[v] = c;
				B.ZmienKlucz(c, v);
			}
		}
	}

	if(B.Size() > 0)
		throw "Blad: Graf nie jest podlaczony";

	return make_pair(father, pathCost);
}



