#include<iostream>
#include<string>
using namespace std;

string GetCastlingRights(string fen){

    size_t castlingRightsPos = fen.find_last_of(" ");
    if (castlingRightsPos != string::npos && castlingRightsPos + 1 < fen.length()) {
        string castlerights = fen.substr(castlingRightsPos + 1);
        return castlerights;
    }

    return "-1";
}


int main(){
    string CastleRights = GetCastlingRights("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qkq");
    cout << CastleRights;




    return 0;
}