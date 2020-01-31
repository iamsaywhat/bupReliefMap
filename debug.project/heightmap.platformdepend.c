#include "heightmap.platformdepend.h"


char map[320054];

/***********************************************
  Получить размер шага по долготе
***********************************************/
double getLongitudeGridStep(void)
{
	// Если значение действительно в секундах, то нужно привести к градусам делением на 3600
	return (*(double*)(map + 46)) / 3600;
}

/***********************************************
  Получить размер шага по широт
***********************************************/
double getLatitudeGridStep(void)
{
	// Если значение действительно в секундах, то нужно привести к градусам делением на 3600
	return (*(double*)(map + 38) / 3600);
}

/***********************************************
  Получить долготу нулевой точки
***********************************************/
double getBottomLeftLongitude(void)
{
	return *(double*)(map + 30);
}

/***********************************************
  Получить широту нулевой точки
***********************************************/
double getBottomLeftLatitude(void)
{
	return *(double*)(map + 22);
}

/***********************************************
  Получить число узловых точек сетки по долготе
***********************************************/
short getNumberOfLongitude(void)
{
	return *(short*)(map + 20);
}

/***********************************************
  Получить число узловых точек сетки по широте
***********************************************/
short getNumberOfLatitude(void)
{
	return *(short*)(map + 18);
}

/***********************************************
  Получить долготу точки приземлени¤
***********************************************/
double getTouchdownLongitude(void)
{
	return 0;
}

/***********************************************
  Получить широту точки приземлени¤
***********************************************/
double getTouchdownLatitude(void)
{
	return 0;
}

/****************************************************************************
  Узнать высоту в узловой точке с индексами (longitudeIndex, latitudeIndex)
****************************************************************************/
short getAltitudeOnJunctionPoint(unsigned int longitudeIndex, unsigned int latitudeIndex)
{
	// Формула дл¤ расчета адреса чтени¤ из пам¤ти 54 + 800*(400 - 1 - j) + 2*i, 
	// где 800 - размер пакета высот, количество точек по широте, 54 размер информационного пакета лежащего перед картой в пам¤ти
	short temp = *(short*)(map + 54 + 800 * (400 - 1 - latitudeIndex) + 2 * longitudeIndex);
	unsigned int adr = 54 + 800 * (400 - 1 - latitudeIndex) + 2 * longitudeIndex;
	adr = (short*)(map + 54 + 800 * (400 - 1 - latitudeIndex) + 2 * longitudeIndex);
	return temp;
}
