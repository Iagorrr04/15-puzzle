/*
    Script to generate a valid input to be used as the initial state of the board.
*/
#include "../src/board_class.h"
#include <dirent.h>
board input_board;
const int MAX_MOVES = 1e8;
const int MIN_MOVES = 3;

// compare the file name with the dimentions of the current board.
bool compare_name(char* file_name, string prefix_input){
    // counts the string size and get the '_' position.
    int size = 0;
    int d_pos = 0;
    string aux;
    while(file_name[size] != '\0'){
        aux.push_back(file_name[size]);
        if(file_name[size] == '_') d_pos = size;
        ++size;
    }

    // valid as a txt.
    if(size < 9) return false;
    if(file_name[size-1] != 't') return false;
    if(file_name[size-2] != 'x') return false;
    if(file_name[size-3] != 't') return false;
    
    // check if is the same dimention as the generated.
    if(aux == prefix_input) return true;

    return false;
}

// save the input with the according name.
int save_file(board input_board){
    // Generating the file name.
    string file_name;
    file_name = file_name + (to_string(input_board.N)) + 'x' + to_string(input_board.M) + '_';

    // Iterate through generated inputs to create the new one.
    struct dirent *entry;
    DIR *input_directory = opendir("./");
    int count = 1;
    while((entry = readdir(input_directory))){
        if(compare_name(entry->d_name, file_name)) count++;
    }
    closedir(input_directory);
    file_name += to_string(count) + ".txt";
    cout << "File name: " << file_name << '\n';

    // Write the new file.
    char file_name_char[file_name.size()+1];
    for(int i = 0 ; i < (int)file_name.size(); ++i) file_name_char[i] = file_name[i];
    file_name_char[(int)file_name.size()] = '\0';
    FILE* new_input_file = fopen(file_name_char, "w");
    fprintf(new_input_file, "%d %d\n", input_board.N, input_board.M);
    for(int i = 0; i < input_board.N; ++i){
        fprintf(new_input_file, "%d", input_board.state[i][0]);
        for(int j = 1; j < input_board.M; ++j){
            fprintf(new_input_file, " %d", input_board.state[i][j]);
        }
        fprintf(new_input_file, "\n");
    }
    fclose(new_input_file);
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    // Creating only square inputs.
    int X = rand()%15;
    input_board.initialize_final_state(X, X);
    cout << "Board dimension: " << X << ' ' << X << '\n';

    // going from the final state to a random state moving randomly.
    int moves = rand()%MAX_MOVES + MIN_MOVES;
    cout << "Total moves: " << moves << '\n';
    int m = 0;
    int d = 0, u = 0, l = 0, r = 0;
    while(m < moves){
        switch (rand()%4)
        {
            case 0:
                if(input_board.move_down()){ 
                    d++;
                    m++;
                }
            break;

            case 1:
                if(input_board.move_left()){ 
                    l++;
                    m++;
                }
            break;

            case 2:
                if(input_board.move_right()){ 
                    r++;
                    m++;
                }
            break;

            case 3:
                if(input_board.move_up()){ 
                    u++;
                    m++;
                }
            break;
        }
    }
    cout << "D: " << d << '\n';
    cout << "L: " << l << '\n';
    cout << "R: " << r << '\n';
    cout << "U: " << u << '\n';
    
    save_file(input_board);
    return 0;
}