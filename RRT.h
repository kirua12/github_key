#pragma once
#include "Map.h"
#include "Node.h"
#include<cmath>
#include <cstdlib>


class RRT
{



	// 랜덤 픽셀 지정 범위
	int m_range;
	//노드의 수
	int m_node_number;

	//노드 전체를 담아두는 vector
	std::vector<Node> m_nodes;

public:
	RRT();
	// 시작과 끝을 설정해주는 코드
	void setFirstEnd(cv::Mat maze_map);
	//rrt 계산 코드
	void calculate(Map maze_map);
	//새로운 노드를 찾는 함수
	Node findNewNode(cv::Mat maze, int *pre_node_number);
	//랜덤 point 생성 함수
	cv::Point findRandomSample(int row, int col);
	// 장애물 체크 함수 장애물이 있으면 false 없으면 true
	bool checkObstacle(Node pre_node, cv::Mat maze, cv::Point *cordi);
	// distance가 가장 가까운 node를 찾는다. 이전노드와 위치가 같거나 범위가 가까우면 -1를 리턴
	int findPreNode(cv::Point cordi);
	//두 좌표의 거리를 계산해주는 함수
	double euclideanDistance(cv::Point a, cv::Point b);
	void mapVisualization(cv::Mat *maze ,int i);

};

