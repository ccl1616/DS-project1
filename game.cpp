#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define MAX_row 15
#define MAX_col 40
int board_row;
int board_col;
int clean_log[MAX_row];
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
            if(pos + width + move > board_col || pos+move < 0) {
                cout << "over width\n";
                return false;
            }
            if(height > board_row) {
                cout << "over height\n";
                return false;
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
                clean_log[i] = 0;
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
        bool check_move_path(Tetris falling);
        void check_clean();
        void do_clean(int target);
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
        if(f.x+dir.x < 0 || f.x+dir.x >= board_row || f.y+dir.y < 0 || f.y+dir.y >= board_col){
            cout << "invlaid to put: " << f.x+dir.x << "," << f.y+dir.y << endl;
            return false;
        }
        game[f.x+dir.x][f.y+dir.y]=1; // make it occupy this pos
        clean_log[f.x+dir.x]++;
        highest = min(highest,f.x+dir.x); //越高的row id會是越小的
    }
    return true;
}
bool GameBoard::check_move(Tetris &falling){
    int x = falling.x;
    int y = falling.y+falling.move;
    if(y < 0 || y >= board_col || !check_move_path(falling) ) return false;

    for(int i = 0; i < 4; i ++){
        Point dir = spots[falling.id][i];
        if(game[x+dir.x][y+dir.y]) return false; // occupied
    }
    //falling.move = 0;
    do_drop_move(falling,x,y);
    return true;
}
bool GameBoard::check_move_path(Tetris falling){
    int x = falling.x;
    int y = falling.y;
    int target = falling.y+falling.move;
    if(falling.move > 0){
        for(int col = y+1; col <= target; col ++){
            for(int i = 0; i < 4; i ++){
                Point dir = spots[falling.id][i];
                if(game[x+dir.x][col+dir.y]) {
                    cout << "hit side wall\n";
                    return false; // hit side wall
                }
            }
        }
    }
    else {
        for(int col = y-1; col >= target; col --){
            for(int i = 0; i < 4; i ++){
                Point dir = spots[falling.id][i];
                if(game[x+dir.x][col+dir.y]) {
                    cout << "hit side wall\n";
                    return false; // hit side wall
                }
            }
        }
    }
    return true;
}
void GameBoard::check_clean(){
    // check need to clean or not
    for(int i = 0; i < board_row; i ++){
        if(clean_log[i] == board_col)
            do_clean(i);
    }
    return;
}
void GameBoard::do_clean(int target){
    // clean the row i
    // 降下row i以上的人們
    // do the log
    for(int i = target; i >= 0; i --){
        game[i] = game[i-1];
        clean_log[i] = clean_log[i-1];
    }
    for(int i = 0; i < board_col; i ++)
        game[0][i] = 0;
    clean_log[0] = 0;
    return;
}

int main(int argc, char** argv)
{
    // ==========================================================================================
    // input 
    //assert if invalid execution command 
	assert(argc == 2);
    time_t start = time(NULL);
    clock_t start_clock = clock();

	freopen(argv[1],"r",stdin);
    freopen("out.txt","w",stdout);
    cin >> board_row >> board_col;
    if(board_row > 15 || board_col > 40) {
        cout << "invalid matrix size\n";
        exit(1);
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
            // input shape, exit if target pos in invalid
            // pos-1 cause input col starts from 1
            cin >> pos >> move;
            Tetris now(inputName,pos-1,move);
            if(! now.valid_in_board() ) {
                cout << "input is out of boundary\n";
                exit(1);
            }
            // do drop, move, clean
            // if its the first shape, just put it to the bottom
            // if not first, drop it above highest and operate
            if(first){
                now.x = mygame->highest;
                now.y = now.pos+move;
                if(! mygame->put_in(now))
                    exit(1);
                first = 0;
            }
            else {
                now.x = mygame->highest-1;
                now.y = now.pos;
                if(!mygame->check_drop(now) ){
                    if(move != 0){
                        if( !mygame->check_move(now)){
                            cout << "invalid to move\n";
                            exit(1);
                        }
                        if( !mygame->check_drop(now)){
                            if(! mygame->put_in(now))
                                exit(1);
                        }
                    }
                    else {
                        if(! mygame->put_in(now))
                            exit(1);
                    }
                }
                else{
                    cout << "invalid drop\n";
                    return 0;
                }
            }
            mygame->check_clean();
        }
    }
    // ==========================================================================================
    for(int i = 0; i < board_row; i ++){
        for(int j = 0; j < board_col; j ++){
            cout << game[i][j] << " ";
        }
        cout << endl;
    }
    //cout << "1 2 3 4 5 6 7 8 9 0 1 2\n";
    cout << "end of game, remember to remove this line!!\n"; 
    time_t stop = time(NULL);
    double duration = (double) difftime(stop,start);
    cout << "duration by time: " << duration << endl;

    clock_t stop_clock = clock();
    double duration_clock = ((double) (stop_clock-start_clock))/ CLOCKS_PER_SEC;
    cout << "duration by clock ticks: "<< duration_clock << endl;
	return 0;
}