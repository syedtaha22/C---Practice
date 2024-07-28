import chess

# Create a chess board
board = chess.Board("2kr1r2/pp6/2n4p/5pp1/1P2p3/Pq1nP1PP/1BR1QPBN/1K1R4 b")

# Get all legal moves
legal_moves = list(board.legal_moves)

# Get the number of possible moves
num_possible_moves = len(legal_moves)

print("Number of possible moves in the current position:", num_possible_moves)
