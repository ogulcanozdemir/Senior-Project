/*
 * ARMatrix.cpp
 *
 *  Created on: May 26, 2013
 *      Author: oulcan
 */

#include "ARMatrix.h"

// Matrix44 identity
// 1.0f	 0	 0    0
//   0	1.0f 0    0
//   0   0   1.0f 0
//   0   0   0   1.0f
void load_identity(float *M) {
	for (int i = 0; i < 16; ++i) {
		M[i] = 0.0f;
	}

	M[0] = 1.0f;
	M[5] = 1.0f;
	M[10] = 1.0f;
	M[15] = 1.0f;
}

// translation matrix
// 1	0	0	tx
// 0	1	0 	ty
// 0	0	1	tz
// 0	0	0	1
void translate_matrix(float tx, float ty, float tz, float *M) {
	load_identity(M);
	M[12] = tx;
	M[13] = ty;
	M[14] = tz;
}

// scaling matrix
// sx	0	0	0
// 0	sy	0	0
// 0	0	sz	0
// 0	0	0	1
void scale_matrix(float sx, float sy, float sz, float *M) {
	load_identity(M);
	M[0] *= sx;
	M[5] *= sy;
	M[10] *= sz;
}

// rotation matrix
void rotate_matrix(float angle, float x, float y, float z, float *M) {
	double radians, c, s, c1, u[3], length;
	int i, j;

	radians = (angle * MYPI) / 180.0;
	c = cos(radians);
	s = sin(radians);
	c1 = 1.0 - cos(radians);
	length = sqrt(x * x + y * y + z * z);
	u[0] = x / length;
	u[1] = y / length;
	u[2] = z / length;

	for (i = 0; i < 16; i++) {
		M[i] = 0.0;
	}
	M[15] = 1.0;

	for (i = 0; i < 3; i++) {
		M[i * 4 + (i + 1) % 3] = u[(i + 2) % 3] * s;
		M[i * 4 + (i + 2) % 3] = -u[(i + 1) % 3] * s;
	}

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			M[i * 4 + j] += c1 * u[i] * u[j] + (i == j ? c : 0.0);
		}
	}
}

void perspective_matrix(float fovy, float aspect, float znear, float zfar, float *M) {
	int i;
	double f;

	load_identity(M);

	f = 1.0 / tan(fovy * 0.5);

	M[0] = f / aspect;
	M[5] = f;
	M[10] = (znear + zfar) / (znear - zfar);
	M[11] = -1.0;
	M[14] = (2.0 * znear * zfar) / (znear - zfar);
	M[15] = 0.0;
}

void multiply_matrix(float *A, float *B, float *C) {
	int i, j, k;
	float tmpM[16];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			tmpM[j * 4 + i] = 0.0;
			for (k = 0; k < 4; k++) {
				tmpM[j * 4 + i] += A[k * 4 + i] * B[j * 4 + k];
			}
		}
	}

	for (i = 0; i < 16; i++) {
		C[i] = tmpM[i];
	}
}


