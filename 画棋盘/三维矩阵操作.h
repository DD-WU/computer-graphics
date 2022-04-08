#pragma once
#include"stdafx.h"
#include"math.h"
#define M_PI 3.14159265358979323846
typedef double type;
int width = getWindowWidth(), height = getWindowHeight();
struct  pixel3D
{
	float x;
	float y;
	float z;
};
struct ET3D
{
	float x;
	int ymax;
	float k;
	float z;
	ET3D* next;
};

class Matrix4 {
public:
	Matrix4(int row, int col);
	void normalize();
	void project(type fovy, type aspectRatio, type zNear, type zFar);
	void UVN(Matrix4 up, Matrix4 eye1, Matrix4 center1);
	void operator*=(Matrix4 &a);
	void operator*=(double k);

	friend Matrix4 operator-(Matrix4 &a, Matrix4 &b);
	friend Matrix4 operator+(Matrix4 &a, Matrix4 &b);
	friend Matrix4 operator*(Matrix4&a, Matrix4&b);
	void setmatrix4();
	void settransform(type x,type y,type z);
	void setscale(type x,type y,type z);
	void setshear(type a, type b, type c, type d, type e, type f);
	void setsymmetry(type x, type y, type z);
	void setrotateZ(type angle);
	void setrotateX(type angle);
	void setrotateY(type angle);
	//正轴侧投影
	void setShaftside(type a, type b);
	void setrow(int i, type x, type y, type z, type homo);
	type getX(int i) {
		return matrix[i][0];
	}
	type getY(int i) {
		return matrix[i][1];
	}
	type getZ(int i) {
		return matrix[i][2];
	}
	int getRow() {
		return row;
	}
	int getCol() {
		return col;
	}
	static int comparemax(double **a/*[][2]*/, int n);
	//返回数组最小值
	static int comparemin(double **a/*[][2]*/, int n);
	Matrix4 chaCheng(Matrix4& a, Matrix4 &b) {
		Matrix4 c(1, 4);
		c.matrix[0][0] = a.matrix[0][1] * b.matrix[0][2] - a.matrix[0][2] * b.matrix[0][1];
		c.matrix[0][1] = a.matrix[0][2] * b.matrix[0][0] - a.matrix[0][0] * b.matrix[0][2];
		c.matrix[0][2] = a.matrix[0][0] * b.matrix[0][1] - a.matrix[0][1] * b.matrix[0][0];
		c.matrix[0][3] = 1;
		return c;
	};
	type dianCheng(Matrix4& a, Matrix4 &b) {
		type temp = a.matrix[0][0] * b.matrix[0][0] + a.matrix[0][1] * b.matrix[0][1] + a.matrix[0][2] * b.matrix[0][2];
		return temp;
	};
	void nomalized();
	void shikoubianhuan();
	static void zbuffer(int color, pixel3D b[100], int num, unsigned int**zhenbuffer, double**z_buffer);
private:
	type **matrix;
	int row, col;
};
int Matrix4::comparemin(double **a, int n) {
	int min = a[0][1];
	for (int i = 0; i < n; i++) {
		if (min >= a[i][1]) {
			min = a[i][1];
		}
	}
	return min;
}
int Matrix4::comparemax(double **a, int n) {
	int max = a[0][1];
	for (int i = 0; i < n; i++) {
		if (max <= a[i][1]) {
			max = a[i][1];
		}
	}
	return max;
}
void Matrix4::setrow(int i, type x, type y, type z, type homo){
	matrix[i][0] = x;
	matrix[i][1] = y;
	matrix[i][2] = z;
	matrix[i][3] = homo;
}
void Matrix4::zbuffer(int color, pixel3D b[100], int num, unsigned int**zhenbuffer, double**z_buffer) {
	double**a;//多边形顶点
	a = new double*[num];
	for (int i = 0; i < num; i++)
	{
		a[i] = new double[3];
		a[i][0] = 0;
		a[i][1] = 0;
		a[i][2] = 0;
	}
	for (int i = 0; i < num; i++) {
		a[i][0] = b[i].x;
		a[i][1] = b[i].y;
		a[i][2] = b[i].z;
	}
	double A = (a[1][1] - a[0][1])*(a[2][2] - a[0][2]) - (a[1][2] - a[0][2])*(a[2][1] - a[0][1]);
	double B = (a[2][0] - a[0][0])*(a[1][2] - a[0][2]) - (a[1][0] - a[0][0])*(a[2][2] - a[0][2]);
	double C = (a[1][0] - a[0][0])*(a[2][1] - a[0][1]) - (a[2][0] - a[0][0])*(a[1][1] - a[0][1]);
	double D = -1*(A * a[0][0] + B * a[0][1] + C * a[0][2]);
		if (num != 0) {
		int ymin = comparemin(a, num), ymax = comparemax(a, num);
		ET3D* et = (ET3D*)malloc((ymax - ymin) * sizeof(ET3D)), *phead = NULL, *ptail = NULL, *p = NULL;//边表
		memset(et, 0, (ymax - ymin) * sizeof(ET3D));
		for (int i = 0; i < (ymax - ymin); i++) {
			et[i].next = NULL;
		}//初始化
		ET3D* aet = NULL, *ahead = aet, *atail = aet;//活性表
													 //构造边表
		for (int y = ymin; y < ymax; y++)
		{
			phead = ptail = &et[y - ymin];
			//插入顶点
			for (int i = 0; i<num; i++) {
				if (y == (int)a[i][1]) {

					if (a[i][1]<a[(i - 1 + num) % num][1] && a[i][1]<a[(i + 1) % num][1]/*|| a[i][1]>=a[(i - 1 + num) % num][1] && a[i][1]>=a[(i + 1) % num][1]*/) {
						float temp1 = float(a[i][0] - a[(i - 1 + num) % num][0]) / float(a[i][1] - a[(i - 1 + num) % num][1]);
						float temp2 = float(a[i][0] - a[(i + 1) % num][0]) / float(a[i][1] - a[(i + 1) % num][1]);
						if (temp1 > temp2) {
							ptail->x = a[i][0];
							ptail->k = temp2;
							ptail->z = a[i][2];
							ptail->ymax = a[(i + 1) % num][1];
							p = (ET3D*)malloc(sizeof(ET3D));
							ptail->next = p;
							ptail = p;
							ptail->x = a[i][0];
							ptail->k = temp1;
							ptail->z = a[i][2];
							ptail->ymax = a[(i - 1 + num) % num][1];
						}
						else {
							ptail->x = a[i][0];
							ptail->k = temp1;
							ptail->z = a[i][2];
							ptail->ymax = a[(i - 1 + num) % num][1];
							p = (ET3D*)malloc(sizeof(ET3D));
							ptail->next = p;
							ptail = p;
							ptail->x = a[i][0];
							ptail->k = temp2;
							ptail->z = a[i][2];
							ptail->ymax = a[(i + 1) % num][1];
						}
					}
					else if (a[i][1]<a[(i - 1 + num) % num][1] && a[i][1] >= a[(i + 1) % num][1])
					{
						float temp1 = float(a[i][0] - a[(i - 1 + num) % num][0]) / float(a[i][1] - a[(i - 1 + num) % num][1]);
						ptail->x = a[i][0];
						ptail->k = temp1;
						ptail->z = a[i][2];
						ptail->ymax = a[(i - 1 + num) % num][1];
					}
					else if (a[i][1] >= a[(i - 1 + num) % num][1] && a[i][1] < a[(i + 1) % num][1]) {
						float temp2 = float(a[i][0] - a[(i + 1) % num][0]) / float(a[i][1] - a[(i + 1) % num][1]);
						ptail->x = a[i][0];
						ptail->k = temp2;
						ptail->z = a[i][2];
						ptail->ymax = a[(i + 1) % num][1];
					}

					p = (ET3D*)malloc(sizeof(ET3D));
					memset(p, 0, sizeof(ET3D));
					ptail->next = p;
					ptail = p;
				}
			}
			ptail->next = NULL;

		}

		//更新活性边表
		int count = 0;
		int y = ymin;
		aet = &et[0];//活性边表
		ahead = atail = aet;
		for (; y < ymax; y++) {
			//将活性边表中的数据取出填色
			for (; atail->next->ymax != 0; atail = atail->next) {
				if (count % 2 == 0) {
					if (atail->next != NULL) {
						for (int i = atail->x; i<atail->next->x; i++) {
							/*selfpixel(i, y, color);*/
							if (i > 0 && i < width &&y>0 && y < height) {
								double z = (-A * i - B * y - D) / C;
								//此处有问题，应计算每个像素处深度
								if (z<= z_buffer[y][i]) {
									zhenbuffer[y][i] = color;
									z_buffer[y][i] = z;
								}
							}
							
							
						}
					}
				}
				count++;
			}
			count = 0;
			atail = ahead;
			ET3D*ehead = NULL, *etail = NULL;
			/*更新活性边表*/
			while (y != ymin) {
				for (; atail != NULL; atail = atail->next) {
					atail->x += (atail->k);
					if (y == atail->ymax) {

						ET3D *q = ahead;
						if (q->ymax == atail->ymax) {
							if (q->ymax == atail->next->ymax) {
								atail = atail->next->next;
								ahead->next = NULL;
								ahead = atail;
							}
							else
							{
								atail = atail->next;
								ahead->next = NULL;
								ahead = atail;
							}

						}
						else {
							for (; q->next->ymax != atail->ymax; q = q->next) {

							}
							q->next = atail->next;
							atail->next = NULL;
							atail = q;
						}

					}

				}
				atail = ahead;
				if (et[y - ymin].ymax != NULL) {
					ehead = etail = &et[y - ymin];
					for (; ehead->next != NULL;) {
						for (; atail->ymax != 0 && etail->x >= atail->x; atail = atail->next) {

						}
						ET3D*q = ahead;
						if (atail == NULL) {
							ehead = ehead->next;

							atail->k = etail->k;
							atail->x = etail->x;
							atail->z = etail->z;
							atail->ymax = etail->ymax;
							p = (ET3D*)malloc(sizeof(ET3D));
							memset(p, 0, sizeof(ET3D));
							atail->next = p;
							p->next = NULL;
							etail->next = NULL;
							etail = ehead;
						}
						else {
							ehead = ehead->next;
							etail->next = atail;
							if (q->ymax == atail->ymax) {
								q = etail;
								ahead = q;
								etail = ehead;
							}
							else {
								for (; q->next->ymax != atail->ymax; q = q->next) {
								}
								q->next = etail;
								/*ahead = q;*/
								etail = ehead;
							}
						}
						atail = ahead;
					}
				}
				break;
			}



		}

	}

}
void Matrix4::shikoubianhuan() {
	for (int i = 0; i < row; i++) {
		matrix[i][0] = getWindowWidth()/2 * matrix[i][0];
	}
	for (int i = 0; i < row; i++)
	{
		matrix[i][1] =getWindowHeight()/2* matrix[i][1];
	}
};
void Matrix4::nomalized() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			matrix[i][j] /= matrix[i][3];
		}	
	}
};
void Matrix4::UVN(Matrix4 up, Matrix4 eye1, Matrix4 center1) {
	Matrix4 eye(1, 4);
	eye = eye1 - center1;
	
	Matrix4 U(1, 4);
	Matrix4 V(1, 4);
	U=chaCheng(up, eye);
	V = chaCheng(eye, U);
	U.normalize();
	V.normalize();
	eye.normalize();
	setmatrix4();
	matrix[0][0] = U.getX(0);
	matrix[0][1] = V.getX(0);
	matrix[0][2] = eye.getX(0);
	matrix[1][0] = U.getY(0);
	matrix[1][1] = V.getY(0);
	matrix[1][2] = eye.getY(0); 
	matrix[2][0] = U.getZ(0);
	matrix[2][1] = V.getZ(0);
	matrix[2][2] = eye.getZ(0);	
	matrix[3][0] = -dianCheng(eye1, U);
	matrix[3][1] = -dianCheng(eye1, V);
	matrix[3][2] = -dianCheng(eye1, eye);
};
void Matrix4::project(type fovy,type aspectRatio,type zNear,type zFar) {
	/*type r = getWindowWidth(), t = getWindowHeight(), l =0 , b = 0;*/
	type h, w;
	setmatrix4();
	h = 2 * zNear*tan(fovy / 180 * M_PI / 2.0);
	w = 2 * zNear*aspectRatio*tan(fovy / 180 * M_PI / 2.0);
	matrix[0][0] = 2 * zNear / w;
	matrix[1][1] = 2 * zNear / h;
	matrix[2][2] = -zFar / (zFar - zNear);
	matrix[2][3] = -1;
	matrix[3][2] = -zNear * zFar / (zFar - zNear);
	/*matrix[0][0] = 2 * zNear / (r - l);
	matrix[1][1] = 2 * zNear / (t - b);
	matrix[2][0] = -1 * (r + l) / (r - l);
	matrix[2][1] = -1 * (t + b) / (t - b);
	matrix[2][2] = -1 * zFar / (zFar - zNear);
	matrix[3][3] = 0;
	matrix[3][2] = -1 * zNear*zFar / (zFar - zNear);*/

}

Matrix4 operator*(Matrix4&a, Matrix4&b)
{
	Matrix4 c(4, 4);
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
Matrix4 operator-(Matrix4 &a, Matrix4 &b) {
	Matrix4 c(1, 4);
	for (int i = 0; i < a.row; i++) {
		for (int j = 0; j < a.col - 1; j++)
		{
			c.matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
		}
	}
	return c;
}
Matrix4 operator+(Matrix4 &a, Matrix4 &b) {
	Matrix4 c(1, 4);
	for (int i = 0; i < a.row; i++) {
		for (int j = 0; j < a.col - 1; j++)
		{
			c.matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
		}
	}
	return c;
};
void Matrix4::normalize() {
	for (int i = 0; i < row; i++) {
		double temp = sqrt((matrix[i][0] * matrix[i][0] + matrix[i][1] * matrix[i][1] + matrix[i][2] * matrix[i][2])*1.0);
		matrix[i][0] /= temp;
		matrix[i][1] /= temp;
		matrix[i][2] /= temp;
	}
	
};
void Matrix4::setShaftside(type a, type b) {
	matrix[0][0]= cos(a / 180 * M_PI *1.0);
	matrix[0][1] = -sin(a / 180 * M_PI *1.0)*sin(b / 180 * M_PI *1.0);
	matrix[1][1] = cos(b / 180 * M_PI *1.0);
	matrix[2][0] = -sin(a / 180 * M_PI *1.0);
	matrix[2][1] = -cos(a / 180 * M_PI *1.0)*sin(b / 180 * M_PI *1.0);
	matrix[2][2] = 0;
};
Matrix4::Matrix4(int row, int col)
{
	this->row = row;
	this->col = col;
	matrix = new type*[row];
	for (int i = 0; i < row; i++)
	{
		matrix[i] = new type[col];
	}
}
void Matrix4::setmatrix4() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
		{
			matrix[i][j] = 0;
		}
	}
	matrix[0][0] = matrix[1][1] = matrix[2][2] =matrix[3][3]= 1;
};
void Matrix4::settransform(type x, type y, type z) {
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;
}
void Matrix4::setscale(type x, type y, type z) {
	matrix[0][0] = x;
	matrix[1][1] = y;
	matrix[2][2] = z;
}
//C,E沿x错切，A,F沿y错切，B,D沿z错切
void Matrix4::setshear(type a, type b, type c, type d, type e, type f) {
	matrix[0][1] = a;
	matrix[0][2] = b;
	matrix[1][0] = c;
	matrix[1][2] = d;
	matrix[2][0] = e;
	matrix[2][1] = f;
}
void Matrix4::setsymmetry(type x, type y, type z) {
	matrix[0][0] = x;
	matrix[1][1] = y;
	matrix[2][2] = z;
}
void Matrix4::setrotateZ(type angle) {

	matrix[0][0] = cos(angle / 180 * M_PI *1.0);
	matrix[1][1] = cos(angle / 180 * M_PI*1.0);
	matrix[0][1] = sin(angle / 180 * M_PI*1.0);
	matrix[1][0] = -sin(angle / 180 * M_PI*1.0);
}
void Matrix4::setrotateX(type angle) {

	matrix[1][1] = cos(angle / 180 * M_PI *1.0);
	matrix[2][2] = cos(angle / 180 * M_PI*1.0);
	matrix[1][2] = sin(angle / 180 * M_PI*1.0);
	matrix[2][1] = -sin(angle / 180 * M_PI*1.0);
}
void Matrix4::setrotateY(type angle) {

	matrix[0][0] = cos(angle / 180 * M_PI *1.0);
	matrix[2][2] = cos(angle / 180 * M_PI*1.0);
	matrix[0][2] = -sin(angle / 180 * M_PI*1.0);
	matrix[2][0] = sin(angle / 180 * M_PI*1.0);
}
void Matrix4::operator*=(double k) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
		{
			matrix[i][j] *= k;
		}
	}
};
void Matrix4::operator*=(Matrix4 &a) {
	if (col == a.row) {
		type t[8][8] = {0};

		for (int i = 0; i < row; i++)
		{

			for (int j = 0; j < a.col; j++)
			{

				for (int k = 0; k < col; k++)
				{

					t[i][j] += (matrix[i][k] * a.matrix[k][j]);
				}

			}
		}
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < a.col; j++)
			{
				matrix[i][j] = t[i][j];
			}
		}

	}

};
