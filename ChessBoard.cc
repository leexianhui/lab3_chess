#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Student::ChessBoard;
using Student::ChessPiece;
using Student::PawnPiece;
using Student::RookPiece;
using Student::BishopPiece;
using Student::KingPiece;

#include <sstream>

ChessBoard::ChessBoard(int numRow, int numCol)
{
    numRows = numRow;
    numCols = numCol;
    turn = White;
    board = std::vector<std::vector<ChessPiece *>>(numRows, std::vector<ChessPiece *>(numCols, nullptr));
}

ChessBoard::~ChessBoard() {
    for (auto& rowVec : board) {
        for (ChessPiece*& p : rowVec) {
            delete p;
            p = nullptr;
        }
    }
}

void ChessBoard::createChessPiece(Color col, Type ty, int startRow, int startColumn)
{
    // clean up existing piece if any
    if (board.at(startRow).at(startColumn) != nullptr) {
        delete board.at(startRow).at(startColumn);
        board.at(startRow).at(startColumn) = nullptr;
    }

    ChessPiece* p = nullptr;
    if (ty == Pawn)   p = new PawnPiece(*this, col, startRow, startColumn);
    else if (ty == Rook)   p = new RookPiece(*this, col, startRow, startColumn);
    else if (ty == Bishop) p = new BishopPiece(*this, col, startRow, startColumn);
    else if (ty == King)   p = nullptr; // Part 1, ignore King creation
    board.at(startRow).at(startColumn) = p;
}

static bool in_bounds(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}


bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    // bounds
    if (!in_bounds(fromRow, fromColumn, numRows, numCols)) return false;
    if (!in_bounds(toRow, toColumn, numRows, numCols)) return false;

    ChessPiece* piece = board.at(fromRow).at(fromColumn);
    if (piece == nullptr) return false;
    if (fromRow == toRow && fromColumn == toColumn) return false;

    // destination has same color?
    ChessPiece* dst = board.at(toRow).at(toColumn);
    if (dst != nullptr && dst->getColor() == piece->getColor()) return false;

    // shape-specific rules (and pawn occupancy rules) via piece API
    if (!piece->canMoveToLocation(toRow, toColumn)) return false;

    // path obstruction for sliding pieces
    Type ty = piece->getType();
    if (ty == Rook) {
        // step along row or column
        int dr = (toRow > fromRow) ? 1 : (toRow < fromRow ? -1 : 0);
        int dc = (toColumn > fromColumn) ? 1 : (toColumn < fromColumn ? -1 : 0);
        int r = fromRow + dr;
        int c = fromColumn + dc;
        while (r != toRow || c != toColumn) {
            if (board.at(r).at(c) != nullptr) return false;
            r += dr; c += dc;
        }
    } else if (ty == Bishop) {
        int dr = (toRow > fromRow) ? 1 : -1;
        int dc = (toColumn > fromColumn) ? 1 : -1;
        int r = fromRow + dr;
        int c = fromColumn + dc;
        while (r != toRow) {
            if (board.at(r).at(c) != nullptr) return false;
            r += dr; c += dc;
        }
    } else if (ty == Pawn) {
        // Pawn path handled inside PawnPiece::canMoveToLocation (including 2-step mid cell)
    }

    return true;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    // Part 1: Not required
    return false;
}

bool ChessBoard::isPieceUnderThreat(int row, int column)
{
    // Part 1: Not required
    return false;
}


std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    // top scale
    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << i << " ";
    }
    outputString << std::endl << "  ";
    // top border
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++){
        outputString << row << "|";
        for (int column = 0; column < numCols; column++){
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString()) << " ";
        }
        outputString << "|" << std::endl;
    }

    // bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl << std::endl;

    return outputString;
}
