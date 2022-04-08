
#include "�Զ��庯��.h"
#include"Graphic.h"
#include"�������.h"
#include"�ⲿ��Ϣ.h"
#include"Utils.h"
#include"LayerImporter.h"
#include"�ü�.h"
#define PixelSelfDefine 40
Dataset g_Dataset;
int num;
static double dx, dy,s;
l_renderstate state; int state1 = 0,a,num1;
c_renderstate renderstate;
p_renderstate polygonstate;
a_renderstate areastate;
vector2 vector2state;
Matrix coeffientmatrix;
Matrix example;

Matrix s_transformMatrix;

PixelPoint pts[100];
PixelPoint pt[100];
void setcoffT(int x,int y) {
	coeffientmatrix.settransformparam( x,y);
}
void setcoffR(int x) {
	coeffientmatrix.setrotate(x);
}
bool openLayer() {
	char path[MAX_PATH] = { 0 };
	if (Utils::selectSingleFile("", "shp", path, MAX_PATH)) {
		g_Dataset.addLayer(LayerImporter::importShpLayer(path));
		return true;
	};
	return false;
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
void drawLayer(Matrix& finalTransformMatrix, Layer* pLayer, unsigned color, Matrix& s_TransformMatrix);
void drawDataset(Matrix& finalTransformMatrix, Matrix& s_TransformMatrix);
void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{

		handleCommandMessage( hWnd, message,  wParam,  lParam);
	}
	break;
	case WM_CREATE:
		init();
		break;
	
	case WM_PAINT:
	{
		handlePaintMessage(hWnd, message, wParam, lParam);
	}
	break;
	case WM_KEYDOWN:
		handleKeyMessage(hWnd, message, wParam, lParam);
		refreshWindow();
		break;
	case WM_MOUSEWHEEL:
		handleMouseMessage(hWnd, message, wParam, lParam);
		refreshWindow();
		break;
	
	case WM_LBUTTONUP:
		handleMouseMessage(hWnd, message, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		handleMouseMessage(hWnd, message, wParam, lParam);
		refreshWindow();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId = LOWORD(wParam);
	// �����˵�ѡ��: 
	switch (wmId)
	{
	case ID_FileOpen:
		example.setmatrix();
		coeffientmatrix.setmatrix();
		if (openLayer())//���ļ�
		{
			
			
			s_transformMatrix.setmatrix();
			
			if (g_Dataset.getLayerCount() == 1)//��ӵ�һ��ͼ��ʱ���ó�ʼ�任����
			{
			
				Layer* pLayer = g_Dataset[/*g_Dataset.getLayerCount()-1*/0];
				dx = -pLayer->bbox.centerX();
			    dy = -pLayer->bbox.centerY();
				coeffientmatrix.makeTranslate(dx, dy);//���������Ƶ��۲�����ϵԭ��
				
			}
			
			refreshWindow();//ˢ��
		}
		break;
	
	case ID_Tailor:
		setRubberMode(rmPolygon);
		polygonstate.operater = p_polygon;
		polygonstate.polygoner = p_ET;
		state1 = 3;
		refreshWindow();
		break;
	case ID_DIGUITIANCHONG:
		setRubberMode(rmPolygon);
		areastate.operater = p_area;
		areastate.areafill = a_recursion;
		refreshWindow();
		break;
	case ID_FEIDIGUITIANCHONG:
		setRubberMode(rmPolyline);
		areastate.operater = p_area;
		areastate.areafill = a_norecursion;
		refreshWindow();
		break;
	case ID_SAOMIAOXIANTIANCHONG:
		setRubberMode(rmPolygon);
		areastate.operater = p_area;
		areastate.areafill = a_scanningline;
		refreshWindow();
		break;
	case ID_Edag:
		setRubberMode(rmPolygon);
		polygonstate.operater = p_polygon;
		polygonstate.polygoner = p_ET;
		refreshWindow();
		break;
	case ID_XRAY:
		setRubberMode(rmPolygon);
		polygonstate.operater = p_polygon;
		polygonstate.polygoner = p_xray;
		refreshWindow();

		break;
	case ID_TUOYUAN:
		setRubberMode(rmPolyline);
		state1 = 2;
		refreshWindow();
		break;
	case ID_circleBresenham:
		setRubberMode(rmLine);
		renderstate.operater = P_circle;
		renderstate.Circleoperate = c_MidBresenham;
		refreshWindow();

		break;
	case ID_CRefresh:
		state.operater = None;
		state1 = 0;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_LRefresh:

		state1 = 0;
		renderstate.operater = None;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_TRefresh:
		state.operater = None;
		state1 = 2;
		renderstate.operater = None;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_pRefresh:
		state.operater = None;
		state1 = 0;
		renderstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_aRefresh:
		state.operater = None;
		state1 = 0;
		renderstate.operater = None;
		polygonstate.operater = None;
		refreshWindow();
		break;
	case ID_circlepoint:
		setRubberMode(rmLine);
		renderstate.operater = P_circle;
		renderstate.Circleoperate = c_eightCircle;
		refreshWindow();

		break;
	case ID_Bresenham:
		setRubberMode(rmLine);
		state.operater = P_line;
		state.Lineoperate = l_Bresenham;
		refreshWindow();

		break;
	case ID_MidBresenham:
		setRubberMode(rmLine);
		state.operater = P_line;
		state.Lineoperate = l_MidBresenham;
		refreshWindow();
		break;
	case ID_HUAQIPAN:
		state1 = 1;
		state.operater = None;
		state.Lineoperate = l_None;
		refreshWindow();
		break;
	case ID_DDA:
		setRubberMode(rmLine);
		state.operater = P_line;
		state.Lineoperate = l_DDALine;
		refreshWindow();
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	
	}
}
void handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
	if (state1 == 1) {

		drawline();
		state1 = 0;
	}
	if (state1 == 2) {
		drawline();
		int r1 = sqrt((pts[1].y - pts[0].y)*(pts[1].y - pts[0].y)*1.0 + (pts[1].x - pts[0].x)*(pts[1].x - pts[0].x)*1.0);
		int r2 = sqrt((pts[2].y - pts[1].y)*(pts[2].y - pts[1].y)*1.0 + (pts[2].x - pts[1].x)*(pts[2].x - pts[1].x)*1.0);
		MidBresenhamEllipse(max(r1,r2), min(r1,r2), BLACK,pts[1]);

	}
	if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_ET && state1 == 3) {
		AET(BLACK, pts, num);
		PixelPoint temp[100];
		for (int i = 0; i < num; i++) {
			temp[i].x = pts[i].x;
			temp[i].y = pts[i].y;
		}
		Ptailor(pts, pt);
	}
	if (areastate.operater == p_area && areastate.areafill == a_recursion) {
		drawline();
		AET(BLACK, pts, num);
		if (num != 0) {
			FloodFill4(pts[0].x + 1, pts[0].y + 1, BLACK, BLUE);
		}
		
		
	}
	if (areastate.operater == p_area && areastate.areafill == a_norecursion) {
		drawline();
		int r1 = sqrt((pts[1].y - pts[0].y)*(pts[1].y - pts[0].y)*1.0 + (pts[1].x - pts[0].x)*(pts[1].x - pts[0].x)*1.0);
		int r2 = sqrt((pts[2].y - pts[1].y)*(pts[2].y - pts[1].y)*1.0 + (pts[2].x - pts[1].x)*(pts[2].x - pts[1].x)*1.0);
		MidBresenhamEllipse(max(r1, r2), min(r1, r2), BLACK, pts[1]);
		if(num!=0)
		areaNoRecursion(pts[1].x , pts[1].y, GREEN, BLACK);

	}
	if (areastate.operater == p_area && areastate.areafill == a_scanningline) {
		drawline();
		x_ray(BLACK, pts, num);
		if(num!=0)
		scanningline(pts[0].x+1, pts[0].y, GREEN, BLACK);

	}
	if (renderstate.operater == P_circle && renderstate.Circleoperate == c_eightCircle) {
		drawline();
		circlePoint(pts[1].x, pts[1].y, BLACK,pts[0]);

	}
	if (renderstate.operater == P_circle && renderstate.Circleoperate == c_MidBresenham) {
		drawline();
		int r=sqrt((pts[1].y-pts[0].y)*(pts[1].y - pts[0].y)*1.0+ (pts[1].x - pts[0].x)*(pts[1].x - pts[0].x)*1.0);
		MidBresenhamCircle(r, BLACK,pts[0]);
	}
	if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_xray) {
		drawline();
		
		x_ray(BLACK,pts,num);

	}
	if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_ET) {
		drawline();
		AET(BLACK,pts, num);

	}
	if (state.operater == P_line && state.Lineoperate == l_DDALine) {
		
		
			drawline();
			DDALine(pts[0].x, pts[1].x, pts[0].y, pts[1].y, BLACK);

	}
	if (state.operater == P_line && state.Lineoperate == l_MidBresenham) {
		drawline();
		MidBresenham(pts[0].x, pts[1].x, pts[0].y, pts[1].y, BLACK);
		
	}
	if (state.operater == P_line && state.Lineoperate == l_Bresenham) {
		drawline();
		Bresenham(pts[0].x, pts[1].x, pts[0].y, pts[1].y, BLACK);
	}

		if (g_Dataset.getLayerCount() > 0)//���ڴ��ڴ�С���ܷ����仯�����ڴ��ټ���任�����������վ���
		{
			Layer* pLayer = g_Dataset[0];

			double sx = getWindowWidth() / pLayer->bbox.width();
			double sy = getWindowHeight() / pLayer->bbox.height();
			s = sx > sy ? sy : sx;

			//���������Ա����������ϵһ��
			coeffientmatrix.setscale(s, s);
			drawDataset(coeffientmatrix,s_transformMatrix);//�任���������ݼ�������ͼ��

		}
	
	EndPaint(hWnd, &ps);
};
void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int key = wParam;
	switch (key)
	{
	case VK_RIGHT:
		if (g_Dataset.getLayerCount() > 0) {

			dx += g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);

		}

		break;
	case VK_LEFT:
		if (g_Dataset.getLayerCount() > 0) {

			dx -= g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);

		}
		break;
	case VK_UP:
		if (g_Dataset.getLayerCount() > 0) {
			dy -= g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);
		}


		break;
	case VK_DOWN:
		if (g_Dataset.getLayerCount() > 0) {
			dy += g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);
		}

		break;
	case  0x41:
		if (g_Dataset.getLayerCount() > 0) {
			example.setrotate(5);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		break;
	case 0x44:
		if (g_Dataset.getLayerCount() > 0) {
			example.setrotate(-5);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		break;
	case VK_SPACE:
		if (g_Dataset.getLayerCount() > 0) {
			example.setsymmetry(-1, 0, 0, -1);
			s_transformMatrix *= example;
			example.setmatrix();

			break;
	case 0x45:
		if (g_Dataset.getLayerCount() > 0) {
			example.setshear(0, 1);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		break;
	case 0x51:
		if (g_Dataset.getLayerCount() > 0) {
			example.setshear(0, -1);
			s_transformMatrix *= example;
			example.setmatrix();
		}

		break;
	default:
		break;
		}
	}
};
void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	short det = (short)HIWORD(wParam);
	switch (message)
	{
		
	case WM_MOUSEWHEEL:
		
		if (det > 0)
		{
			
			example.setscale(1.1, 1.1);
			s_transformMatrix *= example;
			example.setmatrix();
			
		}
		else
		{
			example.setscale(0.9, 0.9);
			s_transformMatrix *= example;
			example.setmatrix();
			
		}
			break;
	case WM_LBUTTONUP:
		if (getRubberMode() !=rmNone) {
			
			num = getRubberPoints(pts);
			
		}

		refreshWindow();
			break;
	case WM_RBUTTONUP:
		if (getRubberMode() != rmNone) {

			num1 = getRubberPoints(pt);

		}

		refreshWindow();
		break;
	}
	
};
void drawDataset(Matrix& finalTransformMatrix,
	Matrix& s_TransformMatrix)
{
	for (int i = 0; i < g_Dataset.getLayerCount(); i++)
	{
		unsigned color = 255;
		if (i == 1) color = GREEN;
		else if (i == 2) color = BLUE;
		
		drawLayer(finalTransformMatrix, g_Dataset[i], color,s_TransformMatrix);
	}
}

void drawLayer(Matrix& finalTransformMatrix, Layer* pLayer, unsigned color, 
	Matrix& s_TransformMatrix)
{
	
	finalTransformMatrix.setscale(s, s);
	switch (pLayer->geomType)
	{
	case gtPoint:
	{
		Matrix v(1,3,false);
		for (int i = 0, size = pLayer->getGeometryCount(); i < size; i++)
		{
			PointGemetry<double>* pGemetry = (PointGemetry<double>*)(*pLayer)[i];

			//��ԭʼ���깹���������
			v.setrow(pGemetry->x, pGemetry->y, 1, 0);
		    finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
			v*=finalTransformMatrix;
			finalTransformMatrix.setmatrix();
			finalTransformMatrix.setscale(s, s);
			v *= finalTransformMatrix;
			v *= s_TransformMatrix;
			
			finalTransformMatrix.setmatrix();
			v *= finalTransformMatrix;//�任
			finalTransformMatrix.setmatrix();
			finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
			
			v *= finalTransformMatrix;
			setPixel(v.getrow(0), v.getcol(0), color);//����
		}
	}
	break;
	case gtPolyline:
	{
		Matrix v(1, 3, false);
		for (int i = 0, size = pLayer->getGeometryCount(); i < size; i++)
		{
			
			PolylineGeometry<double>* pGemetry = (PolylineGeometry<double>*)(*pLayer)[i];
			for (int j = 0, ptsCount = pGemetry->pts.size(); j < ptsCount; j++)
			{
				Point2D<double>& pt = (*pGemetry)[j];//����߶����ԭʼ����
				//��ԭʼ���깹���������
				v.setrow(pt.x, pt.y, 1, 0);
				finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
				v*=finalTransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.setscale(s, s);
				v *= finalTransformMatrix;
				v *= s_TransformMatrix;
			
				finalTransformMatrix.setmatrix();
				v *= finalTransformMatrix;//�任
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
			
				v *= finalTransformMatrix;
				setPixel(v.getrow(0), v.getcol(0), color);//����
			}
		}
	}
	break;
	case gtPolygon:
	{
		Matrix v(1, 3, false);
		for (int i = 0, size = pLayer->getGeometryCount(); i < size; i++)
		{	
			PolygonGeometry<double>* pGemetry = (PolygonGeometry<double>*)(*pLayer)[i];
			for (int j = 0, ptsCount = pGemetry->pts.size(); j < ptsCount; ++j)
			{
				Point2D<double>& pt = (*pGemetry)[j];//��ö���ζ����ԭʼ����
				//��ԭʼ���깹���������
				v.setrow(pt.x, pt.y, 1, 0);
				finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
				v *= finalTransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.setscale(s, s);
				v *= finalTransformMatrix;
				
				v *= s_TransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
				v *= finalTransformMatrix;//�任
				setPixel(v.getrow(0), v.getcol(0), color);//����
			}
		}
	}
	break;
	}

	Matrix v1(1,3,false);//���½ǵ�
	v1.setrow(pLayer->bbox.xmin, pLayer->bbox.ymin, 1, 0);

	Matrix v2(1, 3, false);
	v2.setrow(pLayer->bbox.xmax, pLayer->bbox.ymax,1,0);//���Ͻǵ�
	finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
	v1 *= finalTransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.setscale(s, s);
	v1 *= finalTransformMatrix;
	
	v1 *= s_TransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
	v1 *= finalTransformMatrix;//�任
	finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
	v2 *= finalTransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.setscale(s, s);
	v2 *= finalTransformMatrix;
	
	v2 *= s_TransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
	v2 *= finalTransformMatrix;//�任

										 //���»���ͼ��߿򣬽����ڲ���
	for (int i = v1.getrow(0); i < v2.getrow(0); i++)setPixel(i, v1.getcol(0), YELLOW);//���Ƶױ�
	for (int i = v1.getrow(0); i < v2.getrow(0); i++)setPixel(i, v2.getcol(0), YELLOW);//���ƶ���
	for (int i = v1.getcol(0); i < v2.getcol(0); i++)setPixel(v1.getrow(0), i, YELLOW);//�������
	for (int i = v1.getcol(0); i < v2.getcol(0); i++)setPixel(v2.getrow(0), i, YELLOW);//�����ұ�
	finalTransformMatrix.setmatrix();
}
