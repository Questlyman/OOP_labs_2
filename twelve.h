#pragma once
#include <cstddef>
#include <initializer_list>
#include <string>
#include <stdexcept>

/// Беззнаковое число в системе счисления по основанию 12.
/// Цифры: 0..9, A(10), B(11). Младшая цифра имеет меньший индекс.
/// Внутри — динамический массив unsigned char, без std::vector.
class Twelve {
public:
    Twelve();                                              // 0
    Twelve(const std::string& s);                          // из строки, например "1AB09"
    Twelve(Twelve&& other) noexcept;                       // перемещающий                         
    Twelve(const Twelve& other);
    // --- Базовые сведения ---
    size_t length() const noexcept;                        // кол-во цифр (>=1)
    std::string toString() const;                          // в верхнем регистре, без ведущих нулей (кроме "0")

    // --- Сравнения (без перегрузок) ---
    bool isEqual(const Twelve& rhs) const noexcept;
    int  compare(const Twelve& rhs) const noexcept;        // -1,0,1
    bool isLess(const Twelve& rhs) const noexcept  { return compare(rhs) < 0; }
    bool isGreater(const Twelve& rhs) const noexcept { return compare(rhs) > 0; }

    // --- Арифметика (иммутабельно) ---
    Twelve add(const Twelve& rhs) const;                   // this + rhs
    Twelve sub(const Twelve& rhs) const;                   // this - rhs; если rhs>this -> domain_error

    

    // Совместимость с твоими тестами (сигнатура из твоего билда):
    // Пытается вычесть; если rhs>this, возвращает false и out не меняет.
    bool trySub(const Twelve& rhs, Twelve& out) const;

    // --- Присваивания коп/перем (обычные) ---
    Twelve& operator=(const Twelve& other);
    Twelve& operator=(Twelve&& other) noexcept;

private:
    static constexpr unsigned BASE = 12;
    unsigned char* digits_ = nullptr; // младший разряд в digits_[0]
    size_t size_ = 1;

    // Внутренние утилиты
    static unsigned char charToDigit(char c); // '0'..'9','A','B','a','b'
    static char digitToChar(unsigned char d); // 0..11
    static void ensureDigit(unsigned char d); // throw если >=12
    static void trimZeros(unsigned char*& a, size_t& n); // убрать ведущие нули (старшие)

    // Вспомогательные конструкторы
    Twelve(unsigned char* raw, size_t n); 
};
