/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#ifndef _COMPRESS_HPP_
#define _COMPRESS_HPP_

#define NPOS std::string::npos

// Объявления типов
typedef unsigned int u_int;

// Добавления в область видимости
using std::cout;
using std::cerr;
using std::endl;

// Сжатие
char compress_rle(std::string &input, std::string &output, Image &img) {
    // Объявления
    std::string line; // Строка из файла
    std::string buffer; // Строка для записи в файл 
    u_int count = 1; // Количество одинаковых пикселей под рад
    bool header_written = false; // Идут ли данные о пикселях
    
    // Открытие файлов
    std::ifstream input_file(input); // tmp файл или файл для чтения данных
    std::ofstream output_file(output, std::ios::binary); // Файл для записи

    // Запись параметров в файл
    output_file << "{f}" << '(' << img.format << ")\n";
    output_file << "{c}" << '(' << img.compression << ")\n";
    output_file << "{m}" << '(' << img.mode << ")\n";
    output_file << "{w}" << '(' << img.width << ")\n";
    output_file << "{h}" << '(' << img.height << ")\n";

    // Запись начала данных о пикселях
    output_file << "@s@" << '\n';

    // Проверка на открытие файлов
    if (!input_file.is_open()||!input_file.is_open()) {
        cerr << "\033[31mError opening or create main file.\033[0m" << endl;
        return -1;
    }

    while (getline(input_file, line)) {

        // Проверка на начало данных о пикселях
        if (!header_written) {
            if (line.find("@s@") != NPOS)   header_written = true;
            continue;
        }

        // Проверка на конец данных о пикселях
        if (line.find("@e@") != NPOS) {
            if (count > 1) {
                output_file << '(' << count << ')' << buffer << '\n';
            } else {
                output_file << buffer << '\n';
            }
            break;
        }

        // Сжатие пикселей
        //  Проверка данных из буфера и строки файла (одинаковые ли они)
        if (line.find(buffer) != NPOS && !buffer.empty()) 
            count++; // Увеличения количество одинаковых пикселей под рад
        else {
            // Запись в файл данные о пикселях с учетом сжатия
            if (count > 1) {
                output_file << '(' << count << ')' << buffer << '\n';
            } else if (!buffer.empty()) {
                output_file << buffer << '\n';
            }
            buffer = line;
            count = 1;
        }
    }
    
    // Запись конца данных о пикселях
    output_file << "@e@";

    // Закрытие файлов
    input_file.close();
    output_file.close();
    return 0;
}

#endif