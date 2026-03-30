#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include "node.hpp"

/* ----- Create Utility Functions ----- */
bool is_number(const std::string& n);
std::string import_from_file(const std::string& fp);
void printNode(Node* n);

/* ----- Create Command Enum ----- */
enum Command {
    PRINT,
    IMPORT,
    ADD,
    SEARCH,
    DELETE,
    QUIT,
    UNKNOWN
};
Command process_command(const std::string& input);

#endif