#pragma once

#include "Graf1.h"
#include "BinarySterta.h"
#include <list>
#include <vector>
using namespace std;

#define EVEN 2
#define ODD 1
#define UNLABELED 0

class Dopasowanie
{
public:
	//Konstruktor parametryczny odbiera instancje grafu
	Dopasowanie(const Graf & G);

	//Rozwiazuje problem doskonalego dopasowania przy minimalnym koszcie
//Otrzymuje wektor, ktojciecrego pozycja i ma koszt krawedzi o indeksie i
//Jesli wykres nie ma idealnego dopasowania, zostanie zgloszony wyjatek const char *
//Zwraca pare
//pierwszym elementem pary jest lista indeksojciecw krawedzi w dopasowaniu
//second to koszt dopasowania
	pair< list<int>, double > RozwiazMinimalnyKosztPerfekcyjneDopasowanie(const vector<double> & koszt);

	//Rozwiazuje problem maksymalnej kardynalnosci
//Zwraca liste z indeksami krawedzi w rozwiazaniu
	list<int> ObliczMaksymalneDopasowanie();

private:
	//Rosnie las naprzemienny
	void Rosnij();
	//Rozwija kwiat u
//Jesli wartosc rozszerzZablokowane ma wartosc true, kwiat zostanie rozwiniety, nawet jesli jest zablokowany
	void Rozszerzony(int u, bool rozszerzZablokowane);
	//Wzmaga potrzebe za pomoca sciezki od u do v w lesie naprzemiennym
	void Augment(int u, int v);
	//Wzmaga potrzebe za pomoca sciezki od u do v w lesie naprzemiennym
	void Reset();
	//Tworzy kwiat, ktojciecrego wierzcholek jest pierwszym wspojcieclnym wierzcholkiem na sciezkach od u i v w karpackim lesie xd
	int Kwiatstworz(int u, int v);
	void OdswierzPodwojonyKoszt();
	//Resetuje wszystkie struktury danych
	void Clear();
	void Zniszczkwiaty(int t);
	//Wykorzystuje algorytm heurystyczny do znalezienia maksymalnego rozwiazania wykresu
	void Heuristic();
	//Modyfikuje koszty grafu tak, aby wszystkie krawedzie mialy koszty dodatnie
	void DodadniKoszt();
	list<int> RetrieveDopasowanie();

	int WezWolnyKwiatStworzIndex();
	void DodajWolnyKwiatStworzIndex(int i);
	void CzyscKwiatStworzIndex();

	//Krawedz moze zostac zablokowana z powodu podwojciecjnych kosztojciecw
	bool JestZablokowany(int u, int v);
	bool JestZablokowany(int e);
	//Returns true if u and v are adjacent in G and not zablokowany
	bool JestPrzylegly(int u, int v);

	const Graf & G;

	list<int> free;//Lista bezplatnych indeksojciecw kwiatowych

	vector<int> zewnetrzny;//external[v] podaje indeks najbardziej zewnetrznego kwiatu, ktojciecry zawiera v, external[v] = v jesli v nie jest zawarte w zadnym kwiecie
	vector< list<int> > gleboki;//gleboki[v] to lista wszystkich oryginalnych wierzcholkojciecw zawartych w v, gleboki[v] = v jesli v jest pierwotnym wierzcholkiem
	vector< list<int> > platek;//platek[v] to lista wierzcholkojciecw bezpozycjarednio zawartych w v, plytkie[v] jest puste, jest wartoscia domyslna
	vector<int> glowa;//glowa[v] to wierzcholek kwiatu v
	vector<bool> aktywny;//true, jesli uzywany jest kwiat

	vector<int> zaden;//Even, odd, neither (2, 1, 0)//Parzysty, nieparzysty, zaden (2, 1, 0)
	vector<int> las;//las[v] podaje ojca v w lesie przemiennym
	vector<int> root;//root[v] podaje pierwiastek v w lesie przemiennym

	vector<bool> zablokowany;//Kwiat moze zostac zablokowany ze wzgledu na podwojciecjne koszty, co oznacza, ze zachowuje sie tak, jakby byl oryginalnym wierzcholkiem i nie mozna go rozwinac
	vector<double> podwojny;//podwojciecjne mnozniki powiazane z kwiatami, jesli podwojny[v] > 0, kwiat jest zablokowany i pelny
	vector<double> slack;//z powiazany z kazda krawedzia, jesli slack[e] > 0, nie mozna uzyc krawedzi
	vector<int> wiazanie;//wiazanie[v] daje wiazanie v

	int m, n;

	bool perfect;

	list<int> lasLista;
	vector<int> odwiedzone;
};

