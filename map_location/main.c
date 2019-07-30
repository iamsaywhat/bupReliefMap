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
	// Масштаб по широте (уже в десятичном виде)
	*(double*)(map + 38) = 1.3248;//0.000368;
	temp = *(double*)(map + 38);
	// Масштаб по долготе (уже в десятичном виде)
	*(double*)(map + 46) = 0.72;// 0.000200;
	temp = *(double*)(map + 46);
	// Далее пишем высоты
	for (i = 0; i < 160000; i++)
	{
		*(short*)(map + 54 + 2 * i) = (unsigned char)i;
		temp = *(short*)(map + 54 + 2 * i);
	}



int main(void)
{

	point A;
	point B;
	point C;
	point D;
	point location;
	short alt;

	unsigned short packet_count;
	unsigned short filenum = 21;
	packet_count = (unsigned short)((filenum / 100.0));
	if(filenum % 100 != 0)
		packet_count++;

	filenum = 50;
	packet_count = (unsigned short)((filenum / 100.0));
	if (filenum % 100 != 0)
		packet_count++;

	filenum = 200;
	packet_count = (unsigned short)((filenum / 100.0));
	if (filenum % 100 != 0)
		packet_count++;



	map_load();


	A.lon = 2;
	A.lat = 8;
	A.alt = 5;

	B.lon = 8;
	B.lat = 8;
	B.alt = 9;

	C.lon = 2;
	C.lat = 3;
	C.alt = 4;

	D.lon = 8;
	D.lat = 3;
	D.lat = 7;


	location.lon = 3.2;
	location.lat = 7.1;
	location.alt = 0;

	//location.lon = 4.5;
	//location.lat = 3;
	//location.alt = 0;

	MapHeight(C, A, D, B, location);



	MapHeight(A, C, D, B, location);
	MapHeight(B, C, D, A, location);
	MapHeight(D, C, A, B, location);

	alt = GetHeight_OnThisPoint(30.31, 59.98, 8);
	alt = GetHeight_OnThisPoint(30.31, 59.98, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.31, 59.98, MAX_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.31, 59.98, MED_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, TRIANGULARTION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, MAX_APPROXIMATION);
	alt = GetHeight_OnThisPoint(30.310150, 59.97986, MED_APPROXIMATION);

}