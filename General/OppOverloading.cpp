#include<iostream>
using namespace std;

//This is One Way

// struct Point {
//     int x;
//     int y;

//     Point operator*(Point& after){
//         Point result;
//         result.x = this->x * after.x;
//         result.y = this->y * after.y;
//         return result;
//     }
// };

// int main() {
//     Point point1 = {1, 3};
//     Point point2 = {1, 2};
//     Point point3 = {1, 4};
//     Point result = point1 * point2 * point3;

//     cout << result.x << " " << result.y;
//     return 0;
// }

//This is another way

struct Point {
    int x;
    int y;
};

Point operator*(Point point1, Point point2){
    Point result;
    result.x = point1.x * point2.x;
    result.y = point1.y * point2.y;
    return result;
}

int main() {
    Point point1 = {1, 3};
    Point point2 = {1, 2};
    Point point3 = {1, 4};
    Point result = point1 * point2 * point3;
    cout << result.x << " " << result.y;
    return 0;
}
