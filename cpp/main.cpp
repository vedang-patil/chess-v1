#include <emscripten/bind.h>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <iostream>

using namespace emscripten;
using namespace std;

#define INF 1e9

int coordToIndex(const string &square)
{
    int rank = square[1] - 1;
    int file = (square[0] - 'a');
    return rank * 8 + file;
}

string indexToCoord(int index)
{
    string result;
    result += ('a' + index % 8);
    result += ('1' + index / 8);
    return result;
}

class Board
{
public:
    char pieces[64] = {};
    bool isWhiteTurn;
    int enPassantTargetSquare = -1;
    int halfMoveClock, fullMoveCounter;
    bool canWhiteCastleKingside, canWhiteCastleQueenside, canBlackCastleKingside, canBlackCastleQueenside;

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

        chunks.push_back(fenString.substr(start));

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

        isWhiteTurn = (chunks[1] == "w");

        canWhiteCastleKingside = (chunks[2].find('K') != string::npos);
        canWhiteCastleQueenside = (chunks[2].find('Q') != string::npos);
        canBlackCastleKingside = (chunks[2].find('k') != string::npos);
        canBlackCastleQueenside = (chunks[2].find('q') != string::npos);

        if (chunks[3] != "-") enPassantTargetSquare = coordToIndex(chunks[3]);
        else enPassantTargetSquare = -1;

        halfMoveClock = stoi(chunks[4]);
        fullMoveCounter = stoi(chunks[5]);
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

        if (canWhiteCastleKingside) result.push_back('K');
        if (canWhiteCastleQueenside) result.push_back('Q');
        if (canBlackCastleKingside) result.push_back('k');
        if (canBlackCastleQueenside) result.push_back('q');

        result.push_back(' ');

        if (enPassantTargetSquare == -1) result.push_back('-');
        else for (char c: indexToCoord(enPassantTargetSquare)) result.push_back(c);

        result.push_back(' ');

        for (char c: to_string(halfMoveClock)) result.push_back(c);

        result.push_back(' ');

        for (char c: to_string(fullMoveCounter)) result.push_back(c);

        return string(result.begin(), result.end());
    }

    vector<pair<int, int>> getPseudoLegalMoves()
    {
        vector<pair<int, int>> moves;

        for (int i = 0; i < 64; i++)
        {
            if (pieces[i] == 0 || isWhiteTurn != isupper(pieces[i])) continue;
            if (canWhiteCastleQueenside && pieces[1] == 0 && pieces[2] == 0 && pieces[3] == 0) moves.emplace_back(4, 2);
            if (canWhiteCastleKingside && pieces[5] == 0 && pieces[6] == 0) moves.emplace_back(4, 6);
            if (canBlackCastleQueenside && pieces[57] == 0 && pieces[58] == 0 && pieces[59] == 0) moves.emplace_back(60, 58);
            if (canBlackCastleKingside && pieces[61] == 0 && pieces[62] == 0) moves.emplace_back(60, 62);
            if (pieces[i] == 'k' || pieces[i] == 'K')
            {
                if (i % 8 != 0 && (pieces[i - 1] == 0 || isupper(pieces[i]) != isupper(pieces[i - 1]))) moves.emplace_back(i, i - 1);
                if (i % 8 != 0 && i / 8 != 7 && (pieces[i + 7] == 0 || isupper(pieces[i]) != isupper(pieces[i + 7]))) moves.emplace_back(i, i + 7);
                if (i / 8 != 7 && (pieces[i + 8] == 0 || isupper(pieces[i]) != isupper(pieces[i + 8]))) moves.emplace_back(i, i + 8);
                if (i / 8 != 7 && i % 8 != 7 && (pieces[i + 9] == 0 || isupper(pieces[i]) != isupper(pieces[i + 9]))) moves.emplace_back(i, i + 9);
                if (i % 8 != 7 && (pieces[i + 1] == 0 || isupper(pieces[i]) != isupper(pieces[i + 1]))) moves.emplace_back(i, i + 1);
                if (i % 8 != 7 && i / 8 != 0 && (pieces[i - 7] == 0 || isupper(pieces[i]) != isupper(pieces[i - 7]))) moves.emplace_back(i, i - 7);
                if (i / 8 != 0 && (pieces[i - 8] == 0 || isupper(pieces[i]) != isupper(pieces[i - 8]))) moves.emplace_back(i, i - 8);
                if (i / 8 != 0 && i % 8 != 0 && (pieces[i - 9] == 0 || isupper(pieces[i]) != isupper(pieces[i - 9]))) moves.emplace_back(i, i - 9);
            }
            if (pieces[i] == 'r' || pieces[i] == 'R' || pieces[i] == 'q' || pieces[i] == 'Q')
            {
                for (int j = i + 1; j < (i / 8 + 1) * 8; j++)
                {
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                for (int j = i - 1; j >= ((i / 8) * 8); j--)
                {
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                for (int j = i + 8; j < 64; j += 8)
                {
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                for (int j = i - 8; j >= 0; j -= 8)
                {
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
            }

            if (pieces[i] == 'b' || pieces[i] == 'B' || pieces[i] == 'q' || pieces[i] == 'Q')
            {
                int a = i / 8, b = i % 8;
                while (a + 1 < 8 && b + 1 < 8)
                {
                    int j = (++a) * 8 + (++b);
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b > 0)
                {
                    int j = (--a) * 8 + (--b);
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a + 1 < 8 && b > 0)
                {
                    int j = (++a) * 8 + (--b);
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
                a = i / 8, b = i % 8;
                while (a > 0 && b + 1 < 8)
                {
                    int j = (--a) * 8 + (++b);
                    if (pieces[j] == 0 || isupper(pieces[i]) != isupper(pieces[j])) moves.emplace_back(i, j);
                    if (pieces[j] != 0) break;
                }
            }
            if (pieces[i] == 'n' || pieces[i] == 'N')
            {
                int a = i / 8, b = i % 8;

                if (a + 2 < 8)
                {
                    if (b + 1 < 8 && (pieces[(a + 2) * 8 + b + 1] == 0 || isupper(pieces[i]) != isupper(pieces[(a + 2) * 8 + b + 1]))) moves.emplace_back(i, (a + 2) * 8 + b + 1);
                    if (b - 1 >= 0 && (pieces[(a + 2) * 8 + b - 1] == 0 || isupper(pieces[i]) != isupper(pieces[(a + 2) * 8 + b - 1]))) moves.emplace_back(i, (a + 2) * 8 + b - 1);
                }
                if (a - 2 >= 0)
                {
                    if (b + 1 < 8 && (pieces[(a - 2) * 8 + b + 1] == 0 || isupper(pieces[i]) != isupper(pieces[(a - 2) * 8 + b + 1]))) moves.emplace_back(i, (a - 2) * 8 + b + 1);
                    if (b - 1 >= 0 && (pieces[(a - 2) * 8 + b - 1] == 0 || isupper(pieces[i]) != isupper(pieces[(a - 2) * 8 + b - 1]))) moves.emplace_back(i, (a - 2) * 8 + b - 1);
                }
                if (b + 2 < 8)
                {
                    if (a + 1 < 8 && (pieces[(a + 1) * 8 + b + 2] == 0 || isupper(pieces[i]) != isupper(pieces[(a + 1) * 8 + b + 2]))) moves.emplace_back(i, (a + 1) * 8 + b + 2);
                    if (a - 1 >= 0 && (pieces[(a - 1) * 8 + b + 2] == 0 || isupper(pieces[i]) != isupper(pieces[(a - 1) * 8 + b + 2]))) moves.emplace_back(i, (a - 1) * 8 + b + 2);
                }
                if (b - 2 >= 0)
                {
                    if (a + 1 < 8 && (pieces[(a + 1) * 8 + b - 2] == 0 || isupper(pieces[i]) != isupper(pieces[(a + 1) * 8 + b - 2]))) moves.emplace_back(i, (a + 1) * 8 + b - 2);
                    if (a - 1 >= 0 && (pieces[(a - 1) * 8 + b - 2] == 0 || isupper(pieces[i]) != isupper(pieces[(a - 1) * 8 + b - 2]))) moves.emplace_back(i, (a - 1) * 8 + b - 2);
                }
            }
            if (pieces[i] == 'P')
            {
                if (i + 8 < 64 && pieces[i + 8] == 0) moves.emplace_back(i, i + 8);
                if (i / 8 == 1 && pieces[i + 8] == 0 && pieces[i + 16] == 0) moves.emplace_back(i, i + 16);
                if (i + 7 < 64 && i % 8 != 0 && pieces[i + 7] != 0 && !isupper(pieces[i + 7])) moves.emplace_back(i, i + 7);
                if (i + 9 < 64 && i % 8 != 7  && pieces[i + 9] != 0 && !isupper(pieces[i + 9])) moves.emplace_back(i, i + 9);
                if (i % 8 != 0 && i - 1 == enPassantTargetSquare && !isupper(pieces[enPassantTargetSquare])) moves.emplace_back(i, i + 7);
                if (i % 8 != 7 && i + 1 == enPassantTargetSquare && !isupper(pieces[enPassantTargetSquare])) moves.emplace_back(i, i + 9);
            }
            if (pieces[i] == 'p')
            {
                if (i - 8 >= 0 && pieces[i - 8] == 0) moves.emplace_back(i, i - 8);
                if (i / 8 == 6 && pieces[i - 8] == 0 && pieces[i - 16] == 0) moves.emplace_back(i, i - 16);
                if (i - 7 >= 0 && i % 8 != 7 && pieces[i - 7] != 0 && isupper(pieces[i - 7])) moves.emplace_back(i, i - 7);
                if (i - 9 >= 0 && i % 8 != 0 && pieces[i - 9] != 0 && isupper(pieces[i - 9])) moves.emplace_back(i, i - 9);
                if (i % 8 != 0 && i - 1 == enPassantTargetSquare && isupper(pieces[enPassantTargetSquare])) moves.emplace_back(i, i - 9);
                if (i % 8 != 7 && i + 1 == enPassantTargetSquare && isupper(pieces[enPassantTargetSquare])) moves.emplace_back(i, i - 7);
            }
        }

        return moves;
    }

    vector<pair<int, int>> getMoves()
    {
        vector<pair<int, int>> pseudoLegalMoves = getPseudoLegalMoves();
        vector<pair<int, int>> legalMoves;

        for (const pair<int, int> &move: pseudoLegalMoves)
        {
            legalMoves.push_back(move);
            Board newBoard(*this);
            newBoard.pseudoLegalMove(move);
            for (const pair<int, int> &responseMove: newBoard.getPseudoLegalMoves())
            {
                if (newBoard.pieces[responseMove.second] == (isWhiteTurn ? 'K' : 'k'))
                {
                    legalMoves.pop_back();
                    break;
                }
            }
        }

        return legalMoves;
    }

    bool move(const pair<int, int> &move)
    {
        vector<pair<int, int>> moves = getMoves();
        if (find(moves.begin(), moves.end(), move) == moves.end()) return false;

        if (!isupper(pieces[move.first])) fullMoveCounter++;
        halfMoveClock++;
        if (pieces[move.second] != 0 || pieces[move.first] == 'p' || pieces[move.first] == 'P') halfMoveClock = 0;

        if (pieces[move.first] == 'P' && pieces[move.second] == 0 && (move.first + 7 == move.second || move.first + 9 == move.second)) pieces[enPassantTargetSquare] = 0;
        else if (pieces[move.first] == 'p' && pieces[move.second] == 0 && (move.first - 7 == move.second || move.first - 9 == move.second)) pieces[enPassantTargetSquare] = 0;
        enPassantTargetSquare = -1;
        if (pieces[move.first] == 'P' && move.second == move.first + 16) enPassantTargetSquare = move.second;
        else if (pieces[move.first] == 'p' && move.second == move.first - 16) enPassantTargetSquare = move.second;

        if (pieces[4] == 'K' && move == make_pair(4, 2))
        {
            pieces[0] = 0;
            pieces[3] = 'R';
        }
        else if (pieces[4] == 'K' && move == make_pair(4, 6))
        {
            pieces[7] = 0;
            pieces[5] = 'R';
        }
        else if (pieces[60] == 'k' && move == make_pair(60, 58))
        {
            pieces[56] = 0;
            pieces[59] = 'r';
        }
        else if (pieces[60] == 'k' && move == make_pair(60, 62))
        {
            pieces[63] = 0;
            pieces[61] = 'r';
        }

        if (pieces[move.first] == 'K') canWhiteCastleKingside = false, canWhiteCastleQueenside = false;
        else if (pieces[move.first] == 'k') canBlackCastleKingside = false, canBlackCastleQueenside = false;
        else if (move.first == 0) canWhiteCastleQueenside = false;
        else if (move.first == 7) canWhiteCastleKingside = false;
        else if (move.first == 56) canBlackCastleQueenside = false;
        else if (move.first == 63) canBlackCastleKingside = false;

        if (move.second / 8 >= 7 && pieces[move.first] == 'P') pieces[56 + move.second % 8] = 'Q';
        else if (move.second / 8 <= 0 && pieces[move.first] == 'p') pieces[move.second % 8] = 'q';
        else pieces[move.second] = pieces[move.first];
        pieces[move.first] = 0;

        isWhiteTurn = !isWhiteTurn;
        return true;
    }

    bool pseudoLegalMove(const pair<int, int> &move)
    {
        vector<pair<int, int>> moves = getPseudoLegalMoves();
        if (find(moves.begin(), moves.end(), move) == moves.end()) return false;

        if (!isupper(pieces[move.first])) fullMoveCounter++;
        halfMoveClock++;
        if (pieces[move.second] != 0 || pieces[move.first] == 'p' || pieces[move.first] == 'P') halfMoveClock = 0;

        if (pieces[move.first] == 'P' && pieces[move.second] == 0 && (move.first + 7 == move.second || move.first + 9 == move.second)) pieces[enPassantTargetSquare] = 0;
        else if (pieces[move.first] == 'p' && pieces[move.second] == 0 && (move.first - 7 == move.second || move.first - 9 == move.second)) pieces[enPassantTargetSquare] = 0;
        enPassantTargetSquare = -1;
        if (pieces[move.first] == 'P' && move.second == move.first + 16) enPassantTargetSquare = move.second;
        else if (pieces[move.first] == 'p' && move.second == move.first - 16) enPassantTargetSquare = move.second;

        if (pieces[4] == 'K' && move == make_pair(4, 2))
        {
            pieces[0] = 0;
            pieces[3] = 'R';
        }
        else if (pieces[4] == 'K' && move == make_pair(4, 6))
        {
            pieces[7] = 0;
            pieces[5] = 'R';
        }
        else if (pieces[60] == 'k' && move == make_pair(60, 58))
        {
            pieces[56] = 0;
            pieces[59] = 'r';
        }
        else if (pieces[60] == 'k' && move == make_pair(60, 62))
        {
            pieces[63] = 0;
            pieces[61] = 'r';
        }

        if (pieces[move.first] == 'K') canWhiteCastleKingside = false, canWhiteCastleQueenside = false;
        else if (pieces[move.first] == 'k') canBlackCastleKingside = false, canBlackCastleQueenside = false;
        else if (move.first == 0) canWhiteCastleQueenside = false;
        else if (move.first == 7) canWhiteCastleKingside = false;
        else if (move.first == 56) canBlackCastleQueenside = false;
        else if (move.first == 63) canBlackCastleKingside = false;

        if (move.second / 8 >= 7 && pieces[move.first] == 'P') pieces[56 + move.second % 8] = 'Q';
        else if (move.second / 8 <= 0 && pieces[move.first] == 'p') pieces[move.second % 8] = 'q';
        else pieces[move.second] = pieces[move.first];
        pieces[move.first] = 0;

        isWhiteTurn = !isWhiteTurn;
        return true;
    }
};

int __minimax(Board board, int depth, bool isWhite, int alpha, int beta)
{
    if (depth == 0)
    {
        int score = 0;
        for (char piece: board.pieces)
        {
            int pieceScore = 0;
            switch (toupper(piece))
            {
                case 'P':
                    pieceScore = 100;
                    break;
                case 'N':
                    pieceScore = 300;
                    break;
                case 'B':
                    pieceScore = 325;
                    break;
                case 'R':
                    pieceScore = 500;
                    break;
                case 'Q':
                    pieceScore = 900;
                    break;
            }

            pieceScore *= ((toupper(piece) == piece) ? 1 : -1);
            score += pieceScore;
        }

        return score;
    }

    if (isWhite)
    {
        int bestValue = -INF;
        
        for (const pair<int, int> &move: board.getMoves())
        {
            Board moveBoard(board);
            moveBoard.move(move);
            bestValue = max(bestValue, __minimax(moveBoard, depth + 1, false, alpha, beta));
            alpha = max(alpha, bestValue);
            if (beta <= alpha) break;
        }

        return bestValue;
    }
    else
    {
        int bestValue = INF;
        
        for (const pair<int, int> &move: board.getMoves())
        {
            Board moveBoard(board);
            moveBoard.move(move);
            bestValue = min(bestValue, __minimax(moveBoard, depth + 1, true, alpha, beta));
            beta = min(beta, bestValue);
            if (beta <= alpha) break;
        }

        return bestValue;
    }
}

int minimax(Board board, int depth, bool isWhite)
{
    return __minimax(board, depth, isWhite, -INF, INF);
}

Board makeCopy(const Board& board)
{
    return Board(board);
}

EMSCRIPTEN_BINDINGS(my_module)
{
    class_<pair<int, int>>("Pair")
        .constructor()
        .constructor<int, int>()
        .property("first", &pair<int, int>::first)
        .property("second", &pair<int, int>::second);

    register_vector<pair<int, int>>("PairList");
    
    class_<Board>("Board")
        .constructor()
        .constructor<string>()
        .function("getPseudoLegalMoves", &Board::getPseudoLegalMoves)
        .function("getMoves", &Board::getMoves)
        .function("getAsFenString", &Board::getAsFenString)
        .function("move", &Board::move);
    
    emscripten::function("makeCopy", &makeCopy);
    emscripten::function("minimax", &minimax);
}