#include <iostream>
using namespace std;


class Queue{

    static const int MAX_SIZE = 100; 
    string arr[MAX_SIZE];
    int front, rear;

public:
    Queue(){
        front = -1;
        rear = -1;
    }


    void enqueue(string value){
        if(isFull()){
            cout << "Queue is Full, Can't Enqueue more elements" << endl;
            return;
        }

        if(front == -1) front++;
        rear++;
        arr[rear] = value;
        cout << "Element added to Queue: " << arr[rear] << endl;
        
    }

    void dequeue(){
        if(isEmpty()){
            cout << "Queue is Empty, Can't Dequeue elements" << endl;
            return;
        }

        //Check if element is last element of the Queue
        else if(front >= rear){
            cout << arr[front] << " has been removed from the Queue" << endl;
            front = -1;
            rear = -1;
        }
        else{
            cout << arr[front] << " has been removed from the Queue" << endl;
            front++;
        }   
    }

    string peek() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot peek." << endl;
            return "EMPTY";
        }
        return arr[front];
    }

    bool isEmpty(){
        return front == -1;
    }

    bool isFull(){
        return (front == 0 && rear == MAX_SIZE - 1);
    }

    void display(){
        cout << "Elements are: ";
        for(int i=front; i<=rear; i++){
            cout << arr[i] << " ";

        }
        cout << endl;
    }
};

class IntQueue{

    static const int MAX_SIZE = 100; 
    int arr[MAX_SIZE];
    int front, rear;

public:
    IntQueue(){
        front = -1;
        rear = -1;
    }


    void enqueue(int value){
        if(isFull()){
            cout << "Queue is Full, Can't Enqueue more elements" << endl;
            return;
        }

        if(front == -1) front++;
        rear++;
        arr[rear] = value;
        cout << "Element added to Queue: " << arr[rear] << endl;
        
    }

    void dequeue(){
        if(isEmpty()){
            cout << "Queue is Empty, Can't Dequeue elements" << endl;
            return;
        }

        //Check if element is last element of the Queue
        else if(front >= rear){
            cout << arr[front] << " has been removed from the Queue" << endl;
            front = -1;
            rear = -1;
        }
        else{
            cout << arr[front] << " has been removed from the Queue" << endl;
            front++;
        }   
    }

    int peek() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot peek." << endl;
            return -1;
        }
        return arr[front];
    }

    bool isEmpty(){
        return front == -1;
    }

    bool isFull(){
        return (front == 0 && rear == MAX_SIZE - 1);
    }

    void display(){
        cout << "Elements are: ";
        for(int i=front; i<=rear; i++){
            cout << arr[i] << " ";

        }
        cout << endl;
    }
    bool FindIndex(const int& index) const {
        for (int i = front; i <= rear; i++) {
            if (index == arr[i]) {
                return true;
            }

        }
         return false;

    }
};


int main(){

    Queue premoves;
    premoves.enqueue("e2e4");
    premoves.display();
    premoves.enqueue("f2f4");
    cout << premoves.peek();

    IntQueue index;
    for(int i=0;i<10; i++){
    index.enqueue(i+1);

    }
    index.display();
    cout << index.FindIndex(2) << endl;
    cout << index.FindIndex(20) << endl;






    return 0;
}