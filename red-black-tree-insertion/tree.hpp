#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include "node.hpp"

/* ----- Create Binary Search Tree Class ----- */
class Tree {
    private:
    Node* root;
    
    public:
    Tree();
    ~Tree();
    void set_root(Node* r);
    Node* get_root();
    void print();
};

// Internal functinos to share w/ tree_opp
void insert(Tree* tree, const unsigned short int v);

// share with tree_del
void LL_rotation(Node* child, Node* parent, Node* grandpa);
void RR_rotation(Node* child, Node* parent, Node* grandpa);

namespace TreeOperation {
    void insert_str(Tree* tree, const std::string& tokens);
    Node* remove(Tree* tree, const unsigned short int v);
    bool search(Tree* tree, const std::string& token);
    void clear(Tree* tree);
}
namespace RBTO = TreeOperation;

#endif