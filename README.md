# Simple-Forward-List
This is a simple implementation of forward list

## Futures
- Adding to front and back
- Removing from front and back
- Searching by index
- Searching index by value
- Removing by index
- Insert by index

## Getting statrted
Just include `ForwardList.h` to your `.cpp` file

## Getting started

```cpp
#include "ForwardList.h"

int main() {
    LinkedList<int> list;
    list.push_front(10); // 10 -> nullptr
    list.push_back(20);  // 10 -> 20 -> nullptr
    list.pop_back();     // 10 -> nullptr 
    list.insert(1, 11);  // 10 -> 11 -> nullptr
    list.pop_front();    // 11 -> nullptr
    list.print_list();
    int index = list.index(11); // index = 0

    cout << list[1] << endl; // if invalid index - return head
    cout << list.at(1) << endl; // if invalid index - will throw exception

    return 0;
}
