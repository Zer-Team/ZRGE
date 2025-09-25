/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#pragma once

// Библиотеки
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdint>
#include <array>
// Собственные
#include "image.hpp"
#include "locale.hpp"
#include "parser.hpp"

// Загрузка файла в холст
signed char loadingImage(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, bool isOpenFile);
