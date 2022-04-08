/********************************************************************
file base:	Graphic
file ext:	h
author:		����(njnu2016@foxmail.com)
purpose:	ͼ�ο�ӿ�
version:    1.0
*********************************************************************/
#ifndef _GRAPH_H
#define _GRAPH_H

#define BLACK 0
#define WHITE 0xFFFFFF
#define RED 0xFF
#define GREEN 0xFF00
#define BLUE 0xFF0000
#define YELLOW 0xFFFF00

///���ص�
enum inOrout { ��, ��, �� };
struct PixelPoint
{
	int x,y;
	inOrout judge = ��;
};

/**	��ʼ��
@return �Ƿ��ʼ���ɹ���0-success�� -1 - fail
*/
int init();

/**	ˢ�´���
*/
void refreshWindow();

/**	�����������
*/
void clearWindow();

/**	�Ѻ󻺴���Ϣ��ʾ����Ļ
*/
void swapBuffer();

/**	���ô��ڴ�С
@param  width ���ڿ��
@param  height  ���ڸ߶�
*/
void setWindowSize(int width, int height);

/**	��ȡ���ڿ��
@return ���ڿ��
*/
int getWindowWidth();

/**	��ȡ���ڸ߶�
@return ���ڸ߶�
*/
int getWindowHeight();

/**	����Y�᷽��
@param  isUp true��ʾ���ϣ���������
*/
void setYUp(bool isUp);

/**	����ԭ��Ϊָ������λ��
@param  x ָ������λ��x����
@param  y ָ������λ��y����
*/
void setOrig(int x, int y);

/**	����ָ��λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setPixel(int x, int y, unsigned color);

/**	��ȡָ��λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@return ָ��λ�����ص���ɫ
*/
unsigned getPixel(int x, int y);

/**	�豸����תΪ�߼�����
@param  dx �豸x����
@param  dy  �豸y����
@param  lx �߼�x����
@param  ly  �߼�y����
*/
void DPtToLPt(int dx, int dy, int& lx, int& ly);

/**	�߼�����תΪ�豸����
@param  lx �߼�x����
@param  ly  �߼�y����
@param  dx �豸x����
@param  dy  �豸y����
*/
void LPtToDPt( int lx, int ly, int& dx, int& dy );

/**	���ñ���ɫ
@param  backColor ����ɫ
*/
void setBackColor( unsigned backColor );

/**	��������Ŵ���
@param  fontHeight ����߶�
*/
void setFontHeight( int fontHeight );

/**	����������
@param  fontName ������
*/
void setFontName( const char* fontName );

/**	��ȡ����������С�����ֽ�Ϊ��λ
@param  pTextA ����
*/
int getTextOutlineInfoSize( const char* pTextA );

/**	��ȡ����������Ϣ
@param  pText  ���������
@param  pBuffer  �ַ�������Ϣ��Ż�����
*/
void getTextOutline( const char* pTextA , unsigned char* pBuffer );


///��Ƥ��ģʽ, rmNone -- �ر���Ƥ��ģʽ ��rmLine-- ��ģʽ�� rmRectangle -- ����ģʽ , rmPolyline-- ����ģʽ, rmPolygon -- �����ģʽ
enum RubberMode{ rmNone, rmLine, rmRectangle, rmPolyline, rmPolygon };

/**	���õ�ǰģʽ�Ƿ�Ϊ��Ƥ��ģʽ
@param  value �Ƿ���Ƥ��ģʽ
*/
void setRubberMode(RubberMode mode );

/**	��ȡ��ǰ��Ƥ��ģʽ
@return ��ǰ��Ƥ��ģʽ
*/
RubberMode getRubberMode();

/**	��ȡ��Ƥ�������յ㣬��������������Ƥ��ģʽ����������Ƥ��
@param  x1 ���x����
@param  y1  ���y����
@param  x2 �յ�x����
@param  y2  �յ�y����
@return ��Ч��Ƥ�ߵ���
*/
int getRubberPoints(int& x1, int& y1, int& x2, int& y2);

/**	��ȡ��Ƥ�ߵ㼯�ϣ�һ������rmPolyline��rmPolygonģʽ
@param  pts ��Ƥ�ߵ㼯��
@return ��Ƥ�ߵ���
*/
int getRubberPoints( PixelPoint pts[] );

///�����ʽ��csArrow--��ͷ ��  csHand--���ͣ� csCross--ʮ��˿�� csSize--����
enum CursorStyle{ csArrow = 32512, csHand = 32649, csCross = 32515, csSize = 32646 };

/**	���ù����ʽ
@param  cursor �����ʽ
*/
void setCursor(CursorStyle cursor);

///����ַ������Ķ���Σ��γɾ�̬����ṹ
struct _charPolygon
{
	int pointCount;//��ɶ���εĶ�����
	PixelPoint pts[1];//����ζ�������

	int size();	//�ø����ݽṹ�����Ķ��������ռ�ݵ�ʵ�ʿռ��С�����ֽ�Ϊ��λ

	_charPolygon* next();//����ַ���������һ�������
};

///�ַ�����,��һ�������������ɣ��γɾ�̬����ṹ
struct _charOutline
{
	int cb;//�ø����ݽṹ�������ַ�����ռ�ݵ�ʵ�ʿռ��С�����ֽ�Ϊ��λ
	_charPolygon charPolygons[1];//����ַ������Ķ��������

	_charOutline* next();//��һ���ַ�����

	int count();//��ȡ�ַ����������

	_charPolygon* operator[]( int index );//��ȡָ��λ�õ��ַ������
};

///����ַ��������γɾ�̬����ṹ
struct _textOutline
{
	int cb;//�ø����ݽṹ�����Ķ���ַ�����ռ�ݵ�ʵ�ʿռ��С�����ֽ�Ϊ��λ
	_charOutline charOutlines[1];//�ַ���������

	int count();//��ȡ�ַ���������

	_charOutline* operator[]( int index );//��ȡ��index���ַ�����
};


#endif