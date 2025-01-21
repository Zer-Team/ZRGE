/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#ifndef _DRAW_HPP_
#define _DRAW_HPP_

#include <SFML/Graphics.hpp>

// Слайдер
#define WIDTH_COLOR_SLIDER      255
#define HEIGHT_COLOR_SLIDER     15
//  Индикатор
#define WIDTH_COLOR_INDICATOR   5
#define HEIGHT_COLOR_INDICATOR  28

// Функция для создания ползунков
void drawSlider(sf::RenderWindow &window, sf::RectangleShape &slider, int value, sf::Color color, const sf::Vector2f &position) {
    // Слайдер
    slider.setSize(sf::Vector2f(WIDTH_COLOR_SLIDER, HEIGHT_COLOR_SLIDER));// Ширина Высота
    slider.setFillColor(color);
    slider.setPosition(position);
    
    // Ползунок
    sf::RectangleShape indicator(sf::Vector2f(WIDTH_COLOR_INDICATOR, HEIGHT_COLOR_INDICATOR));// Ширина Высота
    indicator.setFillColor(sf::Color::Black);
    indicator.setPosition(position.x + value, position.y - WIDTH_COLOR_INDICATOR);// Минус ширина

    // Отрисовка
    window.draw(slider);
    window.draw(indicator);
}

// Функция для создания кнопок
void drawButton(sf::RenderWindow &window, const sf::Texture &texture, sf::Sprite &button, const sf::Vector2f &position) {
    // Кнопка
    button.setTexture(texture);
    button.setPosition(position.x, position.y);
    // Отрисовка
    window.draw(button);
}

#endif