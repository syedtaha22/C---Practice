#pragma once
#include<iostream>
#include <chrono>
#include<vector>
#include<cmath>
#include<string>

using namespace std;
using namespace std::chrono;

const char EMPTY = ' ';
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';



const int Total_tiles = 64;
enum class PromotionPiece { QUEEN, ROOK, KNIGHT, BISHOP };