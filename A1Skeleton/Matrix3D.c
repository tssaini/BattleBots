/////////////////////////////////////////////////////////////////////////////
// Simple module for 3D matrix operations using doubles.
//
// Written for CPS511, Prof. Georg Feil, Ryerson University, Fall 2017
/////////////////////////////////////////////////////////////////////////////
//
// 4x4 Homogeneous Transformation Matrix
//
// These functions are designed so that they may be used without malloc/free.
// No functions here call malloc.
//
// Example calling sequences
// 1. Create an identity matrix, perform some transformations on it, then use the
//    resulting matrix to transform a point/vertex. In other words  M = R * T * S; v = M * v
// Matrix3D m = NewIdentity();
// MatrixLeftMultiplyV(&m, NewScale(0.5, 1, 1));
// MatrixLeftMultiplyV(&m, NewTranslate(0, 5.2, -3.9));
// MatrixLeftMultiplyV(&m, NewRotateZ(33));
// Vector3D v = NewVector3D(5, 7, -2.5);
// VectorLeftMultiply(&v, &m);
//
// 2. Create an identity matrix and perform some change-of-coordinate-frame
//    transformations on it (right-multiply like OpenGL). In other words  M = R * T * S
// Matrix3D m = NewIdentity();
// MatrixRightMultiplyV(&m, NewRotateZ(33));
// MatrixRightMultiplyV(&m, NewTranslate(0, 5.2, -3.9));
// MatrixRightMultiplyV(&m, NewScale(0.5, 1, 1));
//
// 3. Same as (1) but more compact, with operation chaining
// Matrix3D m = NewScale(0.5, 1, 1);
// Vector3D v = NewVector3D(5, 7, -2.5);
// VectorLeftMultiply(&v, MatrixLeftMultiplyV(MatrixLeftMultiplyV(&m, NewTranslate(0, 5.2, -3.9)), NewRotateZ(33)));
//
// 4. Same as (1) but using vector multiplication only
// Vector3D v = NewVector3D(5, 7, -2.5);
// VectorLeftMultiplyV(&v, NewScale(0.5, 1, 1));
// VectorLeftMultiplyV(&v, NewTranslate(0, 5.2, -3.9));
// VectorLeftMultiplyV(&v, NewRotateZ(33));
// 
// 5. C/C++ uses row-major ordering, however OpenGL uses column-major ordering,
// so to pass a Matrix3D to OpenGL use:
// Matrix3D matrix = ...;
// double arr[MATRIX_ELEMENTS];
// MatrixGetTransposedArray(&matrix, arr);
// glMultMatrixd(arr);
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "Matrix3D.h"

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewIdentity
//
// Purpose:    Returns a 4x4 identity matrix.
//
// Parameters: Return value is the identity matrix.
//
Matrix3D NewIdentity(void)
{
    Matrix3D M;

    // The identity matrix
    M.matrix[0][0] = 1.0;
    M.matrix[0][1] = 0.0;
    M.matrix[0][2] = 0.0;
    M.matrix[0][3] = 0.0;
    M.matrix[1][0] = 0.0;
    M.matrix[1][1] = 1.0;
    M.matrix[1][2] = 0.0;
    M.matrix[1][3] = 0.0;
    M.matrix[2][0] = 0.0;
    M.matrix[2][1] = 0.0;
    M.matrix[2][2] = 1.0;
    M.matrix[2][3] = 0.0;
    M.matrix[3][0] = 0.0;
    M.matrix[3][1] = 0.0;
    M.matrix[3][2] = 0.0;
    M.matrix[3][3] = 1.0;

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewRotateX
//
// Purpose:    Returns a matrix for rotation around the X axis.
//
// Parameters: 'deg' is the rotation angle in degrees.
//             Return value is the rotation matrix.
//
Matrix3D NewRotateX(double deg)
{
    Matrix3D M = NewIdentity();
    double rad = deg * M_PI / 180;
    if (rad != 0) {
        double c = cos(rad), s = sin(rad);
        M.matrix[1][1] = c;
        M.matrix[1][2] = -s;
        M.matrix[2][1] = s;
        M.matrix[2][2] = c;
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewRotateY
//
// Purpose:    Returns a matrix for rotation around the Y axis.
//
// Parameters: 'deg' is the rotation angle in degrees.
//             Return value is the rotation matrix.
//
Matrix3D NewRotateY(double deg)
{
    Matrix3D M = NewIdentity();
    double rad = deg * M_PI / 180;
    if (rad != 0) {
        double c = cos(rad), s = sin(rad);
        M.matrix[0][0] = c;
        M.matrix[0][2] = s;
        M.matrix[2][0] = -s;
        M.matrix[2][2] = c;
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewRotateZ
//
// Purpose:    Returns a matrix for rotation around the Z axis.
//
// Parameters: 'deg' is the rotation angle in degrees.
//             Return value is the rotation matrix.
//
Matrix3D NewRotateZ(double deg)
{
    Matrix3D M = NewIdentity();
    double rad = deg * M_PI / 180;
    if (rad != 0) {
        double c = cos(rad), s = sin(rad);
        M.matrix[0][0] = c;
        M.matrix[0][1] = -s;
        M.matrix[1][0] = s;
        M.matrix[1][1] = c;
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewTranslate
//
// Purpose:    Returns a translation matrix.
//
// Parameters: 'X' is the x-axis translation.
//             'Y' is the y-axis translation.
//             'Z' is the z-axis translation.
//             Return value is the translation matrix.
//
Matrix3D NewTranslate(double X, double Y, double Z)
{
    Matrix3D M = NewIdentity();
    M.matrix[0][3] = X;
    M.matrix[1][3] = Y;
    M.matrix[2][3] = Z;

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       NewScale
//
// Purpose:    Returns a scaling matrix.
//
// Parameters: 'X' is the x-axis scaling factor.
//             'Y' is the y-axis scaling factor.
//             'Z' is the z-axis scaling factor.
//             Return value is the scaling matrix.
//
Matrix3D NewScale(double X, double Y, double Z)
{
    Matrix3D M = NewIdentity();
    M.matrix[0][0] = X;
    M.matrix[1][1] = Y;
    M.matrix[2][2] = Z;

    return M;
}

// Operations

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixLoad
//
// Purpose:    Loads new matrix values from another matrix (passed by reference).
//
// Parameters: 'M' is the matrix to be loaded (copy destination).
//             'in' is the matrix to load values from (copy source).
//             Return value is the first parameter for convenience when chaining operations.
//
Matrix3D* MatrixLoad(Matrix3D* M, const Matrix3D* in)
{
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; i < MATRIX_COLS; i++) {
            M->matrix[i][j] = in->matrix[i][j];
        }
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixLoadV
//
// Purpose:    Loads new matrix values from another matrix (passed by value).
//
// Parameters: 'M' is the matrix to be loaded (copy destination).
//             'in' is the matrix to load values from (copy source).
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: This is intended to be used with the New* functions, for example
//          Matrix3D R;
//          MatrixLoadV(&R, NewRotateZ(87.0));
//
Matrix3D* MatrixLoadV(Matrix3D* M, const Matrix3D in)
{
    return MatrixLoad(M, &in);
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixAdd
//
// Purpose:    Performs matrix addition, M = M + in.
//
// Parameters: 'M' is the matrix to add to and is updated with the result.
//             'in' is the matrix to add.
//             Return value is the first parameter for convenience when chaining operations.
//
Matrix3D* MatrixAdd(Matrix3D* M, const Matrix3D* in)
{
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; i < MATRIX_COLS; i++) {
            M->matrix[i][j] += in->matrix[i][j];
        }
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixSubtract
//
// Purpose:    Performs matrix subtraction, M = M - in.
//
// Parameters: 'M' is the matrix to subtract from and is updated with the result.
//             'in' is the matrix to subtract.
//             Return value is the first parameter for convenience when chaining operations.
//
Matrix3D* MatrixSubtract(Matrix3D* M, const Matrix3D* in)
{
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; i < MATRIX_COLS; i++) {
            M->matrix[i][j] -= in->matrix[i][j];
        }
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixScalarMultiply
//
// Purpose:    Multiplies a matrix by a scalar, M = val * M
//
// Parameters: 'M' is the matrix to multiply and is updated with the result.
//             'val' is the scalar value.
//             Return value is the first parameter for convenience when chaining operations.
//
Matrix3D* MatrixScalarMultiply(Matrix3D* M, const double val)
{
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; i < MATRIX_COLS; i++) {
            M->matrix[i][j] *= val;
        }
    }

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixLeftMultiply
//
// Purpose:    Performs left-multiply, i.e. M = in * M
//
// Parameters: 'M' is the matrix to be multiplied and is updated with the result.
//             'in' is the matrix to multiply by on the left.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: Assumes MATRIX_ROWS == MATRIX_COLS.
// This function is suitable to be used when performing a sequence of object-centric
// transformations, where object points/vertices are being transformed in a fixed reference frame.
// In other words:   p' = C * B * A * p
//
Matrix3D* MatrixLeftMultiply(Matrix3D* M, const Matrix3D* in)
{
    Matrix3D temp;

    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            temp.matrix[i][j] = 0;
            for (int k = 0; k < MATRIX_ROWS; k++) {
                temp.matrix[i][j] += in->matrix[i][k] * M->matrix[k][j];
            }
        }
    }

    *M = temp;
    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixLeftMultiplyV
//
// Purpose:    Performs left-multiply, i.e. M = in * M  ('in' passed by value)
//
// Parameters: 'M' is the matrix to be multiplied and is updated with the result.
//             'in' is the matrix to multiply by on the left.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: This is intended to be used with the New* functions. See MatrixLeftMultiply.
//
Matrix3D* MatrixLeftMultiplyV(Matrix3D* M, const Matrix3D in)
{
    return MatrixLeftMultiply(M, &in);
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixRightMultiply
//
// Purpose:    Performs right-multiply, i.e. M = M * in
//
// Parameters: 'M' is the matrix to be multiplied and is updated with the result.
//             'in' is the matrix to multiply by on the right.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: Assumes MATRIX_ROWS == MATRIX_COLS.
// This function is suitable to be used when performing a sequence of change-of-coordinate-frame
// transformations, where the coordinate frame origin is being transformed to a new reference frame.
// In other words:   M = A * B * C
//
Matrix3D* MatrixRightMultiply(Matrix3D* M, const Matrix3D* in)
{
    Matrix3D temp;

    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            temp.matrix[i][j] = 0;
            for (int k = 0; k < MATRIX_ROWS; k++) {
                temp.matrix[i][j] += M->matrix[i][k] * in->matrix[k][j];
            }
        }
    }

    *M = temp;
    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixRightMultiplyV
//
// Purpose:    Performs right-multiply, i.e. M = M * in  ('in' passed by value)
//
// Parameters: 'M' is the matrix to be multiplied and is updated with the result.
//             'in' is the matrix to multiply by on the right.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: This is intended to be used with the New* functions. See MatrixRightMultiply.
//
Matrix3D* MatrixRightMultiplyV(Matrix3D* M, const Matrix3D in)
{
    return MatrixRightMultiply(M, &in);
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixInvert
//
// Purpose:    Replaces a transformation matrix by its inverse.
//
// Parameters: 'M' is the matrix to invert and is updated with the result.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: This function works only on 4x4 transformation matrices with an orthonormal
// 3x3 submatrix and translation components. The bottom row must be [0, 0, 0, 1].
// This function cannot be used to invert general matrices (MatrixCheckValid()
// must return true). It works by transposing the upper left 3x3 sub-matrix and
// adjusting the translation components.
//
Matrix3D* MatrixInvert(Matrix3D* M)
{
    // Take transpose of 3x3 sub-matrix
    double m1 = M->matrix[1][0];
    double m2 = M->matrix[2][0];
    double m4 = M->matrix[0][1];
    double m6 = M->matrix[2][1];
    double m8 = M->matrix[0][2];
    double m9 = M->matrix[1][2];

    // Adjust translation components by multiplying column vector [x y z] on the left by negated transpose of 3x3 sub-matrix.
    double x = -(M->matrix[0][3] * M->matrix[0][0] + M->matrix[1][3] * M->matrix[1][0] + M->matrix[2][3] * M->matrix[2][0]);
    double y = -(M->matrix[0][3] * M->matrix[0][1] + M->matrix[1][3] * M->matrix[1][1] + M->matrix[2][3] * M->matrix[2][1]);
    double z = -(M->matrix[0][3] * M->matrix[0][2] + M->matrix[1][3] * M->matrix[1][2] + M->matrix[2][3] * M->matrix[2][2]);

    // Store
    M->matrix[0][1] = m1;
    M->matrix[0][2] = m2;
    M->matrix[1][0] = m4;
    M->matrix[1][2] = m6;
    M->matrix[2][0] = m8;
    M->matrix[2][1] = m9;
    M->matrix[0][3] = x;
    M->matrix[1][3] = y;
    M->matrix[2][3] = z;

    return M;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       VectorLeftMultiply
//
// Purpose:    Performs left-multiply of a column vector by a matrix,
//             i.e. [x  = M * [x
//                   y         y
//                   z]        z
//                             1]
//
// Parameters: 'V' updates a 3-element column vector [x y z].
//             'M' is the matrix to multiply the vector by on the left.
//             Return value is the first parameter for convenience when chaining operations.
//
// Note: The implied fourth element (w) of the vector V is always assumed to be 1.
//
Vector3D* VectorLeftMultiply(Vector3D* V, const Matrix3D* M)
{
    double t[3];
    double v[4];
    v[0] = V->x;
    v[1] = V->y;
    v[2] = V->z;
    v[3] = 1;

    for (int j = 0; j < 3; j++) {
        t[j] = 0;
        for (int k = 0; k < 4; k++) {
            t[j] += v[k] * M->matrix[j][k];
        }
    }

    // These lines may generate some warnings, but we don't cast to float in case Vector3D
    // element types are changed to 'double' in the future.
    V->x = t[0];
    V->y = t[1];
    V->z = t[2];

    return V;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       VectorLeftMultiplyV
//
// Purpose:    Performs left-multiply of a column vector by a matrix, where
//             the matrix is passed by value.
//             i.e. [x  = M * [x
//                   y         y
//                   z]        z
//                             1]
//
// Parameters: 'V' updates a 3-element column vector [x y z].
//             'M' is the matrix to multiply the vector by on the left.
//             Return value is the first parameter for convenience when chaining operations.
//
// Note: This is intended to be used with the New* functions. See VectorLeftMultiply.
//
Vector3D* VectorLeftMultiplyV(Vector3D* V, const Matrix3D M)
{
    return VectorLeftMultiply(V, &M);
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       VectorRightMultiply
//
// Purpose:    Performs right-multiply of a row vector by a matrix,
//             i.e. [x y z] = [x y z 1] * M
//
// Parameters: 'V' updates a 3-element row vector [x y z].
//             'M' is the matrix to multiply the vector by on the right.
//             Return value is the first parameter for convenience when chaining operations.
//
// Notes: The implied fourth element (w) of the vector V is always assumed to be 1.
//        ***For typical situations use VectorLeftMultiply or VectorLeftMultiplyV, not this function.
//
Vector3D* VectorRightMultiply(Vector3D* V, const Matrix3D* M)
{
    double t[3];
    double v[4];
    v[0] = V->x;
    v[1] = V->y;
    v[2] = V->z;
    v[3] = 1;

    for (int j = 0; j < 3; j++) {
        t[j] = 0;
        for (int k = 0; k < 4; k++) {
            t[j] += v[k] * M->matrix[k][j];
        }
    }

    // These lines may generate some warnings, but we don't cast to float in case Vector3D
    // element types are changed to 'double' in the future.
    V->x = t[0];
    V->y = t[1];
    V->z = t[2];

    return V;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixCheckValid
//
// Purpose:    Checks if a matrix is valid.
//
// Parameters: 'M' is the matrix to check.
//             Return value is true if the matrix is valid, false otherwise.
//
// Notes: This check requires the matrix to be pure rotation, or rotation + translation.
// Scaling is not allowed. The 3 x 3 rotation submatrix must be close to
// orthogonal: the rows must have length very close to 1 and must be mutually
// perpendicular. The threshold constant MATRIX_VALID_EPS (epsilon) is
// used when checking "close" to 1 or 0. Note that the translation components
// are not checked in any way.
//
bool MatrixCheckValid(const Matrix3D* M)
{
    bool valid = true;

    // Check that last row is [0 0 0 1]
    if (valid)
        valid = M->matrix[3][0] == 0 && M->matrix[3][1] == 0 && M->matrix[3][2] == 0 && M->matrix[3][3] == 1;

    // Check that the rotational component's rows (3 x 3 submatrix) have unit length
    if (valid)
        valid = fabs(1 - sqrt(M->matrix[0][0] * M->matrix[0][0] + M->matrix[0][1] * M->matrix[0][1] + M->matrix[0][2] * M->matrix[0][2])) < MATRIX_VALID_EPS;
    if (valid)
        valid = fabs(1 - sqrt(M->matrix[1][0] * M->matrix[1][0] + M->matrix[1][1] * M->matrix[1][1] + M->matrix[1][2] * M->matrix[1][2])) < MATRIX_VALID_EPS;
    if (valid)
        valid = fabs(1 - sqrt(M->matrix[2][0] * M->matrix[2][0] + M->matrix[2][1] * M->matrix[2][1] + M->matrix[2][2] * M->matrix[2][2])) < MATRIX_VALID_EPS;

    // Check that rows of 3 x 3 submatrix are mutually orthogonal
    if (valid)
        valid = fabs(M->matrix[0][0] * M->matrix[1][0] + M->matrix[0][1] * M->matrix[1][1] + M->matrix[0][2] * M->matrix[1][2]) < MATRIX_VALID_EPS;
    if (valid)
        valid = fabs(M->matrix[0][0] * M->matrix[2][0] + M->matrix[0][1] * M->matrix[2][1] + M->matrix[0][2] * M->matrix[2][2]) < MATRIX_VALID_EPS;
    if (valid)
        valid = fabs(M->matrix[1][0] * M->matrix[2][0] + M->matrix[1][1] * M->matrix[2][1] + M->matrix[1][2] * M->matrix[2][2]) < MATRIX_VALID_EPS;

    return (valid);
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixEquals
//
// Purpose:    Checks if two matrices are equal.
//
// Parameters: 'M' is one matrix to compare.
//             'in' is the other matrix to compare.
//             Return value is true if all elements of the matrix are
//             equal to within MATRIX_EQUAL_EPS, otherwise false.
//
bool MatrixEquals(const Matrix3D* M, const Matrix3D* in)
{
    bool ret = true;

    for (int i = 0; i < MATRIX_ROWS && ret; i++) {
        for (int j = 0; j < MATRIX_COLS && ret; j++) {
            ret = fabs(M->matrix[i][j] - in->matrix[i][j]) < MATRIX_EQUAL_EPS;
        }
    }

    return ret;
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixGetPosition
//
// Purpose:    Extracts the translation components of a matrix.
//
// Parameters: 'M' is the matrix to get values from.
//             'V' returns the x/y/z translation components, equivalent to the
//                 point that (0,0,0) would be translated to by matrix M.
//
void MatrixGetPosition(const Matrix3D* M, Vector3D* V)
{
    // Get the position
    if (V != NULL) {
        V->x = M->matrix[0][3];
        V->y = M->matrix[1][3];
        V->z = M->matrix[2][3];
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixGetArray
//
// Purpose:    Retrieves matrix contents as a single-dimensional array of doubles.
//
// Parameters: 'M' is the matrix to get values from.
//             'array' returns an array of doubles with MATRIX_ELEMENTS elements
//                 in row-major order.
//             Return value is error code.
//
// Notes: The returned data represents a 4x4 array in row-major order.
//
void MatrixGetArray(const Matrix3D* M, double array[MATRIX_ELEMENTS])
{
    if (array == NULL) {
        return;
    }

    int k = 0;
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            array[k++] = M->matrix[i][j];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixGetTransposedArray
//
// Purpose:    Retrieves transposed matrix contents as a single-dimensional
//             array of doubles.
//
// Parameters: 'M' is the matrix to get values from.
//             'array' returns an array of doubles with MATRIX_ELEMENTS elements.
//             Return value is error code.
//
// Notes: The returned data represents the transpose of a 4x4 array in row-major order.
// Alternatively, it represents the 4x4 array in column-major order.
//
void MatrixGetTransposedArray(const Matrix3D* M, double array[MATRIX_ELEMENTS])
{
    if (array == NULL) {
        return;
    }

    int k = 0;
    for (int i = 0; i < MATRIX_COLS; i++) {
        for (int j = 0; j < MATRIX_ROWS; j++) {
            array[k++] = M->matrix[j][i];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// Name:       MatrixPrint
//
// Purpose:    Prints the elements of a matrix.
//
// Parameters: 'M' is the matrix to print.
//
void MatrixPrint(const Matrix3D* M)
{
    printf("[");
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            printf("%7.4g ", M->matrix[i][j]);
        }
        if (i == MATRIX_ROWS-1)
            printf("]\n");
        else
            printf("\n ");
    }
}

