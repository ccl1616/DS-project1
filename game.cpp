#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
using namespace std;
#define MAX_row 15
#define MAX_col 40
int board_row;
int board_col;
int rowfull_log[MAX_row];
int height_log[MAX_col];

struct Point{
    int x,y;
    Point() : Point(0, 0) {}
	Point(int x, int y) : x(x), y(y) {}
};
std::array<std::array<bool, MAX_col>, MAX_row> game;

const std::array<std::array<Point, 4>, 19> spots{{
            {Point(-1,-1),Point(-1,0),Point(-1,1),Point(0,0)}, // special
            {Point(-2,0),Point(-1,-1),Point(-1,0),Point(0,0)}, // spacial
            {Point(-1,1),Point(0,0),Point(0,1),Point(0,2)},
            {Point(-2,0),Point(-1,0),Point(-1,1),Point(0,0)},

            {Point(-2,0),Point(-1,0),Point(0,0),Point(0,1)},
            {Point(-1,0),Point(-1,1),Point(-1,2),Point(0,0)},
            {Point(-2,-1),Point(-2,0),Point(-1,0),Point(0,0)}, //special
            {Point(-1,2),Point(0,0),Point(0,1),Point(0,2)},

            {Point(-2,1),Point(-1,1),Point(0,0),Point(0,1)},
            {Point(-1,0),Point(0,0),Point(0,1),Point(0,2)},
            {Point(-2,0),Point(-2,1),Point(-1,0),Point(0,0)},
            {Point(-1,-2),Point(-1,-1),Point(-1,0),Point(0,0)}, //special

            {Point(-1,1),Point(-1,2),Point(0,0),Point(0,1)},
            {Point(-2,-1),Point(-1,-1),Point(-1,0),Point(0,0)}, //spacial
            {Point(-1,-1),Point(-1,0),Point(0,0),Point(0,1)}, //special
            {Point(-2,1),Point(-1,0),Point(-1,1),Point(0,0)},

            {Point(-3,0),Point(-2,0),Point(-1,0),Point(0,0)},
            {Point(0,0),Point(0,1),Point(0,2),Point(0,3)},
            {Point(-1,0),Point(-1,1),Point(0,0),Point(0,1)}
}};

class Tetris{
    public:
        string name;
        int id = 0;
        int pos = 0;
        int move = 0;
        int height;
        int width;

        Tetris(string name,int p, int m) :name(name),pos(p),move(m) {
            char a = name[0];
            int num = name[1]-'0';
            if(a == 'T'){
                if(num==1) {id=0; height=2; width=3;}
                else if(num==2) {id=1; height=3; width=2;}
                else if(num==3) {id=2; height=2; width=3;}
                else {id=3; height=3; width=2;}
            }
            else if(a == 'L'){
                if(num==1) {id=4; height=3; width=2;}
                else if(num==2) {id=5; height=2; width=3;}
                else if(num==3) {id=6; height=3; width=2;}
                else {id=7; height=2; width=3;}
            }
            else if(a == 'J'){
                if(num==1) {id=8; height=3; width=2;}
                else if(num==2) {id=9; height=2; width=3;}
                else if(num==3) {id=10; height=3; width=2;}
                else {id=11; height=2; width=3;}
            }
            else if(a == 'S'){
                if(num==1) {id=12; height=2; width=2;}
                else {id=13; height=2; width=2;}
            }
            else if(a == 'Z'){
                if(num==1) {id=14; height=2; width=2;}
                else {id=15; height=2; width=2;}
            }
            else if(a == 'I'){
                if(num==1) {id=16; height=4; width=1;}
                else {id=17; height=1; width=4;}
            }
            else if(a == 'O'){
                id=18; height=2; width=2;
            }

        }
        bool valid_in_board(){
            if(pos + width + move > board_col || pos+move < 0) {
                cout << "over width\n";
                return false;
            }
            if(height > board_row) {
                cout << "over height\n";
                return false;
            }
            else return true;
        }
};

class GameBoard{
    public:
        GameBoard(){
            // reset
            for(int i = 0; i < board_col; i ++)
                height_log[i] = 0;
            for(int i = 0; i < board_row; i ++)
                rowfull_log[i] = 0;
            for(int i = 0; i < board_row; i ++){
                for(int j = 0; j < board_col; j++)
                    game[i][j] = 0;
            }
        }
        ~GameBoard(){ }
        bool drop_move(Tetris falling){
            // check valid to drop or not
            int highest = height_log[falling.pos];
            int y = falling.pos;
            if(falling.id == 11){
                y = falling.pos+2;
                highest = height_log[y];
            }
            else if(falling.id == 0 || falling.id == 1 || falling.id == 6 || falling.id == 13 || falling.id == 14){
                y = falling.pos+1;
                highest = height_log[y];
            }
            else{
                for(int i = 1; i<=(falling.width-1) && i<= board_col; i ++){
                    if( height_log[falling.pos+i] > highest )
                        highest = height_log[i];
                }
                y = falling.pos;
            }
            int x = board_row-1-highest; 
            cout << x << "," << y << endl;
            
            // 要先判完 確定可以放 才可以四個都放進去
            for(int i = 0; i < 4; i ++){
                Point dir = spots[falling.id][i];
                //cout << "x:" <<x << "," << "y:" << y << endl;
                //cout << dir.x << "," << dir.y << endl;
                cout << "going to put:" << x+dir.x << "," << y+dir.y+falling.move << endl;
                if(game[x+dir.x][y+dir.y+falling.move]) return false; // occupied
            }
            for(int i = 0; i < 4; i ++){
                Point dir = spots[falling.id][i];
                game[x+dir.x][y+dir.y+falling.move] = 1;
                // update log
                rowfull_log[x+dir.x]++;
                //height_log[y+dir.y+falling.move]++; //這裡出錯了!!
                height_log[y+dir.y+falling.move] = max(height_log[y+dir.y+falling.move]++,board_row-(x+dir.x));
            }
            return true;
        } 
        void clean(){
            // check need to clean or not
            // if no return
            // then clean
            // drop all above
            return;
        }

};

int main(int argc, char** argv)
{
    // ==========================================================================================
    // input 
	assert(argc == 2); //assert if execution command not==2
	freopen(argv[1],"r",stdin);
    freopen("out.txt","w",stdout);
    cin >> board_row >> board_col;
    if(board_row > 15 || board_col > 40) {
        cout << "invalid matrix size\n";
        return 0;
    }
    GameBoard* mygame = new GameBoard();
    
    string inputName;
    string end = "End";
    int pos = 0, move = 0;
    while( cin >> inputName ){
        if( inputName.compare(end) == 0)
            break;
        else{
            cin >> pos >> move;
            Tetris now(inputName,pos-1,move);
            // check if out boundary
            if(! now.valid_in_board() ) {
                cout << "input is out of boundary\n";
                return 0;
            }
            // drop_move
            if(! mygame->drop_move(now) ){
                cout << "drop_move failed\n";
                return 0;
            }
        }
    }
    // ==========================================================================================
    
    for(int i = 0; i < board_row; i ++){
        for(int j = 0; j < board_col; j ++){
            cout << game[i][j] << " ";
        }
        cout << endl;
    }

    cout << "end of game, remember to remove this line!!\n"; 
	return 0;
}