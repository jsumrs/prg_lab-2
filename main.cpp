#include "main.hpp"
#include <fstream>
#include <iostream>
#include <string>

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

    std::cout << "Would you like to add or multiply? Enter a(dd) or m(ultiply): ";
    char response = '0';
    std::cin >> response;
    Number* list1 = read_file_to_list(input_file1);
    Number* list2 = read_file_to_list(input_file2);
    if (list1 && list2) {
        Number* output;
        if (response == 'a')
            output = add(list1, list2);
        else if (response == 'm')
            output = multiply(list1, list2);
        else {
            std::cout << "Invalid option! Exiting...\n";
            return 1;
        }
        write_list_to_file(output, output_file);
    }
    return 0;
}

Number* read_file_to_list(std::string filename)
{
    std::ifstream input { filename };
    if (input) {
        Number* subject = new Number();
        bool dec_found = false;
        int digits_after_dec = 0;
        char data; // Use a char to allow the decimal point.
        while (input >> data) {
            if (data == '.') {
                dec_found = true;
                continue;
            } else if (dec_found)
                digits_after_dec++;
            subject->digit = static_cast<int>(data - '0');
            subject = new Number(0, subject); // Move along the pointer.
        }
        subject = subject->next;
        return new Number(digits_after_dec, subject); // Dummy will hold how many decimal numbers the list contains.
    } else {
        std::cout << "Couldn't open file.\n";
        return nullptr;
    }
}

void write_list_to_file(Number* head, std::string filename)
{
    reverse_list(head);
    std::ofstream output { filename };
    if (output) {
        int dec_pos = head->digit;
        head = head->next; // Skip dummy header.
        while (head != nullptr) {
            if (dec_pos-- == 0)
                output << '.' << '\n';
            else {
                output << head->digit << '\n';
                head = head->next;
            }
        }
    }
}

void reverse_list(Number*& head)
{ // Note: decimal place information is lost upon reversal.
    Number* r_head = new Number();
    const int original_decimal = head->digit;
    int count = 0;
    head = head->next;
    while (head != nullptr) {
        r_head->digit = head->digit;
        r_head = new Number(0, r_head);
        head = head->next;
        count++;
    }
    head = r_head;
    head->digit = (count - original_decimal);
}

void print_list(Number* head)
{ // Print a list out, ignoring the dummy head.
    if (head != nullptr) {
        while (head != nullptr) {
            std::cout << '[' << head->digit << ']' << "->";
            if (head->next == nullptr)
                std::cout << "∅";
            head = head->next;
        }
        std::cout << '\n';
    }
}

Number* add(Number* list1, Number* list2)
{
    Stack add_stack {};
    add_stack.push(list2);
    while (!add_stack.is_empty())
        list1 = add_helper(list1, add_stack.pop(), &add_stack);
    return list1;
}

Number* add_helper(Number* a, Number* b, Stack* add_stack)
{
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;
    int a_decimals_count = a->digit; // # of digits till '.'
    int b_decimals_count = b->digit; // # of digits till '.'
    a = a->next; // Move past dummy headers.
    b = b->next; // Move past dummy headers.

    Number* c = new Number();
    Number* c_head = c;
    int c_decimals_count = 0;
    // Deal will decimals.
    while ((a_decimals_count > 0 || b_decimals_count > 0) && (a != nullptr && b != nullptr)) {
        int sum = 0;
        if (a_decimals_count > b_decimals_count) {
            sum = a->digit;
            a_decimals_count--;
            a = a->next;
        } else if (a_decimals_count < b_decimals_count) {
            sum = b->digit;
            b_decimals_count--;
            b = b->next;
        } else {
            sum = a->digit + b->digit;
            a = a->next;
            b = b->next;
            a_decimals_count--;
            b_decimals_count--;
            if (sum > 9) {
                add_stack->push(pad_carry(sum / 10, c_decimals_count + 1));
                sum %= 10;
            }
        }
        c = c->next = new Number(sum);
        c_decimals_count++;
    }
    int digit_pos = 1;
    while (a != nullptr && b != nullptr) {
        int sum = a->digit + b->digit;
        a = a->next;
        b = b->next;
        if (sum > 9) {
            add_stack->push(pad_carry(sum / 10, digit_pos + 1));
            sum %= 10;
        }
        c = c->next = new Number(sum);
        digit_pos++;
    }
    // Add the left over nodes to the front of c.
    if (a != nullptr)
        c = c->next = a;
    else if (b != nullptr)
        c = c->next = b;

    c_head->digit = c_decimals_count;
    return c_head;
}

Number* pad_carry(const int carry, const int power)
{ // Return a node which represents carry * 10^power.
    Number* subject = new Number();
    Number* head = subject;
    for (int i = power; i > 0; i--)
        subject = subject->next = new Number();
    subject->digit = carry;
    return head;
}

Number* multiply(Number* list1, Number* list2)
{
    const int decimal_point = list1->digit + list2->digit;
    Stack mult_stack {};
    multiply_helper(list1, list2, &mult_stack);
    Number* result = new Number();
    while (!mult_stack.is_empty())
        result = add_helper(result, mult_stack.pop(), &mult_stack);
    result->digit = decimal_point;
    return result;
}

void multiply_helper(Number* a, Number* b, Stack* mult_stack)
{
    a = a->next; // Move past dummy;
    b = b->next;
    Number* b_head = b; // b_head needs to be the first non dummy head.
    int placeA = 0;
    while (a != nullptr) {
        Number* products = pad_carry(0, placeA);
        Number* prd_head {products};
        while (products->next != nullptr)
            products = products->next;
        int placeB = placeA + 1;
        while (b != nullptr) {
            int product = (a->digit) * (b->digit);
            if (product > 9) // Carry will be added later.
                mult_stack->push(pad_carry(product / 10, placeB + 1));
            products = products->next = new Number(product % 10);
            placeB++;
            b = b->next;
        }
        mult_stack->push(prd_head);
        placeA++;
        a = a->next;
        b = b_head;
    }
}
