#include "big_integer.h"
#include <algorithm>

typedef big_integer::ui ui;
typedef big_integer::ull ull;

void big_integer::fit() {
    while (!data.empty() && ((!sign && data.back() == 0) || (sign && data.back() == UINT32_MAX))) {
        data.pop_back();
    }
}

ui big_integer::get_digit(size_t pos) const {
    if (pos < data.size()) {
        return data[pos];
    } else if (sign) {
        return UINT32_MAX;
    } else {
        return 0;
    }
}

bool big_integer::is_zero() const {
    return !sign && data.empty();
}

big_integer::big_integer() {
    sign = false;
}

big_integer::big_integer(big_integer const& other) : sign(other.sign), data(other.data) {
    fit();
}

big_integer::big_integer(int a) : sign(a < 0), data(1) {
    data[0] = static_cast<ui>(a & UINT32_MAX);
    fit();
}

big_integer::big_integer(ui a) : sign(false), data(1) {
    data[0] = a;
    fit();
}

big_integer::big_integer(bool sign, const std::vector<ui> &data) : sign(sign), data(data) {
    fit();
}

big_integer::big_integer(std::string const& str) {
    if (str.empty()) {
        *this = 0;
        return;
    }

    *this = 0;

    size_t start = 0;
    bool is_negative = false;
    if (str[0] == '+') {
        ++start;
    } else if (str[0] == '-') {
        is_negative = true;
        ++start;
    }
    for (size_t i = start; i < str.size(); ++i) {
        *this *= 10;
        *this += str[i] - '0';
    }

    if (is_negative && !is_zero()) {
        *this = -*this;
    }
}

big_integer& big_integer::operator=(big_integer const& other) {
    big_integer tmp(other);
    std::swap(tmp.sign, sign);
    std::swap(tmp.data, data);
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& other) {
    return *this = *this + other;
}

big_integer& big_integer::operator-=(big_integer const& other) {
    return *this = *this - other;
}

big_integer& big_integer::operator*=(big_integer const& other) {
    return *this = *this * other;
}

big_integer& big_integer::operator/=(big_integer const& other) {
    return *this = *this / other;
}

big_integer& big_integer::operator%=(big_integer const& other) {
    return *this = *this % other;
}

big_integer& big_integer::operator&=(big_integer const& other) {
    return *this = *this & other;
}

big_integer& big_integer::operator|=(big_integer const& other) {
    return *this = *this | other;
}

big_integer& big_integer::operator^=(big_integer const& other) {
    return *this = *this ^ other;
}

big_integer& big_integer::operator<<=(ui other) {
    return *this = *this << other;
}

big_integer& big_integer::operator>>=(ui other) {
    return *this = *this >> other;
}

bool operator==(big_integer const& a, big_integer const& b) {
    return (a.sign == b.sign) && (a.data == b.data);
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a.sign != b.sign) {
        return a.sign;
    }
    if (a.data.size() != b.data.size()) {
        return a.data.size() < b.data.size();
    }
    for (size_t i = a.data.size(); i > 0; --i) {
        if (a.data[i - 1] != b.data[i - 1]) {
            return a.data[i - 1] < b.data[i - 1];
        }
    }
    return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
    return b < a;
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

big_integer operator+(big_integer const& a) {
    return a;
}

big_integer operator-(big_integer const& a) {
    if (a.is_zero()) {
        return a;
    } else if (a.data.empty()) {
        return big_integer(1u);
    }

    size_t n = a.data.size() + 2;
    std::vector<ui> tmp(n);
    ull sum = ~a.get_digit(0) + 1ull;
    ull carry = sum >> 32;
    tmp[0] = static_cast<ui>(sum & UINT32_MAX);

    for (size_t i = 1; i < n - 2; ++i) {
        sum = carry + static_cast<ull>(~a.data[i]);
        tmp[i] = static_cast<ui>(sum & UINT32_MAX);
        carry = sum >> 32;
    }
    for (size_t i = n - 2; i < n; ++i) {
        sum = carry + (a.sign ? 0 : UINT32_MAX);
        tmp[i] = static_cast<ui>(sum & UINT32_MAX);
        carry = sum >> 32;
    }
    bool sgn = (tmp.back() & (1 << 31)) != 0;
    return big_integer(sgn, tmp);
}

big_integer operator+(big_integer const& a, big_integer const& b) {
    size_t n = std::max(a.data.size(), b.data.size()) + 2;
    std::vector<ui> tmp(n);
    ull carry = 0;
    for (size_t i = 0; i < n; ++i) {
        ull sum = carry + a.get_digit(i) + b.get_digit(i);
        tmp[i] = static_cast<ui>(sum & UINT32_MAX);
        carry = sum >> 32;
    }
    bool sgn = (tmp.back() & (1 << 31)) != 0;
    return big_integer(sgn, tmp);
}

big_integer operator-(big_integer const& a, big_integer const& b) {
    return a + (-b); // TODO: replace this
}

void mul(std::vector<ui> const& a, std::vector<ui> const& b, std::vector<ui>& res) {
    size_t n = a.size(), m = b.size();
    for (size_t i = 0; i < n; ++i) {
        ull carry = 0;
        for (size_t j = 0; j < m; ++j) {
            ull mul = static_cast<ull>(a[i]) * static_cast<ull>(b[j]);
            ull tmp = res[i + j] + carry + (mul & UINT32_MAX);
            res[i + j] = static_cast<ui>(tmp & UINT32_MAX);
            carry = (mul >> 32) + (tmp >> 32);
        }
        res[i + m] += static_cast<ui>(carry & UINT32_MAX);
    }
}

big_integer abs(big_integer const& a) {
    return (a >= 0) ? a : -a;
}

void big_integer::fixSign() {
    if (!sign) {
        return;
    } else if (data.empty()) {
        sign = !sign;
        return;
    }

    size_t n = data.size();
    ull sum = static_cast<ull>(~data[0]) + 1ull;
    ull carry = sum >> 32;
    data[0] = static_cast<ui>(sum & UINT32_MAX);
    for (size_t i = 1; i < n; ++i) {
        sum = carry + static_cast<ull>(~data[i]);
        data[i] = static_cast<ui>(sum & UINT32_MAX);
        carry = sum >> 32;
    }
    data.push_back(static_cast<ui>((carry + UINT32_MAX) & UINT32_MAX));
    fit();
}

void mul_long_short(std::vector<ui> const& a, ui b, std::vector<ui>& res) {
    size_t n = a.size();
    res.resize(n + 1);
    ull carry = 0;
    for (size_t i = 0; i < n; ++i) {
        ull mul = static_cast<ull>(a[i]) * static_cast<ull>(b);
        ull tmp = carry + (mul & UINT32_MAX);
        res[i] = static_cast<ui>(tmp & UINT32_MAX);
        carry = (mul >> 32) + (tmp >> 32);
    }
    res[n] = static_cast<ui>(carry & UINT32_MAX);
}

big_integer operator*(big_integer const& a, big_integer const& b) {
    if (a.is_zero() || b.is_zero()) {
        return big_integer(0);
    }

    big_integer a_abs = abs(a);
    big_integer b_abs = abs(b);

    size_t n = a_abs.data.size();
    size_t m = b_abs.data.size();

    if (a_abs.data.size() > b_abs.data.size()) {
        std::swap(a_abs.data, b_abs.data);
        std::swap(a_abs.sign, b_abs.sign);
    }

    std::vector<ui> tmp(n + m + 1);

    if (a_abs.data.size() == 1) {
        mul_long_short(b_abs.data, a_abs.data[0], tmp);
    } else {
        mul(a_abs.data, b_abs.data, tmp);
    }

    big_integer res(a.sign ^ b.sign, tmp);
    res.fixSign();
    return res;
}

ui get_approx(ui a, ui b, ui c) {
    ull res = a;
    res = ((res << 32) + b) / c;
    if (res > UINT32_MAX) {
        res = UINT32_MAX;
    }
    return static_cast<ui>(res & UINT32_MAX);
}

void sub_vectors(std::vector<ui> &a, std::vector<ui> const &b) {
    ull sum = static_cast<ull>(~b[0]) + static_cast<ull>(a[0]) + 1LL;
    ull carry = sum >> 32;
    a[0] = static_cast<ui>(sum & UINT32_MAX);
    for (size_t i = 1; i < b.size(); ++i) {
        sum = static_cast<ull>(~b[i]) + static_cast<ull>(a[i]) + carry;
        a[i] = static_cast<ui>(sum & UINT32_MAX);
        carry = sum >> 32;
    }
}

bool compare_vectors(std::vector<ui> const &a, std::vector<ui> const &b) {
    for (size_t i = a.size(); i > 0; --i) {
        if (a[i - 1] != b[i - 1]) {
           return a[i - 1] < b[i - 1];
        }
    }
    return false;
}

big_integer operator/(big_integer const& a, big_integer const& b) {
    big_integer abs_a = abs(a);
    big_integer abs_b = abs(b);

    if (abs_a < abs_b) {
        return 0;
    }

    ui f = static_cast<ui>(((ull(UINT32_MAX) + 1) / (ull(abs_b.data.back()) + 1)) & UINT32_MAX);
    size_t n = abs_a.data.size();
    size_t m = abs_b.data.size();

    mul_long_short(abs_a.data, f, abs_a.data);
    mul_long_short(abs_b.data, f, abs_b.data);
    abs_a.fit();
    abs_b.fit();

    size_t l = n - m + 1;
    ui divisor = abs_b.data.back();

    std::vector<ui> temp(l);
    std::vector<ui> tmp(m + 1), div(m + 1, 0);

    for (size_t i = 0; i < m; i++) {
        tmp[i] = abs_a.data[n + i - m];
    }
    tmp[m] = abs_a.get_digit(n);

    if (abs_b.data.size() == 1) {
        for (size_t i = 0; i < l; ++i) {
            tmp[0] = abs_a.data[n - m - i];
            size_t rest = l - 1 - i;

            ui cur_approx = get_approx(tmp[m], tmp[m - 1], divisor);
            mul_long_short(abs_b.data, cur_approx, div);
            sub_vectors(tmp, div);

            for (size_t j = m; j > 0; --j) {
                tmp[j] = tmp[j - 1];
            }
            temp[rest] = cur_approx;
        }
    } else {
        for (size_t i = 0; i < l; ++i) {
            tmp[0] = abs_a.data[n - m - i];
            size_t rest = l - 1 - i;

            ui cur_approx = get_approx(tmp[m], tmp[m - 1], divisor);
            mul_long_short(abs_b.data, cur_approx, div);

            while ((cur_approx >= 0) && compare_vectors(tmp, div)) {
                mul_long_short(abs_b.data, --cur_approx, div);
            }
            
            sub_vectors(tmp, div);

            for (size_t j = m; j > 0; --j) {
                tmp[j] = tmp[j - 1];
            }
            temp[rest] = cur_approx;
        }
    }

    big_integer res(a.sign ^ b.sign, temp);
    res.fixSign();
    return res;
}

big_integer operator%(big_integer const& a, big_integer const& b) {
    return a - (a / b) * b;
}

big_integer& operator++(big_integer& a) {
    a += 1;
    return a;
}

big_integer& operator--(big_integer& a) {
    a -= 1;
    return a;
}

big_integer operator++(big_integer& a, int) {
    big_integer tmp(a);
    a += 1;
    return tmp;
}

big_integer operator--(big_integer& a, int) {
    big_integer tmp(a);
    a -= 1;
    return tmp;
}

big_integer operator~(big_integer const& a) {
    std::vector<ui> tmp(a.data.size());
    for (size_t i = 0; i < a.data.size(); ++i) {
        tmp[i] = ~a.data[i];
    }
    return big_integer(!a.sign, tmp);
}

big_integer operator&(big_integer const& a, big_integer const& b) {
    size_t n = std::max(a.data.size(), b.data.size());
    std::vector<ui> tmp(n);
    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) & b.get_digit(i);
    }
    return big_integer(a.sign & b.sign, tmp);
}

big_integer operator|(big_integer const& a, big_integer const& b) {
    size_t n = std::max(a.data.size(), b.data.size());
    std::vector<ui> tmp(n);
    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) | b.get_digit(i);
    }
    return big_integer(a.sign | b.sign, tmp);
}

big_integer operator^(big_integer const& a, big_integer const& b) {
    size_t n = std::max(a.data.size(), b.data.size());
    std::vector<ui> tmp(n);
    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) ^ b.get_digit(i);
    }
    return big_integer(a.sign ^ b.sign, tmp);
}

big_integer operator<<(big_integer const& a, ui b) {
    if (b == 0) {
        return big_integer(a);
    }

    size_t div = b >> 5;
    size_t mod = b & 31;
    size_t new_size = a.data.size() + div + 1;

    std::vector<ui> temp(new_size);
    temp[div] = static_cast<ui>((static_cast<ull>(a.get_digit(0)) << mod) & UINT32_MAX);
    for (size_t i = div + 1; i < new_size; ++i) {
        ull x = static_cast<ull>(a.get_digit(i - div)) << mod;
        ull y = static_cast<ull>(a.data[i - div - 1]) >> (32 - mod);
        temp[i] = static_cast<ui>((x | y) & UINT32_MAX);
    }

    return big_integer(a.sign, temp);
}

big_integer operator>>(big_integer const& a, ui b) {
    if (b == 0) {
        return big_integer(a);
    }

    size_t div = b >> 5;
    size_t mod = b & 31;
    size_t new_size = 0;

    if (div < a.data.size()) {
        new_size = a.data.size() - div;
    }
    std::vector<ui> temp(new_size);
    for (size_t i = 0; i < new_size; ++i) {
        ull x = ull(a.data[i + div]) >> mod;
        ull y = ull(a.get_digit(i + div + 1)) << (32 - mod);
        temp[i] = static_cast<ui>((x | y) & UINT32_MAX);
    }

    return big_integer(a.sign, temp);
}

std::string to_string(big_integer const& a) {
    std::string res;

    big_integer tmp = abs(a);
    while (tmp > 0) {
        big_integer cur = tmp % 10;
        res.push_back(static_cast<char>(cur.get_digit(0) + '0'));
        tmp /= 10;
    }

    if (res.empty()) {
        res.push_back('0');
    }
    if (a.sign) {
        res.push_back('-');
    }

    std::reverse(res.begin(), res.end());
    return res;
}
