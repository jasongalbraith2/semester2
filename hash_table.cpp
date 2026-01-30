#include <string>
#include <iostream> // for input and output
#include <fstream> // for file I/O
#include <sstream> // for file I/o
#include <algorithm> // for to_lower function
#include <utility> // for passing string pair
#include <cstdlib> // for rand() and passing string pair
#include <cctype> // for tolower()
#include <vector> // for storing names

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
    std::vector<std::string>& firstNames, 
    std::vector<std::string>& lastNames
) {
    std::string line;
    
    std::ifstream firstFile(FIRST_NAME_FILE_PATH);
    if(std::getline(firstFile, line)) {
        std::stringstream ss(line);
        std::string name;
        
        while (std::getline(ss, name, ',')) {
            firstNames.push_back(name);
        }
    }
    firstFile.close();
    
    std::ifstream lastFile(LAST_NAME_FILE_PATH);
    if(std::getline(lastFile, line)) {
        std::stringstream ss(line);
        std::string name;
        
        while (std::getline(ss, name, ',')) {
            lastNames.push_back(name);
        }
    }
    lastFile.close();
}


/* -----
Generate random first
and last names of students
----- */
void generate_random_name(
    const std::vector<std::string>& firstNames,
    const std::vector<std::string>& lastNames,
    std::string& refFirstName,
    std::string& refLastName
) {
    /* -----
    Generates random indexes in
    both lists, and then sets the
    variables by reference
    ----- */
    size_t indexNo1 = std::rand() % firstNames.size();
    size_t indexNo2 = std::rand() % lastNames.size();
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
	Node(Student* student) { data = student; next = nullptr; }
	~Node() { delete data; }

	/* Function returns private data (node* next) to point to the next node */
	Node* getNext() { return next; }

	/* Function returns private data (student* data) to point to the student value saved */
	Student* getStudent() { return data; }

	/* Function takes a node pointer to point to the next in the chain of nodes */
	void setNext(Node* node) { next = node; }
};

/* -----
User command processes
----- */


int main() {
    bool running = true;
    std::string userInput;
    Command userCommand;

    /* -----
    Import first and last names
    for random name generation
    ----- */
    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;
    import_names(firstNames, lastNames);

    /* debug-funtime
    std::string rfn;
    std::string rln;
    generate_random_name(firstNames, lastNames, rfn, rln);
    std::cout << "Random name generated: " << rfn << " " << rln << std::endl;
    */

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
            case UNKNOWN: {
                break;
            }
            default: {
                break;
            }
        }
    }

    return 0;
}
