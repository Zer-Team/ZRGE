#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <limits>
#include <cmath>
#include <algorithm>
#include "image.hpp"
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

// Режим рисовани
enum class ModeDraw
{
        BRUSH,              // Рисование кистью
        ERASING,            // Стёрка
        DRAWING_RECT,       // Рисования квадрата
        DRAWING_OVAL,       // Рисования овала
        DRAWING_STAR,       // Рисования звезды
        POURING             // Заливка цветом
};

/**
 @brief Главная функция всей графики
 @param img Данные об изображении 
 @param canvas Холст
 @param texture Текстура холста
 @param filepath Путь к изображению
 @param filepath_temp Путь к временному файлу 
 */
int render(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, std::string &filepath_temp);

#endif // _RENDER_HPP_
