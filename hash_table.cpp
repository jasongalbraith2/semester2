#include <string>
#include <iostream> // for input and output
#include <fstream> // for file I/O
#include <sstream> // for file I/O
#include <algorithm> // for to_lower function
#include <utility> // for passing string pair
#include <cstdlib> // for rand() and passing string pair
#include <cctype> // for tolower()
#include <cmath> // for std::round
#include <ctime> // for std::time

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
        
        while (std::getline(ss, name, ',') && index < FIRST_NAME_NAME_COUNT) {
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
        
        while (std::getline(ss, name, ',') && index < LAST_NAME_NAME_COUNT) {
            lastNames[index] = name;
            index++;
        }
    }
    lastFile.close();
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
    std::string getFName() { return firstName; }
	std::string getFullName() { return firstName + " " + lastName; }
};


/* -----
Generate random first
and last names of students
----- */
Student* generate_random_student(
    std::string (&firstNames)[FIRST_NAME_NAME_COUNT], 
    std::string (&lastNames)[LAST_NAME_NAME_COUNT]
) {
    /* -----
    Generates random indexes in
    both lists, and then sets the
    variables by reference
    ----- */

    size_t indexNo1 = std::rand() % FIRST_NAME_NAME_COUNT;
    size_t indexNo2 = std::rand() % LAST_NAME_NAME_COUNT;
    unsigned int randomID = std::rand() % 900000 + 100000;
    float randomGPA = (std::rand() % 3000 + 1001) / 1000.0f;

    Student* newStudent = new Student(
        firstNames[indexNo1], 
        lastNames[indexNo2],
        randomID,
        randomGPA
    );
    return newStudent; 
}


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
static inline unsigned int create_hash(
    Node* newNode,
    unsigned int hashMod
) {
    return (newNode->getStudent()->getID() * 2654435761u) % hashMod;
}
unsigned int get_linked_list_depth(
    Node* headNode
) {
    unsigned int depth = 0;
    Node* currentNode = headNode;
    while (currentNode != NULL) {
        depth++;
        currentNode = currentNode->getNext();
    }
    return depth;
}
void add_to_linked_list(
    Node* headNode,
    Node* newNode
) {
    if (headNode != NULL) {
        Node* currentNode = headNode;
        while (currentNode->getNext() != NULL) {
            currentNode = currentNode->getNext();
        }
        currentNode->setNext(newNode);
    }
}
bool exceeds_max_depth(
    Node** hashTable,
    unsigned int hashMod
) {
    for (unsigned int i = 0; i < hashMod; ++i) {
        if (hashTable[i] != NULL && get_linked_list_depth(hashTable[i]) > 3) return true;
    }
    return false;
}
bool hash_node(
    unsigned int hashMod,
    Node** hashTable,
    Node* newNode
) {

    /* -----
    Hashing mathematics occurs here
    Here we implement Knuth's multiplicative method,
    which is a simplistic implementation of hashing 
    protocol.
    ----- */
    const unsigned int index = create_hash(newNode, hashMod);

    /* -----
    Attempt to add hashed node
    into key index into hash table
    ----- */
    if (hashTable[index] != NULL) {
        add_to_linked_list(hashTable[index], newNode);
        if (get_linked_list_depth(hashTable[index]) > 3) return true;
    }
    else {
        hashTable[index] = newNode;
    }
    return false;
}
void rehash_nodes(
    unsigned int &hashMod,
    Node** &hashTable
) {
    /* -----
    Reset hash function,
    create new table,
    allocate memory,
    and assign null to table
    ----- */
    hashMod *= 2;
    Node** newHashTable = new Node*[hashMod];
    for (unsigned int i = 0; i < hashMod; ++i) { newHashTable[i] = NULL; }

    /* -----
    Iterate through old table
    and rehash all nodes
    into the new table
    ----- */
    for (unsigned int i = 0; i < (hashMod / 2); ++i) {
        if (hashTable[i] != NULL) {
            Node* currentNode = hashTable[i];
            // step to next node in the linked list and rehash each node
            while (currentNode != NULL) {
                Node* next = currentNode->getNext();
                currentNode->setNext(NULL);
                hash_node(hashMod, newHashTable, currentNode);
                currentNode = next;
            }
        }
    }
    delete[] hashTable;
    hashTable = newHashTable;
}


/* -----
User command processes
----- */
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
                std::cout << " | " << currentNode->getStudent()->getFullName() << " " << currentNode->getStudent()->getID() << " " << currentNode->getStudent()->getGPA();
                currentNode = currentNode->getNext();
            }
            std::cout << "\n";
        }
    }
}

/* -----
Executive loop
----- */
int main() {
    // initiate random
    std::srand(std::time(nullptr));

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

    // initiate loop vars
    bool running = true;
    std::string userInput;
    Command userCommand;

    // add command variables
    bool result = false;
    unsigned int rehashCount = 0;

    // bulk command variables
    unsigned int bulkCount = 0;

    // delete variables
    Node* currentNode;
    Node* priorNode;
    unsigned int toDelete = 0;
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
                    rehashCount++;
                    rehash_nodes(hashMod, hashTable);
                    result = exceeds_max_depth(hashTable, hashMod);

                    if (rehashCount > 16) {
                        std::cout << "Too many rehashes, exiting program.\n";
                        return 1;
                    }
                }
                break;
            }
            case BULK: {
                std::cout << "[] Enter Amount of Names > ";
                std::cin >> bulkCount;
                
                for (unsigned int i = 0; i < bulkCount; ++i) {
                    Student* newStudent = generate_random_student(firstNames, lastNames);
                    Node* newNode = new Node(newStudent);
                    result = hash_node(hashMod, hashTable, newNode);

                    // continue to rehash until function is large enough to 
                    // support all table without 4+ nodes in a single index
                    while (result) {
                        rehashCount++;
                        rehash_nodes(hashMod, hashTable);
                        result = exceeds_max_depth(hashTable, hashMod);

                        if (rehashCount > 16) {
                            std::cout << "Too many rehashes, exiting program.\n";
                            return 1;
                        }
                    }
                }
                break;
            }
            case PRINT: {
                print_table(hashMod, hashTable);
                break;
            }
            case DELETE: {
                std::cout << "[] Enter ID to Delete > ";
                std::cin >> toDelete;
                for (unsigned int i = 0; i < hashMod; ++i) {
                    if (hashTable[i] != NULL) {
                        currentNode = hashTable[i];
                        priorNode = NULL;
                        while (currentNode != NULL) {
                            if (currentNode->getStudent()->getID() == toDelete) {
                                /* FIX */
                                if (priorNode == NULL) {
                                    hashTable[i] = currentNode->getNext();
                                } else {
                                    priorNode->setNext(currentNode->getNext());
                                }
                                delete currentNode;
                                goto complete_deletion;
                            }
                            priorNode = currentNode;
                            currentNode = currentNode->getNext();
                        }
                    }
                }
                std::cout << "ID not found.\n";
                complete_deletion:
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
    for (unsigned int i = 0; i < hashMod; ++i) {
        if (hashTable[i] != NULL) {
            Node* currentNode = hashTable[i];
            while (currentNode != NULL) {
                Node* next = currentNode->getNext();
                delete currentNode;
                currentNode = next;
            }
        }
    }
    delete[] hashTable;

    return 0;
}
