#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <time.h>
using namespace std;
#define MAX_row 15
#define MAX_col 40
int board_row;
int board_col;
int clean_log[MAX_row];
int h_log[MAX_col]; //height log
int number_tc = 0;

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
        int x = 0;
        int y = 0;

        Tetris(string name,int p, int m) :name(name),pos(p),move(m) {
            char a = name[0];
            int num = name[1]-'0';
            if(a == 'T'){
                if(num==1) {y = 1; id=0; height=2; width=3;}//
                else if(num==2) {y = 1; id=1; height=3; width=2;}//
                else if(num==3) {id=2; height=2; width=3;}
                else {id=3; height=3; width=2;}
            }
            else if(a == 'L'){
                if(num==1) {id=4; height=3; width=2;}
                else if(num==2) {id=5; height=2; width=3;}
                else if(num==3) {y = 1; id=6; height=3; width=2;}//
                else {id=7; height=2; width=3;}
            }
            else if(a == 'J'){
                if(num==1) {id=8; height=3; width=2;}
                else if(num==2) {id=9; height=2; width=3;}
                else if(num==3) {id=10; height=3; width=2;}
                else {y = 2; id=11; height=2; width=3;}//
            }
            else if(a == 'S'){
                if(num==1) {id=12; height=2; width=2;}
                else {y = 1; id=13; height=2; width=2;}//
            }
            else if(a == 'Z'){
                if(num==1) {y =1; id=14; height=2; width=2;}//
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
            highest = board_row;
            for(int i = 0; i < board_row; i ++){
                clean_log[i] = 0;
                for(int j = 0; j < board_col; j++){
                    game[i][j] = 0;
                    h_log[j] = board_row;
                }
            }
        }
        ~GameBoard(){ }

        bool check_drop(Tetris &falling); //check can drop to next row or not
        void do_drop_move(Tetris &falling,int x,int y); //change it pos to target spot, used after check_drop & check_move
        bool put_in(Tetris falling); //check spot valid, put in by current x,y
        bool check_move(Tetris &falling); //check can move by input, call check_move_path to check along path
        bool check_move_path(Tetris falling); //check side wall
        void check_clean();
        void do_clean(int target);
        void call_print();
        int h_width(Tetris falling);
};

bool GameBoard::check_drop(Tetris &falling){
    if(falling.x == board_row-1) return false; //already at the bottom
    int x = falling.x;
    int y = falling.y;

    while(x != board_row-1){
        x ++; //try one down
        for(int i = 0; i < 4; i ++){
            Point dir = spots[falling.id][i];
            if(falling.x+dir.x < 0 || falling.x+dir.x >= board_row || falling.y+dir.y < 0 || falling.y+dir.y >= board_col){
                cout << "invalid: check_drop out bound" << falling.name << " " << number_tc << endl;
                exit(1);
            }
            if(game[x+dir.x][y+dir.y]) {
                x--; //go back one row
                do_drop_move(falling,x,y);
                return false;
            }
        }
    }
    do_drop_move(falling,x,y);
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
            cout << "invalid: shape would be out of boundary" << f.name << " " << f.pos << endl;
            return false;
        }
        if(game[f.x+dir.x][f.y+dir.y]) {
            cout << "invalid: here is occupied " << number_tc << endl;
            return false;
        }
        game[f.x+dir.x][f.y+dir.y]=1; // make it occupy this pos
        clean_log[f.x+dir.x]++;
        h_log[f.y+dir.y] = min(h_log[f.y+dir.y],f.x+dir.x); //越高的row id會是越小的
    }
    return true;
}
bool GameBoard::check_move(Tetris &falling){
    int x = falling.x;
    int y = falling.y+falling.move;
    if(y < 0 || y >= board_col || !check_move_path(falling) ) {
        cout << "invalid: moving out of boundary " << number_tc << endl;
        return false;
    }

    for(int i = 0; i < 4; i ++){
        Point dir = spots[falling.id][i];
        if(falling.x+dir.x < 0 || falling.x+dir.x >= board_row || falling.y+dir.y < 0 || falling.y+dir.y >= board_col){
            cout << "invalid: check_move out bound " << falling.name << " " << number_tc << endl;
            return false;
        }
        if(game[x+dir.x][y+dir.y]) {
            cout << "invalid: check_move destination is occupied, at " << x+dir.x << "," << y+dir.y << endl;
            return false; // occupied
        }
    }
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
                //cout << number_tc << " " << x+dir.x << "," << y+dir.y << endl;
                if(game[x+dir.x][col+dir.y]) {
                    cout << "invalid: hit side wall " << falling.name << falling.pos << x+dir.x << "," << y+dir.y << endl;
                    return false; // hit side wall
                }
            }
        }
    }
    else {
        for(int col = y-1; col >= target; col --){
            for(int i = 0; i < 4; i ++){
                Point dir = spots[falling.id][i];
                //cout << number_tc << " " << x+dir.x << "," << y+dir.y << endl;
                if(game[x+dir.x][col+dir.y]) {
                    cout << "invalid: hit side wall"  << falling.name << falling.pos << x+dir.x << "," << y+dir.y << endl;
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
    // i不可以=0 ! 否則i-1=-1 !
    for(int i = target; i > 0; i --){
        game[i] = game[i-1];
        clean_log[i] = clean_log[i-1];
    }
    for(int i = 0; i < board_col; i ++){
        game[0][i] = 0;
        h_log[i]++; //downward one row
    }
    clean_log[0] = 0;
    return;
}
void GameBoard::call_print()
{
    for(int i = 0; i < board_row; i ++){
        for(int j = 0; j < board_col; j ++){
            cout << game[i][j] << " ";
        }
        if(i != board_row-1) cout << endl;
    }
    cout << endl << "1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 |\n";
}
int GameBoard::h_width(Tetris f){
    
    int ans = board_row;
    for(int i = 0; i < 4; i ++){
        Point dir = spots[f.id][i];
        ans = min(h_log[f.y+dir.y], ans);
    }
    return ans;
}

int main(int argc, char** argv)
{
    // ==========================================================================================
    // input and calculate
    time_t start = time(NULL);
    clock_t start_clock = clock();

	freopen(argv[1],"r",stdin);
    freopen("out.txt","w",stdout);
    cin >> board_row >> board_col;
    if(board_row > 15 || board_col > 40) {
        cout << "invalid: matrix size\n";
        exit(1);
    }
    GameBoard* mygame = new GameBoard();
    
    string inputName;
    string end = "End";
    int pos = 0, move = 0;
    bool first = true;
    while( cin >> inputName){
        if( inputName.compare(end) == 0 || number_tc > 1000 )
            break;
        else{
            number_tc++;
            // input shape, exit if target pos in invalid
            // pos-1 cause input col starts from 1
            cin >> pos >> move;
            Tetris now(inputName,pos-1,move);
            if(! now.valid_in_board() ) {
                cout << "invalid: input is out of boundary" << now.name << " " << now.pos << endl;
                exit(1);
            }
            // do drop, move, clean
            // if its the first shape, just put it to the bottom
            // if not first, drop it above highest and operate
            if(first){
                now.x = board_row-1;
                now.y += now.pos+move;
                //cout << number_tc << ":" << now.x << "," << now.y << endl;
                if(! mygame->put_in(now)){
                    cout << now.x << "," << now.y << endl;
                    cout << "invalid to put first tetris\n";
                    exit(1);
                }
                first = 0;
            }
            else {
                now.y += now.pos;
                now.x = mygame->h_width(now) -1;
                //cout << number_tc << ":" << now.x << "," << now.y << endl;
                if(now.x < 0){
                    cout << now.x << endl;
                    cout << "cannot drop in to the board\n";
                    break;
                }
                if(!mygame->check_drop(now) ){
                    //cout << "before move: ";
                    //cout << now.x << "," << now.y << endl;
                    if(move != 0){
                        if( !mygame->check_move(now)){
                            cout << "invalid: move" << now.name << " " << now.pos << endl;
                            break;
                            exit(1);
                        }
                        if( !mygame->check_drop(now)){
                            if(! mygame->put_in(now)){
                                cout << "invalid: drop-move-drop put_in" << now.name << " " << now.pos << endl;
                                break;
                                //exit(1);
                            }
                        }
                    }
                    else {
                        if(! mygame->put_in(now)){
                            cout << "invalid: drop put_in" << now.name << " " << now.pos << endl;
                            break;
                            //exit(1);
                        }
                    }
                }
                else {
                    cout << "invalid to drop\n";
                }
            }
            mygame->check_clean();
        }
    }
    // ==========================================================================================
    //cout << number_tc << endl;
    for(int i = 0; i < board_row; i ++){
        for(int j = 0; j < board_col; j ++){
            cout << game[i][j] << " ";
        }
        if(i != board_row-1) cout << endl;
    }
    //cout << endl << "1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 | 1 2 3 4 5 6 7 8 9 |\n";
    time_t stop = time(NULL);
    double duration = (double) difftime(stop,start);
    //cout << endl << "duration by time: " << duration << endl;

    clock_t stop_clock = clock();
    double duration_clock = ((double) (stop_clock-start_clock))/ CLOCKS_PER_SEC;
    //cout << endl << "duration by clock ticks: "<< duration_clock << endl;
    
    fclose (stdout);
	return 0;
}