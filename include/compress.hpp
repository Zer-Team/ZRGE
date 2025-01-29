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
typedef unsigned char u_int8_t;
typedef unsigned short u_short;

// Добавления в область видимости
using std::cerr;
using std::cout;
using std::endl;

// Преобразование числа в big-endian
inline void convertNumberInBE(u_short value, std::vector<u_int8_t> &buffer)
{
    buffer[0] = value >> 8;
    buffer[1] = value & 0xFF;
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

    // Запись заголовка
    outputFile.write("\xDDZPIF\n", 6);
    outputFile << "{w}(" << img.width << ")\n";
    outputFile << "{h}(" << img.height << ")\n";
    outputFile.write("\x00\x00\xFF\xFF\xFF\xFF\n", 7);

    std::vector<u_int8_t> bufferInp(6), // Буфер для чтения
                          bufferOut(6); // Буфер для записи
    u_short count = 1;

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
                {
                    convertNumberInBE(count, bufferOut);
                }
                outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
                bufferOut = bufferInp;
                count = 1;
            }
        }
        if (count > 1)
        {
            convertNumberInBE(count, bufferOut);
        }
        outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
    }

    // Запись конца данных
    outputFile.write("\x00\x00\x00\x00\x00\x00", 6);
    return 0;
}

#endif