#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#include "node.hpp"
#include "tree.hpp"
#include "utils.hpp"

/* ----- Create Local Helper Functions ----- */
enum Rotation {
    LL,
    LR,
    RR,
    RL,
    NONE
};

void print_helper(const unsigned int depth, Node* n);
void clear_helper(Node* n);
Node* insert_helper(const unsigned short int nodeVal, Node* n);
void LL_rotation(Node* child, Node* parent, Node* grandpa);
void LR_rotation(Node* child, Node* parent, Node* grandpa);
void RR_rotation(Node* child, Node* parent, Node* grandpa);
void RL_rotation(Node* child, Node* parent, Node* grandpa);
Rotation determine_rotation(Node* child, Node* parent, Node* grandpa);

/* ----- Create Tree Functions ----- */
Tree::Tree() { root = nullptr; }
Tree::~Tree() { TreeOperation::clear(this); }
void Tree::set_root(Node* r) { root = r; }
Node* Tree::get_root() { return root; }
void Tree::print() { print_helper(0, root); }


/* ----- Define Tree Operation Functions ----- */
void TreeOperation::insert(Tree* tree, const unsigned short int nodeVal) {
    // cases sourced from: https://www.geeksforgeeks.org/dsa/insertion-in-red-black-tree/
    
    // Child is nn
    Node* nn = insert_helper(nodeVal, tree->get_root());
    if (!nn) return;
    
    // Check if the child is the root
    if (nn == tree->get_root()) {
        nn->set_color(false);
        return;
    }
    
    // Check if the parent is root
    Node* parent = nn->get_pa();
    if (parent == tree->get_root()) {
        nn->set_color(true);
        return;
    }
    
    // Red Black Tree insert cases
    // If the uncle is red, change parent and uncle to black
    Node* uncle;
    Node* grandpa = parent->get_pa();
    if (nn->get_pa() == grandpa->get_c1() && grandpa->get_c2()) uncle = grandpa->get_c2();
    else if (grandpa->get_c1()) uncle = grandpa->get_c1();
    
    if (uncle->get_color() && uncle) {
        uncle->set_color(false);
        parent->set_color(false);
        
        // If the grandpa is not the root then set it to red
        if (grandpa != tree->get_root()) grandpa->set_color(false);
    }
    else if (uncle) {
        switch(determine_rotation(nn, parent, grandpa)) {
            case LL: {
                LL_rotation(nn, parent, grandpa);
                break;
            }
            case LR: {
                LR_rotation(nn, parent, grandpa);
                break;
            }
            case RR: {
                RR_rotation(nn, parent, grandpa);
                break;
            }
            case RL: {
                RL_rotation(nn, parent, grandpa);
                break;
            }
            default: {
                std::cout << "Invalid rotation.\n";
                break;
            }
        }
    }
}
void TreeOperation::insert_str(Tree* tree, const std::string& tokens) {
    std::istringstream iss(tokens);
    std::string token;
    unsigned short int num;
    while (std::getline(iss, token, ' ')) {
        if (is_number(token)) {
            num = std::stoi(token);
            if (num >= 1 && num <= 999) TreeOperation::insert(tree, num);
            else std::cout << "[inner-func] Number out of range (1-999): " << num << "\n";
        }
    }
}
bool TreeOperation::search(Tree* tree, const std::string& token) {
    // Check if the token is a number and if in range
    if (!is_number(token)) return false;
    unsigned short int num = std::stoi(token);
    if (num < 1 || num > 999) return false;
    
    // iterate through the tree to find the number
    Node* n = tree->get_root();
    while (n) {
        if (n->get_value() == num) return true;
        else if (num < n->get_value()) n = n->get_c1();
        else n = n->get_c2();
    }
    return false;
}
void TreeOperation::clear(Tree* tree) {
    // Get the root
    Node* root = tree->get_root();
    
    // If the root exists, clear the tree
    if (!root) return;
    clear_helper(root);
    
    // If the root exists, set the root to nullptr
    delete root;
    tree->set_root(nullptr);
}


/* ----- Define Local Helper Functions ----- */
Rotation determine_rotation(Node* child, Node* parent, Node* grandpa) {
    if (grandpa->get_c1() == parent && parent->get_c1() == child) return LL;
    if (grandpa->get_c1() == parent && parent->get_c2() == child) return LR;
    if (grandpa->get_c2() == parent && parent->get_c2() == child) return RR;
    if (grandpa->get_c2() == parent && parent->get_c1() == child) return RL;
    return NONE;
}
void LL_rotation(Node* child, Node* parent, Node* grandpa) {
    // Case 1: Left Left Case (LL rotation)
    // Right rotation of grandfather G
    // Then swap colors of G and Parent P
    // My own words: move G to the right (c2) and pull parent and child up
    //Node* T1 = child->get_c1();
    //Node* T2 = child->get_c2();
    Node* T3 = parent->get_c2();
    
    grandpa->set_c1(T3);
    T3->set_pa(grandpa);
    grandpa->set_color(true);
    
    parent->set_c2(grandpa);
    parent->set_pa(grandpa->get_pa());
    parent->set_color(false);
    
    grandpa->set_pa(parent);
}
void LR_rotation(Node* child, Node* parent, Node* grandpa) {
    // Case 2: Left Right Case (LR rotation)
    // Left rotation of Parent P
    // My own words: move parent to child C1 and move child to parent
    // then perform LL rotation
    //Node* T1 = parent->get_c1();
    Node* T2 = child->get_c1();
    //Node* T3 = child->get_c2();
    
    // Reassign parents
    child->set_pa(grandpa);
    parent->set_pa(child);
    
    // Swap children
    parent->set_c2(T2);
    T2->set_pa(parent);
    
    child->set_c1(parent);
    
    // Perform LL rotation
    LL_rotation(parent, child, grandpa);
}
void RR_rotation(Node* child, Node* parent, Node* grandpa) {
    // Case 3: Right Right Case (RR rotation)
    // My own words: move parent up, move grandpa to the right
    Node* T3 = parent->get_c1();
    
    // Reassign parents
    parent->set_pa(grandpa->get_pa());
    grandpa->set_pa(parent);
    
    // Swap children
    grandpa->set_c2(T3);
    T3->set_pa(grandpa);
    
    // Finish parent assignments
    parent->set_c1(grandpa);
}
void RL_rotation(Node* child, Node* parent, Node* grandpa) { 
    // Case 4: Right Left Case (RL rotation)
    // My own words: Swap parent and child and
    // then perform RR case
    Node* T4 = child->get_c2();
    
    // Reassign parents
    child->set_pa(grandpa);
    parent->set_pa(child);
    
    // Swap children
    parent->set_c2(T4);
    T4->set_pa(parent);
    
    child->set_c2(parent);
    
    // Perform RR
    RR_rotation(parent, child, grandpa);
    
    return; 
}
void print_helper(const unsigned int depth, Node* n) {
    if (!n) return;
    
    // Print the right side (represented as upper)
    print_helper(depth + 1, n->get_c2());
    
    // Indent based on current depth and print
    // the current node value
    std::cout << "[tree] " << std::string(depth * 4, ' ');
    printNode(n);
    
    // Print the left side (represented as lower)
    print_helper(depth + 1, n->get_c1());
}
void clear_helper(Node* n) {
    // If the node does not exist return
    if (!n) return;
    
    // Else if the children exist clear the children
    if (n) {
        if (n->get_c1()) clear_helper(n->get_c1());
        if (n->get_c2()) clear_helper(n->get_c2());
        delete n;
    }
}
Node* insert_helper(const unsigned short int nodeVal, Node* n) {
    // Case 1: child node is not found
    if (!n) return nullptr;
    
    // Case 2: node is not found, yet
    if (nodeVal < n->get_value()) {
        // Continue to iterate if child exists
        if (n->get_c1()) return insert_helper(nodeVal, n->get_c1());
        
        // Else create the new node
        Node* nn = new Node();
        nn->set_value(nodeVal);
        nn->set_pa(n);
        nn->set_color(true);
        
        // Connect it to the original node
        n->set_c1(nn);
        
        return nn;
    }
    else {
        // Continue to iterate if child exists
        if (n->get_c2()) return insert_helper(nodeVal, n->get_c2());
        
        // Else create the new node
        Node* nn = new Node();
        nn->set_value(nodeVal);
        nn->set_pa(n);
        nn->set_color(true);
        
        // Connect it to the original node
        n->set_c2(nn);
        
        return nn;
    }
    
    return nullptr;
}