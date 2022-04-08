#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"
#include "Define.h"
#include "Utils.h"
#include "LayerImporter.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vec3.h"
#include "Vec4.h"

enum OperationType{ otNone, otDDA, otBresenham };

OperationType g_OperationType = otNone;//��ǰ��������
Dataset g_Dataset;//���ݼ�

Box3D g_box3D( -1, -1, -1, 1, 1 , 1 );
Vec3 g_eye(0,0,3), g_center(0,0,0), g_up(0,1,0);

Matrix3 g_transformMatrix;//�任����
Matrix4 g_modelMatrix;//3Dģ�ͱ任����
Matrix4 g_viewMatrix;//3D��ͼ�任����
Matrix4 g_projectMatrix;//3DͶӰ�任����
Matrix4 g_transformMatrix3D;//3D�ܱ任����

void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool openLayer();

LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		handleCommandMessage(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
		handlePaintMessage(hWnd, message, wParam, lParam);
		break;
	case WM_CREATE:
		init();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		handleKeyMessage(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MOUSEWHEEL:
		handleMouseMessage(hWnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	switch (wmId)
	{
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case ID_FILE_OPEN:
		if( openLayer())//���ļ�
		{
			if( g_Dataset.getLayerCount() == 1 )//��ӵ�һ��ͼ��ʱ���ó�ʼ�任����
			{
				Layer* pLayer = g_Dataset[0];

				double dx = -pLayer->bbox.centerX();
				double dy = -pLayer->bbox.centerY();
				g_transformMatrix.makeTranslate( dx, dy );//���������Ƶ��۲�����ϵԭ��

			}
			refreshWindow();//ˢ��
		}
		break;
	case ID_DDA:
		g_OperationType = otDDA;
		setRubberMode( rmLine);
		setCursor(csArrow);
		//refreshWindow();
		break;
	case ID_BRESENHAM:
		g_OperationType = otBresenham;
		//refreshWindow();
		setCursor(csHand);
		setRubberMode(rmRectangle);
		break;
	}
}

void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int key = wParam;
	switch (key)
	{
	case VK_UP: // ��һ�У��Ϲ��			
		if( g_Dataset.getLayerCount() > 0 )
		{
			g_transformMatrix *= Matrix3::translate( 0 , g_Dataset[0]->bbox.height() / 10.0 ); // �µ��ۻ����� = ԭ�ۻ����� * ƽ�ƾ���
			refreshWindow();
		}		
		break;
	case VK_DOWN:
		if( g_Dataset.getLayerCount() > 0 )
		{
			g_transformMatrix *= Matrix3::translate( 0 , -g_Dataset[0]->bbox.height() / 10.0 ); // �µ��ۻ����� = ԭ�ۻ����� * ƽ�ƾ���
			refreshWindow();
		}		
		break;
	case VK_LEFT:
		if( g_Dataset.getLayerCount() > 0 )
		{
			g_transformMatrix *= Matrix3::translate( -g_Dataset[0]->bbox.width() / 10.0, 0 ); // �µ��ۻ����� = ԭ�ۻ����� * ƽ�ƾ���
			refreshWindow();
		}		
		break;
	case VK_RIGHT:
		if( g_Dataset.getLayerCount() > 0 )
		{
			g_transformMatrix *= Matrix3::translate( g_Dataset[0]->bbox.width() / 10.0, 0 );// �µ��ۻ����� = ԭ�ۻ����� * ƽ�ƾ���
			refreshWindow();
		}		
		break;
	}
}

void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:	
		{
			int x = LOWORD(lParam);
			int y = HIWORD( lParam);
			DPtToLPt( x,y,x,y);
		}
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
	{
		if (getRubberMode() == rmNone) return;

		refreshWindow();
		
	}
	case WM_MOUSEWHEEL:
		{
			short det = (short)HIWORD(wParam);
			if( det > 0 )
				g_transformMatrix *= Matrix3::scale( 0.9, 0.9 );// �µ��ۻ����� = ԭ�ۻ����� * ��������
			else
				g_transformMatrix *= Matrix3::scale( 1.1, 1.1 );// �µ��ۻ����� = ԭ�ۻ����� * ��������

			Vec3 viewline = g_eye - g_center;
			viewline.normalize();

			Vec3 d = viewline * 1.0;
			if( det > 0 )
			{		
				g_eye += d;
				g_center += d;
			}
			else
			{
				g_eye -= d;
				g_center -= d;
			}
			refreshWindow();
		}
		break;
	}
	
}

void drawCell(int row, int col, unsigned color);
//void drawGrid();
void drawDataset( Matrix3& finalTransformMatrix);

void handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	setYUp(true);//y������
	setOrig( getWindowWidth() / 2, getWindowHeight() / 2 );// ԭ��Ϊ��������

	if( g_Dataset.getLayerCount() > 0 )//���ڴ��ڴ�С���ܷ����仯�����ڴ��ټ���任�����������վ���
	{
		Layer* pLayer = g_Dataset[0];
		double sx = getWindowWidth() / pLayer->bbox.width() ;
		double sy = getWindowHeight() / pLayer->bbox.height() ;
		double s = sx > sy ? sy : sx;
		Matrix3 finalTransformMatrix = g_transformMatrix * Matrix3::scale( s, s );//���������Ա����������ϵһ��

		drawDataset( finalTransformMatrix );//�任���������ݼ�������ͼ��

	}	

	//	
	//	extern HDC g_hDC;

	//	for ( int i = 0 ; i < 4 ; ++i )
	//	{
	//		MoveToEx( g_hDC, pts[i].x, pts[i].y ,0 );
	//		LineTo( g_hDC,  pts[(i+1)%4].x, pts[(i+1)%4].y  );
	//	}

	//	for ( int i = 0 ; i < 4 ; ++i )
	//	{
	//		MoveToEx( g_hDC, pts[4+i].x, pts[4+i].y ,0 );
	//		LineTo( g_hDC,  pts[4+(i+1)%4].x, pts[4+(i+1)%4].y  );
	//	}

	//	for ( int i = 0 ; i < 4 ; ++i )
	//	{
	//		MoveToEx( g_hDC, pts[i].x, pts[i].y ,0 );
	//		LineTo( g_hDC,  pts[(i+4)].x, pts[(i+4)].y  );
	//	}


	//}

	//return;

	//switch (g_OperationType)
	//{
	//	case otNone:
	//		//drawGrid();
	//		break;
	//	case otDDA:
	//	{
	//		int x1, y1, x2, y2;
	//		getRubberPoints(x1, y1, x2, y2);
	//		for (int i = x1; i < x1 + 50; ++i )
	//		{
	//			for (int j = y1; j < y1 + 50; ++j )
	//			{
	//				setPixel(i, j, 255);
	//			}
	//		}

	//		for (int i = x2; i < x2 + 50; ++i)
	//		{
	//			for (int j = y2; j < y2 + 50; ++j)
	//			{
	//				setPixel(i, j, 255);
	//			}
	//		}
	//		//drawGrid();
	//	}
	//	break;
	//	case otBresenham:
	//		{
	//			int x1, y1, x2, y2;
	//			getRubberPoints(x1, y1, x2, y2);

	//			for (int i = x2; i < x2 + 50; ++i )
	//			{
	//				for (int j = y1; j < y1 + 50; ++j )
	//				{
	//					setPixel(i, j, 255);
	//				}
	//			}

	//			for (int i = x1; i < x1 + 50; ++i)
	//			{
	//				for (int j = y2; j < y2 + 50; ++j)
	//				{
	//					setPixel(i, j, 255);
	//				}
	//			}

	//		}
	//		break;
	//}
}

//ѡ�񲢴�ͼ��
bool openLayer()
{
	char path[MAX_PATH]={0};
	if( Utils::selectSingleFile( "", "shp", path ,MAX_PATH ))
	{
		g_Dataset.addLayer( LayerImporter::importShpLayer( path ));
		return true;
	}
	return false;
}

void drawLayer( Matrix3& finalTransformMatrix, Layer* pLayer, unsigned color );

//�������ݼ�������ͼ��
void drawDataset( Matrix3& finalTransformMatrix)
{
	for ( int i = 0 ; i < g_Dataset.getLayerCount(); ++i )
	{
		unsigned color = 255;
		if( i == 1 ) color = GREEN;
		else if( i == 2 ) color = BLUE;

		drawLayer( finalTransformMatrix, g_Dataset[i], color );
	}
}

//����һ��ͼ��
void drawLayer( Matrix3& finalTransformMatrix, Layer* pLayer, unsigned color )
{
	switch( pLayer->geomType )
	{
	case gtPoint:
		{
			for ( int i = 0, size = pLayer->getGeometryCount(); i < size; ++i )
			{
				PointGemetry<double>* pGemetry = (PointGemetry<double>*)(*pLayer)[i];
				Vec3 v( pGemetry->x, pGemetry->y );//��ԭʼ���깹���������
				Vec3 v2 = v * finalTransformMatrix;//�任
				setPixel( v2.x(), v2.y(), color );//����
			}
		}
		break;
	case gtPolyline:
		{
			for ( int i = 0, size = pLayer->getGeometryCount(); i < size; ++i )
			{
				PolylineGeometry<double>* pGemetry = (PolylineGeometry<double>*)(*pLayer)[i];
				for ( int j = 0, ptsCount = pGemetry->pts.size(); j < ptsCount; ++j )
				{
					Point2D<double>& pt = (*pGemetry)[j];//����߶����ԭʼ����
					Vec3 v( pt.x, pt.y );//��ԭʼ���깹���������
					Vec3 v2 = v * finalTransformMatrix;//�任
					setPixel( v2.x(), v2.y(), color );//����
				}				
			}
		}
		break;
	case gtPolygon:
		{
			for ( int i = 0, size = pLayer->getGeometryCount(); i < size; ++i )
			{
				PolygonGeometry<double>* pGemetry = (PolygonGeometry<double>*)(*pLayer)[i];
				for ( int j = 0, ptsCount = pGemetry->pts.size(); j < ptsCount; ++j )
				{
					Point2D<double>& pt = (*pGemetry)[j];//��ö���ζ����ԭʼ����
					Vec3 v( pt.x, pt.y );//��ԭʼ���깹���������
					Vec3 v2 = v * finalTransformMatrix;//�任
					setPixel( v2.x(), v2.y(), color );//����
				}
			}
		}
		break;
	}

	Vec3 v1( pLayer->bbox.xmin, pLayer->bbox.ymin );//���½ǵ�
	Vec3 v2( pLayer->bbox.xmax, pLayer->bbox.ymax );//���Ͻǵ�
	Vec3 v11 = v1 * finalTransformMatrix;//�任���½ǵ�
	Vec3 v21 = v2 * finalTransformMatrix;//�任���Ͻǵ�

	//���»���ͼ��߿򣬽����ڲ���
	for( int i = v11.x(); i < v21.x(); ++i)setPixel( i, v11.y(), YELLOW );//���Ƶױ�
	for( int i = v11.x(); i < v21.x(); ++i)setPixel( i, v21.y(), YELLOW );//���ƶ���
	for( int i = v11.y(); i < v21.y(); ++i)setPixel( v11.x(), i, YELLOW );//�������
	for( int i = v11.y(); i < v21.y(); ++i)setPixel( v21.x(), i, YELLOW );//�����ұ�
}

void drawCell(int row, int col, unsigned color)
{
	int gridWidth = 5;

	int h = getWindowHeight();
	for (int i = row; i < (row + 1)* gridWidth; ++i)
	{
		for (int j = col; j < (col + 1)* gridWidth; ++j)
		{
			//if ((row + col) % 2 == 0)
				setPixel(j, i, color);
		}
	}

	//for (int i = row* gridWidth; i < (row + 1)* gridWidth; ++i)
	//{
	//	for (int j = col* gridWidth; j < (col + 1)* gridWidth; ++j)
	//	{
	//		if ((row + col) % 2 == 0)
	//			setPixel(i, h - j, color);
	//	}
	//}
}
//
//void drawGrid()
//{
//	int gridWidth = 10;
//
//	int w = getWindowWidth() / gridWidth + 1;
//	int h = getWindowHeight() / gridWidth + 1;
//	for (int i = 0; i < w; ++i)
//	{
//		for (int j = 0; j < h; ++j)
//		{
//			drawCell(i, j, 0xFF);
//		}
//	}
//}