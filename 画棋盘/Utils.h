#pragma once

#include "Define.h"

class Utils
{
public:
	//���ļ��Ի���ѡ�񵥸��ļ���intialDir ��ʼĿ¼����Ϊ�գ�extension �ļ���׺�� filePath ѡ����ļ��� bufSize filePath�����С
	static int selectSingleFile( const char* intialDir,const char* extension, char* filePath, int bufSize );
	static double DegreesToRadians(double angle) { return angle*PI/180.0; }
	static double RadiansToDegrees(double angle) { return angle*180.0/PI; }
};

