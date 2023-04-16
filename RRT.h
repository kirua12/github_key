#pragma once
#include "Map.h"
#include "Node.h"
#include<cmath>
#include <cstdlib>


class RRT
{



	// ���� �ȼ� ���� ����
	int m_range;
	//����� ��
	int m_node_number;

	//��� ��ü�� ��Ƶδ� vector
	std::vector<Node> m_nodes;

public:
	RRT();
	// ���۰� ���� �������ִ� �ڵ�
	void setFirstEnd(cv::Mat maze_map);
	//rrt ��� �ڵ�
	void calculate(Map maze_map);
	//���ο� ��带 ã�� �Լ�
	Node findNewNode(cv::Mat maze, int *pre_node_number);
	//���� point ���� �Լ�
	cv::Point findRandomSample(int row, int col);
	// ��ֹ� üũ �Լ� ��ֹ��� ������ false ������ true
	bool checkObstacle(Node pre_node, cv::Mat maze, cv::Point *cordi);
	// distance�� ���� ����� node�� ã�´�. �������� ��ġ�� ���ų� ������ ������ -1�� ����
	int findPreNode(cv::Point cordi);
	//�� ��ǥ�� �Ÿ��� ������ִ� �Լ�
	double euclideanDistance(cv::Point a, cv::Point b);
	void mapVisualization(cv::Mat *maze ,int i);

};

