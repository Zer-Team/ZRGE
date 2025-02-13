#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <limits>
#include <iostream>

template <typename T>

// Функция для безопасного ввода чисел
void getNumberOrChar(T &obj)
{
    // Запрос
    std::cin >> obj;
    // Очистка
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif // _UTILS_HPP_