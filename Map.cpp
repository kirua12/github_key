#include "Map.h"

Map::Map(std::string f_name)
{

	c_map = cv::imread(f_name);

	//시작점과 끝점 가시화
	cv::circle(c_map, cv::Point(0,c_map.rows-1), 3, cv::Scalar(0, 255, 0),-1);
	cv::circle(c_map, cv::Point(c_map.cols-1, 0), 3, cv::Scalar(255,0, 0), -1);

}

cv::Mat Map::GetMap()
{
	return c_map;
}
