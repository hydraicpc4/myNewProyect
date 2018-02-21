#include "chessEngine.h"

int main(){
	string FENstring, turn;
	while(std::cin>>FENstring>>turn){
		Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		initChessboard(FENstring, chessboard);

		int team=(turn=="w"?TEAM_WHITE:TEAM_BLACK);
		Movements collectedMoves=collectPieceMovesWithGivenSymbol(chessboard, team, 'r');
		vStr items=getPrettyPrintedStringsFromMovements(collectedMoves);

		printf("[");
		REP(i,SZ(items))
			if(!i)printf("%s",items[i].c_str());
			else printf(", %s",items[i].c_str());
		puts("]");
	}
}
