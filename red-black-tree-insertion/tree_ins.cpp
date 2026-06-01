#include <iostream>

#include "tree.hpp"
#include "utils.hpp"
#include "node.hpp"

enum RotationType { LL, LR, RR, RL, NONE };
void recursive_insert(Node* c, Node* n, const unsigned short int v);
void fix_upward_insertion(Tree* tree, Node* n);
//void LL_rotation(Node* child, Node* parent, Node* grandpa);
void LR_rotation(Node* child, Node* parent, Node* grandpa);
//void RR_rotation(Node* child, Node* parent, Node* grandpa);
void RL_rotation(Node* child, Node* parent, Node* grandpa);
RotationType determine_rotation(Node* child, Node* parent, Node* grandpa);

/* Current logic system for insertion
find its spot downward (if value matches just... dont?)
insert node as red

while node is not root AND parent is red:
get parent, grandpa, uncle

if uncle is red:
parent black
uncle black
grandpa red
node = grandpa
else:
rotate
recolor
break

root black
*/


/* ----- Create Tree Insertion Functions ----- */
void insert(Tree* tree, const unsigned short int v) {
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
    
    // Fix the tree upward from the inserted node
    fix_upward_insertion(tree, n);
    return;
}

void fix_upward_insertion(Tree* tree, Node* n) {
    // As we correct colors the problem moves upward and we need to iterate
    // through the tree in order to continuously fix this problem
    // If we still have room to go up and the parent exists and its red
    // thats not ok!!!
    while (n != tree->get_root()
    && n->get_pa()
    && n->get_pa()->get_color()) {
        
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
            
            n = grandpa;
        }
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
    }
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