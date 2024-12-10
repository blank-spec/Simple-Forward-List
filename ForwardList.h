#pragma once
#include <iostream>
#include <utility>

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

    Node* head;
    Node* tail;  // Добавляем указатель на последний элемент
    size_t size;

    void copy_from(const LinkedList& other) {
        if (!other.head) return;

        head = new Node(other.head->data);
        Node* curr = head;
        Node* other_curr = other.head->next;

        while (other_curr) {
            curr->next = new Node(other_curr->data);
            curr = curr->next;
            other_curr = other_curr->next;
        }
        tail = curr;
        size = other.size;
    }

public:
    template <bool isConst>
    class baseIterator {
    private:
        Node* current;

    public:
        baseIterator() noexcept : current(nullptr) {}
        using iterator_category = forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = conditional_t<isConst, const T*, T*>;
        using reference = conditional_t<isConst, const T&, T&>;

        explicit baseIterator(Node* node) noexcept : current(node) {}

        reference operator*() const { return current->data; }
        pointer operator->() const { return &current->data; }

        baseIterator& operator++() noexcept {
            current = current->next;
            return *this;
        }

        baseIterator operator++(int) noexcept {
            baseIterator tmp = *this;
            current = current->next;
            return tmp;
        }

        bool operator==(const baseIterator& other) const noexcept {
            return current == other.current;
        }

        bool operator!=(const baseIterator& other) const noexcept {
            return !(*this == other);
        }
    };

    using iterator = baseIterator<false>;
    using const_iterator = baseIterator<true>;

    iterator begin() noexcept { return iterator(head); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(head); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept { return const_iterator(head); }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }

    LinkedList() noexcept : head(nullptr), tail(nullptr), size(0) {}

    explicit LinkedList(const T& value) : head(new Node(value)), tail(head), size(1) {}

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        copy_from(other);
    }

    LinkedList(LinkedList&& other) noexcept
        : head(std::exchange(other.head, nullptr))
        , tail(std::exchange(other.tail, nullptr))
        , size(std::exchange(other.size, 0)) {}

    LinkedList& operator=(LinkedList other) noexcept {
        swap(*this, other);
        return *this;
    }

    friend void swap(LinkedList& first, LinkedList& second) noexcept {
        using std::swap;
        swap(first.head, second.head);
        swap(first.tail, second.tail);
        swap(first.size, second.size);
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        Node* curr = head;
        for (size_t i = 0; i < index; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    const T& operator[](size_t index) const {
        return const_cast<LinkedList*>(this)->operator[](index);
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++size;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
        if (!tail) tail = head;
        ++size;
    }

    T& front() {
        if (!head) throw runtime_error("List is empty");
        return head->data;
    }

    const T& front() const {
        if (!head) throw runtime_error("List is empty");
        return head->data;
    }

    T& back() {
        if (!tail) throw runtime_error("List is empty");
        return tail->data;
    }

    const T& back() const {
        if (!tail) throw runtime_error("List is empty");
        return tail->data;
    }

    void reverse() noexcept {
        if (!head || !head->next) return;

        Node* prev = nullptr;
        Node* curr = head;
        tail = head;

        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    void merge(LinkedList& other) {
        if (this == &other) return;

        if (!head) {
            head = std::exchange(other.head, nullptr);
            tail = std::exchange(other.tail, nullptr);
            size = std::exchange(other.size, 0);
            return;
        }

        if (!other.head) return;

        tail->next = other.head;
        tail = other.tail;
        size += other.size;

        other.head = other.tail = nullptr;
        other.size = 0;
    }

    [[nodiscard]] bool empty() const noexcept { return size == 0; }
    [[nodiscard]] size_t getSize() const noexcept { return size; }

    void insert(const int index, const T& value) {
        if (index < 0 || index > size)
            throw out_of_range("Index out of bound");

        size++;
        Node* newNode = new Node(value);

        if (index == 0) {
            newNode->next = head;
            head = newNode;
            if (!tail) tail = head;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < index - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp != nullptr) {
            newNode->next = temp->next;
            temp->next = newNode;
            if (!tail) tail = newNode;
        }
    }

    bool equals(const T& a, const T& b) {
        if constexpr (is_same_v<T, pair<typename T::first_type, typename T::second_type>>) {
            return a.first == b.first;
        } else {
            return a == b;
        }
    }

    // Удаление элемента по значению
    bool remove(const T& value) {
        if (!head) {
            return false;
        }

        if (equals(head->data, value)) {
            Node* temp = head;
            head = head->next;
            if (!head) {
                tail = nullptr;
            }
            delete temp;
            --size;
            return true;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (equals(current->next->data, value)) {
                Node* temp = current->next;
                current->next = temp->next;
                if (temp == tail) {
                    tail = current;
                }
                delete temp;
                --size;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void pop_front() {
        if (!head) {
            return;
        }
        Node* temp = head;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        delete temp;
        --size;
    }

    void pop_back() {
        if (!head) {
            return;
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            size = 0;
            return;
        }

        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
        --size;
    }

    void printList() const {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "nullptr" << endl;
    }

    // Поиск индекса элемента по значению
    int find_index(const T& value) const {
        Node* current = head;
        int index = 0;

        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            ++index;
        }

        return -1;
    }

    ~LinkedList() {
        if (!head)
            return;

        Node* temp = head;
        while (temp) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = nullptr;
    }
};
