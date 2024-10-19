#pragma once

#include <iostream>
#include <istream>
#include <string>

struct Node {
  Node* next;
  int data;
};

void print_list(Node*);
void write_list_to_file(Node*);
Node* read_file_to_list(std::string filename);
Node* add(Node*, Node*);
Node* multiply(Node*, Node*);

