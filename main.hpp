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
void write_list_to_file(Node*);
Node* read_file_to_list(std::string filename);
Node* add(Node*, Node*);
Node* multiply(Node*, Node*);

