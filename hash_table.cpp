#include <string>
#include <iostream> // for input and output
#include <fstream> // for file I/O
#include <sstream> // for file I/O
#include <algorithm> // for to_lower function
#include <utility> // for passing string pair
#include <cstdlib> // for rand() and passing string pair
#include <cctype> // for tolower()
#include <cmath> // for std::round

#define FIRST_NAME_FILE_PATH "firstNames.csv"
#define LAST_NAME_FILE_PATH "lastNames.csv"
#define FIRST_NAME_NAME_COUNT 1001
#define LAST_NAME_NAME_COUNT 1001

/* -----
Establish command types
----- */
enum Command 
{
    ADD,
    BULK,
    PRINT,
    DELETE,
    QUIT,
    UNKNOWN
};


/* -----
Command utility functions
----- */
static void to_lower(
    std::string& inputString
) {
    std::transform(inputString.begin(), inputString.end(), inputString.begin(),
        [](unsigned char c) { return std::tolower(c); } );
}

static Command enumerate_command(std::string& userInput) {
    to_lower(userInput);

    if (userInput == "add") return ADD;
    if (userInput == "bulk") return BULK;
    if (userInput == "print") return PRINT;
    if (userInput == "delete") return DELETE;
    if (userInput == "quit") return QUIT;
    return UNKNOWN;
}


/* -----
File I/O
----- */
void import_names(
    std::string (&firstNames)[FIRST_NAME_NAME_COUNT], 
    std::string (&lastNames)[LAST_NAME_NAME_COUNT]
) {
    std::string line;
    unsigned int index = 0;
    
    std::ifstream firstFile(FIRST_NAME_FILE_PATH);
    if(std::getline(firstFile, line)) {
        std::stringstream ss(line);
        std::string name;
        
        while (std::getline(ss, name, ',')) {
            firstNames[index] = name;
            index++;
        }
    }
    firstFile.close();
    
    index = 0;
    std::ifstream lastFile(LAST_NAME_FILE_PATH);
    if(std::getline(lastFile, line)) {
        std::stringstream ss(line);
        std::string name;
        
        while (std::getline(ss, name, ',')) {
            lastNames[index] = name;
            index++;
        }
    }
    lastFile.close();
}


/* -----
Generate random first
and last names of students
----- */
void generate_random_name(
    std::string (&firstNames)[FIRST_NAME_NAME_COUNT], 
    std::string (&lastNames)[LAST_NAME_NAME_COUNT],
    std::string& refFirstName,
    std::string& refLastName
) {
    /* -----
    Generates random indexes in
    both lists, and then sets the
    variables by reference
    ----- */
    size_t indexNo1 = std::rand() % FIRST_NAME_NAME_COUNT;
    size_t indexNo2 = std::rand() % LAST_NAME_NAME_COUNT;
    refFirstName = firstNames[indexNo1];
    refLastName = lastNames[indexNo2];
}


/* -----
Initiate student class
and define functionality
----- */
class Student {
private:
    const std::string firstName;
    const std::string lastName;
    const int studentID;
    const float GPA;
public:
    Student(
        std::string userFirstName,
        std::string userLastName,
        int userStudentID,
        float userGPA
    )
        : firstName(userFirstName),
          lastName(userLastName),
          studentID(userStudentID),
          GPA(userGPA)
    {}
    
    int getID() { return studentID; }
    float getGPA() { return GPA; }
};


/* -----
Initiate node class
and define functionality
(from linkedlistspt1 repo by jasongalbraith2:
https://github.com/jasongalbraith2/linkedlistspt1/blob/main/node.h)
----- */
class Node {
private:
	Node* next;
	Student* data;
public:

	/* Constructor / Deconstructor */
	Node(Student* student) { data = student; next = NULL; }
	~Node() { delete data; }

	/* Function returns private data (node* next) to point to the next node */
	Node* getNext() { return next; }

	/* Function returns private data (student* data) to point to the student value saved */
	Student* getStudent() { return data; }

	/* Function takes a node pointer to point to the next in the chain of nodes */
	void setNext(Node* node) { next = node; }
};

/* -----
Hashing processes
----- */
unsigned int get_linked_list_depth(
    Node* headNode
) {
    if (headNode->getNext() == NULL) return 1;
    return 1 + get_linked_list_depth(headNode->getNext());
}
void add_to_linked_list(
    Node* headNode,
    Node* newNode
) {
    if (headNode->getNext() == NULL) headNode->setNext(newNode);
    else add_to_linked_list(headNode->getNext(), newNode);
}
bool hash_node(
    unsigned int hashMod,
    Node** hashTable,
    Node* newNode
) {
    const unsigned int index = (newNode->getStudent()->getID() + (unsigned int)(std::round(newNode->getStudent()->getGPA()))) % hashMod;
    if (hashTable[index] != NULL) {
        add_to_linked_list(hashTable[index], newNode);
        if (get_linked_list_depth(hashTable[index]) > 3) return true;
    }
    else {
        hashTable[index] = newNode;
    }
    return false;
}
void print_table(
    unsigned int hashMod,
    Node** hashTable
) {
    Node* currentNode = NULL;
    
    for (int i = 0; i < hashMod; ++i) {
        if (hashTable[i] != NULL && hashTable[i]->getStudent() != NULL) {
            std::cout << "KEY: " << i;
            
            // iterate through the linked list
            // for each node and print data
            currentNode = hashTable[i];
            while (currentNode != NULL) {
                std::cout << " | " << hashTable[i]->getStudent()->getID();
                currentNode = currentNode->getNext();
            }
            std::cout << "\n";
        }
    }
}
void rehash_nodes(
    unsigned int &hashMod,
    Node** hashTable
) {
    /* -----
    Reset hash function,
    create new table,
    allocate memory,
    and assign null to table
    ----- */
    hashMod *= 2;
    Node** newHashTable = new Node*[hashMod];
    for (int i = 0; i < hashMod; i++) { newHashTable[i] = NULL; }

    /* -----
    Iterate through old table
    and rehash all nodes
    into the new table
    ----- */
    for (int i = 0; i < hashMod / 2; ++i) {
        if (hashTable[i] != NULL) {
            Node* currentNode = hashTable[i];
            // step to next node in the linked list and rehash each node
            while (currentNode != NULL) {
                currentNode->setNext(NULL);
                hash_node(hashMod, newHashTable, currentNode);
                currentNode = currentNode->getNext();
            }
        }
    }
    delete[] hashTable;
    hashTable = newHashTable;
}


/* -----
User command processes
----- */


/* -----
Executive loop
----- */
int main() {
    // define hashing variables
    unsigned int hashMod = 100;
    Node** hashTable = new Node*[hashMod];
    for (int i = 0; i < hashMod; i++) { hashTable[i] = NULL; }

    /* -----
    Import first and last names
    for random name generation
    ----- */
    std::string firstNames[FIRST_NAME_NAME_COUNT];
    std::string lastNames[LAST_NAME_NAME_COUNT];
    import_names(firstNames, lastNames);

    /* debug-funtime
    std::string rfn;
    std::string rln;
    generate_random_name(firstNames, lastNames, rfn, rln);
    std::cout << "Random name generated: " << rfn << " " << rln << std::endl;
    */

    // initiate loop vars
    bool running = true;
    std::string userInput;
    Command userCommand;

    // add command variables
    bool result = false;
    while (running) 
    {
        /* ----- 
        Read in user input and determine 
        the desired command.
        ----- */
        userInput = "";
        std::cout << "> ";
        std::cin >> userInput;
        userCommand = enumerate_command(userInput);
        
        /* -----
        Call functions to enact the command
        ----- */
        switch(userCommand) 
        {
            case ADD: {
                /* -----
                Grabs student input and
                allocates memory for a 
                new student pointer
                ----- */
                std::string newFirstName;
                std::string newLastName;
                int studentID;
                float studentGPA;

                std::cout << "[] Enter first name > ";
                std::cin >> newFirstName;
                std::cout << "[] Enter last name > ";
                std::cin >> newLastName;
                std::cout << "[] Enter student ID > ";
                std::cin >> studentID;
                std::cout << "[] Enter student GPA > ";
                std::cin >> studentGPA;

                Student* newStudent = new Student(newFirstName, newLastName, studentID, studentGPA);
                Node* newNode = new Node(newStudent);
                result = hash_node(hashMod, hashTable, newNode);

                // continue to rehash until function is large enough to 
                // support all table without 4+ nodes in a single index
                while (result) {
                    rehash_nodes(hashMod, hashTable);
                    result = hash_node(hashMod, hashTable, newNode);
                }

                break;
            }
            case BULK: {
                break;
            }
            case PRINT: {
                print_table(hashMod, hashTable);
                break;
            }
            case DELETE: {
                break;
            }
            case QUIT: {
                running = false;
                break;
            }
            case UNKNOWN: {
                break;
            }
            default: {
                break;
            }
        }
    }

    // deallocate memory
    delete[] hashTable;

    return 0;
}
