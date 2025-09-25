/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#include "../include/draw.hpp"

// Функция для создания слайдеров на экране
void drawSlider(sf::RenderWindow &window, sf::RectangleShape &slider, const int value, const sf::Color color, const sf::Vector2f &position)
{
    // Слайдер
    slider.setSize(sf::Vector2f(WIDTH_COLOR_SLIDER, HEIGHT_COLOR_SLIDER)); // Ширина Высота
    slider.setFillColor(color);
    slider.setPosition(position);

    // Ползунок
    sf::RectangleShape indicator(sf::Vector2f(WIDTH_COLOR_INDICATOR, HEIGHT_COLOR_INDICATOR)); // Ширина Высота
    indicator.setFillColor(sf::Color::Black);
    indicator.setPosition(sf::Vector2f(position.x + value, position.y - WIDTH_COLOR_INDICATOR)); // Минус ширина

    // Отрисовка
    window.draw(slider);
    window.draw(indicator);
}

// Функция для создания кнопок на экране
void drawButton(sf::RenderWindow &window, sf::Sprite &button, const sf::IntRect &rect, const sf::Vector2f &position)
{
    // Кнопка
    button.setTextureRect(rect);
    button.setPosition(position);
    // Отрисовка
    window.draw(button);
}
