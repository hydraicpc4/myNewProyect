#include"chessEngine.h"

int main(){
	string FENstring;
	while(std::cin>>FENstring){
		if(FENstring=="-1")break;
		Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		initChessboard(FENstring, chessboard);
		
		collectMateInTwoMoves(chessboard, TEAM_WHITE);
	}
}


