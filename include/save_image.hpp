/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _SAVE_IMAGE_HPP_
#define _SAVE_IMAGE_HPP_

// Библиотеки
#include <fstream>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdint>

// Собственные
#include "image.hpp"
#include "compress.hpp"

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Запись данных в изображения
signed char recordPixelData(std::string &filename, sf::Image &image, Image &img);

// Сохранение изображения
signed char saveImageZPIF(sf::Image &canvas, Image &img, std::string &filename, std::string &filename_temp);

#endif