#include "Utils.h"
#include <Windows.h>
#include <string>



int Utils::selectSingleFile(const char* intialDir,const char* extension, char* filePath , int bufSize )
{	
	std::string strFilter;
	strFilter.append("*.");
	strFilter.append( extension );
	strFilter.append("\0", 1 );
	strFilter.append("*.");
	strFilter.append( extension );
	strFilter.append("\0\0", 2 );

	char szBuffer[1024] = { 0 };	
	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.nMaxFile = bufSize;
	ofn.lpstrFilter = strFilter.c_str();
	ofn.lpstrInitialDir = intialDir;//默认的文件路径
	ofn.lpstrFile = filePath;//存放文件的缓冲区
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	return GetOpenFileNameA(&ofn) ;
}
