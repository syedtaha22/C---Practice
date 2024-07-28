#include <iostream>
#include <iomanip>

void printFirstColumnAndLastRow(const int board[64]) {
    // Print the first column
    std::cout << "First column:" << std::endl;
    for (int i = 0; i < 64; i += 8) {
        std::cout << board[i] << " ";  // Accessing the first column of each row
    }
    std::cout << std::endl;

    // Print the last row
    std::cout << "Last row:" << std::endl;
    for (int i = 56; i < 64; ++i) {
        std::cout << board[i] << " ";  // Accessing the elements of the last row
    }
    std::cout << std::endl;
}

void print(const int board[64]){
    for(int i =0; i<64; i++){
       std::cout << std::setw(2) << board[i] << " ";
        if((i+1)%8==0 && i >1 ) std::cout << std::endl;
    }

    std::cout << std::endl;
}


int main(){
    int board[64];
    
    for(int i = 0; i < 64; i++){
        board[i] = i;
    }

    print(board);

    printFirstColumnAndLastRow(board);





}