#pragma once


#include "Deikstra.h"




class Converter {
public:
	Converter();
	Converter(point);
	point_dec_d convertTo(point) const;
	point convertFrom(point_dec) const;



private:
	float epsSquared;
	point anchor;
	float Roo;
	float a;
	float delta_fi;
	float Xo;
	float Yo;
	float Zo;
	float COSxy;
	float COSxx;
	float COSxz;
	float COSzx;
	float COSzy;
	float COSzz;

	

};