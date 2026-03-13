#include <iostream> // i/o
#include <string> // strings
#include <limits> // for getline to utilize strings with spaces
#include <cctype> // for std::isdigit
#include <cstring> // for std::strcpy
#include <iomanip> // for std::setw

static inline int operator_precendence(const char c) {
    switch (c) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

static inline bool is_operator(const char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
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
        tokenValue = '\0';
        next = nullptr;
        left = nullptr;
        right = nullptr;
    }
    void set_next(Node* node) { next = node; }
    void set_left(Node* node) { left = node; }
    void set_right(Node* node) { right = node; }
    void set_token_value(const char op) { tokenValue = op; }
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
    ~Stack() {
        clear();
    }
    void push(const char tokenValue) {
        Node* newNode = new Node();
        newNode->set_token_value(tokenValue);
        newNode->set_next(head);
        head = newNode;
    }
    const char pop() {
        if (head != nullptr) {
            const char tokenValue = head->get_token_value();
            
            Node* temp = head;
            head = head->get_next();
            delete temp;

            return tokenValue;
        }
        return '\0';
    }
    const char peek() {
        if (head != nullptr) return head->get_token_value();
        return '\0';
    }
    Node* peek_node() {
        return head;
    }
    Node* steal_node() {
        Node* temp = head;
        head = head->get_next();

        temp->set_next(nullptr);
        return temp;
    }

    void clear() {
        while (peek() != '\0') pop();
    }
    void debug_repr() {
        std::cout << "\n[debug] Queue Result: ";

        Node* currentNode = head;
        while (currentNode != nullptr) {
            std::cout << currentNode->get_token_value() << " , ";
            currentNode = currentNode->get_next();
        }

        std::cout << "\n";
    }
};

class NodeStack {
    private:
    Node* head;

    public:
    NodeStack() {
        head = nullptr;
    }
    ~NodeStack() {
        clear();
    }

    void push(Node* newNode) {
        if (newNode == nullptr) return;
        newNode->set_next(head);
        head = newNode;
    }

    Node* pop() {
        if (head == nullptr) return nullptr;
        Node* temp = head;
        head = head->get_next();
        temp->set_next(nullptr);
        return temp;
    }

    Node* peek() {
        return head;
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = pop();
            delete temp;
        }
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
    void enqueue(const char tokenValue) {
        Node* newNode = new Node();
        newNode->set_token_value(tokenValue);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* currentNode = head;
            while (currentNode->get_next() != nullptr) {
                currentNode = currentNode->get_next();
            }
            currentNode->set_next(newNode);
        }
    }
    const char dequeue() {
        if (head != nullptr) {
            const char tokenValue = head->get_token_value();
            
            Node* temp = head;
            head = head->get_next();
            delete temp;

            return tokenValue;
        }
        return '\0';
    }

    void debug_repr() {
        std::cout << "\n[debug] Queue Result: ";

        Node* currentNode = head;
        while (currentNode != nullptr) {
            std::cout << currentNode->get_token_value() << " , ";
            currentNode = currentNode->get_next();
        }

        std::cout << "\n";
    }

    void clear() {
        while (head != nullptr) dequeue();
    }
};

/* ----- Initialize Binary Tree Class ----- */
class BinaryTree {
    private:
    Node* root; // haha get it its a tree

    // utility functions
    static inline bool shouldUseParanthesis(const char childOp, const char parentOp) {
        return (operator_precendence(parentOp) < operator_precendence(childOp));
    }

    void sub_represent(Node* currentNode, unsigned int depth) {
        if (currentNode == nullptr) return;

        // Print the right side (represented as upper)
        sub_represent(currentNode->get_right(), depth + 1);

        // Indent based on depth and print
        // the current node
        std::cout << "[tree]" << std::setw(depth * 4) << currentNode->get_token_value() << "\n";

        // Print left side (represented as lower)
        sub_represent(currentNode->get_left(), depth + 1);
    }
    void sub_clear(Node* head) {
        if (head == nullptr) return;
        sub_clear(head->get_left());
        sub_clear(head->get_right());
        delete head;
    }

    std::string infix_helper(Node* n) {
        if (!n) return "";

        // If the left node does not exist then
        // immediately return (end of iter)
        if (!n->get_left() && n->get_right()) return std::string(1, n->get_token_value());

        // Otherwise mash together the children 
        return infix_helper(n->get_left()) + " " + std::string(1, n->get_token_value()) + " " + infix_helper(n->get_right());
    }
    std::string prefix_helper(Node* n) {
        if (!n) return "";
        // return the current op combined with the left and right tokens
        return std::string(1, n->get_token_value()) + " " + prefix_helper(n->get_left()) + prefix_helper(n->get_right());
    }
    std::string postfix_helper(Node* n) {
        if (!n) return "";
        return postfix_helper(n->get_left()) + postfix_helper(n->get_right()) + " " + n->get_token_value();
    }

    public:
    BinaryTree() {
        root = nullptr;
    }
    ~BinaryTree() {
        clear();
        delete root;
    }

    void set_root(Node* rootptr) {
        root = rootptr;
    }
    void represent() {
        std::cout << "[] Expression Tree: \n";
        sub_represent(root, 0);
    }

    void represent_infix() {
        std::cout << "[infix] " << infix_helper(root) << "\n";
    }
    void represent_prefix() {
        std::cout << "[prefix] " << prefix_helper(root) << "\n";
    }
    void represent_postfix() {
        std::cout << "[postfix] " << postfix_helper(root) << "\n";
    }

    void clear() {
        sub_clear(root);
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
    NodeStack* resultantStack = new NodeStack();
    BinaryTree* expressionTree = new BinaryTree();

    // Define program variables
    std::string command; // user input

    // Define main loop variables
    bool running = true;
    do {
        // Grab input
        std::cout << "[] Enter expression to convert (or 'exit' to quit) > ";
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
        std::getline(std::cin, command); // read line

        // Check if the command is "exit" if not execute logic
        running = (command != "exit");
        if (running) {
            // Clear queues and stack for new run
            inputQueue->clear();
            outputQueue->clear();
            operatorStack->clear();
            resultantStack->clear();
            //expressionTree->clear();

            /* ----- Create Postfix Notation ------ */
            // Tokenize the string and process
            // the tokens into the queue
            unsigned int queueSize = 0;
            char token;
            for (unsigned int i = 0; i < command.length(); ++i) {
                token = command[i];
                if (!std::isspace(token)) {
                    inputQueue->enqueue(token);
                    ++queueSize;
                }
            }
            //inputQueue->debug_repr();
            
            // Process the tokens here
            // (algorithm implementation)
            char currentValue;
            for (unsigned int i = 0; i < queueSize; ++i) {
                currentValue = inputQueue->dequeue();

                // If the token is a number, add it to the output queue
                if (std::isdigit(currentValue)) {
                    outputQueue->enqueue(currentValue);
                }

                // if the token is not a number, lose your mind programming
                // if it is a left bracket push to stack
                else if (currentValue == '(') {
                    operatorStack->push(currentValue);
                }

                // if it is a right bracket
                else if (currentValue == ')') {
                    while (operatorStack->peek() != '(' && operatorStack->peek() != '\0') {
                        outputQueue->enqueue(operatorStack->pop());
                    }
                    if (operatorStack->peek() == '\0') {
                        std::cout << "[error] Mismatched paranthesis.\n";
                        return 1;
                    }
                    operatorStack->pop();
                }

                // if it is + - * / ^
                else if (is_operator(currentValue)) {
                    // while operator on stack with greater precendence
                    //   pop operator to queue
                    // then, add selected operator

                    // if the operator on top of the stack exists
                    // and the value is left associative and has less or equal precedence
                    // or the value is right associative and has less precedence
                    while (
                        operatorStack->peek() != '\0'
                        && is_operator(operatorStack->peek())
                        && (
                        currentValue != '^'
                            && operator_precendence(currentValue) <= operator_precendence(operatorStack->peek())
                        || currentValue == '^'
                            && operator_precendence(currentValue) < operator_precendence(operatorStack->peek())
                        )
                    ) {
                        outputQueue->enqueue(operatorStack->pop());
                    }
                    operatorStack->push(currentValue);
                }

                else {
                    std::cout << "[error] Unknown token.\n";
                    return 1;
                }
            }

            // After processing all the tokens flush the
            // remaining operators into the output queue
            while (operatorStack->peek() != '\0') {
                outputQueue->enqueue(operatorStack->pop());
            }

            //outputQueue->debug_repr();

            /* ----- Create Expression Tree ----- */
            // we use the queues/stack to backtrack to create
            // the binary expression tree
            for (unsigned int i = 0; i < queueSize; ++i) {
                operatorStack->debug_repr();
                currentValue = outputQueue->dequeue();
                if (currentValue == '\0') break;

                // if it is a number add to the stack
                if (std::isdigit(currentValue)) {
                    Node* newNode = new Node();
                    newNode->set_token_value(currentValue);
                    resultantStack->push(newNode);
                }

                // if it is an operator pop the last two
                // values and push onto the stack
                else if (is_operator(currentValue)) {
                    Node* right = resultantStack->pop();
                    Node* left = resultantStack->pop();

                    Node* parent = new Node();
                    parent->set_token_value(currentValue);
                    parent->set_left(left);
                    parent->set_right(right);

                    resultantStack->push(parent);
                }

                else {
                    std::cout << "[error] Unexpected token or something in postfix.\n";
                    return 1;
                }
            }

            expressionTree->set_root(resultantStack->pop());
            expressionTree->represent();

            /* ----- Allow user input for notation outputs ----- */
            bool thisExpression;
            do {
                std::cout << "[] Enter what notation you'd like your tree to print (infix, prefix, postfix) or \"next\" to move on. > ";
                std::getline(std::cin, command);

                thisExpression = (command != "next");
                if (thisExpression) {
                    if (command == "infix") {
                        // Handle infix
                        std::cout << "[] Infix result: \n";
                        expressionTree->represent_infix();
                    }
                    else if (command == "prefix") {
                        // Handle prefix
                        std::cout << "[] Prefix result: \n";
                        expressionTree->represent_prefix();
                    }
                    else if (command == "postfix") {
                        // Handle postfix
                        std::cout << "[] Postfix result: \n";
                        expressionTree->represent_postfix();
                    }
                }
            } while (thisExpression);
        }
    } while (running);

    // Deallocate memory for queues and stack
    delete inputQueue;
    delete operatorStack;
    delete outputQueue;
    delete expressionTree;
    delete resultantStack;
    return 0;
}
