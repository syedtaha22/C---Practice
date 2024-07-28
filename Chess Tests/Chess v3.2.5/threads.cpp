#include <iostream>
#include <thread>
#include <string>
using namespace std;

string generateString(string &s)
{
    s += "e2e4";
    return s;
}

string StringMaker(string &str)
{

    generateString(str);
    return str;
}

int main()
{
    string str;
    int i = 0;
    while (i <= 2)
    {
        std::thread stringThread(StringMaker, std::ref(str));
        stringThread.detach();
        cout << str << endl;
        str = "";

        i++;
    }

    return 0;
}