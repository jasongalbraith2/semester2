#include <iostream>

#include "tree.hpp"
#include "utils.hpp"
#include "node.hpp"

/* ----- Create and Define Additional Deletion Functions ----- */
enum Case { 
    X_RED, 
    X_SIBLING_RED,
    X_SIBLING_BLACK_BOTH_NEPHEWS_BLACK,
    X_SIBLING_BLACK_LEFT_NEPHEW_RED,
    X_SIBLING_BLACK_RIGHT_NEPHEW_RED,
    NONE
};
void delete_helper(Node* n);
Node* lurk_up_root(Node* n);
Node* tree_fix(Node* root, Node* x); // copilot suggested this name when i was writing this
Node* grabber(Node* n, const unsigned short int v);
Node* minimum(Node* n); // gets smallest node in the given subtree
Case determine_case(Node* x, Node* sibling);

Node* TreeOperation::remove(Tree* tree, const unsigned short int v) {
    // Identify if the node exists
    if (!TreeOperation::search(tree, std::to_string(v))) {
        std::cout << "[del] Node does not exist.\n";
        return nullptr;
    }
    
    // Since the node exists, get that bad boy.
    // There is an extra checker here just for fun
    Node* z = grabber(tree->get_root(), v);
    if (!z) {
        std::cout << "[del] Node does not exist.\n";
        return nullptr;
    }
    
    // Since the node exists then determine the case and perform the deletion
    // ALGORITHM: https://www2.cs.sfu.ca/CourseCentral/225/jmanuch/lec/11-2.pdf
    Node* x = nullptr;
    Node* y = z;
    
    bool y_co = y->get_color();
    
    if (z->get_c1() && z->get_c2()) {
        y = minimum(z->get_c2());
        y_co = y->get_color();
        // z is y logically now lolz
        z->set_value(y->get_value());
    }
    
    // now find child x from y
    if (y->get_c1()) x = y->get_c1();
    else x = y->get_c2();
    
    // if x is null create a pretend node
    bool x_is_fake = false;
    
    if (!x) {
        x = new Node();
        x->set_color(false);
        x->set_pa(y);
        x_is_fake = true;
    }
    
    // before we detach y we need to get its parent
    Node* y_pa = y->get_pa();
    
    // transplant section
    // we are transplanting x in place of y
    // hence we need to attach x to y's parent and detach y from its parent
    
    // If y is the root just move x up
    if (!y->get_pa()) {
        tree->set_root(x);
        x->set_pa(nullptr);
    }
    
    // Attach x to y's parent for predecessor deletion
    else if (y == y_pa->get_c1()) {
        y_pa->set_c1(x);
        x->set_pa(y_pa);
    }
    
    // Or the other side
    else {
        y_pa->set_c2(x);
        x->set_pa(y_pa);
    }
    
    if (!y_co) {
        tree->set_root(tree_fix(tree->get_root(), x));
    }
    
    // If x is actually the pretend node
    // then we gotta get rid of it after fixing
    // and obviously deallocate some cutesy stuff
    if (x_is_fake) {
        Node* x_pa = x->get_pa();
        
        if (!x_pa) {
            tree->set_root(nullptr);
        }
        else if (x_pa->get_c1() == x) {
            x_pa->set_c1(nullptr);
        }
        else if (x_pa->get_c2() == x) {
            x_pa->set_c2(nullptr);
        }
        
        delete x;
    } 
    
    // and also y will need to be removed from memory
    if (y != z) delete y;
    else delete z;
    
    // reset tree stuff
    // really just a check case stole the idea from wikipedia
    if (tree->get_root()) {
        tree->get_root()->set_color(false);
        tree->get_root()->set_pa(nullptr);
    }
    
    return tree->get_root();
}

Node* tree_fix(Node* root, Node* x) {
    while (x != root && !x->get_color()) {
        // Check which child x is
        // (left case)
        Node* pa = x->get_pa();
        if (!pa) break; // over
        
        // get sibling
        Node* sibling = nullptr;
        if (x == pa->get_c2()) sibling = pa->get_c1();
        else if (x == pa->get_c1()) sibling = pa->get_c2();
        
        // so like if we messed up and there is no sibling lets lurk up
        if (!sibling) {
            x = pa;
            continue;
        }
        
        // now lets define the case
        Case c = determine_case(x, sibling);
        x->set_color(false);
        return root;   
        
        if (x == x->get_pa()->get_c1()) {
            // sibling is right child
            switch (c) {
                case X_SIBLING_RED: {
                    // convert to black by rotating left and recoloring
                    sibling->set_color(false);
                    pa->set_color(true);
                    
                    RR_rotation(sibling, pa, pa->get_pa());
                    
                    // after rotation sibling is parent of pa
                    // we need to update root 
                    // assume pa is root now
                    root = lurk_up_root(sibling);
                    
                    // now we need to update x's sibling
                    sibling = pa->get_c2();
                    
                    // redetermine the root case
                    c = determine_case(x, sibling);
                    break;
                }
                case X_SIBLING_BLACK_BOTH_NEPHEWS_BLACK: {
                    // lets recolor the sibling to change black count
                    sibling->set_color(true);
                    x = pa;
                    break;
                }
                case X_SIBLING_BLACK_LEFT_NEPHEW_RED: {
                    // right rotate sibling
                    Node* kindaNephew = sibling->get_c1();

                    // change their colors
                    kindaNephew->set_color(false);
                    sibling->set_color(true);

                    // now lets rotate
                    LL_rotation(kindaNephew, sibling, pa);
                    
                    // now lurk up the root
                    root = lurk_up_root(kindaNephew);
                    break;
                }
                case X_SIBLING_BLACK_RIGHT_NEPHEW_RED: {
                    // get the further nephew
                    Node* notReallyNephew = sibling->get_c2();

                    // change the colors
                    sibling->set_color(pa->get_color());
                    pa->set_color(true);
                    if (notReallyNephew) notReallyNephew->set_color(false);

                    // now perform the left rotation
                    RR_rotation(notReallyNephew, sibling, pa);

                    // now lurk up the root
                    root = lurk_up_root(sibling);

                    // fun fact we fixed the tree
                    x = root;
                    break;
                }
                case X_RED: case NONE: default: {
                    x = root;
                    break;
                }
            }
        }
        else {
            switch (c) {
                case X_SIBLING_RED: {
                    // change colors by rotation right

                    // first reset colors
                    sibling->set_color(false);
                    pa->set_color(true);

                    // rotate right
                    LL_rotation(sibling, pa, pa->get_pa());

                    // now lurk up root
                    root = lurk_up_root(sibling);

                    // now check case again
                    sibling = pa->get_c1();
                    c = determine_case(x, sibling);
                    break;
                }
                case X_SIBLING_BLACK_BOTH_NEPHEWS_BLACK: {
                    // lets recolor sibling to red to change black count
                    sibling->set_color(true);
                    x = pa;
                    break;
                }
                case X_SIBLING_BLACK_RIGHT_NEPHEW_RED: {
                    // snag close nephew
                    Node* kindaNephew = sibling->get_c2();

                    // recolor 
                    kindaNephew->set_color(false);
                    sibling->set_color(true);

                    // now rotate left
                    RR_rotation(kindaNephew, sibling, pa);

                    // now lurk up root
                    root = lurk_up_root(kindaNephew);
                    break;
                }
                case X_SIBLING_BLACK_LEFT_NEPHEW_RED: {
                    // snag far nephew
                    Node* notReallyNephew = sibling->get_c1();

                    // recolor real quick
                    sibling->set_color(pa->get_color());
                    pa->set_color(true);
                    if (notReallyNephew) notReallyNephew->set_color(false);

                    // now perform the right rotation
                    LL_rotation(notReallyNephew, sibling, pa);

                    // now lurk up root
                    root = lurk_up_root(sibling);

                    // fun fact we fixed the tree
                    x = root;
                    break;
                }
                case X_RED: case NONE: default: {
                    x = root;
                    break;
                }
            }
        }
    }

    // now quickly fix x
    if (x && x->get_color()) x->set_color(false);
    
    if (x) root = lurk_up_root(x);
    else root = lurk_up_root(root);

    // finally reset root
    if (root) {
        root->set_color(false);
        root->set_pa(nullptr);
    }

    return root;
}

Node* grabber(Node* n, const unsigned short int v) {
    if (!n) return nullptr;
    if (n->get_value() == v) return n;
    else if (v < n->get_value()) return grabber(n->get_c1(), v);
    else return grabber(n->get_c2(), v);
}

Node* lurk_up_root(Node* n) {
    if (!n) return nullptr;
    // else just literally climb up and return el root
    while (n->get_pa()) n = n->get_pa();
    return n;
}

Node* minimum(Node* n) {
    if (!n) return nullptr;
    // else is literally go downward as low as possible in the subtree
    while (n->get_c1()) n = n->get_c1();
    return n;
}

Case determine_case(Node* x, Node* sibling) {
    if (x->get_color()) return X_RED;
    
    // roots of subtrees should be black
    else if (sibling->get_color()) return X_SIBLING_RED;
    
    // not checking for sibling because we know the sibling is black now
    // not checking nephew colors because we assume the children are null and ergo black
    else if (!sibling->get_c1() && !sibling->get_c2()) return X_SIBLING_BLACK_BOTH_NEPHEWS_BLACK;
    
    // check if the nephew exists, check if it has the color, and confirm the other thing isnt red/exists
    else if (sibling->get_c1() && sibling->get_c1()->get_color() && ((!sibling->get_c2()) || !sibling->get_c2()->get_color())) return X_SIBLING_BLACK_LEFT_NEPHEW_RED;
    else if (sibling->get_c2() && sibling->get_c2()->get_color() && ((!sibling->get_c1()) || !sibling->get_c1()->get_color())) return X_SIBLING_BLACK_RIGHT_NEPHEW_RED;
    
    // probably wont happen but obviously default case
    else return NONE;
}