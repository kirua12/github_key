#include "RRT.h"



RRT::RRT()
{
	m_range = 15;
	
	m_node_number = 300;





	
}



void RRT::setFirstEnd(cv::Mat maze_map)
{

	// 출발지점과 종료지점을 미리 넣어주는 코드
	///////////////////////////////////////////////////

	//출발지점
	cv::Point s_position(0, maze_map.rows - 1);
	//종료지점
	cv::Point e_position(maze_map.cols-1,0);

	//시작노드와 종료 노드
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
	

	// 파라미터 노드수 만큼 노드를 생성하는 for문
	for (int i = 2; i < m_node_number-2; i++) {

		//새로운 노드와 가장 가까운 노드 넘버를 기록하는 변수
		int pre_node_number;

		Node new_node  = findNewNode(maze, &pre_node_number);

		m_nodes.push_back(new_node);

		//새로운 노드에 가장가까운 노드를 이전 노드로 설정 및 이전 노드에서 현재 노드로 재설정
		m_nodes.at(pre_node_number).setNextNodeIndex(i);
		m_nodes.at(i).setPreNodeIndex(pre_node_number);

		mapVisualization(&maze, i);




	}
	
}



Node RRT::findNewNode(cv::Mat maze,int* pre_node_number)
{
	//랜덤 변수 좌표
	cv::Point cordi;




	// 랜덤 좌표에 장애물을 찾음
	while (1) {
		cordi = findRandomSample(maze.rows, maze.cols);


		//이전 노드거리와 가장 가까운 노드를 찾음
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

	//랜덤 위치 가장 가까운 위치의 기울기 와 각도

	double differ_y = pre_postion.y - (*cordi).y;
	double differ_x = (pre_postion.x - (*cordi).x);

	double angle = std::atan2(differ_y, differ_x);



	//기울기에따라 양수 음수 결정해주는 변수
	int k = 1;

	if (std::tan(angle) < 0) {
		k = -1;
	}

	//내가 정한 크기만큼 위치 지정
	int new_x = pre_postion.x + m_range * cos(angle)*k;
	int new_y = pre_postion.y + m_range * sin(angle)*k;


	// 이전노드와 새 노드 중간지점
	int middle_x = pre_postion.x + m_range * cos(angle) * k/2;
	int middle_y = pre_postion.y + m_range * sin(angle) * k/2;

	

	cvtColor(maze, maze, cv::COLOR_BGR2GRAY);





	//위치가 벗어나거나 장애물이 있으면 false를 리턴
	if (new_x < 0 || new_x >= maze.cols || new_y < 0 || new_y >= maze.rows) return false;
	

	//장애물 확인
	if (maze.at<uchar>(new_y, new_x) < 255 || maze.at<uchar>(middle_y, middle_x) < 255) return false;



	(*cordi).x = new_x;
	(*cordi).y = new_y;

	//이전 노드들이랑 가까우면 새로 
	if (findPreNode(*cordi) == -1) return false;


	return true;
}

int RRT::findPreNode(cv::Point cordi)
{


	double min = 100000000;
	//이전 노드의 넘버
	int pre_number = 0;
	double pre_min = min;

	//이전 노드들 중에 최소값을 찾는다.
	for (int i = 1; i < m_nodes.size();i++) {

		min = std::min(euclideanDistance(m_nodes.at(i).getPostion(), cordi), min);

		//값이 달라지면 pre_number값을 변환
		if (pre_min != min)
			pre_number = i;

		pre_min = min;

	}

	//범위의 절반 이하의 값이 있으면
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


