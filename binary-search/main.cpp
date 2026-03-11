#include <iostream>
#include <string>
#include <cctype>

// aditya c. said ai was A-OK for debugging!
// sophia wang said it was ok if i cheated on my entire thing
// p.s. sophia stop drawing over my incredible artistic masterpieces

/* ----- Create Utility Functions ----- */
bool is_number(const std::string& n);

/* ----- Create Command Enum ----- */
enum Command {
    PRINT,
    ADD,
    DELETE,
    QUIT,
    UNKNOWN
};
Command process_command(const std::string& input);


/* ----- Create Node Class ----- */
class Node {
    private:
    // Value is between 1 - 999
    unsigned short int val;
    Node* child1;
    Node* child2;
    
    public:
    Node();
    ~Node();
    
    // Setter functions
    void set_value(const unsigned short int v);
    void set_c1(Node* c); // Set the first child node
    void set_c2(Node* c); // Set the second child node
    
    // Getter functions
    const unsigned short int get_value();
    Node* get_c1();
    Node* get_c2();
};


/* ----- Create Binary Search Tree Class ----- */
class Tree {
    private:
    Node* root;
    void print_helper(const unsigned int depth, Node* cur);
    void add_helper(const unsigned short int val, Node* cur);
    bool search_helper(const unsigned short int tval, Node* cur);
    Node* delete_helper(const unsigned short int val, Node* cur);
    
    public:
    Tree();
    ~Tree();
    void set_root(Node* r);
    void print();
    void add(const unsigned short int val);
    void add(const std::string& val);
    bool search(const unsigned short int tval);
    void del(const std::string& val);
};


int main() {
    Tree tree;

    std::string command;
    Command result;
    bool running = true;
    do {
        std::cout << "[] Enter Command (add, print, delete, quit) > ";
        std::getline(std::cin, command);
        result = process_command(command);

        switch (result) {
            case PRINT: {
                tree.print();
                break;
            }
            case ADD: {
                std::cout << "[add] Enter number to add > ";
                std::getline(std::cin, command);
                tree.add(command);
                break;
            }
            case DELETE: {
                std::cout << "[del] Enter number to delete > ";
                std::getline(std::cin, command);
                tree.del(command);
                break;
            }
            case QUIT: {
                running = false;
                break;
            }
            default: break;
        }
    } while (running);

    return 0;
}

/* ----- Define Utility Functions ----- */
bool is_number(const std::string& n) {
    if (n.empty()) return false;
    for (int i = 0; i < n.length(); ++i) if (!std::isdigit(n[i])) return false;
    return true;
}

/* ----- Define Command Functions ----- */
Command process_command(const std::string& input) {
    if (input == "print") return PRINT;
    if (input == "add") return ADD;
    if (input == "delete") return DELETE;
    if (input == "quit") return QUIT;
    return UNKNOWN; // TODO: implement unknown
}

/* ----- Define Node Class Functions ----- */
Node::Node() {
    val = 0;
    child1 = nullptr;
    child2 = nullptr;
}
Node::~Node() {
    delete child1;
    delete child2;
}
void Node::set_value(const unsigned short int v) { val = v; }
void Node::set_c1(Node* c) { child1 = c; }
void Node::set_c2(Node* c) { child2 = c; }
const unsigned short int Node::get_value() { return val; }
Node* Node::get_c1() { return child1; }
Node* Node::get_c2() { return child2; }

/* ----- Define Binary Search Tree Functions ----- */
Tree::Tree() { root = nullptr; }
Tree::~Tree() { delete root; }
void Tree::set_root(Node* r) { root = r; }
void Tree::print() { print_helper(0, root); }
void Tree::print_helper(const unsigned int depth, Node* cur) {
    if (!cur) return;

    // Print the right side (represented as upper)
    print_helper(depth + 1, cur->get_c2());

    // Indent based on current depth and print
    // the current node value
    std::cout << "[tree] " << std::string(depth * 4, ' ') << cur->get_value() << "\n";

    // Print the left side (represented as lower)
    print_helper(depth + 1, cur->get_c1());
}
void Tree::add(const unsigned short int val) { 
    if (val < 1 || val > 999) return;
    if (search(val)) return;

    // handle empty tree
    if (root == nullptr) {
        root = new Node();
        root->set_value(val);
        return;
    }

    add_helper(val, root); 
}
void Tree::add(const std::string& val) {
    if (!is_number(val)) return; // and out of range
    const unsigned short int n = static_cast<unsigned short int>(std::stoul(val));
    add(n);
}
void Tree::add_helper(const unsigned short int val, Node* cur) {
    Node* n = new Node();
    n->set_value(val);

    // If lesser
    if (val < cur->get_value()) {
        if (cur->get_c1() == nullptr) {
            cur->set_c1(n);
            return;
        }
        else this->add_helper(val, cur->get_c1());
    }

    // If greater
    if (val > cur->get_value()) {
        if (cur->get_c2() == nullptr) {
            cur->set_c2(n);
            return;
        }
        else this->add_helper(val, cur->get_c2());
    }

    delete n;
    return;
}
bool Tree::search(const unsigned short int tval) { return search_helper(tval, root); }
bool Tree::search_helper(const unsigned short int tval, Node* cur) {
    // If the tree has nothing in it its not there
    if (cur == nullptr) return false;

    // If the value is there return true
    if (cur->get_value() == tval) return true;

    // If not, check to see which side to iterate down on
    if (tval < cur->get_value()) return search_helper(tval, cur->get_c1());
    else return search_helper(tval, cur->get_c2());
}
void Tree::del(const std::string& val) {
    if (!is_number(val)) return; // and out of range
    const unsigned short int n = static_cast<unsigned short int>(std::stoul(val));
    
    // Confirm it is in the tree
    if (!search(n)) return;
    root = delete_helper(n, root);
}
Node* Tree::delete_helper(const unsigned short int val, Node* cur) {
    // Recursively iterate through the tree
    if (!cur) return nullptr;

    // If the value is lesser iterate to the left
    if (val < cur->get_value()) cur->set_c1(delete_helper(val, cur->get_c1()));

    // If the value is greater iterate to the right
    if (val > cur->get_value()) cur->set_c2(delete_helper(val, cur->get_c2()));

    // If the value is found perform actions
    // for each of the 3 cases

    if (val == cur->get_value()) {
        // Case 1: no children
        if (!cur->get_c1() && !cur->get_c2()) {
            delete cur;
            return nullptr;
        }

        // Case 2: has 1 child
        // (left child exists)
        else if (!cur->get_c2()) {
            Node* t = cur->get_c1();
            cur->set_c1(nullptr);
            delete cur;
            return t;
        }

        else if (!cur->get_c1()) {
            Node* t = cur->get_c2();
            cur->set_c2(nullptr);
            delete cur;
            return t;
        }

        // Case 3: has 2 children
        else {
            // Rightmost node is greater than left so will replace
            Node* t = cur->get_c2();

            // Find the smallest value in the right subtree
            while (t->get_c1()) t = t->get_c1();

            // Set the current node with the new value
            cur->set_value(t->get_value());

            // Delete duplicate node from right tree
            cur->set_c2(delete_helper(t->get_value(), cur->get_c2()));
            return cur;
        }
    }

    return cur;
}
