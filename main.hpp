#pragma once

#include <string>

struct Node {
  Node* next;
  int data;
  Node(): next(nullptr), data(0){}
  Node(int d) : next(nullptr), data(d) {}
  Node(Node* n) : next(n), data(0) {}
  Node(Node* n, int d) : next(n), data(d) {}
};

void print_list(Node*);
void write_list_to_file(Node*, std::string filename);
Node* read_file_to_list(std::string filename);
Node* add(Node*, Node*);
Node* multiply(Node*, Node*);
int count_decimals(Node*);
void propagate_carry(Node*, int);
void add_data(Node**, Node**, Node**);
