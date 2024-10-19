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

    Node* list3 = add(list1, list2);
}

Node* read_file_to_list(std::string filename)
{
    std::ifstream input { filename };
    if (input) {
        Node* subject = new Node(); 
        bool decFound = false; 
        char data; // Use a char to allow the decimal point.
        while (input >> data) {
            if (data == '.')
                decFound = true;
            subject->data = static_cast<int>(data - '0'); // Cast back to int. Decimal becomes "-2".
            subject = new Node(subject); // Move along the pointer.
        }
        if (!decFound)  // No decimal found, create a decimal node to make calculations on the list easier.
            return new Node(new Node(subject, static_cast<int>('.' - '0')));
        else
            return new Node(subject);
    } else {
        std::cout << "Something went wrong opening file.\n";
        return nullptr;
    }
}

void print_list(Node* head)
{ // Print a list out, ignoring the dummy head.
    if (head != nullptr) 
        while ((head = head->next) != nullptr) 
            std::cout << head->data << '\n';
}

Node* add(Node* a, Node* b)
{   // Builds a new list 'c' in reverse order, which will be the correct order for the result.
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;

    Node* c;
    while (a != nullptr && b != nullptr) {
        const int decimal = -2;
        if (a->data == decimal && b->data == decimal)
            break;
        if (a->data == decimal) { // Move along the b node.
            c = new Node(c, b->data);
            b = b->next;
        } else if (b->data == decimal) { // Move along the a node.
            c = new Node(c, a->data);
            a = a->next;
        } else {
            // Do addition, remember to take care of carry.
        }
    }
    if (a != nullptr) // add the rest of a to c.
        ;
    if (b != nullptr) // add the rest of b to c.
        ;
    return nullptr;
}
