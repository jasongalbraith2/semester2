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
void Tree::print() { 
    if (!root) {
        std::cout << "[tree] Empty tree.\n";
        return;
    }

    print_helper(0, root); 
}


/* ----- Define Tree Operation Functions ----- */
void TreeOperation::insert(Tree* tree, const unsigned short int nodeVal) {
    // cases sourced from: https://www.geeksforgeeks.org/dsa/insertion-in-red-black-tree/
    
    // Check to see if child will insert to root
    if (tree->get_root() == nullptr) {
        Node* nn = new Node();
        nn->set_value(nodeVal);
        nn->set_color(false);
        tree->set_root(nn);
        return;
    }

    // Child is nn
    Node* nn = insert_helper(nodeVal, tree->get_root());
    if (!nn) return;
    
    // Check if the parent is root
    Node* parent = nn->get_pa();
    if (parent == tree->get_root()) {
        nn->set_color(true);
        return;
    }
    
    // Red Black Tree insert cases
    // If the uncle is red, change parent and uncle to black
    Node* uncle = nullptr;
    Node* grandpa = parent->get_pa();
    if (parent == grandpa->get_c1()) uncle = grandpa->get_c2();
    else uncle = grandpa->get_c1();
    
    if (uncle && uncle->get_color()) {
        uncle->set_color(false);
        parent->set_color(false);
        
        // If the grandpa is not the root then set it to red
        if (grandpa != tree->get_root()) grandpa->set_color(true);
    }
    else if (!parent->get_color()) {
        return;
    }
    else {
        switch(determine_rotation(nn, parent, grandpa)) {
            case LL: {
                LL_rotation(nn, parent, grandpa);
                if (!parent->get_pa()) tree->set_root(parent);
                break;
            }
            case LR: {
                LR_rotation(nn, parent, grandpa);
                if (!nn->get_pa()) tree->set_root(nn);
                break;
            }
            case RR: {
                RR_rotation(nn, parent, grandpa);
                if (!parent->get_pa()) tree->set_root(parent);
                break;
            }
            case RL: {
                RL_rotation(nn, parent, grandpa);
                if (!nn->get_pa()) tree->set_root(nn); 
                break;
            }
            default: {
                std::cout << "Invalid rotation.\n";
                break;
            }
        }
    }

    if (tree->get_root()) tree->get_root()->set_color(false);
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
    Node* god = grandpa->get_pa();
    Node* T3 = parent->get_c2();
    
    // If exists assign third child to grandpa (rotate)
    if (T3) T3->set_pa(grandpa);   
    
    // First exchange parent/grandpa information
    parent->set_pa(god); // flag <----
    if (god) {
        if (god->get_c1() == grandpa) god->set_c1(parent);
        else if (god->get_c2() == grandpa) god->set_c2(parent);
    }
    grandpa->set_pa(parent);
    parent->set_c2(grandpa);

    // Assign children
    grandpa->set_c1(T3);

    // Assign colors
    parent->set_color(false);
    grandpa->set_color(true);

    return;
}
void LR_rotation(Node* child, Node* parent, Node* grandpa) {
    // Case 2: Left Right Case (LR rotation)
    // Left rotation of Parent P
    // My own words: move parent to child C1 and move child to parent
    // then perform LL rotation
    //Node* T1 = parent->get_c1();
    Node* T2 = child->get_c1();
    Node* T3 = child->get_c2();
    
    // Perform initial rotation (to the leeeft)
    parent->set_c2(T2);
    if (T2) T2->set_pa(parent);

    // Perform exchanges
    child->set_c1(parent);
    parent->set_pa(child);

    child->set_pa(grandpa);
    grandpa->set_c1(child);

    // If the child exists connect it back to grandpa
    //if (T3) T3->set_pa(grandpa);
        
    // Perform LL rotation
    LL_rotation(parent, child, grandpa);
}
void RR_rotation(Node* child, Node* parent, Node* grandpa) {
    // Case 3: Right Right Case (RR rotation)
    // My own words: move parent up, move grandpa to the right
    Node* god = grandpa->get_pa();
    Node* T3 = parent->get_c1();
    
    // Check for existence of children and if so assign parent
    if (T3) T3->set_pa(grandpa);
    parent->set_pa(god);

    // Assign god's child by determining which child it is
    if (god) {
        if (god->get_c1() == grandpa) god->set_c1(parent);
        else if (god->get_c2() == grandpa) god->set_c2(parent);
    }
    
    // Assign parents and children
    grandpa->set_pa(parent);
    parent->set_c1(grandpa);
    grandpa->set_c2(T3);

    // Assign colors
    parent->set_color(false);
    grandpa->set_color(true);
}
void RL_rotation(Node* child, Node* parent, Node* grandpa) { 
    // Case 4: Right Left Case (RL rotation)
    // My own words: Swap parent and child and
    // then perform RR case
    Node* T3 = child->get_c1(); // random name
    Node* T4 = child->get_c2();
    // Node* god = grandpa->get_pa(); // is this needed?
    
    // Perform the initial rotation (to the riiiight)
    parent->set_c1(T3);

    // Obviously if it exists then set the new parent
    if (T3) T3->set_pa(parent);

    // Exchange between child/parent
    child->set_c2(parent);
    parent->set_pa(child);

    // Create connections between T3 child and grandpa
    child->set_pa(grandpa);
    grandpa->set_c2(child);

    // If it exists, set the new parent
    //if (T3) T3->set_pa(grandpa);

    // Perform exchanges between grandpa & child
    child->set_pa(grandpa);
    grandpa->set_c2(child);

    // Perform RR
    RR_rotation(parent, child, grandpa);
}
void print_helper(const unsigned int depth, Node* n) {
    if (!n) return;
    
    // Print the right side (represented as upper)
    print_helper(depth + 1, n->get_c2());
    
    // Indent based on current depth and print
    // the current node value
    std::cout << "[tree] " << std::string(depth * 4, ' ');
    printNode(n);
    std::cout << "\n";
    
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