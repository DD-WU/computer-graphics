
#include"�Զ��庯��.h"
#include"math.h"
#include"stdlib.h"
#include"string.h"
#include"������������.h"
#define PixelSelfDefine 10 //pixelselfdefine:�������̸�Ĵ�С

//�������߱Ƚϴ�С���ؽϴ�ֵ
inline int max(int a, int b) {
	int c = a > b ? a : b;
	return c;
} 

//�������߱Ƚϴ�С���ؽ�Сֵ
inline int min(int a, int b) {
	int c = a < b ? a : b;
	return c;
}
void zuobiaozhou() {
	for (int i = -getWindowHeight(); i < getWindowHeight(); i++)
	{
		setPixel(0,i,BLACK);
	}
	for (int i = -getWindowWidth(); i < getWindowWidth(); i++)
	{
		setPixel(i,0, BLACK);
	}
};
//�����̺���
void drawline() {
	int i, j, k, l, m, b;//i,l�ֱ����ڱ���y,x�ᣬj,k���ڼ�¼y,x�᳤�ȣ�m,b����ȷ�����̸��С
	k = getWindowHeight();
	j = getWindowWidth();
	for (i= -getWindowHeight(); i < k; i++) {
		for (l = -getWindowWidth(); l < j; l++) {
			m = abs(i) % (PixelSelfDefine);
			b = abs(l) % (PixelSelfDefine);
			if (i >= 0 && l >= 0||i<0&&l<0) {
				
				if (m<PixelSelfDefine/2 && b<PixelSelfDefine/2 || m >= PixelSelfDefine/2 && b >= PixelSelfDefine/2)
						setPixel(l, i, RED);	

			}
			else
			{
				if (m>=PixelSelfDefine / 2 && b<PixelSelfDefine / 2 || m < PixelSelfDefine / 2 && b >= PixelSelfDefine / 2)
					setPixel(l, i, RED);

			}
						
		}
	}
}
//��ֱ��DDA�㷨���ӣ�x0,y0��������x1,y1����ɫΪcolor
// x0:��ʼx����
//	y0����ʼy����
//	x1:��ֹx����
//	y1:��ֹy����
//	color����ɫ
void DDALine(int x0, int x1, int y0, int y1, int color) {//0,50,0,200
	
		int dx = (x1 - x0)*PixelSelfDefine / 2;//����ֱ�����Զ�����������ռ��250
		int dy = (y1 - y0)*PixelSelfDefine / 2;//����ֱ�����Զ�����������ռ��1000
		int epsl;int i;
		if (abs(dx) > abs(dy)) epsl = abs(dx);//б��С��1��epsl=dx
		else epsl = abs(dy);//б�ʴ���1��epsl=dy1000
		float xincre = (float) dx / (float)epsl;//����x����1/4
		float yincre = (float) dy/ (float)epsl;//����y����1
		float x = x0, y = y0;
		for ( i = 0; i <= epsl; i += PixelSelfDefine / 2) {
			selfpixel(x, y, color);
			x += xincre ;
			y += yincre ;
		}
	
	}
		
//�����̸��С�����صĺ�����������������ȷ����x,y�����ڵ����̸�����
//x:���x����
//y:���y����
void selfpixel(int x, int y, int color) {
	
		int temp1 = int(x) % (PixelSelfDefine / 2);//x�ȶ�Ӧ���̸�����x������
		int temp2 = int(y) % (PixelSelfDefine / 2);//y�ȶ�Ӧ���̸�����y������
												   //���ݲ�ͬ���������������
		if (temp1 > (PixelSelfDefine / 4) && temp2> (PixelSelfDefine / 4)) {
			int tempx = x + ((PixelSelfDefine / 2) - temp1);
			int tempy = y + ((PixelSelfDefine / 2) - temp2);
			for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
				for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
					setPixel(j, k, color);
				}
			}
		}
		else if (temp1 <= (PixelSelfDefine / 4) && temp2 <= (PixelSelfDefine / 4)) {
			int tempx = x - temp1;
			int tempy = y - temp2;
			for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
				for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
					setPixel(j, k, color);
				}
			}
		}
		else if (temp1 >(PixelSelfDefine / 4) && temp2 <= (PixelSelfDefine / 4)) {
			int tempx = x + ((PixelSelfDefine / 2) - temp1);
			int tempy = y - temp2;
			for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
				for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
					setPixel(j, k, color);
				}
			}
		}
		else {
			int tempx = x - temp1;
			int tempy = y + ((PixelSelfDefine / 2) - temp2);
			for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
				for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
					setPixel(j, k, color);
				}
			}
		}

	

	
}
//����һ����������ͬ������ǰ���н���x,y���ֱ��PixelSelfDefineforcirclex��PixelSelfDefineforcircley��
//Ϊ�˽�Բƽ�Ƶ���Ļ��
void selfpixelforCircle(int x, int y, int color, PixelPoint &p) {
	x +=  p.x;
	y +=  p.y;
	int temp1 = int(x) % (PixelSelfDefine / 2);
	int temp2 = int(y) % (PixelSelfDefine / 2);
	if (temp1 >(PixelSelfDefine / 4) && temp2> (PixelSelfDefine / 4)) {
		int tempx = x + ((PixelSelfDefine / 2) - int(x) % (PixelSelfDefine / 2));
		int tempy = y + ((PixelSelfDefine / 2) - int(y) % (PixelSelfDefine / 2));
		for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
			for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
				setPixel(j, k, color);
			}
		}
	}
	else if (temp1 <= (PixelSelfDefine / 4) && temp2 <= (PixelSelfDefine / 4)) {
		int tempx = x - (int(x) % (PixelSelfDefine / 2));
		int tempy = y - (int(y) % (PixelSelfDefine / 2));
		for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
			for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
				setPixel(j, k, color);
			}
		}
	}
	else if (temp1 >(PixelSelfDefine / 4) && temp2 <= (PixelSelfDefine / 4)) {
		int tempx = x + ((PixelSelfDefine / 2) - int(x) % (PixelSelfDefine / 2));
		int tempy = y - (int(y) % (PixelSelfDefine / 2));
		for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
			for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
				setPixel(j, k, color);
			}
		}
	}
	else {
		int tempx = x - (int(x) % (PixelSelfDefine / 2));
		int tempy = y + ((PixelSelfDefine / 2) - int(y) % (PixelSelfDefine / 2));
		for (int j = tempx; j < tempx + PixelSelfDefine / 2; j++) {
			for (int k = tempy; k < tempy + PixelSelfDefine / 2; k++) {
				setPixel(j, k, color);
			}
		}
	}

}
//��ֱ��midbresenham�㷨���ӣ�x0,y0��������x1,y1����ɫΪcolor
// x0:��ʼx����
//	y0����ʼy����
//	x1:��ֹx����
//	y1:��ֹy����
//	color����ɫ
void MidBresenham(int x0,int x1,int y0,int y1,int color) {
	int  dx, dy,x, y, d, upincre, downincre;
	if (x0 > x1) {
		
		x = x1; x1 = x0; x0 = x;
		
		y = y1; y1 = y0; y0 = y;
		
	  }
	dx = (x1 - x0)*PixelSelfDefine / 2;
	dy = (y1 - y0)*PixelSelfDefine / 2;
	float k = float(dy) /(float) dx;
	
	if(dx>dy){
		
	  x = x0; y = y0;

		if (k > 0)
		{
			d = dx - 2 * dy;
		}
		else
		{
			d = -dx - 2 * dy;
		}

		if (k > 0)
		{
			upincre = 2 * dx - 2 * dy;
		}
		else
		{
			upincre = -2 * dx - 2 * dy;
		}

		downincre = -2 * dy;

		while (x <= x1) {

			selfpixel(x, y, color);

			x += PixelSelfDefine / 2;
			if (k > 0) {
				if (d < 0) {

					y += PixelSelfDefine / 2;

					d += upincre;

				}

				else {

					d += downincre;

				}

			}
			else if (k < 0)
			{

				if (d > 0) {

					y -= PixelSelfDefine / 2;

					d += upincre;

				}

				else {

					d += downincre;

				}

			}

		}
	}
	else
	{
		x = x0; y = y0;

		if (k > 0)
		{
			d = dy - 2 * dx;
		}
		else
		{
			d = -dy - 2 * dx;
		}

		if (k > 0)
		{
			upincre = 2 * dy - 2 * dx;
		}
		else
		{
			upincre = -2 * dy - 2 * dx;
		}

		downincre = -2 * dx;
		if (k > 0) {
			while (y <= y1) {

				selfpixel(x, y, color);

				y += PixelSelfDefine / 2;
				if (k > 0) {
					if (d < 0) {

						x += PixelSelfDefine / 2;

						d += upincre;

					}

					else {

						d += downincre;

					}

				}
				else if (k < 0)
				{

					if (d > 0) {

						x -= PixelSelfDefine / 2;

						d += upincre;

					}

					else {

						d += downincre;

					}

				}

			}
		}
		else
		{
			while (y >= y1) {

				selfpixel(x, y, color);

				y -= PixelSelfDefine / 2;
				if (k < 0)
				{

					if (d > 0) {

						x += PixelSelfDefine / 2;

						d -= upincre;

					}

					else {

						d += downincre;

					}

				}

			}
		}
	
	
	}	


}
//��ֱ��bresenham�㷨���ӣ�x0,y0��������x1,y1����ɫΪcolor
// x0:��ʼx����
//	y0����ʼy����
//	x1:��ֹx����
//	y1:��ֹy����
//	color����ɫ
void Bresenham(int x0,int x1,int y0,int y1,int color){
	int x, y;
	if (x0 > x1) {

		x = x1; x1 = x0; x0 = x;

		y = y1; y1 = y0; y0 = y;

	}
	int dx = x1 - x0,dy = y1 - y0,e;
	float k = (float)dy / (float)dx;
	if (dx>dy)
	{
		
		k>0?e = -dx:e=dx;
		x = x0, y = y0;
		while (x < x1) {
				selfpixel(x, y, color);
				x += PixelSelfDefine / 2;
				e += 2*dy;
				if (k > 0) {
					if (e > 0) {
						y += PixelSelfDefine / 2;
						e -= 2 * dx;
					}
				}
				else
				{
					if (e < 0) {
						y -= PixelSelfDefine / 2;
						e += 2 * dx;
					}
				}
			
		}
	}
	else
	{
		k>0 ? e = -dy:e=dy;
		x = x0, y = y0;
		while (/*y < y1*/x<x1) {
			selfpixel(x, y, color);
			k>0?y += PixelSelfDefine / 2: y -= PixelSelfDefine / 2;
			e += 2 * dx;
			if (k > 0) {
				if (e > 0) {
					x += PixelSelfDefine / 2;
					e -= 2 * dy;
				}
			}
			else
			{
				if (e < 0) {
					x += PixelSelfDefine / 2;
					e += 2 * dy;
				}
			}
			
		}
	}
	
}
//��Բ�˷ַ�������Բ����Ļ�У�
//x1:Բ��x����
//y1 : Բ��y����
//color:�����ɫ
void circlePoint(int x1, int y1, int color, PixelPoint &p) {
	int r = sqrt(((x1-p.x)*(x1 - p.x) + (y1-p.y) *( y1-p.y))*1.0);
	int x = 0,y=r;
	for(int i=0;i<=r*sin(45);i++){
		selfpixel(x+ p.x, y + p.y, color); selfpixel(y + p.x, x + p.y, color);
		selfpixel(x + p.x, -y + p.y, color); selfpixel(-y + p.x, x + p.y, color);
		selfpixel(-x + p.x, y + p.y, color); selfpixel(y + p.x, -x + p.y, color);
		selfpixel(-x + p.x, -y + p.y, color); selfpixel(-y + p.x, -x + p.y, color);
		x+=PixelSelfDefine/2;
		y = sqrt((r*r-x*x)*1.0);
	}
}
//��Բmidbresenham��������Բ����Ļ�У�
//x1:Բ��x����
//y1 : Բ��y����
//color:�����ɫ
void MidBresenhamCircle(int r,int color,PixelPoint &p){
	int x= 0, y=r, d;
	d = 1 - r;
	while (x <= y) {
		circlePointDraw(x,y,color,p);
		if (d < 0)
			d += PixelSelfDefine* x + 3/4*PixelSelfDefine;
		else {
			d += PixelSelfDefine * x + 3 / 4 * PixelSelfDefine+PixelSelfDefine/2-PixelSelfDefine*y;
			y -= PixelSelfDefine / 2;
		}
		x += PixelSelfDefine / 2;
	}
}
//���ݰ˷�֮һԲ����ʣ�µ�Բ
//x:Բ��x����
//y : Բ��y����
//color:�����ɫ
void circlePointDraw(int x, int y, int color, PixelPoint &p) {
	selfpixel(x + p.x, y + p.y, color); selfpixel(y + p.x, x + p.y, color);
	selfpixel(x + p.x, -y + p.y, color); selfpixel(-y + p.x, x + p.y, color);
	selfpixel(-x + p.x, y + p.y, color); selfpixel(y + p.x, -x + p.y, color);
	selfpixel(-x + p.x, -y + p.y, color); selfpixel(-y + p.x, -x + p.y, color);
}
//����Բ����
//a:������
//b:�̰���
//color:��ɫ
void MidBresenhamEllipse(int a,int b,int color, PixelPoint &p){
	int x = 0, y = b;
	float d1, d2;
	if (x%PixelSelfDefine / 2 == 0 ) {
		d1 = b * b*PixelSelfDefine/2 + a * a*(PixelSelfDefine/16 - b*PixelSelfDefine/2);
		selfpixelforCircle(x, y, color,p);
		selfpixelforCircle(-x, y, color,p);
		selfpixelforCircle(x, -y, color,p);
		selfpixelforCircle(-x, -y, color,p);
		while (b*b*(x + PixelSelfDefine / 2) < a*a*(y - PixelSelfDefine / 4)) {
			if (d1 <= 0) {
				d1 += b * b*(PixelSelfDefine*x + 3 / 4 * PixelSelfDefine);
				x += PixelSelfDefine / 2;
			}
			else {
				d1 += b * b*(PixelSelfDefine*x + 3 / 4 * PixelSelfDefine) + a * a*(-PixelSelfDefine * y + PixelSelfDefine / 2);
				x += PixelSelfDefine / 2;
				y -= PixelSelfDefine / 2;
			}
			selfpixelforCircle(x, y, color,p);
			selfpixelforCircle(-x, y, color,p);
			selfpixelforCircle(x, -y, color,p);
			selfpixelforCircle(-x, -y, color,p);
		}
			d2 = b * b*(x + PixelSelfDefine / 4)*(x + PixelSelfDefine / 4) + a * a*(y - PixelSelfDefine / 2)*(y - PixelSelfDefine / 2) - a * a*b*b;
			while (y > 0) {
				if(d2<=0){
					d2 += b * b * (PixelSelfDefine* x + PixelSelfDefine/2) + a * a*(3 / 4 * PixelSelfDefine - PixelSelfDefine * y);
					x += PixelSelfDefine / 2;
					y -= PixelSelfDefine / 2;

				}
				else {
					d2 += a * a*(3/4*PixelSelfDefine - PixelSelfDefine * y);
					y -= PixelSelfDefine / 2;
				}
				selfpixelforCircle(x, y, color, p);
				selfpixelforCircle(-x, y, color, p);
				selfpixelforCircle(x, -y, color, p);
				selfpixelforCircle(-x, -y, color, p);
			}

	}
	else {
		d1 = b * b*PixelSelfDefine / 2 + a * a*(PixelSelfDefine / 16 - b * PixelSelfDefine / 2);
		selfpixelforCircle(x, y, color, p);
		selfpixelforCircle(-x, y, color, p);
		selfpixelforCircle(x, -y, color, p);
		selfpixelforCircle(-x, -y, color, p);
		while (b*b*(x + PixelSelfDefine / 2) < a*a*(y - PixelSelfDefine / 4)) {
			if (d1 <= 0) {
				d1 += b * b*(PixelSelfDefine*x + 3 / 4 * PixelSelfDefine);
				x += PixelSelfDefine / 2;
			}
			else {
				d1 += b * b*(PixelSelfDefine*x + 3 / 4 * PixelSelfDefine) + a * a*(-PixelSelfDefine * y + PixelSelfDefine / 2);
				x += PixelSelfDefine / 2;
				y -= PixelSelfDefine / 2;
			}
			selfpixelforCircle(x, y, color, p);
			selfpixelforCircle(-x, y, color, p);
			selfpixelforCircle(x, -y, color, p);
			selfpixelforCircle(-x, -y, color, p);
		}
		d2 = b * b*(x + PixelSelfDefine / 4)*(x + PixelSelfDefine / 4) + a * a*(y - PixelSelfDefine / 2)*(y - PixelSelfDefine / 2) - a * a*b*b;
		while (y > PixelSelfDefine/2) {
			if (d2 <= 0) {
				d2 += b * b * (PixelSelfDefine* x + PixelSelfDefine / 2) + a * a*(3 / 4 * PixelSelfDefine - PixelSelfDefine * y);
				x += PixelSelfDefine / 2;
				y -= PixelSelfDefine / 2;

			}
			else {
				d2 += a * a*(3 / 4 * PixelSelfDefine - PixelSelfDefine * y);
				y -= PixelSelfDefine / 2;
			}
			selfpixelforCircle(x, y, color, p);
			selfpixelforCircle(-x, y, color, p);
			selfpixelforCircle(x, -y, color, p);
			selfpixelforCircle(-x, -y, color, p);
		}
	}
	
}
//�����������ڶ���ν�������
void quicksort(int a[][2], int left, int right) {
	if(left>=right){
		return;
	}
	int i=left, j=right-1,kx=a[i][0],ky=a[i][1];
	while (i < j) {
		while (i < j&&kx <= a[j][0]) {
			j--;
		}
		a[i][0] = a[j][0];
		a[i][1] = a[j][1];
		while (i < j&&kx >= a[i][0]) {
			i++;
		}
		a[j][0] = a[i][0];
		a[j][1] = a[i][1];
	}
	a[i][0] = kx;
	a[i][1] = ky;
	quicksort(a, left, i - 1);
	quicksort(a, j + 1, right);
}
//�����������ֵ
int comparemax(int **a/*[][2]*/,int n) {
	int max = a[0][1];
	for (int i = 0; i < n; i++) {
		if (max <= a[i][1]) {
			max = a[i][1];
		}
	}
	return max;
}
//����������Сֵ
int comparemin(int **a/*[][2]*/, int n) {
	int min = a[0][1];
	for (int i = 0; i < n; i++) {
		if (min >= a[i][1]) {
			min = a[i][1];
		}
	}
	return min;
}
//���������㷨֮xɨ���߷�
//color����ɫ
void x_ray(int color,PixelPoint b[100],int num) {
	int**a;//����ζ���
	a = new int*[num];
	for (int i = 0; i < num; i++)
	{
		a[i] = new int[2];
		a[i][0] = 0;
		a[i][1] = 0;

	}
	for (int i = 0; i < num; i++) {
		a[i][0] = b[i].x;
		a[i][1] = b[i].y;

	}

	int ymin, ymax,y;
	int count = 0;//A���ڴ洢���㣬count������ÿ��ż����ʼ���
	if (num != 0) {
		ymin = comparemin(a, num);
		ymax = comparemax(a, num);
		for (y = ymin; y < ymax; y+=PixelSelfDefine/2) {//ɨ��
			int A[100][2];
			for (int i = 0; i<num; i++) {//��ȡ����
										 //ɨ��������ཻ�������洢
				if (y > min(a[i][1], a[(i + 1) % num][1]) && y<max(a[i][1], a[(i + 1) % num][1])) {
					A[count][0] = a[i][0] + (a[(i + 1) % num][0] - a[i][0])*(y - a[i][1]) / (a[(i + 1) % num][1] - a[i][1]);
					A[count++][1] = y;

				}
				//ɨ�����붥���ཻ�������ѡ������λ��һ��
				else if (y == a[i][1]) {
					if (y > a[(i - 1 + num) % num][1] && y > a[(i + 1) % num][1] || y < a[(i - 1 + num) % num][1] 
						&& y < a[(i + 1) % num][1]) {
						A[count][0] = a[i][0];
						A[count++][1] = y;
						A[count][0] = a[i][0];
						A[count++][1] = y;
					}
					else if (y > a[(i - 1 + num) % num][1] && y < a[(i + 1) % num][1] || y < a[(i - 1 + num) % num][1]
						&& y > a[(i + 1) % num][1])
					{
						A[count][0] = a[i][0];
						A[count++][1] = y;
					}
				}

			}
			
			//��������
			quicksort(A, 0, count);
			//���
			for (int k = 0; k < 100; k++) {
				if (k % 2 == 0) {
					for (int j = A[k][0]; j <= A[(k + 1) % 100][0]; j += PixelSelfDefine / 2) {
						selfpixel(j, A[k][1], color);
					}
				}
			}
			count = 0;
		}
	}
		
	
	
}
//���������㷨֮��Ч�߱�
//color����ɫ
void AET(int color,PixelPoint b[100], int num) {
	//int a[6][2] = { { 10,190 },{ 54,146 },{ 20,13 },{ 90,56 },{ 149,10 },{ 231,220 } };//����
	int**a;//����ζ���
	a = new int*[num];
	for (int i = 0; i < num; i++)
	{
		a[i] = new int[2];
		a[i][0] = 0;
		a[i][1] = 0;

	}
	for (int i = 0; i < num; i++) {
		a[i][0] = b[i].x;
		a[i][1] = b[i].y;

	}
	if (num != 0) {
		int ymin = comparemin(a, num), ymax = comparemax(a, num);
		ET* et = (ET*)malloc((ymax - ymin) * sizeof(ET)), *phead = NULL, *ptail = NULL, *p = NULL;//�߱�
		memset(et, 0, (ymax - ymin) * sizeof(ET));
		for (int i = 0; i < (ymax - ymin); i++) {
			et[i].next = NULL;
		}//��ʼ��
		ET* aet=NULL, *ahead = aet, *atail = aet;//���Ա�
		//����߱�
		for (int y = ymin; y < ymax; y++)
		{
			phead = ptail = &et[y - ymin];
			//���붥��
			for (int i = 0; i<num; i++) {
				if (y == a[i][1]) {

					if (a[i][1]<a[(i - 1 + num) % num][1] && a[i][1]<a[(i + 1) % num][1]/*|| a[i][1]>=a[(i - 1 + num) % num][1] && a[i][1]>=a[(i + 1) % num][1]*/) {
						float temp1 = float(a[i][0] - a[(i - 1 + num) % num][0]) / float(a[i][1] - a[(i - 1 + num) % num][1]);
						float temp2 = float(a[i][0] - a[(i + 1) % num][0]) / float(a[i][1] - a[(i + 1) % num][1]);
						if (temp1 > temp2) {
							ptail->x = a[i][0];
							ptail->k = temp2;
							ptail->ymax = a[(i + 1) % num][1];
							p = (ET*)malloc(sizeof(ET));
							ptail->next = p;
							ptail = p;
							ptail->x = a[i][0];
							ptail->k = temp1;
							ptail->ymax = a[(i - 1 + num) % num][1];
						}
						else {
							ptail->x = a[i][0];
							ptail->k = temp1;
							ptail->ymax = a[(i - 1 + num) % num][1];
							p = (ET*)malloc(sizeof(ET));
							ptail->next = p;
							ptail = p;
							ptail->x = a[i][0];
							ptail->k = temp2;
							ptail->ymax = a[(i + 1) % num][1];
						}
					}
					else if (a[i][1]<a[(i - 1 + num) % num][1] && a[i][1] >= a[(i + 1) % num][1])
					{
						float temp1 = float(a[i][0] - a[(i - 1 + num) % num][0]) / float(a[i][1] - a[(i - 1 + num) % num][1]);
						ptail->x = a[i][0];
						ptail->k = temp1;
						ptail->ymax = a[(i - 1 + num) % num][1];
					}
					else if (a[i][1] >= a[(i - 1 + num) % num][1] && a[i][1] < a[(i + 1) % num][1]) {
						float temp2 = float(a[i][0] - a[(i + 1) % num][0]) / float(a[i][1] - a[(i + 1) % num][1]);
						ptail->x = a[i][0];
						ptail->k = temp2;
						ptail->ymax = a[(i + 1) % num][1];
					}

					p = (ET*)malloc(sizeof(ET));
					memset(p, 0, sizeof(ET));
					ptail->next = p;
					ptail = p;
				}
			}
			ptail->next = NULL;

		}
		//���»��Ա߱�
		int count = 0;
		int y = ymin;
		aet = &et[0];//���Ա߱�
		ahead = atail = aet;
		for (; y < ymax; y++) {
			//�����Ա߱��е�����ȡ����ɫ
			for (; atail->next->ymax != 0; atail = atail->next) {
				if (count % 2 == 0) {
					if (atail->next != NULL) {
						for (int i = atail->x; i<atail->next->x; i++) {
							selfpixel(i, y, color);
						}
					}
				}
				count++;
			}
			count = 0;
			atail = ahead;
			ET*ehead = NULL, *etail = NULL;
			/*���»��Ա߱�*/
			while (y != ymin) {
				for (; atail != NULL; atail = atail->next) {
					atail->x += (atail->k);
					if (y == atail->ymax) {

						ET *q = ahead;
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
						ET*q = ahead;
						if (atail == NULL) {
							ehead = ehead->next;
							
							atail->k = etail->k;
							atail->x = etail->x;
							atail->ymax = etail->ymax;
							p = (ET*)malloc(sizeof(ET));
							memset(p, 0, sizeof(ET));
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
//������㷨
//��x,y��Ϊ���ӵ�����
//oldcolor:ԭɫ
//newcolor:����ɫ
void FloodFill4(int x, int y,unsigned oldColor, unsigned newColor)
{
	if (getPixel(x, y) == oldColor)
	{
		setPixel(x, y, newColor);
		FloodFill4(x, y + 1, oldColor, newColor);
		FloodFill4(x, y - 1, oldColor, newColor);
		FloodFill4(x - 1, y, oldColor, newColor);
		FloodFill4(x + 1, y, oldColor, newColor);
	}
	
}
//�ǵݹ�����㷨
//��x,y��Ϊ���ӵ�����
//oldcolor:ԭɫ
//newcolor:����ɫ
void areaNoRecursion(int x, int y, unsigned newcolor, unsigned edgecolor) {
	
	int x0=x, y0=y;
	int front = 0, rear = 0;//front:ջ����rear:ջ��
	seed p[50000];//ջ
	p[front].x = x0;
	p[front].y = y0;
	front++;
	while (front != rear) {
		front--;
		while (getPixel(p[front].x, p[front].y) == newcolor)//��֮ǰ������ɫ��Ԫ��ȥ��
		{
			front--;
		}
		setPixel(p[front].x, p[front].y, newcolor);//��ɫ
		int temp=front;
		//������ڽ����أ��������ڽ���������������������ѹջ
		if (getPixel(p[temp].x - 1, p[temp].y) != edgecolor && getPixel(p[temp].x - 1, p[temp].y) != newcolor)
		{
			front++;
			p[front].x = p[temp ].x - 1;
			p[front].y = p[temp ].y;
			
		}
		if (getPixel(p[temp].x+1,p[temp].y)!=edgecolor&& getPixel(p[temp].x + 1, p[temp].y)!=newcolor)
		{
			front++;
			p[front].x = p[temp ].x + 1;
			p[front].y = p[temp ].y;
			
		}
		if (getPixel(p[temp].x , p[temp].y+ 1) != edgecolor && getPixel(p[temp].x , p[temp].y+ 1) != newcolor)
		{
			front++;
			p[front].x = p[temp ].x ;
			p[front].y = p[temp ].y + 1;
			
		}
		if (getPixel(p[temp].x , p[temp].y- 1) != edgecolor && getPixel(p[temp].x , p[temp].y- 1) != newcolor)
		{
			front++;
			p[front].x = p[temp ].x;
			p[front].y = p[temp ].y - 1;
			
		}
		front++;
		
	}
	
	
}
//������Ϊɨ���߷��������
//x,yΪ���ӵ����꣬newcolorΪ���ɫ��edgecolorΪԭ����ɫ
void scanningline(int x, int y, unsigned newcolor, unsigned edgecolor) {

	int xl, xr;
	bool spanNeedFill;
	Seed* p=(Seed*)malloc(sizeof(Seed)),*phead=p,*ptail=p;
	int front = 0, rear = 0;
	p->x= x;
	p->y = y;
	p->next = NULL;
	while (phead!=-NULL)
	{
		p = phead;
		phead = phead->next;
		p->next = NULL;
		y = p->y;

		x = p->x;
		while (getPixel(x, y) == edgecolor)//�������
		{
			setPixel(x, y, newcolor);
			x++;
		}
		xr = x - 1;

		x = p->x - 1;
		while (getPixel(x, y) == edgecolor) //�������
		{
			setPixel(x, y, newcolor);
			x--;
		}
		xl = x + 1;



		//������һ��ɨ����
		x = xl;
		y = y + 1;
		while (x < xr)
		{
			spanNeedFill = false;
			while (getPixel(x, y) == edgecolor)
			{
				spanNeedFill = true;
				x++;
			}
			if (spanNeedFill)
			{
				p = (Seed*)malloc(sizeof(Seed));
				p->x = x - 1;
				p->y = y;
				p->next = phead;
				phead = p;
				spanNeedFill = false;
			}
			while (getPixel(x, y) != edgecolor && x < xr)
				x++;
		}
		//������һ��ɨ���ߣ������봦����һ��ɨ��������
		x = xl;
		y = y - 2;

		while (x < xr)
		{
			spanNeedFill = false;
			while (getPixel(x, y) == edgecolor)
			{
				spanNeedFill = true;
				x++;
			}
			if (spanNeedFill)
			{

				p = (Seed*)malloc(sizeof(Seed));
				p->x = x - 1;
				p->y = y;
				p->next = phead;
				phead = p;
				spanNeedFill = false;
			}
			while (getPixel(x, y) != edgecolor && x < xr)
				x++;
		}
	}
}

