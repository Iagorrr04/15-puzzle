#include <ncurses.h>
#include <vector>
#include <iostream>

#include <unistd.h>

struct Bord {

	std::pair<int, int> zero;
	int n, m;
	std::vector<std::vector<int>> bord ;

	Bord(){}
	Bord(std::vector<std::vector<int>> bord, int n, int m, std::pair<int, int> zero) : bord(bord), n(n), m(m), zero(zero){ }

	static Bord readAndGetBord(int n, int m){
		std::vector<std::vector<int>> bord(n, std::vector<int>(m));

		std::pair<int, int> zero;

			for (int i = 0; i < n; i++)
				for ( int j  =0;j < m; j++ ){
						 std::cin >> bord[i][j];
						 if ( bord[i][j] == 0 )
							 zero = { i, j };


				}

			return Bord(bord, n, m, zero);
	}


	void printBorad(WINDOW *w){
		wmove(w, 0, 0);
			for (int i = 0; i < n; i++){
					for ( int j  =0;j < m; j++ )
							printw("%d ", bord[i][j]);
					printw("\n");
			}
	}

	void move ( char m){

		std::pair<int, int> delta = {0, 0};
		auto &[x, y] = delta;

		switch ( m ){
			case 'U':
				x--;
				break;
			case 'D':
				x++;
				break;
			case 'L':
				y--;
				break;
			case 'R':
				y++;
				break;
		}

		auto [xi, yi] = zero;

		std:: swap(bord[xi][yi], bord[xi + x ][yi + y]);

		zero = {xi + x, yi + y};

	}


};



int main(){
    int n, m;

    std::cin>>n>>m;

		Bord bord = Bord::readAndGetBord(n, m);
	
		std::string moves;
		std::cin >> moves;

    WINDOW *w = initscr();

		bord.printBorad(w);
		refresh();
		sleep(5);

		for ( auto m : moves ){

			bord.move(m);
			bord.printBorad(w);
			sleep(1); 
			refresh();
		}

		sleep(5); 
		endwin();
    return 0;
}

