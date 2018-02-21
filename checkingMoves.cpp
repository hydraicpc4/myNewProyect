#include"chessEngine.h"

int main(){
	string FENstring;
	while(std::cin>>FENstring){
		if(FENstring=="-1")break;
		Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		initChessboard(FENstring, chessboard);
		
		Movements checkingMoves=collectCheckingMoves(chessboard, TEAM_WHITE);
		vStr items=getPrettyPrintedStringsFromMovementsDetailed(checkingMoves);

		sort(all(items));
		printf("Checking moves [");
		REP(i,SZ(items))
			if(!i)printf("%s",items[i].c_str());
			else printf(", %s",items[i].c_str());
		puts("]");
	}
}

