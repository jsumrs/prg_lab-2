#include "main.hpp"
#include <fstream>
#include <iostream>


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

    Node* list1 = read_file_to_list(input_file1);
    Node* list2 = read_file_to_list(input_file2);

    print_list(list1);
    print_list(list2);
}

Node* read_file_to_list(std::string filename)
{
    std::ifstream input { filename };
    if (input) {
        Node* subject = new Node();
        char data; // Use a char to deal with the decimal point.
        while (input >> data) {
            subject->data = static_cast<int>(data - '0'); // Cast back to int. Decimal becomes "-2".
            Node* temp = new Node();
            temp->next = subject;
            subject = temp;
        }
        Node* dummy = new Node();
        dummy->next = subject;
        return dummy;
    } else {
        std::cout << "Something went wrong opening file.\n";
        return nullptr;
    }
}

void print_list(Node* head)
{ // Print a list out, ignoring the dummy head.
    if (head != nullptr) {
        while ((head = head->next) != nullptr) {
            std::cout << head->data << '\n';
        }
    }
}
