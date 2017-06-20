#include <bits/stdc++.h>
using namespace std;

const int nax=100*1007;//2*limit na liczbe wierzcholkow+troche

int n;

int ojciec[2][nax];//ojciec w drzewie
vector <int> graf[2][nax];//dzieci w drzewie
int limit[2][nax];//ile przynajmniej niezwolnionych w poddrzewie
int rozmiar[2][nax];//rozmiar poddrzewa

int korzen[2];//korzen danego drzewa

vector <int> wynik;//wektor z wynikiem

int dfs(int p, int v)//p to numer drzewa, v to wierzcholek, liczy rozmiary poddrzew
{
	rozmiar[p][v]=1;
	for (int i : graf[p][v])
		rozmiar[p][v]+=dfs(p, i);//rekurencyjnie wywoluje dfs na kazdym dziecku
	return rozmiar[p][v];
}

vector < pair < int , pair <int*,int*> > > siec[nax];//dokad prowadzi krawedz, przepustowosc w jedna i w druga strone

bool odw[nax];//czy odwiedzony

void add_edge(int v, int u, int lim)//dodaje krawedz z v do u o przepustowosci lim
{
	int *a=new int(lim);
	int *b=new int(0);
	siec[v].push_back({u, {a, b}});
	siec[u].push_back({v, {b, a}});
}

void czysc()
{
	for (int i=0; i<2*(n+1); i++)
		odw[i]=false;
}

bool dfs_flow(int v)//szuka sciezek powiekszajacych
{
	if (v==2*(n+1)-1)
		return true;
	odw[v]=true;
	for (auto i : siec[v]) // pair < int , pair <int*,int*> > 
	{
		if (!odw[i.first] && *i.second.first && dfs_flow(i.first))//jesli jeszcze nieodwiedzony, mozna przejsc krawedzia i dfs zwrocil, ze da sie dojsc
		{
			(*i.second.first)--;
			(*i.second.second)++;
			return true;
		}
	}
	return false;
}

int main()
{
	scanf("%d", &n);//wczytuje wejscie
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<2; j++)
		{
			scanf("%d", &ojciec[j][i]);
			if (ojciec[j][i]==i)
				korzen[j]=i;
			else
				graf[j][ojciec[j][i]].push_back(i);
		}
		for (int j=0; j<2; j++)
			scanf("%d", &limit[j][i]);
	}
	
	for (int i=0; i<2; i++)//licze rozmiary poddrzew
		dfs(i, korzen[i]);
	
	for (int i=0; i<n; i++)//krawedzie w pierwszej warstwie
	{
		if (i==korzen[0])
			add_edge(0, 1+i, rozmiar[0][i]-limit[0][i]);
		else
			add_edge(1+ojciec[0][i], 1+i, rozmiar[0][i]-limit[0][i]);
	}
	for (int i=0; i<n; i++)//krawedzie miedzy warstwami
		add_edge(1+i, 1+n+i, 1);
	for (int i=0; i<n; i++)//krawedzie w drugiej warstwie
	{
		if (i==korzen[1])
			add_edge(1+n+i, 2*(n+1)-1, rozmiar[1][i]-limit[1][i]);
		else
			add_edge(1+n+i, 1+n+ojciec[1][i], rozmiar[1][i]-limit[1][i]);
	}
	
	while(dfs_flow(0))//szukam sciezek powiekszajacych
		czysc();
		
	for (int i=0; i<n; i++)//wypisuje wynik
		for (auto j : siec[1+i])//po wszystkich sasiadach tego wierzcholka i+1
			if (j.first==1+i+n && !(*j.second.first))
				wynik.push_back(i);
	printf("%d\n", (int)wynik.size());
	for (int i : wynik)
		printf("%d ", i);
	printf("\n");
	return 0;
}
