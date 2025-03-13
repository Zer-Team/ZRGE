/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

////////////////////////////////////////////////////////////////
///                       GCC   14.2.1                       ///
///                        SFML 3.0.0                        ///
///                         C++   20                         ///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
///                        ID: HM0101                        ///
///                      Version: 1.0.7                      ///
///                     Date: 2025-03-13                     ///
///                     Author: Zer Team                     ///
////////////////////////////////////////////////////////////////

// Include
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <limits>
#include "../include/image.hpp"
#include "../include/utils.hpp"
#include "../include/parser.hpp"
#include "../include/graphics.hpp"
#include "../include/load_image.hpp"

// Макросы
#define VERSION              "1.0.7"                  // Версия

int main(int argc, char **argv)
{
    Image img;                                        // Изображения
    std::string file_path_temp{".tempZRGEfile.tmp"};  // Путь к временному файлу
    std::string file_path;                            // Путь к файлу
    sf::Image canvas;                                 // Холст
    sf::Texture texture;                              // Текстура холста

    cout << "\033[1;33mWelcome to ZeR Graphics Editor " << VERSION << "!\033[0m" << endl;

    // Получения пути к файлу
    if (argc > 1)
    {
        for (uint16_t i = 1; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help")
            {
                cout << "\033[1;33mUsage: zrge [file path]\033[0m\n"
                     << "Available keys in the program:\n"
                     << "\033[1m Q\033[0m: Eraser\n"
                     << "\033[1m E\033[0m: Brush\n"
                     << "\033[1m +\033[0m: Increase brush size\n"
                     << "\033[1m -\033[0m: Decrease brush size\n"
                     << "\033[1m R\033[0m: Drawing rectangles\n"
                     << "\033[1m O\033[0m: Drawing ovals\n"
                     << "\033[1m S\033[0m: Drawing stars\n"
                     << "\033[1m F\033[0m: Fill with color\n"
                     << "\033[1m C\033[0m: Clear canvas\n"
                     << "\033[1m CTRL\033[0m + \033[1mS\033[0m: Save image"
                     << "\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m: Save the image with a new name" << endl;
                return 0;
            }
            else if (arg == "-v" || arg == "--version")
            {
                cout << "Version: \033[1m" << VERSION << "\033[0m" << endl;
                return 0;
            }
            else
            {
                file_path = arg;
                break;
            }
        }
    }
    else
    {
        cout << "\033[1mEnter the path to the file: \033[0m";
        getline(std::cin, file_path);
    }

    // Проверка наличия файла
    if (std::ifstream(file_path).is_open())
    {
        // Запрос увлечения
        cout << "Enter canvas factor: ";
        getNumberOrChar(img.factor);

        // Заполнение структуры изображения
        //  Формат изображения PNG
        if (file_path.substr(file_path.length() - 4) == ".png")
            img.format = "png";
        //  Формат изображения JPG
        else if (file_path.substr(file_path.length() - 4) == ".jpg" || file_path.substr(file_path.length() - 5) == ".jpeg")
            img.format = "jpg";
        //  Формат изображения ZPIF
        else if (file_path.substr(file_path.length() - 5) == ".zpif")
        {
            img.format = "zpif";

            // Парсинг параметров
            if (parserParams(img, file_path) < 0) return 1;
        }
        //  Ошибка не поддерживаемого формата
        else
        {
            cerr << "\033[1;31mError 2: Unsupported file format.\033[0m" << endl;
            return 1;
        }
    }
    else
    {
        char isCreateFile {0};
        cout << "File does not exist create? [Y/n] ";

        getNumberOrChar(isCreateFile);

        if (isCreateFile != 'Y' && isCreateFile != 'y')
            return 0;

        // Запрос данных
        cout << "Enter canvas width: ";
        getNumberOrChar(img.width);
        cout << "Enter canvas height: ";
        getNumberOrChar(img.height);
        cout << "Enter canvas factor: ";
        getNumberOrChar(img.factor);

        // Проверка размера имени файла
        if (file_path.size() < 5)
        {
            cerr << "\033[1;31mFile name/path is too short '" << file_path << "\'\033[0m" << endl;
            return 1;
        }

        // Параметры изображения
        //  Формат PNG
        if (file_path.substr(file_path.length() - 4) == ".png")
            img.format = "png";

        //  Формат PNG
        else if (file_path.substr(file_path.length() - 4) == ".jpg" || file_path.substr(file_path.length() - 5) == ".jpeg")
            img.format = "jpg";

        //  Формат ZPIF
        else if (file_path.substr(file_path.length() - 5) == ".zpif")
            img.format = "zpif";

        // Ошибка не поддерживаемого формата
        else
        {
            cerr << "\033[1;31mError 2: Unsupported file format.\033[0m" << endl;
            return 1;
        }
    }

    // Загрузка изображения в холст
    if(loadingImage(img, canvas, texture, file_path, std::ifstream(file_path).is_open()) < 0) return 1;

    // Рендер
    if(render(img, canvas, texture, file_path, file_path_temp) < 0) return 1;
}