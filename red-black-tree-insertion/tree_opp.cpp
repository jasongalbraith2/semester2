#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#include "tree.hpp"
#include "utils.hpp"
#include "node.hpp"

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

/* ----- Create Operation Functions ----- */
void TreeOperation::insert_str(Tree* tree, const std::string& tokens) {
    std::istringstream iss(tokens);
    std::string token;
    unsigned short int num;
    while (std::getline(iss, token, ' ')) {
        if (is_number(token)) {
            num = std::stoi(token);
            if (num >= 1 && num <= 999) insert(tree, num);
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