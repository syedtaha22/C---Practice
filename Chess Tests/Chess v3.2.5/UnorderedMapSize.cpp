#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <utility>
using namespace std;


double CalculateSizeOfTranspositionTable(unordered_map<uint64_t, pair<float, int>> transpositionTable){
    size_t pairSize = sizeof(pair<const uint64_t, pair<float, int>>); // Size of each key-value pair in bytes
    size_t numPairs = transpositionTable.size(); // Total number of pairs in the map
    size_t totalPairSize = pairSize * numPairs; // Total size of pairs in bytes
    size_t overhead = sizeof(transpositionTable); // Overhead of the unordered_map itself
    double sizeInMB = static_cast<double>(totalPairSize + overhead) / (1024 * 1024); // Convert bytes to megabytes

    return sizeInMB;
}

int main() {
    // Define the unordered_map
    unordered_map<uint64_t, pair<float, int>> transpositionTable;
    cout << "Size of unordered_map: " << CalculateSizeOfTranspositionTable(transpositionTable) << " MB" << endl; // Output the size
    for(int i=0;i<1000000; i++){
            // Insert some elements into the map
        transpositionTable[i] = make_pair(i*23, i*i);


    }

    cout << "Size of unordered_map: " << CalculateSizeOfTranspositionTable(transpositionTable) << " MB" << endl; // Output the size


    return 0;
}
