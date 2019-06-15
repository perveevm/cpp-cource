#include "ui_vector.h"
#include <memory>
#include <algorithm>
#include <cassert>

size_t ui_vector::size() const {
    return len;
}

bool ui_vector::empty() const {
    return size() == 0;
}

ui_vector::ui_vector(size_t curlen) {
    if (curlen > 1) {
        elements = std::make_shared<std::vector<ui>>(curlen);
    }
    len = curlen;
}

ui ui_vector::operator[](size_t ind) const {
    if (small()) {
        return element;
    }
    return elements->operator[](ind);
}

ui &ui_vector::operator[](size_t ind) {
    if (small()) {
        return element;
    }
    unique();
    return elements->operator[](ind);
}

ui ui_vector::back() const {
    if (small()) {
        return element;
    }
    return elements->back();
}

ui &ui_vector::back() {
    if (small()) {
        return element;
    }
    unique();
    return elements->back();
}

void ui_vector::resize(size_t curlen) {
    if (small()) {
        if (curlen > 1) {
            elements = std::make_shared<std::vector<ui>>(curlen, element);
        }
    } else {
        if (curlen > 1) {
            unique();
            elements->resize(curlen);
        } else if (curlen == 1) {
            ui tmp = *elements->begin();
            elements.reset();
            element = tmp;
        } else {
            elements.reset();
        }
    }
    len = curlen;
}

void ui_vector::push_back(ui value) {
    if (len == 0) {
        element = value;
    } else if (len == 1) {
        elements = std::make_shared<std::vector<ui>>(1, element);
        elements->push_back(value);
    } else {
        unique();
        elements->push_back(value);
    }
    len++;
}

void ui_vector::reverse() {
    if (!small()) {
        std::reverse(elements->begin(), elements->end());
    }
}

void ui_vector::pop_back() {
    if (len > 2) {
        unique();
        elements->pop_back();
    } else if (len == 2) {
        ui tmp = *elements->begin();
        elements.reset();
        element = tmp;
    } else {
        element = 0;
    }
    len--;
}

bool operator==(ui_vector const &a, ui_vector const &b) {
    if (a.small() != b.small()) {
        return false;
    }
    if (a.small()) {
        return a.element == b.element;
    }
    return *a.elements == *b.elements;
}

void ui_vector::unique() {
    if (!elements.unique()) {
        elements = std::make_shared<std::vector<ui>>(*elements);
    }
}

bool ui_vector::small() const {
    return len <= 1;
}

ui_vector &ui_vector::operator=(ui_vector const &other) {
    if (!small()) {
        elements.reset();
    }
    if (other.small()) {
        element = other.element;
    } else {
        elements = other.elements;
    }
    len = other.len;
    return *this;
}

ui_vector::ui_vector() {
    len = 0;
    element = 0;
}

ui_vector::~ui_vector() {
    if (!small()) {
        elements.reset();
    }
}