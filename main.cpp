


#include <string>
#include "RRT.h"


int main() {


	// ��� ���� �����ִ� �Լ�
	for (int i = 1;i < 5; i++) {
		//���� Ȯ���� ��
		std::string extention_name = ".bmp";

		//�̹����� ����
		std::string order = std::to_string(i);
		//���� �̸�
		std::string file_name = order + extention_name;

		//�� ��ü ����
		Map maze_map(file_name);


		//rrt ��ü
		RRT rrt = RRT();

		rrt.calculate(maze_map);



	}




}