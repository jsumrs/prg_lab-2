#include "main.hpp"
#include <fstream>
#include <iostream>
#include <string>

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

    // Output filename is optional, so define a default here.
    std::string output_file = argc < 4 ? "output_file" : argv[3]; 

    Node* list1 = read_file_to_list(input_file1);
    Node* list2 = read_file_to_list(input_file2);
    if (list1 && list2)
        write_list_to_file(add(list1, list2), output_file);
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
        subject = subject->next;
        if (!decFound) // No decimal found, create a decimal node to make calculations on the list easier.
            return new Node(new Node(subject, DECIMAL));
        else
            return new Node(subject);
    } else {
        std::cout << "Couldn't open file.\n";
        return nullptr;
    }
}

void write_list_to_file(Node* head, std::string filename)
{
    std::ofstream output { filename };
    if (output)
        while (head != nullptr) {
            int c = head->data;
            if (c == DECIMAL)
                output << '.' << '\n';
            else
                output << c << '\n';
            head = head->next;
        }
}

void print_list(Node* head)
{ // Print a list out, ignoring the dummy head.
    if (head != nullptr) {
        while (head != nullptr) {
            std::cout << '[' << head->data << ']' << "->";
            if (head->next == nullptr)
                std::cout << "∅";
            head = head->next;
        }
        std::cout << '\n';
    }
}

Node* add(Node* a, Node* b)
{ // Builds a new list 'c' in reverse order, which will be the correct order for the result.
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;

    int a_decimals_count = count_decimals(a); // # of digits till '.'
    int b_decimals_count = count_decimals(b);

    // Move past dummy headers
    a = a->next;
    b = b->next;
    Node* c = new Node();
    // std::cout << "a: " << a->data << " b: " << b->data << '\n';
    while (a->data != DECIMAL || b->data != DECIMAL) {
        if (a->data == DECIMAL || b_decimals_count > a_decimals_count) { // Move along the b node.
            b_decimals_count -= b_decimals_count > a_decimals_count ? 1 : 0;
            c->data = b->data;
            b = b->next;
            c = new Node(c);
        } else if (b->data == DECIMAL || a_decimals_count > b_decimals_count) { // Move along the a node.
            a_decimals_count -= a_decimals_count > b_decimals_count ? 1 : 0;
            c->data = a->data;
            a = a->next;
            c = new Node(c);
        } else
            add_data(&a, &b, &c);
    }

    // Move past decimals:
    a = a->next;
    b = b->next;

    c->data = DECIMAL;
    c = new Node(c);
    while (a != nullptr && b != nullptr)
        add_data(&a, &b, &c);

    // Add the left over nodes to the front of c.
    while (a != nullptr) {
        c->data = a->data;
        a = a->next;
        c = new Node(c);
    }
    while (b != nullptr) {
        c->data = b->data;
        b = b->next;
        c = new Node(c);
    }
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
    if (result < 10)
        return;
    else {
        if (head->next == nullptr)
            head->next = new Node(result / 10);
        else {
            if (head->next->data == DECIMAL)
                propagate_carry(head->next, result);
            else {
                int c = head->next->data + (result / 10);
                propagate_carry(head->next, c);
                head->next->data = c % 10;
            }
        }
    }
}

void add_data(Node** a, Node** b, Node** c)
{ // Add the data of a and b, put the result in a new node of c. Propagate any carries.
    int result = (*a)->data + (*b)->data;
    propagate_carry(*a, result);
    *a = (*a)->next;
    *b = (*b)->next;
    (*c)->data = result % 10;
    *c = new Node(*c);
}
