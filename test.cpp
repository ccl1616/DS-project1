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

std::array<std::array<bool, 15>, 5> game;
int main(int argc, char** argv)
{
    /*
    for(int i = 0; i < 4; i ++)
        cout << spots[0][i].x << " "<< spots[0][i].y << " ";
    cout << endl;
    */
    /*
    int r,c;
    cin >> r >> c;
    string name;
    string end = "End";
    while(cin >> name){
        if( name.compare(end) == 0)
            break;
        else{
            cin >> r >> c;
            cout << name << " " << r << " " << c << endl;
        }
    }*/
    
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 14; j ++){
            cout << game[i][j] << " ";
        }
        cout << endl;
    }
    
    return 0;
}