// 做一些小測試
#include <iostream>
#include <stdio.h>
#include <array>
#include <string>
#include <string.h>
using namespace std;
struct Point{
    int x,y;
    Point() : Point(0, 0) {}
	Point(int x, int y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

const std::array<std::array<Point, 4>, 4> spots{{
            {Point(-1,-0),Point(-1,-1),Point(-1,2),Point(0,1)},
            {Point(-2,1),Point(-1,0),Point(-1,-1),Point(0,1)},
            {Point(-1,-1),Point(0,0),Point(0,1),Point(0,2)},
            {Point(-2,0),Point(-1,0),Point(-1,-1),Point(0,0)}
}};

std::array<std::array<int, 5>, 5> game;
int main(int argc, char** argv)
{
    for(int i = 0; i <5; i ++){
        for(int j = 0; j < 5; j ++)
            game[i][j] = 10*i + j;
    }
    for(int i = 0; i <5; i ++){
        for(int j = 0; j < 5; j ++)
            cout << game[i][j] << " ";
        cout << endl;
    }
    int target = 4;
    for(int i = target; i >= 0; i --){
        game[i] = game[i-1];
    }
    for(int i = 0; i < 5; i ++)
        game[0][i] = 0;
        
    for(int i = 0; i <5; i ++){
        for(int j = 0; j < 5; j ++)
            cout << game[i][j] << " ";
        cout << endl;
    }
    
    return 0;
}