#pragma once
#include"Graphic.h"
#include"vector"
#include<cmath>
using namespace std;
#define eps 1e-8
#define zero(x) (((x)>0?(x):-(x))<eps)
//计算交叉乘积(P1-P0)x(P2-P0)
double xmult(PixelPoint p1, PixelPoint p2, PixelPoint p0) {
	return (p1.x - p0.x)*(p2.y - p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
}
//判点是否在线段上,包括端点
int dot_online_in(PixelPoint p, PixelPoint l1, PixelPoint l2) {
	return zero(xmult(p, l1, l2)) && (l1.x - p.x)*(l2.x - p.x)<eps && (l1.y - p.y)*(l2.y - p.y)<eps;
}
//判两点在线段同侧,点在线段上返回0
int same_side(PixelPoint p1, PixelPoint p2, PixelPoint l1, PixelPoint l2) {
	return xmult(l1, p1, l2)*xmult(l1, p2, l2)>eps;
}
//判两直线平行
int parallel(PixelPoint u1, PixelPoint u2, PixelPoint v1, PixelPoint v2) {
	return zero((u1.x - u2.x)*(v1.y - v2.y) - (v1.x - v2.x)*(u1.y - u2.y));
}
//判三点共线
int dots_inline(PixelPoint p1, PixelPoint p2, PixelPoint p3) {
	return zero(xmult(p1, p2, p3));
}
//判两线段相交,包括端点和部分重合
int intersect_in(PixelPoint u1, PixelPoint u2, PixelPoint v1, PixelPoint v2) {
	if (!dots_inline(u1, u2, v1) || !dots_inline(u1, u2, v2))
		return !same_side(u1, u2, v1, v2) && !same_side(v1, v2, u1, u2);
	return dot_online_in(u1, v1, v2) || dot_online_in(u2, v1, v2) || dot_online_in(v1, u1, u2) || dot_online_in(v2, u1, u2);
}
//计算两线段交点,请判线段是否相交(同时还是要判断是否平行!)
PixelPoint intersection(PixelPoint p1, PixelPoint p2, PixelPoint q1, PixelPoint q2) {
	PixelPoint ret = p1;
	/*double t = ((u1.x - v1.x)*(v1.y - v2.y) - (u1.y - v1.y)*(v1.x - v2.x))
		/ ((u1.x - u2.x)*(v1.y - v2.y) - (u1.y - u2.y)*(v1.x - v2.x));
	ret.x += (u2.x - u1.x)*t;
	ret.y += (u2.y - u1.y)*t;*/
	long tmpLeft, tmpRight;
	tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
    tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
	int x, y;
	x = (int)((double)tmpRight / (double)tmpLeft);
	
	tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
	tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
	y = (int)((double)tmpRight / (double)tmpLeft);
	ret.x = x;
	ret.y = y;
	return ret;
}

void Ptailor(PixelPoint a[100], PixelPoint b[100],int num,int num1) {
	
	int count1=0,count2=0,tnum=num,tnum1=num1;
	vector<PixelPoint> d, e;
	d.reserve(100);
	e.reserve(100);
	for (int i = 0; i < num; i++) {
		d.push_back(a[i]);

	}
	for (int i = 0; i < num1; i++) {
		e.push_back(b[i]);
	}
	for (int i = 0; i < num; i++)
	{
		
		for (int j = 0; j < num1; j++)
		{
			
			if (!intersect_in(a[i],a[(i+1)%num],b[j],b[(j+1)%num1])) {

			}
			else
			{
				
					
					
					PixelPoint f1= intersection(a[i],  a[(i + 1) % num],b[j], b[(j + 1) % num1]); 
					
					count1++;
					if (count1 % 2 == 1) {
						f1.judge = 入;
					}
					else
					{
						f1.judge = 出;
					}
					int temp1 = d.size();
					auto d1 = d.begin();
					int ti = 0, tj = 0;
					while (ti != i + temp1 - num + 1)
					{
						d1++;
						ti++;
					}

					d.insert(d1, f1);
					

			}
		}		
	}
	for (int i = 0; i < num1; i++)
	{

		for (int j = 0; j < num; j++)
		{
			
			if (!intersect_in(b[i], b[(i + 1) % num1], a[j], a[(j + 1) % num])) {

			}
			else
			{
				


				PixelPoint f1 = intersection(b[i], b[(i + 1) % num1], a[j], a[(j + 1) % num]);

				count2++;
				if (count2 % 2 == 1) {
					f1.judge = 出;
				}
				else
				{
					f1.judge =  入;
				}
				int ti = 0, tj = 0;
				
				int temp2 = e.size();
				auto e1 = e.begin();
				while (tj != i + temp2 - num1 + 1)
				{
				e1++;
				tj++;
				}

				e.insert(e1, f1);
				


			}
		}
	}
	for (int i = 0; i < d.size(); i++) {
		a[i].x = d[i].x;
		a[i].y = d[i].y;
		a[i].judge = d[i].judge;
		
	}
	num = d.size();
	for (int i = 0; i < e.size(); i++) {
		b[i].x = e[i].x;
		b[i].y = e[i].y;
		b[i].judge = e[i].judge;
		
	}
	num1 = e.size();
	int temp,l1=0;
	PixelPoint s,f1;
	vector<PixelPoint> point;
	vector<PixelPoint>::iterator p = e.begin();
	vector<PixelPoint>::iterator q = d.begin();
	while ((*p).judge!=入)
	{
		p++;
	}
	s = (*p);
	while (p!=e.end())
	{
		PixelPoint f = *p;
		if(f.judge==入){
			point.push_back(f);
			(*p).judge = 无;
			p++;
			l1++;
		}
		else
		{
			while (f.judge != 出)
			{

				point.push_back(f);
				p++;
				f = *p;
				
			}
			f1 = *q;
			while ((*q).x != f.x || (*q).y != f.y || (*q).judge != f.judge)
			{
				q++;
				f1 = *q;
			}
			while (f1.judge != 入)
			{
				point.push_back(f1);
				q++;
				f1 = *q;
			}
			if (f1.x != s.x || f1.y != s.y || f1.judge != s.judge) {
				
				while ((*p).x != f1.x || (*p).y != f1.y || (*p).judge != f1.judge)
				{
					p++;
					f = *p;
				}
				continue;
			}
		}

			p++;


	}
			/*if (l1 %2== 1) {
				PixelPoint f1 = *p;
				while (f1.judge != 出) {
					point.push_back(f1);
					p = e.erase(p);
					f1 = *p;
				}
				int l = 0;
				p = e.erase(p);
				while (q != d.end())
				{
					PixelPoint f2 = *q;
					if (f2.x == f1.x&&f2.y == f1.y&&f2.judge == 出) {
						point.push_back(f2);
						q = d.erase(q);
						f2 = *q;
						l++;
					}
					else {
						if (l == 1) {
							while (f2.judge != 入)
							{
								point.push_back(f2);
								q = d.erase(q);
								f2 = *q;
							}
							q = d.erase(q);
							break;
						}
						else
						{
							q++;
							
						}

					}

				}
					
				
			}*/
			
			
	/*for (int i = 0; i < num1; i++)
	{
		if (b[i].judge != 入) {
			while (!point.empty()) {
				if (point.back().judge == 出) {
					while (b[i].judge != 入)
					{

						for (int j = i; j < num1 - 1; j++)
						{
							b[j].x = b[j + 1].x;
							b[j].y = b[j + 1].y;
							b[j].judge = b[j + 1].judge;
						}
						point.push_back(b[i]);
						b[num1 - 1].x = 0;
						b[num1 - 1].y = 0;
						b[num1 - 1].judge = 无;
						num1--;
					}
				}
				break;
			}
			
		}
		else
		{
			temp = i;
			point.push_back(b[i]);
			for (int j = i; j < num1-1; j++)
			{
				b[j].x = b[j + 1].x;
				b[j].y = b[j + 1].y;
				b[j].judge = b[j + 1].judge;
			}
			b[num1 - 1].x = 0;
			b[num1 - 1].y = 0;
			b[num1 - 1].judge = 无;
			num1--;
			for (int k = 0; k < num; k++)
			{
				if (a[k].x == point.back().x&&a[k].y == point.back().y) {
					for (int l = k; l < num-1; l++)
					{
						a[l].x = a[l + 1].x;
						a[l].y = a[l + 1].y;
						a[l].judge = a[l + 1].judge;
					}
					a[num - 1].x = 0;
					a[num - 1].y = 0;
					a[num - 1].judge = 无;
					num--;
				
					while (a[k].judge != 出)
					{
						point.push_back(a[k]);
						for (int l = k; l < num - 1; l++)
						{
							a[l].x = a[l + 1].x;
							a[l].y = a[l + 1].y;
							a[l].judge = a[l + 1].judge;
						}
						a[num - 1].x = 0;
						a[num - 1].y = 0;
						a[num - 1].judge = 无;
						num--;
					}
					point.push_back(a[k]);
					for (int l = k; l < num - 1; l++)
					{
						a[l].x = a[l + 1].x;
						a[l].y = a[l + 1].y;
						a[l].judge = a[l + 1].judge;
					}
					a[num - 1].x = 0;
					a[num - 1].y = 0;
					a[num - 1].judge = 无;
					num--;
					break;
				}
				
			}
		}

	}*/
	/*for (int i = 0; i < temp; i++)
	{
		point.push_back(a[i]);
		
	}*/

	PixelPoint c[100];
	for (int i = 0; i < point.size(); i++)
	{
		c[i].x = point[i].x;
		c[i].y = point[i].y;
	}
	AET(BLACK,c, point.size() - 1);
	
}
