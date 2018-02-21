#include"chessEngine.h"

string formatCapture(Movement movement){
	return string(1, tolower(movement.symbol)=='p'?('a'+movement.y):toupper(movement.symbol))+"x"+formatCoordinates(movement.nx,movement.ny);
}

string toString(int number){
	std::ostringstream out;
	out<<number;
	return out.str();
}

void printCapture(vStr items, string label){
	printf("Captures By %s : [", label.c_str());
	REP(i,SZ(items))
		if(!i)printf("%s",items[i].c_str());
		else printf(", %s",items[i].c_str());
	puts("]");

}

int main(){
	vvStr testCases(1);
	string str;
	while(std::cin>>str){
		if(str=="-1"){
			testCases.push_back(vStr());
		}else{
			testCases.back().push_back(str);
		}
	}
	testCases.pop_back();

	REP(tc, SZ(testCases)){
		int team=TEAM_WHITE;
		int blackPos=1;
		int whitePos=1;
		vStr whiteCaptures;
		vStr blackCaptures;
		REP(k, SZ(testCases[tc])-1){
			string FENstring=testCases[tc][k+1];
			string lastFENstring=testCases[tc][k];
			Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
			initChessboard(lastFENstring, chessboard);

			Movements movements=collectLegalMoves(chessboard, team);
			REP(i, SZ(movements)){
				Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
				copyChessBoard(chessboard, nextChessboard);
				updateMovementOnTable(nextChessboard, movements[i]);
				if(getFENstringFromChessboard(nextChessboard)==FENstring){
					if(movements[i].symbol=='.'||movements[i].nSymbol=='.'){
					}else{
						if(team==TEAM_BLACK){
							blackCaptures.push_back(toString(blackPos)+") .. "+formatCapture(movements[i]));
						}else{
							whiteCaptures.push_back(toString(blackPos)+") "+formatCapture(movements[i]));
						}
					}
					if(team==TEAM_BLACK){
						blackPos++;
					}else{
						whitePos++;
					}
				}
			}
			team^=1;
		}
		printf("Number of Captures in the game : %d\n", SZ(blackCaptures)+SZ(whiteCaptures));
		printCapture(whiteCaptures, "White");
		printCapture(blackCaptures, "Black");
	}
}

