#include<iostream>
using namespace std;
int player_move, selected_piece;

int arr[3][3] = {{1,2,3},
                 {4,5,6},
                 {7,8,9}};

void DisplayArray(){
    for(int i = 0; i < 3; i++ ){
        for(int j = 0; j < 3; j++ ){
        cout << arr[i][j] << " ";

        }
    cout << endl;

    }
    cout << endl;
}

void UpdateChessBoard(int piece_to_move, int new_location) {
    int selected_piece_row = piece_to_move /10;
    int selected_piece_column = piece_to_move % 10;
    int new_row = new_location /10;
    int new_column = new_location % 10;

	int temp = arr[selected_piece_row][selected_piece_column];
	arr[selected_piece_row][selected_piece_column] = arr[new_row][new_column];
	arr[new_row][new_column] = temp;


}

int main(){

    DisplayArray();

    cout << "Select Piece: ";
	cin >> selected_piece;
	cout << "Enter Move: ";
	cin >> player_move;
    UpdateChessBoard(selected_piece, player_move);
    DisplayArray();


   









    return 0;
}