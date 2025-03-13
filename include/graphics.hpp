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

int render(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, std::string &filepath_temp);

#endif // _RENDER_HPP_
