/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <limits>
#include <cmath>
#include <algorithm>
#include "image.hpp"
#include "locale.hpp"
#include "draw.hpp"
#include "save_image.hpp"
#include "utils.hpp"

// Размеры
#define INDENT_X 10 // Разделитель
//  Просмотр цвета
#define HEIGHT_COLOR_PREVIEW 50
// Слайдер
#define WIDTH_COLOR_SLIDER 255
#define HEIGHT_COLOR_SLIDER 15
//  Индикатор
#define WIDTH_COLOR_INDICATOR 5
#define HEIGHT_COLOR_INDICATOR 28
//  Индикатор и слайдер в ./include/draw.hpp
#define Y_SLIDER HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 3 + 10 // Y координата для слайдеров

using std::cout;
using std::endl;

enum class Mode {
    BRUSH,     // Кисть
    DRAW_RECT, // Прямоугольник
    DRAW_OVAL, // Овол
    DRAW_STAR, // Звезда
    POURING    // Заливка
};

/**
 @brief Главная функция всей графики
 @param img Данные об изображении 
 @param canvas Холст
 @param texture Текстура холста
 @param filepath Путь к изображению
 @param filepath_temp Путь к временному файлу 
 */
int render(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &iconpath, std::string &filepath, std::string &filepath_temp);
