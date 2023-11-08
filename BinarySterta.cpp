#include "BinarySterta.h"

void BinarySterta::Clear()
{
	klucz.clear();
	pozycja.clear();
	satelita.clear();
}

void BinarySterta::Wstaw(double k, int s)
{
	//Dostosuj struktury, aby pasowaly do nowych danych
	if(s >= (int)pozycja.size())
	{
		pozycja.resize(s+1, -1);
		klucz.resize(s+1);
		//Przypomnij sobie, ze pozycja 0 satelity jest nieuzywana
		satelita.resize(s+2);
	}
	//Jesli satelita jest juz na stercie
	else if(pozycja[s] != -1)
	{
		throw "Error: satelita already in heap";
	}

	int i;
	for(i = ++size; i/2 > 0 && GREATER(klucz[satelita[i/2]], k); i /= 2)
	{
		satelita[i] = satelita[i/2];
		pozycja[satelita[i]] = i;
	}
	satelita[i] = s;
	pozycja[s] = i;
	klucz[s] = k;
}

int BinarySterta::Size()
{
	return size;
}

int BinarySterta::UsunMin()
{
	if(size == 0)
		throw "Error: empty heap";

	int min = satelita[1];
	int slast = satelita[size--];


	int child;
	int i;
	for(i = 1, child = 2; child  <= size; i = child, child *= 2)
	{
		if(child < size && GREATER(klucz[satelita[child]], klucz[satelita[child+1]]))
			child++;

		if(GREATER(klucz[slast], klucz[satelita[child]]))
		{
			satelita[i] = satelita[child];
			pozycja[satelita[child]] = i;
		}
		else
			break;
	}
	satelita[i] = slast;
	pozycja[slast] = i;

	pozycja[min] = -1;

	return min;
}

void BinarySterta::ZmienKlucz(double k, int s)
{
	Usun(s);
	Wstaw(k, s);
}

void BinarySterta::Usun(int s)
{
	int i;
	for(i = pozycja[s]; i/2 > 0; i /= 2)
	{
		satelita[i] = satelita[i/2];
		pozycja[satelita[i]] = i;
	}
	satelita[1] = s;
	pozycja[s] = 1;

	UsunMin();
}

