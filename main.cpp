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

    std::cout << "Would you like to add or multiply? Enter a(dd) or m(ultiply): ";
    char response = '0';
    std::cin >> response;

    Node* list1 = read_file_to_list(input_file1);
    Node* list2 = read_file_to_list(input_file2);
    print_list(list1);
    print_list(list2);
    if (response == 'a') {
        // add
        if (list1 && list2)
            write_list_to_file(add_driver(list1, list2), output_file);
    } else if (response == 'm') {
        // multiply
        write_list_to_file(multiply(list1, list2), output_file);
    } else {
        std::cout << "Invalid option! Exiting...";
        return 1;
    }

    return 0;
}

Node* read_file_to_list(std::string filename)
{
    std::ifstream input { filename };
    if (input) {
        Node* subject = new Node();
        bool dec_found = false;
        int digits_after_dec = 0;
        char data; // Use a char to allow the decimal point.
        while (input >> data) {
            if (data == '.') {
                dec_found = true;
                continue;
            } else if (dec_found)
                digits_after_dec++;
            subject->data = static_cast<int>(data - '0'); // Cast back to int. Decimal becomes "-2".
            subject = new Node(subject); // Move along the pointer.
        }
        subject = subject->next;
        return new Node(subject, digits_after_dec); // Dummy will hold how many decimal numbers the list contains.
    } else { // digits_after_dec = 0 implies that there was no decimal.
        std::cout << "Couldn't open file.\n";
        return nullptr;
    }
}

void write_list_to_file(Node* head, std::string filename)
{
    std::ofstream output { filename };
    if (output) {
        int dec_pos = head->data;
        head = head->next; // Skip dummy header.
        while (head != nullptr) {
            if (dec_pos-- == 0)
                output << '.' << '\n';
            else {
                output << head->data << '\n';
                head = head->next;
            }
        }
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

Node* add_driver(Node* list1, Node* list2)
{
    Queue* q_subject = new Queue();
    Queue* main_q_head = q_subject;
    q_subject->next = new Queue(list1);
    q_subject = q_subject->next;
    q_subject->next = new Queue(list2);
    q_subject = main_q_head->next;
    while (main_q_head->next != nullptr) {
        q_subject->data = add(q_subject->data, q_subject->next->data, q_subject);
        q_subject = main_q_head;
    }
    return main_q_head->data;
}
Node* add(Node* a, Node* b, Queue* subject_q)
{ 
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;

    std::cout << "Adding: \n";
    print_list(a);
    std::cout << "with: \n";
    print_list(b);
    
    int a_decimals_count = a->data; // # of digits till '.'
    int b_decimals_count = b->data; // # of digits till '.'
    std::cout << "a: " << a->data << " b: " << b->data << '\n';
    a = a->next; // Move past dummy headers.
    b = b->next; // Move past dummy headers.

    Node* c = new Node();
    Node* c_head = c;
    int c_decimals_count = 0;
    // Deal will decimals.
    while (a_decimals_count > 0 && b_decimals_count > 0) {
    std::cout << "a: " << a_decimals_count << " b: " << b_decimals_count << '\n';
        int new_data = 0;
        if (a_decimals_count > b_decimals_count) {
            new_data = a->data;
            a_decimals_count--;
            a = a->next;
        } else if (a_decimals_count < b_decimals_count) {
            new_data = b->data;
            b_decimals_count--;
            b = b->next;
        } else {
            new_data = a->data + b->data;
            a = a->next;
            b = b->next;
            a_decimals_count--;
            b_decimals_count--;
            if (new_data > 9) {
                // Add the carry to the queue.
                subject_q->next->data = pad_carry(new_data / 10, c_decimals_count + 1);
                subject_q = subject_q->next;
                new_data %= 10;
            }
        }
        Node* temp = new Node(new_data);
        c->next = temp;
        c = temp;
        c_decimals_count++;
    }

    int digit_pos = c_decimals_count + 1;
    while (a != nullptr || b != nullptr) {
        int sum = a->data + b->data;
        if (sum > 9) {
            subject_q->next->data = pad_carry(sum / 10, digit_pos + 1);
            subject_q = subject_q->next;
            sum %= 10;
        }
        Node* temp = new Node(sum);
        c->next = temp;
        c = temp;
        digit_pos++;
    }
    // Add the left over nodes to the front of c.
    if (a != nullptr) {
        c->next = a;
        c = c->next;
    }
    else if (b != nullptr) {
        c->next = b;
        c = b->next;
    }

    c_head->data = c_decimals_count;   
    return c;
}

Node* multiply(Node* a, Node* b)
{
    Queue* to_add = new Queue(); // List of lists of sums to add later.
    Queue* q_head = to_add;
    a = a->next; // Move past dummy;
    b = b->next;
    Node* b_head = b;
    int placeA = 0;
    while (a != nullptr) {
        if (a->data == DECIMAL) {
            a = a->next;
            continue;
        }
        Node* products = new Node();
        Node* prd_head = products;
        int placeB = placeA + 1;
        while (b != nullptr) {
            if (b->data == DECIMAL || b->data == 0) {
                b = b->next;
                continue;
            }
            int product = (a->data) * (b->data);
            if (product > 10) { // Carry will be added later.
                to_add->next = create_padded_carry(product, placeB);
                to_add = to_add->next;
            }
            Node* t = new Node(product % 10);
            products->next = t;
            products = t;
            placeB++;
            b = b->next;
        }
        Queue* qt = new Queue();
        qt->data = prd_head;
        to_add->next = qt;
        to_add = qt;
        placeA++;
        a = a->next;
        b = b_head;
    }
    return add_queue(q_head);
}



Node* pad_carry(const int carry, const int power)
{ // Return a node which represents carry * 10^power.
    Node* subject = new Node();
    Node* head = subject;
    for (int i = power; i > 0; i--) {
        Node* t = new Node();
        subject->next = t;
        subject = t;
    }
    subject->data = carry;
    return head;
}

Node* add_queue(Queue* subject)
{
    if (subject == nullptr || subject->next == nullptr)
        return nullptr;
    Queue* q_head = subject->next; // first place
    subject = subject->next;
    while (subject->next != nullptr) {
        // keep adding till q is only one list.
        // if a new carry happens, just add it to the q
        Node* a = q_head->data->next;
        Node* b = subject->next->data->next;
        int place = 1;
        Node* a_prev = new Node(a); // Drag this node behind a.
        while (a != nullptr && b != nullptr) {
            if (a->data == DECIMAL) {
                a = a->next;
                continue;
            }
            if (b->data == DECIMAL) {
                b = b->next;
                continue;
            }
            int result = a->data + b->data;
            if (result < 10) {
                a->data = result;

            } else {
                Queue* q_temp = q_head->next;
                q_head->next = create_padded_carry(result, place);
                q_head->next->next = q_temp;
            }
            a = a->next;
            a_prev = a_prev->next;
            b = b->next;
        }

        // Nothing needs to be done if a was longer that b.
        if (b != nullptr)
            a_prev->next = b;
        subject = subject->next;
    }
    return q_head->data;
}

Queue* create_padded_carry(const int result, const int place)
{
    Queue* q_temp = new Queue();
    q_temp->data = pad_carry(result / 10, place + 1);
    return q_temp;
}
