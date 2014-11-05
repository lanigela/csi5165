#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <numeric>
using namespace std;

int function;
int maxlevel;
int vn;
int vc[20];

inline int HammingDist(int a, int b, int n)
{
	int c = a^b; 
	int ans = 0;
	for(int i=0; i<n; i++)
		if(c&(1<<i))
			ans++;
	return ans;
}

void GenerateCand(int n, int d, vector<int> &cand)
{
	for(int i=0; i<(1<<n); i++) {
		if (HammingDist(0, i, n)>=d)
			cand.push_back(i);
	}
	
	//printf("candsize=%d\n",cand.size());
}


void backtrack(int n, int d, int node, vector<int> cand, int level)
{
	vector<int> candNext;
	vn++;
	for (int i=0; i<cand.size(); i++)
	{
		if(HammingDist(node, cand[i], n)>=d) {
			candNext.push_back(cand[i]);
		}
	}
	if(candNext.size()==0) {
		if(level>maxlevel) {
			maxlevel = level;
			//printf("update maxlevel=%d\n",maxlevel);
			
		}
		return;
	}
	
	for (int i=0; i<candNext.size(); i++) {
		backtrack(n, d, candNext[i], candNext, level+1);
	}
}



void knuth(int n, int d, int node, vector<int> cand, int level)
{
	vector<int> candNext;
	
	for (int i=0; i<cand.size(); i++)
	{
		if(HammingDist(node, cand[i], n)>=d) {
			candNext.push_back(cand[i]);
		}
	}
	if(candNext.size()==0) {
		if(level>maxlevel) {
			maxlevel = level;
			//printf("update maxlevel=%d\n",maxlevel);
			
		}
		return;
	}
	
	vc[level]=candNext.size();
	backtrack(n, d, candNext[1], candNext, level+1);
}

void init(int n, int d)
{
	vector<int> cand;
	GenerateCand(n, d, cand);
	maxlevel = 0;
	vn=0;
	memset(vc, 0, sizeof(int)*20);
	if(function==0)
		backtrack(n, d, 0, cand, 0);
	else if(function==1){
		knuth(n, d, 0, cand, 0);
		int sum = 0;
		int est = 1;
		for(int i=0; i<maxlevel; i++)
		{
			est *= vc[i];
			sum += est;
		}
		printf("Estimate node visit=%d\n",sum);
	}
}

int main()
{
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	srand(systime.wMilliseconds);

	_int64 start, end;
	LARGE_INTEGER litmp;
	double dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);

	_int64 estimatenode;
	_int64 estimateans;
	for (function=0; function<=1; function++) {

		for (int i=4; i<8; i++){
			if(function==0) {
				start = litmp.QuadPart;
				init(i, 4);
				QueryPerformanceCounter(&litmp);
				end = litmp.QuadPart;
				printf("A(%d,4)=%d Visited node=%-9dRunning time(ms)=%lf\n", i, maxlevel+1,vn,(double)(end-start)*1000/dfFreq);
			}
			else if(function==1) {
				init(i, 4);
			}
		}

	}
	system("pause");
	return 0;
}