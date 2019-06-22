#ifndef VECTOR_OPTIMIZED_VECTOR_H
#define VECTOR_OPTIMIZED_VECTOR_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <algorithm>

template<typename T>
class vector {
public:
    typedef T value_type;

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<T*> reverse_iterator;
    typedef std::reverse_iterator<const T*> const_reverse_iterator;

    vector() noexcept : cur_data_(nullptr) {}

    vector(const vector<T>& other) {
        cur_data_ = other.cur_data_;

        if (other.is_big()) {
            buffer_.big = other.buffer_.big;
            ++get_any(COUNT_INDEX);
        } else if (other.cur_data_ != nullptr) {
            new (&buffer_) T(other.buffer_.small);
            cur_data_ = (T*)&buffer_;
        }
    }

    template<typename InputIterator>
    vector(InputIterator first, InputIterator last) : vector() {
        while (first != last) {
            push_back(*first);
            ++first;
        }
    }

    ~vector() noexcept {
        if (is_big()) {
            --get_any(COUNT_INDEX);

            if (get_any(COUNT_INDEX) == 0) {
                for (size_t i = 0; i < size(); ++i) {
                    cur_data_[i].~T();
                }
                free(buffer_.big);
            }
        } else {
            if (cur_data_ != nullptr) {
                buffer_.small.~T();
            }
        }
    }

    vector<T>& operator=(const vector<T>& other) {
        if (this == &other) {
            return *this;
        }

        vector<T> tmp(other);
        swap(tmp, *this);
        return *this;
    }

    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last) {
        *this = vector(first, last);
    }

    T& operator[](size_t index) {
        make_unique();
        return cur_data_[index];
    }

    const T& operator[](size_t index) const noexcept {
        return cur_data_[index];
    }

    T& front() {
        return (*this)[0];
    }

    const T& front() const noexcept {
        return (*this)[0];
    }

    T& back() {
        return (*this)[size() - 1];
    }

    const T& back() const noexcept {
        return (*this)[size() - 1];
    }

    void push_back(const T& item) {
        T item_copy(item);

        if (is_small() && cur_data_ == nullptr) {
            cur_data_ = (T*)&buffer_;

            try {
                new (cur_data_) T(item_copy);
            } catch (...) {
                cur_data_ = nullptr;
                throw;
            }

            return;
        }

        if (is_small()) {
            make_big();
        }

        make_unique();

        if (get_any(SIZE_INDEX) == get_any(CAPACITY_INDEX)) {
            change_capacity(get_any(CAPACITY_INDEX) * 2);
        }

        try {
            ++get_any(SIZE_INDEX);
            new (cur_data_ + size() - 1) T(item_copy);
        } catch (...) {
            --get_any(SIZE_INDEX);
            throw;
        }
    }

    void pop_back() {
        if (is_small()) {
            buffer_.small.~T();
            cur_data_ = nullptr;
            return;
        }

        make_unique();

        --get_any(SIZE_INDEX);
        cur_data_[size()].~T();
    }

    T* data() {
        make_unique();
        return cur_data_;
    }

    const T* data() const noexcept {
        return cur_data_;
    }

    iterator begin() {
        make_unique();
        return cur_data_;
    }

    const_iterator begin() const noexcept {
        return cur_data_;
    }

    iterator end() {
        make_unique();
        return cur_data_ + size();
    }

    const_iterator end() const noexcept {
        return cur_data_ + size();
    }

    reverse_iterator rbegin() {
        return (reverse_iterator)(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return (const_reverse_iterator)(end());
    }

    reverse_iterator rend() {
        return (reverse_iterator)(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return (const_reverse_iterator)(begin());
    }

    bool empty() const noexcept {
        return size() == 0;
    }

    size_t size() const noexcept {
        if (cur_data_ == nullptr) {
            return 0;
        } else if (is_small()) {
            return 1;
        } else {
            return get_any(SIZE_INDEX);
        }
    }

    void reserve(size_t new_size) {
        if (is_small()) {
            make_big();
        }

        change_capacity(new_size);
    }

    size_t capacity() const noexcept {
        if (is_small()) {
            return 1;
        } else {
            return get_any(CAPACITY_INDEX);
        }
    }

    void shrink_to_fit() {
        if (is_small()) {
            return;
        }

        change_capacity(size());
    }

    void resize(size_t new_size, const T& item) {
        vector<T> tmp = *this;

        if (new_size > size()) {
            tmp.reserve(new_size);

            for (size_t i = size(); i < new_size; ++i) {
                tmp.push_back(item);
            }
        } else {
            while (size() > new_size) {
                tmp.pop_back();
            }
        }

        swap(tmp, *this);
    }

    void clear() {
        if (is_big()) {
            make_unique();
            get_any(SIZE_INDEX) = 0;
        } else if (cur_data_ != nullptr) {
            buffer_.small.~T();
            cur_data_ = nullptr;
        }
    }

    iterator insert(const_iterator it, const T& item) {
        make_unique();

        size_t index = it - begin();
        push_back(item);
        for (size_t i = size() - 1; i > index; --i) {
            std::swap(cur_data_[i], cur_data_[i - 1]);
        }

        return begin() + index;
    }

    iterator erase(const_iterator it) {
        make_unique();

        size_t index = it - begin();
        for (size_t i = index; i < size() - 1; ++i) {
            std::swap(cur_data_[i], cur_data_[i + 1]);
        }
        pop_back();

        return begin() + index;
    }

    iterator erase(const_iterator first, const_iterator last) {
        make_unique();

        size_t first_index = first - begin();
        size_t last_index = last - begin();
        size_t len = last_index - first_index;

        for (size_t i = first_index; i + len < size(); ++i) {
            std::swap(cur_data_[i], cur_data_[i + len]);
        }
        for (size_t i = 0; i < len; ++i) {
            pop_back();
        }

        return begin() + first_index;
    }

    template<typename Type>
    friend void swap(vector<Type>& a, vector<Type>& b);

private:
    static const size_t CAPACITY_INDEX = 0;
    static const size_t SIZE_INDEX = 1;
    static const size_t COUNT_INDEX = 2;

    union optimized_data {
        T small;
        void* big;

        optimized_data() : big(nullptr) {}
        ~optimized_data() {}
    };

    T* cur_data_;
    optimized_data buffer_;

    bool is_small() const noexcept {
        return (cur_data_ == nullptr) || (cur_data_ == (T*)&buffer_);
    }

    bool is_big() const noexcept {
        return !is_small();
    }

    size_t& get_any(size_t index) noexcept {
        size_t* ptr = (size_t*)buffer_.big;
        return ptr[index];
    }

    size_t get_any(size_t index) const noexcept {
        size_t* ptr = (size_t*)buffer_.big;
        return ptr[index];
    }

    void make_big() {
        size_t size = (cur_data_ == nullptr) ? 0 : 1;

        void* ptr = malloc(3 * sizeof(size_t) + 2 * sizeof(T));
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }

        if (size == 0) {
            buffer_.big = ptr;
            cur_data_ = (T*)((size_t*)ptr + 3);
        } else {
            cur_data_ = (T*)((size_t*)ptr + 3);

            try {
                new (cur_data_) T(buffer_.small);
            } catch (...) {
                cur_data_ = (T*)&buffer_;
                free(ptr);
                throw;
            }

            buffer_.small.~T();
            buffer_.big = ptr;
        }

        get_any(CAPACITY_INDEX) = 2;
        get_any(SIZE_INDEX) = size;
        get_any(COUNT_INDEX) = 1;
    }

    void make_unique() {
        if (is_small() || get_any(COUNT_INDEX) == 1) {
            return;
        }

        --get_any(COUNT_INDEX);
        size_t size = get_any(SIZE_INDEX);
        size_t capacity = get_any(CAPACITY_INDEX);

        void* ptr = malloc(3 * sizeof(size_t) + capacity * sizeof(T));
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }

        T* new_data = (T*)((size_t*)ptr + 3);

        size_t i = 0;
        try {
            for (; i < size; ++i) {
                new (new_data + i) T(cur_data_[i]);
            }
        } catch (...) {
            for (; i-- != 0;) {
                new_data[i].~T();
            }

            ++get_any(COUNT_INDEX);
            free(ptr);

            throw;
        }

        cur_data_ = new_data;
        buffer_.big = ptr;

        get_any(SIZE_INDEX) = size;
        get_any(CAPACITY_INDEX) = capacity;
        get_any(COUNT_INDEX) = 1;
    }

    void change_capacity(size_t new_capacity) {
        make_unique();

        size_t size = get_any(SIZE_INDEX);

        void* ptr = malloc(3 * sizeof(size_t) + new_capacity * sizeof(T));
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }

        T* new_data = (T*)((size_t*)ptr + 3);

        size_t i = 0;
        try {
            for (; i < size; ++i) {
                new (new_data + i) T(cur_data_[i]);
            }
        } catch (...) {
            for (; i-- != 0;) {
                new_data[i].~T();
            }

            free(ptr);
            throw;
        }

        for (i = 0; i < size; ++i) {
            cur_data_[i].~T();
        }
        free(buffer_.big);

        buffer_.big = ptr;
        cur_data_ = new_data;

        get_any(SIZE_INDEX) = size;
        get_any(CAPACITY_INDEX) = new_capacity;
        get_any(COUNT_INDEX) = 1;
    }
};

template<typename T>
void swap(vector<T>& a, vector<T>& b) {
    if (a.is_small() && b.is_small()) {
        if (a.cur_data_ != nullptr && b.cur_data_ != nullptr) {
            std::swap(a.buffer_.small, b.buffer_.small);
        } else if (a.cur_data_ != nullptr) {
            new ((T*)&b.buffer_) T(a.buffer_.small);
            a.buffer_.small.~T();
        } else if (b.cur_data_ != nullptr) {
            new ((T*)&a.buffer_) T(b.buffer_.small);
            b.buffer_.small.~T();
        }
    } else if (a.is_small() && b.is_big()) {
        void* tmp = b.buffer_.big;

        if (a.cur_data_ != nullptr) {
            new ((T*)&b.buffer_) T(a.buffer_.small);
            a.buffer_.small.~T();
        }

        a.buffer_.big = tmp;
    } else if (a.is_big() && b.is_small()) {
        void* tmp = a.buffer_.big;

        if (b.cur_data_ != nullptr) {
            new ((T*)&a.buffer_) T(b.buffer_.small);
            b.buffer_.small.~T();
        }

        b.buffer_.big = tmp;
    } else {
        std::swap(a.buffer_.big, b.buffer_.big);
    }

    if (a.is_small() && a.cur_data_ != nullptr) {
        a.cur_data_ = (T*)&b.buffer_;
    }
    if (b.is_small() && b.cur_data_ != nullptr) {
        b.cur_data_ = (T*)&a.buffer_;
    }

    std::swap(a.cur_data_, b.cur_data_);
}

template<typename T>
bool operator==(const vector<T>& a, const vector<T>& b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool operator!=(const vector<T>& a, const vector<T>& b) {
    return !(a == b);
}

template<typename T>
bool operator<(const vector<T>& a, const vector<T>& b) {
    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }

    return a.size() < b.size();
}

template<typename T>
bool operator>(const vector<T>& a, const vector<T>& b) {
    return b < a;
}

template<typename T>
bool operator<=(const vector<T>& a, const vector<T>& b) {
    return !(a > b);
}

template<typename T>
bool operator>=(const vector<T>& a, const vector<T>& b) {
    return !(a < b);
}

#endif //VECTOR_OPTIMIZED_VECTOR_H
