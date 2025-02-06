/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _CLASSES_HPP_
#define _CLASSES_HPP_

// Библиотеки
#include <string>
#include <cstdint>

// Класс изображения
class Image
{
public:
    std::string format{""};        // Формат изображения

    uint32_t width{0};             // Ширина изображения
    uint32_t height{0};            // Высота изображения

    uint8_t rgba[4]{0};            // Цвет пикселя

    uint16_t quantity{0};          // Количество подряд (для сжатия)
    uint64_t point{0};             // Позиция пикселя заполнения с лево на право сверху вниз

    std::streampos renderStart{0}; // Позиция начала данных о пикселе
};

#endif 