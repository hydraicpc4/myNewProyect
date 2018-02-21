#include<vector>
#include<cstdio>
#include<iostream>
#include<cassert>
#include<string>
#include<cctype>
#include<algorithm>
#define REP(i,n) for(int i=0;i<n;i++)
#define all(v) v.begin(), v.end()
#define SZ(v) ((int)v.size())

typedef std::string string;
typedef std::vector<string> vStr;

const int TEAM_BLACK=0;
const int TEAM_WHITE=1;
const int TEAM_NOTEAM=2;
const int THRESHOLD_SHORT=1;
const int THRESHOLD_MEDIUM=2;
const int THRESHOLD_LARGE=8;
const int CHESSBOARD_SIZE=8;
const int ACTIONTYPE_MOVE=0;
const int ACTIONTYPE_CAPTURE=1;
const int ACTIONTYPE_MOVE_AND_CAPTURE=2;

//black minus zero, white mayus one

bool checkCoordinate(int x){
	return x>=0&&x<CHESSBOARD_SIZE;
}

bool checkCoordinates(int x, int y){
	return checkCoordinate(x)&&checkCoordinate(y);
}

struct Direction{
	int x;
	int y;
	int actionType;
	int threshold;//movement threshold of a piece.
	Direction(){}
	Direction(int x, int y, int actionType, int threshold):x(x), y(y), actionType(actionType), threshold(threshold){}
};
typedef std::vector<Direction> Directions;

Directions findDirectionsInsideBoxWithDistanceBetween(int boxSize, int lowestDist, int highestDist, int threshold){
	Directions directions;
	for(int dx=-boxSize;dx<=boxSize;dx++){
		for(int dy=-boxSize;dy<=boxSize;dy++){
			int squareEuclideanDistance=dx*dx+dy*dy;
			if(squareEuclideanDistance>=lowestDist&&squareEuclideanDistance<=highestDist){
				directions.push_back(Direction(dx, dy, ACTIONTYPE_MOVE_AND_CAPTURE, threshold));
			}
		}
	}
	return directions;
}

void printChessboard(vStr chessboard){
	REP(i,SZ(chessboard))
		puts(chessboard[i].c_str());
}
string formatCoordinates(int x, int y){
	return string(1, 'a'+y)+string(1, '0'+8-x);
}

struct Piece{
	int x;
	int y;
	int team;
	char symbol;
	
	Piece(int x, int y, int team, char symbol):x(x),y(y),team(team),symbol(symbol){}
	
	virtual Directions getDirections()=0;
	
	bool isBlackTeam(){
		return team==TEAM_BLACK;
	}
	
	bool isWhiteTeam(){
		return team==TEAM_WHITE;
	}
	
	bool isEmptyCell(){
		return symbol=='.';
	}
	
	bool isKing(){
		return tolower(symbol)=='k';
	}
	
	bool isBishop(){
		return tolower(symbol)=='b';
	}
	
	bool isPawn(){
		return tolower(symbol)=='p';
	}
	
	bool isKnight(){
		return tolower(symbol)=='n';
	}
	
	bool isRook(){
		return tolower(symbol)=='r';
	}
};

bool areFromSameTeam(Piece *pieceA, Piece *pieceB){
	if(pieceA->isBlackTeam()&&pieceB->isBlackTeam()){
		return true;
	}
	if(pieceA->isWhiteTeam()&&pieceB->isWhiteTeam()){
		return true;
	}
	return false;
}

bool areFromDifferentTeam(Piece *pieceA, Piece *pieceB){
	if(pieceA->isBlackTeam()&&pieceB->isWhiteTeam()){
		return true;
	}
	if(pieceA->isWhiteTeam()&&pieceB->isBlackTeam()){
		return true;
	}
	return false;
}

bool itBelongsToThisTeam(Piece *piece, int team){
	if(team==TEAM_BLACK && piece->team==TEAM_BLACK){
		return true;
	}
	if(team==TEAM_WHITE && piece->team==TEAM_WHITE){
		return true;
	}
	return false;
}

bool itBelongsToTheirEnemyTeam(Piece *piece, int team){
	if(team==TEAM_BLACK && piece->team==TEAM_WHITE){
		return true;
	}
	if(team==TEAM_WHITE && piece->team==TEAM_BLACK){
		return true;
	}
	return false;
}

int getEnemyTeam(int team){
	if(team==TEAM_WHITE)return TEAM_BLACK;
	if(team==TEAM_BLACK)return TEAM_WHITE;
}

struct EmptyCell:Piece{
	EmptyCell(int x, int y):Piece(x, y, TEAM_NOTEAM, '.'){}
	Directions getDirections(){
		return Directions();
	}
};

struct Pawn:Piece{
	Pawn(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		Directions directions;
		if(isBlackTeam()){
			directions.push_back(Direction(+1, -1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
			directions.push_back(Direction(+1, 0, ACTIONTYPE_MOVE, (x==1?THRESHOLD_MEDIUM:THRESHOLD_SHORT)));
			directions.push_back(Direction(+1, +1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
		}else{
			directions.push_back(Direction(-1, -1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
			directions.push_back(Direction(-1, 0, ACTIONTYPE_MOVE, (x==6?THRESHOLD_MEDIUM:THRESHOLD_SHORT)));
			directions.push_back(Direction(-1, +1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
		}
		return directions;
	}
};

struct Rook:Piece{
	Rook(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		return findDirectionsInsideBoxWithDistanceBetween(1, 1, 1, THRESHOLD_LARGE);
	}
};

struct Knight:Piece{
	Knight(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		return findDirectionsInsideBoxWithDistanceBetween(2, 5, 5, THRESHOLD_SHORT);
	}
};

struct Bishop:Piece{
	Bishop(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		return findDirectionsInsideBoxWithDistanceBetween(1, 2, 2, THRESHOLD_LARGE);
	}
};

struct Queen:Piece{
	Queen(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		return findDirectionsInsideBoxWithDistanceBetween(1, 1, 2, THRESHOLD_LARGE);
	}
};

struct King:Piece{
	King(int x, int y, int team, char symbol):Piece(x, y, team, symbol){}
	Directions getDirections(){
		return findDirectionsInsideBoxWithDistanceBetween(1, 1, 2, THRESHOLD_SHORT);
	}
};

void printDirections(Directions directions){
	REP(i,SZ(directions))
		printf("%d %d\n", directions[i].x, directions[i].y);
}

char getTeamFromSymbol(char symbol){
	return islower(symbol)?TEAM_BLACK:TEAM_WHITE;
}

void setPiece(Piece* &piece, int x, int y, char c){
	if(tolower(c)=='p'){
		piece=new Pawn(x, y, getTeamFromSymbol(c), c);
	}else if(tolower(c)=='r'){
		piece=new Rook(x, y, getTeamFromSymbol(c), c);
	}else if(tolower(c)=='n'){
		piece=new Knight(x, y, getTeamFromSymbol(c), c);
	}else if(tolower(c)=='b'){
		piece=new Bishop(x, y, getTeamFromSymbol(c), c);
	}else if(tolower(c)=='q'){
		piece=new Queen(x, y, getTeamFromSymbol(c), c);
	}else if(tolower(c)=='k'){
		piece=new King(x, y, getTeamFromSymbol(c), c);
	}else{
		piece=new EmptyCell(x, y);
	}
}

void initChessboard(string FENstring, Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE]){
	REP(i,CHESSBOARD_SIZE){
		REP(j,CHESSBOARD_SIZE){
			chessboard[i][j]=new EmptyCell(i , j);
		}
	}
	int x=0,y=0;
	REP(i,SZ(FENstring)){
		char c=FENstring[i];
		if(c=='/'){
			x++;
			y=0;
		}else if(isdigit(c)){
			y+=c-'0';
		}else{
			setPiece(chessboard[x][y], x, y, c);
			y++;
		}
	}
}

void copyChessBoard(Piece *from[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *to[CHESSBOARD_SIZE][CHESSBOARD_SIZE]){
	REP(x,CHESSBOARD_SIZE){
		REP(y,CHESSBOARD_SIZE){
			char c=from[x][y]->symbol;
			setPiece(to[x][y], x, y, c);
		}
	}
}

void printChessboard(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE]){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			putchar(chessboard[i][j]->symbol);
		}
		puts("");
	}
	puts("");
}

struct Movement{
	int x;
	int y;
	int symbol;
	int nx;
	int ny;
	int nSymbol;
	Movement(int x, int y, int symbol, int nx, int ny, int nSymbol):x(x), y(y), symbol(symbol), nx(nx), ny(ny), nSymbol(nSymbol) {}
};

typedef std::vector<Movement> Movements;
bool compareMovementsByNewCoordinates(Movement movA, Movement movB){
	if(movA.nx!=movB.nx)return movA.nx<movB.nx;
	return movA.ny<movB.ny;
}

string formatMovement(Movement movement){
	int x=movement.x;
	int y=movement.y;
	int symbol=movement.symbol;
	int nx=movement.nx;
	int ny=movement.ny;
	int nSymbol=movement.nSymbol;
	//mate involving a pawn
	if(tolower(symbol)=='p'){
		return formatCoordinates(x, y)+"-"+formatCoordinates(nx, ny);
	}
	//movement involves capture
	if(nSymbol!='.'){
		return string(1, symbol)+formatCoordinates(x, y)+"-"+string(1, symbol)+"x"+formatCoordinates(nx, ny);
	}
	//movement dont involve capture
	return string(1, symbol)+formatCoordinates(x, y)+"-"+string(1, symbol)+formatCoordinates(nx, ny);
}

string formatMovementSimplified(Movement movement){
	int x=movement.x;
	int y=movement.y;
	int nx=movement.nx;
	int ny=movement.ny;
	return formatCoordinates(x, y)+formatCoordinates(nx, ny);
}

const int FILTER_ALL=0;
const int FILTER_MOVE_ON_TO_EMPTY_CELL=1;
const int FILTER_CAPTURE_ENEMY_PIECE=2;

Movements getAllMovementsFromPieceWithFilter(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, int filter){
	Movements movements;
	Directions directions=movingPiece->getDirections();
	int x=movingPiece->x;
	int y=movingPiece->y;
	int symbol=movingPiece->symbol;
	
	REP(i, SZ(directions)){
		bool isBlocked=false;
		int actionType=directions[i].actionType;
		int threshold=directions[i].threshold;
		for(int length=1;length<=threshold&&!isBlocked;length++){
			int nx=x+directions[i].x*length;
			int ny=y+directions[i].y*length;
			if(checkCoordinates(nx, ny)){
				if(!chessboard[nx][ny]->isEmptyCell()){
					isBlocked=true;
				}
				int nSymbol=chessboard[nx][ny]->symbol;
				//if is empty THEN move(can_move,can_move_and_capture)
				//if is not empty THEN cell is friend or foe.
					//if is friend can't move nor capture(do nothing)
					//if is foe THEN capture(can_capture,can_move_and_capture)
				if(chessboard[nx][ny]->isEmptyCell()){
					if(actionType==ACTIONTYPE_MOVE||actionType==ACTIONTYPE_MOVE_AND_CAPTURE){
						//CAN MOVE TO A EMPTY CELL						
						if(filter==FILTER_ALL||filter==FILTER_MOVE_ON_TO_EMPTY_CELL){
							movements.push_back(Movement(x, y, symbol, nx, ny, nSymbol));
						}
					}
				}else{
					if(areFromSameTeam(movingPiece, chessboard[nx][ny])){
						//FRIEND FIRE.
					}else{
						//FOE FIRE
						if(actionType==ACTIONTYPE_CAPTURE||actionType==ACTIONTYPE_MOVE_AND_CAPTURE){
							//CAN CAPTURE A ENEMY PIECE						
							if(filter==FILTER_ALL||filter==FILTER_CAPTURE_ENEMY_PIECE){
								movements.push_back(Movement(x, y, symbol, nx, ny, nSymbol));
							}
						}
					}
				}
			}
		}
	}
	sort(all(movements), compareMovementsByNewCoordinates);
	return movements;
}

/*
	bool canCaptureEnemyKing(Piece *chessboard[][], Piece *attackingPiece):Returns true if any of possible movements from attacking piece 
		leads to capture enemy king.
		PSEUDOCODE: loop over all movements if the piece target is the enemy king then return true, otherwise return false.
		for(all valid moves from piece that can capture)
			if(target position is enemy king position) return true;
		return false;
*/
bool canCaptureEnemyKing(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *attackingPiece){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, attackingPiece, FILTER_ALL);
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *pointedPiece=chessboard[nx][ny];
		if(pointedPiece->isKing() && areFromDifferentTeam(attackingPiece, pointedPiece)){
			return true;
		}
	}
	return false;
}

/*
	bool isTeamInCheck(Piece *chessboard[][], int team):Given curent chessboard and team whose is their turn to play, return 
		true if the given team is in check.Be in check means that the king is already threatened, it means there is at least 
		one enemy piece that can capture king in next enemy turn.
		PSEUDOCODE: loop over all pieces from enemy team of 'team' and then try to move them(possibly many movements per piece)
			and check if it is possible capture king with this movements.
			REP(i, CHESSBOARD_SIZE)
				REP(j, CHESSBOARD_SIZE)
					if(chessboard[i][j] is on enemy(team) ){
						if(canAttackEnemyKing(chessboard, chessboard[i][j])){
							return true;
						}
					}
			return false;
*/
bool isTeamInCheck(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleEnemyPiece=chessboard[i][j];
			if(itBelongsToTheirEnemyTeam(possibleEnemyPiece, team)){
				if(canCaptureEnemyKing(chessboard, possibleEnemyPiece)){
					return true;
				}
			}
		}
	}
	return false;
}

/*
	bool canRevertCheckFromPiece(Piece *chessboard[][], Piece *revertingPiece) Assuming being checked try to move a piece 
		in order to not continuing being checked, return true if there exist at least a movement that makes the king not threatened.
			for(all valid moves from piece){
				let nextChessboard the chessboard after the movement.
				if(!isTeamInCheck(nextChessboard, revertingPiece->team )){
					return true;
				}
			}
		return false;
*/
bool canRevertCheckFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *revertingPiece){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, revertingPiece, FILTER_ALL);
	int x=revertingPiece->x;
	int y=revertingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, revertingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, revertingPiece->team )){
			return true;
		}
	}
	return false;	
}

/*
	bool canRevertCheck(Piece *chessboard[][], int team) Given a team that it's already in check return true if there exists
		at least a movement that makes the king not threatened.
		PSEUDOCODE: loop over all pieces and then try to move them to valid positions if some of next positions are not longer 
		in check return true, otherwise return false.
			REP(i, CHESSBOARD_SIZE)
				REP(j, CHESSBOARD_SIZE)
					if(chessboard[i][j] is on team){
						if(canRevertCheckFromPiece(chessboard, chessboard[i][j])){
							return true;
						}
					}
		return false;
*/
bool canRevertCheck(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team){
	REP(i, CHESSBOARD_SIZE){
			REP(j, CHESSBOARD_SIZE){
				Piece *possibleFriendPiece=chessboard[i][j];
				if(itBelongsToThisTeam(possibleFriendPiece, team)){
					if(canRevertCheckFromPiece(chessboard, possibleFriendPiece)){
						return true;
					}
				}
			}
	}
	return false;
}

/*
	void collectMatingMovesFromPiece(Piece *chessboard[][], Piece *matingPiece, vString &items):move a piece from 
		original position to a next valid position and then check if this movement is a mating move, if it is then store 
		the movement in items.To be a mating move the performed movements should'nt lead a direct enemy attack to the king of 
		team 'team' (mantaing king security principle) or in another words, that curent movements shouldn't lead to a check 
		in team.To be a mating move all possible next moves of enemy team must not revert performed checking.
		PSEUDOCODE: loop over all movements if the piece target is the enemy king then return true, otherwise return false.
			let curentTeam=matingPiece->team;
			let enemyTeam=enemy(curentTeam);
			for(all valid moves from piece){
				let nextChessboard the chessboard after the movement.
				if(!isTeamInCheck(nextChessboard, curentTeam)){
					if(isTeamInCheck(nextChessboard, enemyTeam)){
						if(!canRevertCheck(nextChessboard, enemyTeam)){
							store in items the movement
						}
					}
				}
			}
*/
void collectMatingMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *matingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, matingPiece, FILTER_ALL);
	int curentTeam=matingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=matingPiece->x;
	int y=matingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, matingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			if(isTeamInCheck(nextChessboard, enemyTeam)){
				if(!canRevertCheck(nextChessboard, enemyTeam)){
					items.push_back(formatMovement(movements[i]));
				}
			}
		}
	}
}

/*	
	void collectMatingMoves(Piece *chessboard[][], int team, vString &items):collect all checking moves from a specific team
		and store them in items.
		PSEUDOCODE: loop over all pieces from team 'team' and then try to move them(possibly many movements per piece).
			REP(i, CHESSBOARD_SIZE)
				REP(j, CHESSBOARD_SIZE)
					if(chessboard[i][j] is on team){
						collectMatingMovesFromPiece(chessboard, chessboard[i][j], &items)
					}
	
*/
void collectMatingMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team)){
				collectMatingMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}
///////
void collectCheckingMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *matingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, matingPiece, FILTER_ALL);
	int curentTeam=matingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=matingPiece->x;
	int y=matingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, matingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			if(isTeamInCheck(nextChessboard, enemyTeam)){
				items.push_back(formatMovement(movements[i]));
			}
		}
	}
}

void collectCheckingMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team)){
				collectCheckingMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

//collectPseudoLegalMovesFromPiece
void collectPseudoLegalMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, movingPiece, FILTER_ALL);
	int curentTeam=movingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=movingPiece->x;
	int y=movingPiece->y;
	REP(i, SZ(movements)){
		items.push_back(formatMovementSimplified(movements[i]));
	}
}

void collectPseudoLegalMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team)){
				collectPseudoLegalMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

//bishopMoves
void collectBishopMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, movingPiece, FILTER_ALL);
	int curentTeam=movingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=movingPiece->x;
	int y=movingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, movingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			items.push_back(formatMovementSimplified(movements[i]));
		}
	}
}

void collectBishopMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team) && possibleFriendPiece->isBishop()){
				collectBishopMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

//pawnMoves
void collectPawnMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, movingPiece, FILTER_ALL);
	int curentTeam=movingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=movingPiece->x;
	int y=movingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, movingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			items.push_back(formatMovementSimplified(movements[i]));
		}
	}
}

void collectPawnMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team) && possibleFriendPiece->isPawn()){
				collectPawnMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

//knightMoves
void collectKnightMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, movingPiece, FILTER_ALL);
	int curentTeam=movingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=movingPiece->x;
	int y=movingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, movingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			items.push_back(formatMovementSimplified(movements[i]));
		}
	}
}

void collectKnightMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team) && possibleFriendPiece->isKnight()){
				collectKnightMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

//rookMoves
void collectRookMovesFromPiece(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece, vStr &items){
	Movements movements=getAllMovementsFromPieceWithFilter(chessboard, movingPiece, FILTER_ALL);
	int curentTeam=movingPiece->team;
	int enemyTeam=getEnemyTeam(curentTeam);
	int x=movingPiece->x;
	int y=movingPiece->y;
	REP(i, SZ(movements)){
		int nx=movements[i].nx;
		int ny=movements[i].ny;
		Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		copyChessBoard(chessboard, nextChessboard);
		setPiece(nextChessboard[nx][ny], nx, ny, movingPiece->symbol);
		nextChessboard[x][y]= new EmptyCell(x, y);
		
		if(!isTeamInCheck(nextChessboard, curentTeam)){
			items.push_back(formatMovementSimplified(movements[i]));
		}
	}
}

void collectRookMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team, vStr &items){
	REP(i, CHESSBOARD_SIZE){
		REP(j, CHESSBOARD_SIZE){
			Piece *possibleFriendPiece=chessboard[i][j];
			if(itBelongsToThisTeam(possibleFriendPiece, team) && possibleFriendPiece->isRook()){
				collectRookMovesFromPiece(chessboard, possibleFriendPiece, items);
			}
		}
	}
}

int main(){
	string FENstring;
	while(std::cin>>FENstring){
		if(FENstring=="-1")break;
		Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
		initChessboard(FENstring, chessboard);
		vStr items;
		collectMatingMoves(chessboard, items);
		
		sort(all(items));
		printf("Mating moves [");
		REP(i,SZ(items))
			if(!i)printf("%s",items[i].c_str());
			else printf(", %s",items[i].c_str());
		puts("]");
	}
}


