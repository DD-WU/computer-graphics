#pragma once

#include <vector>

using namespace std;

#define  PI  3.14159265358979323846

//2D点
template<typename T>
struct Point2D
{
	Point2D(){ x = y = 0 ;}
	Point2D( T x, T y ){ this->x = x, this->y = y; }

	T x,y;
};

//2D包围盒
template<typename T>
struct Box2D
{
	Box2D(){ setBox(0, 0, 0, 0); }
	Box2D( T xmin,T ymin , T xmax, T ymax )
	{
		setBox(xmin, ymin, xmax, ymax);

	}

	void setBox( T xmin,T ymin , T xmax, T ymax )//设置包围盒范围
	{
		this->xmin = xmin, this->xmax = xmax, this->ymin = ymin, this->ymax = ymax ;
	}

	T width(){ return xmax - xmin; }//包围盒宽度
	T height(){ return ymax - ymin; }//包围盒高度
	T centerX(){ return (xmin + xmax) /2 ; }//包围盒中心点x坐标
	T centerY(){ return (ymin + ymax) /2 ; }//包围盒中心点y坐标

	T xmin,ymin;
	T xmax,ymax;
};

//图形对象类型
enum GeomType{ gtUnkown = 0, gtPoint = 1, gtPolyline = 2, gtPolygon = 3 };

//图形对象基类，供继承
struct Geometry
{
	virtual ~Geometry(){}
	virtual GeomType getGeomType() =  0; //获取图形对象类型
};

//点图像对象
template<typename T>
struct PointGemetry:Geometry
{
	PointGemetry(  )
	{
		x = y = 0;
	}
	PointGemetry( T x, T y )
	{
		this->x= x;
		this->y = y;
	}

	virtual GeomType getGeomType(){ return gtPoint; }

	T x,y;
};

//线图形对象
template<typename T>
struct PolylineGeometry:Geometry
{
	virtual GeomType getGeomType(){ return gtPolyline; }

	//运算符重载，获取第i个点
	Point2D<T>& operator[]( int i ){ return pts[i]; }

	//添加点
	void addPoint( T x, T y )
	{
		pts.push_back( Point2D<T>( x, y ));
	}

	//所有点
	vector<Point2D<T>> pts;
};

//多边形图形对象
template<typename T>
struct PolygonGeometry:PolylineGeometry<T>
{
	virtual GeomType getGeomType(){ return gtPolygon; }
};


//图层
struct Layer
{
	Layer()
	{
		geomType = gtUnkown;
	}

	Layer( GeomType geomType )
	{
		this->geomType = geomType;
	}

	virtual ~Layer()
	{
		for( size_t i = 0 , size = geometrySet.size(); i < size ; ++i ) delete geometrySet[i];//析构函数删除所有图形对象
	}

	//运算符重载，返回第i个图形对象
	Geometry* operator[]( int i ){ return geometrySet[i]; }

	//添加图像对象
	void addGeometry( Geometry* pGeometry)
	{
		geometrySet.push_back( pGeometry );
	}

	//设置图层范围
	void setExtent( double xmin,double ymin , double xmax, double ymax  )
	{
		bbox.setBox( xmin, ymin,xmax,ymax );
	}

	//获取图层包含的图形对象数量
	int getGeometryCount(){ return geometrySet.size(); }	

	vector<Geometry*> geometrySet;//图形对象集合
	Box2D<double> bbox;//图层范围对应的包围盒
	GeomType geomType;//图层类型
};

//数据集
struct Dataset
{
	virtual ~Dataset()
	{
		for( size_t i = 0, size = layerSet.size() ; i < size ; ++i ) delete layerSet[i];//析构函数删除图层
	}

	//运算符重载，返回第i个图层
	Layer* operator[]( int i ){ return layerSet[i]; }

	//获取图层数
	int getLayerCount(){ return layerSet.size(); }	

	//添加图层
	void addLayer( Layer* pLayer )
	{
		layerSet.push_back( pLayer );
	}

	//图层集合
	vector<Layer*> layerSet;
};

///////////////////////////////////////3D//////////////////////////////////////////////

//3D点
struct Point3D
{
	Point3D(){ x = y = z = 0 ;}
	Point3D( double x, double y , double z ){ this->x = x, this->y = y, this->z = z; }

	double x,y,z;
};

//3D包围盒
struct Box3D
{
	Box3D(){ setBox(0, 0, 0, 0, 0, 0);}
	Box3D( double xmin,double ymin ,double zmin, double xmax, double ymax, double zmax )
	{
		setBox(xmin, ymin, zmin, xmax, ymax, zmax);
	}

	void setBox( double xmin,double ymin ,double zmin, double xmax, double ymax, double zmax )
	{
		this->xmin = xmin, this->xmax = xmax, this->ymin = ymin, this->ymax = ymax , this->zmin = zmin, this->zmax = zmax ;
	}

	Point3D operator[]( int index )
	{
		switch( index )
		{
		case 0:
			return Point3D( xmin, ymin, zmin );
			break;
		case 1:
			return Point3D( xmax, ymin, zmin );
			break;
		case 2:
			return Point3D( xmax, ymax, zmin );
			break;
		case 3:
			return Point3D( xmin, ymax, zmin );
			break;
		case 4:
			return Point3D( xmin, ymin, zmax );
			break;
		case 5:
			return Point3D( xmax, ymin, zmax );
			break;
		case 6:
			return Point3D( xmax, ymax, zmax );
			break;
		case 7:
			return Point3D( xmin, ymax, zmax );
			break;
		}
		throw;
	}

	double xmin,ymin,zmin;
	double xmax,ymax,zmax;
};

//三角形
struct Face
{
	Face( )
	{
		ids[0] = ids[1] = ids[2] = 0;
	}

	Face( int v0, int v1, int v2 )
	{
		ids[0] = v0, ids[1] = v1, ids[2] = v2;
	}

	int operator[](  int i ){ return ids[i]; }

	int next( int i ){ return (i + 1) % 3 ; }
	int ids[3];//保存顶点索引
};

//三角网
struct Tin
{
	vector<Point3D> pts;//顶点集合
	vector<Face> faces;//三角形集合
	vector<double> normals;//顶点法线集合

	void calcNormals();
};