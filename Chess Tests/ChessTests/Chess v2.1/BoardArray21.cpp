#include<iostream>
using namespace std;


int main(){

    int array[64];
    for (int i = 0; i < 64; i++){
        array[i] = i;
    }

    for (int i = 0; i < 64; i++){
        cout <<  "[ " << array[i] << " ]" << " ";
        if((i+1)%8 == 0){
            cout << endl;
        }
    }





    return 0;
}