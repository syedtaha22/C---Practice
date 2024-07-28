#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

// Define a class for TrieNode
class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    int value;  // For minimax value
    // Constructor
    TrieNode() : value(0) {}
};

// Define Trie class
class Trie {
public:
    TrieNode* root;

    // Constructor
    Trie() {
        root = new TrieNode();
    }

    // Function to insert a node into the trie
    void insert(string word, int value) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->value = value;
    }

    // Function to generate DOT representation of the trie
    void generateDot(const string& filename) {
        ofstream dotFile(filename);
        if (!dotFile) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        dotFile << "digraph Trie {" << endl;
        dotFile << "node [shape=circle];" << endl;

        generateDotRec(root, "", dotFile);

        dotFile << "}" << endl;
        dotFile.close();
    }

private:
    // Recursive function to generate DOT
    void generateDotRec(TrieNode* node, const string& prefix, ofstream& dotFile) {
        if (node) {
            for (const auto& pair : node->children) {
                dotFile << '"' << prefix << "\" -> \"" << prefix + pair.first << "\" [label=\"" << pair.first << "\"];" << endl;
                generateDotRec(pair.second, prefix + pair.first, dotFile);
            }
        }
    }
};

// Example usage
int main() {
    Trie trie;

    // Inserting nodes with some values
    trie.insert("node1", 5);
    trie.insert("node2", 10);
    trie.insert("node3", 7);

    // Generate DOT representation and store as PNG
    trie.generateDot("trie.dot");

    // Convert DOT file to PNG using Graphviz
    system("dot -Tpng trie.dot -o trie.png");

    return 0;
}
