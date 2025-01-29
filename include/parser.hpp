/*
    MIT License

    Copyright (c) 2025 Zakhar Shakhanov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

// Библиотеки
#include <iostream>
#include <fstream>
#include <string>
//   Собственные
#include "classes.hpp"

// npos
#define NPOS std::string::npos

// Объявления типов
typedef unsigned short u_short;
typedef unsigned char  u_int8_t;

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Функция для чтения unsigned short из файла в формате big-endian
inline unsigned short convertBEInNumber(const u_int8_t &byteBig, const u_int8_t &byteLittle)
{
    // Объединение байтов в число (big-endian)
    return (byteBig << 8) | byteLittle;
}

// Функция для получения текста из заданных символов
std::string extractContent(const std::string &str, char startChar, char endChar) {
    size_t start = str.find(startChar);
    if (start == NPOS) return "";
    size_t end = str.find(endChar, start + 1);
    return (end != NPOS) ? str.substr(start + 1, end - start - 1) : "";
}

// Функция парсинга параметров
signed char parserParams(Image &img, const std::string &filepath)
{
    std::string str; // Строка для записи данных из файла
    std::ifstream file{filepath, std::ios::binary}; // Файл для чтения

    // Чтение первой строки
    getline(file, str);

    // Проверка заголовка
    if (str != "\xDDZPIF")
    {
        cerr << "\033[1;31mError 1: The file is damaged or the format is not supported.\033[0m" << std::endl;
        return -1;
    }

    // Парсинг параметров из файла
    while (getline(file, str))
    {
        // Пропуск пустых строк.
        if (str[0] == '\n' || str.empty())
            continue;

        // Парсинг из строки
        if (str.find('{') != NPOS && str.find('}') != NPOS && str.find('(') != NPOS && str.find(')') != NPOS)
        {
            // Создания объекта и присвоения значений
            Parameter param{extractContent(str, '{', '}'), extractContent(str, '(', ')')};
            // Получения данных
            //   Получения размеров
            if (param.getName()[0] == 'w')
                img.width = std::strtoul((param.getValue().data()), nullptr, 10);
            else if (param.getName()[0] == 'h')
                img.height = std::strtoul((param.getValue().data()), nullptr, 10);
        }

        else if (str.c_str() == std::string("\x00\x00\xFF\xFF\xFF\xFF"))
        {
            if (img.width <= 0 || img.height <= 0)
            {
                cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << std::endl;
                return -2;
            }

            img.renderStart = file.tellg();
            return 1;
        }

        else
        {
            cerr << "\033[1;31mError 1: The file is damaged or the format is not supported.\033[0m" << std::endl;
            return -1;
        }
    
    }
            
    return 0;
}

// Функция парсинга пикселя
signed char parserPixel(const std::vector<u_int8_t> &buffer, Image &img) {
    if (buffer.size() != 6)
    {
        cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << endl;
        return -2;
    }
    if (buffer == std::vector<u_int8_t>(6, 0x00)) return 1;

    // Количество пикселей подряд
    img.quantity = convertBEInNumber(buffer[0], buffer[1]);
    // Запись цвета пикселя
    std::copy(buffer.begin() + 2, buffer.begin() + 6, img.rgba);

    return 0;
}

#endif
