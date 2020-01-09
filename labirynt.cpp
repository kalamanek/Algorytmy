#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;


#define WYSOKOSC (size_t)30
#define SZEROKOSC (size_t)40
#define MAX SZEROKOSC*WYSOKOSC
#define WYSOKOSCLABIRYNTU WYSOKOSC*2+1
#define SZEROKOSCLABIRYNTU SZEROKOSC*2+1


class wierzcholki{
	

	public:

	vector<unsigned int> rodzic;
	vector<unsigned int> rank;

	wierzcholki()
	{
		rodzic.resize(MAX);
		rank.resize(MAX);
		for (size_t i = 0; i < MAX; i++) {
			rodzic[i] = i;
			rank[i] = 0;
		}
	}
	
	unsigned int znajdz(unsigned int x)
	{
		if (rodzic[x] != x)
			rodzic[x] = znajdz(rodzic[x]);
	
		
		return rodzic[x];
		/*
		while (x != rodzic[x])
			x = rodzic[x];
		return x;
		*/
	}
	void wypiszSciezkeDoRodzica(unsigned x)
	{
		while (x != rodzic[x])
		{
			cout << x << " ";
			x = rodzic[x];
		}
		cout << x << " ";
		cout << endl;
	}

	bool polaczJezeliRozlaczne(unsigned int x, unsigned int y)
	{
		unsigned int a = znajdz(x);
		unsigned int b = znajdz(y);
		
		if (a == b)
			return false;
		else
		{
			rodzic[a] = b;
			//rodzic[a] = y;
			return true;
		}

	}
	void wypisz()
	{
		for (int i = 0; i < MAX; i++)
			cout << i<<" "<<rodzic[i]<<endl;
	}	
};

void przygotujLabirynt(vector<vector<char>> &v)
{
	for (int i = 0; i < WYSOKOSCLABIRYNTU  ; i++)
	{

		for (int j = 0; j < SZEROKOSCLABIRYNTU; j++)
		{
			if(i%2 == 1 && j%2 == 1)
				v[i][j] = ' ';
			else
				v[i][j] = 'X';

		}
	}
	
	v[1][0] = ' ';
	v[WYSOKOSCLABIRYNTU-2][SZEROKOSCLABIRYNTU-1] = ' ';
	

}
void wypiszLabirynt(const vector<vector<char>>& v)
{
	for (auto linia : v)
	{
		for (auto e : linia)
			cout << ((e == ' ') ? "  " : "()");
		cout << endl;
	}
}
pair<unsigned int, unsigned int> pozycja(unsigned a)
{
	return make_pair(((a / SZEROKOSC) * 2) + 1 
		, ((a % SZEROKOSC) * 2) + 1) ;
}


void usunSciane(unsigned first, unsigned second, vector<vector<char>>& labirynt)
{
	pair<unsigned, unsigned> wsp = pozycja(first);
	
	(second - first) == 1 ? // czy to sciezka w prawo jak nie to musi byc w dol (tak byly tworzone sciezki) 
		labirynt[wsp.first ][wsp.second + 1] = ' ' :
		labirynt[wsp.first + 1][wsp.second ] = ' ';
	
}

int main()
{
	wierzcholki* lista = new wierzcholki();
	
	vector<pair<unsigned int, unsigned int>> sciezki;

	vector<vector<char>> labirynt;
	labirynt.resize(WYSOKOSCLABIRYNTU, vector<char>(SZEROKOSCLABIRYNTU));
	przygotujLabirynt(labirynt);
	//wypiszLabirynt(labirynt);
	
	// dodanie mozliwych sciezek w labiryncie
	for (unsigned int i = 0; i < MAX; i++) {
		if (i % SZEROKOSC != SZEROKOSC - 1)
			sciezki.push_back(make_pair(i, i + 1));
		
		if(i/SZEROKOSC != WYSOKOSC - 1 )
			sciezki.push_back(make_pair(i, i + SZEROKOSC));
	}

	
	shuffle(begin(sciezki), end(sciezki), 
			default_random_engine(
				chrono::system_clock::now().time_since_epoch().count()
			)
		);
	
	/*for (auto a : sciezki)
		cout << a.first << " " << a.second << endl;
	*/

	//tworzenie spojnego grafu
	unsigned iteracje = 0;
	for(int i = MAX ; i > 1 ;)
	{
		pair<unsigned int, unsigned int> a = sciezki.back();
		sciezki.pop_back();
		if (lista->polaczJezeliRozlaczne(a.first, a.second)) {
			i--;
			usunSciane(a.first, a.second, labirynt);
		}
		iteracje++;
	}

	//lista->wypisz();
	wypiszLabirynt(labirynt);
	lista->wypiszSciezkeDoRodzica(0);
	lista->wypiszSciezkeDoRodzica(SZEROKOSC*WYSOKOSC-1);
	cout << "udalo sie po " << iteracje << " iteracjach";
	
	delete lista;
	
	return 0;
}
