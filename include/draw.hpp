/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#ifndef _DRAW_HPP_
#define _DRAW_HPP_

#include <SFML/Graphics.hpp>
#include <stack>
#include "image.hpp"

// Слайдер
#define WIDTH_COLOR_SLIDER 255
#define HEIGHT_COLOR_SLIDER 15
//  Индикатор
#define WIDTH_COLOR_INDICATOR 5
#define HEIGHT_COLOR_INDICATOR 28

// Функция для создания ползунков
void drawSlider(sf::RenderWindow &window, sf::RectangleShape &slider, const int value, const sf::Color color, const sf::Vector2f &position);

// Функция для создания кнопок
void drawButton(sf::RenderWindow &window, const sf::Texture &texture, sf::Sprite &button, const sf::Vector2f &position);
// Функция для заливки цветом
void fillColor(const int &x, const int &y, const Image &img, sf::Image &canvas, sf::Texture &texture, const sf::Color newColor);

#endif