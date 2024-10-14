#pragma once
#include <iostream>
using namespace std;

template <class T>
class LinkedList {
private:
    template<class T>
    struct Node {
        T data;
        Node<T>* next;

        Node(const T& value) : data(value), next(nullptr) {}
        Node() : data(T()), next(nullptr) {}
    };

    using TemTemp = Node<T>*;
    TemTemp head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    LinkedList(const T& value) : head(new Node<T>(value)), size(1) {}

    LinkedList(const LinkedList& other) : head(nullptr), size(other.size) {
        if (other.head) {
            head = new Node<T>(other.head->data);
            TemTemp temp = head;
            TemTemp otherTemp = other.head->next;

            while (otherCurrent) {
                temp->next = new Node<T>(otherTemp->data);
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
                head = new Node<T>(other.head->data);
                TemTemp temp = head;
                TemTemp otherTemp = other.head->next;

                while (otherTemp) {
                    temp->next = new Node<T>(otherTemp->data);
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
        }
    }

    void pop_front()                 { remove(0); }

    void pop_back()                  { remove(size - 1); }

    void push_front(const T& value)  { insert(0, value); }

    void push_back(const T& value)   { insert(size, value); }

    int& at(const int index) const {
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

    int getSize() const { return size; }

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
