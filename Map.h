#pragma once
#include <opencv2/opencv.hpp>


class Map
{

	cv::Mat c_map;

public:


	//������
	Map();
	Map(std::string f_name);



	//get �Լ�
	cv::Mat GetMap();

};

