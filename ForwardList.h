#pragma once
#include <iostream>

#include "Vector.h"
using namespace std;

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
        Node() : data(T()), next(nullptr) {}
    };

    using TemTemp = Node*;
    TemTemp head;
    int size;

public:
    template <bool isConst>
    class baseIterator {
    private:
        Node* current;

    public:
        baseIterator(Node* node) : current(node) {}
        baseIterator() : current(nullptr) {}

        using pointerType = conditional_t<isConst, const T*, T*>;
        using referenceType = conditional_t<isConst, const T&, T&>;
        using valueType = T;
        using difference_type = ptrdiff_t;
        using iterator_category = random_access_iterator_tag;

        baseIterator(const baseIterator&) = default;
        baseIterator& operator=(const baseIterator&) = default;

        referenceType operator*() const { return current->data; }
        pointerType operator->() const { return &current->data; }

        baseIterator& operator++() {
            current = current->next;
            return *this;
        }

        baseIterator operator++(int){
            baseIterator temp = *this;
            current = current->next;
            return temp;
        }

        bool operator==(const baseIterator& other) const { return current == other.current; }
        bool operator!=(const baseIterator& other) const { return current != other.current; }
    };


    using Iterator = baseIterator<false>;
    using constIterator = baseIterator<true>;

    Iterator begin() {
        return { head };
    }

    Iterator end() {
        return { nullptr };
    }

    constIterator begin() const {
        return { head };
    }

    constIterator end() const {
        return { nullptr };
    }

    LinkedList() : head(nullptr), size(0) {}

    LinkedList(const T& value) : head(new Node(value)), size(1) {}

    LinkedList(const LinkedList& other) : head(nullptr), size(other.size) {
        if (other.head) {
            head = new Node(other.head->data);
            TemTemp temp = head;
            TemTemp otherTemp = other.head->next;

            while (otherTemp) {
                temp->next = new Node(otherTemp->data);
                temp = temp->next;
                otherTemp = otherTemp->next;
            }
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            this->~LinkedList();

            size = other.size;

            if (other.head) {
                head = new Node(other.head->data);
                TemTemp temp = head;
                TemTemp otherTemp = other.head->next;

                while (otherTemp) {
                    temp->next = new Node(otherTemp->data);
                    temp = temp->next;
                    otherTemp = otherTemp->next;
                }
            }
            else {
                head = nullptr;
            }
        }
        return *this;
    }

    LinkedList(LinkedList&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            this->~LinkedList();

            head = other.head;
            size = other.size;

            other.head = nullptr;
            other.size = 0;
        }

        return *this;
    }

    void insert(const int index, const T& value) {
        if (index < 0 || index > size)
            throw out_of_range("Index out of bound");

        size++;
        TemTemp newNode = new Node(value);

        if (index == 0) {
            newNode->next = head;
            head = newNode;
            return;
        }

        TemTemp temp = head;
        for (int i = 0; i < index - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp != nullptr) {
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }

    void remove(const int index) {
        if (!head || index < 0 || index > size)
            throw out_of_range("Index out of bound");

        TemTemp temp = head;
        --size;
        if (index == 0) {
            head = head->next;
            delete temp;
            return;
        }

        for (int i = 0; i < index - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp != nullptr && temp->next != nullptr) {
            TemTemp nodeToDelete = temp->next;
            temp->next = nodeToDelete->next;
            delete nodeToDelete;
            cout << "Deleted" << endl;
        }
    }

    void pop_front() { remove(0); }

    void pop_back() { remove(size - 1); }

    void push_front(const T& value) { insert(0, value); }

    void push_back(const T& value) { insert(size, value); }

    bool empty() const { return head == nullptr; }

    [[nodiscard]] int& at(const int index) const {
        if (index < 0 || index > size) {
            throw out_of_range("Index out of bound");
        }

        TemTemp temp = head;

        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }

        return temp->data;
    }

    int& operator[](const int index) const {
        if (index < 0 || index >= size)
            return head->data;

        TemTemp temp = head;

        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }

        return temp->data;
    }

    int index(const T& value) const {
        TemTemp temp = head;

        int count = 0;

        while (temp) {
            if (temp->data == value)
                return count;
            ++count;
            temp = temp->next;
        }

        return size;
    }

    T& front() const  {
        return head->data;
    }

    T& back() const {
        if (!head) {
            throw std::runtime_error("List is empty");
        }

        Node* node = head;

        while (node->next) {
            node = node->next;
        }
        return node->data;
    }

    bool find(const T& value) const {
        TemTemp temp = head;
        while (temp) {
            if (temp->data == value)
                return true;

            temp = temp->next;
        }
        return false;
    }

    void printList() const {
        TemTemp temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "nullptr" << endl;
    }

    [[nodiscard]] int getSize() const { return size; }

    ~LinkedList() {
        if (!head)
            return;

        TemTemp temp = head;
        while (temp) {
            TemTemp nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = nullptr;
    }
};
