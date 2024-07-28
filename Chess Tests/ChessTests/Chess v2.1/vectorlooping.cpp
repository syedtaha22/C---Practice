#include <iostream>
#include <vector>
#include <utility> // for pair

using namespace std;

int main() {
    vector<pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // vertical and horizontal
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // diagonals
    };

    // Iterate over the first four elements
    for (int i = 0; i < 4; ++i) {
        // Accessing the pair elements using .first and .second
        cout << "Element " << i << ": (" << directions[i].first << ", " << directions[i].second << ")" << endl;

        // Perform operations on the elements here...
    }

    return 0;
}
