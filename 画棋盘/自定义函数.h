#pragma once

#include "resource.h"
#include"Graphic.h"
enum Dimension{twoD,threeD,Map};
enum vector2operation{ v_none,v_translation,v_rotation,v_ratio,v_symmetry,v_shear};
enum  operation{None,P_line,P_circle,p_polygon,p_area};
enum LineAlogorithmType{l_None,l_DDALine,l_MidBresenham,l_Bresenham};
enum circleAlogorithmType {c_None, c_MidBresenham,c_eightCircle
};
enum PolygonALgorithmType
{
	p_none,p_xray,p_ET
};
enum area
{
	a_none,a_recursion,a_norecursion,a_scanningline
};
struct DEM {
	Dimension type=twoD;
};
struct  vector2
{
	vector2operation vect;
};
struct seed
{
	int x;
	int y;
};
struct Seed
{
	int x;
	int y;
	Seed *next;
};
struct ET
{
	float x;
	int ymax;
	float k;
	ET* next;
};
struct a_renderstate
{
	operation operater=None;
	area areafill = a_none;

};
struct l_renderstate
{
	operation operater=None;
	LineAlogorithmType Lineoperate=l_None;

};
struct c_renderstate
{
	operation operater = None;
	circleAlogorithmType Circleoperate = c_None;

};
struct p_renderstate
{
	operation operater = None;
	PolygonALgorithmType polygoner = p_none;
};
//画棋盘函数
void drawline();
//画直线DDA算法，从（x0,y0）划到（x1,y1）颜色为color
// x0:起始x坐标
//	y0：起始y坐标
//	x1:终止x坐标
//	y1:终止y坐标
//	color：颜色
void DDALine(int x0, int x1, int y0, int y1, int color);
//画棋盘格大小的像素的函数，根据四舍五入确定（x,y）所在的棋盘格像素
//x:点的x坐标
//y:点的y坐标
void selfpixel(int x, int y, int color);
//画直线midbresenham算法，从（x0,y0）划到（x1,y1）颜色为color
// x0:起始x坐标
//	y0：起始y坐标
//	x1:终止x坐标
//	y1:终止y坐标
//	color：颜色
void MidBresenham(int x0, int x1, int y0, int y1, int color);
//画直线bresenham算法，从（x0,y0）划到（x1,y1）颜色为color
// x0:起始x坐标
//	y0：起始y坐标
//	x1:终止x坐标
//	y1:终止y坐标
//	color：颜色
void Bresenham(int x0, int x1, int y0, int y1, int color);
//画圆八分法（整个圆在屏幕中）
//x1:圆心x坐标
//y1 : 圆心y坐标
//color:填充颜色
void circlePoint(int x, int y, int color, PixelPoint &p);
//画圆midbresenham法（整个圆在屏幕中）
//x1:圆心x坐标
//y1 : 圆心y坐标
//color:填充颜色
void MidBresenhamCircle(int r, int color,PixelPoint &p);
//根据八分之一圆画出剩下的圆
//x:圆心x坐标
//y : 圆心y坐标
//color:填充颜色
void circlePointDraw(int i, int r, int color, PixelPoint &p);
//画椭圆函数
//a:长半轴
//b:短半轴
//color:颜色
void MidBresenhamEllipse(int a, int b, int color,PixelPoint &p);

//多边形填充算法之x扫描线法
//color：颜色
void x_ray(int color,PixelPoint b[100], int num);
//与selfpixel()基本相同，仅在前两行将（x,y）分别加PixelSelfDefineforcirclex，PixelSelfDefineforcircley，
//为了将圆平移到屏幕中
void selfpixelforCircle(int x, int y, int color, PixelPoint &p);
//多边形填充算法之有效边表法
//color：颜色
void AET(int color, PixelPoint b[100], int num);
//泛填充算法
//（x,y）为种子点坐标
//oldcolor:原色
//newcolor:新颜色
void FloodFill4(int x, int y,unsigned oldColor, unsigned newColor);
//非递归填充算法
//（x,y）为种子点坐标
//oldcolor:原色
//newcolor:新颜色
void areaNoRecursion(int x, int y, unsigned newcolor, unsigned edgecolor);
//本函数为扫描线法区域填充
//x,y为种子点坐标，newcolor为填充色，edgecolor为原来颜色
void scanningline(int x, int y, unsigned newcolor, unsigned edgecolor);
void zuobiaozhou();