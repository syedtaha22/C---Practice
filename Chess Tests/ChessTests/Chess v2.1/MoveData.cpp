// #include <vector>
// #include <cmath>

// class ChessBoard {
//     // Your existing code...

// public:
//     // Function to precompute possible moves for sliding pieces
//     void precomputeSlidingMoves(int pieceIndex, vector<int>& possibleMoves) const {
//         ChessPiece piece = board[pieceIndex];

//         int row = pieceIndex / 8;
//         int col = pieceIndex % 8;

//         // Directions for sliding pieces: up, down, left, right, diagonals
//         vector<pair<int, int>> directions = {
//             {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // vertical and horizontal
//             {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // diagonals
//         };

//         for (const auto& dir : directions) {
//             int dRow = dir.first;
//             int dCol = dir.second;

//             int newRow = row + dRow;
//             int newCol = col + dCol;

//             while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
//                 int newIndex = newRow * 8 + newCol;
//                 if (board[newIndex].type == EMPTY) {
//                     possibleMoves.push_back(newIndex);
//                 } else {
//                     // Stop searching in this direction if we encounter a piece
//                     break;
//                 }
//                 newRow += dRow;
//                 newCol += dCol;
//             }
//         }
//     }
// };
