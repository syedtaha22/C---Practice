#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;

const string MOVES_FILE = "moves_data.txt";

// Function to store the first 10 moves made by the AI along with the FEN string
void StoreMoves(const string& fen, const string& move) {
    ofstream outfile(MOVES_FILE, ios::app);
    if (outfile.is_open()) {
        outfile << fen << " " << move << endl;
        outfile.close();
    } else {
        cerr << "Unable to open file for storing moves." << endl;
    }
}

// Function to load stored moves data into a vector<pair<string, string>>
vector<pair<string, string>> LoadMoves() {
    vector<pair<string, string>> moves;
    ifstream infile(MOVES_FILE);
    if (infile.is_open()) {
        string fen, move;
        while (infile >> fen >> move) {
            moves.push_back(make_pair(fen, move));
        }
        infile.close();
    } else {
        cerr << "Unable to open file for loading moves." << endl;
    }
    return moves;
}

// Function to check if the current FEN string is present in the loaded moves data
string GetMoveFromStoredData(const string& fen, const vector<pair<string, string>>& moves) {
    auto it = find_if(moves.begin(), moves.end(), [&](const pair<string, string>& p) {
        return p.first == fen;
    });
    if (it != moves.end()) {
        return it->second; // Return corresponding move if found
    }
    return ""; // Return empty string if FEN string not found
}

int main() {
    // Sample usage
    string currentFEN = "your_current_fen_string";
    string move = "your_generated_move";

    // Store the move in the file
    StoreMoves(currentFEN, move);

    // Load stored moves data
    vector<pair<string, string>> storedMoves = LoadMoves();

    // Check if the current FEN string is present in the loaded data
    string storedMove = GetMoveFromStoredData(currentFEN, storedMoves);
    if (!storedMove.empty()) {
        cout << "Move found in stored data: " << storedMove << endl;
    } else {
        cout << "Move not found in stored data. Continue with regular AI logic." << endl;
    }

    return 0;
}
