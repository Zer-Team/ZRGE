/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

// Библиотеки
#include <string>
#include <cstdint>

namespace zrg {
    // Форматы изображения
    enum class ImageFormat
    {
        PNG,
        JPEG,
        ZPIF,
        NONE
    };
}

namespace zp {
    // Класс изображения
    class Image
    {
    public:
        zrg::ImageFormat format{zrg::ImageFormat::NONE}; // Формат изображения

        uint32_t width{0};                     // Ширина изображения
        uint32_t height{0};                    // Высота изображения
        float factor{0};                       // Фактор для увлечения (НЕ МОЖЕТ БЫТЬ <0)

        uint16_t quantity{0};                  // Количество одинаковых пикселей подряд (для сжатия)
        uint64_t point{0};                     // Позиция пикселя (с лево на право сверху вниз)

        std::streampos renderStart{0};         // Позиция начала данных о пикселе
    };
}

#endif // _IMAGE_HPP_