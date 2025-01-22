/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

////////////////////////////////////////////////////////////////
///                       GCC   14.2.1                       ///
///                        SFML 2.6.2                        ///
///                         C++   20                         ///
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
///                        ID: HM0100                        ///
///                      Version: 1.0.2                      ///
///                     Date: 2025-01-22                     ///
///                     Author: Zer Team                     ///
////////////////////////////////////////////////////////////////

// Библиотеки
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <stdexcept>
#include <limits>

//   Собственные
#include "./include/classes.hpp"
#include "./include/draw.hpp"
#include "./include/parser.hpp"
#include "./include/save_image.hpp"

// Макросы
#define VERSION               "1.0.2"                                          // Версия
#define INDENT_X              10                                               // Разделитель
// Размеры
//  Просмотр цвета
#define HEIGHT_COLOR_PREVIEW  50
#define COLOR_FACTOR          1                                                // Дополнения цвета (рассчитывается по размеру слайдера если он меньше 255)
//  Индикатор и слайдер в ./include/draw.hpp
#define Y_SLIDER              HEIGHT_COLOR_PREVIEW+HEIGHT_COLOR_INDICATOR*2+10 // Y координата для слайдеров

#define COLOR_ERASING sf::Color::White

// Объявления типов
typedef unsigned short u_short;

// Добавления в область видимости
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

template <typename T>

// Функция для вывода чисел
void getNumberAndChar(T &num)
{
    // Запрос
    cin >> num;
    // Очистка
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Главная функция
int main(int argc, char **argv)
{
    Image img;                                       // Изображения
    std::string filepath_temp{".tempZRGE_file.tmp"}; // Путь к временному файлу
    std::string filepath;                            // Путь к файлу
    std::string stringFile;                          // Хранения строки файлов
    std::ifstream file;                              // Файл для парсинга
    u_short factor{1};                               // Увеличения
    char Error{0};                                   // Ошибки
    sf::Image canvas;                                // Холст
    sf::Texture texture;                             // Текстура холста

    cout << "\033[1;33mWelcome to ZeR Graphics Editor " << VERSION << "!\033[0m" << endl;

    // Получения пути к файлу
    if (argc > 1)
    {
        for (u_short i = 1; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help")
            {
                cout << "\033[1;33mUsage: zrge [file path]\033[0m" << endl;
                cout << "Available keys in the program:\n"
                     << "\033[1m Q\033[0m: Eraser\n"
                     << "\033[1m E\033[0m: Brush\n"
                     << "\033[1m L\033[0m: Finishing the lines\n"
                     << "\033[1m +\033[0m: Increase brush size\n"
                     << "\033[1m -\033[0m: Decrease brush size\n"
                     << "\033[1m R\033[0m: Drawing rectangles\n"
                     << "\033[1m O\033[0m: Drawing ovals\n"
                     << "\033[1m F\033[0m: Fill with color\n"
                     << "\033[1m C\033[0m: Clear canvas\n"
                     << "\033[1m CTRL\033[0m + \033[1mS\033[0m: Save image" << endl;
                return 0;
            }
            else if (arg == "-v" || arg == "--version")
            {
                cout << "Version: \033[1m" << VERSION << "\033[0m" << endl;
                return 0;
            }
            else
            {
                filepath = arg.data();
                break;
            }
        }
    }
    else
    {
        cout << "\033[1mEnter the path to the file: \033[0m";
        getline(cin, filepath);
    }

    // Проверка наличия файла
    if (std::ifstream(filepath).is_open())
    {
        // Запрос увлечения
        cout << "Enter canvas factor: ";
        getNumberAndChar(factor);

        // Заполнение структуры изображения
        //  Формат изображения PNG
        if (filepath.substr(filepath.length() - 4) == ".png")
        {
            img.format = "png";
            img.compression = "rle";
        }
        //  Формат изображения JPG
        if (filepath.substr(filepath.length() - 4) == ".jpg" || filepath.substr(filepath.length() - 5) == ".jpeg")
        {
            img.format = "jpg";
            img.compression = "rle";
        }
        //  Формат изображения ZPIF
        else if (filepath.substr(filepath.length() - 5) == ".zpif")
        {
            img.format = "zpif";

            file.open(filepath, std::ios::binary);

            // Парсинг параметров из файла
            while (getline(file, stringFile))
            {
                // Пропуск комментариев и пустых строк.
                if (stringFile[0] == '\n' || stringFile.empty())
                    continue;

                // Вызов парсира на текущей строке
                Error = parserParam(stringFile, img);
                if (Error == 1)
                    break;
                if (Error != 0)
                    return 1;
            }
            stringFile.clear();
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
        char createFile{'\0'};
        cout << "File does not exist create? [Y/n] ";

        getNumberAndChar(createFile);

        if (createFile != 'Y' && createFile != 'y')
            return 0;

        // Запрос данных
        cout << "Enter canvas width: ";
        getNumberAndChar(img.width);
        cout << "Enter canvas height: ";
        getNumberAndChar(img.height);
        cout << "Enter canvas factor: ";
        getNumberAndChar(factor);
        // Запрос на использование сжатия
        cout << "\033[1mUse RLE compression? [Y/n] \033[0m";
        getNumberAndChar(img.compression);
        if (img.compression == "y" || img.compression == "Y")
            img.compression = "rle";
        else
            img.compression = "0";

        // Параметры изображения
        //  Формат PNG
        if (filepath.substr(filepath.length() - 4) == ".png")
            img.format = "png";

        //  Формат PNG
        else if (filepath.substr(filepath.length() - 4) == ".jpg" || filepath.substr(filepath.length() - 5) == ".jpeg")
            img.format = "jpg";

        //  Формат ZPIF
        else if (filepath.substr(filepath.length() - 5) == ".zpif")
            img.format = "zpif";

        // Ошибка не поддерживаемого формата
        else
        {
            cerr << "\033[1;31mError 2: Unsupported file format.\033[0m" << endl;
            return 1;
        }
    }

    // Загрузка файла в холст
    if ((img.format == "png" || img.format == "jpg") && std::ifstream(filepath).is_open())
    {
        // Загрузка изображения
        canvas.loadFromFile(filepath);

        // Получение размеров изображения
        img.width = canvas.getSize().x;
        img.height = canvas.getSize().y;
    }
    else if (img.format == "png" || img.format == "jpg")
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::White);
    }
    else if (img.format == "zpif")
    {
        // Создания холста
        canvas.create(img.width, img.height, sf::Color::White);
    }
    else
        return 1;

    // Загрузка холста в текстуру
    texture.loadFromImage(canvas);

    // Создания окна
    sf::RenderWindow window(sf::VideoMode(img.width * factor + WIDTH_COLOR_SLIDER + INDENT_X * 2, img.height * factor > Y_SLIDER * 12 ? img.height * factor : Y_SLIDER * 12), "ZeR Graphics Editor", sf::Style::Close);

    // Создание индикатора
    sf::Sprite sprite(texture);

    // Загрузка иконки
    sf::Image icon;
    if (!icon.loadFromFile("/usr/share/zrge/images/icon.png"))
    {
        std::cerr << "\033[1;31mError: Failed to load icon!\033[0m" << std::endl;
        return 1;
    }

    // Установка иконки окна
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); // Установка иконки

    // Растягивание текстуры
    sprite.setScale(img.width * factor / texture.getSize().x, img.height * factor / texture.getSize().y);

    // Функция заливки цветом
    auto fillColor = [&](int x, int y, sf::Color newColor) {
        sf::Color targetColor = canvas.getPixel(x, y);
        if (targetColor == newColor) return; // Если цвет совпадает, ничего не делаем

        std::stack<sf::Vector2i> stack;
        stack.push({x, y});

        while (!stack.empty()) {
            sf::Vector2i pos = stack.top();
            stack.pop();

            if (pos.x < 0 || pos.x >= img.width || pos.y < 0 || pos.y >= img.height) continue;
            if (canvas.getPixel(pos.x, pos.y) != targetColor) continue;

            canvas.setPixel(pos.x, pos.y, newColor);

            stack.push({pos.x + 1, pos.y});
            stack.push({pos.x - 1, pos.y});
            stack.push({pos.x, pos.y + 1});
            stack.push({pos.x, pos.y - 1});
        }

        texture.update(canvas);
    };

    // Переменные для холста
    sf::Color brushColor = sf::Color::Black; // Цвет кисти
    //   Для проверок
    bool isErasing = false,         // Стёрка (false)
        isPaint = true,             // Рисование (true)
        isSave = true,              // Сохранено (true)
        isPipette = false,          // Пипетка (false)
        isDrawingRect = false,      // Рисования квадрата (false)
        isDrawingOval = false,      // Рисования овала (false)
        isFigureBeingDrawn = false, // Предпросмотр фигур (false)
        isPouring = false;          // Заливка цветом (false)
    //   Размер кисти
    u_short brushSize = 5;
    //   Дорисовывания
    sf::Vector2i prevMousePos(-1, -1); // Последняя позиция мыши
    //   Предпросмотр фигур
    sf::Vector2i figuresStart(-1, -1), figuresEnd(-1, -1); // Первая и последняя позиции курсора для рисования фигур
    //      Объект для предпросмотра рисования фигур
    sf::RectangleShape previewFigures;
    //      Настройки объекта
    previewFigures.setFillColor(sf::Color::Transparent); // Цвет фона
    previewFigures.setOutlineThickness(2);      // Размер контура
    previewFigures.setOutlineColor(sf::Color::Red);      // Цвет контура
    //   Какой цвет настраивается
    bool draggingR = false, draggingG = false, draggingB = false;

    // Элементы интерфейса
    // Текстуры для кнопок
    sf::Texture buttonBrushTexture, buttonEraserTexture, buttonPipetteTexture, buttonPlusTexture, buttonMinusTexture, buttonDrawRectTexture, buttonDrawOvalTexture, buttonPouringTexture;
    // Загрузка текстур кнопок
    if (!buttonBrushTexture.loadFromFile("/usr/share/zrge/images/brush.png") ||
        !buttonEraserTexture.loadFromFile("/usr/share/zrge/images/eraser.png") ||
        !buttonPlusTexture.loadFromFile("/usr/share/zrge/images/plus.png") ||
        !buttonMinusTexture.loadFromFile("/usr/share/zrge/images/minus.png") ||
        !buttonPipetteTexture.loadFromFile("/usr/share/zrge/images/pipette.png") ||
        !buttonDrawRectTexture.loadFromFile("/usr/share/zrge/images/draw_rect.png") ||
        !buttonDrawOvalTexture.loadFromFile("/usr/share/zrge/images/draw_oval.png") ||
        !buttonPouringTexture.loadFromFile("/usr/share/zrge/images/pouring.png"))
    {
        std::cerr << "\033[1;31mError: Failed to load icon!\033[0m" << std::endl;
        return -1;
    }

    // Кнопки
    sf::Sprite buttonBrush, buttonEraser, buttonPipette, buttonPlus, buttonMinus, buttonDrawRect, buttonDrawOval, buttonPouring;
    // Слайдеры
    sf::RectangleShape sliderR, sliderG, sliderB, colorPreview, indent;
    sliderR.setFillColor(sf::Color::Red);
    sliderG.setFillColor(sf::Color::Green);
    sliderB.setFillColor(sf::Color::Blue);
    // Разделитель
    indent.setFillColor(sf::Color(60, 60, 60));
    indent.setSize(sf::Vector2f(INDENT_X / 2, window.getSize().y));
    indent.setPosition(img.width * factor, 0);
    // Показатель цвета
    colorPreview.setSize(sf::Vector2f(WIDTH_COLOR_SLIDER + INDENT_X + INDENT_X / 2, HEIGHT_COLOR_PREVIEW));
    colorPreview.setPosition(img.width * factor + INDENT_X / 2, 0);
    colorPreview.setFillColor(brushColor);

    // Загрузка изображения текстуры
    if (std::ifstream(filepath).is_open() && img.format == "zpif")
    {
        // Парсинг и заполнения пикселя
        while (getline(file, stringFile))
        {
            // Пропускаем пустые строки и комментарии
            if (stringFile.empty() || stringFile[0] == '\n')
                continue;

            // Парсинг пикселя
            Error = parserPixel(stringFile, img);

            if (Error < 0)
                return -1;
            if (Error == 1)
                break; // Завершение рендера

            // Проверка на равенства
            if (img.point > img.width * img.height)
                break;

            // Устанавливаем цвет пикселя
            sf::Color color(img.rgb[0], img.rgb[1], img.rgb[2]);

            // Обработка сжатия
            if (img.quantity > 0 && containsString(img.compression, "rle"))
            {
                while (img.quantity > 0 && (img.point < img.width * img.height))
                {
                    canvas.setPixel(((img.point - 1) % img.width), ((img.point - 1) / img.width), color);
                    img.quantity--;
                    img.point++;
                }
            }
            // Заполнения пикселя
            canvas.setPixel(((img.point - 1) % img.width), ((img.point - 1) / img.width), color);
            // Проверка на на равенства
            if (img.point + img.quantity > img.width * img.height)
                cout << "\033[1;33mWARNING: Number of pixels exceeds available\033[0m" << endl;
        }

        // Обновления текстуру из изменённого изображения
        texture.update(canvas);
        file.close();
        img.rgb[0] = img.rgb[1] = img.rgb[2] = 0;
    }

    // Цикл программы (окна)
    while (window.isOpen())
    {
        sf::Event event;
        
        // Обработка событий
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // Закрытие
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                if (isSave == false)
                {
                    char answer;

                    // Запрос на сохранения изображения
                    cout << "Save image? [Y/n] ";
                    getNumberAndChar(answer);
                    if (answer == 'Y' || answer == 'y')
                    {
                        Error = saveImageZPIF(canvas, img, filepath, filepath_temp, Error);
                        if (Error < 0)
                            return 1;
                    }
                }
                cout << "\033[1;33mExit\033[0m" << endl;
                window.close();
                return 0;
            }

            // Мышь
            //   Нажатие кнопок мыши
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // Кнопки
                //   Кисть
                if (buttonBrush.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isErasing = isDrawingRect = isDrawingOval = isPouring = false;
                //   Ластик
                else if (buttonEraser.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isErasing = true;
                //   Пипетка
                else if (buttonPipette.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isPipette = true;
                //   Плюс (размер кисти)
                else if (buttonPlus.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    brushSize = std::min(brushSize + 3, 50);
                //   Минус (размер кисти)
                else if (buttonMinus.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    brushSize = std::max(brushSize - 3, 1);
                // Заливка цветом
                else if (buttonPouring.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isPouring = true;
                //   Рисования прямоугольников
                else if (buttonDrawRect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isDrawingRect = true, isDrawingOval = isErasing = false;
                //   Рисования овалов
                else if (buttonDrawOval.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                    isDrawingOval = true, isDrawingRect = isErasing = false;
                //   Заливка цветом
                else if (isPouring && mousePos.x >= 0 && mousePos.x / factor < img.width && mousePos.y >= 0 && mousePos.y / factor < img.height) 
                    fillColor(mousePos.x / factor, mousePos.y / factor, brushColor), isSave = false;
                // Слайдеры
                else if (sliderR.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    img.rgb[0] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingR = true;
                    isPaint = false;
                }
                else if (sliderG.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    img.rgb[1] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingG = true;
                    isPaint = false;
                }
                else if (sliderB.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    img.rgb[2] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingB = true;
                    isPaint = false;
                }
                // Рисование фигур
                if ((isDrawingRect || isDrawingOval) && event.mouseButton.button == sf::Mouse::Left && !isPipette)
                {
                    figuresStart = sf::Mouse::getPosition(window), isFigureBeingDrawn = true;
                }
                // Пипетка CTRL
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && (mousePos.x / factor <= img.width && mousePos.y / factor <= img.height))
                {
                    brushColor = canvas.getPixel(mousePos.x / factor, mousePos.y / factor);
                    colorPreview.setFillColor(brushColor);
                    img.rgb[0] = brushColor.r / COLOR_FACTOR;
                    img.rgb[1] = brushColor.g / COLOR_FACTOR;
                    img.rgb[2] = brushColor.b / COLOR_FACTOR;
                    isPipette = false;
                }
            }
            //   Отжатие кнопок мыши
            if (event.type == sf::Event::MouseButtonReleased)
            {
                // Пипетка
                if (isPipette && (mousePos.x / factor <= img.width && mousePos.y / factor <= img.height))
                {
                    brushColor = canvas.getPixel(mousePos.x / factor, mousePos.y / factor);
                    colorPreview.setFillColor(brushColor);
                    img.rgb[0] = brushColor.r / COLOR_FACTOR;
                    img.rgb[1] = brushColor.g / COLOR_FACTOR;
                    img.rgb[2] = brushColor.b / COLOR_FACTOR;
                    isPipette = false;
                }
                // Рисование фигур
                //   Прямоугольник
                else if (isDrawingRect && isFigureBeingDrawn && event.mouseButton.button == sf::Mouse::Left && figuresStart.x / factor < img.width)
                {
                    figuresEnd = sf::Mouse::getPosition(window);

                    // Нормализация координат
                    figuresStart.x /= factor, figuresStart.y /= factor, figuresEnd.x /= factor, figuresEnd.y /= factor;

                    int rectWidth = figuresEnd.x - figuresStart.x, rectHeight = figuresEnd.y - figuresStart.y;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                        rectWidth = rectHeight;
                    if (rectWidth < 0)
                        figuresStart.x += rectWidth, rectWidth = -rectWidth;
                    if (rectHeight < 0)
                        figuresStart.y += rectHeight, rectHeight = -rectHeight;
                    for (int x = figuresStart.x; x < figuresStart.x + rectWidth; ++x)
                    {
                        for (int y = figuresStart.y; y < figuresStart.y + rectHeight; ++y)
                        {
                            if (x >= 0 && x < img.width && y >= 0 && y < img.height)
                                canvas.setPixel(x, y, isErasing ? COLOR_ERASING : brushColor);
                        }
                    }
                    texture.update(canvas);
                    isFigureBeingDrawn = isSave = false;
                }
                //   Овал
                else if (isDrawingOval && isFigureBeingDrawn && event.mouseButton.button == sf::Mouse::Left && figuresStart.x / factor < img.width)
                {
                    figuresEnd = sf::Mouse::getPosition(window);

                    // Нормализация координат
                    figuresStart.x /= factor, figuresStart.y /= factor, figuresEnd.x /= factor, figuresEnd.y /= factor;

                    int rectWidth = figuresEnd.x - figuresStart.x, rectHeight = figuresEnd.y - figuresStart.y;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                        rectWidth = rectHeight;
                    if (rectWidth < 0)
                        figuresStart.x += rectWidth, rectWidth = -rectWidth;
                    if (rectHeight < 0)
                        figuresStart.y += rectHeight, rectHeight = -rectHeight;
                    int centerX = figuresStart.x + rectWidth / 2, centerY = figuresStart.y + rectHeight / 2, radiusX = rectWidth / 2, radiusY = rectHeight / 2;
                    float radiusXSq = static_cast<float>(radiusX * radiusX);
                    float radiusYSq = static_cast<float>(radiusY * radiusY);
                    for (int y = -radiusY; y <= radiusY; ++y)
                    {
                        for (int x = -radiusX; x <= radiusX; ++x)
                        {
                            if ((x * x) / radiusXSq + (y * y) / radiusYSq <= 1.0f)
                            {
                                int pixelX = centerX + x, pixelY = centerY + y;
                                if (pixelX >= 0 && pixelX < img.width && pixelY >= 0 && pixelY < img.height)
                                    canvas.setPixel(pixelX, pixelY, isErasing ? COLOR_ERASING : brushColor);
                            }
                        }
                    }
                    texture.update(canvas);
                    isFigureBeingDrawn = isSave = false;
                }

                draggingR = draggingG = draggingB = false;
                isPaint = true;
            }

            // Передвижения курсора мыши
            if (event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (draggingR)
                {
                    img.rgb[0] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                }
                else if (draggingG)
                {
                    img.rgb[1] = std::clamp(mousePos.x - sliderG.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                }
                else if (draggingB)
                {
                    img.rgb[2] = std::clamp(mousePos.x - sliderB.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                }
            }

            // Клавиатура
            else if (event.type == sf::Event::KeyPressed)
            {
                // Кисточка
                if (event.key.code == sf::Keyboard::E)
                    isErasing = isDrawingOval = isDrawingRect = false;
                // Ластик
                else if (event.key.code == sf::Keyboard::Q)
                    isErasing = true;
                // Рисования прямоугольников
                else if (event.key.code == sf::Keyboard::R)
                    isDrawingRect = true, isDrawingOval = false;
                // Рисования овалов
                else if (event.key.code == sf::Keyboard::O)
                    isDrawingOval = true, isDrawingRect = false;
                // Заливка цветом
                else if (event.key.code == sf::Keyboard::F) 
                    isPouring = true;
                // Очистка
                else if (event.key.code == sf::Keyboard::C)
                {
                    canvas.create(img.width, img.height, sf::Color::White);
                    texture.update(canvas);
                    isSave = true;
                }
                // Увелечения и уменьшения размера кисти
                else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
                    brushSize = std::min(brushSize + 2, 50);
                else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen)
                    brushSize = std::max(brushSize - 2, 1);
                // Сохранение
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    // Проверка нажатия Shift
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    {
                        // Запрос нового названия файла
                        cout << "\033[1mEnter a new file name: \033[0m";
                        getline(cin, filepath);

                        // Получения нового формата файла
                        if (filepath.substr(filepath.length() - 4) == ".png")
                            img.format = "png";
                        else if (filepath.substr(filepath.length() - 4) == ".jpg" || filepath.substr(filepath.length() - 5) == ".jpeg")
                            img.format = "jpg";
                        else if (filepath.substr(filepath.length() - 5) == ".zpif")
                            img.format = "zpif";
                    }

                    // Сохранение PNG и JPG
                    if (img.format == "png" || img.format == "jpg")
                    {
                        if (canvas.saveToFile(filepath))
                        {
                            cout << "\033[32mImage saved successfully.\033[0m" << endl;
                        }
                        else
                        {
                            cerr << "\033[1;31mFailed to save image.\033[0m" << endl;
                            return 1;
                        }
                    }
                    // Сохранение ZPIF
                    else if (img.format == "zpif")
                    {
                        Error = saveImageZPIF(canvas, img, filepath, filepath_temp, Error);
                        if (Error < 0)
                            return 1;
                    }
                    isSave = true;
                }
            }
        }

        // Обновление цвета кисти
        brushColor = sf::Color(img.rgb[0] * COLOR_FACTOR, img.rgb[1] * COLOR_FACTOR, img.rgb[2] * COLOR_FACTOR);
        colorPreview.setFillColor(brushColor);

        // Рисование мышью
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isPaint && !isPouring && !isPipette && !isDrawingOval && !isDrawingRect && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Нормализация координат мыши
            mousePos.x /= factor;
            mousePos.y /= factor;

            int x = mousePos.x;
            int y = mousePos.y;

            if (x >= 0 && x < img.width && y >= 0 && y < img.height)
            {
                sf::Color colorToUse = isErasing ? COLOR_ERASING : brushColor;

                if (prevMousePos.x != -1 && prevMousePos.y != -1)
                {
                    // Линия между предыдущей и текущей позицией
                    sf::Vector2i diff = mousePos - prevMousePos;
                    int steps = std::max(std::abs(diff.x), std::abs(diff.y));

                    for (int step = 0; step <= steps; ++step)
                    {
                        float t = static_cast<float>(step) / steps;
                        int drawX = static_cast<int>(prevMousePos.x + t * diff.x);
                        int drawY = static_cast<int>(prevMousePos.y + t * diff.y);

                        for (int i = -brushSize; i <= brushSize; ++i)
                        {
                            for (int j = -brushSize; j <= brushSize; ++j)
                            {
                                if (i * i + j * j <= brushSize * brushSize)
                                {
                                    int pixelX = drawX + i;
                                    int pixelY = drawY + j;

                                    if (pixelX >= 0 && pixelX < static_cast<int>(img.width) && pixelY >= 0 && pixelY < static_cast<int>(img.height))
                                    {
                                        canvas.setPixel(pixelX, pixelY, colorToUse);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    // Только текущая позиция мыши
                    for (int i = -brushSize; i <= brushSize; ++i)
                    {
                        for (int j = -brushSize; j <= brushSize; ++j)
                        {
                            if (i * i + j * j <= brushSize * brushSize)
                            {
                                int pixelX = x + i;
                                int pixelY = y + j;

                                if (pixelX >= 0 && pixelX < static_cast<int>(img.width) && pixelY >= 0 && pixelY < static_cast<int>(img.height))
                                {
                                    canvas.setPixel(pixelX, pixelY, colorToUse);
                                }
                            }
                        }
                    }
                }

                prevMousePos = mousePos;
                texture.update(canvas);
            }

            isSave = false;
        }
        else
        {
            prevMousePos = {-1, -1};
        }

        // Предпросмотр фигур
        if (isFigureBeingDrawn)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Ширина и высота прямоугольника
            int rectWidth = (mousePos.x / factor > img.width ? img.width : mousePos.x) - figuresStart.x, rectHeight = mousePos.y - figuresStart.y;
            // Квадрат если зажата Shift
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                rectWidth = rectHeight;

            // Отрисовка превью фигуры
            previewFigures.setPosition(figuresStart.x, figuresStart.y), previewFigures.setSize(sf::Vector2f(rectWidth, rectHeight));
        }

        // Очистка окна (цвет фона интерфейса)
        window.clear(sf::Color(120, 120, 120));
        // Отрисовка элементов
        window.draw(sprite);
        //   Предпросмотр фигур
        if (isFigureBeingDrawn && figuresStart.x / factor < img.width && !isPipette)
            window.draw(previewFigures);
        //   Интерфеис
        window.draw(indent);
        window.draw(colorPreview);

        // Создание слайдеров
        drawSlider(window, sliderR, img.rgb[0], sf::Color::Red, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + 10));
        drawSlider(window, sliderG, img.rgb[1], sf::Color::Green, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR + 10 * 2));
        drawSlider(window, sliderB, img.rgb[2], sf::Color::Blue, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 2 + 10 * 3));

        // Создание кнопок
        //   1
        drawButton(window, buttonBrushTexture, buttonBrush, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X, Y_SLIDER * 7));
        drawButton(window, buttonEraserTexture, buttonEraser, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 7));
        drawButton(window, buttonPipetteTexture, buttonPipette, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 7));
        drawButton(window, buttonPlusTexture, buttonPlus, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + (26 + 20) * 3, Y_SLIDER * 7));
        drawButton(window, buttonMinusTexture, buttonMinus, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + (26 + 20) * 4, Y_SLIDER * 7));
        //   2
        drawButton(window, buttonDrawRectTexture, buttonDrawRect, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X, Y_SLIDER * 8 + 40));
        drawButton(window, buttonDrawOvalTexture, buttonDrawOval, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 8 + 40));
        drawButton(window, buttonPouringTexture, buttonPouring, sf::Vector2f((img.width * factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 8 + 40));

        // Отрисовка окна
        window.display();
    }

    return 0;
}

////////////////////////////////////////////////////////////////
///                        ID: HM0100                        ///
////////////////////////////////////////////////////////////////
