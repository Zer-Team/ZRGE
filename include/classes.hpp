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

#ifndef _CLASSES_HPP_
#define _CLASSES_HPP_

// Библиотеки
#include <string>

// Объявления типов
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned char  u_int8_t;

// Класс изображения
class Image
{
public:
    std::string format{""};            // Формат изображения

    u_short width{0};                  // Ширина изображения
    u_short height{0};                 // Высота изображения

    u_int8_t rgba[4]{0};               // Цвет пикселя

    u_int quantity{0};                 // Количество подряд (для сжатия)
    size_t point{0};                   // Позиция пикселя заполнения с лево на право сверху вниз

    std::streampos renderStart{0};     // Позиция начала данных о пикселе
};

#endif 