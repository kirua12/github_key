#pragma once
#include <opencv2/opencv.hpp>


class Map
{

	cv::Mat c_map;

public:


	//생성자
	Map();
	Map(std::string f_name);



	//get 함수
	cv::Mat GetMap();

};

