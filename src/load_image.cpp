/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#include "../include/load_image.hpp"
#include "../include/locale.hpp"
#include "../include/parser.hpp"
#include <fstream>
#include <cstdint>
#include <array>

signed char loadingImage(zp::Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, bool isOpenFile)
{
    // Проверки форматов
    if ((img.format == zrg::ImageFormat::PNG || img.format == zrg::ImageFormat::JPEG) && isOpenFile)
    {
        // Загрузка изображения
        if (!canvas.loadFromFile(filepath))
        {
            std::cerr << "\033[1;31m" << locale->error_no_supp_file << "\033[0m" << std::endl;
            return -3;
        }

        // Получение размеров изображения
        img.width = canvas.getSize().x;
        img.height = canvas.getSize().y;
    }
    else if (img.format == zrg::ImageFormat::ZPIF && isOpenFile)
    {
        // Объявление переменных
        std::ifstream inputFile{filepath, std::ios::binary}; // Файл изображения
        std::array<uint8_t, 6> buffer;                       // Буфер для чанков

        // Создания холста
        canvas.resize(sf::Vector2u(img.width, img.height), sf::Color::Transparent);

        inputFile.seekg(img.renderStart);

        // Парсинг пикселей из файла (работа с чанками пикселей)
        while (inputFile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()))
        {
            if (buffer == std::array<uint8_t, 6>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
                break; // Выход при достижении чанка структурирования 00

            uint16_t quantity{convertBEInShort(buffer)};

            sf::Color color(buffer[2], buffer[3], buffer[4], buffer[5]);

            for (uint16_t i = 0; i < quantity; ++i)
            {
                if (img.point >= img.height * img.width)
                {
                    cout << "\033[1;31m" << locale->warning_big_img << "\033[0m" << endl;
                    break;
                }
                
                canvas.setPixel(sf::Vector2u((img.point % img.width), (img.point / img.width)), color);
                ++img.point;
            }
        }

        inputFile.close();
    }
    else if (!isOpenFile)
        // Создания холста
        canvas.resize(sf::Vector2u(img.width, img.height), sf::Color::White);
    else
        return -1;

    // Изменение размеров текстуры
    if (!texture.resize(canvas.getSize()))
    {
        std::cerr << "\033[1;31m" << locale->error_resize_texture << ' ' << img.width << "x" << img.height << "\033[0m" << std::endl;
        return 1;
    }

    // Обнавления холста
    texture.update(canvas);

    return 0;
}