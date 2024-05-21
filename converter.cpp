#include "converter.h"
#include<math.h>
Converter::Converter() : Converter({ 0.f, 0.f }) {}

Converter::Converter(point anchor) : anchor(anchor) {
	a = 6378.245;
	epsSquared = 0.0066934;
	Roo = a * (1 - 0.5 * epsSquared * sin(anchor.fi));
	delta_fi = epsSquared * sin(anchor.fi) * cos(anchor.fi);
	Xo = Roo * cos(anchor.fi - delta_fi) * cos(anchor.lambda - anchor.lambda);
	Yo = Roo * cos(anchor.fi - delta_fi) * sin(anchor.lambda - anchor.lambda);
	Zo = Roo * sin(anchor.fi - delta_fi);
	COSxy = 0;
	COSzx = 0;
	COSzy = 1;
	COSzz = 0;
	COSxx = -Zo / Roo;
	COSxz = Xo / Roo;
}

point_dec_d Converter::convertTo(point from) const{
	point_dec_d res;
	float delta_fi = epsSquared * sin(from.fi) * cos(from.fi);
	float X = Roo * static_cast<float>( cos(from.fi - delta_fi) * cos(from.lambda - anchor.lambda));
	float Y = Roo * static_cast<float>(cos(from.fi - delta_fi) * sin(from.lambda - anchor.lambda));
	float Z = Roo * static_cast<float>(sin(from.fi - delta_fi));

	res.x = (X - Xo) * COSxx + (Y - Yo) * COSxy + (Z - Zo) * COSxz;
	res.y = (X - Xo) * COSzx + (Y - Yo) * COSzy + (Z - Zo) * COSzz;
	return res;
}
