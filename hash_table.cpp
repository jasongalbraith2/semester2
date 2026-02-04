#include <string>
#include <iostream> // for input and output
#include <fstream> // for file I/O
#include <sstream> // for file I/o
#include <algorithm> // for to_lower function
#include <utility> // for passing string pair
#include <cstdlib> // for rand() and passing string pair
#include <cctype> // for tolower(

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
    Node* headNode,
) {
    if (headNode->getNext() == NULL) return 1;
    return 1 + get_linked_list_depth(headNode->getNext());
}
void add_to_linked_list(
    Node* headNode,
    Node* newNode
) {
    if (headNode->getNext() == NULL) headNode->setNext(newNode);
    else add_to_linked_list(headNode->getNext());
}
bool hash_node(
    unsigned int hashMod,
    Node** hashTable,
    Node* newNode
) {
    const unsigned int index = (newNode->getStudent()->studentID + std::round(newNode->getStudent()->GPA)) % hashMod;
    if (hashTable != NULL) {
        if (get_linked_list_depth(hashTable[index]) >= 3) return true;
        else add_to_linked_list(hashTable[index], newNode);
    }
    else {
        hashTable[index] = newNode;
    }
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
    Node* hashTable = new Node*[hashMod];
    for (int i = 0; i < hashMod; i++) { hashMod[i] == NULL; }

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
                break;
            }
            case BULK: {
                break;
            }
            case PRINT: {
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
