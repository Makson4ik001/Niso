#pragma once

#include<vector>


//7
struct coordinats {
	float x;
	float y;
};

struct Contur {
	long pointCount;
	short Levelndex;
	std::vector<coordinats> deviation_coordinates;
	long fPointCount;
	std::vector<long> fPoinIndex;
};

struct Invision {
	long Kontrol_Sum;
	std::vector<char> name_lvc;
	float fi_0;
	float lambda_0;
	long contCount;
	std::vector<Contur> contur;
};