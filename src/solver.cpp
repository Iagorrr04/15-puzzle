#include <bits/stdc++.h>
#include "heap.cpp"
using namespace std;

#ifndef ALGORITHM
#define ALGORITHM bfs
#endif

using board_t = vector<vector<int>>;
using hash_t = long long;
vector<pair<int, int>> directions{{1,0},{0,1},{-1,0},{0,-1}};
enum Action {LEFT = 1, UP, RIGHT, DOWN};
int N, M;
struct State
{
    State(){}
    State(const board_t &board_) : board{board_}
    {
        for (auto i = 0; i < N; i++)
            for (auto j = 0; j < M; j++)
                if (board[i][j] == 0)
                {
                    zero_pos = {i, j};
                    return;
                }
    }
    State(const State &state_, Action action_) : State(state_)
    {
        resolve_action(action_);
    }

    vector<Action> possible_actions() const
    {
        vector<Action> actions;
        for (auto [dx, dy] : directions)
        {
            auto x = dx + zero_pos.first;
            auto y = dy + zero_pos.second;
            if (x < 0 or y < 0 or x >= N or y >= M)
                continue;

            if (dx)
                actions.push_back(dx == 1 ? DOWN : UP);
            else
                actions.push_back(dy == 1 ? RIGHT : LEFT);
        }

        return actions;
    }

    void resolve_action(Action action_)
    {
        switch (action_)
        {
            case LEFT:
                swap(board[zero_pos.first][zero_pos.second], board[zero_pos.first][zero_pos.second-1]);
                --zero_pos.second;
                break;
            case RIGHT:
                swap(board[zero_pos.first][zero_pos.second], board[zero_pos.first][zero_pos.second+1]);
                ++zero_pos.second;
                break;
            case UP:
                swap(board[zero_pos.first][zero_pos.second], board[zero_pos.first-1][zero_pos.second]);
                --zero_pos.first;
                break;
            case DOWN:
                swap(board[zero_pos.first][zero_pos.second], board[zero_pos.first+1][zero_pos.second]);
                ++zero_pos.first;
                break;
        }

        calculated_hash = 0;
    }

    int compare(const State &other) const
    {
        auto dist = 0;
        for (auto i = 0; i < N; i++)
            for (auto j = 0; j < M; j++)
                dist += abs(board[i][j] - other.board[i][j]);
        return dist;
    }

    hash_t hash() const
    {
        if (calculated_hash)
            return calculated_hash;

        cerr << "Error! Must calculate hash first\n";
        return -1;
    }

    hash_t hash()
    {
        if (calculated_hash)
            return calculated_hash;

        calculated_hash = 389;
        for (const auto &row : board)
            for (const auto &x : row)
                calculated_hash = (calculated_hash * 33 + x) % 100000000003LL;

        return calculated_hash;
    }

    board_t board;
    pair<int, int> zero_pos;
    hash_t calculated_hash;
    // Keep F and G scores for optimization
    pair<int, int> scores;
};
struct {State state; int depth, iteration, distance;} best_match;

auto reconstruct_path(const map<hash_t, pair<hash_t, Action>> &visited)
{
    auto current_hash = best_match.state.hash();

    vector<Action> actions_path;
    while (current_hash)
    {
        auto it = visited.find(current_hash);
        if (it == visited.end())
        {
            cerr << "WTF?\n";
            break;
        }
        auto [next_hash, action] = it->second;
        actions_path.push_back(action);
        current_hash = next_hash;
    }
    reverse(actions_path.begin(), actions_path.end());

    best_match.depth = (int)actions_path.size() - 1;
    return actions_path;
}

auto bfs(const State &initial, const State &target)
{
    best_match = {initial, 0, 0, initial.compare(target)};

    queue<State> q;
    q.emplace(initial);

    map<hash_t, pair<hash_t, Action>> visited;
    visited[initial.hash()];

    auto iteration = 0;
    while (not q.empty() and best_match.distance)
    {
        auto state = q.front();
        q.pop();
        for (auto action : state.possible_actions())
        {
            State next(state, action);
            auto hs = next.hash();
            if (visited.count(hs))
                continue;

            auto dist = next.compare(target);
            if (dist < best_match.distance)
                best_match = {next, 0, iteration, dist};

            q.emplace(next);
            visited[hs] = make_pair(state.hash(), action);
        }
        iteration++;
    }

    return visited;
}

auto dfs(const State &initial, const State &target)
{
    best_match = {initial, 0, 0, initial.compare(target)};

    stack<State> st;
    st.emplace(initial);

    map<hash_t, pair<hash_t, Action>> visited;
    visited[initial.hash()];

    auto iteration = 0;
    while (not st.empty() and best_match.distance)
    {
        auto state = st.top();
        st.pop();
        for (auto action : state.possible_actions())
        {
            State next(state, action);
            auto hs = next.hash();
            if (visited.count(hs))
                continue;

            auto dist = next.compare(target);
            if (dist < best_match.distance)
                best_match = {next, 0, iteration, dist};

            st.emplace(next);
            visited[hs] = make_pair(state.hash(), action);
        }
        iteration++;
    }

    return visited;
}

auto manhattan(const pair<int, int> &a, const pair<int, int> &b)
{
    return abs(a.first - b.first) + abs(a.second - b.second);
}

auto distance(const State &current, const State &target)
{
    unordered_map<int, pair<int, int>> target_pos;
    for (auto i = 0; i < N; i++)
        for (auto j = 0; j < M; j++)
            target_pos[target.board[i][j]] = make_pair(i, j);

    auto dist = 0;
    for (auto i = 0; i < N; i++)
        for (auto j = 0; j < M; j++)
            dist += manhattan(make_pair(i, j), target_pos[current.board[i][j]]);
    return dist;
}

auto astar(const State &initial, const State &target)
{
    best_match = {initial, 0, 0, initial.compare(target)};

    // G and F scores
    map<hash_t, pair<int, int>> scores;
    scores[initial.hash()] = make_pair(0, distance(initial, target));

    auto heuristic = [&scores](const State &a, const State &b)
    {
        return a.scores.second > b.scores.second;
        // return scores[a.hash()].second > scores[b.hash()].second;
    };

    // priority_queue<State, vector<State>, decltype(heuristic)> pq(heuristic);
    PriorityQueue<State> pq(heuristic);
    pq.emplace(initial);

    map<hash_t, pair<hash_t, Action>> visited;
    visited[initial.hash()];

    auto iteration = 0;
    while (not pq.empty() and best_match.distance)
    {
        auto state = pq.top();
        pq.pop();

        for (auto action : state.possible_actions())
        {
            State next(state, action);

            auto next_g_score = scores[state.hash()].first + 1;
            auto &next_scores = scores[next.hash()];

            if (visited.count(next.hash()) and next_g_score >= next_scores.first)
                continue;

            next_scores.first = next_g_score;
            next_scores.second = next_g_score + distance(next, target);

            auto dist = next.compare(target);
            if (dist < best_match.distance)
                best_match = {next, 0, iteration, dist};

            visited[next.hash()] = make_pair(state.hash(), action);

            next.scores = next_scores;
            pq.push(next);
        }
        iteration++;
    }

    return visited;
}

ostream &operator<<(ostream &os, const Action &action)
{
#define ACTION_CASE(x) case(x): os << #x; break;
    switch(action)
    {
        ACTION_CASE(UP);
        ACTION_CASE(DOWN);
        ACTION_CASE(LEFT);
        ACTION_CASE(RIGHT);
        default: os << "INITIAL STATE";
    }
#undef ACTION_CASE

    return os;
}

void debug_best_match()
{
    cerr << "==== Best match ====\n";
    cerr << "Distance: " << best_match.distance << '\n';
    cerr << "Iteration: " << best_match.iteration << '\n';
    cerr << "Depth: " << best_match.depth << '\n';
    cerr << "State:\n";
    for (const auto &row : best_match.state.board)
    {
        for (const auto &x : row) cerr << x << ' ';
        cerr << '\n';
    }
}

void debug_path(State initial, const vector<Action> &actions_path)
{
    cerr << "\n==== Path for best match ====\n";
    cerr << "Path length: " << actions_path.size() << '\n';
    for (auto action : actions_path)
    {
        cerr << "On Action: " << action << '\n';
        initial.resolve_action(action);
        for (const auto &row : initial.board)
        {
            for (const auto &x : row) cerr << x << ' ';
            cerr << '\n';
        }
        cerr << '\n';
    }
}

void solve(board_t &board, const board_t &target)
{
    auto initial = State(board);
    auto path = reconstruct_path(ALGORITHM(initial, State(target)));
    for (auto i = 1u; i < path.size(); i++)
    {
        initial.resolve_action(path[i]);
        cout << path[i] << '\n';
    }

#ifdef DEBUG
    debug_best_match();
    debug_path(initial, path);
#endif
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> N >> M;

    board_t board(N, vector<int>(M)), target(N, vector<int>(M));
    for (auto i = 0; i < N; i++)
        for (auto j = 0; j < M; j++)
            target[i][j] = i * M + j;

    for (auto i = 0; i < N; i++)
        for (auto j = 0; j < M; j++)
            cin >> board[i][j];

    solve(board, move(target));

    return 0;
}
