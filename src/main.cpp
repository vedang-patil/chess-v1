#include <iostream>
#include <vector>

using namespace std;

enum class PieceType
{
    None,
    King,
    Rook,
    Bishop,
    Queen,
    Knight,
    Pawn,
};

enum class PieceColour
{
    None,
    White,
    Black
};

struct Piece
{
    PieceColour colour;
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
        fill(pieces, pieces + 64, Piece(PieceColour::None, PieceType::None));
    }

    void generatePseudoLegalMoves(PieceColour colour)
    {
        moves.clear();

        for (int i = 0; i < 64; i++)
        {
            if (pieces[i].colour != colour) continue;

            if (pieces[i].type == PieceType::King)
            {
                if (i % 8 != 0 && (colour != pieces[i - 1].colour))
                {
                    moves.emplace_back(i, i - 1);
                }
                if (i % 8 != 7 && (colour != pieces[i + 1].colour))
                {
                    moves.emplace_back(i, i + 1);
                }
                if (i % 8 != 0 && (colour != pieces[i - 8].colour))
                {
                    moves.emplace_back(i, i - 8);
                }
                if (i % 8 != 7 && (colour != pieces[i + 8].colour))
                {
                    moves.emplace_back(i, i + 8);
                }
            }
            if (pieces[i].type == PieceType::Rook || pieces[i].type == PieceType::Queen)
            {
                for (int j = i + 1; j < (i / 8 + 1); j++)
                {
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                for (int j = i - 1; j >= ((i / 8) * 8); j--)
                {
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                for (int j = i + 8; j < 64; j += 8)
                {
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                for (int j = i - 8; j >= 0; j -= 8)
                {
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
            }
            if (pieces[i].type == PieceType::Bishop || pieces[i].type == PieceType::Queen)
            {
                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8)
                {
                    int j = (++a) * 8 + (++b);
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0)
                {
                    int j = (--a) * 8 + (--b);
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0)
                {
                    int j = (++a) * 8 + (--b);
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8)
                {
                    int j = (--a) * 8 + (++b);
                    if (pieces[j].colour == colour) break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None) break;
                }
            }
            if (pieces[i].type == PieceType::Knight)
            {
                int a = i / 8, b = i % 8;

                if (a + 2 < 8)
                {
                    if (b + 1 < 8 && pieces[(a + 2) * 8 + b + 1].colour != colour) moves.emplace_back(i, (a + 2) * 8 + b + 1);
                    if (b - 1 >= 0 && pieces[(a + 2) * 8 + b - 1].colour != colour) moves.emplace_back(i, (a + 2) * 8 + b - 1);
                }
                if (a - 2 >= 0)
                {
                    if (b + 1 < 8 && pieces[(a - 2) * 8 + b + 1].colour != colour) moves.emplace_back(i, (a - 2) * 8 + b + 1);
                    if (b - 1 >= 0 && pieces[(a - 2) * 8 + b - 1].colour != colour) moves.emplace_back(i, (a - 2) * 8 + b - 1);
                }
                if (b + 2 < 8)
                {
                    if (a + 1 < 8 && pieces[(a + 1) * 8 + b + 2].colour != colour) moves.emplace_back(i, (a + 1) * 8 + b + 2);
                    if (a - 1 >= 0 && pieces[(a - 1) * 8 + b + 2].colour != colour) moves.emplace_back(i, (a - 1) * 8 + b + 2);
                }
                if (b - 2 >= 0)
                {
                    if (a + 1 < 8 && pieces[(a + 1) * 8 + b - 2].colour != colour) moves.emplace_back(i, (a + 1) * 8 + b - 2);
                    if (a - 1 >= 0 && pieces[(a - 1) * 8 + b - 2].colour != colour) moves.emplace_back(i, (a - 1) * 8 + b - 2);
                }
            }
            if (pieces[i].type == PieceType::Pawn)
            {
                if (pieces[i].colour == PieceColour::White)
                {
                }
                else
                {
                }
            }
        }
    }
};

int main()
{
    
}