#include "../include/draw.hpp"

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

// Функция для создания кнопок
void drawButton(sf::RenderWindow &window, const sf::Texture &texture, sf::Sprite &button, const sf::Vector2f &position)
{
    // Кнопка
    button.setTexture(texture);
    button.setPosition(position);
    // Отрисовка
    window.draw(button);
}

// Функция для заливки цветом
void fillColor(const int &x, const int &y, const Image &img, sf::Image &canvas, sf::Texture &texture, const sf::Color newColor)
{
    sf::Color targetColor = canvas.getPixel(sf::Vector2u(x, y));
    if (targetColor == newColor)
        return; // Если цвет совпадает, ничего не делаем

    std::stack<sf::Vector2i> stack;
    stack.push(sf::Vector2i(x, y));

    while (!stack.empty())
    {
        sf::Vector2i pos = stack.top();
        stack.pop();

        // Проверка границ и совпадение цвета
        if (pos.x < 0 || pos.x >= img.width || pos.y < 0 || pos.y >= img.height ||
            canvas.getPixel(static_cast<sf::Vector2u>(pos)) != targetColor)
        {
            continue;
        }

        // Новый цвет
        canvas.setPixel(static_cast<sf::Vector2u>(pos), newColor);

        // Добавление соседнего пиксели в стек
        if (pos.x + 1 < img.width)
            stack.push({pos.x + 1, pos.y});
        if (pos.x - 1 >= 0)
            stack.push({pos.x - 1, pos.y});
        if (pos.y + 1 < img.height)
            stack.push({pos.x, pos.y + 1});
        if (pos.y - 1 >= 0)
            stack.push({pos.x, pos.y - 1});
    }

    texture.update(canvas);
}