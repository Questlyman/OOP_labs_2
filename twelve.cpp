#include "twelve.h"
#include <algorithm>
#include <cstring>

// --- статические утилиты ---
unsigned char Twelve::charToDigit(char c) {
    if (c >= '0' && c <= '9') return static_cast<unsigned char>(c - '0');
    if (c == 'A' || c == 'a') return 10;
    if (c == 'B' || c == 'b') return 11;
    throw std::invalid_argument("Bad base-12 character");
}
char Twelve::digitToChar(unsigned char d) {
    ensureDigit(d);
    return (d < 10) ? static_cast<char>('0' + d)
                    : static_cast<char>('A' + (d - 10));
}
void Twelve::ensureDigit(unsigned char d) {
    if (d >= BASE) throw std::out_of_range("Digit must be < 12");
}
void Twelve::trimZeros(unsigned char*& a, size_t& n) {
    while (n > 1 && a[n-1] == 0) --n;
    if (n == 0) { n = 1; a[0] = 0; }
}

// --- приватный ctor из сырого буфера ---
Twelve::Twelve(unsigned char* raw, size_t n) : digits_(raw), size_(n) {
    if (!digits_ || size_ == 0) {
        delete[] digits_;
        digits_ = new unsigned char[1]{0};
        size_ = 1;
    }
    trimZeros(digits_, size_);
}

// --- публичные ctors/dtor ---
Twelve::Twelve() {
    digits_ = new unsigned char[1]{0};
    size_ = 1;
}

Twelve::Twelve(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty string");
    // пропустим пробелы по краям
    size_t l = 0, r = s.size();
    while (l < r && s[l] == ' ') ++l;
    while (r > l && s[r-1] == ' ') --r;
    if (l >= r) throw std::invalid_argument("String has no digits");

    // строим в обратном порядке
    size_ = r - l;
    digits_ = new unsigned char[size_];
    size_t j = 0;
    for (size_t i = r; i-- > l; ) {
        unsigned char d = charToDigit(s[i]);
        digits_[j++] = d;
    }
    trimZeros(digits_, size_);
}

// --- присваивания ---
Twelve& Twelve::operator=(const Twelve& other) {
    if (this == &other) return *this;
    unsigned char* nd = new unsigned char[other.size_];
    std::memcpy(nd, other.digits_, other.size_);
    delete[] digits_;
    digits_ = nd;
    size_ = other.size_;
    return *this;
}
Twelve& Twelve::operator=(Twelve&& other) noexcept {
    if (this == &other) return *this;
    delete[] digits_;
    digits_ = other.digits_;
    size_ = other.size_;
    other.digits_ = new unsigned char[1]{0};
    other.size_ = 1;
    return *this;
}

// --- базовые сведения ---
size_t Twelve::length() const noexcept { return size_; }

std::string Twelve::toString() const {
    std::string s;
    s.reserve(size_);
    for (size_t i = size_; i-- > 0; )
        s.push_back(digitToChar(digits_[i]));
    return s;
}

// --- сравнение ---
bool Twelve::isEqual(const Twelve& rhs) const noexcept {
    if (size_ != rhs.size_) return false;
    for (size_t i = 0; i < size_; ++i)
        if (digits_[i] != rhs.digits_[i]) return false;
    return true;
}
int Twelve::compare(const Twelve& rhs) const noexcept {
    if (size_ != rhs.size_) return (size_ < rhs.size_) ? -1 : 1;
    // сравниваем со старших разрядов
    for (size_t i = size_; i-- > 0; ) {
        if (digits_[i] != rhs.digits_[i])
            return (digits_[i] < rhs.digits_[i]) ? -1 : 1;
    }
    return 0;
}

// --- арифметика ---
Twelve Twelve::add(const Twelve& rhs) const {
    const size_t n = std::max(size_, rhs.size_);
    unsigned char* out = new unsigned char[n + 1]; // + возможный перенос
    unsigned carry = 0;
    size_t i = 0;
    for (; i < n; ++i) {
        unsigned a = (i < size_) ? digits_[i] : 0;
        unsigned b = (i < rhs.size_) ? rhs.digits_[i] : 0;
        unsigned sum = a + b + carry;
        out[i] = static_cast<unsigned char>(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) { out[i++] = static_cast<unsigned char>(carry); }
    return Twelve(out, i);
}

Twelve Twelve::sub(const Twelve& rhs) const {
    if (this->compare(rhs) < 0)
        throw std::domain_error("Negative result in Twelve::sub");
    const size_t n = size_;
    unsigned char* out = new unsigned char[n];
    int borrow = 0;
    size_t i = 0;
    for (; i < n; ++i) {
        int a = digits_[i] - borrow;
        int b = (i < rhs.size_) ? rhs.digits_[i] : 0;
        if (a < b) { a += BASE; borrow = 1; } else borrow = 0;
        out[i] = static_cast<unsigned char>(a - b);
    }
    size_t m = n;
    Twelve::trimZeros(out, m);
    return Twelve(out, m);
}

bool Twelve::trySub(const Twelve& rhs, Twelve& out) const {
    if (this->compare(rhs) < 0) return false;
    out = this->sub(rhs);
    return true;
}
