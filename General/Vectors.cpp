#include<iostream>
#include<vector>
using namespace std;


int main(){
    vector<int> v1 = {1,2,3,4}; // vector <type> name = {}

    // cout << v1.capacity() << endl; //Returns the Capacity of vector
    // cout << v1.size() << endl << endl; //Returns to total elements of the vector
    


    // v1.push_back(5); //Adds 9 to the end of vector#
    // cout << v1.capacity() << endl; 
    // cout << v1.size() << endl << endl;

    // v1.pop_back(); //Removes last element ----> Size changes, capacity does not
    // cout << v1.capacity() << endl; 
    // cout << v1.size() << endl << endl;

    // v1.shrink_to_fit(); //Reduces capacity to the Current size
    // cout << v1.capacity() << endl; 
    // cout << v1.size() << endl << endl;

    v1.insert(v1.begin(), 5);
    // cout << v1[0];
    for (int i = 0; i < v1.size();i++){
        cout << v1[i] << endl;
    }

    for(auto itr = v1.begin(); itr != v1.end(); itr++){
        cout << *itr << endl;
    }



}