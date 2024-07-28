#include<iostream>
#include "raylib.h"

const int boardSize = 8;
const int tileSize = 65;

Color lightSquare = { 238, 238, 210, 255 };
Color darkSquare = { 118, 150, 86, 255 };
Color Background = { 49, 46, 43, 255 };





struct ChessPiece {
    Texture2D texture;
    Rectangle rect;
    bool isDragged;
};

void DrawBoard() {
   
    for (int rank = 0; rank < boardSize; rank++) {
        for (int file = 0; file < boardSize; file++) {
            Color squareColor = ((rank + file) % 2 == 0) ? lightSquare : darkSquare;
            DrawRectangle(file * tileSize, rank * tileSize, tileSize, tileSize, squareColor);

        }
    }
}

ChessPiece InitChessPiece(const char* imagePath, int file, int rank) {
    ChessPiece piece;
    piece.texture = LoadTexture(imagePath);
    piece.rect.x = (file * tileSize);
    piece.rect.y =  (rank * tileSize);
    piece.rect.width = tileSize;
    piece.rect.height = tileSize;
    piece.isDragged = false;
    return piece;
}

void UpdateChessPiece(ChessPiece& piece, ChessPiece enemyPieces[]) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), piece.rect)) {
        piece.isDragged = true;
    }
    if (piece.isDragged) {
        piece.rect.x = GetMouseX() - piece.rect.width / 2;
        piece.rect.y = GetMouseY() - piece.rect.height / 2;
        if (piece.rect.x < 0) piece.rect.x = 0;
        if (piece.rect.y < 0) piece.rect.y = 0;
        if (piece.rect.x + piece.rect.width > GetScreenWidth()) piece.rect.x = GetScreenWidth() - piece.rect.width;
        if (piece.rect.y + piece.rect.height > GetScreenHeight()) piece.rect.y = GetScreenHeight() - piece.rect.height;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < 16; i++) {
                if (CheckCollisionPointRec(GetMousePosition(), enemyPieces[i].rect)) {
                    enemyPieces[i].rect.x = 1000;
                    enemyPieces[i].rect.y = 1000;


                }
            }
            piece.isDragged = false;
            piece.rect.x = static_cast<float>((static_cast<int>(piece.rect.x + piece.rect.width / 2) / tileSize) * tileSize);
            piece.rect.y = static_cast<float>((static_cast<int>(piece.rect.y + piece.rect.height / 2) / tileSize) * tileSize);

        }
    }
}


void DrawChessPiece(const ChessPiece& piece) {
    DrawTexture(piece.texture, static_cast<int>(piece.rect.x), static_cast<int>(piece.rect.y), WHITE);
}

int main() {
    const int screenWidth = 520;
    const int screenHeight = 520;
    InitWindow(screenWidth, screenHeight, "Chess");

    SetTargetFPS(60);

    ChessPiece whitePawns[8]{};
    ChessPiece blackPawns[8]{};
    ChessPiece whiteBishop[2]{};
    ChessPiece blackBishop[2]{};
    ChessPiece whiteRook[2]{};
    ChessPiece blackRook[2]{};
    ChessPiece whiteKing[1]{};
    ChessPiece blackKing[1]{};
    ChessPiece whiteKnight[2]{};
    ChessPiece blackKnight[2]{};
    ChessPiece whiteQueen[1]{};
    ChessPiece blackQueen[1]{};

    for (int i = 0; i < 8; i++) {
        whitePawns[i] = InitChessPiece("Resources\\WhitePieces\\wp.png", i, 6);
        blackPawns[i] = InitChessPiece("Resources\\BlackPieces\\bp.png", i, 1);
    }

    whiteBishop[0] = InitChessPiece("Resources\\WhitePieces\\wb.png", 2, 7);
    whiteBishop[1] = InitChessPiece("Resources\\WhitePieces\\wb.png", 5, 7);
    blackBishop[0] = InitChessPiece("Resources\\BlackPieces\\bb.png", 2, 0);
    blackBishop[1] = InitChessPiece("Resources\\BlackPieces\\bb.png", 5, 0);

    whiteRook[0] = InitChessPiece("Resources\\WhitePieces\\wr.png", 0, 7);
    whiteRook[1] = InitChessPiece("Resources\\WhitePieces\\wr.png", 7, 7);
    blackRook[0] = InitChessPiece("Resources\\BlackPieces\\br.png", 0, 0);
    blackRook[1] = InitChessPiece("Resources\\BlackPieces\\br.png", 7, 0);

    whiteKing[0] = InitChessPiece("Resources\\WhitePieces\\wk.png", 4, 7);
    blackKing[0] = InitChessPiece("Resources\\BlackPieces\\bk.png", 4, 0);

    whiteKnight[0] = InitChessPiece("Resources\\WhitePieces\\wn.png", 1, 7);
    whiteKnight[1] = InitChessPiece("Resources\\WhitePieces\\wn.png", 6, 7);
    blackKnight[0] = InitChessPiece("Resources\\BlackPieces\\bn.png", 1, 0);
    blackKnight[1] = InitChessPiece("Resources\\BlackPieces\\bn.png", 6, 0);

    whiteQueen[0] = InitChessPiece("Resources\\WhitePieces\\wq.png", 3, 7);
    blackQueen[0] = InitChessPiece("Resources\\BlackPieces\\bq.png", 3, 0);

    ChessPiece blackPieces[16] = { blackPawns[0],blackPawns[1],blackPawns[2],blackPawns[3],blackPawns[4],blackPawns[5],blackPawns[6],blackPawns[7],
                                  blackBishop[0],blackBishop[1],blackRook[0],blackRook[1],blackKnight[0],blackKnight[1], blackQueen[0],blackKing[0] };

    ChessPiece whitePieces[16] = { whitePawns[0],whitePawns[1],whitePawns[2],whitePawns[3],whitePawns[4],whitePawns[5],whitePawns[6],whitePawns[7],
                                  whiteBishop[0],whiteBishop[1],whiteRook[0],whiteRook[1],whiteKnight[0],whiteKnight[1], whiteQueen[0],whiteKing[0] };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Background);
        DrawBoard();
        for (int i = 0; i < 16; i++) {
            UpdateChessPiece(blackPieces[i], whitePieces);
            UpdateChessPiece(whitePieces[i], blackPieces);

            DrawChessPiece(blackPieces[i]);
            DrawChessPiece(whitePieces[i]);
        }
        EndDrawing();
    }
    for (int i = 0; i < 16; i++) {
        UnloadTexture(blackPieces[i].texture);
        UnloadTexture(whitePieces[i].texture);
    }
    CloseWindow();
    return 0;
}
