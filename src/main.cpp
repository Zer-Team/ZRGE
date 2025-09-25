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
///                      Version: 1.0.8                      ///
///                     Date: 2025-09-25                     ///
///                     Author: Zer Team                     ///
////////////////////////////////////////////////////////////////

// Include
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <clocale>
#include "../include/image.hpp"
#include "../include/locale.hpp"
#include "../include/utils.hpp"
#include "../include/parser.hpp"
#include "../include/graphics.hpp"
#include "../include/load_image.hpp"

// Макросы
#define VERSION    "1.0.8"                           // Версия
#define NPOS       std::string::npos                 // NPOS

int main(int argc, char **argv)
{

    // Устанавливаем локаль из переменных окружения (LANG, LC_*)
    std::setlocale(LC_ALL, "");

    Image img;                                        // Изображения
    std::string file_path_temp{".tempZRGEfile.tmp"}; // Путь к временному файлу
    std::string file_path;                            // Путь к файлу
    std::string icon_path{"/usr/share/zrge"};        // Путь к иконкам
    sf::Image canvas;                                 // Холст
    sf::Texture texture;                              // Текстура холста

    // Локаль
    // Получаем текущую локаль для всех категорий
    const string locale_use = std::setlocale(LC_ALL, nullptr);

    if (locale_use.find("ru_RU") != NPOS)
        locale = &ru_locale;
    else if (locale_use.find("fr_FR") != NPOS)
        locale = &fr_locale;
    else if (locale_use.find("de_DE") != NPOS)
        locale = &de_locale;
    else if (locale_use.find("es_ES") != NPOS)
        locale = &es_locale;

    // Получения пути к файлу
    if (argc > 1)
    {
        for (uint16_t i = 1; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help")
            {
                cout << locale->help << endl;
                return 0;
            }
            else if (arg == "-v" || arg == "--version")
            {
                cout << locale->version << ": \033[1m" << VERSION << "\033[0m" << endl;
                return 0;
            }
            else if (arg.find("-pathicon=") != NPOS)
            {
                icon_path = arg.substr(10);
            }
            else if (arg.find("-lang=") != NPOS)
            {
                string l = arg.substr(6);
                if (l.find("ru") != NPOS)
                    locale = &ru_locale;
                else if (l.find("fr") != NPOS)
                    locale = &fr_locale;
                else if (l.find("de") != NPOS)
                    locale = &de_locale;
                else if (l.find("es") != NPOS)
                    locale = &es_locale;
                else
                    locale = &en_locale;
            }
            else
            {
                file_path = arg;
            }
        }
    cout << "\033[1;33m" << locale->welcome << ' ' << locale->zrge << ' ' << VERSION << "!\033[0m" << endl;
    }
    else
    {
        cout << "\033[1;33m" << locale->welcome << ' ' << locale->zrge << ' ' << VERSION << "!\033[0m" << endl;
        cout << "\033[1m" << locale->enter_file_path << ": \033[0m";
        getline(std::cin, file_path);
    }

    // Проверка наличия файла
    if (std::ifstream(file_path).is_open())
    {
        // Запрос увлечения
        cout << locale->enter_canvas_factor;
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
            cerr << "\033[1;31m" << locale->error_no_supp_file << ".\033[0m" << endl;
            return 1;
        }
    }
    else
    {
        char isCreateFile {0};
        cout << locale->create_file << " [Y/n] ";

        getNumberOrChar(isCreateFile);

        if (isCreateFile != 'Y' && isCreateFile != 'y')
            return 0;

        // Запрос данных
        cout << locale->enter_img_w;
        getNumberOrChar(img.width);
        cout << locale->enter_img_h;
        getNumberOrChar(img.height);
        cout << locale->enter_canvas_factor;
        getNumberOrChar(img.factor);

        // Проверка размера имени файла
        if (file_path.size() < 5)
        {
            cerr << "\033[1;31m" << locale->error_path_len << " \'" << file_path << "\'\033[0m" << endl;
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
            cerr << "\033[1;31m" << locale->error_no_supp_file << "\033[0m" << endl;
            return 1;
        }
    }

    // Загрузка изображения в холст
    if(loadingImage(img, canvas, texture, file_path, std::ifstream(file_path).is_open()) < 0) return 1;

    // Рендер
    if(render(img, canvas, texture, icon_path, file_path, file_path_temp) < 0) return 1;
}