#include "RRT.h"



RRT::RRT()
{
	m_range = 15;
	
	m_node_number = 300;





	
}



void RRT::setFirstEnd(cv::Mat maze_map)
{

	// ��������� ���������� �̸� �־��ִ� �ڵ�
	///////////////////////////////////////////////////

	//�������
	cv::Point s_position(0, maze_map.rows - 1);
	//��������
	cv::Point e_position(maze_map.cols-1,0);

	//���۳��� ���� ���
	Node s_node(s_position);
	Node e_node(e_position);

	m_nodes.push_back(e_node);
	m_nodes.push_back(s_node);

	//////////////////////////////////////////////////

}

void RRT::calculate(Map maze_map)
{

	cv::Mat maze = maze_map.GetMap();

	setFirstEnd(maze);
	

	// �Ķ���� ���� ��ŭ ��带 �����ϴ� for��
	for (int i = 2; i < m_node_number-2; i++) {

		//���ο� ���� ���� ����� ��� �ѹ��� ����ϴ� ����
		int pre_node_number;

		Node new_node  = findNewNode(maze, &pre_node_number);

		m_nodes.push_back(new_node);

		//���ο� ��忡 ���尡��� ��带 ���� ���� ���� �� ���� ��忡�� ���� ���� �缳��
		m_nodes.at(pre_node_number).setNextNodeIndex(i);
		m_nodes.at(i).setPreNodeIndex(pre_node_number);

		mapVisualization(&maze, i);




	}
	
}



Node RRT::findNewNode(cv::Mat maze,int* pre_node_number)
{
	//���� ���� ��ǥ
	cv::Point cordi;




	// ���� ��ǥ�� ��ֹ��� ã��
	while (1) {
		cordi = findRandomSample(maze.rows, maze.cols);


		//���� ���Ÿ��� ���� ����� ��带 ã��
		*pre_node_number = findPreNode(cordi);
		if (*pre_node_number == -1) continue;
		Node pre_node = m_nodes.at(*pre_node_number);


		if (checkObstacle(pre_node, maze, &cordi) == true) break;
		


	}	



	return Node{cordi};
}

cv::Point RRT::findRandomSample(int row, int col)
{



	int random_x = rand() % col;
	int random_y = rand() % row;




	cv::Point result(random_x, random_y);

	return result;


}

bool RRT::checkObstacle(Node pre_node, cv::Mat maze, cv::Point *cordi)
{
	cv::Point pre_postion = pre_node.getPostion();

	//���� ��ġ ���� ����� ��ġ�� ���� �� ����

	double differ_y = pre_postion.y - (*cordi).y;
	double differ_x = (pre_postion.x - (*cordi).x);

	double angle = std::atan2(differ_y, differ_x);



	//���⿡���� ��� ���� �������ִ� ����
	int k = 1;

	if (std::tan(angle) < 0) {
		k = -1;
	}

	//���� ���� ũ�⸸ŭ ��ġ ����
	int new_x = pre_postion.x + m_range * cos(angle)*k;
	int new_y = pre_postion.y + m_range * sin(angle)*k;


	// �������� �� ��� �߰�����
	int middle_x = pre_postion.x + m_range * cos(angle) * k/2;
	int middle_y = pre_postion.y + m_range * sin(angle) * k/2;

	

	cvtColor(maze, maze, cv::COLOR_BGR2GRAY);





	//��ġ�� ����ų� ��ֹ��� ������ false�� ����
	if (new_x < 0 || new_x >= maze.cols || new_y < 0 || new_y >= maze.rows) return false;
	

	//��ֹ� Ȯ��
	if (maze.at<uchar>(new_y, new_x) < 255 || maze.at<uchar>(middle_y, middle_x) < 255) return false;



	(*cordi).x = new_x;
	(*cordi).y = new_y;

	//���� �����̶� ������ ���� 
	if (findPreNode(*cordi) == -1) return false;


	return true;
}

int RRT::findPreNode(cv::Point cordi)
{


	double min = 100000000;
	//���� ����� �ѹ�
	int pre_number = 0;
	double pre_min = min;

	//���� ���� �߿� �ּҰ��� ã�´�.
	for (int i = 1; i < m_nodes.size();i++) {

		min = std::min(euclideanDistance(m_nodes.at(i).getPostion(), cordi), min);

		//���� �޶����� pre_number���� ��ȯ
		if (pre_min != min)
			pre_number = i;

		pre_min = min;

	}

	//������ ���� ������ ���� ������
	if(min <= 10) return -1;

	return pre_number;

}




double RRT::euclideanDistance(cv::Point a, cv::Point b)
{
	double differ_x = a.x - b.x;
	double differ_y = a.y - b.y;

	
	return std::sqrt(std::pow(differ_x,2)+ std::pow(differ_y,2));
}

void RRT::mapVisualization(cv::Mat *maze,int i)
{
	cv::Point node_cordi = m_nodes.at(i).getPostion();
	int pre_node_index = m_nodes.at(i).getPretNodeIndex();
	cv::Point pre_node_cordi = m_nodes.at(pre_node_index).getPostion();
	cv::circle(*maze, node_cordi, 3, cv::Scalar(255, 0, 0), -1);

	cv::line(*maze, node_cordi, pre_node_cordi, cv::Scalar(255, 0, 0));

	cv::imshow("maze", *maze);
	cv::waitKey(30);
}


