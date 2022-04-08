/********************************************************************
file base:	Graphic
file ext:	h
author:		胡斌(njnu2016@foxmail.com)
purpose:	图形库接口
version:    1.0
*********************************************************************/
#ifndef _GRAPH_H
#define _GRAPH_H

#define BLACK 0
#define WHITE 0xFFFFFF
#define RED 0xFF
#define GREEN 0xFF00
#define BLUE 0xFF0000
#define YELLOW 0xFFFF00

///像素点
enum inOrout { 无, 入, 出 };
struct PixelPoint
{
	int x,y;
	inOrout judge = 无;
};

/**	初始化
@return 是否初始化成功，0-success， -1 - fail
*/
int init();

/**	刷新窗口
*/
void refreshWindow();

/**	清除窗口内容
*/
void clearWindow();

/**	把后缓存信息显示到屏幕
*/
void swapBuffer();

/**	设置窗口大小
@param  width 窗口宽度
@param  height  窗口高度
*/
void setWindowSize(int width, int height);

/**	获取窗口宽度
@return 窗口宽度
*/
int getWindowWidth();

/**	获取窗口高度
@return 窗口高度
*/
int getWindowHeight();

/**	设置Y轴方向
@param  isUp true表示向上，否则向下
*/
void setYUp(bool isUp);

/**	设置原点为指定像素位置
@param  x 指定像素位置x坐标
@param  y 指定像素位置y坐标
*/
void setOrig(int x, int y);

/**	设置指定位置像素的颜色
@param  x 像素x坐标
@param  y  像素y坐标
@param color 颜色
*/
void setPixel(int x, int y, unsigned color);

/**	获取指定位置像素的颜色
@param  x 像素x坐标
@param  y  像素y坐标
@return 指定位置像素的颜色
*/
unsigned getPixel(int x, int y);

/**	设备坐标转为逻辑坐标
@param  dx 设备x坐标
@param  dy  设备y坐标
@param  lx 逻辑x坐标
@param  ly  逻辑y坐标
*/
void DPtToLPt(int dx, int dy, int& lx, int& ly);

/**	逻辑坐标转为设备坐标
@param  lx 逻辑x坐标
@param  ly  逻辑y坐标
@param  dx 设备x坐标
@param  dy  设备y坐标
*/
void LPtToDPt( int lx, int ly, int& dx, int& dy );

/**	设置背景色
@param  backColor 背景色
*/
void setBackColor( unsigned backColor );

/**	设置字体放大倍数
@param  fontHeight 字体高度
*/
void setFontHeight( int fontHeight );

/**	设置字体名
@param  fontName 字体名
*/
void setFontName( const char* fontName );

/**	获取文字轮廓大小，以字节为单位
@param  pTextA 文字
*/
int getTextOutlineInfoSize( const char* pTextA );

/**	获取文字轮廓信息
@param  pText  待输出文字
@param  pBuffer  字符轮廓信息存放缓冲区
*/
void getTextOutline( const char* pTextA , unsigned char* pBuffer );


///橡皮线模式, rmNone -- 关闭橡皮线模式 ，rmLine-- 线模式， rmRectangle -- 矩形模式 , rmPolyline-- 折线模式, rmPolygon -- 多边形模式
enum RubberMode{ rmNone, rmLine, rmRectangle, rmPolyline, rmPolygon };

/**	设置当前模式是否为橡皮线模式
@param  value 是否橡皮线模式
*/
void setRubberMode(RubberMode mode );

/**	获取当前橡皮线模式
@return 当前橡皮线模式
*/
RubberMode getRubberMode();

/**	获取橡皮线起点和终点，必须先启用了橡皮线模式并绘制了橡皮线
@param  x1 起点x坐标
@param  y1  起点y坐标
@param  x2 终点x坐标
@param  y2  终点y坐标
@return 有效橡皮线点数
*/
int getRubberPoints(int& x1, int& y1, int& x2, int& y2);

/**	获取橡皮线点集合，一般用于rmPolyline或rmPolygon模式
@param  pts 橡皮线点集合
@return 橡皮线点数
*/
int getRubberPoints( PixelPoint pts[] );

///光标样式：csArrow--箭头 ，  csHand--手型， csCross--十字丝， csSize--扩张
enum CursorStyle{ csArrow = 32512, csHand = 32649, csCross = 32515, csSize = 32646 };

/**	设置光标样式
@param  cursor 光标样式
*/
void setCursor(CursorStyle cursor);

///组成字符轮廓的多边形，形成静态链表结构
struct _charPolygon
{
	int pointCount;//组成多边形的顶点数
	PixelPoint pts[1];//多边形顶点数组

	int size();	//用该数据结构描述的多边形数据占据的实际空间大小，以字节为单位

	_charPolygon* next();//组成字符轮廓的下一个多边形
};

///字符轮廓,由一个或多个多边形组成，形成静态链表结构
struct _charOutline
{
	int cb;//用该数据结构描述的字符轮廓占据的实际空间大小，以字节为单位
	_charPolygon charPolygons[1];//组成字符轮廓的多边形数组

	_charOutline* next();//下一条字符轮廓

	int count();//获取字符多边形总数

	_charPolygon* operator[]( int index );//获取指定位置的字符多边形
};

///多个字符轮廓，形成静态链表结构
struct _textOutline
{
	int cb;//用该数据结构描述的多个字符轮廓占据的实际空间大小，以字节为单位
	_charOutline charOutlines[1];//字符轮廓数组

	int count();//获取字符轮廓总数

	_charOutline* operator[]( int index );//获取第index个字符轮廓
};


#endif