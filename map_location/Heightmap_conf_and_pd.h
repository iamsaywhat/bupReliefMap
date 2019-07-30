/***************************************************************************************************
		Heightmap_conf_and_pd - ��������� Heightmap, ��������� ��������, ������������������ 
		������� � ����������� �������� ��������. ��������� ��������� ������� � ����� �����
		� ������������ ������� ������������ �� ����� ���������
***************************************************************************************************/
#ifndef _HEIGHT_MAP_CONF_AND_PD_
#define _HEIGHT_MAP_CONF_AND_PD_


/***********************************************
      �������� ������ ���� �� �������
***********************************************/
double GetMapProperties_MapStepLon(void);

/***********************************************
	  �������� ������ ���� �� �����
***********************************************/
double GetMapProperties_MapStepLat(void);

/***********************************************
	  �������� ������� ������� �����
***********************************************/
double GetMapProperties_NullPointLon(void);

/***********************************************
	  �������� ������ ������� �����
***********************************************/
double GetMapProperties_NullPointLat(void);

/***********************************************
 �������� ����� ������� ����� ����� �� �������
***********************************************/
short GetMapProperties_LonCount(void);

/***********************************************
  �������� ����� ������� ����� ����� �� ������
***********************************************/
short GetMapProperties_LatCount(void);

/***********************************************
  �������� ������� ����� �����������
***********************************************/
double GetTouchDownPointLon(void);

/***********************************************
  �������� ������ ����� �����������
***********************************************/
double GetTouchDownPointLat(void);

/*****************************************************************
 ������ ������ � ������� ����� � ��������� (LON_INDEX, LAT_INDEX)
******************************************************************/
short GetMapProperties_Alt(unsigned int LON_INDEX, unsigned int LAT_INDEX);

#endif
