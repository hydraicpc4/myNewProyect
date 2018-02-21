/*
 * chessEngine.h
 *
 *  Created on: 31/10/2017
 *      Author: asus-pc
 */

#ifndef SRC_CHESSENGINE_H_
#define SRC_CHESSENGINE_H_

#include<vector>
#include<cstdio>
#include<iostream>
#include<cassert>
#include<string>
#include<cctype>
#include<algorithm>
#include<sstream>
#define REP(i,n) for(int i=0;i<n;i++)
#define all(v) v.begin(), v.end()
#define SZ(v) ((int)v.size())

typedef std::string string;
typedef std::vector<string> vStr;
typedef std::vector<vStr> vvStr;

const int TEAM_BLACK = 0;
const int TEAM_WHITE = 1;
const int TEAM_NOTEAM = 2;
const int THRESHOLD_SHORT = 1;
const int THRESHOLD_MEDIUM = 2;
const int THRESHOLD_LARGE = 8;
const int CHESSBOARD_SIZE = 8;
const int ACTIONTYPE_MOVE = 0;
const int ACTIONTYPE_CAPTURE = 1;
const int ACTIONTYPE_MOVE_AND_CAPTURE = 2;

//black minus zero, white mayus one

bool checkCoordinate(int x) {
  return x >= 0 && x < CHESSBOARD_SIZE;
}

bool checkCoordinates(int x, int y) {
  return checkCoordinate(x) && checkCoordinate(y);
}

struct Direction {
  int x;
  int y;
  int actionType;
  int threshold;  //movement threshold of a piece.
  Direction() {
  }
  Direction(int x, int y, int actionType, int threshold)
      : x(x),
        y(y),
        actionType(actionType),
        threshold(threshold) {
  }
};
typedef std::vector<Direction> Directions;

Directions findDirectionsInsideBoxWithDistanceBetween(int boxSize,
                                                      int lowestDist,
                                                      int highestDist,
                                                      int threshold) {
  Directions directions;
  for (int dx = -boxSize; dx <= boxSize; dx++) {
    for (int dy = -boxSize; dy <= boxSize; dy++) {
      int squareEuclideanDistance = dx * dx + dy * dy;
      if (squareEuclideanDistance >= lowestDist
          && squareEuclideanDistance <= highestDist) {
        directions.push_back(
            Direction(dx, dy, ACTIONTYPE_MOVE_AND_CAPTURE, threshold));
      }
    }
  }
  return directions;
}

void printChessboard(vStr chessboard) {
  REP(i,SZ(chessboard)){
    puts(chessboard[i].c_str());
  }
}

string formatCoordinates(int x, int y) {
  return string(1, 'a' + y) + string(1, '0' + 8 - x);
}

struct Piece {
  int x;
  int y;
  int team;
  char symbol;

  Piece(int x, int y, int team, char symbol)
      : x(x),
        y(y),
        team(team),
        symbol(symbol) {
  }

  virtual Directions getDirections()=0;

  bool isBlackTeam() {
    return team == TEAM_BLACK;
  }

  bool isWhiteTeam() {
    return team == TEAM_WHITE;
  }

  bool isEmptyCell() {
    return symbol == '.';
  }

  bool isKing() {
    return tolower(symbol) == 'k';
  }

  bool isBishop() {
    return tolower(symbol) == 'b';
  }

  bool isPawn() {
    return tolower(symbol) == 'p';
  }

  bool isKnight() {
    return tolower(symbol) == 'n';
  }

  bool isRook() {
    return tolower(symbol) == 'r';
  }

  bool hasThisSymbol(char thatSymbol) {
    return tolower(symbol) == tolower(thatSymbol);
  }
};

bool areFromSameTeam(Piece *pieceA, Piece *pieceB) {
  if (pieceA->isBlackTeam() && pieceB->isBlackTeam()) {
    return true;
  }
  if (pieceA->isWhiteTeam() && pieceB->isWhiteTeam()) {
    return true;
  }
  return false;
}

bool areFromDifferentTeam(Piece *pieceA, Piece *pieceB) {
  if (pieceA->isBlackTeam() && pieceB->isWhiteTeam()) {
    return true;
  }
  if (pieceA->isWhiteTeam() && pieceB->isBlackTeam()) {
    return true;
  }
  return false;
}

bool itBelongsToThisTeam(Piece *piece, int team) {
  if (team == TEAM_BLACK && piece->team == TEAM_BLACK) {
    return true;
  }
  if (team == TEAM_WHITE && piece->team == TEAM_WHITE) {
    return true;
  }
  return false;
}

bool itBelongsToTheirEnemyTeam(Piece *piece, int team) {
  if (team == TEAM_BLACK && piece->team == TEAM_WHITE) {
    return true;
  }
  if (team == TEAM_WHITE && piece->team == TEAM_BLACK) {
    return true;
  }
  return false;
}

int getEnemyTeam(int team) {
  if (team == TEAM_WHITE)
    return TEAM_BLACK;
  if (team == TEAM_BLACK)
    return TEAM_WHITE;
}

struct EmptyCell : Piece {
  EmptyCell(int x, int y)
      : Piece(x, y, TEAM_NOTEAM, '.') {
  }
  Directions getDirections() {
    return Directions();
  }
};

struct Pawn : Piece {
  Pawn(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    Directions directions;
    if (isBlackTeam()) {
      directions.push_back(
          Direction(+1, -1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
      directions.push_back(
          Direction(+1, 0, ACTIONTYPE_MOVE,
                    (x == 1 ? THRESHOLD_MEDIUM : THRESHOLD_SHORT)));
      directions.push_back(
          Direction(+1, +1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
    } else {
      directions.push_back(
          Direction(-1, -1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
      directions.push_back(
          Direction(-1, 0, ACTIONTYPE_MOVE,
                    (x == 6 ? THRESHOLD_MEDIUM : THRESHOLD_SHORT)));
      directions.push_back(
          Direction(-1, +1, ACTIONTYPE_CAPTURE, THRESHOLD_SHORT));
    }
    return directions;
  }
};

struct Rook : Piece {
  Rook(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    return findDirectionsInsideBoxWithDistanceBetween(1, 1, 1, THRESHOLD_LARGE);
  }
};

struct Knight : Piece {
  Knight(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    return findDirectionsInsideBoxWithDistanceBetween(2, 5, 5, THRESHOLD_SHORT);
  }
};

struct Bishop : Piece {
  Bishop(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    return findDirectionsInsideBoxWithDistanceBetween(1, 2, 2, THRESHOLD_LARGE);
  }
};

struct Queen : Piece {
  Queen(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    return findDirectionsInsideBoxWithDistanceBetween(1, 1, 2, THRESHOLD_LARGE);
  }
};

struct King : Piece {
  King(int x, int y, int team, char symbol)
      : Piece(x, y, team, symbol) {
  }
  Directions getDirections() {
    return findDirectionsInsideBoxWithDistanceBetween(1, 1, 2, THRESHOLD_SHORT);
  }
};

void printDirections(Directions directions) {
  REP(i,SZ(directions))
    printf("%d %d\n", directions[i].x, directions[i].y);
}

char getTeamFromSymbol(char symbol) {
  return islower(symbol) ? TEAM_BLACK : TEAM_WHITE;
}

void setPiece(Piece* &piece, int x, int y, char c) {
  if (tolower(c) == 'p') {
    piece = new Pawn(x, y, getTeamFromSymbol(c), c);
  } else if (tolower(c) == 'r') {
    piece = new Rook(x, y, getTeamFromSymbol(c), c);
  } else if (tolower(c) == 'n') {
    piece = new Knight(x, y, getTeamFromSymbol(c), c);
  } else if (tolower(c) == 'b') {
    piece = new Bishop(x, y, getTeamFromSymbol(c), c);
  } else if (tolower(c) == 'q') {
    piece = new Queen(x, y, getTeamFromSymbol(c), c);
  } else if (tolower(c) == 'k') {
    piece = new King(x, y, getTeamFromSymbol(c), c);
  } else {
    piece = new EmptyCell(x, y);
  }
}

void initChessboard(string FENstring,
                    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE]) {
  REP(i,CHESSBOARD_SIZE){
    REP(j,CHESSBOARD_SIZE){
      chessboard[i][j] = new EmptyCell(i, j);
    }
  }
  int x = 0, y = 0;
  REP(i,SZ(FENstring)){
    char c = FENstring[i];
    if (c == '/') {
      x++;
      y = 0;
    } else if (isdigit(c)) {
      y += c - '0';
    } else {
      setPiece(chessboard[x][y], x, y, c);
      y++;
    }
  }
}

string getFENstringFromChessboard(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE]) {
  string FENstring;
  int emptyCellsCounter = 0;
  REP(i,CHESSBOARD_SIZE){
    if (i > 0) {
      FENstring += '/';
    }
    REP(j,CHESSBOARD_SIZE){
      if (chessboard[i][j]->isEmptyCell()) {
        emptyCellsCounter++;
      } else {
        if (emptyCellsCounter > 0) {
          FENstring += string(1, '0' + emptyCellsCounter);
        }
        emptyCellsCounter = 0;
        FENstring += string(1, chessboard[i][j]->symbol);
      }
    }
    if (emptyCellsCounter > 0) {
      FENstring += string(1, '0' + emptyCellsCounter);
    }
    emptyCellsCounter = 0;
  }
  return FENstring;
}

void copyChessBoard(Piece *from[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                    Piece *to[CHESSBOARD_SIZE][CHESSBOARD_SIZE]) {
  REP(x,CHESSBOARD_SIZE){
    REP(y,CHESSBOARD_SIZE){
      char c = from[x][y]->symbol;
      setPiece(to[x][y], x, y, c);
    }
  }
}

void printChessboard(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE]) {
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      putchar(chessboard[i][j]->symbol);
    }
    puts("");
  }
  puts("");
}

struct Movement {
  int x;
  int y;
  int symbol;
  int nx;
  int ny;
  int nSymbol;
  Movement(int x, int y, int symbol, int nx, int ny, int nSymbol)
      : x(x),
        y(y),
        symbol(symbol),
        nx(nx),
        ny(ny),
        nSymbol(nSymbol) {
  }
};

typedef std::vector<Movement> Movements;

bool compareMovementsByNewCoordinates(Movement movA, Movement movB) {
  if (movA.nx != movB.nx)
    return movA.nx < movB.nx;
  return movA.ny < movB.ny;
}

void updateMovementOnTable(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                           Movement movement) {
  int x = movement.x;
  int y = movement.y;
  int nx = movement.nx;
  int ny = movement.ny;
  setPiece(chessboard[nx][ny], nx, ny, chessboard[x][y]->symbol);
  chessboard[x][y] = new EmptyCell(x, y);
}

string formatMovement(Movement movement) {
  int x = movement.x;
  int y = movement.y;
  int symbol = movement.symbol;
  int nx = movement.nx;
  int ny = movement.ny;
  int nSymbol = movement.nSymbol;
  //mate involving a pawn
  if (tolower(symbol) == 'p') {
    return formatCoordinates(x, y) + "-" + formatCoordinates(nx, ny);
  }
  //movement involves capture
  if (nSymbol != '.') {
    return string(1, symbol) + formatCoordinates(x, y) + "-" + string(1, symbol)
        + "x" + formatCoordinates(nx, ny);
  }
  //movement don't involve capture
  return string(1, symbol) + formatCoordinates(x, y) + "-" + string(1, symbol)
      + formatCoordinates(nx, ny);
}

string formatMovementSimplified(Movement movement) {
  int x = movement.x;
  int y = movement.y;
  int nx = movement.nx;
  int ny = movement.ny;
  return formatCoordinates(x, y) + formatCoordinates(nx, ny);
}

Movements getAllMovementsFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece) {
  Movements movements;
  Directions directions = movingPiece->getDirections();
  int x = movingPiece->x;
  int y = movingPiece->y;
  int symbol = movingPiece->symbol;

  REP(i, SZ(directions)){
    bool isBlocked = false;
    int actionType = directions[i].actionType;
    int threshold = directions[i].threshold;
    for (int length = 1; length <= threshold && !isBlocked; length++) {
      int nx = x + directions[i].x * length;
      int ny = y + directions[i].y * length;
      if (checkCoordinates(nx, ny)) {
        if (!chessboard[nx][ny]->isEmptyCell()) {
          isBlocked = true;
        }
        int nSymbol = chessboard[nx][ny]->symbol;
        //if is empty THEN move(can_move,can_move_and_capture)
        //if is not empty THEN cell is friend or foe.
        //if is friend can't move nor capture(do nothing)
        //if is foe THEN capture(can_capture,can_move_and_capture)
        if (chessboard[nx][ny]->isEmptyCell()) {
          if (actionType == ACTIONTYPE_MOVE
              || actionType == ACTIONTYPE_MOVE_AND_CAPTURE) {
            //CAN MOVE TO A EMPTY CELL
            movements.push_back(Movement(x, y, symbol, nx, ny, nSymbol));
          }
        } else {
          if (areFromSameTeam(movingPiece, chessboard[nx][ny])) {
            //FRIEND FIRE.
          } else {
            //FOE FIRE
            if (actionType == ACTIONTYPE_CAPTURE
                || actionType == ACTIONTYPE_MOVE_AND_CAPTURE) {
              //CAN CAPTURE A ENEMY PIECE
              movements.push_back(Movement(x, y, symbol, nx, ny, nSymbol));
            }
          }
        }
      } else {
        break;
      }
    }
  }
  sort(all(movements), compareMovementsByNewCoordinates);
  return movements;
}

/*mateInOne*/
// Returns true if any of the possible movements from attacking piece leads to capture the enemy king.
bool canCaptureEnemyKing(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                         Piece *attackingPiece) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           attackingPiece);
  REP(i, SZ(movements)){
    int nx = movements[i].nx;
    int ny = movements[i].ny;
    Piece *pointedPiece = chessboard[nx][ny];
    if (pointedPiece->isKing()
        && areFromDifferentTeam(attackingPiece, pointedPiece)) {
      return true;
    }
  }
  return false;
}

// Returns true if the given team is on check.
bool isTeamInCheck(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                   int team) {
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleEnemyPiece = chessboard[i][j];
      if (itBelongsToTheirEnemyTeam(possibleEnemyPiece, team)) {
        if (canCaptureEnemyKing(chessboard, possibleEnemyPiece)) {
          return true;
        }
      }
    }
  }
  return false;
}

// Returns true if exists from a given piece a movement that makes the king not threatened.
bool canRevertCheckFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
    Piece *revertingPiece) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           revertingPiece);
  REP(i, SZ(movements)){
    Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
    copyChessBoard(chessboard, nextChessboard);
    updateMovementOnTable(nextChessboard, movements[i]);

    if (!isTeamInCheck(nextChessboard, revertingPiece->team)) {
      return true;
    }
  }
  return false;
}

// Returns true if there exists a movement that makes the king not threatened.
bool canRevertCheck(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                    int team) {
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        if (canRevertCheckFromPiece(chessboard, possibleFriendPiece)) {
          return true;
        }
      }
    }
  }
  return false;
}

// Calculates mating moves of a piece, filtering from all legal moves from a piece.
void collectMatingMovesFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *matingPiece,
    Movements &matingMoves) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           matingPiece);
  int curentTeam = matingPiece->team;
  int enemyTeam = getEnemyTeam(curentTeam);
  REP(i, SZ(movements)){
    Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
    copyChessBoard(chessboard, nextChessboard);
    updateMovementOnTable(nextChessboard, movements[i]);

    if (!isTeamInCheck(nextChessboard, curentTeam)) {
      if (isTeamInCheck(nextChessboard, enemyTeam)) {
        if (!canRevertCheck(nextChessboard, enemyTeam)) {
          matingMoves.push_back(movements[i]);
        }
      }
    }
  }
}

// Returns the mating moves from all pieces of a given team.
Movements collectMatingMoves(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team) {
  Movements matingMoves;
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        collectMatingMovesFromPiece(chessboard, possibleFriendPiece,
                                    matingMoves);
      }
    }
  }
  return matingMoves;
}

// Returns true if current team is performing a mate to their enemy team.
bool isPerformingAMate(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                       int team) {
  int curentTeam = team;
  int enemyTeam = getEnemyTeam(curentTeam);
  if (!isTeamInCheck(chessboard, curentTeam)) {
    if (isTeamInCheck(chessboard, enemyTeam)) {
      if (!canRevertCheck(chessboard, enemyTeam)) {
        return true;
      }
    }
  }
  return false;
}

/*end mateInOne*/

/*checkingMoves*/
// Calculates checking moves of a piece, filtering from all legal moves from a piece.
void collectCheckingMovesFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *matingPiece,
    Movements &checkingMoves) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           matingPiece);
  int curentTeam = matingPiece->team;
  int enemyTeam = getEnemyTeam(curentTeam);
  REP(i, SZ(movements)){
    Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
    copyChessBoard(chessboard, nextChessboard);
    updateMovementOnTable(nextChessboard, movements[i]);

    if (!isTeamInCheck(nextChessboard, curentTeam)) {
      if (isTeamInCheck(nextChessboard, enemyTeam)) {
        checkingMoves.push_back(movements[i]);
      }
    }
  }
}

// Returns the checking moves from all pieces of a given team.
Movements collectCheckingMoves(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team) {
  Movements checkingMoves;
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        collectCheckingMovesFromPiece(chessboard, possibleFriendPiece,
                                      checkingMoves);
      }
    }
  }
  return checkingMoves;
}

/*collectPseudoLegalMovesFromPiece*/
// Calculates pseudolegal moves of a piece, filtering from all legal moves from a piece.
void collectPseudoLegalMovesFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece,
    Movements &pseudoLegalMovements) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           movingPiece);
  int curentTeam = movingPiece->team;
  int enemyTeam = getEnemyTeam(curentTeam);
  REP(i, SZ(movements)){
    pseudoLegalMovements.push_back(movements[i]);
  }
}

// Returns the pseudolegal moves from all pieces of a given team.
Movements collectPseudoLegalMoves(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team) {
  Movements pseudoLegalMovements;
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        collectPseudoLegalMovesFromPiece(chessboard, possibleFriendPiece,
                                         pseudoLegalMovements);
      }
    }
  }
  return pseudoLegalMovements;
}
/*end collectPseudoLegalMovesFromPiece*/

/*legalMovements*/
// Calculates legal moves of a piece, filtering from all legal moves from a piece.
void collectLegalMovesFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *movingPiece,
    Movements &legalMovements) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           movingPiece);
  int curentTeam = movingPiece->team;
  int enemyTeam = getEnemyTeam(curentTeam);
  REP(i, SZ(movements)){
    Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
    copyChessBoard(chessboard, nextChessboard);
    updateMovementOnTable(nextChessboard, movements[i]);

    if (!isTeamInCheck(nextChessboard, curentTeam)) {
      legalMovements.push_back(movements[i]);
    }
  }
}

// Returns the legal moves from all pieces of a given team.
Movements collectLegalMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                            int team) {
  Movements movements;
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        collectLegalMovesFromPiece(chessboard, possibleFriendPiece, movements);
      }
    }
  }
  return movements;
}
/*end legalMovements*/

/*collectMateInTwoMove*/
void collectMateInTwoMovesFromPiece(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], Piece *matingPiece) {
  Movements movements = getAllMovementsFromPiece(chessboard,
                                                           matingPiece);
  int curentTeam = matingPiece->team;
  int enemyTeam = getEnemyTeam(curentTeam);
  REP(i, SZ(movements)){
    Piece *nextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
    copyChessBoard(chessboard, nextChessboard);
    updateMovementOnTable(nextChessboard, movements[i]);
    if (!isTeamInCheck(nextChessboard, curentTeam)) {
      Movements enemyMovements = collectLegalMoves(nextChessboard, enemyTeam);
      if (SZ(enemyMovements) == 1) {
        Piece *nextNextChessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
        copyChessBoard(nextChessboard, nextNextChessboard);
        updateMovementOnTable(nextNextChessboard, enemyMovements[0]);
        Movements endingMoves = collectMatingMoves(nextNextChessboard,
                                                   curentTeam);
        if (SZ(endingMoves) == 1) {
          std::cout << formatMovementSimplified(movements[i]) << "-"
                    << formatMovementSimplified(enemyMovements[0]) << "-"
                    << formatMovementSimplified(endingMoves[0]) << '\n';
        }
      }
    }
  }
}

void collectMateInTwoMoves(Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE],
                           int team) {
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)) {
        collectMateInTwoMovesFromPiece(chessboard, possibleFriendPiece);
      }
    }
  }
}
/*end collectMateInTwoMove*/

/*Piece moving*/
// Returns all movements from all pieces which belongs from a given team and matches a given symbol.
Movements collectPieceMovesWithGivenSymbol(
    Piece *chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE], int team,
    char symbol) {
  Movements movements;
  REP(i, CHESSBOARD_SIZE){
    REP(j, CHESSBOARD_SIZE){
      Piece *possibleFriendPiece = chessboard[i][j];
      if (itBelongsToThisTeam(possibleFriendPiece, team)
          && possibleFriendPiece->hasThisSymbol(symbol)) {
        collectLegalMovesFromPiece(chessboard, possibleFriendPiece, movements);
      }
    }
  }
  return movements;
}
/*end Piece moving*/

//formatMovement
vStr getPrettyPrintedStringsFromMovements(Movements movements) {
  vStr items;
  REP(i, SZ(movements)){
    items.push_back(formatMovementSimplified(movements[i]));
  }
  return items;
}

vStr getPrettyPrintedStringsFromMovementsDetailed(Movements movements) {
  vStr items;
  REP(i, SZ(movements)){
    items.push_back(formatMovement(movements[i]));
  }
  return items;
}

#endif /* SRC_CHESSENGINE_H_ */
