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

struct Queue {
  Queue* next;
  Node* data;

  Queue(): next(nullptr), data(nullptr) {}
  Queue(Node* n): next(nullptr), data(n) {}
};

void print_list(Node*);
void write_list_to_file(Node*, std::string filename);
Node* read_file_to_list(std::string filename);
Node* add_driver(Node*, Node*);
Node* add(Node*, Node*, Queue*);
Node* multiply(Node*, Node*);
Node* pad_carry(const int, const int);
Node* add_queue(Queue*);
Queue* create_padded_carry(const int, const int);
