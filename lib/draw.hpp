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