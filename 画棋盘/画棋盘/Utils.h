#pragma once

#include "Define.h"

class Utils
{
public:
	//打开文件对话框，选择单个文件，intialDir 初始目录，可为空，extension 文件后缀， filePath 选择的文件， bufSize filePath数组大小
	static int selectSingleFile( const char* intialDir,const char* extension, char* filePath, int bufSize );
	static double DegreesToRadians(double angle) { return angle*PI/180.0; }
	static double RadiansToDegrees(double angle) { return angle*180.0/PI; }
};

