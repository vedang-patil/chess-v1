#include <iostream>

using namespace std;

enum class PieceType
{
    None
    King,
    Rook,
    Bishop,
    Queen,
    Knight,
    Pawn,
};

struct Piece
{
    bool isWhite;
    PieceType type;
};

class Board
{
private:
    Piece pieces[64];
    vector<pair<int, int>> moves;

public:
    Board()
    {
        fill(pieces.begin(), pieces.end(), PieceType.None);
    }

    void generatePseudoLegalMoves(bool colour)
    {
        moves.clear();

        for (int i = 0; i < 64; i++)
        {
            if (pieces[i] == PieceType.King)
            {
                if (i % 8 != 0) moves.emplace_back(i, i - 1);
                if (i % 8 != 7) moves.emplace_back(i, i + 1);
                if (i % 8 != 0) moves.emplace_back(i, i - 8);
                if (i % 8 != 7) moves.emplace_back(i, i + 8);
            }
            else if (pieces[i] == PieceType.Rook)
            {
                for (int j = i + 1; j < (i / 8 + 1); j++) moves.emplace_back(i, j);
                for (int j = i - 1; j >= ((i / 8) * 8); j--) moves.emplace_back(i, j);
                for (int j = i + 8; j < 64; j += 8) moves.emplace_back(i, j);
                for (int j = i - 8; j >= 0; j -= 8) moves.emplace_back(i, j);
            }
            else if (pieces[i] == PieceType.Bishop)
            {
                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8) moves.emplace_back(i, (++a) * 8 + (++b));
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0) moves.emplace_back(i, (--a) * 8 + (--b));
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0) moves.emplace_back(i, (++a) * 8 + (--b));
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8) moves.emplace_back(i, (--a) * 8 + (++b));
            }
            else if (pieces[i] == PieceType.Queen)
            {
                for (int j = i + 1; j < (i / 8 + 1); j++) moves.emplace_back(i, j);
                for (int j = i - 1; j >= ((i / 8) * 8); j--) moves.emplace_back(i, j);
                for (int j = i + 8; j < 64; j += 8) moves.emplace_back(i, j);
                for (int j = i - 8; j >= 0; j -= 8) moves.emplace_back(i, j);

                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8) moves.emplace_back(i, (++a) * 8 + (++b));
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0) moves.emplace_back(i, (--a) * 8 + (--b));
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0) moves.emplace_back(i, (++a) * 8 + (--b));
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8) moves.emplace_back(i, (--a) * 8 + (++b));
            }
            else if (pieces[i] == PieceType.Knight)
            {
                int a = i / 8, b = i % 8;

                if (a + 2 < 8)
                {
                    if (b + 1 < 8) moves.emplace_back((a + 2) * 8 + b + 1);
                    if (b - 1 >= 0) moves.emplace_back((a + 2) * 8 + b - 1);
                }
                if (a - 2 >= 0)
                {
                    if (b + 1 < 8) moves.emplace_back((a - 2) * 8 + b + 1);
                    if (b - 1 >= 0) moves.emplace_back((a - 2) * 8 + b - 1);
                }
                if (b + 2 < 8)
                {
                    if (a + 1 < 8) moves.emplace_back((a + 1) * 8 + b + 2);
                    if (a - 1 >= 0) moves.emplace_back((a - 1) * 8 + b + 2);
                }
                if (b - 2 >= 0)
                {
                    if (a + 1 < 8) moves.emplace_back((a + 1) * 8 + b - 2);
                    if (a - 1 >= 0) moves.emplace_back((a - 1) * 8 + b - 2);
                }
            }
        }
    }
};

int main()
{
    
}