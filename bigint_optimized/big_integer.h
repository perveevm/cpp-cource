#ifndef BIG_INTEGER_BIG_INTEGER_H
#define BIG_INTEGER_BIG_INTEGER_H

#include <string>
#include "vector/ui_vector.h"

struct big_integer {
    typedef uint32_t ui;
    typedef uint64_t ull;

    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(ui a);
    big_integer(bool sign, ui_vector const& data);
    explicit big_integer(std::string const& str);

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& other);
    big_integer& operator-=(big_integer const& other);
    big_integer& operator*=(big_integer const& other);
    big_integer& operator/=(big_integer const& other);
    big_integer& operator%=(big_integer const& other);

    big_integer& operator&=(big_integer const& other);
    big_integer& operator|=(big_integer const& other);
    big_integer& operator^=(big_integer const& other);

    big_integer& operator<<=(ui other);
    big_integer& operator>>=(ui other);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend big_integer operator+(big_integer const& a);
    friend big_integer operator-(big_integer const& a);
    friend big_integer operator+(big_integer const& a, big_integer const& b);
    friend big_integer operator-(big_integer const& a, big_integer const& b);
    friend big_integer operator*(big_integer const& a, big_integer const& b);
    friend big_integer operator/(big_integer const& a, big_integer const& b);
    friend big_integer operator%(big_integer const& a, big_integer const& b);

    friend big_integer& operator++(big_integer& a);
    friend big_integer& operator--(big_integer& a);
    friend big_integer operator++(big_integer& a, int);
    friend big_integer operator--(big_integer& a, int);

    friend big_integer operator~(big_integer const& a);
    friend big_integer operator&(big_integer const& a, big_integer const& b);
    friend big_integer operator|(big_integer const& a, big_integer const& b);
    friend big_integer operator^(big_integer const& a, big_integer const& b);

    friend big_integer operator<<(big_integer const& a, ui b);
    friend big_integer operator>>(big_integer const& a, ui b);

    friend std::string to_string(big_integer const& a);

    bool is_zero() const;
private:
    bool sign = false;
    ui_vector data{};

    void fit();
    void fixSign();
    ui get_digit(size_t pos) const;
};

#endif //BIG_INTEGER_BIG_INTEGER_H
