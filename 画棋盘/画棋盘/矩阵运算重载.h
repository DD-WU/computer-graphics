#pragma once

class Matrix
{

public:
	Matrix() {
		row = 3;
		col = 3;
		matrixl = new int*[row];
		for (int i = 0; i < row; i++) {
			matrixl[i] = new int[col];
		}
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++) {
				matrixl[i][j] = 0;
			}

		}
		matrixl[0][0] = matrixl[1][1] = matrixl[2][2] = 1;
	}
	Matrix(int m,int n) {
		row = m;
		col = n;
		matrixl = new int*[m];
		for (int i = 0; i < row; i++) {
			matrixl[i] = new int[col];
		}
	}
	friend int** operator+(  Matrix& a, Matrix& b);
	friend int** operator-( Matrix& a, Matrix& b);
	friend int** operator*(Matrix& a, Matrix& b);
	friend int** operator*(int a, Matrix& b);
	void setTraslation(int a, int b);
	void setmatrix();
	int** getmatrix() {
		return matrixl;
	};
	int getrow() {
		return row;
	};
	int getcol() {
		return col;
	};
private:
	int **matrixl;
	int x, y,row,col;
};

