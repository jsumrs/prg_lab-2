#include "main.hpp"
#include <fstream>
#include <iostream>

constexpr int DECIMAL { static_cast<int>('.' - '0') };

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
    print_list(list3);
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
        if (!decFound) // No decimal found, create a decimal node to make calculations on the list easier.
            return new Node(new Node(subject, DECIMAL));
        else
            return new Node(subject);
    } else {
        std::cout << "Something went wrong opening file.\n";
        return nullptr;
    }
}

void print_list(Node* head)
{ // Print a list out, ignoring the dummy head.
    if (head != nullptr) {
        while ((head = head->next) != nullptr)
            std::cout <<'['<< head->data << ']' << "->";
        std::cout << '\n';
    }
}

Node* add(Node* a, Node* b)
{ // Builds a new list 'c' in reverse order, which will be the correct order for the result.
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;
    int a_decimals_count = count_decimals(a);
    int b_decimals_count = count_decimals(b);
    Node* c;
    while (a != nullptr && b != nullptr) {
        if (a->data == DECIMAL || b_decimals_count > a_decimals_count) { // Move along the b node.
            b_decimals_count -= b_decimals_count > a_decimals_count ? 1 : 0;
            c = new Node(c, b->data);
            b = b->next;
        } else if (b->data == DECIMAL || a_decimals_count > b_decimals_count) { // Move along the a node.
            a_decimals_count -= a_decimals_count > b_decimals_count ? 1 : 0;
            c = new Node(c, a->data);
            a = a->next;
        } else {
            const int result = a->data + b->data;
            if (result > 9) {
                propagate_carry(a, result);
                c = new Node(c, result % 10);
            } else {
                c = new Node(c, result);
                a = a->next;
                b = b->next;
            }
        }
    }
    if (a != nullptr) // add the rest of a to c.
        c->next = a;
    if (b != nullptr) // add the rest of b to c.
        c->next = b;
    return c;
}

int count_decimals(Node* n)
{ // count the number of digits before a decimal is found.
    int count = 0;
    while (n && n->data != DECIMAL) {
        n = n->next;
        count++;
    }
    return count;
}

void propagate_carry(Node* head, int result)
{ // Propagate a carry throughout the list so that all digits are 0 to 9.
    // Function destroys the original data.
    Node* subject = head;
    subject->data += result;
    while (subject) {
        if (subject->data != DECIMAL) {
            if (subject->data > 9) {
                const int diff = subject->data - 9;
                subject->data %= 10;
                if (subject->next)
                    subject->next->data += diff;
                else
                    subject->next = new Node(diff);
            }
        }
        subject = subject->next;
    }
    subject = nullptr; // Cleanup
}
