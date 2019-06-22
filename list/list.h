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

    struct list_iterator;

    struct list_const_iterator {
        typedef T value_type;
        typedef list_const_iterator self_type;
        typedef const T& reference;
        typedef const T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int difference_type;

        list_const_iterator(std::nullptr_t) = delete;

        list_const_iterator(node_base* ptr) : ptr(ptr) {}

        self_type operator++() {
            ptr = ptr->next;
            return *this;
        }

        self_type operator--() {
            ptr = ptr->prev;
            return *this;
        }

        self_type operator++(int) {
            list_const_iterator cur = *this;
            ++(*this);
            return cur;
        }

        self_type operator--(int) {
            list_const_iterator cur = *this;
            --(*this);
            return cur;
        }

        const reference operator*() const {
            return static_cast<node<T>*>(ptr)->value;
        }

        const pointer operator->() const {
            return &static_cast<node<T>*>(ptr)->value;
        }

        bool operator==(const self_type& other) const {
            return ptr == other.ptr;
        }

        bool operator==(const list_iterator& other) const {
            return *this == list_const_iterator(other);
        }

        bool operator!=(const self_type& other) const {
            return !(*this == other);
        }

        bool operator!=(const list_iterator& other) const {
            return !(*this == list_const_iterator(other));
        }

        friend list;

    private:
        node_base* ptr = nullptr;

        node_base* get() {
            return ptr;
        }
    };

    struct list_iterator {
        typedef T value_type;
        typedef list_iterator self_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int difference_type;

        list_iterator(std::nullptr_t) = delete;

        list_iterator(node_base* ptr) : ptr(ptr) {}

        self_type operator++() {
            ptr = ptr->next;
            return *this;
        }

        self_type operator--() {
            ptr = ptr->prev;
            return *this;
        }

        self_type operator++(int) {
            list_iterator cur = *this;
            ++(*this);
            return cur;
        }

        self_type operator--(int) {
            list_iterator cur = *this;
            --(*this);
            return cur;
        }

        reference operator*() const {
            return static_cast<node<T>*>(ptr)->value;
        }

        pointer operator->() const {
            return &static_cast<node<T>*>(ptr)->value;
        }

        bool operator==(const self_type& other) const {
            return ptr == other.ptr;
        }

        bool operator==(const list_const_iterator& other) const {
            return list_const_iterator(ptr) == other;
        }

        bool operator!=(const self_type& other) const {
            return !(*this == other);
        }

        bool operator!=(const list_const_iterator& other) const {
            return !(*this == other);
        }

        operator list_const_iterator() const {
            return list_const_iterator(ptr);
        }

        friend list;

    private:
        node_base* ptr = nullptr;

        node_base* get() {
            return ptr;
        }
    };

public:
    typedef T value_type;

    typedef list_iterator iterator;
    typedef list_const_iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list() = default;

    list(const list<T>& other) : list() {
        for (const auto& i : other) {
            push_back(i);
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

    ~list() {
        clear();
    }

    bool empty() const noexcept {
        return (fake_node_.next == &fake_node_) && (fake_node_.prev == &fake_node_);
    }

    void clear() {
        while (!empty()) {
            pop_front();
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
        return const_iterator(const_cast<node_base*>(&fake_node_));
    }

    reverse_iterator rbegin() {
        return reverse_iterator(&fake_node_);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_cast<node_base*>(&fake_node_));
    }

    reverse_iterator rend() {
        return reverse_iterator(fake_node_.next);
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(fake_node_.next);
    }

    iterator insert(const_iterator pos, const T& item) {
        auto* new_node = new node<T>(item);

        new_node->prev = pos.get()->prev;
        new_node->next = pos.get();

        pos.get()->prev->next = new_node;
        pos.get()->prev = new_node;

        return iterator(new_node);
    }

    iterator erase(const_iterator pos) {
        pos.get()->prev->next = pos.get()->next;
        pos.get()->next->prev = pos.get()->prev;

        iterator res(pos.get()->next);
        delete static_cast<node<T>*>(pos.get());

        return res;
    }

    void splice(const_iterator pos, list<T>& other, const_iterator first, const_iterator last) {
        if (first == last) {
            return;
        }
        --last;

        first.get()->prev->next = last.get()->next;
        last.get()->next->prev = first.get()->prev;

        first.get()->prev = pos.get()->prev;
        last.get()->next = pos.get();

        pos.get()->prev->next = first.get();
        pos.get()->prev = last.get();
    }

    template<typename Type>
    friend void swap(list<Type>& a, list<Type>& b);
};

template<typename T>
void swap(list<T>& a, list<T>& b) {
    if (a.empty()) {
        a.fake_node_.next = &b.fake_node_;
        a.fake_node_.prev = &b.fake_node_;
    } else {
        a.fake_node_.next->prev = &b.fake_node_;
        a.fake_node_.prev->next = &b.fake_node_;
    }

    if (b.empty()) {
        b.fake_node_.next = &a.fake_node_;
        b.fake_node_.prev = &a.fake_node_;
    } else {
        b.fake_node_.next->prev = &a.fake_node_;
        b.fake_node_.prev->next = &a.fake_node_;
    }

    std::swap(a.fake_node_, b.fake_node_);
}

#endif //LIST_LIST_H
