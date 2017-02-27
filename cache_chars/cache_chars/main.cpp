#include	<stdio.h> 
#include	<stdlib.h> 
#include	<string.h> 
#include	<time.h> 
#include	<iostream> 
#include	<Windows.h> 

using namespace std;

#define		T	char 
#define		MAX_S	0x1000000 
#define		L	101 

volatile T A[MAX_S];
int m_rand[0xFFFFFF];

int main(){
	LARGE_INTEGER freq; 
	LARGE_INTEGER time1; 	
	LARGE_INTEGER time2;
	QueryPerformanceFrequency(&freq);

	memset((void*)A, 0, sizeof (A));

	srand(time(NULL));

	int v, M;
	register int i, j, k, m, x;

	for (k = 1024; k < MAX_S;) {
		M = k / L;

		printf("%g\t", (k + M * 4) / (1024. * 1024));

		for (i = 0; i < M; i++) m_rand[i] = L * i;

		for (i = 0; i < M / 4; i++)	{
			j = rand() % M;
			x = rand() % M;

			m = m_rand[j];
			m_rand[j] = m_rand[i];
			m_rand[i] = m;

		}

		if (k < 100 * 1024) j = 1024;
		else if (k < 300 * 1024) j = 128;
		else j = 32;

		QueryPerformanceCounter(&time1);
		for (i = 0; i < j; i++) {

			for (m = 0; m < L; m++) {
				for (x = 0; x < M; x++){
					v = A[m_rand[x] + m];
				}
			}

		}
		QueryPerformanceCounter(&time2);

		time2.QuadPart -= time1.QuadPart;
		double span = (double)time2.QuadPart / freq.QuadPart;

		printf("%g\n", 1000000000. * span / (double)(L*M*j));

		if (k > 100 * 1024)	k += k / 16;
		else k += 4 * 1024;
	}
	return 0;
}