#include <iostream>
#include "twelve.h"

int main() {
    try {
        Twelve z0;                  // "0"
        Twelve a("1AB0B");          // из строки
        Twelve b("9");              // из строки

        std::cout << "z0 = " << z0.toString() << "\n";
        std::cout << "a  = " << a.toString()  << "\n";
        std::cout << "b  = " << b.toString()  << "\n";

        // Сравнения
        std::cout << "a == a ? " << (a.isEqual(a) ? "true" : "false") << "\n";
        std::cout << "a <  b ? " << (a.isLess(b)   ? "true" : "false") << "\n";
        std::cout << "compare(a, a) = " << a.compare(a) << "  (-1,0,1)\n\n";

        // Арифметика (иммутабельно: возвращают новый объект)
        Twelve sum = a.add(b);      // "1AB0B" + "9" = "1AB14"
        Twelve dif = sum.sub(b);    // обратно вычли 9 => "1AB0B"

        std::cout << a.toString() << " + " << b.toString()
                  << " = " << sum.toString() << "\n";
        std::cout << sum.toString() << " - " << b.toString()
                  << " = " << dif.toString() << "\n\n";

        // trySub — безопасная версия: false, если вычитание нельзя
        Twelve big("144");
        Twelve small("1");
        Twelve res("0");

        bool ok = big.trySub(small, res);
        std::cout << "big.trySub(small) -> " << (ok ? "true" : "false")
                  << ", res = " << res.toString() << "\n";

        ok = small.trySub(big, res);     // тут должно быть false, res не меняется
        std::cout << "small.trySub(big) -> " << (ok ? "true" : "false")
                  << ", res = " << res.toString() << " (не изменился)\n";

        // Попытка создать из плохой строки — поймаем исключение
        // Twelve bad("1AC"); // раскомментируй — упадёт с invalid_argument
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
