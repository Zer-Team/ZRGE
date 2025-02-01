/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

// Библиотеки
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
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
    // Проверки форматов
    if ((img.format == "png" || img.format == "jpg") && isOpenFile)
    {
        // Загрузка изображения
        if (!canvas.loadFromFile(filepath))
        {
            cout << "\033[1;31mError 3: The file format is not supported or the file is damaged. CHECK THE FILE.\033[0m" << endl;
            return -3;
        }

        // Получение размеров изображения
        img.width = canvas.getSize().x;
        img.height = canvas.getSize().y;
    }
    else if (img.format == "zpif" && isOpenFile)
    {
        // Объявление переменных
        std::ifstream inputFile{filepath, std::ios::binary};
        std::vector<u_int8_t> buffer(6);
        char Error{0};

        // Создания холста
        canvas.create(img.width, img.height, sf::Color::Transparent);

        inputFile.seekg(img.renderStart);

        // Парсинг и заполнения пикселя
        while (inputFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()))
        {
            // Парсинг пикселя
            Error = parserPixel(buffer, img);

            // Завершение рендера
            //   Ошибки
            if (Error < 0)
                return -1;
            //   Конец файла
            if (Error == 1)
                break; 

            // Проверка не выходят ли пиксели за размер изображения
            if (img.point + img.quantity > img.width * img.height)
                cout << "\033[1;33mWARNING: Number of pixels exceeds available\033[0m" << endl;

            // Устанавливаем цвет пикселя
            sf::Color color(img.rgba[0], img.rgba[1], img.rgba[2], img.rgba[3]);

            // Обработка сжатия
            if (img.quantity > 0)
            {
                while (img.quantity > 0 && (img.point <= img.width * img.height))
                {
                    canvas.setPixel((img.point % img.width), (img.point / img.width), color);
                    img.quantity--;
                    img.point++;
                }
            }
        }

        inputFile.close();
    }
    else if (img.format == "zpif" || img.format == "png" || img.format == "jpg")
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::White);
    }
    else
        return -1;

    texture.update(canvas);
    img.rgba[0] = img.rgba[1] = img.rgba[2] = img.rgba[3] = 0;
    img.rgba[3] = 255;

    return 0;
}

#endif