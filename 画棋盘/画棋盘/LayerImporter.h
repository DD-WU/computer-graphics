#pragma once

struct Layer;
//图层导入类
class LayerImporter
{
public:
	static Layer* importShpLayer( const char* path );//导入shp文件，返回图层对象，导入失败则返回NULL
};

