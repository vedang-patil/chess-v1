#include <iostream>

using namespace std;

typedef unsigned long long int U64;

class Board
{
    U64 whitePawns;
    U64 whiteRooks;
    U64 whiteBishops;
    U64 whiteKnights;
    U64 whiteQueens;
    U64 whiteKing;

    U64 blackPawns;
    U64 blackRooks;
    U64 blackBishops;
    U64 blackKnights;
    U64 blackQueens;
    U64 blackKing;

    Board()
    {
        whitePawns = 65280;
        whiteRooks = 129;
        whiteBishops = 46;
        whiteKnights = 66;
        whiteQueens = 8;
        whiteKing = 16;

        blackPawns = 71776119061217280;
        blackRooks = 9295429630892703744;
        blackBishops = 2594073385365405696;
        blackKnights = 4755801206503243776;
        blackQueens = 576460752303423488;
        blackKing = 1152921504606846976;
    }
};

int main()
{
}