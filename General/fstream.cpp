#include<iostream>
#include<fstream>
using namespace std;

int main(){
    ofstream MyFile("testfile.txt");
    MyFile << "This is a test file";
    MyFile.close();




}