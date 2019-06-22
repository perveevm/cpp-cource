#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>

template<typename T>
class list {
    struct node_base {
        node_base* prev;
        node_base* next;

        node_base() : prev(this), next(this) {}
    };

    template<typename Type>
    struct node : node_base {
        Type value;

        explicit node(const Type& other) : node_base(), value(other) {}
    };

    node_base fake_node_;

    template<typename Type>
    struct list_iterator {
        node_base* ptr = nullptr;

        list_iterator() = default;

        list_iterator(node_base* ptr) : ptr(ptr) {}

        list_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        list_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        const list_iterator operator++(int) {
            list_iterator cur = *this;
            ++(*this);
            return cur;
        }

        const list_iterator operator--(int) {
            list_iterator cur = *this;
            --(*this);
            return cur;
        }

        Type& operator*() {
            return static_cast<node<Type>*>(ptr)->value;
        }

        bool operator==(const list_iterator<Type>& other) {
            return ptr == other.ptr;
        }

        bool operator!=(const list_iterator<Type>& other) {
            return !(*this == other);
        }

        node_base* get() {
            return ptr;
        }
    };

    template<typename Type>
    struct list_reverse_iterator {
        node_base* ptr = nullptr;

        list_reverse_iterator() = default;

        list_reverse_iterator(node_base* ptr) : ptr(ptr) {}

        list_reverse_iterator& operator++() {
            ptr = ptr->prev;
            return *this;
        }

        list_reverse_iterator& operator--() {
            ptr = ptr->next;
            return *this;
        }

        const list_reverse_iterator operator++(int) {
            list_reverse_iterator cur = *this;
            ++(*this);
            return cur;
        }

        const list_reverse_iterator operator--(int) {
            list_reverse_iterator cur = *this;
            --(*this);
            return cur;
        }

        Type& operator*() {
            return static_cast<node<Type>*>(ptr->prev)->value;
        }

        bool operator==(const list_reverse_iterator<Type>& other) {
            return ptr == other.ptr;
        }

        bool operator!=(const list_reverse_iterator<Type>& other) {
            return !(*this == other);
        }

        node_base* get() {
            return ptr->prev;
        }
    };

public:
    typedef T value_type;

    typedef list_iterator<T> iterator;
    typedef list_iterator<const T> const_iterator;
    typedef list_reverse_iterator<T> reverse_iterator;
    typedef list_reverse_iterator<const T> const_reverse_iterator;

    list() = default;

    list(const list<T>& other) : fake_node_() {
        node_base* first = other.fake_node_.next;

        while (first != &other.fake_node_) {
            push_back(static_cast<node<T>*>(first)->value);
            first = first->next;
        }
    }

    list<T>& operator=(const list<T>& other) {
        if (this == &other) {
            return *this;
        }

        list<T> tmp(other);
        swap(tmp, *this);
        return *this;
    }

    ~list() noexcept {
        clear();
    }

    bool empty() const noexcept {
        return (fake_node_.next == &fake_node_) && (fake_node_.prev == &fake_node_);
    }

    void clear() {
        while (!empty()) {
            pop_back();
        }
    }

    void push_back(const T& item) {
        insert(end(), item);
    }

    void pop_back() {
        erase(--end());
    }

    void push_front(const T& item) {
        insert(begin(), item);
    }

    void pop_front() {
        erase(begin());
    }

    T& front() {
        return static_cast<node<T>*>(fake_node_.next)->value;
    }

    const T& front() const noexcept {
        return static_cast<const node<T>*>(fake_node_.next)->value;
    }

    T& back() {
        return static_cast<node<T>*>(fake_node_.prev)->value;
    }

    const T& back() const noexcept {
        return static_cast<const node<T>*>(fake_node_.prev)->value;
    }

    iterator begin() {
        return iterator(fake_node_.next);
    }

    const_iterator begin() const {
        return const_iterator(fake_node_.next);
    }

    iterator end() {
        return iterator(&fake_node_);
    }

    const_iterator end() const {
        return const_iterator(&fake_node_);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(&fake_node_);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(&fake_node_);
    }

    reverse_iterator rend() {
        return reverse_iterator(fake_node_.next);
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(fake_node_.next);
    }

    template<typename InputIterator>
    InputIterator insert(InputIterator pos, const T& item) {
        auto* new_node = new node<T>(item);

        new_node->prev = pos.get()->prev;
        new_node->next = pos.get();

        pos.get()->prev->next = new_node;
        pos.get()->prev = new_node;

        return iterator(new_node);
    }

    template<typename InputIterator>
    InputIterator erase(InputIterator pos) {
        pos.get()->prev->next = pos.get()->next;
        pos.get()->next->prev = pos.get()->prev;

        iterator res(pos.get()->next);
        delete pos.get();

        return res;
    }

    template<typename InputIterator>
    void splice(InputIterator pos, list<T>& other, InputIterator first, InputIterator last) {
        if (first == last) {
            return;
        }
        --last;

        first.get()->prev = pos.get()->prev;
        last.get()->next = pos.get();

        pos.get()->prev->next = first.get();
        pos.get()->prev = last.get();
    }

    template<typename Type>
    friend void swap(list<Type>& a, list<Type>& b);

private:
};

template<typename T>
void swap(list<T>& a, list<T>& b) { // Not work!
//    a.fake_node_.prev->next = &b.fake_node_;
//    a.fake_node_.next->prev = &b.fake_node_;
//
//    b.fake_node_.prev->next = &a.fake_node_;
//    b.fake_node_.next->prev = &a.fake_node_;
//
//    std::swap(a.fake_node_, b.fake_node_);

//    auto it = a.end();
//    a.splice(a.end(), b, b.begin(), b.end());
//    b.splice(b.begin(), a, a.begin(), it);
}

#endif //LIST_LIST_H
