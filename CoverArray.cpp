#include <iostream>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#include <time.h>
#include <numeric>
#endif
#define N_MAX 30 // using char to represent n, this can't exceed 127
#define K_MAX 15
#define V_MAX 5
#define PRICISION 10000
using namespace std;

void GenerateInit(char (*a)[K_MAX], int n, int k, int v)
{
	int i, j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<k; j++)
		{
			a[i][j] = rand()%v;
		}
	}
}

int DensityFunction(char (*a)[K_MAX], int n, int k, int v)
{
	int i, j, c, sum;
	char cover[V_MAX*V_MAX];
	sum = 0;
	
	for(i=0; i<n; i++)
	{
		for(j=i+1; j<k; j++)
		{
			memset(cover, 0, sizeof(char)*V_MAX*V_MAX);
			for(c=0; c<n; c++)
			{
				cover[a[c][i]*v + a[c][j]] = 1;
			}
			for(c=0; c<v*v; c++)
				sum += cover[c];
		}
	}
	
	return sum;
}

void Neighbour(char (*a)[K_MAX], char (*b)[K_MAX], int n, int k, int v)
{
	
	int i, j, c;
	int alpos, alcov;
	char cover[V_MAX*V_MAX];
	char temppos[V_MAX*V_MAX];
	
	vector<int> pos;
	vector<int> alcover;

	
	alpos = alcov = -1;
	
	memcpy(b, a, sizeof(char)*K_MAX*N_MAX);
	for(i=0; i<n; i++)
	{
		for(j=i+1; j<k; j++)
		{
			pos.clear();
			alcover.clear();
			memset(cover, 0, sizeof(char)*V_MAX*V_MAX);
			memset(temppos, -1, sizeof(char)*V_MAX*V_MAX);
			for(c=0; c<n; c++)
			{
				if(cover[a[c][i]*v + a[c][j]])
				{
					pos.push_back(c);
					if(temppos[a[c][i]*v + a[c][j]]>=0)
					{
						pos.push_back(temppos[a[c][i]*v + a[c][j]]);
						temppos[a[c][i]*v + a[c][j]] = -2;
					}
				}
				cover[a[c][i]*v + a[c][j]] = 1;
				if(temppos[a[c][i]*v + a[c][j]] == -1)
					temppos[a[c][i]*v + a[c][j]] = c;
			}
			for(c=0; c<v*v; c++)
			{
				if(!cover[c])
				{
					alcover.push_back(c);
				}
			}
			if(!alcover.empty() && !pos.empty())
			{
				alpos = pos[rand()%pos.size()];
				alcov = alcover[rand()%alcover.size()];
				if(rand()%2)
					b[alpos][i] = alcov/v;
				else
					b[alpos][j] = alcov%v;
				i = n;
				break;
			}
			
		}
		
	}
	
}

bool CoveringArray(int n, int k, int v, double T0, double alpha, int cmax)
{
	int c, d1, d2, de, dbest;
	double T;
	char x[N_MAX][K_MAX];
	char xnext[N_MAX][K_MAX];
	char xbest[N_MAX][K_MAX];

	_int64 start, end;
	LARGE_INTEGER litmp;
	double dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	start = litmp.QuadPart;


	memset(x, 0, sizeof(char)*N_MAX*K_MAX);
	memset(xnext, 0, sizeof(char)*N_MAX*K_MAX);
	GenerateInit(x, n, k, v);


	c = 0;
	T = T0;
	dbest = DensityFunction(x, n, k, v);

	while(c<cmax)
	{
		Neighbour(x, xnext, n, k, v);
		d1 = DensityFunction(x, n, k, v);
		d2 = DensityFunction(xnext, n, k, v);
		de = d2 - d1;
		if(d2 == v*v*k*(k-1)/2)
		{
			dbest = d2;
			QueryPerformanceCounter(&litmp);
			end = litmp.QuadPart;
			printf("CA(2, %d, %d), n=%d iteration=%d running time=", k, v, n, dbest, c);
			cout<<(double)(end-start)*1000/dfFreq<<endl;
			return true;
		}
		if(de>0)
		{
			if(d2 > dbest)
			{
				//memcpy(xbest, xnext, sizeof(char)*N_MAX*K_MAX);
				dbest = d2;
			}
			memcpy(x, xnext, sizeof(char)*N_MAX*K_MAX);
		}
		else
		{
			if(exp(de/T)*PRICISION > rand()%PRICISION)
			{
				memcpy(x, xnext, sizeof(char)*N_MAX*K_MAX);
				if(d2 > dbest)
				{
					//memcpy(xbest, xnext, sizeof(char)*N_MAX*K_MAX);
					dbest = d2;
				}
			}
		}

		T *= alpha;
		c++;
	}
	//printf("CA(%d, 2, %d, %d)=%d/%d\n", n, k, v, dbest, v*v*k*(k-1)/2);
	return false;
}

int main()
{

#ifdef _WIN32
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	srand(systime.wSecond);
#endif
	 
	for(int v=2; v<=4; v++)
	{
		for(int k=3; k<=10; k++)
		{
			for(int n=4+(v-2)*2; n<=29; n++)
			{
				if(CoveringArray(n, k, v, 100.0, 0.9999, 100000))
					break;
			}
			//printf("\n");
		}
	}
	system("pause");
	return 0;
}