#include "../include/compress.hpp"

// Преобразование числа u_short в big-endian
void convertNumberInBE(const uint16_t value, std::array<uint8_t, 6> &bytes, const uint8_t &shift)
{
    bytes[0 + shift] = value >> 8;
    bytes[1 + shift] = value & 0xFF;
}

// Преобразование числа u_int в big-endian
void convertNumberInBE(const uint32_t value, std::array<uint8_t, 6> &bytes)
{
    bytes[1] = (value >> 24) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 8)  & 0xFF;
    bytes[4] = value & 0xFF;
}

// Сжатие RLE
signed char compress_rle(const std::string &input, const std::string &output, const Image &img)
{
    std::ifstream inputFile(input, std::ios::binary);   // Файл для чтения
    std::ofstream outputFile(output, std::ios::binary); // Файл для записи

    if (!inputFile || !outputFile)
    {
        cerr << "\033[31mError opening file.\033[0m" << endl;
        return -1;
    }

    std::array<uint8_t, 6> bufferInp,                     // Буфер для чтения
                           bufferOut;                     // Буфер для записи
    uint16_t count {1};                                   // Количество символов подряд

    std::time_t nowTime {std::time(nullptr)};             // Получения текущее время
    std::tm *localTime {std::localtime(&nowTime)};        // Преобразования в локальное время

    // Буфер для накопления данных перед записью в файл
    std::vector<uint8_t> fileBuffer;

    // Запись заголовка
    const char header[] = "\x89ZPIF\x0A";
    fileBuffer.insert(fileBuffer.end(), header, header + 6);
    
    // Запись размеров изображения
    bufferOut[0] = '\x77'; // w
    convertNumberInBE(static_cast<uint32_t>(img.width), bufferOut);
    fileBuffer.insert(fileBuffer.end(), bufferOut.begin(), bufferOut.end());
    bufferOut[0] = '\x68'; // h
    convertNumberInBE(static_cast<uint32_t>(img.height), bufferOut);
    fileBuffer.insert(fileBuffer.end(), bufferOut.begin(), bufferOut.end());
    
    // Время
    bufferOut[0] = '\x74'; // t
    //   Преобразования года
    convertNumberInBE(static_cast<uint16_t>(localTime->tm_year + 1900), bufferOut, 1);
    //   Месяц день час
    bufferOut[3] = static_cast<uint8_t>(localTime->tm_mon + 1);
    bufferOut[4] = static_cast<uint8_t>(localTime->tm_mday);
    bufferOut[5] = static_cast<uint8_t>(localTime->tm_hour);

    fileBuffer.insert(fileBuffer.end(), bufferOut.begin(), bufferOut.end());
    
    // Запись начала данных о пикселях
    const char pixelDataStart[] = "\x00\x00\xFF\xFF\xFF\xFF";
    fileBuffer.insert(fileBuffer.end(), pixelDataStart, pixelDataStart + 6);

    // Чтение первого блока
    if (inputFile.read(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size()))
    {
        // Чтения остальных блоков
        while (inputFile.read(reinterpret_cast<char *>(bufferInp.data()), bufferInp.size()))
        {
            // Завершаем сжатие при встрече 6 нулевых байтов (конца файла)
            if (bufferInp == std::array<uint8_t, 6>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
                break;

            // Проверка данных из буфера out и буфера inp (одинаковые ли они) и не превышает ли count двух байтов
            if (bufferInp == bufferOut && count < 65535)
                count++;
            else
            {
                // Проверка нужна ли запись count в блок буфера
                if (count > 1)
                    convertNumberInBE(count, bufferOut);
                fileBuffer.insert(fileBuffer.end(), bufferOut.begin(), bufferOut.end());
                bufferOut = bufferInp;
                count = 1;
            }
        }
        if (count > 1)
            convertNumberInBE(count, bufferOut);
        fileBuffer.insert(fileBuffer.end(), bufferOut.begin(), bufferOut.end());
    }

    // Запись конца данных
    const char pixelDataEnd[] = "\x00\x00\x00\x00\x00\x00";
    fileBuffer.insert(fileBuffer.end(), pixelDataEnd, pixelDataEnd + 6);

    // Запись всего буфера в файл
    outputFile.write(reinterpret_cast<char *>(fileBuffer.data()), fileBuffer.size());

    return 0;
}