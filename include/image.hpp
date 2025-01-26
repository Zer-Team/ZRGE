/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "classes.hpp"
#include "parser.hpp"

// Добавления в область видимости
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

// Загрузка файла в холст
signed char loadingImage(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, bool isOpenFile)
{
    // Объявление переменных
    std::string stringFile;
    std::ifstream fileInput{filepath, std::ios::binary};
    char Error{0};

    // Проверки
    if ((img.format == "png" || img.format == "jpg") && isOpenFile)
    {
        // Загрузка изображения
        canvas.loadFromFile(filepath);

        // Получение размеров изображения
        img.width = canvas.getSize().x;
        img.height = canvas.getSize().y;
    }
    else if (img.format == "png" || img.format == "jpg")
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::White);
    }
    else if (img.format == "zpif" && isOpenFile)
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::Transparent);

        while (getline(fileInput, stringFile))
        {
            if (stringFile.find("@s@") != NPOS)
                break;
        }

        // Парсинг и заполнения пикселя
        while (getline(fileInput, stringFile))
        {
            // Парсинг пикселя
            Error = parserPixel(stringFile, img);

            if (Error < 0)
                return -1;
            if (Error == 1)
                break; // Завершение рендера

            // Проверка на равенства
            if (img.point > img.width * img.height)
                break;

            // Устанавливаем цвет пикселя
            sf::Color color(img.rgba[0], img.rgba[1], img.rgba[2], img.rgba[3]);


            // Обработка сжатия
            if (img.quantity > 0 && containsString(img.compression, "rle"))
            {
                while (img.quantity > 0 && (img.point < img.width * img.height))
                {
                    canvas.setPixel(((img.point - 1) % img.width), ((img.point - 1) / img.width), color);
                    img.quantity--;
                    img.point++;
                }
            }
            // Заполнения пикселя
            canvas.setPixel(((img.point - 1) % img.width), ((img.point - 1) / img.width), color);
            // Проверка на на равенства
            if (img.point + img.quantity > img.width * img.height)
                cout << "\033[1;33mWARNING: Number of pixels exceeds available\033[0m" << endl;
        }

        // Обновления текстуру из изменённого изображения
        texture.update(canvas);
        fileInput.close();
        img.rgba[0] = img.rgba[1] = img.rgba[2] = img.rgba[3] = 0;
    }
    else if (img.format == "zpif")
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::White);
    }
    else
        return -1;

    img.rgba[0] = img.rgba[1] = img.rgba[2] = 0;
    img.rgba[3] = 255;

    return 0;
}

#endif