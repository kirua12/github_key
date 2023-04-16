#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>



class Node
{
	// 이전 노드
	int m_pre_node_index;

	// 다음 노드들
	std::vector<int> m_next_node_index;
	
	//현채 위치
	cv::Point m_position;

public:

	Node(cv::Point position);
	Node() {};
	//set 함수
	void setPreNodeIndex(int pre);
	void setNextNodeIndex(int next);

	//get 함수
	std::vector<int> getNextNodeIndex();
	int getPretNodeIndex();
	cv::Point getPostion();



};

