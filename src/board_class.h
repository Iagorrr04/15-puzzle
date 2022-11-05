// board_class.h
#ifndef BOARD_CLASS_H
#define BOARD_CLASS_H
#include <bits/stdc++.h>
using namespace std;
int N, M;

class board{
    public:
        int N, M;
        int x, y;
        vector<vector<int>> state;
        string moves;

        // initialize given the dimensions and values.
        void initialize_board(int N, int M, vector<vector<int>> state){
            this->N = N;
            this->M = M;
            this->state = state;
        }

        // set the board as the final state.
        void initialize_final_state(int N, int M){
            this->N = N;
            this->M = M;
            // empty cell position.
            this-> x = 0;
            this-> y = 0;

            vector<vector<int>> state(N, vector<int>(M));
            int v =0;
            for(int i = 0; i < N; ++i)
                for(int j = 0; j < M; ++j)
                    state[i][j] = v++;
            this->state = state;
        }

        // verify if the current state is the final one. O(N^2) pretty bad.
        bool check_end(board final_state){
            
            for(int i = 0; i < this->N; ++i)
                for(int j = 0; j < this-> M; ++j)
                    if(this->state[i][j] != final_state.state[i][j]) return false;
            return true;
        }

        // show the current state in terminal.
        void print_it(){
            cout << '\n';
            for(int i = 0; i < this-> N; ++i){
                for(int j = 0; j < this-> M; ++j){
                    cout << "[" << this->state[i][j] << "] ";
                }
                cout << '\n';
            }
            cout << '\n';
        }

        // each possible move.
        bool move_up(){
            // can't go up.
            if(this->x == 0) return false;

            int t = this->state[x][y];
            this->state[x][y] = this->state[x-1][y];
            this->state[x-1][y] = t;
            this->moves.push_back('U');
            x--;
            
            return true;
        }

        bool move_down(){
            // can't go down.
            if(this->x == N-1) return false;
            
            int t = this->state[x][y];
            this->state[x][y] = this->state[x+1][y];
            this->state[x+1][y] = t;
            this->moves.push_back('D');
            x++;
            
            return true;
        }

        bool move_left(){
            // can't go left
            if(this->y == 0) return false;
            
            int t = this->state[x][y];
            this->state[x][y] = this->state[x][y-1];
            this->state[x][y-1] = t;
            this->moves.push_back('L');
            y--;

            return true;
        }

        bool move_right(){
            // can't go right.
            if(this->y == M-1) return false;
            
            int t = this->state[x][y];
            this->state[x][y] = this->state[x][y+1];
            this->state[x][y+1] = t;
            this->moves.push_back('R');
            y++;
            
            return true;
        }
};
board final_state;
#endif // BOARD_CLASS_H