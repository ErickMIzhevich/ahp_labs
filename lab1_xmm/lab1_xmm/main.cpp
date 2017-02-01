//#pragma novector

#include <iostream>
#include <xmmintrin.h>
#include <Windows.h>
#include <time.h>
#include "TimeCounter.hpp"

#define NUM_OF_MATRIXES 300
#define MATRIX_SIZE 8


double* matrix_mem_allocation(double*);
double* matrix_init(double*);
void show_matrix(double*);
void free_mem(double*);

void main_work();
void matrix_multiply(double*, double*, double*);
void sse_multiply(double*, double*, double*);


int main() {

	system("color 0a");
	srand(time(0));

	main_work();

	system("pause");
	return 0;
}
void main_work() {

	double ***m1 = { NULL }, ***m2 = { NULL }, ***m_res = { NULL };
	TimeCounter obj;

	m1 = (double***)calloc(NUM_OF_MATRIXES, sizeof(double));
	m2 = (double***)calloc(NUM_OF_MATRIXES, sizeof(double));
	m_res = (double***)calloc(NUM_OF_MATRIXES, sizeof(double));

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		m1[i] = (double**)calloc(NUM_OF_MATRIXES, sizeof(double));
		m2[i] = (double**)calloc(NUM_OF_MATRIXES, sizeof(double));
		m_res[i] = (double**)calloc(NUM_OF_MATRIXES, sizeof(double));
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			m1[i][j] = matrix_mem_allocation(m1[i][j]);
			m1[i][j] = matrix_init(m1[i][j]);
			m2[i][j] = matrix_mem_allocation(m2[i][j]);
			m2[i][j] = matrix_init(m2[i][j]);
			m_res[i][j] = matrix_mem_allocation(m_res[i][j]);
		}
	}

	show_matrix(m1[0][0]);
	show_matrix(m2[0][0]);

	obj.StartCounter();

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			matrix_multiply(m1[i][j], m2[i][j], m_res[i][j]);
		}
	}

	printf("The result matrix: \n");
	show_matrix(m_res[0][0]);
	printf("Time result: %lf ms\n\n", obj.GetCounter());

#ifndef NULL
	// size and data type of matrixes are not supported
	sse_multiply(m1, m2, m_res);
#endif // !0

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			free_mem(m1[i][j]);
			free_mem(m2[i][j]);
			free_mem(m_res[i][j]);
		}
	}
}

void matrix_multiply(double* matrix_a, double* matrix_b, double* matrix_res) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			for (int k = 0; k < MATRIX_SIZE; k++) {
				matrix_res[i * MATRIX_SIZE + j] += matrix_a[i * MATRIX_SIZE + k] * matrix_b[j * MATRIX_SIZE + k];
			}
		}
	}
}

// massives size: matrix 4x4 = 16
// matrix 4x4 multiplying with sse comands
void sse_multiply(float *m1, float *m2, float *dst) {

	__m128 b0, b1, b2, b3;
	__m128 row, rslt, tmp;

	b0 = _mm_loadh_pi(_mm_loadl_pi(b0, &(__m64&)(m2[0])), &(__m64&)m2[2]);
	b1 = _mm_loadh_pi(_mm_loadl_pi(b1, &(__m64&)m2[4]), &(__m64&)m2[6]);
	b2 = _mm_loadh_pi(_mm_loadl_pi(b2, &(__m64&)m2[8]), &(__m64&)m2[10]);
	b3 = _mm_loadh_pi(_mm_loadl_pi(b3, &(__m64&)m2[12]), &(__m64&)m2[14]);

	//0-3 

	row = _mm_set_ps1(m1[0]);
	rslt = _mm_mul_ps(row, b0);

	row = _mm_set_ps1(m1[1]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b1));

	row = _mm_set_ps1(m1[2]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b2));

	row = _mm_set_ps1(m1[3]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b3));
	
	_mm_store_ps(&dst[0], rslt);

	//4-8

	row = _mm_set_ps1(m1[4]);
	rslt = _mm_mul_ps(row, b0);

	row = _mm_set_ps1(m1[5]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b1));

	row = _mm_set_ps1(m1[6]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b2));

	row = _mm_set_ps1(m1[7]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b3));

	_mm_store_ps(&dst[4], rslt);

	// 9-12

	row = _mm_set_ps1(m1[8]);
	rslt = _mm_mul_ps(row, b0);

	row = _mm_set_ps1(m1[9]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b1));

	row = _mm_set_ps1(m1[10]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b2));

	row = _mm_set_ps1(m1[11]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b3));

	_mm_store_ps(&dst[8], rslt);

	// 13-16

	row = _mm_set_ps1(m1[12]);
	rslt = _mm_mul_ps(row, b0);

	row = _mm_set_ps1(m1[13]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b1));

	row = _mm_set_ps1(m1[14]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b2));

	row = _mm_set_ps1(m1[15]);
	rslt = _mm_add_ps(rslt, _mm_mul_ps(row, b3));

	_mm_store_ps(&dst[12], rslt);
}

double* matrix_mem_allocation(double* matrix) {

	matrix = (double*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));

	return matrix;
}

double* matrix_init(double* matrix) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix[i * MATRIX_SIZE + j] = (double)(rand() % 10);
		}
	}

	return matrix;
}

void show_matrix(double* matrix) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			printf("%7.2f", matrix[i * MATRIX_SIZE + j]);
		}
		printf("\n");
	}

	printf("\n\n");
}

void free_mem(double* matrix) {

	free(matrix);
}