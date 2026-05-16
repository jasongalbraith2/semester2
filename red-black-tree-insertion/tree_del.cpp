#include <iostream>

#include "tree.hpp"
#include "utils.hpp"
#include "node.hpp"

/* ----- Create and Define Additional Deletion Functions ----- */
enum DeletionType { NO_CHILD, NONE };
void delete_helper(Node* n);
Node* grabber(Node* n, const unsigned short int v);
void transplant(Tree* tree, Node* u, Node* v); // copilot suggested this name when i was writing this
DeletionType determine_case(Tree* tree, Node* n);

void TreeOperation::remove(Tree* tree, const unsigned short int v) {
    // Identify if the node exists
    if (!TreeOperation::search(tree, std::to_string(v))) {
        std::cout << "[del] Node does not exist.\n";
        return;
    }

    // Since the node exists then determine the case and perform the deletion
    // Grab the node to do this
}

Node* grabber(Node* n, const unsigned short int v) {
    if (!n) return nullptr;
    if (n->get_value() == v) return n;
    else if (v < n->get_value()) return grabber(n->get_c1(), v);
    else return grabber(n->get_c2(), v);
}

/* ----- Create Deletion Functions ----- */
DeletionType determine_case(Tree* tree, Node* n) {
    // First, check if it has no children
    if (!n->get_c1() && !n->get_c2()) return NO_CHILD;

    // second...


    // Undefined case
    return NONE;
}