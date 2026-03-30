#ifndef NODE_HPP
#define NODE_HPP

class Node {
    private:
    // Value is between 1 - 999
    unsigned short int val;
    Node* parent;
    Node* child1;
    Node* child2;
    bool color; // 0 is black, 1 is red
    
    public:
    Node();
    ~Node();
    
    // Setter functions
    void set_value(const unsigned short int v);
    void set_c1(Node* c); // Set the first child node
    void set_c2(Node* c); // Set the second child node
    void set_pa(Node* c);
    void set_color(bool c);
    
    // Getter functions
    const unsigned short int get_value();
    Node* get_c1();
    Node* get_c2();
    Node* get_pa();
    bool get_color();
};

#endif