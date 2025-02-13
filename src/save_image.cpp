#include "../include/save_image.hpp"

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
            sf::Color pixelColor = image.getPixel(sf::Vector2u(x, y));

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