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
// Converte uma cordenada bidirecional pra uni.
inline pair<int,int> uni_to_bi(const int pos){
    return {pos/M,pos%M-1};
}

struct State
{
    board_t board;
    int zero_pos;
    int f_score; // g_score somado da heurística.
    int g_score; // gasto até este estado.

    string moves;

    State(){}
    State(const board_t &board, int pos) : board{board}, zero_pos{pos}
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
                actions.push_back(dx == 1 ? 'R' : 'L');
        }
    }

    // Calcula a heurística de maneira muito top :)
    int calc_heuristic(){
        int dist_heuristic = 0;
        for(int i = 0; i < N; ++i)
            for(int j = 0; j < M; ++j)
                dist_heuristic += abs(i - (int)board[i*M+j]) + abs(j - (int)board[i*M+j]);
                
        return 0;
    }

    // muda a posição, assumindo que já é uma posição válida.
    State move(char action)
    {
        State new_state = *this;
        new_state.moves.push_back(action);

        pair<int,int> pos = uni_to_bi(zero_pos);
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
        swap(new_state.board[zero_pos], new_state.board[new_zero_pos]);
        new_state.zero_pos = new_zero_pos;
        return new_state;
    }

    // pra funcionar a fila de prioridade.
    bool operator<(const State &x) const
    {
        return f_score > x.f_score;
    }
    
};

// Nossa querida variação do a_star.
auto astra(const State &start)
{
    priority_queue<State> pq;// lembra que isso aqui é um astra.
    pq.emplace(start);

    // Dijkstra melhorado :)
    unordered_map<string, int> scores;
    while (not pq.empty())
    {
        cout << "ranamamamamama\n";
        State current = pq.top();
        pq.pop();

        for (auto action : current.get_possible_actions())
        {
            auto next = current.move(action);
            if (next.calc_heuristic() == 0)// significa que esse já é o estado final.
                return next;
            auto new_cost = current.g_score + 1;
            auto current_scores = scores[next.board];
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

    // leitura do input.
    cin >> N >> M;
    int x;
    string initial_board_t;
    int zero_position;
    for(int i = 0; i < N*M; ++i){
            cin >> x;
            zero_position = x == 0 ? i : zero_position;
            initial_board_t += (char) x;
    }

    // inicializar o state.
    
    State initial(initial_board_t, zero_position);

    // achar o estado final.
    State finalzao_top = astra(initial);

    // reconstruir os movimentos usado para chegar até o final.
    // auto moves = reconstruct_path(finalzao_top);  // esse auto é uma string !

    cout << finalzao_top.moves.size() << '\n';

    
    return 0;
}