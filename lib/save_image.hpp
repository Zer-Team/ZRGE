/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _SAVE_IMAGE_HPP_
#define _SAVE_IMAGE_HPP_

// Библиотеки
#include <fstream>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <limits>
// Собственные
#include "./classes.hpp"
#include "./compress.hpp"

// Объявления типов
typedef unsigned int u_int;

// Добавления в область видимости
using std::cout;
using std::cerr;
using std::endl;

// Запись данных в изображения
char savingImage(std::string &filename, sf::Image &image, Image &img)
{
    std::ofstream file(filename, std::ios::binary); // Открытия файла на запись

    // Проверка на открытие файла
    if (!file.is_open()) {
        cerr << "\033[31mError opening/create temp file.\033[0m" << endl;
        return -1;
    }

    // Проверка на сжатие   если оно есть то эти данные не нужны (потому что запись идёт в tmp файл)
    if (img.compression == "0")
    {
        file << "{f}" << '(' << img.format << ")\n";
        file << "{c}" << '(' << img.compression << ")\n";
        file << "{m}" << '(' << img.mode << ")\n";
        file << "{w}" << '(' << img.width << ")\n";
        file << "{h}" << '(' << img.height << ")\n";
    }
    
    // Начало данных о пикселах
    file << "@s@" << '\n';
    
    // Запись пикселей в файл
    for (u_int i = 0; i < img.width*img.height; i++)
    {
        sf::Color pixelColor = image.getPixel((i % img.width), (i / img.width));
        file << '[' << pixelColor.r << pixelColor.g << pixelColor.b << "]\n";
        img.point++;
    }
    
    // Конец данных о пикселах
    file << "@e@" << '\n';
    
    // Закрытие файла
    file.close();
    return 0;
}

// Сохранение изображения
char saveImageZPIF(sf::Image &canvas, Image &img, std::string &filename, std::string &filename_temp, char &Error)
{
    // Проверка на сжатие
    if (img.compression == "rle")
    {
        // Запись данных в tmp файл
        cout << "Writing data to a temporary file..." << std::endl;
        Error = savingImage(filename_temp, canvas, img);
                    
        if (Error < 0)
            return -1;

        // Запись с сжатием из tmp файла в основной 
        cout << "Create, record and compress image..." << endl;
        Error = compress_rle(filename_temp, filename, img);
                    
        if (Error < 0)
            return -1;

        // Удаление tmp файла
        cout << "Remove temp file..." << std::endl;
        remove(filename_temp.c_str());
    }
    else
    {
        // Запись данных в основной файл
        cout << "Writing data to file..." << std::endl;
        Error = savingImage(filename, canvas, img);
        if (Error < 0)
            return -1;
    }
    
    cout << "\033[32mImage saved successfully.\033[0m" << endl;

    return 0;
}
#endif