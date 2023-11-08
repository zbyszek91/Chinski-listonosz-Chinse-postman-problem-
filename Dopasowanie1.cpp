#include "Dopasowanie1.h"

Dopasowanie::Dopasowanie(const Graf & G):
	G(G),
	zewnetrzny(2*G.WezNumWierzcholka()),
	gleboki(2*G.WezNumWierzcholka()),
	platek(2*G.WezNumWierzcholka()),
	glowa(2*G.WezNumWierzcholka()),
	aktywny(2*G.WezNumWierzcholka()),
	zaden(2*G.WezNumWierzcholka()),
	las(2*G.WezNumWierzcholka()),
	root(2*G.WezNumWierzcholka()),
	zablokowany(2*G.WezNumWierzcholka()),
	podwojny(2*G.WezNumWierzcholka()),
	slack(G.GetNumEdges()),
	wiazanie(2*G.WezNumWierzcholka()),
	m(G.GetNumEdges()),
	n(G.WezNumWierzcholka()),
	odwiedzone(2*G.WezNumWierzcholka())
{
}

void Dopasowanie::Rosnij()
{
	Reset();

	//Wszystkie niedopasowane wierzcholki beda korzeniami w lesie, ktojciecry zostanie wyhodowany
//Las rosnie poprzez przedluzenie niedopasowanego wierzcholka w przez dopasowana krawedz u-v w spozycjaojciecb BFS
	while(!lasLista.empty())
	{
		int w = zewnetrzny[lasLista.front()];
		lasLista.pop_front();

		//w moze byc kwiatem
//musimy zbadac wszystkie polaczenia wierzcholkojciecw wewnatrz kwiatu z innymi wierzcholkami
		for(list<int>::iterator it = gleboki[w].begin(); it != gleboki[w].end(); it++)
		{
			int u = *it;

			int cont = false;
			for(list<int>::const_iterator jt = G.AdjList(u).begin(); jt != G.AdjList(u).end(); jt++)
			{
				int v = *jt;

				if(JestZablokowany(u, v)) continue;

				//u jest parzyste, a v jest nieparzyste
				if(zaden[zewnetrzny[v]] == ODD) continue;

				//jesli v nie jest oznaczone
				if(zaden[zewnetrzny[v]] != EVEN)
				{
					//Uprawiamy las naprzemienny
					int vm = wiazanie[zewnetrzny[v]];

					las[zewnetrzny[v]] = u;
					zaden[zewnetrzny[v]] = ODD;
					root[zewnetrzny[v]] = root[zewnetrzny[u]];
					las[zewnetrzny[vm]] = v;
					zaden[zewnetrzny[vm]] = EVEN;
					root[zewnetrzny[vm]] = root[zewnetrzny[u]];

					if(!odwiedzone[zewnetrzny[vm]])
					{
						lasLista.push_back(vm);
						odwiedzone[zewnetrzny[vm]] = true;
					}
				}
				//Jesli v jest parzyste, a u i v znajduja sie na rojciecznych drzewach
//znalezlismy sciezke rozszerzajaca
				else if(root[zewnetrzny[v]] != root[zewnetrzny[u]])
				{
					Augment(u,v);
					Reset();

					cont = true;
					break;
				}
				//Jesli u i v sa parzyste i naleza do tego samego drzewa
//znalezlismy kwiat
				else if(zewnetrzny[u] != zewnetrzny[v])
				{
					int b = Kwiatstworz(u,v);

					lasLista.push_front(b);
					odwiedzone[b] = true;

					cont = true;
					break;
				}
			}
			if(cont) break;
		}
	}

	//Sprawdz, czy dopasowanie jest idealne
	perfect = true;
	for(int i = 0; i < n; i++)
		if(wiazanie[zewnetrzny[i]] == -1)
			perfect = false;
}

bool Dopasowanie::JestPrzylegly(int u, int v)
{
	return (G.AdjMat()[u][v] and not JestZablokowany(u, v));
}

bool Dopasowanie::JestZablokowany(int u, int v)
{
	return GREATER(slack[ G.IndexKrawedzi(u, v) ], 0);
}

bool Dopasowanie::JestZablokowany(int e)
{
	return GREATER(slack[e], 0);
}

//Wierzcholki zostana wybrane w kolejnosci niemalejacej ich stopnia
//Za kazdym razem, gdy wybrany jest niedopasowany wierzcholek, jest on dopasowywany do sasiedniego niedopasowanego wierzcholka o minimalnym stopniu
void Dopasowanie::Heuristic()
{
	vector<int> degree(n, 0);
	BinarySterta B;

	for(int i = 0; i < m; i++)
	{
		if(JestZablokowany(i)) continue;

		pair<int, int> p = G.WezKrawedz(i);
		int u = p.first;
		int v = p.second;

		degree[u]++;
		degree[v]++;
	}

	for(int i = 0; i < n; i++)
		B.Wstaw(degree[i], i);

	while(B.Size() > 0)
	{
		int u = B.UsunMin();
		if(wiazanie[zewnetrzny[u]] == -1)
		{
			int min = -1;
			for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
			{
				int v = *it;

				if(JestZablokowany(u, v) or
					(zewnetrzny[u] == zewnetrzny[v]) or
					(wiazanie[zewnetrzny[v]] != -1) )
					continue;

				if(min == -1 or degree[v] < degree[min])
					min = v;
			}
			if(min != -1)
			{
				wiazanie[zewnetrzny[u]] = min;
				wiazanie[zewnetrzny[min]] = u;
			}
		}
	}
}

//Rekurencyjnie niszczy kwiat
void Dopasowanie::Zniszczkwiaty(int t)
{
	if((t < n) or
		(zablokowany[t] and GREATER(podwojny[t], 0))) return;

	for(list<int>::iterator it = platek[t].begin(); it != platek[t].end(); it++)
	{
		int s = *it;
		zewnetrzny[s] = s;
		for(list<int>::iterator jt = gleboki[s].begin(); jt != gleboki[s].end(); jt++)
			zewnetrzny[*jt] = s;

		Zniszczkwiaty(s);
	}

	aktywny[t] = false;
	zablokowany[t] = false;
	DodajWolnyKwiatStworzIndex(t);
	wiazanie[t] = -1;
}

void Dopasowanie::Rozszerzony(int u, bool rozszerzZablokowane = false)
{
	int v = zewnetrzny[wiazanie[u]];

	int index = m;
	int p = -1, q = -1;
	//Znajdz regularna krawedz {p,q} minimalnego indeksu laczacego u i jego towarzysza
//Uzywamy minimalnego indeksu, aby przyznac, ze dwa mozliwe kwiaty u i v beda uzywac tej samej krawedzi dla wiazania
	for(list<int>::iterator it = gleboki[u].begin(); it != gleboki[u].end(); it++)
	{
		int di = *it;
		for(list<int>::iterator jt = gleboki[v].begin(); jt != gleboki[v].end(); jt++)
		{
			int dj = *jt;
			if(JestPrzylegly(di, dj) and G.IndexKrawedzi(di, dj) < index)
			{
				index = G.IndexKrawedzi(di, dj);
				p = di;
				q = dj;
			}
		}
	}

	wiazanie[u] = q;
    wiazanie[v] = p;
	//Jesli u jest wierzcholkiem regularnym, to koniec
	if(u < n or (zablokowany[u] and not rozszerzZablokowane)) return;

	bool found = false;
	//Znajdz polozenie t nowego wierzcholka kwiatu
	for(list<int>::iterator it = platek[u].begin(); it != platek[u].end() and not found; )
	{
		int si = *it;
		for(list<int>::iterator jt = gleboki[si].begin(); jt != gleboki[si].end() and not found; jt++)
		{
			if(*jt == p )
				found = true;
		}
		it++;
		if(not found)
		{
			platek[u].push_back(si);
			platek[u].pop_front();
		}
	}

	list<int>::iterator it = platek[u].begin();
	//Dostosuj dopasowanie koñcojciecwki
	wiazanie[*it] = wiazanie[u];
	it++;
	//Teraz przechodzimy przez dziwny obwojciecd dopasowujac nowych polaczen
	while(it != platek[u].end())
	{
		list<int>::iterator itnext = it;
		itnext++;
		wiazanie[*it] = *itnext;
		wiazanie[*itnext] = *it;
		itnext++;
		it = itnext;
	}

	//Aktualizujemy ustawienia kwiatow, platkow
	for(list<int>::iterator it = platek[u].begin(); it != platek[u].end(); it++)
	{
		int s = *it;
		zewnetrzny[s] = s;
		for(list<int>::iterator jt = gleboki[s].begin(); jt != gleboki[s].end(); jt++)
			zewnetrzny[*jt] = s;
	}
	aktywny[u] = false;
	DodajWolnyKwiatStworzIndex(u);

	//Rozwiñ wierzcholki kwiatu
	for(list<int>::iterator it = platek[u].begin(); it != platek[u].end(); it++)
		Rozszerzony(*it, rozszerzZablokowane);

}

//Rozszerz katalog glojciecwny sciezki[u], ..., u, v, ..., root[v]
void Dopasowanie::Augment(int u, int v)
{
//We go from u and v to its respective roots, alternating the matching
	int p = zewnetrzny[u];
	int q = zewnetrzny[v];
    int outv = q;
	int fp = las[p];
	wiazanie[p] = q;
	wiazanie[q] = p;
	Rozszerzony(p);
	Rozszerzony(q);
	while(fp != -1)
	{
		q = zewnetrzny[las[p]];
		p = zewnetrzny[las[q]];
		fp = las[p];

		wiazanie[p] = q;
		wiazanie[q] = p;
		Rozszerzony(p);
		Rozszerzony(q);
	}

	p = outv;
	fp = las[p];
	while(fp != -1)
	{
		q = zewnetrzny[las[p]];
		p = zewnetrzny[las[q]];
		fp = las[p];

		wiazanie[p] = q;
		wiazanie[q] = p;
		Rozszerzony(p);
		Rozszerzony(q);
	}
}

void Dopasowanie::Reset()
{
	for(int i = 0; i < 2*n; i++)
	{
		las[i] = -1;
		root[i] = i;

		if(i >= n and aktywny[i] and zewnetrzny[i] == i)
			Zniszczkwiaty(i);
	}

	odwiedzone.assign(2*n, 0);
	lasLista.clear();
	for(int i = 0; i < n; i++)
	{
		if(wiazanie[zewnetrzny[i]] == -1)
		{
			zaden[zewnetrzny[i]] = 2;
			if(!odwiedzone[zewnetrzny[i]])
				lasLista.push_back(i);
			odwiedzone[zewnetrzny[i]] = true;
		}
		else zaden[zewnetrzny[i]] = 0;
	}
}

int Dopasowanie::WezWolnyKwiatStworzIndex()
{
	int i = free.back();
	free.pop_back();
	return i;
}

void Dopasowanie::DodajWolnyKwiatStworzIndex(int i)
{
	free.push_back(i);
}

void Dopasowanie::CzyscKwiatStworzIndex()
{
	free.clear();
	for(int i = n; i < 2*n; i++)
		DodajWolnyKwiatStworzIndex(i);
}

//kontakty  blossom w, ..., u, v, ..., w, gdzie w ijest pierwszym wierzcholkiem ktory pojawia sie w sciezce od u i v od ich wspolrzednych respective roots
int Dopasowanie::Kwiatstworz(int u, int v)
{
	int t = WezWolnyKwiatStworzIndex();

	vector<bool> isInPath(2*n, false);

	//Znajdz czubek kwiatu
	int u_ = u;
	while(u_ != -1)
	{
		isInPath[zewnetrzny[u_]] = true;

		u_ = las[zewnetrzny[u_]];
	}

	int v_ = zewnetrzny[v];
	while(not isInPath[v_])
		v_ = zewnetrzny[las[v_]];
	glowa[t] = v_;

	//Znajdz nieparzysta scieszke , zaktualizuj platki, zewnetrzny, kwitnacy i gleboki
//Najpierw konstruujemy zestaw platki (nieparzysty sciezka)
	list<int> circuit;
	u_ = zewnetrzny[u];
	circuit.push_front(u_);
	while(u_ != glowa[t])
	{
		u_ = zewnetrzny[las[u_]];
		circuit.push_front(u_);
	}

	platek[t].clear();
	gleboki[t].clear();
	for(list<int>::iterator it = circuit.begin(); it != circuit.end(); it++)
	{
		platek[t].push_back(*it);
	}

	v_ = zewnetrzny[v];
	while(v_ != glowa[t])
	{
		platek[t].push_back(v_);
		v_ = zewnetrzny[las[v_]];
	}

	//Teraz konstruujemy glebokie i aktualizujemy zewnetrzne
	for(list<int>::iterator it = platek[t].begin(); it != platek[t].end(); it++)
	{
		u_ = *it;
		zewnetrzny[u_] = t;
		for(list<int>::iterator jt = gleboki[u_].begin(); jt != gleboki[u_].end(); jt++)
		{
			gleboki[t].push_back(*jt);
			zewnetrzny[*jt] = t;
		}
	}

	las[t] = las[glowa[t]];
	zaden[t] = EVEN;
	root[t] = root[glowa[t]];
	aktywny[t] = true;
	zewnetrzny[t] = t;
	wiazanie[t] = wiazanie[glowa[t]];

	return t;
}

void Dopasowanie::OdswierzPodwojonyKoszt()
{
	double e1 = 0, e2 = 0, e3 = 0;
	int inite1 = false, inite2 = false, inite3 = false;
	for(int i = 0; i < m; i++)
	{
		int u = G.WezKrawedz(i).first,
			v = G.WezKrawedz(i).second;

		if( (zaden[zewnetrzny[u]] == EVEN and zaden[zewnetrzny[v]] == UNLABELED) or (zaden[zewnetrzny[v]] == EVEN and zaden[zewnetrzny[u]] == UNLABELED) )
		{
			if(!inite1 or GREATER(e1, slack[i]))
			{
				e1 = slack[i];
				inite1 = true;
			}
		}
		else if( (zewnetrzny[u] != zewnetrzny[v]) and zaden[zewnetrzny[u]] == EVEN and zaden[zewnetrzny[v]] == EVEN )
		{
			if(!inite2 or GREATER(e2, slack[i]))
			{
				e2 = slack[i];
				inite2 = true;
			}
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(aktywny[i] and i == zewnetrzny[i] and zaden[zewnetrzny[i]] == ODD and (!inite3 or GREATER(e3, podwojny[i])))
		{
			e3 = podwojny[i];
			inite3 = true;
		}
	}
	double e = 0;
	if(inite1) e = e1;
	else if(inite2) e = e2;
	else if(inite3) e = e3;

	if(GREATER(e, e2/2.0) and inite2)
		e = e2/2.0;
	if(GREATER(e, e3) and inite3)
		e = e3;

	for(int i = 0; i < 2*n; i++)
	{
		if(i != zewnetrzny[i]) continue;

		if(aktywny[i] and zaden[zewnetrzny[i]] == EVEN)
		{
			podwojny[i] += e;
		}
		else if(aktywny[i] and zaden[zewnetrzny[i]] == ODD)
		{
			podwojny[i] -= e;
		}
	}

	for(int i = 0; i < m; i++)
	{
		int u = G.WezKrawedz(i).first,
			v = G.WezKrawedz(i).second;

		if(zewnetrzny[u] != zewnetrzny[v])
		{
			if(zaden[zewnetrzny[u]] == EVEN and zaden[zewnetrzny[v]] == EVEN)
				slack[i] -= 2.0*e;
			else if(zaden[zewnetrzny[u]] == ODD and zaden[zewnetrzny[v]] == ODD)
				slack[i] += 2.0*e;
			else if( (zaden[zewnetrzny[v]] == UNLABELED and zaden[zewnetrzny[u]] == EVEN) or (zaden[zewnetrzny[u]] == UNLABELED and zaden[zewnetrzny[v]] == EVEN) )
				slack[i] -= e;
			else if( (zaden[zewnetrzny[v]] == UNLABELED and zaden[zewnetrzny[u]] == ODD) or (zaden[zewnetrzny[u]] == UNLABELED and zaden[zewnetrzny[v]] == ODD) )
				slack[i] += e;
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(GREATER(podwojny[i], 0))
		{
			zablokowany[i] = true;
		}
		else if(aktywny[i] and zablokowany[i])
		{
			//Kwiat zostaje odblokowany
			if(wiazanie[i] == -1)
			{
				Zniszczkwiaty(i);
			}
			else
			{
				zablokowany[i] = false;
				Rozszerzony(i);
			}
		}
	}
}

pair< list<int>, double> Dopasowanie::RozwiazMinimalnyKosztPerfekcyjneDopasowanie(const vector<double> & cost)
{
	ObliczMaksymalneDopasowanie();
	if(!perfect)
		throw "Blad: Graf nie ma idealnego dopasowania";

	Clear();

	//Zainicjuj luzy (zmniejszone koszty krawedzi)
	slack = cost;

	DodadniKoszt();

	//Jesli dopasowanie skompresowanego Grafa jest idealne, to koniec
	perfect = false;
	while(not perfect)
	{
		//Uruchom algorytm maksymalnego dopasowania
		Heuristic();
		//Urosnij lesie karpacki
		Rosnij();
		OdswierzPodwojonyKoszt();
		//Ustaw algorytm dla nowego etapu wzrostu
		Reset();
	}

	list<int> dopasowanie = RetrieveDopasowanie();

	double obj = 0;
	for(list<int>::iterator it = dopasowanie.begin(); it != dopasowanie.end(); it++)
		obj += cost[*it];

	double dualObj = 0;
	for(int i = 0; i < 2*n; i++)
	{
		if(i < n) dualObj += podwojny[i];
		else if(zablokowany[i]) dualObj += podwojny[i];
	}

	return pair< list<int>, double >(dopasowanie, obj);
}

void Dopasowanie::DodadniKoszt()
{
	double minEdge = 0;
	for(int i = 0; i < m ;i++)
		if(GREATER(minEdge - slack[i], 0))
			minEdge = slack[i];

	for(int i = 0; i < m; i++)
		slack[i] -= minEdge;
}

list<int> Dopasowanie::ObliczMaksymalneDopasowanie()
{
	Clear();
	Rosnij();
	return RetrieveDopasowanie();
}

//Konfiguruje algorytm dla nowego uruchomienia
void Dopasowanie::Clear()
{
	CzyscKwiatStworzIndex();

	for(int i = 0; i < 2*n; i++)
	{
		zewnetrzny[i] = i;
		gleboki[i].clear();
		if(i<n)
			gleboki[i].push_back(i);
		platek[i].clear();
		if(i < n) aktywny[i] = true;
		else aktywny[i] = false;

		zaden[i] = 0;
		las[i] = -1;
		root[i] = i;

		zablokowany[i] = false;
		podwojny[i] = 0;
		wiazanie[i] = -1;
		glowa[i] = i;
	}
	slack.assign(m, 0);
}

list<int> Dopasowanie::RetrieveDopasowanie()
{
	list<int> dopasowanie;

	for(int i = 0; i < 2*n; i++)
		if(aktywny[i] and wiazanie[i] != -1 and zewnetrzny[i] == i)
			Rozszerzony(i, true);

	for(int i = 0; i < m; i++)
	{
		int u = G.WezKrawedz(i).first;
		int v = G.WezKrawedz(i).second;

		if(wiazanie[u] == v)
			dopasowanie.push_back(i);
	}
	return dopasowanie;
}
