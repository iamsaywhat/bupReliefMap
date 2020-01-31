/***************************************************************************************************
  heightmap.platformdepend - Подмодуль Heightmap, благодаря которому, платформозависимые 
  функции и определения вынесены отдельно. Позволяет проводить отладку в любой среде
  и осуществлять быстрое портирование на любые платформы
***************************************************************************************************/
#ifndef _HEIGHTMAP_PLATFORMDEPEND_
#define _HEIGHTMAP_PLATFORMDEPEND_

/***********************************************
  Получить размер шага по долготе
***********************************************/
double getLongitudeGridStep(void);

/***********************************************
  Получить размер шага по широт
***********************************************/
double getLatitudeGridStep(void);

/***********************************************
  Получить долготу нулевой точки
***********************************************/
double getBottomLeftLongitude(void);

/***********************************************
  Получить широту нулевой точки
***********************************************/
double getBottomLeftLatitude(void);

/***********************************************
   Получить число узловых точек сетки по долготе
***********************************************/
short getNumberOfLongitude(void);

/***********************************************
  Получить число узловых точек сетки по широте
***********************************************/
short getNumberOfLatitude(void);

/***********************************************
  Получить долготу точки приземления
***********************************************/
double getTouchdownLongitude(void);

/***********************************************
  Получить широту точки приземления
***********************************************/
double getTouchdownLatitude(void);

/****************************************************************************
  Узнать высоту в узловой точке с индексами (longitudeIndex, latitudeIndex)
****************************************************************************/
short getAltitudeOnJunctionPoint(unsigned int longitudeIndex, unsigned int latitudeIndex);

#endif
