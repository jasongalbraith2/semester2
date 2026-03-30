/* 
----- Notes -----
Node Color: Each node is either red or black.
Root Property: The root of the tree is always black.
Red Node Property: Red nodes cannot have red children (Red nodes cannot be adjacent).
Black Node Property: Every path from a node to its descendant leaves must have the same number of black nodes.
Leaf Property: All leaves (NIL nodes) are black.

GIVE FUNCS FOR EACH ROTATION

ALL INSERTED NODES ARE RED
^ (https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/)

TO RUN: g++ *.cpp -o rbt
*/
#include <iostream>
#include <string>

#include "node.hpp"
#include "tree.hpp"
#include "utils.hpp"

int main() {
    Tree tree;

    std::string command;
    Command result;
    bool running = true;
    do {
        std::cout << "[] Enter Command (add, search, import, print, [dysfunctional] delete, quit) > ";
        std::getline(std::cin, command);
        result = process_command(command);

        switch (result) {
            case PRINT: {
                tree.print();
                break;
            }
            case IMPORT: {
                std::cout << "[import] Enter filename to import from > ";
                std::getline(std::cin, command);
                command = import_from_file(command);
                
                // use insert method
                RBTO::insert_str(&tree, command);
                break;
            }
            case ADD: {
                std::cout << "[add] Enter number to add > ";
                std::getline(std::cin, command);
                RBTO::insert_str(&tree, command);
                break;
            }
            case SEARCH: {
                std::cout << "[srch] Enter number to search > ";
                std::getline(std::cin, command);
                if (RBTO::search(&tree, command)) std::cout << "[srch] Exists.\n";
                else std::cout << "[srch] Does not exist.\n";
                break;
            }
            case DELETE: {
                /*
                std::cout << "[del] Enter number to delete > ";
                std::getline(std::cin, command);
                tree.del(command);
                */
                break;
            }
            case QUIT: {
                running = false;
                break;
            }
            default: break;
        }
    } while (running);

    return 0;
}