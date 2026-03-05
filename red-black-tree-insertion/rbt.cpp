#include <iostream>


/* 
----- Notes -----
Node Color: Each node is either red or black.
Root Property: The root of the tree is always black.
Red Node Property: Red nodes cannot have red children (Red nodes cannot be adjacent).
Black Node Property: Every path from a node to its descendant leaves must have the same number of black nodes.
Leaf Property: All leaves (NIL nodes) are black.
^ (https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/)
*/


/* ----- Create Node Class Structure ----- */
class Node {
    private:
    Node* parent;
    Node* leftLeaf;
    Node* rightLeaf;

    // Where 0 = black, 1 = red
    bool* color;

    public:
    Node(
        Node* p,
        Node* ll,
        Node* rl,
        bool* c
    ) : parent(p), leftLeaf(ll), rightLeaf(rl), color(c) {}
    ~Node();
};

int main() {
    std::cout << "its done!!!!";
    return 0;
}


/* ----- Define Node Class Functions (constructor & deconstructor) ----- */
// Node()...;
Node::~Node() {
    delete parent;
    delete leftLeaf;
    delete rightLeaf;
    delete color;
}