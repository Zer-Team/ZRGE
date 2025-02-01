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
#include <stdint.h>
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
inline u_short convertBEInShort(const std::vector<u_int8_t> &bytes)
{
    return (bytes[0] << 8) | bytes[1];
}

// Функция для чтения unsigned int из файла в формате big-endian
inline uint64_t convertBEInInt(const std::vector<u_int8_t> &bytes)
{
    return (static_cast<u_int>(bytes[1]) << 24) | // Не с 0 потому что 0 хранит имя параметра 
           (static_cast<u_int>(bytes[2]) << 16) |
           (static_cast<u_int>(bytes[3]) << 8)  |
           (static_cast<u_int>(bytes[4]));
}

// Функция парсинга параметров
signed char parserParams(Image &img, const std::string &filepath)
{
    std::vector<u_int8_t> buffer(6);                // Буфер для хранения данных
    std::ifstream file{filepath, std::ios::binary}; // Файл для чтения

    // Чтение первой строки
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

    // Проверка заголовка
    if (buffer != std::vector<u_int8_t>{0x89, 'Z', 'P', 'I', 'F', 0x0A})
    {
        cerr << "\033[1;31mError 1: The file is damaged or the format is not supported.\033[0m" << std::endl;
        return -1;
    }

    // Парсинг параметров из файла
    while (file.read(reinterpret_cast<char *>(buffer.data()), buffer.size()))
    {
        // Парсинг размеров изображения
        if (buffer[0] == 0x77)      // w
            img.width = convertBEInInt(buffer);

        else if (buffer[0] == 0x68) // h
            img.height = convertBEInInt(buffer);

        // Прекращение чтения при достижении начала данных о пикселях
        else if (buffer == std::vector<u_int8_t>{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF})
        {
            if (img.width <= 0 || img.height <= 0)
            {
                cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << std::endl;
                return -2;
            }

            img.renderStart = file.tellg();
            return 1;
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
    img.quantity = convertBEInShort(buffer);
    
    // Запись цвета пикселя
    std::copy(buffer.begin() + 2, buffer.begin() + 6, img.rgba);

    return 0;
}

#endif