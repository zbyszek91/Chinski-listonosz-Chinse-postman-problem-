#include "DijkstraAlgorytm.h"
#include "Dopasowanie1.h"
#include "Graf1.h"

bool Connected(const Graf & G)
{
    vector<bool> odwiedzone(G.WezNumWierzcholka(), false);
    list<int> L;

    int n = 0;
    L.push_back(0);
    while(not L.empty())
    {
        int u = L.back();
        L.pop_back();
        if(odwiedzone[u]) continue;

        odwiedzone[u] = true;
        n++;

        for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
		{
			int v = *it;
		    L.push_back(v);
		}
    }

    return G.WezNumWierzcholka() == n;
}

/*
Rozwiazuje problem chiñskiego listonosza
zwraca pare zawierajaca liste i wartosc double
lista to sekwencja wierzcholkojciecw w rozwiazaniu
podwojciecjna wartosc to koszt rozwiazania
*/
pair< list<int>, double > ChinesePostman(const Graf & G, const vector<double> & koszt)
{
	//Sprawdz, czy wykres jest podlaczony
	if(not Connected(G))
		throw "Error: Graf.h is not connected";

	//Build adjacency lists using edges in the graph
	vector< list<int> > A(G.WezNumWierzcholka(), list<int>());
	for(int u = 0; u < G.WezNumWierzcholka(); u++)
	    A[u] = G.AdjList(u);

	//Znajdz wierzcholki o stopniu nieparzystym
	vector<int> odd;
	for(int u = 0; u < G.WezNumWierzcholka(); u++)
		if(A[u].size() % 2)
			odd.push_back(u);

   //Jesli istnieja wierzcholki stopnia nieparzystego
	if(not odd.empty())
	{
		//Utwojciecrz graf z wierzcholkami stopnia nieparzystego
		Graf O(odd.size());
		for(int u = 0; u < (int)odd.size(); u++)
			for(int v = u+1; v < (int)odd.size(); v++)
				O.DodajKrawedz(u, v);

        vector<double> kosztO(O.GetNumEdges());

       //Znajdz najkrojciectsze sciezki pomiedzy wszystkimi wierzcholkami stopnia nieparzystego
		vector< vector<int> > shortestPath(O.WezNumWierzcholka());
		for(int u = 0; u < (int)odd.size(); u++)
		{
			pair< vector<int>, vector<double> > sp = Dijkstra(G, odd[u], koszt);

			shortestPath[u] = sp.first ;

			//Koszt krawedzi UV w O bedzie kosztem odpowiedniej najkrojciectszej sciezki w G
			for(int v = 0; v < (int)odd.size(); v++)
			    if(v != u)
    			    kosztO[ O.IndexKrawedzi(u, v) ] = sp.second[odd[v]];
		}

	    //Znajdz minimalne koszty idealnego dopasowania grafu o wierzcholkach stopnia nieparzystego
	    Dopasowanie M(O);
	    pair< list<int>, double > p = M.RozwiazMinimalnyKosztPerfekcyjneDopasowanie(kosztO);
	    list<int> dopasowanie = p.first;

	   //Jesli krawedz uv jest dopasowana, krawedzie najkrojciectszej sciezki od u do v powinny zostac zduplikowane w G
	    for(list<int>::iterator it = dopasowanie.begin(); it != dopasowanie.end(); it++)
	    {
		    pair<int, int> p = O.WezKrawedz(*it);
		    int u = p.first, v = odd[p.second];

		    //Przejdz sciezke dodajac krawedzie
		    int w = shortestPath[u][v];
		    while(w != -1)
		    {
		        A[w].push_back(v);
		        A[v].push_back(w);
		        v = w;
		        w = shortestPath[u][v];
		    }
	    }
	}

	//Znajdz cykl Eulera na wykresie zaproponowanym przez A
	list<int> cycle;
	//Ma to na celu sledzenie, ile razy mozemy pokonac krawedz
	vector<int> traversed(G.GetNumEdges(), 0);
	for(int u = 0; u < G.WezNumWierzcholka(); u++)
	{
		for(list<int>::iterator it = A[u].begin(); it != A[u].end(); it++)
		{
			int v = *it;

			//robimy to, aby krawedz nie byla liczona dwukrotnie
			if(v < u) continue;

			traversed[G.IndexKrawedzi(u, v)]++;
		}
	}

	cycle.push_back(0);
	list<int>::iterator itp = cycle.begin();
	double obj = 0;
	while(itp != cycle.end())
	{
		//Niech u bedzie biezacym wierzcholkiem cyklu, zaczynajac od pierwszego
		int u = *itp;
		list<int>::iterator jtp = itp;
		jtp++;

		//jesli istnieja nieprzebiegajace krawedzie, ktojciecre przechodza przez u, znajdz podcykl rozpoczynajacy sie od u
    //zamieñ u w cyklu na podcykl
		while(not A[u].empty())
		{
			while(not A[u].empty() and traversed[ G.IndexKrawedzi(u, A[u].back()) ] == 0)
				A[u].pop_back();

			if(not A[u].empty())
			{
				int v = A[u].back();
				A[u].pop_back();
				cycle.insert(jtp, v);
				traversed[G.IndexKrawedzi(u, v)]--;
		        obj += koszt[ G.IndexKrawedzi(u, v) ];
				u = v;
			}
		}

		//przejdz do nastepnego wierzcholka cyklu i zrojciecb to samo
		itp++;
	}

	return pair< list<int>, double >(cycle, obj);
}
