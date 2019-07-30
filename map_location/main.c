#include <stdio.h>
#include "Heightmap.h"


char map[320054];

void map_load(void)
{
	double temp;
	long i;

	// Число широт
	*(short*)(map + 18) = 400;
	temp = *(short*)(map + 18);
	// Число долгот
	*(short*)(map + 20) = 400;
	temp = *(short*)(map + 20);
	// Левый нижний угол широта
	*(double*)(map + 22) = 59.956306;
	temp = *(double*)(map + 22);
	// Левый нижний угол долгота
	*(double*)(map + 30) = 30.291973;
	temp = *(double*)(map + 30);
	// Масштаб по широте: количество секунд в одном делении
	*(double*)(map + 38) = 20.0;
	temp = *(double*)(map + 38);
	// Масштаб по долготе: количество секунд в одном делении
	*(double*)(map + 46) = 20.0;
	temp = *(double*)(map + 46);
	// Далее пишем высоты
	for (i = 0; i < 160000; i++)
	{
		*(short*)(map + 54 + 2 * i) = (unsigned char)i;
		temp = *(short*)(map + 54 + 2 * i);
	}
}


int main(void)
{

	point A;
	point B;
	point C;
	point D;
	point location;
	short alt;

	map_load();

	alt = GetHeight_OnThisPoint(30.31, 59.98, 8);


	// Проверка угловых квадратов**********************************************************************
	// Точка в нижнем левом квадрате
	alt = GetHeight_OnThisPoint(30.292020, 59.956434, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.292020, 59.956434, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.292020, 59.956434, MAX_APPROXIMATION);
	// Точка в верхнем левом квадрате
	alt = GetHeight_OnThisPoint(30.292020, 62.175000, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.292020, 62.175000, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.292020, 62.175000, MAX_APPROXIMATION);
	// Точка в верхнем правом квадрате
	alt = GetHeight_OnThisPoint(32.514194, 62.175000, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(32.514194, 62.175000, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(32.514194, 62.175000, MAX_APPROXIMATION);
	// Точка в нижнем правом квадрате
	alt = GetHeight_OnThisPoint(32.514194, 59.956434, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(32.514194, 59.956434, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(32.514194, 59.956434, MAX_APPROXIMATION);
	// ***************************************************************************************************



	// Проверка выхода за пределы карты*******************************************************************
	// Точка слева от квадрата
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MAX_APPROXIMATION);
	// Точка сверху от квадрата
	alt = GetHeight_OnThisPoint(30.292020, 62.178600, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MAX_APPROXIMATION);
	// Точка справа от квадрата
	alt = GetHeight_OnThisPoint(32.514196, 62.175000, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.291972, 59.956434, MAX_APPROXIMATION);
	// Точка снизу от квадрата
	alt = GetHeight_OnThisPoint(32.514194, 59.9562, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(32.514194, 59.9562, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(32.514194, 59.9562, MAX_APPROXIMATION);
	// ****************************************************************************************************





	alt = GetHeight_OnThisPoint(30.511806, 60.11245, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(31.511806, 61.11245, TRIANGULARTION);



	alt = GetHeight_OnThisPoint(30.31, 59.98, MAX_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.31, 59.98, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, MAX_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, MED_APPROXIMATION);

}