// Type Conversion:
//     1) Implicit ----> Automatically done by Complier
//     2) Explicit ----> Type Casting 
//         I)  C-style casting
//         II) Casting by Keyword
//             a) static_cast
//             b) dynamic_cast
//             c) const_cast
//             d) reinterpret_cast


// 2I) C-style Casting:  (datatype) variable

// #include<iostream>
// using namespace std;

// int main(){
//     // char char1 = 'A'; //ASCII ---> A = 65
//     // cout << (int) char1 << endl;

//     cout << endl << "ASCII table: " << endl;
//     for(int i = 0; i<=256; i++){
//         cout << i << " | " << (char) i << endl;
//     }

//     // cout << (char) 27;
//     return 0;
// }

// 2IIa) static_cast: static_cast<datatype> (variable)

#include<iostream>
using namespace std;

int main(){                                                 
    // int num1 = 20;
    // char char1 = static_cast<char>(num1);
    // cout << char1;

    cout << endl << "ASCII table: " << endl;
    for(int i = 0; i<= 256; i++){
        cout << i << " | " << static_cast<char>(i) << endl;
    }




}
