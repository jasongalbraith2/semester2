#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cctype>

#define MAX_NUMBERS 100

enum Method {
    IMPORT,
    INSERT,
    PRINT,
    DELETE,
    UNKNOWN
};

static void to_lower(
    std::string& inputString
) {
    std::transform(inputString.begin(), inputString.end(), inputString.begin(),
        [](unsigned char c) { return std::tolower(c); } );
}

Method process_command(std::string command) {
    to_lower(command);
    if (command == "import") { return IMPORT; }
    if (command == "insert") { return INSERT; }
    if (command == "print")  { return PRINT; }
    if (command == "delete") { return DELETE; }
    return UNKNOWN;
}

class Heap {
private:
    unsigned int* heapArray;
    unsigned int heapSize;

    static inline unsigned int left(unsigned int index) {
        return 2 * index + 1;
    }
    static inline unsigned int right(unsigned int index) {
        return 2 * index + 2;
    }
    static inline unsigned int parent(unsigned int index) {
        return (index - 1) / 2;
    }

    unsigned int get_depth(unsigned int index) {
        // Implementation for calculating the depth of the heap
        // Recursively iterate through the heap and count the depth
        if (index >= heapSize) return 0; 
        unsigned int leftDepth = get_depth(left(index));
        unsigned int rightDepth = get_depth(right(index));

        // Return the maximum depth of either side (plus 1)
        return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    }

public:
    Heap() {
        heapArray = new unsigned int[MAX_NUMBERS];
        for (unsigned int i = 0; i < MAX_NUMBERS; ++i) {
            heapArray[i] = 0;
        }

        heapSize = 0;
    }
    ~Heap() {
        delete[] heapArray;
    }

    void insert(unsigned int value) {
        // Implementation for inserting a value into the heap
        ++heapSize;
        heapArray[heapSize - 1] = value;
        if (heapSize != 1) {
            unsigned int currentIndex = heapSize - 1;

            // While the index is not the top index and the current value is greater than the parent value
            while (currentIndex > 0 && heapArray[currentIndex] > heapArray[parent(currentIndex)]) {
                // std::swap is a built-in function that swaps the values of two variables
                // geeks4geeks saved me

                // Swap the indexes
                std::swap(heapArray[currentIndex], heapArray[parent(currentIndex)]);
                // Iterate up the heap
                currentIndex = parent(currentIndex);
            }
        }
    }

    void represent_as_arr() {
        // Implementation for representing the heap as an array
        std::cout << "[] Max Heap (Array Representation): \n[";
        for (unsigned int i = 0; i < heapSize; ++i) {
            std::cout << heapArray[i] << ",";
        }
        std::cout << "]\n";
    }

    void represent() {
        // Implementation for representing the heap
        std::cout << "[] Max Heap: ";

        // Print the right side (represented as upper)
        unsigned int rightDepth = get_depth(right(0));

        // Print middle (first/parent node)
        std::cout << heapArray[0] << "\n";

        // Print left side (represented as lower)
        unsigned int leftDepth = get_depth(left(0));
    }
};

void import_method_1(
    unsigned int*& numbers
) {
    std::cout << "[] Importing numbers (Method 1): Enter numbers until you type quit\nEnds import process.\n";

    // Implementation for import method 1
    std::string input;

    for (unsigned int i = 0; i < MAX_NUMBERS; ++i) {
        std::cout << "[] Enter number > ";
        std::cin >> input;

        // If the input is a number
        if (!input.empty() && std::all_of(input.begin(), input.end(), ::isdigit)) {
            // If not, convert the input to an unsigned integer
            // and store it in the array
            numbers[i] = std::stoul(input);
        }

        // If the string is "quit", quit the import cycle
        else {
            to_lower(input);
            if (input == "quit") return;

            // If not
            else --i; 
        }
    }
}

void print_heap(Heap& heap) {
    // Implementation for printing the heap
}

int main() {
    // Create array for imported integers
    // Each integer ranges from 1 - 1000

    // Allocate memory
    unsigned int* importedNumbers = new unsigned int [MAX_NUMBERS];
    for (unsigned int i = 0; i < MAX_NUMBERS; ++i) {
        importedNumbers[i] = 0;
    }
    Heap* heap = new Heap();

    // Main loop
    bool running = true;
    std::string command;
    unsigned int toInsert;
    Method method;
    do {
        std::cout << "[] Enter Command (Import, Insert, Print, Delete, Quit) > ";
        std::cin >> command;
        method = process_command(command);

        switch (method) {
            case IMPORT:
                // Handle import command
                std::cout << "[] Enter Import Method (1 or 2) > ";
                std::cin >> command;
                if (command == "1") import_method_1(importedNumbers);
                if (command == "2") break; // TODO: Implement method 2
                else break;

                // Insert imported numbers into the heap
                for (unsigned int i = 0; i < MAX_NUMBERS; ++i) {
                    if (importedNumbers[i] != 0) {
                        heap->insert(importedNumbers[i]);
                    }
                }
                break;
            case INSERT:
                // Handle insert command
                std::cout << "[] Enter number to insert > ";
                std::cin >> toInsert;
                heap->insert(toInsert);
                break;
            case PRINT:
                // Handle print command
                heap->represent_as_arr(); // for debugging
                break;
            case DELETE:
                // Handle delete command
                break;
            case UNKNOWN:
                return 1;
                // break;
            default:
                break;
        }
    } while (running);

    // Deallocate memory
    delete heap;
    delete[] importedNumbers;
    return 0;
}