```cpp
#include "main.hpp"

void Stack::push(Number* n)
{
    Node* temp = head->next;
    head->next = new Node { n };
    head->next->next = temp;
}

Number* Stack::pop()
{
    if (head->next) {
        Node* temp = head->next;
        head->next = head->next->next;
        return temp->number;
    } else {
        return nullptr;
    }
}

bool Stack::is_empty()
{
    return head->next == nullptr;
}
































```
