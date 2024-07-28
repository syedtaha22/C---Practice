#include <iostream>

// Define a Node structure
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

Node* insertAtBeginning(Node* head, int value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    return newNode;
}

void display(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

int main() {

    Node* myList = nullptr;
    myList = insertAtBeginning(myList, 3);
    myList = insertAtBeginning(myList, 2);
    myList = insertAtBeginning(myList, 1);
    display(myList);

    return 0;
}
