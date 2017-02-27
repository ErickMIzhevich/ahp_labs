#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BLOCKSIZE_IN_BYTES 3145728		//32768; 262144; 3145728
#define MAX_WAY_NUMBER 25
#define OFFSET_IN_BYTES 16777216		// 16 Мб
#define STRUCT_ARRAY_SIZE 60

typedef unsigned long long int ULII;

struct ArrayNode  
{
	__int32 next;
	__int8 arr[STRUCT_ARRAY_SIZE];

	operator __int32() { 
		return next; 
	}

	ArrayNode& operator=(int n) {
		next = n;
		return *this;
	}
};

void initArray(int* buffer, int fragmentSize, int offset, int numberOfWays);
ULII* showAssotiativity(int buf[]); 

void setProcessPriority();
void mainWork();
ULII* initTimeResultsArray(ULII*);
int* memAllocateNumBuffer(int*);
ULII* setCycles(ULII*, int*, int);
void printResults(ULII*, int*, int);

int main()
{
	setProcessPriority();
	mainWork();

	system("pause");
	return 0;
}

void setProcessPriority() {

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
}
void mainWork() {

	int* buffer = memAllocateNumBuffer(buffer);
	int number = 4;

	ULII *resultsTime = initTimeResultsArray(resultsTime);
	resultsTime = setCycles(resultsTime, buffer, number);

	printResults(resultsTime, buffer, number);

	_aligned_free(buffer);
	_aligned_free(resultsTime);
}

ULII* initTimeResultsArray(ULII* resultsTime) {

	resultsTime = (ULII *)_aligned_malloc(
		(SIZE_T)(MAX_WAY_NUMBER * sizeof(ULII)), 64);

	for (int j = 1; j < MAX_WAY_NUMBER; j++) {
		resultsTime[j] = 0;
	}

	return resultsTime;
}
int* memAllocateNumBuffer(int* numBuffer) {
	numBuffer = (int *)_aligned_malloc(
		(SIZE_T)(OFFSET_IN_BYTES * MAX_WAY_NUMBER), 64);

	return numBuffer;
}
ULII* setCycles(ULII* resultsTime, int* numBuffer, int number) {

	for (int i = 0; i < number; i++)	{
		ULII *result = showAssotiativity(numBuffer);

		for (int j = 1; j < MAX_WAY_NUMBER; j++) {
			resultsTime[j] += result[j];
		}
	}

	return resultsTime;
}
void printResults(ULII* resultsTime, int* numBuffer, int number) {

	for (int j = 1; j < MAX_WAY_NUMBER; j++) {
		resultsTime[j] /= number;
		printf("possible assosiativity: %2d, CPU cycles: %llu\n", j, resultsTime[j]);
	}
}

void initArray(int* buffer, int fragmentSize, int offset, int numberOfWays) {

	for (int i = 0; i < numberOfWays; i++) {
		for (int j = 0; j < fragmentSize; j++) {
			if (i < numberOfWays - 1) {
				buffer[i * offset + j] = offset * (i + 1) + j;
			}
			else {
				(j < fragmentSize - 1)
					? buffer[i * offset + j] = j + 1
					: buffer[i * offset + j] = 0;
			}
		}
	}
}
ULII* showAssotiativity(int buf[]) {

	int index;
	ULII beginTime, endTime;
	ULII* resultsTime = new ULII[MAX_WAY_NUMBER];

	int offset = OFFSET_IN_BYTES / sizeof(int);

	for (int i = 1; i < MAX_WAY_NUMBER; i++) {
		int iterationsCount = BLOCKSIZE_IN_BYTES / (sizeof(int)* i) * i;
		index = 0;

		initArray(buf, BLOCKSIZE_IN_BYTES / (sizeof(int)* i),
			OFFSET_IN_BYTES / sizeof(int), i);


		beginTime = __rdtsc();
		for (int j = 0; j < iterationsCount; j++) {
			index = buf[index++];
		}
		resultsTime[i] = __rdtsc() - beginTime;

		// Чтобы оптимизатор не убрал цикл
		if (index == 100) {
			printf("");
		}
	}

	return resultsTime;
}