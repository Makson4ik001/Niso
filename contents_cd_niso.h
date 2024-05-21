#pragma once

#include<vector>



struct folder {
	std::vector<char> Name_folder;
	long Size_foder;
};

struct struct_name{
	long Control_Sum;
	std::vector<char> id_CD;
	long Kol_folder;
	std::vector<folder> folder;
};

