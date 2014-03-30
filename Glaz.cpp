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
void showWarehouse(std::vector<std::vector<int>> warehouse, int w, int h);
coordinates drawRobotPath(std::vector<std::vector<int>> &warehouse, robot drawingRobot, int w, int h, int x, int y);
bool checkRobotsPath(std::vector<std::vector<int>> warehouse, robot drawingRobot, int w, int h, int x, int y);

int main(int argc, char** argv){
	int coverage;								//How much robots must clean
	int numberOfRobots;							//Number of robots on input
	int w,h;									//Width and Height of Warehouse
	std::vector<robot> robots;					//Vector when I keep robots from input

	//Reading from input
	readFromInput(w, h, coverage, numberOfRobots, robots);

	std::vector<std::vector<int>> warehouse(w ,std::vector<int>(h));
	//Warehouse: if field is equal 0 there is not robot else it is id of robot wich clean this field

	drawRobotPath(warehouse, robots[1], w, h, 1, 0);
	drawRobotPath(warehouse, robots[0], w, h, 0, 0);
	drawRobotPath(warehouse, robots[2], w, h, 2, 1);
	drawRobotPath(warehouse, robots[3], w, h, 1, 2);
	showWarehouse(warehouse, w, h);

	return 0;
}

void readFromInput(int &w, int &h, int &coverage, int &numberOfRobots, std::vector<robot> &robots){
	std::cin >> w >> h;
	std::cin >> coverage >> numberOfRobots;

	// std::cout << w << " " << h << "\n";
	// std::cout << coverage << "\n" << numberOfRobots << "\n";
	
	robots = std::vector<robot>(numberOfRobots);
	for(int i = 0; i < numberOfRobots; i++){
		std::cin >> robots[i].id >> robots[i].numberOfMoves;
		// std::cout << robots[i].id << "\n" << robots[i].numberOfMoves << "\n";

		if(robots[i].numberOfMoves != 0){
			robots[i].moves = std::vector<move>(robots[i].numberOfMoves);

			for(int j = 0; j < robots[i].numberOfMoves; j++){
				std::cin >> robots[i].moves[j].direct >> robots[i].moves[j].length;
				// std::cout << robots[i].moves[j].direct << " " << robots[i].moves[j].length << "\n";
			}
		}

		robots[i].startCoordinates.x = -1;
		robots[i].startCoordinates.y = -1;
	}
}

void showWarehouse(std::vector<std::vector<int>> warehouse, int w, int h){
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			std::cout << warehouse[j][i] << " ";
		}
		std::cout << "\n";
	}
}

coordinates drawRobotPath(std::vector<std::vector<int>> &warehouse, robot drawingRobot, int w, int h, int x, int y){
	int dx = x;
	int dy = y;
	coordinates retCoordinates;
	retCoordinates.x = x;
	retCoordinates.y = y;

	warehouse[dx][dy] = drawingRobot.id;
	for(int i = 0; i < drawingRobot.numberOfMoves; i++){
		switch(drawingRobot.moves[i].direct){
			case 'W':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dx--;
					warehouse[dx][dy] = drawingRobot.id;
				}
			break;

			case 'E':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dx++;
					warehouse[dx][dy] = drawingRobot.id;
				}
			break;

			case 'N':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dy--;
					warehouse[dx][dy] = drawingRobot.id;
				}
			break;

			case 'S':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dy++;
					warehouse[dx][dy] = drawingRobot.id;
				}
			break;

			default:
			break;
		}
	}
	return retCoordinates;
}

bool checkRobotsPath(std::vector<std::vector<int>> warehouse, robot drawingRobot, int w, int h, int x, int y){
	if(x < 0 || x >= w || y < 0 || y >= h || warehouse[x][y] != 0)
		return false;

	for(int i = 0; i < drawingRobot.numberOfMoves; i++){
		switch(drawingRobot.moves[i].direct){
			case 'W':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					x--;
					if(x < 0 || warehouse[x][y] != 0)
						return false;
				}
			break;

			case 'E':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					x++;
					if(x >= w || warehouse[x][y] != 0)
						return false;
				}
			break;

			case 'N':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					y--;
					if(y < 0 || warehouse[x][y] != 0)
						return false;
				}
			break;

			case 'S':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					y++;
					if(y >= h || warehouse[x][y] != 0)
						return false;
				}
			break;

			default:
			break;
		}
	}
	return true;
}