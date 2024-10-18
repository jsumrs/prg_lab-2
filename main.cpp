#include <iostream>
#include <string>

struct Node {
    int data;
    Node* next;
};

int main(int argc, char* argv[])
{
    if (argc < 3) { // Ensure program was run with correct amount of arguments.
        std::string main_filename = argv[0];
        std::cout << main_filename << ": missing file operands\n";
        std::cout << main_filename << ": try '" << main_filename << " file_one file_two [output_file]'\n";
        return 1;
    }
    std::string input_file1 = argv[1];
    std::string input_file2 = argv[2];
    std::string output_file = argc < 4 ? "output_file" : argv[3]; // Output filename is optional, so define a default here.
}
