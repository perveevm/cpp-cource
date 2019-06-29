#ifndef LIST_SET_H
#define LIST_SET_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

template<typename T>
struct set {
    struct node_base {
        node_base* left;
        node_base* right;
        node_base* parent;

        node_base() : left(nullptr), right(nullptr), parent(this) {}
    };

    template<typename Type>
    struct node : node_base {
        Type value;

        explicit node(const Type& other) : node_base(), value(other) {}
    };

    node_base fake_node_;

    static bool is_left(node_base* ptr) {
        return ptr->parent->left == ptr;
    }

    struct set_const_iterator {
        typedef T value_type;
        typedef set_const_iterator self_type;
        typedef const T& reference;
        typedef const T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int difference_type;

        set_const_iterator() = default;

        set_const_iterator(std::nullptr_t) = delete;

        set_const_iterator(node_base* ptr) : ptr(ptr) {}

        self_type operator++() {
            if (ptr->right != nullptr) {
                ptr = ptr->right;

                while (ptr->left != nullptr) {
                    ptr = ptr->left;
                }
            } else {
                if (is_left(ptr)) {
                    ptr = ptr->parent;
                } else {
                    while (!is_left(ptr)) {
                        ptr = ptr->parent;
                    }

                    ptr = ptr->parent;
                }
            }

            return *this;
        }

        self_type operator--() {
            if (ptr->left != nullptr) {
                ptr = ptr->left;

                while (ptr->right != nullptr) {
                    ptr = ptr->right;
                }
            } else {
                if (!is_left(ptr)) {
                    ptr = ptr->parent;
                } else {
                    while (is_left(ptr)) {
                        ptr = ptr->parent;
                    }

                    ptr = ptr->parent;
                }
            }

            return *this;
        }

        self_type operator++(int) {
            set_const_iterator cur = *this;
            ++(*this);
            return cur;
        }

        self_type operator--(int) {
            set_const_iterator cur = *this;
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

        bool operator!=(const self_type& other) const {
            return !(*this == other);
        }

        friend set;

    private:
        node_base* ptr = nullptr;

        node_base* get() {
            return ptr;
        }
    };

    node_base* get_left(node_base* cur) const {
        node_base* ptr = cur;

        while (ptr->left != nullptr) {
            ptr = ptr->left;
        }

        return ptr;
    }

    set_const_iterator lower_bound(const T& value, node_base* ptr) const {
        if (ptr == nullptr) {
            return end();
        }

        if (static_cast<node<T>*>(ptr)->value == value) {
            return set_const_iterator(ptr);
        }

        if (static_cast<node<T>*>(ptr)->value < value) {
            return lower_bound(value, ptr->right);
        } else {
            set_const_iterator l = lower_bound(value, ptr->left);

            if (l == end()) {
                return set_const_iterator(ptr);
            } else {
                return l;
            }
        }
    }

public:
    typedef T value_type;

    typedef set_const_iterator iterator;
    typedef set_const_iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    set() = default;

    set(const set<T>& other) {
    	try {
	        for (const auto& item : other) {
	            insert(item);
	        }
	    } catch (...) {
	    	clear();
	    	throw;
	    }
    }

    set<T>& operator=(const set<T>& other) {
        if (this == &other) {
            return *this;
        }

        set<T> tmp(other);
        swap(tmp, *this);
        return *this;
    }

    ~set() noexcept {
        clear();
    }

    bool empty() const noexcept {
        return begin() == end();
    }

    void clear() noexcept {
        while (!empty()) {
            erase(begin());
        }
    }

    iterator begin() noexcept {
        return iterator(get_left(const_cast<node_base*>(&fake_node_)));
    }

    const_iterator begin() const noexcept {
        return const_iterator(get_left(const_cast<node_base*>(&fake_node_)));
    }

    iterator end() noexcept {
        return iterator(const_cast<node_base*>(&fake_node_));
    }

    const_iterator end() const noexcept {
        return const_iterator(const_cast<node_base*>(&fake_node_));
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(const_cast<node_base*>(&fake_node_));
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(const_cast<node_base*>(&fake_node_));
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(get_left(const_cast<node_base*>(&fake_node_)));
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(get_left(const_cast<node_base*>(&fake_node_)));
    }

    std::pair<iterator, bool> insert(const T& value) {
        node_base* prev = &fake_node_;
        node_base* cur = fake_node_.left;
        bool is_left = true;

        while (cur != nullptr) {
            if (static_cast<node<T>*>(cur)->value == value) {
                return std::make_pair(iterator(cur), false);
            }

            if (static_cast<node<T>*>(cur)->value < value) {
                prev = cur;
                cur = cur->right;
                is_left = false;
            } else {
                prev = cur;
                cur = cur->left;
                is_left = true;
            }
        }

        auto* new_node = new node<T>(value);
        new_node->parent = prev;

        if (is_left) {
            prev->left = new_node;
        } else {
            prev->right = new_node;
        }

        return std::make_pair(iterator(new_node), true);
    }

    iterator erase(const_iterator it) noexcept {
        node_base* cur = it.get();
        node_base* prev = cur->parent;
        iterator res = it;
        ++res;

        bool is_left = (prev->left == cur);

        if (cur->left == nullptr && cur->right == nullptr) {
            if (is_left) {
                prev->left = nullptr;
            } else {
                prev->right = nullptr;
            }

            delete static_cast<node<T>*>(cur);
        } else if (cur->left != nullptr && cur->right == nullptr) {
            if (is_left) {
                prev->left = cur->left;
            } else {
                prev->right = cur->left;
            }

            cur->left->parent = prev;

            delete static_cast<node<T>*>(cur);
        } else if (cur->left == nullptr && cur->right != nullptr) {
            if (is_left) {
                prev->left = cur->right;
            } else {
                prev->right = cur->right;
            }

            cur->right->parent = prev;

            delete static_cast<node<T>*>(cur);
        } else {
            node_base* min = get_left(cur->right);

            if (min->parent->left == min) {
                min->parent->left = min->right;
                if (min->right != nullptr) {
                    min->right->parent = min->parent;
                }
            } else {
                min->parent->right = min->right;
                if (min->right != nullptr) {
                    min->right->parent = min->parent;
                }
            }

            min->parent = prev;
            if (is_left) {
                prev->left = min;
            } else {
                prev->right = min;
            }

            min->left = cur->left;
            min->right = cur->right;

            if (min->left != nullptr) {
                min->left->parent = min;
            }
            if (min->right != nullptr) {
                min->right->parent = min;
            }

            delete static_cast<node<T>*>(cur);
        }

        return res;
    }

    const_iterator find(const T& value) const {
        node_base* cur = fake_node_.left;

        while (cur != nullptr) {
            if (static_cast<node<T>*>(cur)->value == value) {
                return const_iterator(cur);
            }

            if (static_cast<node<T>*>(cur)->value < value) {
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }

        return end();
    }

    const_iterator lower_bound(const T& value) const {
        return lower_bound(value, fake_node_.left);
    }

    const_iterator upper_bound(const T& value) const {
        if (find(value) != end()) {
            return ++lower_bound(value);
        } else {
            return lower_bound(value);
        }
    }

    template<typename Type>
    friend void swap(set<Type>& a, set<Type>& b);
};

template<typename T>
void swap(set<T>& a, set<T>& b) {
    if (!a.empty() && !b.empty()) {
        std::swap(a.fake_node_.left, b.fake_node_.left);
        std::swap(a.fake_node_.left->parent, b.fake_node_.left->parent);
    } else if (!a.empty() && b.empty()) {
        b.fake_node_.left = a.fake_node_.left;
        b.fake_node_.left->parent = &b.fake_node_;

        a.fake_node_.left = nullptr;
    } else if (a.empty() && !b.empty()) {
        a.fake_node_.left = b.fake_node_.left;
        a.fake_node_.left->parent = &a.fake_node_;

        b.fake_node_.left = nullptr;
    }
}

#endif //LIST_SET_H
