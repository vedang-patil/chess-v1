#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

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

    Piece()
    {
        this->colour = PieceColour::None;
        this->type = PieceType::None;
    }

    Piece(PieceColour colour, PieceType type)
    {
        this->colour = colour;
        this->type = type;
    }
};

class Board
{
public:
    Piece pieces[64] = {};
    vector<pair<int, int>> moves;

    Board(const string &fenString)
    {
        fill(pieces, pieces + 64, Piece(PieceColour::None, PieceType::None));
        vector<string> chunks;

        int start = 0;
        int end = fenString.find(' ');

        while (end != (int)string::npos)
        {
            chunks.push_back(fenString.substr(start, end - start));
            start = end + 1;
            end = (int)fenString.find(' ', start);
        }

        int rank = 7, file = 0;

        for (const char &c: chunks[0])
        {
            if (c == '/') continue;
            int i = rank * 8 + file;

            if (isdigit(c))
            {
                file += (c - '0');
            }
            else
            {
                switch (c)
                {
                    case 'r':
                        pieces[i] = {PieceColour::Black, PieceType::Rook};
                        break;
                    case 'n':
                        pieces[i] = {PieceColour::Black, PieceType::Knight};
                        break;
                    case 'b':
                        pieces[i] = {PieceColour::Black, PieceType::Bishop};
                        break;
                    case 'q':
                        pieces[i] = {PieceColour::Black, PieceType::Queen};
                        break;
                    case 'k':
                        pieces[i] = {PieceColour::Black, PieceType::King};
                        break;
                    case 'p':
                        pieces[i] = {PieceColour::Black, PieceType::Pawn};
                        break;

                    case 'R':
                        pieces[i] = {PieceColour::White, PieceType::Rook};
                        break;
                    case 'N':
                        pieces[i] = {PieceColour::White, PieceType::Knight};
                        break;
                    case 'B':
                        pieces[i] = {PieceColour::White, PieceType::Bishop};
                        break;
                    case 'Q':
                        pieces[i] = {PieceColour::White, PieceType::Queen};
                        break;
                    case 'K':
                        pieces[i] = {PieceColour::White, PieceType::King};
                        break;
                    case 'P':
                        pieces[i] = {PieceColour::White, PieceType::Pawn};
                        break;
                }

                file++;
            }

            rank -= file / 8;
            file %= 8;
        }
    }

    Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

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
                if (i % 8 != 0 && i / 8 != 7 && (colour != pieces[i + 7].colour))
                {
                    moves.emplace_back(i, i + 7);
                }
                if (i / 8 != 7 && (colour != pieces[i + 8].colour))
                {
                    moves.emplace_back(i, i + 8);
                }
                if (i / 8 != 7 && i % 8 != 7 && (colour != pieces[i + 9].colour))
                {
                    moves.emplace_back(i, i + 9);
                }
                if (i % 8 != 7 && (colour != pieces[i + 1].colour))
                {
                    moves.emplace_back(i, i + 1);
                }
                if (i % 8 != 7 && i / 8 != 0 && (colour != pieces[i - 7].colour))
                {
                    moves.emplace_back(i, i - 7);
                }
                if (i / 8 != 0 && (colour != pieces[i - 8].colour))
                {
                    moves.emplace_back(i, i - 8);
                }
                if (i / 8 != 0 && i % 8 != 0 && (colour != pieces[i - 9].colour))
                {
                    moves.emplace_back(i, i - 9);
                }
            }
            if (pieces[i].type == PieceType::Rook || pieces[i].type == PieceType::Queen)
            {
                for (int j = i + 1; j < (i / 8 + 1); j++)
                {
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                for (int j = i - 1; j >= ((i / 8) * 8); j--)
                {
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                for (int j = i + 8; j < 64; j += 8)
                {
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                for (int j = i - 8; j >= 0; j -= 8)
                {
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
            }
            if (pieces[i].type == PieceType::Bishop || pieces[i].type == PieceType::Queen)
            {
                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8)
                {
                    int j = (++a) * 8 + (++b);
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0)
                {
                    int j = (--a) * 8 + (--b);
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0)
                {
                    int j = (++a) * 8 + (--b);
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8)
                {
                    int j = (--a) * 8 + (++b);
                    if (pieces[j].colour == colour)
                        break;
                    moves.emplace_back(i, j);
                    if (pieces[j].colour != PieceColour::None)
                        break;
                }
            }
            if (pieces[i].type == PieceType::Knight)
            {
                int a = i / 8, b = i % 8;

                if (a + 2 < 8)
                {
                    if (b + 1 < 8 && pieces[(a + 2) * 8 + b + 1].colour != colour)
                        moves.emplace_back(i, (a + 2) * 8 + b + 1);
                    if (b - 1 >= 0 && pieces[(a + 2) * 8 + b - 1].colour != colour)
                        moves.emplace_back(i, (a + 2) * 8 + b - 1);
                }
                if (a - 2 >= 0)
                {
                    if (b + 1 < 8 && pieces[(a - 2) * 8 + b + 1].colour != colour)
                        moves.emplace_back(i, (a - 2) * 8 + b + 1);
                    if (b - 1 >= 0 && pieces[(a - 2) * 8 + b - 1].colour != colour)
                        moves.emplace_back(i, (a - 2) * 8 + b - 1);
                }
                if (b + 2 < 8)
                {
                    if (a + 1 < 8 && pieces[(a + 1) * 8 + b + 2].colour != colour)
                        moves.emplace_back(i, (a + 1) * 8 + b + 2);
                    if (a - 1 >= 0 && pieces[(a - 1) * 8 + b + 2].colour != colour)
                        moves.emplace_back(i, (a - 1) * 8 + b + 2);
                }
                if (b - 2 >= 0)
                {
                    if (a + 1 < 8 && pieces[(a + 1) * 8 + b - 2].colour != colour)
                        moves.emplace_back(i, (a + 1) * 8 + b - 2);
                    if (a - 1 >= 0 && pieces[(a - 1) * 8 + b - 2].colour != colour)
                        moves.emplace_back(i, (a - 1) * 8 + b - 2);
                }
            }
            if (pieces[i].type == PieceType::Pawn)
            {
                if (pieces[i].colour == PieceColour::White)
                {
                    if (i + 8 < 64 && pieces[i + 8].colour == PieceColour::None)
                    {
                        moves.emplace_back(i, i + 8);
                    }

                    if (i / 8 == 1 && pieces[i + 8].colour == PieceColour::None && pieces[i + 16].colour == PieceColour::None)
                    {
                        moves.emplace_back(i, i + 16);
                    }

                    if (i + 7 < 64 && i % 8 != 0 && pieces[i + 7].colour == PieceColour::Black)
                    {
                        moves.emplace_back(i, i + 7);
                    }
                    
                    if (i + 9 < 64 && i % 8 != 7  && pieces[i + 9].colour == PieceColour::Black)
                    {
                        moves.emplace_back(i, i + 9);
                    }
                }
                else
                {
                    if (i - 8 >= 0 && pieces[i - 8].colour == PieceColour::None)
                    {
                        moves.emplace_back(i, i - 8);
                    }

                    if (i / 8 == 6 && pieces[i - 8].colour == PieceColour::None && pieces[i - 16].colour == PieceColour::None)
                    {
                        moves.emplace_back(i, i - 16);
                    }

                    if (i - 7 >= 0 && i % 8 != 7 && pieces[i - 7].colour == PieceColour::White)
                    {
                        moves.emplace_back(i, i - 7);
                    }
                    
                    if (i - 9 >= 0 && i % 8 != 0 && pieces[i - 9].colour == PieceColour::White)
                    {
                        moves.emplace_back(i, i - 9);
                    }
                }
            }
        }
    }
};

int main()
{
    Board board("8/8/8/4k3/8/2R1K3/4P3/8 b - - 0 1");
    board.generatePseudoLegalMoves(PieceColour::Black);

    for (int r = 7; r >= 0; r--)
    {
        for (int c = 0; c < 8; c++)
        {
            int i = r * 8 + c;
            if (board.pieces[i].colour == PieceColour::White) cout << "W ";
            else if (board.pieces[i].colour == PieceColour::Black) cout << "B ";
            else cout << "☐ ";
        }

        cout << "\n";
    }
}