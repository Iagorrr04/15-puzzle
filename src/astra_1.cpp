/**
 * @file astra.cpp
 * @author noises tudo
 * @brief astra
 * @version 0.1
 * @date 2022-12-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include <bits/stdc++.h>
using namespace std;
#define fastio ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);


using board_t = string;// tabuleiro vai ser uma string.
int N, M;

// U R D L
const vector<pair<int, int>> BI_DIRECTIONS{ {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
const vector<int> UNI_DIRECTIONS{ 0, 0, 0, 0};// TODO: RECEBER N E m EM TEMPO DE COMPILAÇÃO.

// Converte uma coordenada bidirecional pra uni.
inline int bi_to_uni(const pair<int, int> &bi){
    return bi.first*M + bi.second;
}
// Converte uma coordenada bidirecional pra uni.
inline pair<int,int> uni_to_bi(const int pos){
    return {pos/M,pos%M};
}


struct State
{
    board_t board;
    int zero_pos;
    int f_score; // g_score somado da heurística.
    int g_score; // gasto até este estado.

    string moves;

    State(){}
    State(const board_t &board_, int pos) : board{board_}, zero_pos{pos}, f_score{INT_MAX-666}, g_score{INT_MAX-666}, moves{}
    {
    }

    // retorna uma string contendo as posições que pode ir.
    string get_possible_actions() const
    {
        string actions;
        auto [row, col] = uni_to_bi(zero_pos);
        for (auto [dx, dy] : BI_DIRECTIONS)
        {
            auto x = dx + row;
            auto y = dy + col;

            if (x < 0 or y < 0 or x >= N or y >= M)
                continue;

            if (dx)
                actions.push_back(dx == 1 ? 'D' : 'U');
            else
                actions.push_back(dy == 1 ? 'R' : 'L');
        }

        return actions;
    }

    // Calcula a heurística de maneira muito top :)
    int calc_heuristic(){
        int dist_heuristic = 0;
        for(int i = 0; i < N; ++i)
            for(int j = 0; j < M; ++j)
                    dist_heuristic += abs(i - (int)board[i*M+j] / M) + abs(j - ((int)board[i*M+j] % M));
        return dist_heuristic;
    }

    // muda a posição, assumindo que já é uma posição válida.
    void move(char action)
    {
        pair<int,int> pos = uni_to_bi(zero_pos);
        moves.push_back(action);
        switch (action)
        {
            case 'U':
                --pos.first;
                break;
            case 'R':
                ++pos.second;
                break;
            case 'D':
                ++pos.first;
                break;
            case 'L':
                --pos.second;
                break;
        }

        int new_zero_pos = bi_to_uni(pos);
        swap(board[zero_pos], board[new_zero_pos]);
        zero_pos = new_zero_pos;
    }

    // pra funcionar a fila de prioridade.
    bool operator<(const State &x) const
    {
        return f_score > x.f_score;
    }
    
};

// Nossa querida variação do a_star.
auto astra(State start)
{
    if (start.calc_heuristic() == 0)
        return start;

    priority_queue<State> pq;// lembra que isso aqui é um astra.
    start.g_score = 0;
    pq.emplace(start);

    // Dijkstra melhorado :)
    unordered_map<string, int> scores;
    while (not pq.empty())
    {
        State current = pq.top();
        pq.pop();
        
        for (auto action : current.get_possible_actions())
        {   
            State next = current;
            next.move(action);
            if (next.calc_heuristic() == 0)// significa que esse já é o estado final.
                return next;
            auto new_cost = current.g_score + 1;
            auto &current_scores = scores[next.board];
            if (current_scores != 0 and current_scores <= new_cost)
                continue;

            next.g_score = new_cost;
            next.f_score = new_cost + next.calc_heuristic();

            pq.push(next);
            current_scores = next.g_score;
        }
    }
    return start;
}

int32_t main()
{

    fastio
    cout << "Sinta o poder do astra !\n";

    // leitura do input.
    cin >> N >> M;
    int x;
    string initial_board;
    int zero_position=666;
    for(int i = 0; i < N*M; ++i){
            cin >> x;
            zero_position = x == 0 ? i : zero_position;
            initial_board += (char) x;
    }

    // inicializar o state.
    State initial(initial_board, zero_position);

    // achar o estado final.
    State finalzao_top = astra(initial);

    // reconstruir os movimentos usado para chegar até o final.
    // auto moves = reconstruct_path(finalzao_top);  // esse auto é uma string !

    cout << finalzao_top.moves << '\n';
    cout << finalzao_top.moves.size() << '\n';
    return 0;
        
}