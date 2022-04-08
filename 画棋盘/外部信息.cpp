
#include "自定义函数.h"
#include"Graphic.h"
#include"矩阵操作.h"
#include"外部信息.h"
#include"裁剪.h"
#include"Utils.h"
#include"LayerImporter.h"
#include"三维矩阵操作.h"
Dataset g_Dataset;
int num, x3D = 0, y3D = 0, angle3D = 10;
double scale = 1;
int state1 = 0,a,num1,count1=0;
static double dx, dy,s;
DEM typer;
l_renderstate state; 
c_renderstate renderstate;
p_renderstate polygonstate;
a_renderstate areastate;
vector2 vector2state;
Matrix coeffientmatrix;
Matrix example;
Matrix s_transformMatrix;
Matrix4 rectangle1(8, 4);//实例矩阵
Matrix4 g_transformMatrix3(4, 4);//几何变换
Matrix4 g_transformMatrix2(4,4);//规范化投影中的几何变换
Matrix4 g_transformMatrix1(4, 4);//视口变换
Matrix4 projection(4, 4);//投影变换
Matrix4 eye(1, 4);//注视点
Matrix4 center(1, 4);//视点
Matrix4 up(1, 4);//辅助向量
Matrix4 d(1, 4);//暂时不知道干嘛用
PixelPoint pts[100];
PixelPoint pt[100];
pixel3D p1[4], p2[4], p3[4],p4[4],p5[4],p6[4];
unsigned int**zhenbuffer;//缓冲器
double**z_buffer;//缓冲器
void setcoffT(int x,int y) {
	coeffientmatrix.settransformparam( x,y);
}
void bubblesort(pixel3D& a,pixel3D& b) {
	pixel3D c;
	c.x = a.x;
	c.y = a.y;
	c.z = a.z;
	a.x = b.x;
	a.y = b.y;
	a.z = b.z;
	b.x = c.x;
	b.y = c.y;
	b.z = c.z;
}
void setcoffR(int x) {
	coeffientmatrix.setrotate(x);
}
void createRectangle(){
	
	rectangle1.setrow(0, 50, 50, -10, 1);
	rectangle1.setrow(1, 10, 50, -10, 1);
	rectangle1.setrow(2, 10, 10, -10, 1);
	rectangle1.setrow(3, 50, 10, -10, 1);
	rectangle1.setrow(4, 50, 50, -20, 1);
	rectangle1.setrow(5, 10, 50, -20, 1);
	rectangle1.setrow(6, 10, 10, -20, 1);
	rectangle1.setrow(7, 50, 10, -20, 1);
}
void setperspective(){
	eye.setrow(0, 0, 0, 1, 1);
	center.setrow(0, 0, 0, 0, 1);
	up.setrow(0, 0, 1, 0, 1);
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
void drawLayer(Matrix& finalTransformMatrix, Layer* pLayer, unsigned color, Matrix& s_TransformMatrix);
void drawDataset(Matrix& finalTransformMatrix, Matrix& s_TransformMatrix);
void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT /*CALLBACK WndProc*/handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
void resetZ_buffer() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			zhenbuffer[i][j] = WHITE;
		}
	}
	for (int i = 0; i <height ; i++) {
		for (int j = 0; j < width; j++) {
			z_buffer[i][j] = 21000.0;
		}
	}
}

void handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int wmId = LOWORD(wParam);
	// 分析菜单选择: 
	switch (wmId)
	{
	
	case ID_TS3D:
		//缓冲器建立
		setOrig(0 , 0);
		zhenbuffer = new unsigned int*[height];
		for (int j = 0; j < height; j++) {
			zhenbuffer[j] = new unsigned int[width];
			
		}
		
		z_buffer = new double*[height];
		for (int j = 0; j < height; j++) {
			z_buffer[j] = new double[width];
		
		}
		resetZ_buffer();
		typer.type = threeD;
		g_transformMatrix2.setmatrix4();
		g_transformMatrix3.setmatrix4();
		
		refreshWindow();
		break;
	case ID_FileOpen:
		typer.type = Map;
		example.setmatrix();
		coeffientmatrix.setmatrix();
		if (openLayer())//打开文件
		{
			
			
			s_transformMatrix.setmatrix();
			
			if (g_Dataset.getLayerCount() == 1)//添加第一个图层时设置初始变换矩阵
			{
			
				Layer* pLayer = g_Dataset[/*g_Dataset.getLayerCount()-1*/0];
				dx = -pLayer->bbox.centerX();
			    dy = -pLayer->bbox.centerY();
				coeffientmatrix.makeTranslate(dx, dy);//窗口中心移到观察坐标系原点
				
			}
			
			refreshWindow();//刷新
		}
		break;
	

	case ID_DIGUITIANCHONG:
		typer.type = twoD;
		setRubberMode(rmPolygon);
		areastate.operater = p_area;
		areastate.areafill = a_recursion;
		refreshWindow();
		break;
	case ID_FEIDIGUITIANCHONG:
		typer.type = twoD;
		setRubberMode(rmPolyline);
		areastate.operater = p_area;
		areastate.areafill = a_norecursion;
		refreshWindow();
		break;
	case ID_SAOMIAOXIANTIANCHONG:
		typer.type = twoD;
		setRubberMode(rmPolygon);
		areastate.operater = p_area;
		areastate.areafill = a_scanningline;
		refreshWindow();
		break;
	case ID_Edag:
		typer.type = twoD;
		setRubberMode(rmPolygon);
		polygonstate.operater = p_polygon;
		polygonstate.polygoner = p_ET;
		refreshWindow();
		break;
	case ID_XRAY:
		typer.type = twoD;
		setRubberMode(rmPolygon);
		polygonstate.operater = p_polygon;
		polygonstate.polygoner = p_xray;
		refreshWindow();

		break;
	case ID_TUOYUAN:
		typer.type = twoD;
		setRubberMode(rmPolyline);
		state1 = 2;
		refreshWindow();
		break;
	case ID_circleBresenham:
		typer.type= twoD;
		setRubberMode(rmLine);
		renderstate.operater = P_circle;
		renderstate.Circleoperate = c_MidBresenham;
		refreshWindow();

		break;
	case ID_CRefresh:
		typer.type = twoD;
		state.operater = None;
		state1 = 0;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_LRefresh:
		typer.type = twoD;
		state1 = 0;
		renderstate.operater = None;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_TRefresh:
		typer.type = twoD;
		state.operater = None;
		state1 = 2;
		renderstate.operater = None;
		polygonstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_pRefresh:
		typer.type = twoD;
		state.operater = None;
		state1 = 0;
		renderstate.operater = None;
		areastate.operater = None;
		refreshWindow();
		break;
	case ID_aRefresh:
		typer.type = twoD;
		state.operater = None;
		state1 = 0;
		renderstate.operater = None;
		polygonstate.operater = None;
		refreshWindow();
		break;
	case ID_circlepoint:
		typer.type = twoD;
		setRubberMode(rmLine);
		renderstate.operater = P_circle;
		renderstate.Circleoperate = c_eightCircle;
		refreshWindow();

		break;
	case ID_Bresenham:
		typer.type = twoD;
		setRubberMode(rmLine);
		state.operater = P_line;
		state.Lineoperate = l_Bresenham;
		refreshWindow();

		break;
	case ID_MidBresenham:
		typer.type = twoD;
		setRubberMode(rmLine);
		state.operater = P_line;
		state.Lineoperate = l_MidBresenham;
		refreshWindow();
		break;
	case ID_HUAQIPAN:
		typer.type = twoD;
		state1 = 1;
		state.operater = None;
		state.Lineoperate = l_None;
		refreshWindow();
		break;
	case ID_DDA:
		typer.type = twoD;
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
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	
	
	if (typer.type == twoD) {
		if (state1 == 1) {

			drawline();
			state1 = 0;
		}
		if (state1 == 2) {
			drawline();
			int r1 = sqrt((pts[1].y - pts[0].y)*(pts[1].y - pts[0].y)*1.0 + (pts[1].x - pts[0].x)*(pts[1].x - pts[0].x)*1.0);
			int r2 = sqrt((pts[2].y - pts[1].y)*(pts[2].y - pts[1].y)*1.0 + (pts[2].x - pts[1].x)*(pts[2].x - pts[1].x)*1.0);
			MidBresenhamEllipse(max(r1, r2), min(r1, r2), BLACK, pts[1]);

		}
		//if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_ET && state1 == 3) {
		//	AET(BLACK, pts, num);
		//	count1++;
		//	/*num = num1 = 3;*/
		//	if (num != 0 && num1 != 0)
		//		Ptailor(pt, pts, num1, num);

		//}
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
			if (num != 0)
				areaNoRecursion(pts[1].x, pts[1].y, GREEN, BLACK);

		}
		if (areastate.operater == p_area && areastate.areafill == a_scanningline) {
			drawline();
			x_ray(BLACK, pts, num);
			if (num != 0)
				scanningline(pts[0].x + 1, pts[0].y, GREEN, BLACK);

		}
		if (renderstate.operater == P_circle && renderstate.Circleoperate == c_eightCircle) {
			drawline();
			circlePoint(pts[1].x, pts[1].y, BLACK, pts[0]);

		}
		if (renderstate.operater == P_circle && renderstate.Circleoperate == c_MidBresenham) {
			drawline();
			int r = sqrt((pts[1].y - pts[0].y)*(pts[1].y - pts[0].y)*1.0 + (pts[1].x - pts[0].x)*(pts[1].x - pts[0].x)*1.0);
			MidBresenhamCircle(r, BLACK, pts[0]);
		}
		if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_xray) {
			drawline();

			x_ray(BLACK, pts, num);

		}
		if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_ET) {
			drawline();
			AET(BLACK, pts, num);

		}
		if (state.operater == P_line && state.Lineoperate == l_DDALine) {


			drawline();
			if (num != 0)
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
	}
	if (typer.type == Map) {
		if (g_Dataset.getLayerCount() > 0)//由于窗口大小可能发生变化，故在此再计算变换到视区的最终矩阵
			{
			Layer* pLayer = g_Dataset[0];

			double sx = getWindowWidth() / pLayer->bbox.width();
			double sy = getWindowHeight() / pLayer->bbox.height();
			s = sx > sy ? sy : sx;

			//比例缩放以便和视区坐标系一致
			coeffientmatrix.setscale(s, s);
			drawDataset(coeffientmatrix,s_transformMatrix);//变换并绘制数据集包含的图层

		}
	}
		
		if (typer.type == threeD) {
			//矩阵变换
			type fovy = 120, aspectRatio = (double)getWindowWidth() / getWindowHeight(), zNear = 10, zFar = 100;
			createRectangle();
			setperspective();
			projection.project(fovy, aspectRatio, zNear, zFar);
			g_transformMatrix1.UVN(up, eye, center);
			g_transformMatrix1 *= projection;
			g_transformMatrix2.setscale(0.5, 0.5, 1);
			g_transformMatrix1 *= g_transformMatrix2;
			rectangle1 *= g_transformMatrix1;
			rectangle1.nomalized();
			rectangle1.shikoubianhuan();		
			rectangle1 *= g_transformMatrix3;
			
			for (int i = 0; i < 4; i++)
			{

				p1[i].x = rectangle1.getX(i);
				p1[i].y = rectangle1.getY(i);
				p1[i].z = rectangle1.getZ(i);
			}
			int j = 0;
			for (int i = 0; i < 8; i++)
			{
				if (i==4||i==5||i==6||i==7) {
					p2[j].x = rectangle1.getX(i);
					p2[j].y = rectangle1.getY(i);
					p2[j].z = rectangle1.getZ(i);
					j++;
				}
				
			}
			j = 0;
			for (int i = 0; i < 8; i++)
			{
				if (i == 0 || i == 1 || i == 4 || i == 5) {
					p3[j].x = rectangle1.getX(i);
					p3[j].y = rectangle1.getY(i);
					p3[j].z = rectangle1.getZ(i);
					j++;
				}
				

			}
			bubblesort(p3[2], p3[3]);
			j = 0;
			for (int i = 0; i < 8; i++)
			{
				if (i == 0 || i == 3 || i == 4 || i == 7) {
					p4[j].x = rectangle1.getX(i);
					p4[j].y = rectangle1.getY(i);
					p4[j].z = rectangle1.getZ(i);
					j++;
				}
				
			}
			bubblesort(p4[2], p4[3]);
			j = 0;
			for (int i = 0; i < 8; i++)
			{
				if (i == 1 || i == 2 || i == 5 || i == 6) {
					p5[j].x = rectangle1.getX(i);
					p5[j].y = rectangle1.getY(i);
					p5[j].z = rectangle1.getZ(i);
					j++;
				}
				
			}
			bubblesort(p5[2], p5[3]);
			j = 0;
			for (int i = 0; i < 8; i++)
			{
				if (i == 2 || i == 3 || i == 6 || i == 7) {
					p6[j].x = rectangle1.getX(i);
					p6[j].y = rectangle1.getY(i);
					p6[j].z = rectangle1.getZ(i);
					j++;
				}
				
			}
			bubblesort(p6[0], p6[1]);
			Matrix4::zbuffer(BLACK, p1, 4,zhenbuffer,z_buffer);
			Matrix4::zbuffer(YELLOW, p2, 4, zhenbuffer, z_buffer);
			Matrix4::zbuffer(BLUE, p3, 4, zhenbuffer, z_buffer);
			Matrix4::zbuffer(RED, p4, 4, zhenbuffer, z_buffer);
			Matrix4::zbuffer(GREEN, p5, 4, zhenbuffer, z_buffer);
			Matrix4::zbuffer(0xF08080, p6, 4, zhenbuffer, z_buffer);
			for (int i = 0; i <height; i++)
			{
				for (int j = 0; j < width ; j++)
				{
					setPixel(j, i, zhenbuffer[i][j]);
				}
			}
			resetZ_buffer();
			/*DDALine(rectangle1.getX(0), rectangle1.getX(1), rectangle1.getY(0), rectangle1.getY(1), BLACK);
			DDALine(rectangle1.getX(1), rectangle1.getX(2), rectangle1.getY(1), rectangle1.getY(2), YELLOW);
			DDALine(rectangle1.getX(2), rectangle1.getX(3), rectangle1.getY(2), rectangle1.getY(3), RED);
			DDALine(rectangle1.getX(0), rectangle1.getX(3), rectangle1.getY(0), rectangle1.getY(3), BLACK);
			DDALine(rectangle1.getX(0), rectangle1.getX(4), rectangle1.getY(0), rectangle1.getY(4), BLUE);
			DDALine(rectangle1.getX(1), rectangle1.getX(5), rectangle1.getY(1), rectangle1.getY(5), YELLOW);
			DDALine(rectangle1.getX(2), rectangle1.getX(6), rectangle1.getY(2), rectangle1.getY(6), BLUE);
			DDALine(rectangle1.getX(3), rectangle1.getX(7), rectangle1.getY(3), rectangle1.getY(7), RED);
			DDALine(rectangle1.getX(4), rectangle1.getX(5), rectangle1.getY(4), rectangle1.getY(5), BLACK);
			DDALine(rectangle1.getX(5), rectangle1.getX(6), rectangle1.getY(5), rectangle1.getY(6), BLUE);
			DDALine(rectangle1.getX(6), rectangle1.getX(7), rectangle1.getY(6), rectangle1.getY(7), YELLOW);
			DDALine(rectangle1.getX(4), rectangle1.getX(7), rectangle1.getY(4), rectangle1.getY(7), RED);*/
			/*AET(BLACK, p1, 3);
			AET(BLUE, p2, 3);
			AET(RED,p3,3);
			AET(YELLOW, p4, 3);*/
			

		}
		
	EndPaint(hWnd, &ps);
};
void handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int key = wParam;
	switch (key)
	{
	case VK_RIGHT:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {

			dx += g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);

		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			x3D += 10;		
			g_transformMatrix3.settransform(x3D, y3D, 0);
			
			
		}
		break;
	case VK_LEFT:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {

			dx -= g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);

		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			x3D -= 10;
			g_transformMatrix3.settransform(x3D, y3D, 0);
			
		}
		break;
	case VK_UP:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			dy -= g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);
		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			y3D -= 10;
			g_transformMatrix3.settransform(x3D,y3D,0);
			
		}

		break;
	case VK_DOWN:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			dy += g_Dataset[0]->bbox.width() * 5;
			coeffientmatrix.makeTranslate(dx, dy);
		}

		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			y3D += 10;
			g_transformMatrix3.settransform(x3D, y3D, 0);
			
		}

		break;
	case  0x41:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			example.setrotate(5);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			angle3D += 10;
			g_transformMatrix3.setrotateX(angle3D);
			
		}
		break;
	case 0x44:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			example.setrotate(-5);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			angle3D -= 10;
			g_transformMatrix3.setrotateX(angle3D);
			
		}
		break;
	case VK_SPACE:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			example.setsymmetry(-1, 0, 0, -1);
			s_transformMatrix *= example;
			example.setmatrix();
		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			g_transformMatrix3.setsymmetry(-1, -1, -1);
		}
			break;
	case 0x45:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			example.setshear(0, 1);
			s_transformMatrix *= example;
			example.setmatrix();

		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			angle3D += 10;
			g_transformMatrix3.setrotateY(angle3D);
			
		}
		break;
	case 0x51:
		if (typer.type == Map && g_Dataset.getLayerCount() > 0) {
			example.setshear(0, -1);
			s_transformMatrix *= example;
			example.setmatrix();
		}
		if (typer.type == threeD) {
			/*g_transformMatrix3.setmatrix4();*/
			angle3D -= 10;
			g_transformMatrix3.setrotateY(angle3D);
			
		}
		break;
	/*default:
		break;*/
		
	}
};
void handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	short det = (short)HIWORD(wParam);
	switch (message)
	{
		
	case WM_MOUSEWHEEL:
		d = eye - center;
		d.normalize();
		if (det > 0)
		{
			switch (typer.type)
			{
			case twoD:
				break;
			case threeD:
				
				scale *= 1.1;
				/*g_transformMatrix3.setmatrix4();*/
				g_transformMatrix3.setscale(scale, scale, scale);		
				break;
			case Map:
				example.setscale(1.1, 1.1);
				s_transformMatrix *= example;
				example.setmatrix();
				break;
			default:
				break;
			}
			
			
		}
		else
		{
			switch (typer.type)
			{
			case Map:
				example.setscale(0.9, 0.9);
				s_transformMatrix *= example;
				example.setmatrix();
				break;
			case threeD:
				scale *= 0.9;
				g_transformMatrix3.setscale(scale, scale, scale);
			
				break;
			default:
				break;

			}
		}
			break;
	case WM_LBUTTONUP:
		if (getRubberMode() !=rmNone) {
		
			if (polygonstate.operater == p_polygon && polygonstate.polygoner == p_ET && state1 == 3) {
				if (count1 % 2 == 1) {
					num = getRubberPoints(pts);
				}
				else
				{
					num1 = getRubberPoints(pt);
				}
			}
			else
			{
				num = getRubberPoints(pts);
			}
			
		
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

			//由原始坐标构建齐次坐标
			v.setrow(pGemetry->x, pGemetry->y, 1, 0);
		    finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
			v*=finalTransformMatrix;
			finalTransformMatrix.setmatrix();
			finalTransformMatrix.setscale(s, s);
			v *= finalTransformMatrix;
			v *= s_TransformMatrix;
			
			finalTransformMatrix.setmatrix();
			v *= finalTransformMatrix;//变换
			finalTransformMatrix.setmatrix();
			finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
			
			v *= finalTransformMatrix;
			setPixel(v.getrow(0), v.getcol(0), color);//绘制
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
				Point2D<double>& pt = (*pGemetry)[j];//获得线顶点的原始坐标
				//由原始坐标构建齐次坐标
				v.setrow(pt.x, pt.y, 1, 0);
				finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
				v*=finalTransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.setscale(s, s);
				v *= finalTransformMatrix;
				v *= s_TransformMatrix;
			
				finalTransformMatrix.setmatrix();
				v *= finalTransformMatrix;//变换
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
			
				v *= finalTransformMatrix;
				setPixel(v.getrow(0), v.getcol(0), color);//绘制
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
				Point2D<double>& pt = (*pGemetry)[j];//获得多边形顶点的原始坐标
				//由原始坐标构建齐次坐标
				v.setrow(pt.x, pt.y, 1, 0);
				finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
				v *= finalTransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.setscale(s, s);
				v *= finalTransformMatrix;
				
				v *= s_TransformMatrix;
				finalTransformMatrix.setmatrix();
				finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
				v *= finalTransformMatrix;//变换
				setPixel(v.getrow(0), v.getcol(0), color);//绘制
			}
		}
	}
	break;
	}

	Matrix v1(1,3,false);//左下角点
	v1.setrow(pLayer->bbox.xmin, pLayer->bbox.ymin, 1, 0);

	Matrix v2(1, 3, false);
	v2.setrow(pLayer->bbox.xmax, pLayer->bbox.ymax,1,0);//右上角点
	finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
	v1 *= finalTransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.setscale(s, s);
	v1 *= finalTransformMatrix;
	
	v1 *= s_TransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
	v1 *= finalTransformMatrix;//变换
	finalTransformMatrix.settransformparam(-pLayer->bbox.centerX(), -pLayer->bbox.centerY());
	v2 *= finalTransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.setscale(s, s);
	v2 *= finalTransformMatrix;
	
	v2 *= s_TransformMatrix;
	finalTransformMatrix.setmatrix();
	finalTransformMatrix.settransformparam(pLayer->bbox.centerX(), pLayer->bbox.centerY());
	v2 *= finalTransformMatrix;//变换

										 //以下绘制图层边框，仅用于测试
	for (int i = v1.getrow(0); i < v2.getrow(0); i++)setPixel(i, v1.getcol(0), YELLOW);//绘制底边
	for (int i = v1.getrow(0); i < v2.getrow(0); i++)setPixel(i, v2.getcol(0), YELLOW);//绘制顶边
	for (int i = v1.getcol(0); i < v2.getcol(0); i++)setPixel(v1.getrow(0), i, YELLOW);//绘制左边
	for (int i = v1.getcol(0); i < v2.getcol(0); i++)setPixel(v2.getrow(0), i, YELLOW);//绘制右边
	finalTransformMatrix.setmatrix();
}
