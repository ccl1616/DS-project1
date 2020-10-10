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
            {Point(-1,0),Point(-1,1),Point(-1,2),Point(0,1)}, 
            {Point(-2,1),Point(-1,0),Point(-1,1),Point(0,1)}, 
            {Point(-1,1),Point(0,0),Point(0,1),Point(0,2)},
            {Point(-2,0),Point(-1,0),Point(-1,1),Point(0,0)},

            {Point(-2,0),Point(-1,0),Point(0,0),Point(0,1)},
            {Point(-1,0),Point(-1,1),Point(-1,2),Point(0,0)},
            {Point(-2,0),Point(-2,1),Point(-1,1),Point(0,1)}, 
            {Point(-1,2),Point(0,0),Point(0,1),Point(0,2)},

            {Point(-2,1),Point(-1,1),Point(0,0),Point(0,1)},
            {Point(-1,0),Point(0,0),Point(0,1),Point(0,2)},
            {Point(-2,0),Point(-2,1),Point(-1,0),Point(0,0)},
            {Point(-1,0),Point(-1,1),Point(-1,2),Point(0,2)}, 

            {Point(-1,1),Point(-1,2),Point(0,0),Point(0,1)},
            {Point(-2,0),Point(-1,0),Point(-1,1),Point(0,1)}, 
            {Point(-1,0),Point(-1,1),Point(0,1),Point(0,2)}, 
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
        int x = -1;
        int y = -1;

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
            if(x == -1 && y == -1){
                if(pos + width + move > board_col || pos+move < 0) {
                    cout << "over width\n";
                    return false;
                }
                if(height > board_row) {
                    cout << "over height\n";
                    return false;
                }
            }
            else{
                // check it by now pos
            }
            return true;
        }
};

class GameBoard{
    public:
        int highest;
        GameBoard(){
            // reset
            highest = board_row-1;
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

        bool is_empty(Tetris falling); //check this place is able to drop
        bool check_drop(Tetris &falling); //check next row
        void do_drop_move(Tetris &falling,int x,int y); // drop to x,y
        bool put_in(Tetris falling); //check valid, put in by current x,y
        
        bool check_move(Tetris &falling);
        void clean();
};

bool GameBoard::is_empty(Tetris f){
    for(int i = 0; i < 4; i ++){
        Point dir = spots[f.id][i];
        //cout << "x:" <<x << "," << "y:" << y << endl;
        //cout << dir.x << "," << dir.y << endl;
        //cout << "going to put:" << x+dir.x << "," << y+dir.y << endl;
        if(game[f.x+dir.x][f.y+dir.y]) return false; // is occupied
    }
    return true;
}
bool GameBoard::check_drop(Tetris &falling){
    if(falling.x == board_row-1) return false;
    int x = falling.x+1;
    int y = falling.y;
    //cout << "check_drop: " << x << "," << y << endl;
    for(int i = 0; i < 4; i ++){
        Point dir = spots[falling.id][i];
        //cout << "x:" <<x << "," << "y:" << y << endl;
        //cout << dir.x << "," << dir.y << endl;
        //cout << "going to put:" << x+dir.x << "," << y+dir.y << endl;
        if(game[x+dir.x][y+dir.y]) return false; // occupied
    }
    if(x == board_row-1){
        do_drop_move(falling,x,y);
        return false;
    }
    do_drop_move(falling,x,y);
    if(!check_drop(falling)) return false;
    return false;
}
void GameBoard::do_drop_move(Tetris &falling,int x, int y){
    falling.x = x;
    falling.y = y;
}
bool GameBoard::put_in(Tetris f){
    for(int i = 0; i < 4; i ++){
        Point dir = spots[f.id][i];
        game[f.x+dir.x][f.y+dir.y]=1; // occupied
        highest = min(highest,f.x+dir.x); //越高的row id會是越小的
    }
    return true;
}

bool GameBoard::check_move(Tetris &falling){
    cout << "before move: " << falling.x << "," << falling.y << endl;
    int x = falling.x;
    int y = falling.y+falling.move;
    cout << "going to move to: " << x << "," << y << endl;

    if(y < 0 || y >= board_col) return false;
    for(int i = 0; i < 4; i ++){
        Point dir = spots[falling.id][i];
        if(game[x+dir.x][y+dir.y]) return false; // occupied
    }
    //falling.move = 0;
    do_drop_move(falling,x,y);
    return true;
}
void GameBoard::clean(){
            // check need to clean or not
            // if no return
            // then clean
            // drop all above
            return;
}

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
    bool first = true;
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
            if(first){
                now.x = mygame->highest;
                now.y = now.pos+move;
                //cout << now.x << "," << now.y << endl;
                mygame->put_in(now);
                first = 0;
            }
            else {
                now.x = mygame->highest-1;
                now.y = now.pos;
                //cout << inputName << ":" << now.x << "," << now.y << endl;
                if(! mygame->is_empty(now) ){
                    cout << "this place in not empty\n";
                    return 0;
                }
                if(!mygame->check_drop(now) ){
                    cout << "after drop pt: " << now.x << "," << now.y << endl;
                    if(move != 0){
                        if( !mygame->check_move(now)){
                            cout << "invalid to move\n";
                            return 0;
                        }
                        if( !mygame->check_drop(now))
                            mygame->put_in(now);
                    }
                    else mygame->put_in(now);
                }
            }
            // 看要不要move
            /*
            if(!move){
                if(! mygame->move(now) ){
                    cout << "move failed\n";
                    return 0;
                }
            }
            */
        }
    }
    // ==========================================================================================
    
    for(int i = 0; i < board_row; i ++){
        for(int j = 0; j < board_col; j ++){
            cout << game[i][j] << " ";
        }
        cout << endl;
    }
    cout << "1 2 3 4 5 6 7 8 9 0 1 2\n";
    cout << "end of game, remember to remove this line!!\n"; 
	return 0;
}