#ifndef MY_UI_VECTOR
#define MY_UI_VECTOR

#include <memory>
#include <vector>

typedef uint32_t ui;

class ui_vector {
public:
    explicit ui_vector(size_t curlen);
    ui_vector();
    ~ui_vector();

    size_t size() const;
    bool empty() const;
    ui operator[](size_t ind) const;
    ui &operator[](size_t ind);

    ui back() const;
    ui &back();

    void resize(size_t curlen);
    void push_back(ui value);
    void reverse();
    void pop_back();

    friend bool operator==(ui_vector const &a, ui_vector const &b);
    ui_vector& operator=(ui_vector const &other);

private:
    size_t len;

    ui element;
    std::shared_ptr<std::vector<ui>> elements;

    bool small() const;

    void unique();

};

bool operator==(ui_vector const &a, ui_vector const &b);

#endif