#include "Graf1.h"

Graf::Graf(int n, const list< pair<int, int> > & edges):
	n(n),
	m(edges.size()),
	adjMat(n, vector<bool>(n, false)),
	adjList(n),
	edges(),
	krawedzIndex(n, vector<int>(n, -1))
{
	for(list< pair<int, int> >::const_iterator it = edges.begin(); it != edges.end(); it++)
	{
		int u = (*it).first;
		int v = (*it).second;

		DodajKrawedz(u, v);
	}
}

pair<int, int> Graf::WezKrawedz(int e) const
{
	if(e > (int)edges.size())
		throw "Blad: krawedzi nie istnieje";

	return edges[e];
}

int Graf::IndexKrawedzi(int u, int v) const
{
	if( u > n or
		v > n )
		throw "Blad: wierzcholek nie istnieje";

	if(krawedzIndex[u][v] == -1)
		throw "Blad: krawedz nie istnieje";

	return krawedzIndex[u][v];
}

void Graf::AddVertex()
{
	for(int i = 0; i < n; i++)
	{
		adjMat[i].push_back(false);
		krawedzIndex[i].push_back(-1);
	}
	n++;
	adjMat.push_back( vector<bool>(n, false) );
	krawedzIndex.push_back( vector<int>(n, -1) );
	adjList.push_back( list<int>() );
}

void Graf::DodajKrawedz(int u, int v)
{
	if( u > n or
		v > n )
		throw "Error: wierzcholek nie istnieje";

	if(adjMat[u][v]) return;

	adjMat[u][v] = adjMat[v][u] = true;
	adjList[u].push_back(v);
	adjList[v].push_back(u);

	edges.push_back(pair<int, int>(u, v));
	krawedzIndex[u][v] = krawedzIndex[v][u] = m++;
}

const list<int> & Graf::AdjList(int v) const
{
	if(v > n)
		throw "Error: wierzcholek nie istnieje";

	return adjList[v];
}

const vector< vector<bool> > & Graf::AdjMat() const
{
	return adjMat;
}

