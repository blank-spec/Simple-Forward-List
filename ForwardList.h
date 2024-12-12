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

    Node* head_;
    Node* tail_;
    size_t size_;

    void copy_from(const LinkedList& other) {
        if (!other.head_) return;

        head_ = new Node(other.head_->data);
        Node* curr = head_;
        Node* other_curr = other.head_->next;

        while (other_curr) {
            curr->next = new Node(other_curr->data);
            curr = curr->next;
            other_curr = other_curr->next;
        }
        tail_ = curr;
        size_ = other.size_;
    }

public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

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

    iterator begin() noexcept { return iterator(head_); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(head_); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept { return const_iterator(head_); }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }

    LinkedList() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}

    explicit LinkedList(const T& value) : head_(new Node(value)), tail_(head_), size_(1) {}

    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
        copy_from(other);
    }

    LinkedList(LinkedList&& other) noexcept
        : head_(std::exchange(other.head_, nullptr))
        , tail_(std::exchange(other.tail_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
    }

    LinkedList& operator=(LinkedList other) noexcept {
        swap(*this, other);
        return *this;
    }

    friend void swap(LinkedList& first, LinkedList& second) noexcept {
        using std::swap;
        swap(first.head_, second.head_);
        swap(first.tail_, second.tail_);
        swap(first.size_, second.size_);
    }

    T& operator[](size_t index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        Node* curr = head_;
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
        if (!head_) {
            head_ = tail_ = new_node;
        }
        else {
            tail_->next = new_node;
            tail_ = new_node;
        }
        ++size_;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head_;
        head_ = new_node;
        if (!tail_) tail_ = head_;
        ++size_;
    }

    T& front() {
        if (!head_) throw runtime_error("List is empty");
        return head_->data;
    }

    const T& front() const {
        if (!head_) throw runtime_error("List is empty");
        return head_->data;
    }

    T& back() {
        if (!tail_) throw runtime_error("List is empty");
        return tail_->data;
    }

    const T& back() const {
        if (!tail_) throw runtime_error("List is empty");
        return tail_->data;
    }

    void reverse() noexcept {
        if (!head_ || !head_->next) return;

        Node* prev = nullptr;
        Node* curr = head_;
        tail_ = head_;

        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head_ = prev;
    }

    void merge(LinkedList& other) {
        if (this == &other) return;

        if (!head_) {
            head_ = std::exchange(other.head_, nullptr);
            tail_ = std::exchange(other.tail_, nullptr);
            size_ = std::exchange(other.size_, 0);
            return;
        }

        if (!other.head_) return;

        tail_->next = other.head_;
        tail_ = other.tail_;
        size_ += other.size_;

        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] size_t getSize() const noexcept { return size_; }

    void insert(const int index, const T& value) {
        if (index < 0 || index > size_)
            throw out_of_range("Index out of bound");

        size_++;
        Node* newNode = new Node(value);

        if (index == 0) {
            newNode->next = head_;
            head_ = newNode;
            if (!tail_) tail_ = head_;
            return;
        }

        Node* temp = head_;
        for (int i = 0; i < index - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp != nullptr) {
            newNode->next = temp->next;
            temp->next = newNode;
            if (!tail_) tail_ = newNode;
        }
    }

    bool equals(const T& a, const T& b) {
        if constexpr (is_same_v<T, pair<typename T::first_type, typename T::second_type>>) {
            return a.first == b.first;
        }
        else {
            return a == b;
        }
    }

    bool remove(const T& value) {
        if (!head_) {
            return false;
        }

        if (equals(head_->data, value)) {
            Node* temp = head_;
            head_ = head_->next;
            if (!head_) {
                tail_ = nullptr;
            }
            delete temp;
            --size_;
            return true;
        }

        Node* current = head_;
        while (current->next != nullptr) {
            if (equals(current->next->data, value)) {
                Node* temp = current->next;
                current->next = temp->next;
                if (temp == tail_) {
                    tail_ = current;
                }
                delete temp;
                --size_;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void pop_front() {
        if (!head_) {
            return;
        }
        Node* temp = head_;
        head_ = head_->next;
        if (!head_) {
            tail_ = nullptr;
        }
        delete temp;
        --size_;
    }

    void pop_back() {
        if (!head_) {
            return;
        }

        if (head_ == tail_) {
            delete head_;
            head_ = tail_ = nullptr;
            size_ = 0;
            return;
        }

        Node* current = head_;
        while (current->next != tail_) {
            current = current->next;
        }

        delete tail_;
        tail_ = current;
        tail_->next = nullptr;
        --size_;
    }

    void printList() const {
        Node* temp = head_;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "nullptr" << endl;
    }

    int find_index(const T& value) const {
        Node* current = head_;
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

    size_t size() const {
        return size_;
    }

    ~LinkedList() {
        if (!head_)
            return;

        Node* temp = head_;
        while (temp) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head_ = nullptr;
    }
};
