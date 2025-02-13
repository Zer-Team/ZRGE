#include "../include/load_image.hpp"

signed char loadingImage(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, bool isOpenFile)
{
    // Проверки форматов
    if ((img.format == "png" || img.format == "jpg") && isOpenFile)
    {
        // Загрузка изображения
        if (!canvas.loadFromFile(filepath))
        {
            std::cerr << "\033[1;31mError 3: The file format is not supported or the file is damaged. CHECK THE FILE.\033[0m" << std::endl;
            return -3;
        }

        // Получение размеров изображения
        img.width = canvas.getSize().x;
        img.height = canvas.getSize().y;
    }
    else if (img.format == "zpif" && isOpenFile)
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

            uint16_t quantity {convertBEInShort(buffer)};

            sf::Color color(buffer[2], buffer[3], buffer[4], buffer[5]);

            for (uint16_t i = 0; i < quantity; ++i) {
                canvas.setPixel(sf::Vector2u((img.point % img.width), (img.point / img.width)), color);
                img.point++;
            }
        }

        inputFile.close();
    }
    else if (img.format == "zpif" || img.format == "png" || img.format == "jpg")
        // Создания холста
        canvas.resize(sf::Vector2u(img.width, img.height), sf::Color::White);
    else
        return -1;

    // Изменение размеров текстуры
    if (!texture.resize(canvas.getSize()))
    {
        std::cerr << "\033[1;31mError: Failed to resize texture! Width=" << img.width << ", Height=" << img.height << "\033[0m" << std::endl;
        return 1;
    }
    texture.update(canvas);
    img.rgba[0] = img.rgba[1] = img.rgba[2] = img.rgba[3] = 0;
    img.rgba[3] = 255;

    return 0;
}