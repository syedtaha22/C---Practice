#include<iostream>
#include<fstream>
#include<string>
using namespace std;




struct User_Data{
    string username;
    string name;
    string email;
    int number;

};

// User_Data Init_User_Data(string username, string name, string email, int number){
//     User_Data interim;
//     interim.username = username;
//     interim.name = name;
//     interim.email = email;
//     interim.number = number;
//     return interim;
// }


User_Data Get_User_Data(User_Data interim){
    cout << "Enter Your Username: ";
    cin >> interim.username;
    cout << "Enter Your Name: ";
    cin >> interim.name;
    cout << "Enter Your Email: ";
    cin >> interim.email;
    cout << "Enter Your Number: ";
    cin >> interim.number;
    return interim;
}


User_Data Save_User_Data(User_Data interim){
    ofstream User_Data("User_Data.txt");
    User_Data << "\n*******************************\n\n";
    User_Data << "Name: " << interim.name << "\n";
    User_Data << "Username: " << interim.username << "\n";
    User_Data << "Email: " << interim.email << "\n";
    User_Data << "Number: " << interim.number << "\n";
    User_Data.close();
}

int main(){
    while(true){
            User_Data user;
            user = Get_User_Data(user);
            Save_User_Data(user);
    }

}


        // char option;
        // cout << "Enter data in to Data Base(y/n)? ";
        // cin >> option;
        // if(option == 'y'){

        // }
        // else if(option == 'n'){
        //     break;
        // }
        // else{
        //     cout << "Invalid Option: Try Again\n";
        // }


