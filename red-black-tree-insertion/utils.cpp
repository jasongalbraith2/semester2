#include <string>
#include <fstream>
#include <iostream>

#include "utils.hpp"
#include "node.hpp"

/* ----- Define Utility Functions ----- */
bool is_number(const std::string& n) {
    if (n.empty()) return false;
    for (int i = 0; i < n.length(); ++i) if (!std::isdigit(n[i])) return false;
    return true;
}
std::string import_from_file(const std::string& fp) {
    std::ifstream f(fp);
    std::string line;

    if (std::getline(f, line) && f.is_open()) {
        std::cout << line << "\n";
        f.close();

        return line;
    }

    f.close();
    return "";
}
inline void printRed(const std::string& n) {
    std::cout << "\x1b[1;31m" << n << "\x1b[37m";
}
inline void printBlack(const std::string& n) {
    std::cout << "\x1b[1;34m" << n << "\x1b[37m";
}
void printNode(Node* n) {
    if (!n) return;
    std::string v = std::to_string(n->get_value());
    (n->get_color()) ? printRed(v) : printBlack(v);
    std::cout << "\n";
}

/* ----- Define Command Functions ----- */
Command process_command(const std::string& input) {
    if (input == "print") return PRINT;
    if (input == "import") return IMPORT;
    if (input == "add") return ADD;
    if (input == "search") return SEARCH;
    if (input == "delete") return DELETE;
    if (input == "quit") return QUIT;
    return UNKNOWN; // TODO: implement unknown
}