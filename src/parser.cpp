#include "../include/parser.hpp"

// Функция для чтения unsigned short из файла в формате big-endian
uint16_t convertBEInShort(const std::array<uint8_t, 6> &bytes)
{
    return (bytes[0] << 8) | bytes[1];
}

// Функция для чтения unsigned int из файла в формате big-endian
uint64_t convertBEInInt(const std::array<uint8_t, 6> &bytes)
{
    return (static_cast<uint32_t>(bytes[1]) << 24) | // Не с 0 потому что 0 хранит имя параметра 
           (static_cast<uint32_t>(bytes[2]) << 16) |
           (static_cast<uint32_t>(bytes[3]) << 8)  |
           (static_cast<uint32_t>(bytes[4]));
}

// Функция парсинга параметров
signed char parserParams(Image &img, const std::string &filepath)
{
    std::array<uint8_t, 6> buffer;                  // Буфер для хранения данных
    std::ifstream file{filepath, std::ios::binary}; // Файл для чтения

    // Чтение первой строки
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

    // Проверка заголовка
    if (buffer != std::array<uint8_t, 6>{0x89, 'Z', 'P', 'I', 'F', 0x0A})
    {
        cerr << "\033[1;31mError 1: The file is damaged or the format is not supported.\033[0m" << std::endl;
        return -1;
    }

    // Парсинг параметров из файла
    while (file.read(reinterpret_cast<char *>(buffer.data()), buffer.size()))
    {
        // Парсинг размеров изображения
        if (buffer[0] == 0x77)      // w
            img.width = convertBEInInt(buffer);

        else if (buffer[0] == 0x68) // h
            img.height = convertBEInInt(buffer);

        // Прекращение чтения при достижении начала данных о пикселях
        else if (buffer != std::array<uint8_t, 6>{0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF})
        {
            if (img.width <= 0 || img.height <= 0)
            {
                cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << std::endl;
                return -2;
            }

            img.renderStart = file.tellg();
            return 1;
        }
    }
            
    return 0;
}