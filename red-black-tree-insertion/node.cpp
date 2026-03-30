#include "node.hpp"

/* ----- Define Node Class Functions ----- */
Node::Node() {
    val = 0;
    child1 = nullptr;
    child2 = nullptr;
    parent = nullptr;
    color = false;
}
Node::~Node() {
    // Children are deleted by Tree::clear() to avoid recursion
}
void Node::set_value(const unsigned short int v) { val = v; }
void Node::set_c1(Node* c) { child1 = c; }
void Node::set_c2(Node* c) { child2 = c; }
void Node::set_pa(Node* c) { parent = c; }
void Node::set_color(const bool c) { color = c; }
const unsigned short int Node::get_value() { return val; }
Node* Node::get_c1() { return child1; }
Node* Node::get_c2() { return child2; }
Node* Node::get_pa() { return parent; }
bool Node::get_color() { return color; }
