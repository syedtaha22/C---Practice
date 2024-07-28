#include <iostream>


int calculatePlayerScore(int playerColor, int board, int Total_tiles) {
    int totalScore = 0;
    for (int i = 0; i < Total_tiles; i++) {
        if (board[i]->color == playerColor) {
            totalScore += board[i]->getScore();
            std::cout << totalScore << std::endl;
        }
    }
    return totalScore;
}