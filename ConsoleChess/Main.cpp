#include <iostream>
#include <string> 
#include <vector>
#include <variant>
#include <regex>

#include "Headers/BoardItem.h"

const int squaresOnAxes = 8;
const int gridSpacing = 6;
const int xWidth = 8;
const int yLength = 3;
const int yLineOverflow = 9; //the vertical lines displace x axis by 9 spaces
const int pieceOverflow = 1; //each board piece displaces x axis by 1 space

std::vector<std::vector<std::string>> takenPieces = {
    {""},
    {""}
};

int rowCount = 0;
int itemInRowCount = 0;

void out(std::string message) {
    std::cout << message;
}

std::string input(std::string message) {
    std::string put;

    out(message);
    std::getline(std::cin, put);
    return put;
}

std::string generateIconSpacing(int spacing) {
    std::string whitespace = "";
    for (int i = 0; i < spacing; i++) {
        whitespace += " ";
    }

    return whitespace;
}

void printXGridline() {
    for (int i=0; i < (gridSpacing * xWidth) + (squaresOnAxes * pieceOverflow) + yLineOverflow; i++) {
        out("―");
    }

    out("\n");
}

void printYGridline(std::vector<std::vector<BoardItem>> board) {
    std::string spacing;
    int midPoint = 1;

    for (int i = 0; i < yLength; i++) {
        out("|");

        for (int j = 0; j < squaresOnAxes; j++) {
            spacing = generateIconSpacing(gridSpacing + pieceOverflow);

            if (i == midPoint) {
                spacing = generateIconSpacing(gridSpacing / 2);
                out(spacing + board[rowCount][itemInRowCount].icon);

                itemInRowCount ++;

                if (itemInRowCount % squaresOnAxes == 0) {
                    rowCount ++;
                    itemInRowCount = 0;
                }
            }

            out(spacing + "|");
        }

        if (i == midPoint) {
            out(spacing + std::to_string(9-rowCount)); //9 inverts numbers
        }

        out("\n");
    }
}

void printXLabels() {
    std::string spacing = generateIconSpacing(gridSpacing / 2);
    std::vector labels = {"a", "b", "c", "d", "e", "f", "g", "h"};
    int labelIndex = 0;

    for (int i=0; i < (gridSpacing * xWidth) + (squaresOnAxes * pieceOverflow) + yLineOverflow; i++) {
        if (i % squaresOnAxes == 0 && i != 0) {
            out(" " + spacing + labels[labelIndex] + spacing);
            labelIndex ++;
        }
    }

    out("\n");
}

void drawBoard(std::vector<std::vector<BoardItem>> board) {
    for (int i = 0; i < squaresOnAxes; i++) {
        printXGridline();
        printYGridline(board);
    }

    printXGridline();
    printXLabels();

    rowCount = 0;
    itemInRowCount = 0;
}

std::vector<BoardItem> generateMajorPieceRow(bool isDark) {
    std::vector<BoardItem> majorPieceRow;
    std::vector<std::string> names = {"ROOK", "KNHT", "BSHP", "QUEN", "KING", "BSHP", "KNHT", "ROOK"};

    for (int i = 0; i < names.size(); i++) {
        BoardItem boardItem;
        boardItem.isDark = isDark;
        boardItem.setName(names[i]);
        boardItem.setIcon();
        majorPieceRow.push_back(boardItem);
    }

    return majorPieceRow;
}

std::vector<BoardItem> generatePawnRow(bool isDark) {
    std::vector<BoardItem> pawnRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        BoardItem boardItem;
        boardItem.isDark = isDark;
        boardItem.setName("PAWN");
        boardItem.setIcon();
        pawnRow.push_back(boardItem);
    }

    return pawnRow;
}

std::vector<BoardItem> generateEmptyRow() {
    std::vector<BoardItem> emptyRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        BoardItem boardItem;
        boardItem.setName("EMTY");
        boardItem.setIcon();
        emptyRow.push_back(boardItem);
    }

    return emptyRow;
}

std::string boolToString(bool b) {
  return b ? "true" : "false";
}

std::vector<std::vector<BoardItem>> initialiseBoardStructure() {
    std::vector<std::vector<BoardItem>> board;
    int emptyRowAmount = 4;

    board.push_back(generateMajorPieceRow(true));
    board.push_back(generatePawnRow(true));
    //board.push_back(generateEmptyRow());

    for (int i = 0; i < emptyRowAmount; i++) {
        board.push_back(generateEmptyRow());
    }

    //board.push_back(generateEmptyRow());
    board.push_back(generatePawnRow(false));
    board.push_back(generateMajorPieceRow(false));

    return board;
} 

std::vector<std::vector<BoardItem>> assignStartPositions(std::vector<std::vector<BoardItem>> board) {
    std::vector<std::string> xAxisLabels = {"a", "b", "c", "d", "e", "f", "g", "h"};
    std::vector<std::string> yAxisLabels = {"8", "7", "6", "5", "4", "3", "2", "1"}; //backwards to assign smallest from bottom to top


    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j].position = xAxisLabels[j] + yAxisLabels[i];
        }
    }

    return board;
}

std::pair<int, int> findIndexInVector(std::vector<std::vector<BoardItem>> board, std::string position) {
    std::pair<int, int> indices;
    indices.first = -1;
    indices.second = -1;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j].position == position) {
                indices.first = i; //y
                indices.second = j; //x

                return indices;
            }
        }
    }

    return indices;
}

bool validateLength(std::string input) {
    int acceptedLength = 4; // 2 chars each position, 1 char for whitespace

    if (input.size() == acceptedLength) {
        return true;
    } 

    return false;
}

bool validateNoMove(std::string input) {
    if (input.substr(0, 2) != input.substr(2, 2)) {
        return true;
    } 

    return false;
}

bool validateChars(std::string input) {
    std::smatch matchChars;
    std::regex charsExpression("[^0-8] [^a-h]");

    if (std::regex_search(input, matchChars, charsExpression)) {
        return false;
    }

    std::regex digitsExpression("[^0-8]");

    std::ptrdiff_t digitOccurences(std::distance(
        std::sregex_iterator(input.begin(), input.end(), digitsExpression),
        std::sregex_iterator())
    );

    std::regex lettersExpression("[^a-h]");

    std::ptrdiff_t lettersOccurences(std::distance(
        std::sregex_iterator(input.begin(), input.end(), lettersExpression),
        std::sregex_iterator())
    );
    
    if (digitOccurences > 2 || lettersOccurences > 2) {
        return false;
    }

    std::smatch matchDigits;

    std::string inputSecond = input.substr(1, 1);
    std::string inputFourth = input.substr(3, 1);

    if (std::regex_search(inputSecond, matchDigits, digitsExpression) || std::regex_search(inputFourth, matchDigits, digitsExpression)) {
        return false;
    }

    std::smatch matchLetters;
    
    std::string inputFirst = input.substr(0, 1);
    std::string inputThird = input.substr(2, 1);

    if (std::regex_search(inputFirst, matchLetters, lettersExpression) || std::regex_search(inputThird, matchLetters, lettersExpression)) {
        return false;
    }
    
    return true;
}

std::vector<std::vector<BoardItem>> swap(std::vector<std::vector<BoardItem>> board, std::string positionFrom, std::string positionTo) {
    std::pair<int, int> indexFrom = findIndexInVector(board, positionFrom);
    std::pair<int, int> indexTo = findIndexInVector(board, positionTo);
    BoardItem temp = board[indexTo.first][indexTo.second];
    
    board[indexTo.first][indexTo.second] = board[indexFrom.first][indexFrom.second];
    board[indexTo.first][indexTo.second].position = positionTo;

    board[indexFrom.first][indexFrom.second] = temp;
    board[indexFrom.first][indexFrom.second].position = positionFrom;

    return board;
}

std::vector<std::vector<BoardItem>> take(std::vector<std::vector<BoardItem>> board, std::string positionFrom, std::string positionTo) {
    std::pair<int, int> indexFrom = findIndexInVector(board, positionFrom);
    std::pair<int, int> indexTo = findIndexInVector(board, positionTo);
    BoardItem temp = board[indexTo.first][indexTo.second];
    
    BoardItem empty;
    empty.setName("EMTY");
    empty.setIcon();
    
    board[indexTo.first][indexTo.second] = board[indexFrom.first][indexFrom.second];
    board[indexTo.first][indexTo.second].position = positionTo;

    board[indexFrom.first][indexFrom.second] = empty;
    board[indexFrom.first][indexFrom.second].position = positionFrom;

    return board;
}

std::string lower(std::string toBeLowered) {
    std::string lowerString = "";
    std::smatch matchLetters;
    std::regex lettersExpression("[^A-H]");
    
    for (int i = 0; i < toBeLowered.size(); i++) {
        if (std::regex_search(toBeLowered, matchLetters, lettersExpression)) {
            char lowered = tolower(toBeLowered[i]);
            lowerString += lowered;
        }
        else {
            lowerString += toBeLowered[i];
        }
    }

    return lowerString;    
}

void clearAndDraw(std::vector<std::vector<BoardItem>> board) {
    system("clear");
    drawBoard(board);
}

bool coinToss() {
	int random;
    srand((unsigned)time(0)); //generates random seed
    random = (rand() % 2) + 1; //generates random int of 0 or 1

    if (random == 1)
        return true;
    
    return false;
}

int findLargest(std::pair<int, int> vector) {
    if ((std::abs(vector.first) == std::abs(vector.second)) || (std::abs(vector.first) > std::abs(vector.second))) {
        return std::abs(vector.first);
    }

    return std::abs(vector.second);
}

bool isPositive(int toFind) {
    if (toFind > 0) 
        return true;
    else
        return false;
}

bool stepThrough(std::pair<int, int> currentPosition, std::pair<int, int> desiredPosition, std::pair<int, int> vector, std::vector<std::vector<BoardItem>> board) {
    int largestAbsolute = findLargest(vector);
    int stepX = 0;
    int stepY = 0;

    /*
    out("largest: " + std::to_string(largestAbsolute) + "\n");
    out("***********\ncurrent position - x: " + std::to_string(currentPosition.second) + ", y: " + std::to_string(currentPosition.first) + "\n");
    out("***********\ndesired position - x: " + std::to_string(desiredPosition.second) + ", y: " + std::to_string(desiredPosition.first) + "\n");

    out("x: " + std::to_string(vector.first) + "\ny: " +  std::to_string(vector.second) + "\n");
    */
    
    for (int i = 0; i < largestAbsolute; i++) {
        if (vector.first != 0 && vector.first != stepX) {
            if (isPositive(vector.first)) 
                stepX ++;
            else 
                stepX --;
        }

        if (vector.second != 0 && vector.second != stepY) {
            if (isPositive(vector.second)) 
                stepY ++;
            else 
                stepY --;
        }

        /*
        out("\nstepX: " + std::to_string(stepX));
        out("\nstepY: " + std::to_string(stepY) + "\n");

        out("\nstepX + currentposition.second: " + std::to_string(currentPosition.second + stepX));
        out("\nstepY - currentposition.first: " + std::to_string(currentPosition.first - stepY) + "\n");
        */

        if (currentPosition.second + stepX != desiredPosition.second || currentPosition.first - stepY != desiredPosition.first) {
            //out("cuurent BI: " + board[currentPosition.first - stepY][currentPosition.second + stepX].name + "\n"); 
            if (board[currentPosition.first - stepY][currentPosition.second + stepX].name != "EMTY") {
                //out("not empty\n");
                return false;
            }
        }
        else if (currentPosition.second + stepX == desiredPosition.second && currentPosition.first - stepY == desiredPosition.first) {
            //out("valid\n");
            return true;
        }

        //c2c4 e2e4 f1d3 d1d5

        
    }
}

/*
                if (takenPieces[0].size() != 1) {
                    out(" You have taken:");

                    for (int i = 1; i < takenPieces[0].size(); i++) {
                        out(" " + takenPieces[0][i]);
                    }
                }

                if (takenPieces[1].size() != 1) {
                    out(" You have lost:");

                    for (int i = 1; i < takenPieces[1].size(); i++) {
                        out(" " + takenPieces[1][i]);
                    }
                }
*/

void outputTakes(bool isDark) {
    int index = (isDark) ? 0 : 1;

    if (takenPieces[index].size() != 1) {
        out(" You have taken:");

        for (int i = 1; i < takenPieces[index].size(); i++) {
            out(" " + takenPieces[index][i]);
        }
    }
}

void outputLosses(bool isDark) {
    int index = (isDark) ? 0 : 1;
    
    if (takenPieces[index].size() != 1) {
        out(" You have lost:");

        for (int i = 1; i < takenPieces[index].size(); i++) {
            out(" " + takenPieces[index][i]);
        }
    }
}

int main()
{
    bool isGameFinished = false;
    bool isTurnOver = false;
    bool isDarkTurn = coinToss();
    std::vector<std::vector<BoardItem>> board = initialiseBoardStructure();

    board = assignStartPositions(board);
    clearAndDraw(board);


    while(!isGameFinished) {
        isTurnOver = false;

        while(!isTurnOver) {
            if (isDarkTurn) {
                out("\nDark, it is your turn.");
                outputTakes(true);
                outputLosses(false);
            }
            else {
                out("\nLight, it is your turn.");
                outputTakes(false);
                outputLosses(true);
            }

            std::string move = lower(input("\nMake your move: "));
            bool isVectorValid = false;

            if (validateLength(move) && validateChars(move) && validateNoMove(move)) {
                std::string currentString = move.substr(0, 2);
                std::pair<int, int> currentIndices = findIndexInVector(board, currentString);
                BoardItem curr = board[currentIndices.first][currentIndices.second];

                std::string desiredString = move.substr(2, 2);
                std::pair<int, int> desiredIndices = findIndexInVector(board, desiredString);
                BoardItem des = board[desiredIndices.first][desiredIndices.second];

                if (curr.isDark == isDarkTurn) {
                    if (curr.name == "EMTY") {
                        clearAndDraw(board);
                        out("There is no piece on position '" + curr.position + "' to move to position '" + desiredString + "'");
                    }
                    else {
                        std::pair<int, int> vector = curr.createColumnVector(desiredString);
                        //out("x: " + std::to_string(vector.first) + ", y: " +  std::to_string(vector.second) + "\n");

                        if (curr.name == "KING") {
                            isVectorValid = curr.validateOneAround(vector);
                        }
                        else if (curr.name == "QUEN") {
                            if (curr.validateOneAround(vector) || curr.validateDiagonal(vector) || curr.validateStraight(vector))
                                isVectorValid = true;
                        }
                        else if (curr.name == "KNHT") {
                            isVectorValid = curr.validateJumps(vector);
                        }
                        else if (curr.name == "BSHP") {
                            isVectorValid = curr.validateDiagonal(vector);
                        }
                        else if (curr.name == "ROOK") {
                            isVectorValid = curr.validateStraight(vector);
                        }
                        else if (curr.name == "PAWN") {
                            isVectorValid = curr.validateStep(vector, curr.isDark);
                        }
                        
                        if (isVectorValid) {
                            if (stepThrough(currentIndices, desiredIndices, vector, board) || curr.name == "KNHT") {
                                //need to consider castling and eventually change this
                                if (des.name == "EMTY") {
                                    board = swap(board, currentString, desiredString);
                                    isTurnOver = true;
                                    isDarkTurn = !isDarkTurn;
                                    clearAndDraw(board);
                                }
                                else {
                                    if (des.isDark != isDarkTurn) {
                                        if (curr.isDark) 
                                            takenPieces[0].push_back(des.icon);
                                        else
                                            takenPieces[1].push_back(des.icon);

                                        board = take(board, currentString, desiredString);
                                        isTurnOver = true;
                                        isDarkTurn = !isDarkTurn;
                                        clearAndDraw(board);
                                    }
                                    else {
                                        clearAndDraw(board);
                                        out("you cannot take your own piece '" + des.icon + "'");
                                    }
                                }

                                //out("x: " + std::to_string(vector.first) + "\ny: " +  std::to_string(vector.second) + "\n");
                            }
                            else 
                                out("The path of the piece '" + curr.icon + "' on the way to '" + desiredString + "' is being blocked by another piece");
                        }
                        else {
                            clearAndDraw(board);
                            out("The move '" + move + "' is not part of the moveset of the board piece '" + curr.icon + " '");
                        }
                        
                        //out("x: " + std::to_string(vector.second) + "\ny: " +  std::to_string(vector.first) + "\n");
                    }
                }
                else {
                    clearAndDraw(board);
                    out("The piece on position '" + curr.position + "' is not your piece to move");
                }
            }
            else {
                clearAndDraw(board);
                out("Input is invalid, it should match the following pattern:\n<coordinate of piece to move><coordinate of where to move to>\nThe 2 coordinates must be different.\ne.g: d2g5\n");
            }
        }
    }
}

