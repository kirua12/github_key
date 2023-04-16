


#include <string>
#include "RRT.h"


int main() {


	// 모든 맵을 돌려주는 함수
	for (int i = 1;i < 5; i++) {
		//파일 확장자 명
		std::string extention_name = ".bmp";

		//이미지의 순서
		std::string order = std::to_string(i);
		//파일 이름
		std::string file_name = order + extention_name;

		//맵 객체 선언
		Map maze_map(file_name);


		//rrt 객체
		RRT rrt = RRT();

		rrt.calculate(maze_map);



	}




}