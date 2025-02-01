/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#ifndef _COMPRESS_HPP_ 
#define _COMPRESS_HPP_

#include <stdint.h>
#include <ctime>

// Объявления типов
typedef unsigned char  u_int8_t;
typedef unsigned short u_short;

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Преобразование числа u_short в big-endian
inline void convertNumberInBE(const u_short value, std::vector<u_int8_t> &bytes, const u_int8_t &shift = 0)
{
    bytes[0 + shift] = value >> 8;
    bytes[1 + shift] = value & 0xFF;
}

// Преобразование числа u_int в big-endian
inline void convertNumberInBE(const uint64_t value, std::vector<u_int8_t> &bytes)
{
    bytes[1] = (value >> 24) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 8)  & 0xFF;
    bytes[4] = value & 0xFF;
}

// Сжатие RLE
signed char compress_rle(const std::string &input, const std::string &output, const Image &img)
{
    std::ifstream inputFile(input, std::ios::binary);
    std::ofstream outputFile(output, std::ios::binary);

    if (!inputFile || !outputFile)
    {
        cerr << "\033[31mError opening file.\033[0m" << endl;
        return -1;
    }

    const size_t bufferSize = img.width * img.height * 6; // Размер буфера

    std::vector<u_int8_t> buffer(bufferSize);             // Буфер для хранения данных
    std::vector<u_int8_t> bufferInp(6),                   // Буфер для чтения
                          bufferOut(6);                   // Буфер для записи
    u_short count {1};                                    // Количество символов подряд

    std::time_t nowTime = std::time(nullptr);             // Получения текущее время
    std::tm *localTime = std::localtime(&nowTime);        // Преобразования в локальное время

    // Запись заголовка
    outputFile.write("\x89ZPIF\x0A", 6);
    
    // Запись размеров изображения
    bufferOut[0] = '\x77'; // w
    convertNumberInBE(static_cast<uint64_t>(img.width), bufferOut);
    outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
    bufferOut[0] = '\x68'; // h
    convertNumberInBE(static_cast<uint64_t>(img.height), bufferOut);
    outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
    // Время
    bufferOut[0] = '\x74'; // t
    convertNumberInBE(static_cast<u_short>(localTime->tm_year + 1900), bufferOut, 1); // Преобразования года
    bufferOut[3] = static_cast<u_int8_t>(localTime->tm_mon + 1);
    bufferOut[4] = static_cast<u_int8_t>(localTime->tm_mday);
    bufferOut[5] = static_cast<u_int8_t>(localTime->tm_hour);

    outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
    
    // Запись начала данных о пикселях
    outputFile.write("\x00\x00\xFF\xFF\xFF\xFF", 6);

    // Чтение первого блока
    if (inputFile.read(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size()))
    {
        // Чтения остальных блоков
        while (inputFile.read(reinterpret_cast<char *>(bufferInp.data()), bufferInp.size()))
        {
            // Завершаем сжатие при встрече 6 нулевых байтов (конца файла)
            if (bufferInp == std::vector<u_int8_t>(6, 0x00))
                break;

            // Проверка данных из буфера out и буфера inp (одинаковые ли они) и не превышает ли count двух байтов
            if (bufferInp == bufferOut && count < 65535)
                count++;
            else
            {
                // Проверка нужна ли запись count в блок буфера
                if (count > 1)
                    convertNumberInBE(count, bufferOut);
                outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
                bufferOut = bufferInp;
                count = 1;
            }
        }
        if (count > 1)
            convertNumberInBE(count, bufferOut);
        buffer.insert(buffer.end(), bufferOut.begin(), bufferOut.end());
    }

    // Запись конца данных
    outputFile.write("\x00\x00\x00\x00\x00\x00", 6);
    return 0;
}

#endif