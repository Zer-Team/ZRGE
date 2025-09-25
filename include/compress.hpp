/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#pragma once

#include <cstdint>
#include <ctime>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include "image.hpp"
#include "locale.hpp"

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Преобразование числа u_short в big-endian
void convertNumberInBE(const uint16_t value, std::array<uint8_t, 6> &bytes, const uint8_t &shift = 0);

// Преобразование числа u_int в big-endian
void convertNumberInBE(const uint32_t value, std::array<uint8_t, 6> &bytes);

// Сжатие RLE
signed char compress_rle(const std::string &input, const std::string &output, const Image &img);