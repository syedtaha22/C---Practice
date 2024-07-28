#include<iostream>
#include<unordered_map>
#include<typeinfo>
#include <chrono>

using namespace std;
using namespace std::chrono;

int number_of_rows = 8;
int number_of_files = 8;
string player_move;
unordered_map<char, int> file_map = { {'a', 0},
									  {'b', 1},
									  {'c', 2},
									  {'d', 3},
									  {'e', 4},
									  {'f', 5},
									  {'g', 6},
									  {'h', 7} };


string tiles[8][8] = { {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"},
					   {"[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]","[ ]"} };

char getKeyFromValue(const std::unordered_map<char, int>& my_unordered_map, int value) {
	for (const auto& pair : my_unordered_map) {
		if (pair.second == value) {
			return pair.first;
		}
	}
	return '0';
}

void DisplayBoard() {
	for (int i = 0; i < number_of_rows; i++) {
		cout << 8 - (i)  << " ";
		for (int j = 0; j < number_of_files; j++) {
			cout << tiles[i][j];
		}
		cout << endl;
	}
	cout << "   a  b  c  d  e  f  g  h" << endl;
}

void UpdateChessBoard(string move) {

	int piece_row = 8 - (move[1] - '0');
	int piece_file = file_map[move[0]];
	int move_to_row = 8 - (move[3] - '0');
	int move_to_file = file_map[move[2]];
	string temp = tiles[piece_row][piece_file];
	tiles[piece_row][piece_file] = tiles[move_to_row][move_to_file];
	tiles[move_to_row][move_to_file] = temp;

}
bool Is_Invalid(string move) {
	if (move.length() == 4 || move[0] >= 'a' || move[0] <= 'h'|| move[1] >= '1' || move[1] <= '8' /*|| move[2] >= 'a' || move[2] <= 'h' || move[3] >= '1' || move[3] <= '8'*/) {
		int piece_row = 8 - (move[1] - '0');
		int piece_file = file_map[move[0]];
		int move_to_row = 8 - (move[3] - '0');
		int move_to_file = file_map[move[2]];
		cout << "Selected Square: " << tiles[piece_row][piece_file] << endl;
		if (tiles[piece_row][piece_file] != "[ ]") {
			return false;
		}
		else {
			cout << "Square is Empty, ";
			return true;
		}
	}
	cout << "Invalid Peice, ";
	return true;
}

void GetUserMove() {
	cout << "Enter your move: ";
	cin >> player_move;
	while (Is_Invalid(player_move)) {
		cout << "Try again: ";
		cin >> player_move;
	}
}


class ChessPiece {
	string Piece_type = "";
	bool Is_white = false; 
	int Rank = 0 , File = 0;
public:
	ChessPiece(){} //default constructer
	ChessPiece(string piece_type, bool is_white, int row, int column, string symbol) {
		Piece_type = piece_type;
		Is_white = is_white;
		Rank = row;
		File = column;
		tiles[row][column] = "[" + symbol + "]";
	}

	void DisplayInfo() {
		cout << "Piece Type: " << Piece_type << endl;
		string peice_color = (Is_white) ? "white" : "black";
		cout << "Piece Color: " << peice_color << endl;
		cout << "Peice Position: " << 8 - Rank << ", " <<  getKeyFromValue(file_map, File)<< endl;
	}

};

class Pawn : public ChessPiece {
public:
	Pawn(){} //Default Constructer
	Pawn(string piece_type, bool is_white, int row, int column): 
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "P" : "p") {}

};

class Rook : public ChessPiece {
public:
	Rook() {} //Default Constructer
	Rook(string piece_type, bool is_white, int row, int column) :
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "R" : "r") {}
};

class Bishop : public ChessPiece {
public:
	Bishop() {} //Default Constructer
	Bishop(string piece_type, bool is_white, int row, int column) :
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "B" : "b") {}
};

class Knight : public ChessPiece {
public:
	Knight() {} //Default Constructer
	Knight(string piece_type, bool is_white, int row, int column) :
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "N" : "n") {}
};

class King : public ChessPiece {
public:
	King() {} //Default Constructer
	King(string piece_type, bool is_white, int row, int column) :
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "K" : "k") {}
};

class Queen : public ChessPiece {
public:
	Queen() {} //Default Constructer
	Queen(string piece_type, bool is_white, int row, int column) :
		ChessPiece(piece_type, is_white, row, column, (is_white) ? "Q" : "q") {}
};

int main() {
	auto start = high_resolution_clock::now();

	//Initialise White Pieces
	Pawn white_pawns[8];
	Rook white_rooks[2] = { Rook("Rook", true, 7,0), Rook("Rook", true, 7,7) };
	Knight white_knights[2] = { Knight("Knight", true, 7,1),Knight("Knight", true, 7,6) };
	Bishop white_bishops[2] = { Bishop("Bishop", true, 7,2),Bishop("Bishop", true, 7,5) };
	Queen white_queen = Queen("Queen", true, 7, 3);
	King white_king = King("king", true, 7, 4);

	//Initialise Black Pieces
	Pawn black_pawns[8];
	Rook black_rooks[2] = { Rook("Rook", false, 0,0),Rook("Rook", false, 0,7) };
	Knight black_knights[2] = { Knight("Knight", false, 0,1),Knight("Knight", false, 0,6) };
	Bishop black_bishops[2] = { Bishop("Bishop", false, 0,2),Bishop("Bishop", false, 0,5) };
	Queen black_queen = Queen("Queen", false, 0, 3);
	King black_king = King("king", false, 0, 4);

	for (int column = 0; column < 8; column++) {
		white_pawns[column] = Pawn("pawn", true, 6, column);
		black_pawns[column] = Pawn("pawn", false, 1, column);
	}
	DisplayBoard();

	//Whites move
	// GetUserMove();
	UpdateChessBoard(player_move);
	DisplayBoard();

	// Blacks Move
	GetUserMove();
	UpdateChessBoard(player_move);
	DisplayBoard();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Time taken to execute main function: " << duration.count() << " milliseconds" << endl;



	return 0;
}

/* 
* Add Captre Logic
* Implement Piece Movement Rules
* Implement Checks and Check Mates
* Implement En passant
* Implement Player turns
* Implement Better Chess Notation 
* Implement other rules
* Implement Chess AI
* Add GUI
* Exception Handling





*/