#include "Heightmap.h"
#include "Heightmap_conf_and_pd.h"
#include <math.h>


/********************************************************************

	   nav_transform - Замена координат (приведение диапазонов)

	   Параметры: указатель на point

	   Возвращает: через указатель приведенные координаты

	   Примечание:
	   исходно широта  [-90, 90]    приводим к [0, 180]
	          долгота  [-180, 180]             [0, 360]
       
	   для того, чтобы удобно было сравнивать координаты

********************************************************************/
void nav_transform(point* p)
{
	p->lon += 180;
	p->lat += 90;
}


/*********************************************************************************************
			
			area - Вычисление площади треугольника по 3м точкам
			
			Параметры: a, b, c точки типа point с координатами

*********************************************************************************************/
double area(point a, point b, point c)
{
	double res;
	res = ((a.lon - c.lon) * (b.lat - c.lat) + (b.lon - c.lon) * (c.lat - a.lat));
	// Берём модуль
	res = fabs(res);

	return res;
}

/**********************************************************************************************
			 TrianglePos - Определение принадлежности точки loc, треугольнику с вершинами 
			 a,b,c.
***********************************************************************************************/
char TrianglePos(point a, point b, point c, point loc)
{
	double abc = area(a, b, c);
	double abd = area(a, b, loc);
	double adc = area(a, loc, c);
	double bdc = area(b, loc, c);
	double sum, eps;

	sum = abd + adc + bdc;
	eps = (abc / 100.0) * 0.0000000001;
	//if (abc == abd + adc + bdc)
	if (fabs(abd + adc + bdc - abc) < eps)
		return 1;
	else
		return 0;
}

/************************************************************************************************
			HeightFromPlane - Построение поверхности по трём точкам и вычисление высоты
			в точке с координатами loc (метод триангуляции).
************************************************************************************************/
void HeightFromPlane(point a, point b, point c, point* loc)
{
	double matr[9];                   // Матрица для вычисления уравнения плоскости
	double delta1, delta2, delta3;    // Миноры матрицы

	/* Заполняем матрицу
	|x-x1   x2-x1   x3-x1|     |matr[0] matr[1] matr[2]|
	|y-y1   y2-y1   y3-y1|  =  |matr[3] matr[4] matr[5]|
	|z-z1   z2-z1   z3-z1|     |matr[6] matr[7] matr[8]|  	*/
            
	matr[0] = loc->lon - a.lon;  // x-x1
	matr[1] = b.lon - a.lon;     // x2-x1
	matr[2] = c.lon - a.lon;     // x3-x1
	matr[3] = loc->lat - a.lat;  // y-y1
	matr[4] = b.lat - a.lat;     // y2-y1
	matr[5] = c.lat - a.lat;     // y3-y1
 	matr[6] = 0;                 // z-z1 (не сипользуем, так как сразу выразим высоту ниже)
	matr[7] = b.alt - a.alt;     // z2-z1
	matr[8] = c.alt - a.alt;     // z3-z1

	// Считаем миноры матрицы
	delta1 = matr[4] * matr[8] - matr[5] * matr[7];
	delta2 = matr[1] * matr[8] - matr[2] * matr[7];
	delta3 = matr[1] * matr[5] - matr[2] * matr[4];
	
	/* Уравнение плоскости находится из определителя матрицы, то есть
	(x-x1)*delta1 - (y-y1)*delta2 + (z-z1)*delta3 = 0 ,
	но мы уравнение прямой искать не будем, вместо этого выразим z, а вместо x и y подставим координаты loc.lon и loc.lat соответсвенно.
	Тогда получим : */
	loc->alt = (matr[3] * delta2 - matr[0] * delta1) / delta3 + a.alt;
	// Результат записываем сразу в структуру
}


/*********************************************************************************************************************************************
		MapHeight - Определение высоты рельефа в точке location, по 4 ближайшим вершинам сетки
*********************************************************************************************************************************************/
unsigned short MapHeight(point left_lower, point left_upper, point right_lower, point right_upper, point location)
{
	point A, B, C, D;
	point LOC;

	LOC.lon = location.lon;
	LOC.lat = location.lat;
	LOC.alt = 0;

	// Фиксируем точки нашего квадрата 
	A = left_upper;  
	B = right_upper;
	C = left_lower;
	D = right_lower;

	// Контроль входных параметров
	// Точка С должна быть в левом нижнем углу, поэтому не должно быть точек с меньшей долготой и меньшей широтой, чем у С
	if ((C.lon > A.lon) || (C.lon > B.lon) || (C.lon > D.lon) || (C.lat > A.lat) || (C.lat > B.lat) || (C.lat > D.lat))
		return 0xFFFF; // Ошибка возвращаем некорректное число
	// Точка A должна быть в левом верхнем углу, поэтому не должно быть точек с меньшей долготой и большей широтой, чем у А
	if ((A.lon > C.lon) || (A.lon > B.lon) || (A.lon > D.lon) || (A.lat < C.lat) || (A.lat < B.lat) || (A.lat < D.lat))
		return 0xFFFF; // Ошибка возвращаем некорректное число
	// Точка B должна быть в правом верхнем углу, поэтому не должно быть точек с большей долготой и большей широтой, чем у B
	if ((B.lon < A.lon) || (B.lon < C.lon) || (B.lon < D.lon) || (B.lat < A.lat) || (B.lat < C.lat) || (B.lat < D.lat))
		return 0xFFFF; // Ошибка возвращаем некорректное число
	/* Точку D можно не проверять */

	// Проверим принадлежность точки location треугольнику ABC
	if (TrianglePos(A, B, C, LOC))
	{
		HeightFromPlane(A, B, C, &LOC); 
		return (unsigned short)(LOC.alt + 0.5);
	}
	// Тогда точка location должна принадлежность треугольнику BCD
	else if (TrianglePos(C, B, D, LOC))
	{
		HeightFromPlane(C, B, D, &LOC); 
		return (unsigned short)(LOC.alt+0.5);
	}
	// Странно, точка не приналежит ни одному из треугольников - верну ошибку
	return 0xFFFF;;
}



/********************************************************************************************************************
			 GetHeight_OnThisPoint - Узнать высоту рельефа в данной точке
*********************************************************************************************************************/
unsigned short GetHeight_OnThisPoint(double lon, double lat, MAP_MODE mode)
{
	unsigned int i,j;                     // Индексы навигации по узлам карты
	point ThisPoint = {lon, lat, 0};      // Текущая геолокация
	point NullPoint;                      // Нуль точка на карте (координаты левого нижнего угла карты)
	unsigned short LonCount;              // Количество узловых точек по долготе
	unsigned short LatCount;              // Количество узловых точек по долготе
	double MapStepLon;                    // Масштаб по долготе (количество секунд в одном делении)
	double MapStepLat;                    // Масштаб по широте (количество секунд в одном делении)
	point A, B, C, D;                     // Вершины квадрата местоположения
	char OutRangeLat = 1;
	char OutRangeLon = 1;

	// Дополнительные переменные для поддержки грубых целочисленных режимов работы 
	integer_point _NullPoint;            // Нуль точка на карте (координаты левого нижнего угла карты)
	unsigned int  _MapStepLon;           // Масштаб по долготе (количество секунд в одном делении)
	unsigned int  _MapStepLat;           // Масштаб по широте (количество секунд в одном делении)
	integer_point _ThisPoint;            // Текущая геолокация


	// Считаем масштабы
	MapStepLon = GetMapProperties_MapStepLon();
	MapStepLat = GetMapProperties_MapStepLat(); 
	// Узнаем нуль-точку (левая нижняя точка отсчета при построениb карты местности)
	NullPoint.lon = GetMapProperties_NullPointLon(); 
	NullPoint.lat = GetMapProperties_NullPointLat(); 
	// Сделаем перенос координат в положительную полуплоскость для удобного сравнения
	nav_transform(&NullPoint); // Нуль точку
	nav_transform(&ThisPoint); // Текущая геолокация
	// Узнаем количество узловых точек по долготе и широте
	LonCount = GetMapProperties_LonCount(); 
	LatCount = GetMapProperties_LatCount();

	/* Ищем квадрат в котором сейчас находимся
	Будем перебирать сначала долготы от нуль-точки, пока не найдем индексы узловых точек по долготе, между которыми находимся
	После будем перебирать широты от нуль-точки, пока не найдем индексы узловых точек по широте, между которыми находимся
	Фактически нужно найти координаты точки с индексами (i,j), такими что точка ThisPoint (текущая геолокация) будет лежать 
	в квадрате:
		|        |         |         |
	____|________|_________|_________|___	
     j+2|        |         |         |
		|        |         |         |
	    |        |         |         |
    ____|________|_________|_________|___
	j+1 |        |\\\\\\\\\|         |
		|        |\\\\\\\\\|         |<-------- Закрашеный это нужный квадрат, где- то в этом квадрате находится точка ThisPoint
		|        |\\\\\\\\\|         |          квадрат задан индексами (i,j+1)_________(i+1, j+1)
	____|i-1_____|i________|i+1______|___		                               |        |
	   j|		 |         |         |                                         |   o    |
		|		 |         |         |                                    (i,j)|________|(i+1, j)
		|        |         |         |          Зная индексы можно узнать кординаты всех этих точек как          
	____|________|_________|_________|___       (i,j) -> (NullPoint.lon + MapStepLon*i) - долгота узловой точки с индексом (i,j)
	 j-1|        |         |         |                   (NullPoint.lat + MapStepLat*j) - широта узловой точки с индексом (i,j)
			   
   */

	// Дальше реализуем ветвление алгоритма по методам
	// Первый метод наиболее точный - триангуляция
	if(mode == TRIANGULARTION)
	{
		// Ищем по долготе
		for (i = 0; i < LonCount; i++)
		{
			// Необходимо найти такой индекс i, при котором текущая геолокация будет лежать между i и i+1 шагом по долготе в сетке карты
			if (NullPoint.lon < ThisPoint.lon && (NullPoint.lon + MapStepLon) > ThisPoint.lon)
			{
				// Сбрасываем флаг выхода за пределы загруженой карты
				OutRangeLon = 0;
				break; // i и i+1 наши
			}

			else NullPoint.lon += MapStepLon;
		}
		// Ищем по широте
		for (j = 0; j < LatCount; j++)
		{
			// Необходимо найти такой индекс j, при котором текущая геолокация будет лежать между j и j+1 шагом по широте в сетке карты
			if (NullPoint.lat < ThisPoint.lat && (NullPoint.lat + MapStepLat) > ThisPoint.lat)
			{
				// Сбрасываем флаг выхода за пределы загруженой карты
				OutRangeLat = 0;
				break; // j и j+1 наши
			}
			else NullPoint.lat += MapStepLat;
		}
		// Возможно мы вышли из цикла не по условию нахождения нужного квадрата, а по концу карты
		// Тогда необходимо проверить флаги выхода за пределы карты, если они не были сброшены, значит мы вне карты
		// И высоту рельефа узнать не можем, поэтому возвращаем некорректное значение и выходим
		if (OutRangeLat || OutRangeLon)
			return 0xFFFF;

		// Теперь необходимо узнать координаты всех 4 точек образующих квадрат
		// Зная индексы от нуль точки и шаг сетки, это сделать легко:
		// Широты
		A.lon = NullPoint.lon;
		C.lon = NullPoint.lon;
		B.lon = NullPoint.lon + MapStepLon;
		D.lon = NullPoint.lon + MapStepLon;
		// Долготы
		C.lat = NullPoint.lat;
		D.lat = NullPoint.lat;
		A.lat = NullPoint.lat + MapStepLat;
		B.lat = NullPoint.lat + MapStepLat;

		// Теперь необходимо узнать высоты, зная индексы точек, можно это сделать
		C.alt = GetMapProperties_Alt(i, j);
		A.alt = GetMapProperties_Alt(i, j + 1);
		B.alt = GetMapProperties_Alt(i + 1, j + 1);
		D.alt = GetMapProperties_Alt(i + 1, j);

		// Данные подготовлены, теперь узнаем высоту в точке нашей геолокациии методом триангуляции, и сразу же вернём значение
		return  MapHeight(C, A, D, B, ThisPoint);
	}
	// Второй и третий метод имеют одинаковое начало
	else if (mode == MED_APPROXIMATION || mode == MAX_APPROXIMATION)
	{
		// Осуществляем переход на целочисленный формат вычислений с точностью до 7 знаков после запятой
		_NullPoint.lon = (unsigned int)(NullPoint.lon * 10000000 + 0.5);
		_NullPoint.lat = (unsigned int)(NullPoint.lat * 10000000 + 0.5);
		_MapStepLon = (unsigned int)(MapStepLon * 10000000 + 0.5);
		_MapStepLat = (unsigned int)(MapStepLat * 10000000 + 0.5);
		_ThisPoint.lon = (unsigned int)(ThisPoint.lon * 10000000 + 0.5);
		_ThisPoint.lat = (unsigned int)(ThisPoint.lat * 10000000 + 0.5);

		// Ищем по долготе
		for (i = 0; i < LonCount - 1; i++)
		{
			// Необходимо найти такой индекс i, при котором текущая геолокация будет лежать между i и i+1 шагом по долготе в сетке карты
			if (_NullPoint.lon < _ThisPoint.lon && (_NullPoint.lon + _MapStepLon) > _ThisPoint.lon)
			{
				// Сбрасываем флаг выхода за пределы загруженой карты
				OutRangeLon = 0;
				break; // i и i+1 наши
			}

			else _NullPoint.lon += _MapStepLon;
		}
		// Ищем по широте
		for (j = 0; j < LatCount - 1; j++)
		{
			// Необходимо найти такой индекс j, при котором текущая геолокация будет лежать между j и j+1 шагом по широте в сетке карты
			if (_NullPoint.lat < _ThisPoint.lat && (_NullPoint.lat + _MapStepLat) > _ThisPoint.lat)
			{
				// Сбрасываем флаг выхода за пределы загруженой карты
				OutRangeLat = 0;
				break; // j и j+1 наши
			}
			else _NullPoint.lat += _MapStepLat;
		}
		// Возможно мы вышли из цикла не по условию нахождения нужного квадрата, а по концу карты
		// Тогда необходимо проверить флаги выхода за пределы карты, если они не были сброшены, значит мы вне карты
		// И высоту рельефа узнать не можем, поэтому возвращаем некорректное значение и выходим
		if (OutRangeLat || OutRangeLon)
			return 0xFFFF;


		// Здесь наконец второй и третий методы расходятся
		// Второй метод - медианный, производит оценку высоты внутри квадрата, как среднее от высот в вершинах квадрата
		if (mode == MED_APPROXIMATION)
		{
			// Берем среднее арифметическое из высот всех вершин квадрата и округляем до ближайшего целого
			_ThisPoint.alt = (unsigned short)((GetMapProperties_Alt(i, j) + GetMapProperties_Alt(i + 1, j) + GetMapProperties_Alt(i, j + 1) + GetMapProperties_Alt(i + 1, j + 1)) / 4 + 0.5);
			return _ThisPoint.alt;
		}
		// Третий метод - супремум, высота в квадрате определяется как максимальная среди вершин
		else if (mode == MAX_APPROXIMATION)
		{
			// Поиск максимума методом всплывающего пузырька
			// Берем значение высоты первой вершины
			_ThisPoint.alt = GetMapProperties_Alt(i, j);
			// Если в следующей вершине высота больше, берем ее, если нет - то не берем.
			if (_ThisPoint.alt < GetMapProperties_Alt(i + 1, j))
				_ThisPoint.alt = GetMapProperties_Alt(i + 1, j);
			// Если в следующей вершине высота больше, берем ее, если нет - то не берем.
			if (_ThisPoint.alt < GetMapProperties_Alt(i, j + 1))
				_ThisPoint.alt = GetMapProperties_Alt(i, j + 1);
			// Если в следующей вершине высота больше, берем ее, если нет - то не берем.
			if (_ThisPoint.alt < GetMapProperties_Alt(i + 1, j + 1))
				_ThisPoint.alt = GetMapProperties_Alt(i + 1, j + 1);
			
			return _ThisPoint.alt;
		}
	}
	// Если попали сюда, значит был указан несуществующий режим, возвращаем некорректное значение
	return 0xFFFF;
}



