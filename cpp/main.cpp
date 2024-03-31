#include <emscripten/bind.h>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace emscripten;
using namespace std;

bool isWhite(char piece)
{
    return isupper(piece);
}

class Board
{
private:
    char pieces[64] = {};
    bool isWhiteTurn = true;
public:
    vector<pair<int, int>> moves;

    Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}
    Board(const string &fenString)
    {
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
                pieces[i] = c;
                file++;
            }

            rank -= file / 8;
            file %= 8;
        }
    }

    string getAsFenString()
    {
        vector<char> result;

        int rank = 7, file = 0;
        int emptyCount = 0;

        for (int i = 0; i < 64; i++)
        {
            if (pieces[rank * 8 + file] == 0) emptyCount += 1;
            else
            {
                if (emptyCount > 0)
                {
                    result.push_back('0' + emptyCount);
                    emptyCount = 0;
                }

                result.push_back(pieces[rank * 8 + file]);
            }

            file++;
            if (file == 8)
            {
                if (emptyCount > 0)
                {
                    result.push_back('0' + emptyCount);
                    emptyCount = 0;
                }

                result.push_back('/');
            }

            rank -= file / 8;
            file %= 8;
        }

        result[result.size() - 1] = ' ';
        if (isWhiteTurn) result.push_back('w');
        else result.push_back('b');
        result.push_back(' ');

        return string(result.begin(), result.end()) + "KQkq - 0 1";
    }

    void generatePseudoLegalMoves(bool isWhiteTurn)
    {
        moves.clear();

        for (int i = 0; i < 64; i++)
        {
            if (pieces[i] == 0 || isWhiteTurn != isWhite(pieces[i])) continue;
            if (pieces[i] == 'k' || pieces[i] == 'K')
            {
                if (i % 8 != 0 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i - 1]))) moves.emplace_back(i, i - 1);
                if (i % 8 != 0 && i / 8 != 7 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i + 7]))) moves.emplace_back(i, i + 7);
                if (i / 8 != 7 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i + 8]))) moves.emplace_back(i, i + 8);
                if (i / 8 != 7 && i % 8 != 7 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i + 9]))) moves.emplace_back(i, i + 9);
                if (i % 8 != 7 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i + 1]))) moves.emplace_back(i, i + 1);
                if (i % 8 != 7 && i / 8 != 0 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i - 7]))) moves.emplace_back(i, i - 7);
                if (i / 8 != 0 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i - 8]))) moves.emplace_back(i, i - 8);
                if (i / 8 != 0 && i % 8 != 0 && (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[i - 9]))) moves.emplace_back(i, i - 9);
            }
            if (pieces[i] == 'r' || pieces[i] == 'R' || pieces[i] == 'q' || pieces[i] == 'Q')
            {
                for (int j = i + 1; j < (i / 8 + 1); j++)
                {
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                for (int j = i - 1; j >= ((i / 8) * 8); j--)
                {
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                for (int j = i + 8; j < 64; j += 8)
                {
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                for (int j = i - 8; j >= 0; j -= 8)
                {
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
            }
            if (pieces[i] == 'b' || pieces[i] == 'B' || pieces[i] == 'q' || pieces[i] == 'Q')
            {
                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8)
                {
                    int j = (++a) * 8 + (++b);
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0)
                {
                    int j = (--a) * 8 + (--b);
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0)
                {
                    int j = (++a) * 8 + (--b);
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8)
                {
                    int j = (--a) * 8 + (++b);
                    if (pieces[i] == 0 || isWhite(pieces[i]) != isWhite(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[i] != 0) break;
                }
            }
            if (pieces[i] == 'n' || pieces[i] == 'N')
            {
                int a = i / 8, b = i % 8;

                if (a + 2 < 8)
                {
                    if (b + 1 < 8 && (pieces[(a + 2) * 8 + b + 1] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a + 2) * 8 + b + 1]))) moves.emplace_back(i, (a + 2) * 8 + b + 1);
                    if (b - 1 >= 0 && (pieces[(a + 2) * 8 + b - 1] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a + 2) * 8 + b - 1]))) moves.emplace_back(i, (a + 2) * 8 + b - 1);
                }
                if (a - 2 >= 0)
                {
                    if (b + 1 < 8 && (pieces[(a - 2) * 8 + b + 1] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a - 2) * 8 + b + 1]))) moves.emplace_back(i, (a - 2) * 8 + b + 1);
                    if (b - 1 >= 0 && (pieces[(a - 2) * 8 + b - 1] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a - 2) * 8 + b - 1]))) moves.emplace_back(i, (a - 2) * 8 + b - 1);
                }
                if (b + 2 < 8)
                {
                    if (a + 1 < 8 && (pieces[(a + 1) * 8 + b + 2] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a + 1) * 8 + b + 2]))) moves.emplace_back(i, (a + 1) * 8 + b + 2);
                    if (a - 1 >= 0 && (pieces[(a - 1) * 8 + b + 2] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a - 1) * 8 + b + 2]))) moves.emplace_back(i, (a - 1) * 8 + b + 2);
                }
                if (b - 2 >= 0)
                {
                    if (a + 1 < 8 && (pieces[(a + 1) * 8 + b - 2] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a + 1) * 8 + b - 2]))) moves.emplace_back(i, (a + 1) * 8 + b - 2);
                    if (a - 1 >= 0 && (pieces[(a - 1) * 8 + b - 2] == 0 || isWhite(pieces[i]) != isWhite(pieces[(a - 1) * 8 + b - 2]))) moves.emplace_back(i, (a - 1) * 8 + b - 2);
                }
            }
            if (pieces[i] == 'P')
            {
                if (i + 8 < 64 && pieces[i + 8] == 0) moves.emplace_back(i, i + 8);
                if (i / 8 == 1 && pieces[i + 8] == 0 && pieces[i + 16] == 0) moves.emplace_back(i, i + 16);
                if (i + 7 < 64 && i % 8 != 0 && pieces[i + 7] != 0 && !isWhite(pieces[i + 7])) moves.emplace_back(i, i + 7);
                if (i + 9 < 64 && i % 8 != 7  && pieces[i + 9] != 0 && !isWhite(pieces[i + 9])) moves.emplace_back(i, i + 9);
            }
            if (pieces[i] == 'p')
            {
                if (i - 8 >= 0 && pieces[i - 8] == 0) moves.emplace_back(i, i - 8);
                if (i / 8 == 6 && pieces[i - 8] == 0 && pieces[i - 16] == 0) moves.emplace_back(i, i - 16);
                if (i - 7 >= 0 && i % 8 != 7 && pieces[i - 7] != 0 && isWhite(pieces[i - 7])) moves.emplace_back(i, i - 7);
                if (i - 9 >= 0 && i % 8 != 0 && pieces[i - 9] != 0 && isWhite(pieces[i - 9])) moves.emplace_back(i, i - 9);
            }
        }
    }
};

EMSCRIPTEN_BINDINGS(my_module)
{
    register_vector<pair<int, int>>("PairList");
    
    class_<Board>("Board")
        .constructor()
        .constructor<string>()
        .property("moves", &Board::moves)
        .function("generatePseudoLegalMoves", &Board::generatePseudoLegalMoves)
        .function("getAsFenString", &Board::getAsFenString);
}