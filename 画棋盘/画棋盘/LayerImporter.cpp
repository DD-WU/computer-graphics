#include "LayerImporter.h"
#include "Define.h"

#include "ogrsf_frmts.h"
#include "ogr_feature.h"
#include "gdal.h"
#include "ogr_api.h"
//
//
//#pragma comment(lib,"../3rdParty/gdal/lib/gdal_i.lib")

Layer* LayerImporter::importShpLayer(const char* path)
{
	GDALAllRegister();
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	OGRSFDriver* poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
	OGRDataSource* poDS = poDriver->Open( path );
	if( poDS == NULL ) return NULL;

	OGRLayer* poLayer = poDS->GetLayer(0);
	if( poLayer == NULL ) return NULL;

	OGREnvelope ev;
	poLayer->GetExtent(&ev);
	Layer* pLayer = new Layer( (GeomType)poLayer->GetGeomType());
	pLayer->setExtent( ev.MinX, ev.MinY, ev. MaxX, ev.MaxY );

	OGRFeature * pFeature;
	while ((pFeature = poLayer->GetNextFeature()) )
	{
		OGRGeometry* pOGRGeometry = pFeature->GetGeometryRef();

		switch (pOGRGeometry->getGeometryType())
		{
		case wkbPoint:
			{
				OGRPoint* pOGRPoint = (OGRPoint*)pOGRGeometry;
				PointGemetry<double>* pGeometry = new PointGemetry<double>( pOGRPoint->getX(),pOGRPoint->getY() ) ;
				pLayer->addGeometry( pGeometry );
			}
			break;
		case wkbLineString:
			{
				OGRLineString* pOGRLineString = (OGRLineString*)pOGRGeometry;
				PolylineGeometry<double>* pGeometry = new PolylineGeometry<double>( ) ;
				for (int i = 0, size = pOGRLineString->getNumPoints(); i < size ; ++i)
				{
					pGeometry->addPoint( pOGRLineString->getX(i),pOGRLineString->getY(i) );
				}
				pLayer->addGeometry( pGeometry );
			}
			break;
		case wkbPolygon:
			{
				OGRPolygon* pOGRPolygon = (OGRPolygon*)pOGRGeometry;
				PolygonGeometry<double>* pGeometry = new PolygonGeometry<double>( ) ;
				OGRLinearRing* ring = pOGRPolygon->getExteriorRing();
				for (int i = 0, size = ring->getNumPoints(); i < size ; ++i)
				{
					pGeometry->addPoint( ring->getX(i),ring->getY(i) );
				}		
				pLayer->addGeometry( pGeometry );
			}
			break;
		}
		OGRFeature::DestroyFeature(pFeature);
	}

	OGRDataSource::DestroyDataSource(poDS);
	OGRCleanupAll();

	return pLayer;
}
