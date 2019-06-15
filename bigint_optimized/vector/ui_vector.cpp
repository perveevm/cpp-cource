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
        _data.elements = std::make_shared<std::vector<ui>>(curlen);
    }
    len = curlen;
}

ui ui_vector::operator[](size_t ind) const {
    if (small()) {
        return _data.element;
    }
    return _data.elements->operator[](ind);
}

ui &ui_vector::operator[](size_t ind) {
    if (small()) {
        return _data.element;
    }
    unique();
    return _data.elements->operator[](ind);
}

ui ui_vector::back() const {
    if (small()) {
        return _data.element;
    }
    return _data.elements->back();
}

ui &ui_vector::back() {
    if (small()) {
        return _data.element;
    }
    unique();
    return _data.elements->back();
}

void ui_vector::resize(size_t curlen) {
    if (small()) {
        if (curlen > 1) {
            _data.elements = std::make_shared<std::vector<ui>>(curlen, _data.element);
        }
    } else {
        if (curlen > 1) {
            unique();
            _data.elements->resize(curlen);
        } else if (curlen == 1) {
            ui tmp = *_data.elements->begin();
            _data.elements.reset();
            _data.element = tmp;
        } else {
            _data.elements.reset();
        }
    }
    len = curlen;
}

void ui_vector::push_back(ui value) {
    if (len == 0) {
        _data.element = value;
    } else if (len == 1) {
        _data.elements = std::make_shared<std::vector<ui>>(1, _data.element);
        _data.elements->push_back(value);
    } else {
        unique();
        _data.elements->push_back(value);
    }
    len++;
}

void ui_vector::reverse() {
    if (!small()) {
        std::reverse(_data.elements->begin(), _data.elements->end());
    }
}

void ui_vector::pop_back() {
    if (len > 2) {
        unique();
        _data.elements->pop_back();
    } else if (len == 2) {
        ui tmp = *_data.elements->begin();
        _data.elements.reset();
        _data.element = tmp;
    } else {
        _data.element = 0;
    }
    len--;
}

bool operator==(ui_vector const &a, ui_vector const &b) {
    if (a.small() != b.small()) {
        return false;
    }
    if (a.small()) {
        return a._data.element == b._data.element;
    }
    return *a._data.elements == *b._data.elements;
}

void ui_vector::unique() {
    if (!_data.elements.unique()) {
        _data.elements = std::make_shared<std::vector<ui>>(*_data.elements);
    }
}

bool ui_vector::small() const {
    return len <= 1;
}

ui_vector &ui_vector::operator=(ui_vector const &other) {
    if (!small()) {
        _data.elements.reset();
    }
    if (other.small()) {
        _data.element = other._data.element;
    } else {
        _data.elements = other._data.elements;
    }
    len = other.len;
    return *this;
}

ui_vector::ui_vector() {
    len = 0;
    _data.element = 0;
}

ui_vector::~ui_vector() {
    if (!small()) {
        _data.elements.reset();
    }
}