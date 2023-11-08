#include "Dopasowanie1.h"
#include <fstream>
#include "Graf1.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

pair< Graf, vector<double> > CreateRandomGraph()
{
	//slepa kura
	int x;
	cin >> x;
	srand( x );

	//Prosze zobaczyc Graph.h, aby zapoznac sie z opisem interfejsu
	int n = 50;

	Graf G(n);
	vector<double> koszt;
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(rand()%10 == 0)
			{
				G.DodajKrawedz(i, j);
				koszt.push_back(rand()%1000);
			}

	return make_pair(G, koszt);
}

Graf CzytajGrafu(string filename)
{
	//Prosze zobaczyc Graph.h, aby zapoznac sie z opisem interfejsu

	ifstream file;
	file.open(filename.c_str());

	string s;
	getline(file, s);
	stringstream ss(s);
	int n;
	ss >> n;
	getline(file, s);
	ss.str(s);
	ss.clear();
	int m;
	ss >> m;

	Graf G(n);
	for(int i = 0; i < m; i++)
	{
		getline(file, s);
		ss.str(s);
		ss.clear();
		int u, v;
		ss >> u >> v;

		G.DodajKrawedz(u, v);
	}

	file.close();
	return G;
}

pair< Graf, vector<double> > CzytajWageGrafu(string filename)
{
	//Prosze zobaczyc Graph.h, aby zapoznac sie z opisem interfejsu

	ifstream file;
	file.open(filename.c_str());

	string s;
	getline(file, s);
	stringstream ss(s);
	int n;
	ss >> n;
	getline(file, s);
	ss.str(s);
	ss.clear();
	int m;
	ss >> m;

	Graf G(n);
	vector<double> koszt(m);
	for(int i = 0; i < m; i++)
	{
		getline(file, s);
		ss.str(s);
		ss.clear();
		int u, v;
		double c;
		ss >> u >> v >> c;

		G.DodajKrawedz(u, v);
		koszt[G.IndexKrawedzi(u, v)] = c;
	}

	file.close();
	return make_pair(G, koszt);
}

void MinimalnyKosztPerfekcyjnyDopasowaniePrzyklad(string filename)
{
	Graf G;
	vector<double> koszt;

	//czytaj graf
	pair< Graf, vector<double> > p = CzytajWageGrafu(filename);
	//pair< Graph, vector<double> > p = CreateRandomGraph();
	G = p.first;
	koszt = p.second;

	//Utwojciecrz instancje dopasowania przekazującą wykres
	Dopasowanie M(G);

	//Przekaz koszty, aby rozwiązac problem
	pair< list<int>, double > solution = M.RozwiazMinimalnyKosztPerfekcyjneDopasowanie(koszt);

	list<int> dopasowanie = solution.first;
	double obj = solution.second;

	cout << "Optymalna liczba krawedzi w dopasowaniu: " << obj << endl;
	cout << "Liczba krawedzi w dopasowaniu:" << endl;
	for(list<int>::iterator it = dopasowanie.begin(); it != dopasowanie.end(); it++)
	{
		pair<int, int> e = G.WezKrawedz( *it );

		cout <<  e.first << " " <<  e.second << endl;
	}
}

void MaksymalneDopasowaniePrzyklad(string filename)
{
	Graf G = CzytajGrafu(filename);
	Dopasowanie M(G);

	list<int> dopasowanie;
	dopasowanie = M.ObliczMaksymalneDopasowanie();

	cout << "Liczba krawedzi w maksymalnym dopasowaniu: " << dopasowanie.size() << endl;
	cout << "Krawedzie w dopasowaniu:" << endl;
	for(list<int>::iterator it = dopasowanie.begin(); it != dopasowanie.end(); it++)
	{
		pair<int, int> e = G.WezKrawedz( *it );

		cout << " "<<e.first << " " << " "<< e.second << endl;
	}
}

int main(int argc, char* argv[])
{
	string filename = "";
	string algorithm = "";

	int i = 1;
	while(i < argc)
	{
		string a(argv[i]);
		if(a == "-f")
			filename = argv[++i];
		else if(a == "--minwaga")
			algorithm = "minwaga";
		else if(a == "--maxwaga")
			algorithm = "maxwaga";
		i++;
	}

	if(filename == "" || algorithm == "")
	{
		cout << "uzycie: ./example -f <filename> <--minwaga | --maxwaga>" << endl;
		cout << "--minwaga dla idealnego dopasowania minimalnej wagi" << endl;
		cout << "--maxwaga dla maksymalnego dopasowania kardynalnosci" << endl;
		cout << "plik format:" << endl;
		cout << "pierwsze dwie linie dają n (liczba wierzcholkojciecw) i m (liczba krawedzi)," << endl;
		cout << "po ktojciecrych nastepuje m linii, kazda z krotką (u, v [, c]) reprezentującą krawedzie," << endl;
	   	cout << "gdzie u i v to punkty końcowe (indeksowanie oparte na 0) krawedzi, a c to jej koszt"<< endl;
		cout << "koszt jest opcjonalny, jesli okreslono --max"<< endl;
		return 1;
	}

	try
	{
		if(algorithm == "minwaga")
			MinimalnyKosztPerfekcyjnyDopasowaniePrzyklad(filename);
		else
			MaksymalneDopasowaniePrzyklad(filename);
	}
	catch(const char * msg)
	{
		cout << msg << endl;
		return 1;
	}

	return 0;
}



