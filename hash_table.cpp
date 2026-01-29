#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

enum Command 
{
    ADD,
    PRINT,
    DELETE,
    QUIT,
    UNKNOWN
};

static void to_lower(std::string& inputString) {
    std::transform(inputString.begin(), inputString.end(), inputString.begin(),
        [](unsigned char c) { return std::tolower(c); } );
}

static Command enumerate_command(std::string& userInput) {
    to_lower(userInput);

    if (userInput == "add") return ADD;
    if (userInput == "print") return PRINT;
    if (userInput == "delete") return DELETE;
    if (userInput == "quit") return QUIT;
    return UNKNOWN;
}

int main() {
    bool running = true;
    std::string userInput;
    Command userCommand;

    while (running) 
    {
        /* 
        Read in user input and determine 
        the desired command.
        */
        userInput = "";
        std::cout << "> ";
        std::cin >> userInput;
        userCommand = enumerate_command(userInput);
        
        /*
        Call functions to enact the command
        */
        switch(userCommand) 
        {
            case ADD:
                break;
            case PRINT:
                break;
            case DELETE:
                break;
            case QUIT:
                running = false;
                break;
            case UNKNOWN:
                break;
            default:
                break;
        }
    }

    return 0;
}
