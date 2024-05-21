#pragma once
#include <vector>
#include "point.h"

/*  char 1  ����
short 2 �����
long  4
float  4
double  8
*/

//2

struct land;
struct OSUS;
struct OBSUS;
struct Arr_LAND;
struct Arr_IO;
struct dec_coordinats;



point readPoint(char* buffer, int& top) {
	point res;
	res.fi = *(float*)(buffer + top);
	top += sizeof(float);
	res.lambda = *(float*)(buffer + top);
	top += sizeof(float);
	return res;
}

struct dec_coordinats {
	double x_;//8 ����
	double y_;//8 ����
};



dec_coordinats readDECCOOR(char* buffer, int& top) {
	dec_coordinats res;
	res.x_ = *(double*)(buffer + top);
	top += sizeof(double);
	res.y_ = *(double*)(buffer + top);
	top += sizeof(double);
	return res;
}


struct Arr_IO {
	std::vector<dec_coordinats> io;//4 �����
	std::vector<dec_coordinats> real_land_line;//4 �����
	float h;//4 �����
	long prevIndex;//4 �����
	long nextIndex;//4 �����
	long poin_Count;//4 ����� ��������� �����
	long FPoint_Count;//4 ����� ���������
	std::vector<dec_coordinats> arr_point;//�������� FPoint_Count + poin_Count ���������. ������� ���������� ��������� ����� ����� ����� ���������� �����.
	long beginndex;//4 �����
	long endIndex;//4 �����
	long count2;//4 �����
	std::vector<dec_coordinats> contur;//�������� count2 ���������
};


Arr_IO readARRIO(char* buffer, int& top) {
	Arr_IO res;
	res.io.resize(4);
	for (int i = 0; i < 4; i++) {
		res.io[i] = readDECCOOR(buffer, top);
	}
	res.real_land_line.resize(4);
	for (int i = 0; i < 4; i++) {
		res.real_land_line[i] = readDECCOOR(buffer, top);
	}

	res.h = *(float*)(buffer + top);
	top += sizeof(float);

	res.prevIndex = *(long*)(buffer + top);
	top += sizeof(long);
	res.nextIndex = *(long*)(buffer + top);
	top += sizeof(long);
	res.poin_Count = *(long*)(buffer + top);
	top += sizeof(long);
	res.FPoint_Count = *(long*)(buffer + top);
	top += sizeof(long);

	long count = res.poin_Count + res.FPoint_Count;

	for (int i = 0; i < count; i++) {
		res.arr_point.push_back(readDECCOOR(buffer, top));
	}

	res.beginndex = *(long*)(buffer + top);
	top += sizeof(long);
	res.endIndex = *(long*)(buffer + top);
	top += sizeof(long);
	res.count2 = *(long*)(buffer + top);
	top += sizeof(long);

	for (int i = 0; i < res.count2; i++) {
		res.contur.push_back(readDECCOOR(buffer, top));
	}

	return res;
}


struct Arr_LAND {
	long ioCount;//4 �����
	std::vector<Arr_IO> arr_io;// ����������� ioCount ���
};



Arr_LAND readARRLAND(char* buffer, int& top) {
	Arr_LAND res;
	res.ioCount = *(long*)(buffer + top);
	top += sizeof(long);
	for (int i = 0; i < res.ioCount; i++) {

		res.arr_io.push_back(readARRIO(buffer, top));

	}
	return res;
}


struct OBSUS {
	float fi_0;// 4      ����� ��������
	float lambda_0;// ����������      ����� ��������
	long landCount;//4 �����(<=250)
	std::vector<Arr_LAND> arr_land;//����������� landCount ���
	double fi_min;//8 ����  ����� �����(� ��������)
	double fi_max;//8 ����
	double lambda_min;//8 ����
	double lambda_max;// 8 ����
};


struct OSUS {

	std::vector<long>pointCount_i;//����������� 20 ���  ��� i> polCount ���������� 0 . 0<=pointCount_i<=120
	std::vector<std::vector<point>> contur_i;// ����������� pulCount ���, ������ �� point �������� pointCount_i ���������

};

OSUS readOSUS(char* buffer, int& top, long polCount) {
	OSUS res;
	for (int i = 0; i < 20; i++)
	{
		res.pointCount_i.push_back(*(long*)(buffer + top));
		top += sizeof(long);
	}

	res.contur_i.resize(polCount);
	for (int j = 0; j < polCount; j++)
	{
		long count = res.pointCount_i[j];
		for (int i = 0; i < count; i++)
		{
			res.contur_i[j].push_back(readPoint(buffer, top));

		}
	}

	return res;
}


land readLand(char* buffer, int bufferSize);

struct land {

	long control_sum;// 4 �����
	long sizeFile;//4
	long Maket[3];//3 ���� �� 4 ����� ������������ ��� {0,0,0}
	long polCount; //from OSUS
	OSUS osus;//<=20 ���������
	OBSUS obsus;
};



OBSUS readOBSUS(char* buffer, int& top) {
	OBSUS res;

	res.fi_0 = *(float*)(buffer + top);
	top += sizeof(float);
	res.lambda_0 = *(float*)(buffer + top);
	top += sizeof(float);
	res.landCount = *(long*)(buffer + top);
	top += sizeof(long);

	for (int i = 0; i < res.landCount; i++) {
		res.arr_land.push_back(readARRLAND(buffer, top));
	}
	
	res.fi_min = *(double*)(buffer + top);
	top += sizeof(double);
	res.fi_max = *(double*)(buffer + top);
	top += sizeof(double);
	res.lambda_min = *(double*)(buffer + top);
	top += sizeof(double);
	res.lambda_max = *(double*)(buffer + top);
	top += sizeof(double);

	return res;
}



land readLand(char* buffer, int bufferSize) {

	land res;
	int top = 0;
	res.control_sum = *((long*)(buffer + top));
	top += sizeof(long);
	res.sizeFile = *((long*)(buffer + top));
	top += sizeof(long);
	for (int i = 0; i < 3; i++) {
		res.Maket[i] = *((long*)(buffer + top));
		top += sizeof(long);
	}


	res.polCount = *(long*)(buffer + top);
	top += sizeof(long);

	res.osus = readOSUS(buffer, top, res.polCount);

	res.obsus = readOBSUS(buffer, top);


	return res;
}






