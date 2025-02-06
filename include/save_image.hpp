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
#include <cstdint>

// Собственные
#include "classes.hpp"
#include "compress.hpp"

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Запись данных в изображения
signed char recordPixelData(std::string &filename, sf::Image &image, Image &img)
{
    std::ofstream file(filename, std::ios::binary);       // Файл на запись
    const size_t bufferSize = img.width * img.height * 6; // Размер буфера
    std::vector<uint8_t> buffer(bufferSize);             // Буфер для хранения данных
    size_t index = 0;                                     // Индекс

    // Проверка на открытие файла
    if (!file.is_open())
    {
        cerr << "\033[31mError opening/create temp file.\033[0m" << endl;
        return -1;
    }

    for (uint32_t y = 0; y < img.height; y++)
    {
        for (uint32_t x = 0; x < img.width; x++)
        {
            // Получения цвета
            sf::Color pixelColor = image.getPixel(x, y);

            // Количества пикселей подряд (сжатия)
            buffer[index++] = 0x00;
            buffer[index++] = 0x01;
            // Цвета
            buffer[index++] = pixelColor.r; // Красный компонент
            buffer[index++] = pixelColor.g; // Зелёный компонент
            buffer[index++] = pixelColor.b; // Синий компонент
            buffer[index++] = pixelColor.a; // Альфа-канал
        }
    }

    // Запись всего буфера в файл за один вызов
    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());

    // Конец данных о пикселах
    file.write("\x00\x00\x00\x00\x00\x00", 6);

    // Закрытие файла
    file.close();
    return 0;
}

// Сохранение изображения
signed char saveImageZPIF(sf::Image &canvas, Image &img, std::string &filename, std::string &filename_temp)
{
    // Запись данных в tmp файл
    cout << "Writing data to a temporary file..." << std::endl;

    if (recordPixelData(filename_temp, canvas, img) < 0)
        return -1;

    // Запись с сжатием из tmp файла в основной
    cout << "Create, record and compress image..." << endl;

    if (compress_rle(filename_temp, filename, img) < 0)
        return -1;

    // Удаление tmp файла
    cout << "Remove temp file..." << std::endl;
    remove(filename_temp.c_str());

    cout << "\033[32mImage saved successfully.\033[0m" << endl;

    return 0;
}

#endif