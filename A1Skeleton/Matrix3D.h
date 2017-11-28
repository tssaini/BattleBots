/////////////////////////////////////////////////////////////////////////////
// Simple module for 3D matrix operations using doubles.
//
// Written for CPS511, Prof. Georg Feil, Ryerson University, Fall 2017
// For additional documentation see comments in Matrix3D.c
/////////////////////////////////////////////////////////////////////////////

#ifndef MATRIX3D_H
#define MATRIX3D_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <math.h>
#include "Vector3D.h"

// Some <math.h> files do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#define MATRIX_ROWS         4           // Number of rows
#define MATRIX_COLS         4           // Number of columns
#define MATRIX_ELEMENTS     (MATRIX_ROWS * MATRIX_COLS)  // Number of elements in the matrix (4 x 4 unrolled)
#define MATRIX_VALID_EPS    0.000001    // Epsilon value used for matrix validity checks
#define MATRIX_EQUAL_EPS    0.000001    // Epsilon value used for matrix equality check

typedef struct Matrix3D {
    double matrix[MATRIX_ROWS][MATRIX_COLS];     // The 4x4 transformation matrix
} Matrix3D;


// Creation of specific types of useful matrices
Matrix3D NewIdentity(void);
Matrix3D NewRotateX(double deg);
Matrix3D NewRotateY(double deg);
Matrix3D NewRotateZ(double deg);
Matrix3D NewTranslate(double X, double Y, double Z);
Matrix3D NewScale(double X, double Y, double Z);

// Operations. These return the same matrix as passed for the first parameter, for convenience in chaining operations.
Matrix3D* MatrixLoad(Matrix3D* M, const Matrix3D* in);
Matrix3D* MatrixLoadV(Matrix3D* M, const Matrix3D in);
Matrix3D* MatrixAdd(Matrix3D* M, const Matrix3D* in);
Matrix3D* MatrixSubtract(Matrix3D* M, const Matrix3D* in);
Matrix3D* MatrixScalarMultiply(Matrix3D* M, const double val);
Matrix3D* MatrixLeftMultiply(Matrix3D* M, const Matrix3D* in);     // M = in * M
Matrix3D* MatrixLeftMultiplyV(Matrix3D* M, const Matrix3D in);     // M = in * M
Matrix3D* MatrixRightMultiply(Matrix3D* M, const Matrix3D* in);    // M = M * in
Matrix3D* MatrixRightMultiplyV(Matrix3D* M, const Matrix3D in);    // M = M * in
Matrix3D* MatrixInvert(Matrix3D* M);

Vector3D* VectorLeftMultiply(Vector3D* V, const Matrix3D* M);      // V = M * V  (assumes column vector)
Vector3D* VectorLeftMultiplyV(Vector3D* V, const Matrix3D M);      // V = M * V  (assumes column vector)
Vector3D* VectorRightMultiply(Vector3D* V, const Matrix3D* M);     // V = V * M  (assumes row vector)

// Misc
bool MatrixCheckValid(const Matrix3D* M);
bool MatrixEquals(const Matrix3D* M, const Matrix3D* in);
void MatrixGetPosition(const Matrix3D* M, Vector3D* V);
void MatrixGetArray(const Matrix3D* M, double array[MATRIX_ELEMENTS]);
void MatrixGetTransposedArray(const Matrix3D* M, double array[MATRIX_ELEMENTS]);
void MatrixPrint(const Matrix3D* M);


#ifdef __cplusplus
    }
#endif
#endif	//MATRIX3D_H
