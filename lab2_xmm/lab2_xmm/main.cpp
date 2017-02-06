#include <stdio.h>
#include <immintrin.h>
#include <intrin.h>
#include <Windows.h>
#include <time.h>
#include "TimeCounter.hpp"

#define NUM_OF_MATRIXES 1
#define MATRIX_SIZE 8
#define TOO_LARGE_MATRIX (MATRIX_SIZE > 8)

float*** matrix_mem_allocation(float***);
float* matrix_init(float*);
void show_matrix(float*);
void free_mem(float***);

void main_work();
void c_comands_work(float ***m1, float ***m2, float ***m_res);
void sse_comands_work(float ***m1, float ***m2, float ***m_res);

void matrix_multiply(float*, float*, float*);
void matrix_sum_result(float*, float*, float*);
void sse_float_multiply(float*, float*, float*);

int main() {

	system("color 0a");
	srand(time(0));
	
	main_work();

	system("pause");
	return 0;
}

void main_work() {

	float ***m1 = { NULL }, ***m2 = { NULL }, ***m_res = { NULL }, *temp = { NULL };
	
	m1 = matrix_mem_allocation(m1);
	m2 = matrix_mem_allocation(m2);
	m_res = matrix_mem_allocation(m_res);

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			m1[i][j] = matrix_init(m1[i][j]);
			m2[i][j] = matrix_init(m2[i][j]);
		}
	}

#if !TOO_LARGE_MATRIX

	show_matrix(m1[0][0]);
	show_matrix(m2[0][0]);

#endif // MATRIX_SIZE > 8

	c_comands_work(m1, m2, m_res);
	sse_comands_work(m1, m2, m_res);
	
	free_mem(m1);
	free_mem(m2);
	free_mem(m_res);
}
void c_comands_work(float ***m1, float ***m2, float ***m_res) {

	TimeCounter obj;
	obj.StartCounter();

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			for (int k = 0; k < NUM_OF_MATRIXES; k++) {
				float *temp = (float*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
				matrix_multiply(m1[i][k], m2[j][k], temp);
				matrix_sum_result(temp, m_res[i][j], m_res[i][j]);
				free(temp);
			}
		}
	}

#if !TOO_LARGE_MATRIX

	printf("The C result matrix: \n");
	show_matrix(m_res[0][0]);

#endif // MATRIX_SIZE > 8

	printf("Time of C comands: %lf sec\n\n", obj.GetCounter());

}
void sse_comands_work(float ***m1, float ***m2, float ***m_res) {

	TimeCounter obj;
	
	free_mem(m_res); 
	m_res = (float***)calloc(NUM_OF_MATRIXES, sizeof(float));

	obj.StartCounter();

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		m_res[i] = (float**)calloc(NUM_OF_MATRIXES, sizeof(float));
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			m_res[i][j] = (float*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
			for (int k = 0; k < NUM_OF_MATRIXES; k++) {
				float *temp = (float*)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
				sse_float_multiply(m1[i][k], m2[j][k], temp);
				matrix_sum_result(temp, m_res[i][j], m_res[i][j]);
				free(temp);
			}
		}
	}

#if !TOO_LARGE_MATRIX

	printf("The sse result matrix: \n");
	show_matrix(m_res[0][0]);

#endif // MATRIX_SIZE > 8

	printf("Time sse comands: %lf sec\n\n", obj.GetCounter());
}

void matrix_multiply(float* matrix_a, float* matrix_b, float* matrix_res) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			for (int k = 0; k < MATRIX_SIZE; k++) {
				matrix_res[i * MATRIX_SIZE + j] += matrix_a[i * MATRIX_SIZE + k]
					* matrix_b[k * MATRIX_SIZE + j];
			}
		}
	}
}
void matrix_sum_result(float* matrix_a, float* matrix_b, float* matrix_res) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix_res[i * MATRIX_SIZE + j] += matrix_a[i * MATRIX_SIZE + j]
				+ matrix_b[i * MATRIX_SIZE + j];
		}
	}
}
void sse_float_multiply(float *m1, float *m2, float *dst) {

	int tmp_1, tmp_2;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp_1 = i * MATRIX_SIZE;
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp_2 = j * MATRIX_SIZE;

			__m256 r0 = _mm256_broadcast_ss(&m1[tmp_2 + i]);

			for (int step = 0; step < MATRIX_SIZE; step += 8) {
				__m256 r1 = _mm256_loadu_ps(&dst[tmp_2 + step]);
				__m256 r2 = _mm256_loadu_ps(&m2[tmp_1 + step]);
				r2 = _mm256_mul_ps(r2, r0);
				r1 = _mm256_add_ps(r1, r2);
				_mm256_storeu_ps(&dst[tmp_2 + step], r1);
			}
		}
	}
}

float*** matrix_mem_allocation(float*** matrix) {

	matrix = (float***)calloc(NUM_OF_MATRIXES, sizeof(float));

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		matrix[i] = (float**)calloc(NUM_OF_MATRIXES, sizeof(float));
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			matrix[i][j] = (float*)calloc(MATRIX_SIZE * MATRIX_SIZE, 
				sizeof(float));
		}
	}

	return matrix;
}
float* matrix_init(float* matrix) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix[i * MATRIX_SIZE + j] = (float)(rand() % 10);
		}
	}

	return matrix;
}
void show_matrix(float* matrix) {

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			printf("%7.2f", matrix[i * MATRIX_SIZE + j]);
		}
		printf("\n");
	}

	printf("\n\n");
}
void free_mem(float*** matrix) {

	for (int i = 0; i < NUM_OF_MATRIXES; i++) {
		for (int j = 0; j < NUM_OF_MATRIXES; j++) {
			free(matrix[i][j]);
		}
		free(matrix[i]);
	}
	free(matrix);
}