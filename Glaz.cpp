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
	bool isUsed;
};

int robotsNo;

void readFromInput(int &w, int &h, int &coverage, int &numberOfRobots, std::vector<robot> &robots);
void showWarehouse(std::vector<std::vector<int>> warehouse, int w, int h);
coordinates drawRobotPath(std::vector<std::vector<int>> &warehouse, robot drawingRobot, int w, int h, int x, int y, int fill);
bool checkRobotsPath(std::vector<std::vector<int>> warehouse, robot drawingRobot, int w, int h, int x, int y);
void chooseRobots(std::vector<std::vector<int>> &warehouse, std::vector<robot> &robots, int w, int h, int iter_robots, int iter_h, int iter_w, int coverage);
bool isCoverage(std::vector<std::vector<int>> &warehouse, int coverage, int w, int h);

int main(int argc, char** argv){
	int coverage;								//How much robots must clean
	int numberOfRobots;							//Number of robots on input
	int w,h;									//Width and Height of Warehouse
	std::vector<robot> robots;					//Vector when I keep robots from input
	robotsNo = 0;

	//Reading from input
	readFromInput(w, h, coverage, numberOfRobots, robots);

	std::vector<std::vector<int>> warehouse(h ,std::vector<int>(w));
	//Warehouse: if field is equal 0 there is not robot else it is id of robot wich clean this field

	chooseRobots(warehouse, robots, w, h, 0, 0, 0, coverage);
	showWarehouse(warehouse, w, h);

	std::cout<<"\n" << robotsNo << "\n";
	for(int i = 0; i < robots.size(); i++){
		if(robots[i].isUsed)
			std::cout<<robots[i].id << " " << robots[i].startCoordinates.x + 1 << " " << robots[i].startCoordinates.y + 1 << "\n";
	}

	return 0;
}

void readFromInput(int &w, int &h, int &coverage, int &numberOfRobots, std::vector<robot> &robots){
	std::cin >> w >> h;
	std::cin >> coverage >> numberOfRobots;
	
	robots = std::vector<robot>(numberOfRobots);
	for(int i = 0; i < numberOfRobots; i++){
		std::cin >> robots[i].id >> robots[i].numberOfMoves;

		if(robots[i].numberOfMoves != 0){
			robots[i].moves = std::vector<move>(robots[i].numberOfMoves);

			for(int j = 0; j < robots[i].numberOfMoves; j++){
				std::cin >> robots[i].moves[j].direct >> robots[i].moves[j].length;
			}
		}

		robots[i].startCoordinates.x = -1;
		robots[i].startCoordinates.y = -1;
		robots[i].isUsed = false;
	}
}

void showWarehouse(std::vector<std::vector<int>> warehouse, int w, int h){
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++){
			std::cout << warehouse[i][j] << " ";
		}
		std::cout << "\n";
	}
}

coordinates drawRobotPath(std::vector<std::vector<int>> &warehouse, robot drawingRobot, int w, int h, int x, int y, int fill){
	int dx = x;
	int dy = y;
	coordinates retCoordinates;
	retCoordinates.x = x;
	retCoordinates.y = y;

	warehouse[dy][dx] = fill;
	for(int i = 0; i < drawingRobot.numberOfMoves; i++){
		switch(drawingRobot.moves[i].direct){
			case 'W':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dx--;
					warehouse[dy][dx] = fill;
				}
			break;

			case 'E':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dx++;
					warehouse[dy][dx] = fill;
				}
			break;

			case 'N':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dy--;
					warehouse[dy][dx] = fill;

				}
			break;

			case 'S':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					dy++;
					warehouse[dy][dx] = fill;
				}
			break;

			default:
			break;
		}
	}
	return retCoordinates;
}

bool checkRobotsPath(std::vector<std::vector<int>> warehouse, robot drawingRobot, int w, int h, int x, int y){
	if(x < 0 || x >= w || y < 0 || y >= h || warehouse[y][x] != 0)
		return false;

	for(int i = 0; i < drawingRobot.numberOfMoves; i++){
		switch(drawingRobot.moves[i].direct){
			case 'W':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					x--;
					if(x < 0 || warehouse[y][x] != 0)
						return false;
				}
			break;

			case 'E':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					x++;
					if(x >= w || warehouse[y][x] != 0)
						return false;
				}
			break;

			case 'N':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					y--;
					if(y < 0 || warehouse[y][x] != 0)
						return false;
				}
			break;

			case 'S':
				for(int j = 0; j < drawingRobot.moves[i].length; j++){
					y++;
					if(y >= h || warehouse[y][x] != 0)
						return false;
				}
			break;

			default:
			break;
		}
	}
	return true;
}

void chooseRobots(std::vector<std::vector<int>> &warehouse, std::vector<robot> &robots, int w, int h, int iter_robots, int iter_h, int iter_w, int coverage){

	for(int k = iter_robots; k < robots.size(); k++){
		if(!robots[k].isUsed)
			for(int i = iter_h; i < h; i++){
				for(int j = iter_w; j < w; j++){
					if(checkRobotsPath(warehouse, robots[k], w, h, j, i)){
						robots[k].startCoordinates = drawRobotPath(warehouse, robots[k], w, h, j, i, robots[k].id);
						robots[k].isUsed = true;
						robotsNo++;
						break;
					}
				}
				if(robots[k].isUsed)
					break;
			}
		iter_h = 0;
		iter_w = 0;
	}

	if(!isCoverage(warehouse, coverage, w, h)){
		int iter = robots.size() - 1;
		do{
			while(iter >= 0 && !robots[iter].isUsed)
				iter--;
			drawRobotPath(warehouse, robots[iter], w, h, robots[iter].startCoordinates.x, robots[iter].startCoordinates.y, 0);
			robots[iter].isUsed = false;
			// robots[iter].startCoordinates.x = -1;
			// robots[iter].startCoordinates.y = -1;
			robotsNo--;
		}while(robots[iter].startCoordinates.x == w - 1 && robots[iter].startCoordinates.y == h - 1);

		if(robots[iter].startCoordinates.x == w - 1){
			iter_w = 0;
			iter_h = robots[iter].startCoordinates.y + 1;
		}else{
			iter_w = robots[iter].startCoordinates.x + 1;
			iter_h = robots[iter].startCoordinates.y;
		}

		chooseRobots(warehouse, robots, w, h, iter, iter_h, iter_w, coverage);
	}
}

bool isCoverage(std::vector<std::vector<int>> &warehouse, int coverage, int w, int h){
	int count = 0;
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			if(warehouse[i][j] != 0)
				count++;
	if(count >= coverage)
		return true;
	else
		return false;
}