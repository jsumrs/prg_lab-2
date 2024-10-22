#pragma once

#include <string>

struct Number {
    Number* next;
    int digit; // If dummy this holds the decimal position. 0 implies no decimal. 1 implies decimal after first node.
    Number(int d = 0, Number* n = nullptr)
        : next { n }
        , digit { d }
    {
    }
};

class Stack {
private:
    struct Node {
        Number* number;
        Node* next;
        Node(Number* n = nullptr, Node* ne = nullptr)
            : number { n }
            , next { ne }
        {
        }
    };

public:
    Stack()
        : head { new Node() }
    {
    }

    void push(Number*);
    Number* pop();
    bool is_empty();

private:
    Node* head;
    void init();
};

void print_list(Number*);
void write_list_to_file(Number*, std::string filename);
void reverse_list(Number*&);
Number* read_file_to_list(std::string filename);
Number* add(Number*, Number*);
Number* add_helper(Number*, Number*, Stack*);
Number* multiply(Number*, Number*);
void multiply_helper(Number*, Number*, Stack*);
Number* pad_carry(const int, const int);
Number* pad_product(const int, const int);
