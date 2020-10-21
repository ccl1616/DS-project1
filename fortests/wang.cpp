#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class teris_board
{
private:
    int row, col;
public:
    int **board;
    teris_board(int, int);
    ~teris_board();
    void check_fill();      //check, if horizen_fill, move all down
    int check_error();      //check, if the game terminate due to fail, return 1
    int check_valid_move(int, int, int*, int*); //check if blocks move is valid
    void draw_on_board(int, int, int*, int*);   //draw blocks on board
    int getrow() { return row; }
    int getcol() { return col; }
};

class block
{
private:
public:
    int r_pos_y, r_pos_x;
    int occupied_y[4];
    int occupied_x[4];
    int move;
    block(string, int, int);
    ~block();
};

int main(int argc, char** argv)
{
    if(argc != 2) {
        return 1;
    }
    fstream fin, fout;
    fin.open(argv[1], ios::in);
    if(!fin) {
    }
    int n, m;
    fin >> n >> m;
    teris_board TB(n, m);
    string in_type;
    int in_pos, in_move; 
    while(fin >> in_type) {
        if(in_type == "End") {
            break;
        }
        fin >> in_pos >> in_move;
        block BL(in_type, in_pos, in_move);
        while(1) {
            if(TB.check_valid_move(BL.r_pos_y+1, BL.r_pos_x, BL.occupied_y, BL.occupied_x) == 1) {  //valid to move down, y++
                BL.r_pos_y++;
            } 
            else if(BL.move != 0) {     //invalid to move down, move left or right
                BL.r_pos_x += BL.move;
                BL.move = 0;
            } 
            else {      //invalid to move down or left or right, stop and draw
                TB.draw_on_board(BL.r_pos_y, BL.r_pos_x, BL.occupied_y, BL.occupied_x);
                break;
            }
        }
        TB.check_fill();
        if(TB.check_error()) {
            break;
        }
    }
    fin.close();
    fout.open("108062209_proj1.final", ios::out);
    //fout.open("output.data", ios::out);
    for(int i=4; i<TB.getrow(); i++) {
        fout << TB.board[i][0];
        for(int j=1; j<TB.getcol(); j++)
            fout << " " << TB.board[i][j];
        fout << "\n";
    }
    fout.close();
    return 0;
}

teris_board::teris_board(int r = 0, int c = 0)
    :row(r+4), col(c)   //plus 4 for reference point of blocks at top initially
{
    board = new int*[row];
    for(int i=0; i<row; i++)
        board[i] = new int[col];
    for(int i=0; i<row; i++)
        for(int j=0; j<col; j++)
            board[i][j] = 0;
}

teris_board::~teris_board()
{
    for(int i=0; i<row; i++)
        delete [] board[i];
    delete [] board;
}

void teris_board::check_fill()
{
    for(int i=row-1; i>0; i--) {    //check every row from bottom to top
        int fill = 1;
        for(int j=0; j<col; j++) {  //check if a row is filled
            if(board[i][j] == 0) {
                fill = 0;
                break;
            }
        }
        if(fill) {
            for(int j=i; j>0; j--) {    //elements of the row = elements of the row-1, and so does upper row
                for(int k=0; k<col; k++)
                    board[j][k] = board[j-1][k];
            }
            for(int j=0; j<col; j++)    //the top row = 0
                board[0][j] = 0;
            i++;                        //for close filled row
        }
    }
}

int teris_board::check_error()
{
    for(int i=0; i<4; i++) {
        for(int j=0; j<col; j++) {
            if(board[i][j] != 0)
                return 1;
        }
    }
    return 0;
}

int teris_board::check_valid_move(int r_y, int r_x, int* y, int* x)
{
    for(int i=0; i<4; i++) {
        if(r_y < 0 || r_y >= row || r_x < 0 || r_x >= col)
            return 0;
        else if(board[r_y + y[i]][r_x + x[i]] != 0)
            return 0;
    }
    return 1;
}

void teris_board::draw_on_board(int r_y, int r_x, int* y, int* x)
{
    for(int i=0; i<4; i++) {
        board[r_y + y[i]][r_x + x[i]] = 1;
    }
}

block::block(string type = "", int pos = 0, int m = 0)
    :r_pos_y(3), r_pos_x(pos-1), move(m)    //pos-1 for justify col_index
{
    if(type == "T1") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = -1;
        occupied_x[2] = 2;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "T2") {
        occupied_y[0] = -2;
        occupied_x[0] = 1;
        occupied_y[1] = -1;
        occupied_x[1] = 0;
        occupied_y[2] = -1;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "T3") {
        occupied_y[0] = -1;
        occupied_x[0] = 1;
        occupied_y[1] = 0;
        occupied_x[1] = 0;
        occupied_y[2] = 0;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 2;
    }
    else if(type == "T4") {
        occupied_y[0] = -2;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 0;
        occupied_y[2] = -1;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 0;
    }
    else if(type == "L1") {
        occupied_y[0] = -2;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 0;
        occupied_y[2] = 0;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "L2") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = -1;
        occupied_x[2] = 2;
        occupied_y[3] = 0;
        occupied_x[3] = 0;
    }
    else if(type == "L3") {
        occupied_y[0] = -2;
        occupied_x[0] = 0;
        occupied_y[1] = -2;
        occupied_x[1] = 1;
        occupied_y[2] = -1;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "L4") {
        occupied_y[0] = -1;
        occupied_x[0] = 2;
        occupied_y[1] = 0;
        occupied_x[1] = 0;
        occupied_y[2] = 0;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 2;
    }
    else if(type == "J1") {
        occupied_y[0] = -2;
        occupied_x[0] = 1;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = 0;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "J2") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = 0;
        occupied_x[1] = 0;
        occupied_y[2] = 0;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 2;
    }
    else if(type == "J3") {
        occupied_y[0] = -2;
        occupied_x[0] = 0;
        occupied_y[1] = -2;
        occupied_x[1] = 1;
        occupied_y[2] = -1;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 0;
    }
    else if(type == "J4") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = -1;
        occupied_x[2] = 2;
        occupied_y[3] = 0;
        occupied_x[3] = 2;
    }
    else if(type == "S1") {
        occupied_y[0] = -1;
        occupied_x[0] = 1;
        occupied_y[1] = -1;
        occupied_x[1] = 2;
        occupied_y[2] = 0;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "S2") {
        occupied_y[0] = -2;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 0;
        occupied_y[2] = -1;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
    else if(type == "Z1") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = 0;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 2;
    }
    else if(type == "Z2") {
        occupied_y[0] = -2;
        occupied_x[0] = 1;
        occupied_y[1] = -1;
        occupied_x[1] = 0;
        occupied_y[2] = -1;
        occupied_x[2] = 1;
        occupied_y[3] = 0;
        occupied_x[3] = 0;
    }
    else if(type == "I1") {
        occupied_y[0] = -3;
        occupied_x[0] = 0;
        occupied_y[1] = -2;
        occupied_x[1] = 0;
        occupied_y[2] = -1;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 0;
    }
    else if(type == "I2") {
        occupied_y[0] = 0;
        occupied_x[0] = 0;
        occupied_y[1] = 0;
        occupied_x[1] = 1;
        occupied_y[2] = 0;
        occupied_x[2] = 2;
        occupied_y[3] = 0;
        occupied_x[3] = 3;
    }
    else if(type == "O") {
        occupied_y[0] = -1;
        occupied_x[0] = 0;
        occupied_y[1] = -1;
        occupied_x[1] = 1;
        occupied_y[2] = 0;
        occupied_x[2] = 0;
        occupied_y[3] = 0;
        occupied_x[3] = 1;
    }
}

block::~block()
{
}

//g++ -o 108062209_proj1_first -O2 --std=c++11 108062209_proj1_first.cpp

//by Paul Wang
