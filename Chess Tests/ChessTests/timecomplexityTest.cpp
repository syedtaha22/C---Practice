#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    auto start = high_resolution_clock::now();
    // Your main function code here
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << fixed << setprecision(32) << "\n\nTime taken to execute main function: " << duration.count() * 1000.0 << " milliseconds" << endl;

    return 0;
}
