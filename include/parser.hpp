/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#pragma once

// Библиотеки
#include <iostream>
#include <cstdint>
#include <array>
#include "image.hpp"

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Функция для чтения unsigned short из файла в формате big-endian
uint16_t convertBEInShort(const std::array<uint8_t, 6> &bytes);

// Функция для чтения unsigned int из файла в формате big-endian
uint64_t convertBEInInt(const std::array<uint8_t, 6> &bytes);

// Функция парсинга параметров
signed char parserParams(zp::Image &img, const std::string &filepath);
