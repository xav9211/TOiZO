#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

struct coordinates{
	int x,y;
};

struct move{
	char direct;
	int length;
};

struct robot{
	int id;
	int numberOfMoves;
	coordinates startCoordinates;
	std::vector<move> moves;
};

void readFromInput(int &w, int &h, int &coverage, int &numberOfRobots, std::vector<robot> &robots);

int main(int argc, char** argv){
	int coverage;						//How much robots must clean
	int numberOfRobots;					//Number of robots on input
	int w,h;							//Width and Height of Warehouse
	std::vector<robot> robots;			//Vector when I keep robots from input

	//Reading from input
	readFromInput(w, h, coverage, numberOfRobots, robots);

	//vector<vector<int>> warehouse(,vector<int>());

	return 0;
}

void readFromInput(int &w, int &h, int &coverage, int &numberOfRobots, std::vector<robot> &robots){
	std::cin >> w >> h;
	std::cin >> coverage >> numberOfRobots;

	std::cout << w << " " << h << "\n";
	std::cout << coverage << "\n" << numberOfRobots << "\n";
	
	robots = std::vector<robot>(numberOfRobots);
	for(int i = 0; i < numberOfRobots; i++){
		std::cin >> robots[i].id >> robots[i].numberOfMoves;
		std::cout << robots[i].id << "\n" << robots[i].numberOfMoves << "\n";

		if(robots[i].numberOfMoves != 0){
			robots[i].moves = std::vector<move>(robots[i].numberOfMoves);

			for(int j = 0; j < robots[i].numberOfMoves; j++){
				std::cin >> robots[i].moves[j].direct >> robots[i].moves[j].length;
				std::cout << robots[i].moves[j].direct << " " << robots[i].moves[j].length << "\n";
			}
		}

		robots[i].startCoordinates.x = -1;
		robots[i].startCoordinates.y = -1;
	}
}