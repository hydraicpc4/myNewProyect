#include"chessEngine.h"

int main(){
	string FENstring, turn, x, y, z, w;
	while(std::cin>>FENstring>>turn>>x>>y>>z>>w){
		if(FENstring=="-1")break;
		Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		initChessboard(FENstring, chessboard);
		int team=(turn=="w"?TEAM_WHITE:TEAM_BLACK);
		Movements pseudoLegalMovements=collectPseudoLegalMoves(chessboard, team);
		
		printf("%d\n", SZ(pseudoLegalMovements));
	}
}

