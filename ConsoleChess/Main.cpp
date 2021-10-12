#include <iostream>
#include <string> 
#include <vector>
#include <variant>

#include "Headers/King.h"
#include "Headers/Queen.h"
#include "Headers/Bishop.h"
#include "Headers/Knight.h"
#include "Headers/Rook.h"
#include "Headers/Pawn.h"
#include "Headers/Empty.h"

const int gridAmount = 8;
const int xSpacing = 8;
const int ySpacing = 3;
const int yLineOverflow = 9; //the vertical lines displace x axis by 9 spaces
const int pieceOverflow = 1; //each board piece displaces x axis by 1 space

void out(std::string message) {
    std::cout << message;
}

std::string generateWhitespace(int spacing) {
    std::string whitespace = "";
    for (int i = 0; i < spacing; i ++) {
        whitespace += " ";
    }

    return whitespace;
}

void printXGridline() {
    for (int i=0; i < (gridAmount * xSpacing) + (gridAmount * pieceOverflow) + yLineOverflow; i++) {
        out("―");
    } 
}

void printYGridline() {
    std::string spacing = generateWhitespace(gridAmount + pieceOverflow);

    for (int i = 0; i < ySpacing; i++) {
        out("|");

        for (int j = 0; j < gridAmount; j++) {
            out(spacing + "|");
        }

        out("\n");
    }
}

void drawBoard() {
        for (int i = 0; i < gridAmount; i++) {
            printXGridline();
            out("\n");
            printYGridline();
        }

    printXGridline();
    out("\n");
}

std::vector<Piece> generateMajorPieceRow(bool isDark) {
    std::vector<Piece> majorPieceRow;

    Rook leftRook;
    leftRook.setName();
    leftRook.isDark = isDark;
    majorPieceRow.push_back(leftRook);

    Knight leftKnight;
    leftKnight.setName();
    leftKnight.isDark = isDark;
    majorPieceRow.push_back(leftKnight);

    Bishop leftBishop;
    leftBishop.setName();
    leftBishop.isDark = isDark;
    majorPieceRow.push_back(leftBishop);

    Queen queen;
    queen.setName();
    queen.isDark = isDark;
    majorPieceRow.push_back(queen);
    
    King king;
    king.setName();
    king.isDark = isDark;
    majorPieceRow.push_back(king);

    Bishop rightBishop;
    rightBishop.setName();
    rightBishop.isDark = isDark;
    majorPieceRow.push_back(rightBishop);

    Knight rightKnight;
    rightKnight.setName();
    rightKnight.isDark = isDark;
    majorPieceRow.push_back(rightKnight);

    Rook rightRook;
    rightRook.setName();
    rightRook.isDark = isDark;
    majorPieceRow.push_back(rightRook);

    for (int i = 0; i < majorPieceRow.size(); i++) {
        out(majorPieceRow[i].name + " ");
    }

    return majorPieceRow;
}

std::vector<Piece> generatePawnRow(bool isDark) {
    std::vector<Piece> pawnRow;

    for (int i = 0; i < gridAmount; i++) {
        Pawn pawn;
        pawn.setName();
        pawn.isDark = isDark;
        pawnRow.push_back(pawn);
    }

    return pawnRow;
}

std::vector<BoardItem> generateEmptyRow() {
    std::vector<BoardItem> emptyRow;

    for (int i = 0; i < gridAmount; i++) {
        Empty empty;
        empty.setName();
        emptyRow.push_back(empty);
    }

    return emptyRow;
}

std::vector<std::variant<Piece, BoardItem>> initialiseBoard() {
    std::vector<std::variant<Piece, BoardItem>> board;

    

    return board;
} 

int main()
{
    drawBoard();
    generateMajorPieceRow(false);
}