#include"¾ØÕó²Ù×÷.h"
#include"Graphic.h"
#include"math.h"
#include"stdafx.h"
#define M_PI 3.14159265358979323846
void Matrix::makeTranslate(type &dx, type &dy) {
	setOrig(dx + getWindowWidth() / 2, dy + getWindowHeight() / 2);
}
void Matrix::settransformparam(type tx, type ty) {
	matrix[2][0] = tx;
	matrix[2][1] = ty;
};
void Matrix::setscale(type sx, type sy) {
	matrix[0][0] = sx;
	matrix[1][1] = sy;
}
void Matrix::setsymmetry(type a, type b, type c, type d) {
	matrix[0][0] = a;
	matrix[0][1] = b;
	matrix[1][0] = c;
	matrix[1][1] = d;
}
void Matrix::setrotate(type angle) {
	matrix[0][0] = cos(angle / 180 * M_PI *1.0);
	matrix[1][1] = cos(angle / 180 * M_PI*1.0);
	matrix[0][1] = sin(angle / 180 * M_PI*1.0);
	matrix[1][0] = -sin(angle / 180 * M_PI*1.0);
}
void Matrix::setshear(type b, type d) {
	matrix[0][1] = d;
	matrix[1][0] = b;
	matrix[1][1] = 1;
	matrix[0][0] = 1;
}
void Matrix::sethomogeneous(type i) {
	matrix[2][2] = i;
}
void Matrix::setmatrix() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
		{
			matrix[i][j] = 0;
		}
	}
	matrix[0][0] = matrix[1][1] = matrix[2][2] = 1;
};
void Matrix::setrow(type x, type y, int homogeneous, int row) {
	matrix[row][0] = x;
	matrix[row][1] = y;
	matrix[row][2] = homogeneous;
};
type Matrix::getrow(int row) {
	return matrix[row][0];
};
type Matrix::getcol(int row) {
	return matrix[row][1];
};
Matrix::Matrix(int row, int col, bool coeff)
{
	this->row = row;
	this->col = col;
	matrix = new type*[row];
	for (int i = 0; i < row; i++)
	{
		matrix[i] = new type[col];
	}
}

void Matrix::operator*=(Matrix &a) {
	if (col == a.row) {
		double t[3][3];

		memset(t, 0, row*col * sizeof(double));


		for (int i = 0; i < row; i++)
		{

			for (int j = 0; j < a.col; j++)
			{

				for (int k = 0; k < col; k++)
				{

					t[i][j] += matrix[i][k] * a.matrix[k][j];
				}

			}
		}
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++)
			{
				matrix[i][j] = t[i][j];
			}
		}

	}

};
Matrix operator*(Matrix&a, Matrix&b)
{
	Matrix c(a.row, b.col, false);
	if (a.col == b.row) {
		for (int i = 0; i < a.row; i++)
		{
			for (int j = 0; j < b.col; j++)
			{
				for (int k = 0; k < a.col; k++)
				{
					c.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
				}
			}
		}
	}

	return c;
}
Matrix::~Matrix()
{

}

