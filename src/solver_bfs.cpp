#include "board_class.h"
#include "functions.h"

auto bfs(const board &initial, const board &target){

    queue<board> q;
    q.emplace(initial);

    map<ull, bool> visited;
    visited[initial.hash_t] = true;

    ll iteration = 0;
    while(not q.empty()){
        board current = q.front();
        q.pop();

        iteration++;

        // At the final state.
        if(current.manha_ideal_dis == 0){
            cout << "Found !!! iteration : " << iteration << '\n';
            return current;
        }
        ll prev_hash = current.hash_t;

        if(current.move_up() and !visited[current.hash_t]){
            q.push(current);
            visited[current.hash_t] = true;
            current.reverse_prev_mov();
            current.hash_t = prev_hash;
        }   

        if(current.move_right() and !visited[current.hash_t]){
            q.push(current);
            visited[current.hash_t] = true;
            current.reverse_prev_mov();
            current.hash_t = prev_hash;
        }

        if(current.move_down() and !visited[current.hash_t]){
            q.push(current);
            visited[current.hash_t] = true;
            current.reverse_prev_mov();
            current.hash_t = prev_hash;
        }

        if(current.move_left() and !visited[current.hash_t]){
            q.push(current);
            visited[current.hash_t] = true;
            current.reverse_prev_mov();
            current.hash_t = prev_hash;
        }
    }

}
int main(){
    // sws

    // Dimensions of the board.
    cin >> N >> M;

    // TODO: implement contruction.
    vvi initial_state(N, vector<int>(M));
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j)
            cin >> initial_state[i][j];

    // inital state.
    board initial;
    initial.initialize_board(N, M, initial_state);
    cout << "Initial : \n";
    initial.print_it();

    // final state default.
    board target;
    target.initialize_final_state(N, M);
    cout << "Target : \n";
    target.print_it();


    auto ans = bfs(initial, target);
    cout << "Final : \n";
    ans.print_it();
    cout << "Steps: " << '\n';
    cout << ans.moves.size() << '\n';
    cout << ans.moves << '\n';
    return 0;
}