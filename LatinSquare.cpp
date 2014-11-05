#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <numeric>
using namespace std;

int function;

char board[8][8];
int row[8];
int col[8];
int c[64];
int maxlevel;
int estnode;
int estans;
_int64 ans;
_int64 vn;


void backtrack(int x, int y, int n)
{
	int cand = row[x] | col[y];
	for (int i=0; i<n; i++) {
		 if(!(cand & (1<<i))) {
			 vn++;
			 // leaf
			if(x>=n-2 && y>=n-1) {
				ans++;
				continue;
			}
			row[x] += 1<<i;
			col[y] += 1<<i;
			backtrack(y==n-1?x+1:x, y==n-1?1:y+1, n);
			row[x] -= 1<<i;
			col[y] -= 1<<i;
		}
	}
}

void knuth(int x, int y, int n, int level)
{
	int cn = 0;
	int cand = row[x] | col[y];
	vector<int> candv;

	for (int i=0; i<n; i++) {
		 if(!(cand & (1<<i))) {
			 candv.push_back(i);
			 cn++;
		}
	}
	if((x>=n-2 && y>=n-1) || (cn==0)){
		if(level>maxlevel)
			maxlevel = level;
		//printf("max level=%d\n",level);
		return;
	}

	c[level] = cn;
	
	cand = candv[rand()%cn];
	row[x] += 1<<cand;
	col[y] += 1<<cand;
	knuth(y==n-1?x+1:x, y==n-1?1:y+1, n, level+1);
}

void init(int n)
{
	if(n<2) {
		ans =  1;
		return ;
	}
	memset(board, 0, sizeof(char)*8*8);
	memset(row, 0, sizeof(int)*8);
	memset(col, 0, sizeof(int)*8);
	memset(c, 0, sizeof(int)*64);

	for (int i=1; i<n; i++) {
		board[0][i] = board[i][0] = 1<<i;
		row[0] += board[0][i];
		col[i] += board[0][i];

		row[i] += board[i][0];
		col[0] += board[i][0];
	}
	row[0] -= 1;
	col[0] -= 1;

	ans = 0;
	vn = 0;
	if(function ==0)
		backtrack(1, 1, n);
	else if(function ==1) {
		int j;
		maxlevel = 0;
		knuth(1, 1, n, 0);
		_int64 est, sum;
		est=1;
		sum=0;
		for(j=0; j<maxlevel; j++) {
			est *= (c[j]);
			sum += est;
		}
		estnode = sum;
		if(maxlevel == ((n-1)*(n-2)-1))
			estans = est;
		else
			estans = 0;
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
	for (function=0; function<2; function++) {
		for (int i=2; i<8; i++){
				if(function==0) {
				start = litmp.QuadPart;
				init(i);
				QueryPerformanceCounter(&litmp);
				end = litmp.QuadPart;
				printf("L%d=%-11I64dNode visited:%-11I64dRunning time(ms):%lf\n", i, ans, vn,(double)(end-start)*1000/dfFreq);
			}
			else if(function==1) {
				estimatenode = 0;
				estimateans = 0;
				int repeat = 10000;
				for (int j=0;j<repeat;j++) {
					init(i);
					estimatenode += estnode;
					estimateans += estans;
				}

				printf("L%-3d Estimate visit node is: %-10I64d Estimate answer is: %I64d\n",i, estimatenode/repeat, estimateans/repeat);
			}
		}

	}
	system("pause");
	return 0;
}