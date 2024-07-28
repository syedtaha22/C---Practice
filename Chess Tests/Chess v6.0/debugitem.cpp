#include <iostream>
#include <vector>
#include <type_traits>

const int Total_tiles = 64; // Assuming a standard chess board size

class ChessPiece {
public:
    std::string type; // Example member variable to represent the type of chess piece
};

class DebugItem {
    // Base case function to print the last argument
    static void print() {
        std::cout << std::endl;
    }



public:
    // Display Vector on Terminal
    static void moves(const std::vector<std::string>& moves) {
        for (const auto& move : moves) { 
            std::cout << move << " "; 
        }
        std::cout << std::endl;
    }

    // Display Board on Terminal
    static void board(ChessPiece* board[]) {
        std::cout << "8 ";
        for (int i = 0; i < Total_tiles; i++) {
            std::cout << "[" << board[i]->type << "]";
            if ((i + 1) % 8 == 0 && (i + 1) < Total_tiles) {
                std::cout << std::endl;
                std::cout << 8 - (i + 1) / 8 << " ";
            }
        }
        std::cout << std::endl;
        std::cout << "   a  b  c  d  e  f  g  h" << std::endl;
    }

    // Wrapper function to allow usage with std::cout
    // Recursive variadic template function to print arguments
    template<typename T, typename... Args>
    static void print(const T& firstArg, const Args&... args) {
        std::cout << firstArg << " ";
        print(args...); // Recur for the remaining arguments
    }

};

class ChessDebugger {

    // Base case function to print the last argument
    void print_args(std::ostream& os) {
        os << std::endl;
    }

    // Recursive variadic template function to print arguments
    template<typename T, typename... Args>
    void print_args(std::ostream& os, const T& firstArg, const Args&... args) {
        os << firstArg << " ";
        print_args(os, args...); // Recur for the remaining arguments
    }

public:

    //Display Vector on Terminal
    static void moves(const std::vector<std::string>& mvs) {
        for (const auto& move : mvs) { std::cout << move << " "; }
        std::cout << std::endl;
    }


    // Wrapper function to allow usage with std::cout
    template<typename... Args>
    void operator()(Args&&... args) {
        print_args(std::cout, std::forward<Args>(args)...);
    }

};



int main() {

    // Example to show vector printing
    std::vector<std::string> moves = {"e4", "e5", "Nf3", "Nc6"};
    ChessDebugger::moves(moves);

    // Example to show board printing
    ChessPiece* board[Total_tiles];
    for (int i = 0; i < Total_tiles; ++i) {
        board[i] = new ChessPiece();
        board[i]->type = (i % 2 == 0) ? "W" : "B"; // Just an example, set piece types
    }
    ChessDebugger()(23);

    ChessDebugger()(1,2,4,5);

    // Clean up
    for (int i = 0; i < Total_tiles; ++i) {
        delete board[i];
    }

    return 0;
}
