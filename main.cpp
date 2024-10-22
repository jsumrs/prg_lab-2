#include "main.hpp"
#include <fstream>
#include <iostream>
#include <string>

// constexpr int DECIMAL { static_cast<int>('.' - '0') };

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
        if (response == 'a') {
            Number* output = add_driver(list1, list2);
            write_list_to_file(output, output_file);
        } else if (response == 'm') {
            std::cout << "Sorry, multiply is not implemented.\n";
        } else {
            std::cout << "Invalid option! Exiting...";
            return 1;
        }
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
    } else { // digits_after_dec = 0 implies that there was no decimal.
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

Number* add_driver(Number* list1, Number* list2)
{
    Stack add_stack {};
    add_stack.push(list2);
    while (!add_stack.is_empty())
        list1 = add(list1, add_stack.pop(), &add_stack);
    return list1;
}

Number* add(Number* a, Number* b, Stack* add_stack)
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
        int new_data = 0;
        if (a_decimals_count > b_decimals_count) {
            new_data = a->digit;
            a_decimals_count--;
            a = a->next;
        } else if (a_decimals_count < b_decimals_count) {
            new_data = b->digit;
            b_decimals_count--;
            b = b->next;
        } else {
            new_data = a->digit + b->digit;
            a = a->next;
            b = b->next;
            a_decimals_count--;
            b_decimals_count--;
            if (new_data > 9) {
                add_stack->push(pad_carry(new_data / 10, c_decimals_count + 1));
                new_data %= 10;
            }
        }
        Number* temp = new Number(new_data);
        c->next = temp;
        c = temp;
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
        Number* temp = new Number(sum);
        c->next = temp;
        c = temp;
        digit_pos++;
    }
    // Add the left over nodes to the front of c.
    if (a != nullptr) {
        c->next = a;
        c = c->next;
    } else if (b != nullptr) {
        c->next = b;
        c = c->next;
    }

    c_head->digit = c_decimals_count;
    a = c_head;
    return a;
}

Number* pad_carry(const int carry, const int power)
{ // Return a node which represents carry * 10^power.
    Number* subject = new Number();
    Number* head = subject;
    for (int i = power; i > 0; i--) {
        Number* t = new Number();
        subject->next = t;
        subject = t;
    }
    subject->digit = carry;
    return head;
}

// Number* multiply(Number* a, Number* b)
// {
//     Stack* to_add = new Stack(); // List of lists of sums to add later.
//     Stack* q_head = to_add;
//     a = a->next; // Move past dummy;
//     b = b->next;
//     Number* b_head = b;
//     int placeA = 0;
//     while (a != nullptr) {
//         if (a->digit == DECIMAL) {
//             a = a->next;
//             continue;
//         }
//         Number* products = new Number();
//         Number* prd_head = products;
//         int placeB = placeA + 1;
//         while (b != nullptr) {
//             if (b->digit == DECIMAL || b->digit == 0) {
//                 b = b->next;
//                 continue;
//             }
//             int product = (a->digit) * (b->digit);
//             if (product > 10) { // Carry will be added later.
//                 to_add->next = create_padded_carry(product, placeB);
//                 to_add = to_add->next;
//             }
//             Number* t = new Node(product % 10);
//             products->next = t;
//             products = t;
//             placeB++;
//             b = b->next;
//         }
//         Stack* qt = new Stack();
//         qt->data = prd_head;
//         to_add->next = qt;
//         to_add = qt;
//         placeA++;
//         a = a->next;
//         b = b_head;
//     }
//     return add_queue(q_head);
// }
