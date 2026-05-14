#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#include "tree.hpp"
#include "utils.hpp"
#include "node.hpp"

/* ----- Define Tree Operation Functions ----- */
enum RotationType { LL, LR, RR, RL, NONE };
void print_helper(Node* n, const unsigned int depth);
void clear_helper(Node* n);
void recursive_insert(Node* c, Node* n, const unsigned short int v);
RotationType determine_rotation(Node* child, Node* parent, Node* grandpa);
void LL_rotation(Node* child, Node* parent, Node* grandpa);
void LR_rotation(Node* child, Node* parent, Node* grandpa);
void RR_rotation(Node* child, Node* parent, Node* grandpa);
void RL_rotation(Node* child, Node* parent, Node* grandpa);

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
    print_helper(root, 0);
}

/* ----- Create Tree Insertion Functions ----- */
void TreeOperation::insert(Tree* tree, const unsigned short int v) {
    // cases sourced from: https://www.geeksforgeeks.org/dsa/insertion-in-red-black-tree/
    
    // Create node
    Node* n = new Node();
    n->set_value(v);
    
    // Is the child the root ?
    if (!tree->get_root()) {
        // If there is no root, create the new node as the root !
        n->set_color(false);
        tree->set_root(n);
        return;
    }
    
    // Now, figure out the insert point
    // Iterate through the tree and figure out the placement point
    // based on the values
    recursive_insert(tree->get_root(), n, v);
    
    // If the parent is the root then perform that individual condition
    if (n->get_pa() == tree->get_root()) {
        n->set_color(true);
        return;
    }
    
    // Now we check for the uncle situation
    Node* parent = n->get_pa();
    Node* uncle;
    Node* grandpa = parent->get_pa();
    
    // Define uncle
    if (grandpa) {
        if (grandpa->get_c1() == parent) uncle = grandpa->get_c2();
        else uncle = grandpa->get_c1();
    }
    else uncle = nullptr;
    
    // If the uncle is red change parent + uncle to black and grandpa to red
    if (uncle && uncle->get_color()) {
        uncle->set_color(false);
        parent->set_color(false);
        if (tree->get_root() == grandpa) grandpa->set_color(false);
        else grandpa->set_color(true);
        
        return;
    }
    else if (!parent->get_color()) return;
    else {
        // Perform LL / LR / RL/ RR rotations
        switch (determine_rotation(n, parent, grandpa)) {
            case LL: {
                LL_rotation(n, parent, grandpa);
                if (!parent->get_pa()) tree->set_root(parent);
                break;
            }
            case LR: {
                LR_rotation(n, parent, grandpa);
                if (!n->get_pa()) tree->set_root(n);
                break;
            }
            case RR: {
                RR_rotation(n, parent, grandpa);
                if (!parent->get_pa()) tree->set_root(parent);
                break;
            }
            case RL: {
                RL_rotation(n, parent, grandpa);
                if (!n->get_pa()) tree->set_root(n);
                break;
            }
            default: {
                break;
            }
        }
    }
    tree->get_root()->set_color(false);

    return;
}

RotationType determine_rotation(Node* child, Node* parent, Node* grandpa) {
    // Quickly make sure the nodes we need exist
    if (!child || !parent || !grandpa) return NONE;
    if (grandpa->get_c1() == parent && parent->get_c1() == child) return LL;
    if (grandpa->get_c1() == parent && parent->get_c2() == child) return LR;
    if (grandpa->get_c2() == parent && parent->get_c2() == child) return RR;
    if (grandpa->get_c2() == parent && parent->get_c1() == child) return RL;
    return NONE;
}

void LL_rotation(Node* child, Node* parent, Node* grandpa) {
    /*
    Case 1 LL
    Rotate the grandfather to the right
    Swap colors of the grandfather and the parent
    Pull the parent up and child up
    Swap children etc.
    */

    // parents new father
    Node* god = grandpa->get_pa();

    // the child that becomes the new child of grandpa
    Node* T3 = parent->get_c2();

    // Exchange familial information
    if (god) {
        if (god->get_c1() == grandpa) god->set_c1(parent);
        else god->set_c2(parent);
    }
    parent->set_pa(god);
    grandpa->set_pa(parent);
    parent->set_c2(grandpa);
    grandpa->set_c1(T3);
    if (T3) T3->set_pa(grandpa);

    // Finally swap colors
    parent->set_color(false);
    grandpa->set_color(true);

    return;
}
void LR_rotation(Node* child, Node* parent, Node* grandpa) {
    /*
    Case 2 LR
    Rotate parent to the left
    Then perform the LL rotation obviously
    We do this by moving the parent down left and the child upward
    */
    
    Node* T2 = child->get_c1();

    // Exchange familial information for the primary rotation
    child->set_pa(grandpa);
    grandpa->set_c1(child);
    child->set_c1(parent);
    parent->set_pa(child);
    parent->set_c2(T2);
    if (T2) T2->set_pa(parent);

    // Perform LL rotation
    LL_rotation(parent, child, grandpa);

    return;
}
void RR_rotation(Node* child, Node* parent, Node* grandpa) {
    /*
    Case 3 RR
    Left rotation of the grandpa
    Swap colors of grandpa and parent
    */
    Node* T3 = parent->get_c1();

    // Exchange familial information
    Node* god = grandpa->get_pa();
    if (god) {
        if (god->get_c1() == grandpa) god->set_c1(parent);
        else god->set_c2(parent);
    }
    parent->set_pa(god);
    grandpa->set_pa(parent);
    parent->set_c1(grandpa);
    grandpa->set_c2(T3);
    if (T3) T3->set_pa(grandpa);

    // Swap colors
    parent->set_color(false);
    grandpa->set_color(true);

    return;
}
void RL_rotation(Node* child, Node* parent, Node* grandpa) {
    /*
    Case 4 RL
    Rotate parent to the right
    Then perform RR
    */
    Node* T4 = child->get_c2();

    // Exchange familial information
    child->set_pa(grandpa);
    grandpa->set_c2(child);
    child->set_c2(parent);
    parent->set_pa(child);
    parent->set_c1(T4);
    if (T4) T4->set_pa(parent);

    // Perform RR rotation
    RR_rotation(parent, child, grandpa);

    return;
}

void recursive_insert(Node* c, Node* n, const unsigned short int v) {
    if (!n) return;
    
    if (v < c->get_value()) {
        // now if it has a child continue the iteration
        if (c->get_c1()) recursive_insert(c->get_c1(), n, v);
        else {
            c->set_c1(n);
            n->set_pa(c);
            n->set_color(true);
            return;
        }
    }
    else {
        // now if it has a child continue the iteration
        if (c->get_c2()) recursive_insert(c->get_c2(), n, v);
        else {
            c->set_c2(n);
            n->set_pa(c);
            n->set_color(true);
            return;
        }
    }
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

void print_helper(Node* n, const unsigned int depth) {
    if (!n) return;
    
    // Print the right side (represented as upper)
    print_helper(n->get_c2(), depth + 1);
    
    // Indent based on current depth and print
    // the current node value
    std::cout << "[tree] " << std::string(depth * 4, ' ');
    printNode(n);
    std::cout << "\n";
    
    // Print the left side (represented as lower)
    print_helper(n->get_c1(), depth + 1);
}

/* ----- Create Operation Functions ----- */
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