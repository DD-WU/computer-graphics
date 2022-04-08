/********************************************************************
file base:	Graphic
file ext:	cpp
author:		胡斌(njnu2016@foxmail.com)
purpose:	图形库实现
version:    1.0
*********************************************************************/
#include "Graphic.h"
#include <windows.h>
#include <math.h>
#include <vector>
#include <assert.h>

using namespace std;

//#define  DIRECT_DRAW

#ifndef DIRECT_DRAW

template< typename T>
class DynamicMatrix
{
public:
	DynamicMatrix( int alignBytes = 1 ){ this->alignBytes = alignBytes; pData =  0; width = height = 0 ; }
	~DynamicMatrix(void);

	void freeMatrix();
	T* operator[](int i );
	void setSize( int width, int height );
	T* dataPtr()const{ return pData ? pData[0] : 0 ; }
	template<class U> void clear( const U& value );
	bool valid()const{return pData || width > 0 || height > 0;}
	int getLineWidth();

	int alignBytes;
	int width ,height;
	T** pData;
};

template< class T>
DynamicMatrix<T>::~DynamicMatrix(void)
{
	freeMatrix();
}

template< typename T>
int DynamicMatrix<T>::getLineWidth()
{
	return ( width * sizeof(T) + alignBytes - 1) / alignBytes * alignBytes;
}

template< typename T>
void DynamicMatrix<T>::setSize(int width, int height)
{
	if( width == this->width && height == this->height ) return;

	this->width = width;
	this->height = height;

	//freeMatrix();	

	int lineWidth = getLineWidth();
	T* pBuf;
	if( pData )
	{
		pBuf = (T*)realloc( pData[0], lineWidth * height ) ;
		pData = (T**)realloc( pData, sizeof(T*) * height) ;	
	}
	else
	{
		//pBuf = (T*)malloc( sizeof(T) * width * height );
		pBuf = (T*)malloc( lineWidth * height );
		pData = (T**)malloc( sizeof(T*) * height) ;
	}

	for ( int i = 0 ; i < height; ++i )
	{
		//pData[i] = pBuf + width * i;
		pData[i] = (T*)((char*)pBuf + lineWidth * i);
	}

}

template< typename T>
T* DynamicMatrix<T>::operator[](int i)
{
	return pData[i];
}

template< typename T>
void DynamicMatrix<T>::freeMatrix()
{
	if( pData )
	{
		free( pData[0]);
		free( pData);		
		pData = 0;
	}
}

template< typename T >
template< typename U>
void DynamicMatrix<T>::clear( const U& value  )
{
	if( !pData || !pData[0]) return;

	T* p = pData[0];
	int lineWidth = getLineWidth();	
	if( sizeof(U) == 1 )
	{
		memset( p,value, lineWidth * height );
		//memset( pData[0],value, sizeof(T) * width * height );
	}
	else
	{
		unsigned val = value & 0xFF;
		if( val == value )
		{
			memset( p,val, lineWidth * height );
		}
		else
		{
			int sizeT = sizeof(T);
			int sizeU = sizeof(U);

			if( sizeT <= sizeU )
			{
				for( int i = 0 ; i < height; ++i )
				{
					for( int j = 0 ; j < width; ++j )
					{
						unsigned char* pT = (unsigned char*)( pData[i] + j);
						unsigned char* pU = (unsigned char*)&value;
						for( int k = 0; k < sizeT; ++k )
						{
							pT[k] = pU[k];
						}
					}
					//p[i] = (T&)value;
				}
			}
			else
			{
				for( int i = 0 ; i < height; ++i )
				{
					for( int j = 0 ; j < width; ++j )
					{
						unsigned char* pT = (unsigned char*)( pData[i] + j);
						unsigned char* pU = (unsigned char*)&value;
						for( int k = 0; k < sizeU; ++k )
						{
							pT[k] = pU[k];
						}
					}

					//p[i] = (T&)value;
				}
			}
		}		
	}
}

#endif // !USE_GDI_FUNCTION

LRESULT CALLBACK WndProcNew(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
WNDPROC g_lpfnOldProc = 0;

int g_clientWidth = 800;
int g_clientHeight = 600;

POINT g_orig = { 0, 0 };
int g_upY = 1;

HWND g_hWnd = 0;
HDC g_hDC = 0;

unsigned g_backColor =  WHITE;
float g_fontHeight = 100;
char g_fontName[32] = "宋体";

//int g_pointSize = 10;
//unsigned g_penColor = 255;
bool g_isDragging = false;
POINT g_lastPoint = { 0, 0 };



#ifndef DIRECT_DRAW

HDC g_hdcMem = 0;
typedef unsigned char rgb[3];
DynamicMatrix<rgb> g_frameBuffer(4);
BITMAPINFO g_bmpInfo;
HBITMAP g_hBitmap;

struct DummyClass
{
	DummyClass()
	{

	}
	~DummyClass()
	{ 
		if ( g_hdcMem )	DeleteObject(g_hdcMem);
	}
} g_dummyClass;

#endif

BOOL CALLBACK _enumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	if (dwProcessId == *(DWORD*)lParam)
	{
		g_hWnd = hWnd;
		return FALSE;
	}
	
	return TRUE;
}

BOOL _isConsoleApplication()
{
	return GetConsoleWindow() != NULL;
}


int init()
{
	DWORD hProcessID = GetCurrentProcessId();

	// 查找主窗口  
	EnumWindows(_enumWindowCallBack, (LPARAM)&hProcessID);

	g_lpfnOldProc = (WNDPROC)SetWindowLong(g_hWnd, GWL_WNDPROC, (DWORD)WndProcNew);

	g_hDC = GetDC(g_hWnd);

	if (g_hDC)
	{
#ifndef DIRECT_DRAW

		g_hdcMem = CreateCompatibleDC(g_hDC);
		g_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		g_bmpInfo.bmiHeader.biPlanes = 1;
		g_bmpInfo.bmiHeader.biBitCount = 24;
		g_bmpInfo.bmiHeader.biCompression = BI_RGB;
		g_bmpInfo.bmiHeader.biSizeImage = 0;
		g_bmpInfo.bmiHeader.biXPelsPerMeter = 3000;
		g_bmpInfo.bmiHeader.biYPelsPerMeter = 3000;
		g_bmpInfo.bmiHeader.biClrUsed = 0;
		g_bmpInfo.bmiHeader.biClrImportant = 0;
#endif

		RECT rect;
		GetClientRect(g_hWnd, &rect);

		setWindowSize(rect.right, rect.bottom );

		setYUp( g_upY == -1 ? true : false );
		setOrig( g_orig.x, g_orig.y );

		return 0;
	}
	return -1;
}

void _ensure_inited()
{
	if (!g_hDC)	init( );
	if (!g_hDC)exit(-1);
}

#ifndef DIRECT_DRAW

void _initMemDC()
{
	g_hBitmap = CreateCompatibleBitmap(g_hDC, g_clientWidth, g_clientHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(g_hdcMem, g_hBitmap);
	DeleteObject(hOldBitmap);
}

#endif

void setWindowSize(int width, int height)
{

	if (!g_hDC)
	{
		init();
		return;
	}	

	if (_isConsoleApplication())
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_FONT_INFO fontInfo;
		GetCurrentConsoleFont(handle, TRUE, &fontInfo);
		COORD fontSize = GetConsoleFontSize(handle, fontInfo.nFont);

		double scaleX = GetDeviceCaps(g_hDC, LOGPIXELSX) / 72.0;
		double scaleY = GetDeviceCaps(g_hDC, LOGPIXELSY) / 72.0;
		fontSize.X *= scaleX;//font pixel num
		fontSize.Y *= scaleY;//font pixel num

		COORD size;
		if (width == -1 && height == -1)
		{
			size = GetLargestConsoleWindowSize(handle);
		}
		else
		{		
			size.X = (double)width / fontSize.X;
			size.Y = (double)height / fontSize.Y;			

			COORD sizeMax = GetLargestConsoleWindowSize(handle);
			size.X = size.X > sizeMax.X ? sizeMax.X : size.X;
			size.Y = size.Y > sizeMax.Y ? sizeMax.Y : size.Y;
		}
		
		int rs = SetConsoleScreenBufferSize(handle, size);

		SMALL_RECT rc;
		rc.Left = 0, rc.Top = 0, rc.Right = size.X - 1, rc.Bottom = size.Y - 1;
		rs = SetConsoleWindowInfo(handle, TRUE, &rc);

		CONSOLE_SCREEN_BUFFER_INFO bInfo;
		GetConsoleScreenBufferInfo(handle, &bInfo);

		g_clientWidth = (bInfo.srWindow.Right - bInfo.srWindow.Left) * fontSize.X;
		g_clientHeight = (bInfo.srWindow.Bottom - bInfo.srWindow.Top) * fontSize.Y;
	}
	else
	{
		//if (width != -1 && height != -1)
		//{
		//	RECT wndRect = { 0, 0, width, height };// client Rect
		//	AdjustWindowRectEx(&wndRect, WS_OVERLAPPEDWINDOW, FALSE, NULL);
		//	MoveWindow(g_hWnd, 0, 0, wndRect.right, wndRect.bottom, TRUE);
		//}	
		RECT rect;
		GetClientRect(g_hWnd, &rect);
		g_clientWidth = rect.right - rect.left;
		g_clientHeight = rect.bottom - rect.top;
	}	
	
#ifndef DIRECT_DRAW
	_initMemDC();
	g_frameBuffer.setSize(g_clientWidth, g_clientHeight);
	g_bmpInfo.bmiHeader.biWidth = g_clientWidth;
	g_bmpInfo.bmiHeader.biHeight = -g_clientHeight;
	//g_frameBuffer.clearWithValue(g_backColor);
#endif
}

#ifndef DIRECT_DRAW

void _buildMemDCFromFrameBuffer( )
{
	SetDIBits( g_hdcMem, g_hBitmap, 0, abs( g_bmpInfo.bmiHeader.biHeight), g_frameBuffer.dataPtr(), &g_bmpInfo, DIB_RGB_COLORS); 

	//for (int row = 0; row < g_frameBuffer.height; ++row)
	//{
	//	for (int col = 0; col < g_frameBuffer.width; ++col)
	//	{
	//		unsigned color = g_frameBuffer[row][col];
	//		if (color != g_backColor)
	//			::SetPixel(g_hdcMem, col, row, color);
	//	}
	//}
}


void _clearMemDC()
{
	static HBRUSH hBrush = CreateSolidBrush( g_backColor );
	RECT rect;
	GetClientRect( g_hWnd, &rect);
	::FillRect(g_hdcMem, &rect , hBrush);
}

/**	把后缓存信息显示到屏幕
*/
void swapBuffer()
{
	_ensure_inited();

	_buildMemDCFromFrameBuffer();

	RECT rect;
	GetClientRect(g_hWnd, &rect);
	
	BitBlt(g_hDC, 0, 0, g_clientWidth, g_clientHeight /*g_frameBuffer.width, g_frameBuffer.height*/, g_hdcMem, 0, 0, SRCCOPY);
}


#endif

void refreshWindow()
{
	InvalidateRect(g_hWnd, NULL, TRUE);
}

unsigned RGBtoBGR( unsigned color )
{
	unsigned char* pColor = (unsigned char*)&color;
	unsigned char c = pColor[0] ;
	pColor[0] = pColor[2];
	pColor[2] = c;
	return color;
}

void clearWindow()
{
	_ensure_inited();	

#ifndef DIRECT_DRAW
	g_frameBuffer.clear( RGBtoBGR( g_backColor));

	//_buildMemDCFromFrameBuffer();

	_clearMemDC();
#endif	
}

void setOrig(int x, int y)
{
	_ensure_inited();

#ifdef DIRECT_DRAW
	//SetWindowOrgEx(g_hDC, -x, getWindowHeight() - y, 0);
	//SetWindowOrgEx(g_hdcMem, -x, getWindowHeight() - y, 0);
	SetViewportOrgEx(g_hDC, x, y, 0);
	//SetViewportOrgEx(g_hdcMem, x, y, 0);
#endif
	g_orig.x = x;
	g_orig.y = y;

}

void setYUp(bool isUp)
{
#ifdef DIRECT_DRAW
	if (isUp)
	{
		SetMapMode(g_hDC, MM_ANISOTROPIC);
		SetWindowExtEx(g_hDC, 1, 1, 0);
		SetViewportExtEx(g_hDC, 1, -1, 0);

		//SetMapMode(g_hdcMem, MM_ANISOTROPIC);
		//SetWindowExtEx(g_hdcMem, 1, 1, 0);
		//SetViewportExtEx(g_hdcMem, 1, -1, 0);
}
	else
	{
		SetMapMode(g_hDC, MM_ANISOTROPIC);
		SetWindowExtEx(g_hDC, 1, 1, 0);
		SetViewportExtEx(g_hDC, 1, 1, 0);

		//SetMapMode(g_hdcMem, MM_ANISOTROPIC);
		//SetWindowExtEx(g_hdcMem, 1, 1, 0);
		//SetViewportExtEx(g_hdcMem, 1, 1, 0);
	}
#endif
	g_upY = isUp ? -1 : 1;

}

/**	设置背景色
@param  backColor 背景色
*/
void setBackColor( unsigned backColor )
{
	g_backColor = backColor;
}

/**	设置字体放大倍数
@param  fontScale 字体放大倍数
*/
void setFontHeight( int fontHeight )
{
	g_fontHeight = fontHeight;
}

void setFontName( const char* fontName )
{
	strcpy( g_fontName, fontName );
}

void setPixel(int x, int y, unsigned color)
{
	//_ensure_inited();

#ifndef DIRECT_DRAW

	LPtToDPt( x,y, x,y);

	if (x < 0 || x >= g_frameBuffer.width) return;
	if (y < 0 || y >= g_frameBuffer.height) return;

	char* pBytes = (char*)( g_frameBuffer[y] + x);
	char* pColor = (char*)&color;
	pBytes[0] = pColor[2];
	pBytes[1] = pColor[1];
	pBytes[2] = pColor[0];
	//g_frameBuffer[y][x] = color;
#else
	//SetPixel( g_hdcMem, x, y, color );
	SetPixel(g_hDC, x, y, color);
#endif
}

unsigned getPixel(int x, int y)
{
	//_ensure_inited();

#ifndef DIRECT_DRAW

	LPtToDPt( x,y, x,y);

	if (x < 0 || x >= g_frameBuffer.width) return 0;
	if (y < 0 || y >= g_frameBuffer.height) return 0;

	char* pBytes = (char*)( g_frameBuffer[y] + x);
	unsigned color = 0;
	char* pColor = (char*)&color;
	pColor[0] = pBytes[2];
	pColor[1] = pBytes[1];
	pColor[2] = pBytes[0];
	return color;
	//return g_frameBuffer[y][x];
#else
	//return GetPixel( g_hdcMem, x, y );
	return GetPixel(g_hDC, x, y);
#endif
}

void DPtToLPt(int dx, int dy, int& lx, int& ly)
{
#ifndef DIRECT_DRAW
	lx = dx - g_orig.x;
	ly = (dy - g_orig.y)*g_upY;
#else
	//lx = dx;
	//ly = dy;
	POINT pt = { dx, dy };
	DPtoLP( g_hDC, &pt, 1 );
	lx = pt.x;
	ly = pt.y;
#endif
}

void LPtToDPt( int lx, int ly, int& dx, int& dy )
{
#ifndef DIRECT_DRAW
	dx = lx + g_orig.x;
	dy = g_orig.y + g_upY * ly;
#else
	//dx = lx;
	//dy = ly;
	POINT pt = { lx, ly };
	LPtoDP( g_hDC, &pt, 1 );
	dx = pt.x;
	dy = pt.y;
#endif
}

int getWindowWidth()
{
	return g_clientWidth;
}

int getWindowHeight()
{
	return g_clientHeight;
}


class RubberPad
{
public:
	POINT pt1;
	POINT pt2;
	vector<POINT> pts;
	unsigned penColor;
	unsigned oldPenColor;
	int oldROP2Mode;
	RubberMode rubberMode ;
	bool startDrawing ;
	bool lineToFirst;

	RubberPad()
	{
		penColor = YELLOW;
		rubberMode = rmNone;
		startDrawing = false;
		lineToFirst = true;
	}

	void mouseDown(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		//if (wMsg == WM_LBUTTONDOWN)
		//{
		//	if (rubberMode == rmNone) return;

		//	oldROP2Mode = SetROP2(g_hDC, R2_NOT);
		//	SelectObject(g_hDC, GetStockObject(DC_PEN));
		//	oldPenColor = SetDCPenColor(g_hDC, penColor);

		//	pt1.x = LOWORD(lParam);
		//	pt1.y = HIWORD(lParam);

		//	pt2 = pt1;
		//	startDrawing = true;
		//}
	}

	void mouseMove(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		if (rubberMode == rmNone) return;

		if (startDrawing)
		{
			drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);

			if( rubberMode == rmPolygon && pts.size() >= 2 )
			{
				/*lineToFirst ? drawRubberline(g_hDC, pts[0].x, pts[0].y, pt2.x, pt2.y) : (lineToFirst = true);*/
				if (lineToFirst) {
					drawRubberline(g_hDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
				}
				else
				{
					lineToFirst = true;
				}
			}			

			pt2.x = LOWORD(lParam);
			pt2.y = HIWORD(lParam);

			drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);

			if( rubberMode == rmPolygon && pts.size() >= 2 )
			{
				drawRubberline(g_hDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
			}
		}
		
	}

	void mouseUp(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		if (wMsg == WM_LBUTTONUP)
		{
			if (rubberMode == rmNone) return;

			if( !startDrawing )
			{
				oldROP2Mode = SetROP2(g_hDC, R2_NOT);
				SelectObject(g_hDC, GetStockObject(DC_PEN));
				oldPenColor = SetDCPenColor(g_hDC, penColor);

				pt1.x = LOWORD(lParam);
				pt1.y = HIWORD(lParam);

				pt2 = pt1;
				pts.clear();

				startDrawing = true;
			}
			else
			{
				if( rubberMode == rmLine || rubberMode == rmRectangle )
				{
					drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);	

					pts.push_back( pt2 );

					DPtToLPt( pts[0].x, pts[0].y, (int&)pts[0].x, (int&)pts[0].y );
					DPtToLPt( pts[1].x, pts[1].y, (int&)pts[1].x, (int&)pts[1].y );
					//DPtToLPt( pt1.x, pt1.y, (int&)pt1.x, (int&)pt1.y );
					//DPtToLPt( pt2.x, pt2.y, (int&)pt2.x, (int&)pt2.y );

					SelectObject(g_hDC, GetStockObject(DC_PEN));
					oldPenColor = SetDCPenColor(g_hDC, penColor);
					SetROP2(g_hDC, oldROP2Mode );
					startDrawing = false;

					return;
				}
				else
				{
					if( rubberMode == rmPolygon && pts.size() == 1 )
					{
						drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);//擦除
						lineToFirst = false;//多边形模式下防止第一条线被擦除
					}

					pt2.x = LOWORD(lParam);
					pt2.y = HIWORD(lParam);

					if( rubberMode == rmPolygon && pts.size() == 1 )
					{
						drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);
					}				

					pt1 = pt2;
				}				
			}

			pts.push_back( pt1 );

		}
		else if( wMsg == WM_RBUTTONUP )
		{
			if( startDrawing == false ) return;

			if( rubberMode == rmPolyline || rubberMode == rmPolygon )
			{
				drawRubberline(g_hDC, pt1.x, pt1.y, pt2.x, pt2.y);	//擦除P1P2

				if( rubberMode == rmPolygon && pts.size() >= 2 ) // >= 2 --> P2 与两个点相连
				{
					lineToFirst == true ? drawRubberline(g_hDC, pts[0].x, pts[0].y, pt2.x, pt2.y) : (void)(lineToFirst = true);
					//if (lineToFirst) {//擦除P0P2
					//	drawRubberline(g_hDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
					//}
					//else
					//{
					//	lineToFirst = true;
					//}
				}

				for ( int i = 0 ; i < (int)pts.size() - 1; ++i )
				{
					drawRubberline(g_hDC, pts[i].x, pts[i].y, pts[i+1].x, pts[i+1].y);
				}
	
				for ( int i = 0 ; i < (int)pts.size() ; ++i )
				{
					DPtToLPt( pts[i].x, pts[i].y, (int&)pts[i].x, (int&)pts[i].y );
				}

				SelectObject(g_hDC, GetStockObject(DC_PEN));
				oldPenColor = SetDCPenColor(g_hDC, penColor);
				SetROP2(g_hDC, oldROP2Mode );
				startDrawing = false;
			}
		}
	}

	void drawRubberline(HDC hdc, int x0, int y0, int x1, int y1)
	{

		if (rubberMode != rmRectangle )
		{
			drawLine(hdc, x0, y0, x1, y1);
		}
		else
		{
			drawRectangle(hdc, x0, y0, x1, y1);
		}
	}

	void drawLine(HDC hdc, int x0, int y0, int x1, int y1)
	{
#ifdef DIRECT_DRAW
		DPtToLPt( x0, y0, x0 ,y0 );
		DPtToLPt( x1, y1, x1 ,y1 );
#endif
		MoveToEx(hdc, x0, y0, NULL);
		LineTo(hdc, x1, y1);
	}

	void drawRectangle(HDC hdc, int x0, int y0, int x1, int y1)
	{
#ifdef DIRECT_DRAW
		DPtToLPt( x0, y0, x0 ,y0 );
		DPtToLPt( x1, y1, x1 ,y1 );
#endif
		MoveToEx(hdc, x0, y0, NULL);
		LineTo(hdc, x1, y0);
		LineTo(hdc, x1, y1);
		LineTo(hdc, x0, y1);
		LineTo(hdc, x0, y0);
	}
};

RubberPad g_rubberPad;

void setRubberMode(RubberMode mode)
{
	g_rubberPad.rubberMode = mode;
}

RubberMode getRubberMode()
{
	return g_rubberPad.rubberMode;
}

int getRubberPoints(int& x1, int& y1, int& x2, int& y2)
{
	int size = g_rubberPad.pts.size();
	if( size > 0 )
	{
		x1 = g_rubberPad.pts[0].x;
		y1 = g_rubberPad.pts[0].y;
	}
	if( size > 1 )
	{
		x2 = g_rubberPad.pts[1].x;
		y2 = g_rubberPad.pts[1].y;
	}
	return size < 3 ? size : 2 ;

	//x1 = g_rubberPad.pt1.x;
	//y1 = g_rubberPad.pt1.y;
	//x2 = g_rubberPad.pt2.x;
	//y2 = g_rubberPad.pt2.y;
	//DPtToLPt(g_rubberPad.pt1.x, g_rubberPad.pt1.y, x1, y1);
	//DPtToLPt(g_rubberPad.pt2.x, g_rubberPad.pt2.y, x2, y2);
}

int getRubberPoints( PixelPoint pts[] )
{
	switch( g_rubberPad.rubberMode )
	{
	case rmLine:
	case rmRectangle:
		//pts[0].x = g_rubberPad.pt1.x;
		//pts[0].y = g_rubberPad.pt1.y;
		//pts[1].x = g_rubberPad.pt2.x;
		//pts[1].y = g_rubberPad.pt2.y;
		//return 2;
	case rmPolyline:
	case rmPolygon:
		for( int i = 0 ; i < g_rubberPad.pts.size(); ++i )
		{
			pts[i].x = g_rubberPad.pts[i].x;
			pts[i].y = g_rubberPad.pts[i].y;
		}
		return g_rubberPad.pts.size();
	}
	return 0;
}

void setCursor(CursorStyle cursor)
{
	SetClassLong(g_hWnd, GCL_HCURSOR, (long)LoadCursor(NULL, MAKEINTRESOURCE( cursor )));
}

LRESULT CALLBACK WndProcNew(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wMsg) 
	{
		case WM_SIZE:
			setWindowSize(-1, -1);
			CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
			refreshWindow();
			return 0;
		case WM_ERASEBKGND:
#ifndef DIRECT_DRAW
			//clearWindow();			
			return 0;
#endif
			break;
		case WM_PAINT:
			if (g_rubberPad.startDrawing) return 0;

			clearWindow();
			CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
			ValidateRect( g_hWnd, NULL );

#ifndef DIRECT_DRAW			
			swapBuffer();
#endif
			return 0;
		case WM_LBUTTONDOWN:
			CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
			g_rubberPad.mouseDown(wMsg, wParam, lParam);
			return 0;
		case WM_MOUSEMOVE:
			g_rubberPad.mouseMove(wMsg, wParam, lParam);
			CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			g_rubberPad.mouseUp(wMsg, wParam, lParam);
			CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
			return 0;

	}
	return CallWindowProc(g_lpfnOldProc, hWnd, wMsg, wParam, lParam);
}

int getTextOutlineInfoSize( const char* pTextA )
{
	int size = sizeof(int);// _textOutline cb

	int uLen = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, NULL, 0 );  
	if ( uLen <= 0 )  return size;

	wchar_t textw[1024];
	int nRtn = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, textw, uLen );  
	if ( nRtn != uLen )  return size;

	HFONT hFont = CreateFontA( g_fontHeight, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, g_fontName );
	HFONT hOldFont = (HFONT)SelectObject( g_hDC, hFont );

	GLYPHMETRICS gm;
	MAT2 mat2 = {{0,1}, {0,0}, {0,0}, {0,1}};

	static unsigned char charBuffer[10240];
	for( int c = 0; c < uLen - 1; ++c )
	{		
		wchar_t ch = textw[c];
		DWORD returnBytes = GetGlyphOutlineW( g_hDC, ch, GGO_BEZIER, &gm, sizeof(charBuffer), charBuffer, &mat2 );
		if( returnBytes != GDI_ERROR )
		{
			TTPOLYGONHEADER* pTTPH = (TTPOLYGONHEADER*)charBuffer;
			assert( pTTPH->dwType == TT_POLYGON_TYPE);

			size += sizeof(int);//_charOutline cb

			while( returnBytes > 0) //一次循环处理一个多边形
			{
				TTPOLYCURVE* pCurrentCurve = (TTPOLYCURVE*)( pTTPH + 1 );
				
				size += sizeof(int);//pointCount
				size += sizeof(int) * 2;//pTTPH->pfxStart.x.value, pTTPH->pfxStart.y.value

				int remainBytes = pTTPH->cb - sizeof(TTPOLYGONHEADER);
				while( remainBytes > 0)
				{
					size += pCurrentCurve->cpfx * sizeof(int) * 2;
					int count = sizeof(TTPOLYCURVE) + ( pCurrentCurve->cpfx - 1 ) * sizeof(POINTFX);
					pCurrentCurve = (TTPOLYCURVE*)((char*)pCurrentCurve + count);
					remainBytes -= count;
				}			
				returnBytes -= pTTPH->cb;
				pTTPH = (TTPOLYGONHEADER*)((char*)pTTPH + pTTPH->cb);	
			}
		}
	}

	SelectObject(g_hDC,hOldFont);
	DeleteObject(hFont);

	return size;
}


void getTextOutline( const char* pTextA , unsigned char* pBuffer )
{
	int uLen = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, NULL, 0 );  
	if ( uLen <= 0 )  return;

	wchar_t textw[1024];
	int nRtn = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, textw, uLen );  
	if ( nRtn != uLen )  return;

	HFONT hFont = CreateFontA( g_fontHeight, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, g_fontName );
	HFONT hOldFont = (HFONT)SelectObject( g_hDC, hFont );

	GLYPHMETRICS gm;
	MAT2 mat2 = {{0,1}, {0,0}, {0,0}, {0,1}};

	_textOutline* p_textOutline = ( _textOutline* )pBuffer;
	p_textOutline->cb = sizeof(int);
	_charOutline* p_charOutline = (_charOutline*)p_textOutline->charOutlines;

	static unsigned char charBuffer[10240];
	int offset_x = 0;
	int x, y;
	for( int c = 0; c < uLen - 1; ++c )
	{		
		wchar_t ch = textw[c];
		DWORD returnBytes = GetGlyphOutlineW( g_hDC, ch, GGO_BEZIER, &gm, sizeof(charBuffer), charBuffer, &mat2 );
		if( returnBytes != GDI_ERROR )
		{
			TTPOLYGONHEADER* pTTPH = (TTPOLYGONHEADER*)charBuffer;
			assert( pTTPH->dwType == TT_POLYGON_TYPE);

			p_charOutline->cb = sizeof(int);

			_charPolygon* p_charPolygon = p_charOutline->charPolygons;

			while( returnBytes > 0) //一次循环处理一个多边形
			{
				p_charPolygon->pointCount = 0 ;

				PixelPoint* pts = p_charPolygon->pts;

				DPtToLPt( offset_x + pTTPH->pfxStart.x.value, pTTPH->pfxStart.y.value , x, y );
				pts[p_charPolygon->pointCount].x = x;//offset_x + pTTPH->pfxStart.x.value;
				pts[p_charPolygon->pointCount++].y = y;//pTTPH->pfxStart.y.value;

				TTPOLYCURVE* pCurrentCurve = (TTPOLYCURVE*)( pTTPH + 1 );
				int remainBytes = pTTPH->cb - sizeof(TTPOLYGONHEADER);
				while( remainBytes > 0)
				{
					switch(pCurrentCurve->wType)
					{
					case TT_PRIM_LINE:
					case TT_PRIM_QSPLINE:
					case TT_PRIM_CSPLINE:
						{
							for(int i = 0; i < pCurrentCurve->cpfx; ++i)
							{
								DPtToLPt( offset_x + pCurrentCurve->apfx[i].x.value, pCurrentCurve->apfx[i].y.value , x, y );
								pts[p_charPolygon->pointCount].x = x;
								pts[p_charPolygon->pointCount++].y = y;
								/*pts[p_charPolygon->pointCount].x = offset_x + pCurrentCurve->apfx[i].x.value;
								pts[p_charPolygon->pointCount++].y = pCurrentCurve->apfx[i].y.value;*/
							}
						}
						break;
					}
					int count = sizeof(TTPOLYCURVE) + ( pCurrentCurve->cpfx - 1 ) * sizeof(POINTFX);
					pCurrentCurve = (TTPOLYCURVE*)((char*)pCurrentCurve + count);
					remainBytes -= count;
				}			

				returnBytes -= pTTPH->cb;
				pTTPH = (TTPOLYGONHEADER*)((char*)pTTPH + pTTPH->cb);

				p_charOutline->cb +=  p_charPolygon->size();
				p_charPolygon = p_charPolygon->next();				
			}

			p_textOutline->cb += p_charOutline->cb ;
			p_charOutline = p_charOutline->next() ;

			offset_x += gm.gmCellIncX;
		}
	}

	SelectObject(g_hDC,hOldFont);
	DeleteObject(hFont);
}

int _charPolygon::size()
{
	return sizeof(int) + sizeof(PixelPoint ) * pointCount;
}

_charPolygon* _charPolygon::next()
{
	return (_charPolygon*)((char*)this + size());
}

_charOutline* _charOutline::next()
{
	return (_charOutline*)((char*)this + cb );
}

int _charOutline::count()
{
	if( cb == 0 ) return 0;

	_charPolygon* p_charPolygon = charPolygons;
	int sum =  sizeof(cb) + p_charPolygon->size(), c = 1;
	while ( sum < cb )
	{
		p_charPolygon = p_charPolygon->next();
		sum += p_charPolygon->size();	
		++c;
	}
	return c;
}

_charPolygon* _charOutline::operator[](int index)
{
	if( index < 0 || cb == 0 ) return 0;

	_charPolygon* p_charPolygon = charPolygons;
	int sum = sizeof(cb) + p_charPolygon->size(), c = 0;
	while ( sum < cb && c < index )
	{
		p_charPolygon = p_charPolygon->next();
		sum += p_charPolygon->size();	
		++c;
	}

	return c == index ? p_charPolygon: 0;
}

int _textOutline::count()
{
	if( cb == 0 ) return 0;

	_charOutline* p_charOutline = charOutlines;
	int sum =  sizeof(cb) + p_charOutline->cb, c = 1;
	while ( sum < cb )
	{
		p_charOutline = p_charOutline->next();
		sum += p_charOutline->cb;	
		++c;
	}
	return c;
}

_charOutline* _textOutline::operator[](int index)
{
	if( index < 0 || cb == 0 ) return 0;

	_charOutline* p_charOutline = charOutlines;
	int sum =  sizeof(cb) + p_charOutline->cb, c = 0;
	while ( sum < cb && c < index )
	{
		p_charOutline = p_charOutline->next();
		sum += p_charOutline->cb;	
		++c;
	}

	return c == index ? p_charOutline: 0;
}




