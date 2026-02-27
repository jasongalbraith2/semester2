#include <iostream> // included for input and output
#include <limits> // included for std::numeric_limits
#include <iomanip> // included for std::setw
#include <algorithm> // included for std::transform and std::swap
#include <string> // included to utilize strings
#include <sstream> // included to process string of numbers
#include <cctype> // included to process string commands
#include <fstream> // included to import from file

#define MAX_NUMBERS 100

enum Method {
    IMPORT,
    INSERT,
    PRINT,
    DELETE,
    DELETE_ALL,
    QUIT,
    UNKNOWN
};

static inline void to_lower(
    std::string& inputString
) {
    std::transform(inputString.begin(), inputString.end(), inputString.begin(),
    [](unsigned char c) { return std::tolower(c); } );
}

static inline bool in_range(
    unsigned int value
) {
    return value >= 1 && value <= 1000;
}

Method process_command(std::string command) {
    to_lower(command);
    if (command == "import") { return IMPORT; }
    if (command == "insert") { return INSERT; }
    if (command == "print")  { return PRINT; }
    if (command == "quit")   { return QUIT; }
    if (command == "delete") { return DELETE; }
    if (command == "deleteall") { return DELETE_ALL; }
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

    void sub_print(
        unsigned int index,
        unsigned int depth
    ) {
        // Implementation for representing the heap
        //std::cout << "[] Max Heap: ";
        if (index >= heapSize) return;

        // Print the right side (represented as upper)
        sub_print(right(index), depth + 1);

        // Indent based on depth and print
        // the current node
        std::cout << std::setw(depth * 4) << heapArray[index] << "\n";
        
        // Print left side (represented as lower)
        sub_print(left(index), depth + 1);
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
    void delete_head() {
        // Implementation for deleting the top value from the heap
        // Effectively reverse insert
        if (heapSize != 0) {
            std::cout << "[] Head Value: " << heapArray[0] << "\n";
            
            // Swap the last value with the top value
            std::swap(heapArray[0], heapArray[heapSize - 1]);
            heapArray[heapSize - 1] = 0; // Clear the last value
            --heapSize;
            
            // Look at both children and swap with larger child if larger
            unsigned int currentIndex = 0;
            while (currentIndex < heapSize) {
                unsigned int leftIndex = left(currentIndex);
                unsigned int rightIndex = right(currentIndex);
                
                // If the left child is largest
                if (leftIndex < heapSize &&
                    heapArray[leftIndex] > heapArray[currentIndex] &&
                    heapArray[leftIndex] >= heapArray[rightIndex]
                ) {
                    std::swap(heapArray[currentIndex], heapArray[leftIndex]);
                    currentIndex = leftIndex;
                }
                
                // If the right child is the largest
                else if (rightIndex < heapSize &&
                    heapArray[rightIndex] > heapArray[currentIndex] &&
                    heapArray[rightIndex] >= heapArray[leftIndex]
                ) {
                    std::swap(heapArray[currentIndex], heapArray[rightIndex]);
                    currentIndex = rightIndex;
                }
                
                // If the new head is the largest quit
                else {
                    break;
                }
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
        std::cout << "[] Max Heap > \n";
        sub_print(0, 0);
    }

    unsigned int get_size() {
        return heapSize;
    }
};

void process_number_string(
    std::string numbers,
    Heap* heap
) {
    // Create string stream to loop through the numbers
    // Process each number and insert into the heap
    unsigned int remaining = MAX_NUMBERS - heap->get_size();
    unsigned int count = 0;
    unsigned int num;
    std::string token;
    std::stringstream ss(numbers);

    while (count < remaining && (ss >> token)) {
        num = std::stoul(token);
        if (in_range(num)) {
            heap->insert(num);
            ++count;
        }
    }
}
void process_from_file(
    const std::string& filename,
    Heap* heap
) {
    // Implementation for reading a file with space-separated
    // numbers and insert them into the heap
    std::ifstream file(filename);
    std::string numbers;
    getline(file, numbers);
    process_number_string(numbers, heap);
    file.close();
}

int main() {
    Heap* heap = new Heap();
    
    // Main loop
    bool running = true;
    std::string command; // basically any user input
    unsigned int toInsert;

    Method method;
    do {
        std::cout << "[] Enter Command (Import, Insert, Print, Delete, Quit) > ";
        std::cin >> command;
        method = process_command(command);
        
        switch (method) {
            case IMPORT: {
                // Handle import command
                std::cout << "[] Enter filename to import from > ";
                std::cin >> command;
                process_from_file(command, heap);
                break;
            }
            case INSERT: {
                // Handle insert command
                std::cout << "[] Enter numbers to insert separated by spaces > ";
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
                std::getline(std::cin, command); // read line
                process_number_string(command, heap);
                break;
            }
            case PRINT: {
                // Handle print command
                heap->represent_as_arr(); // for debugging
                heap->represent();
                break;
            }
            case DELETE: {
                // Handle delete command
                heap->delete_head();
                break;
            }
            case DELETE_ALL: {
                // Handle delete all command
                while (heap->get_size() > 0) {
                    heap->delete_head();
                }
                break;
            }
            case UNKNOWN: {
                return 1;
                // break;
            }
            default: {
                break;
            }
        }
    } while (running);
    
    // Deallocate memory
    delete heap;
    return 0;
}