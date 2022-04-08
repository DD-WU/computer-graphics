#pragma once

#include <vector>

using namespace std;

#define  PI  3.14159265358979323846

//2D��
template<typename T>
struct Point2D
{
	Point2D(){ x = y = 0 ;}
	Point2D( T x, T y ){ this->x = x, this->y = y; }

	T x,y;
};

//2D��Χ��
template<typename T>
struct Box2D
{
	Box2D(){ setBox(0, 0, 0, 0); }
	Box2D( T xmin,T ymin , T xmax, T ymax )
	{
		setBox(xmin, ymin, xmax, ymax);

	}

	void setBox( T xmin,T ymin , T xmax, T ymax )//���ð�Χ�з�Χ
	{
		this->xmin = xmin, this->xmax = xmax, this->ymin = ymin, this->ymax = ymax ;
	}

	T width(){ return xmax - xmin; }//��Χ�п��
	T height(){ return ymax - ymin; }//��Χ�и߶�
	T centerX(){ return (xmin + xmax) /2 ; }//��Χ�����ĵ�x����
	T centerY(){ return (ymin + ymax) /2 ; }//��Χ�����ĵ�y����

	T xmin,ymin;
	T xmax,ymax;
};

//ͼ�ζ�������
enum GeomType{ gtUnkown = 0, gtPoint = 1, gtPolyline = 2, gtPolygon = 3 };

//ͼ�ζ�����࣬���̳�
struct Geometry
{
	virtual ~Geometry(){}
	virtual GeomType getGeomType() =  0; //��ȡͼ�ζ�������
};

//��ͼ�����
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

//��ͼ�ζ���
template<typename T>
struct PolylineGeometry:Geometry
{
	virtual GeomType getGeomType(){ return gtPolyline; }

	//��������أ���ȡ��i����
	Point2D<T>& operator[]( int i ){ return pts[i]; }

	//��ӵ�
	void addPoint( T x, T y )
	{
		pts.push_back( Point2D<T>( x, y ));
	}

	//���е�
	vector<Point2D<T>> pts;
};

//�����ͼ�ζ���
template<typename T>
struct PolygonGeometry:PolylineGeometry<T>
{
	virtual GeomType getGeomType(){ return gtPolygon; }
};


//ͼ��
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
		for( size_t i = 0 , size = geometrySet.size(); i < size ; ++i ) delete geometrySet[i];//��������ɾ������ͼ�ζ���
	}

	//��������أ����ص�i��ͼ�ζ���
	Geometry* operator[]( int i ){ return geometrySet[i]; }

	//���ͼ�����
	void addGeometry( Geometry* pGeometry)
	{
		geometrySet.push_back( pGeometry );
	}

	//����ͼ�㷶Χ
	void setExtent( double xmin,double ymin , double xmax, double ymax  )
	{
		bbox.setBox( xmin, ymin,xmax,ymax );
	}

	//��ȡͼ�������ͼ�ζ�������
	int getGeometryCount(){ return geometrySet.size(); }	

	vector<Geometry*> geometrySet;//ͼ�ζ��󼯺�
	Box2D<double> bbox;//ͼ�㷶Χ��Ӧ�İ�Χ��
	GeomType geomType;//ͼ������
};

//���ݼ�
struct Dataset
{
	virtual ~Dataset()
	{
		for( size_t i = 0, size = layerSet.size() ; i < size ; ++i ) delete layerSet[i];//��������ɾ��ͼ��
	}

	//��������أ����ص�i��ͼ��
	Layer* operator[]( int i ){ return layerSet[i]; }

	//��ȡͼ����
	int getLayerCount(){ return layerSet.size(); }	

	//���ͼ��
	void addLayer( Layer* pLayer )
	{
		layerSet.push_back( pLayer );
	}

	//ͼ�㼯��
	vector<Layer*> layerSet;
};

///////////////////////////////////////3D//////////////////////////////////////////////

//3D��
struct Point3D
{
	Point3D(){ x = y = z = 0 ;}
	Point3D( double x, double y , double z ){ this->x = x, this->y = y, this->z = z; }

	double x,y,z;
};

//3D��Χ��
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

//������
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
	int ids[3];//���涥������
};

//������
struct Tin
{
	vector<Point3D> pts;//���㼯��
	vector<Face> faces;//�����μ���
	vector<double> normals;//���㷨�߼���

	void calcNormals();
};