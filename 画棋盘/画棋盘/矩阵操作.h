#pragma once

typedef  double type;

class Matrix
{
public:

	Matrix(int row=3,int col=3,bool coeff=true);
	~Matrix();
	
	void makeTranslate( type &dx,  type &dy);

	void settransformparam(type tx, type ty) ;
	void setscale(type sx, type sy);
	void setsymmetry(type a, type b, type c, type d);
	void setrotate(type angle) ;
	void setshear(type b, type d);
	void sethomogeneous(type i);
	void setmatrix();
	void setrow(type x, type y, int homogeneous, int row);

	type getrow(int row);
	type getcol(int row);
	
	void operator*=(Matrix &a);
	friend Matrix operator*(Matrix&a, Matrix&b);
private:
	type * * matrix;
	int row, col;
};

