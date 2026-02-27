#include <iostream> // i/o
#include <string> // strings
#include <limits> // for getline to utilize strings with spaces
#include <cctype> // for std::isdigit
#include <cstring> // for std::strcpy

// "in" c++ equivalent
static inline int get_arr_index()
{
    /*
    loop thru array
    if found in the array return the index
    if not found in the array return -1
    */
    return 0;
}


/* ----- Initialize Linked List Class ----- */
class Node {
    private:
    char tokenValue;
    Node* next;
    Node* left;
    Node* right;

    public:
    Node() {
        next = nullptr;
        left = nullptr;
        right = nullptr;
    }
    void set_next(Node* node) { next = node; }
    void set_left(Node* node) { left = node; }
    void set_right(Node* node) { right = node; }
    void set_token_value(char op) { tokenValue = op; }
    Node* get_next() { return next; }
    Node* get_left() { return left; }
    Node* get_right() { return right; }
    char get_token_value() { return tokenValue; }
};


/* ----- Initialize Stack Class ----- */
// even though std::stack is a real thing and will work the SAME stupid way
// implemented using https://www.w3schools.com/cpp/cpp_stacks.asp
class Stack {
    private:
    Node* head;

    public:
    Stack() {
        head = nullptr;
    }
    void push(Node* node) {
        node->set_next(head);
        head = node;
    }
    void pop() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->get_next();
            delete temp;
        }
    }
    Node* peek() {
        if (head != nullptr) return head;
        return nullptr;
    }
};

/* ----- Initialize Queue Class ----- */
// once again, std::queue is a real thing and will work the SAME stupid way
// implemented using https://www.w3schools.com/cpp/cpp_queues.asp
// and https://stackoverflow.com/questions/16433397/difference-between-enqueue-and-dequeue
class Queue {
    private:
    Node* head;

    public:
    Queue() {
        head = nullptr;
    }
    void enqueue(Node* node) {
        if (head == nullptr) {
            head = node;
        }
        else {
            Node* currentNode = head;
            while (currentNode->get_next() != nullptr) {
                if (currentNode->get_next() == nullptr) {
                    currentNode->set_next(node);
                    return;
                }
                currentNode = currentNode->get_next();
            }
        }
    }
    Node* dequeue() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->get_next();
            return temp;
            // might want to not return and just straight up delete
            // if that is what is ended up being needed
        }
        return nullptr;
    }
};

/* ----- Initialize Binary Tree Class ----- */
class BinaryTree {
    private:
    Node* root; // haha get it its a tree

    public:
    BinaryTree() {
        root = nullptr;
    }
};


/* ----- Notes ----- */
// right side is queue
// bottom is stack
// left side is queue
int main() {
    // Allocate memory for queues and stack
    Queue* inputQueue = new Queue();
    Stack* operatorStack = new Stack();
    Queue* outputQueue = new Queue();
    BinaryTree* expressionTree = new BinaryTree();

    // Define program variables
    std::string command; // user input

    // Define main loop variables
    bool running = true;
    do {
        // Grab input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
        std::getline(std::cin, command); // read line

        // Check if the command is "exit" if not execute logic
        running = (command != "exit");
        if (running) {
            // Process the command here

            // Tokenize the string and process
            // the tokens into the queue
            char token;
            for (unsigned int i = 0; i < command.length(); ++i) {
                token = command[i];
                if (!std::isspace(token)) {
                    Node* tokenNode = new Node();
                    tokenNode->set_token_value(token);
                    inputQueue->enqueue(tokenNode);
                }
            }
            
            // Process the tokens here
            // (algorithm implementation)
            Node* tempNode;
            for (unsigned int i = 0; i < command.length(); ++i) {
                tempNode = inputQueue->dequeue();

                // If the token is a number, add it to the output queue
                if (std::isdigit(tempNode->get_token_value())) {
                    outputQueue->enqueue(tempNode);
                }

                // if the token is not a number, lose your mind programming
                // and then execute logic
                else {
                    // TODO: implement here
                }
            }

        }
    } while (running);

    // Deallocate memory for queues and stack
    delete inputQueue;
    delete operatorStack;
    delete outputQueue;
    delete expressionTree;
    return 0;
}