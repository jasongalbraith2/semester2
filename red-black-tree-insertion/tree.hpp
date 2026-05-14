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

namespace TreeOperation {
    void insert(Tree* tree, const unsigned short int v);
    void insert_str(Tree* tree, const std::string& tokens);
    bool search(Tree* tree, const std::string& token);
    void clear(Tree* tree);
}
namespace RBTO = TreeOperation;

#endif