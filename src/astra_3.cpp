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

// Heurística do indiano só tentando otimizar.

#include <bits/stdc++.h>
using namespace std;
#define fastio ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);


using board_t = string;// tabuleiro vai ser uma string.
using puzzle_t = int;
puzzle_t N, M;

using ull = unsigned long long;
const ull MAX_STATES = 1e13;
// U R D L
const vector<pair<int, int>> BI_DIRECTIONS{ {0, 1}, {1, 0}, {0, -1}, {-1, 0}};
const vector<int> UNI_DIRECTIONS{ 0, 0, 0, 0};// TODO: RECEBER N E m EM TEMPO DE COMPILAÇÃO.

// Converte uma coordenada bidirecional pra uni.
inline puzzle_t bi_to_uni(const pair<puzzle_t, puzzle_t> &bi){
    return bi.first*M + bi.second;
}
// Converte uma coordenada bidirecional pra uni.
inline pair<puzzle_t, puzzle_t> uni_to_bi(const puzzle_t pos){
    return {pos/M,pos%M};
}

string final_board;

struct State
{
    board_t board;
    puzzle_t zero_pos;
    ull f_score; // g_score somado da heurística.
    ull g_score; // gasto até este estado.

    string moves;

    State(){}
    State(const board_t &board_, puzzle_t pos) : board{board_}, zero_pos{pos}, f_score{INT_MAX-666}, g_score{INT_MAX-666}, moves{}
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

    // Calcula a heurística do nó para o estado final.
    int calc_heuristic(){
        int manhas = 0;
        int i, j;
        for(i = 0; i < N; ++i)
            for(j = 0; j < M; ++j)
                    manhas += abs(i - (int)board[i*M+j] / M) + abs(j - ((int)board[i*M+j] % M));
        


        // Posição ideal daquela peça.
        int pR[(N * M) + 1];
        int pC[(N * M) + 1];
        int conflicts = 0;
        int r, c;
        for (r = 0; r < N; r++) {
            for (c = 0; c < M; c++) {
                pR[(int)board[r*M+c]] = r;
                pC[(int)board[r*M+c]] = c;
            }
        }

        // pegar os conflitos linha por linha.
        int cl, cr;
        for (r = 0; r < N; r++) {
            for (cl = 0; cl < M; cl++) {
                for (cr = cl + 1; cr < N; cr++) {
                    if ((int)final_board[r*M+cl] && (int)final_board[r*M+cr] && r == pR[(int)final_board[r*M+cl]] && pR[(int)final_board[r*M+cl]] == pR[(int)final_board[r*M+cr]] && pC[(int)final_board[r*M+cl]] > pC[(int)final_board[r*M+cl]]) {
                        conflicts++;
                    }
                }
            }
        }

        // pegar os conflitos linha por linha.
        int rU, rD;
        for (c = 0; c < N; c++) {
            for ( rU = 0; rU < N; rU++) {
                for (rD = rU + 1; rD < N; rD++) {
                    if ((int)final_board[rU*M+c] && (int)final_board[rD*M+c] && c == pC[(int)final_board[rU*M+c]] && pC[(int)final_board[rU*M+c]] == pC[(int)final_board[rD*M+c]] &&
                        pR[(int)final_board[rU*M+c]] > pR[(int)final_board[rD*M+c]]) {
                        conflicts++;
                    }
                }
            }
        }

        return manhas + 2 * conflicts;
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

        puzzle_t new_zero_pos = bi_to_uni(pos);
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
pair<State, ull> astra(State start)
{
    if (start.calc_heuristic() == 0)
        return {start, 0};

    priority_queue<State> pq;// lembra que isso aqui é um astra.
    start.g_score = 0;
    pq.emplace(start);

    unordered_map<string, ull> g_scores; // total de movimentos que foram gasto para chegar nesse estado na melhor ocasião.
    ull total_states = 0;
    ull total_iterations = 0;
    while (not pq.empty())
    {
        ++total_iterations;
        State current = pq.top();
        pq.pop();
        // cout << "States: " << total_states++ << " moves: " << current.g_score << '\n';
        if(g_scores.size() > MAX_STATES){
            cerr << "Máximo de nós atingido !\n";
            break;
        }

        for (char action : current.get_possible_actions())
        {   
            State next = current;
            next.move(action);

            ull next_heuristic = next.calc_heuristic();
            if (next_heuristic == 0)// significa que esse já é o estado final.
                return {next, total_iterations};

            ull new_cost = current.g_score + 1;
            auto &current_scores = g_scores[next.board];
            if (current_scores != 0 and current_scores <= new_cost)
                continue;
            
            next.g_score = new_cost;
            next.f_score = new_cost + next_heuristic;


            pq.push(next);
            current_scores = next.g_score;
        }
    }
    return {start, total_iterations};
}

int32_t main()
{
    fastio
    // cout << "Sinta o poder do astra!\n";

    int i;
    // leitura do input.
    cin >> N >> M;
    puzzle_t x;

    // inicializar o finalboard.
    for(i = 0; i < N*M; ++i) final_board[i]= (char)i;


    string initial_board;
    puzzle_t zero_position=0;
    for(i = 0; i < N*M; ++i){
            cin >> x;
            cout << x << ' ';
            zero_position = x == 0 ? i : zero_position;
            initial_board += (char)x;
            if(i % M == M-1) cout << '\n';
    }
    // inicializar o state inicial.
    State initial(initial_board, zero_position);

    // achar o estado final.
    auto [finalzao_top, total_i] = astra(initial);

    
    // só exibir a string de movimentos que conseguiu chegar até o final.
    cout << "Iterations: " << total_i << '\n';
    cout << finalzao_top.moves.size() << '\n';
    cout << finalzao_top.moves << '\n';
    return 0;
        
}