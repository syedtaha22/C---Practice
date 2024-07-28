#include<iostream>
#include<string>
using namespace std;

string alphabets = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void iterater(string message){
    string result = "";
    for(int i = 0; i < message.length(); i++){
        for(int j = 0; j < alphabets.length(); j++){
            result += alphabets[j];
            cout << result << endl;
            if (alphabets[j] != message[i]){
               result.erase(result.length() - 1,1);
            }
            else{
                break;
            }
        }
    }
}

int main(){
    string user_string = "";
    cout << "What would you like to iterate over? ";
    getline(cin, user_string);
    iterater(user_string);
}