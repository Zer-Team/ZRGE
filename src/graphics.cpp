#include "../include/graphics.hpp"

int render(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, std::string &filepath_temp)
{
    // Получаем размеры экрана:
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.size.x;
    unsigned int screenHeight = desktop.size.y;

    // Создания окна
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(img.width * img.factor + WIDTH_COLOR_SLIDER + INDENT_X * 2, img.height * img.factor > Y_SLIDER * 12 ? img.height * img.factor : Y_SLIDER * 12)), "ZeR Graphics Editor", sf::Style::Close);

    // Установка позиции окна
    window.setPosition(sf::Vector2i(((screenWidth - window.getSize().x) / 2), ((screenHeight - window.getSize().y) / 2)));

    // Создание индикатора
    sf::Sprite sprite(texture);

    // Загрузка иконки и фона
    sf::Image icon;
    sf::Texture backgroundTexture;
    if (!icon.loadFromFile("/usr/share/zrge/images/icon.png") || !backgroundTexture.loadFromFile("/usr/share/zrge/images/background.jpg"))
    {
        std::cerr << "\033[1;31mError: Failed to load image!\033[0m" << std::endl;
        return 1;
    }

    // Установка иконки окна
    window.setIcon(icon);

    // Нормализация фона
    sf::Sprite backgroundSprite{backgroundTexture};

    // Проверка размеров
    if (backgroundTexture.getSize().x == 0 || backgroundTexture.getSize().y == 0 || texture.getSize().y == 0 || texture.getSize().x == 0)
    {
        cerr << "ERROR SIZE\n"
             << "Texture:" << texture.getSize().x << "x" << texture.getSize().y << '\n'
             << "BG texture:" << backgroundTexture.getSize().x << "x" << backgroundTexture.getSize().y << '\n'
             << "Image:" << img.width << "x" << img.height << endl
             << '\n'
             << "Canvas:" << canvas.getSize().x << "x" << canvas.getSize().y << endl;
        return 1;
    }

    // Вычисления масштаба для сохранения пропорций
    float scale = std::max(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    // Установка масштаба спрайта
    backgroundSprite.setScale(sf::Vector2f(scale, scale));

    // Централизация текстуры
    backgroundSprite.setPosition(sf::Vector2f(
        (static_cast<float>(window.getSize().x) - backgroundTexture.getSize().x * scale) / 2.f,
        (static_cast<float>(window.getSize().y) - backgroundTexture.getSize().y * scale) / 2.f));

    // Растягивание текстуры
    sprite.setScale(sf::Vector2f(img.width * img.factor / texture.getSize().x, img.height * img.factor / texture.getSize().y));

    // Переменные для холста
    sf::Color brushColor = sf::Color::Black; // Цвет кисти
    //   Для проверок
    bool isErasing = false,         // Стёрка (false)
        isPaint = true,             // Рисование (true)
        isSave = true,              // Сохранено (true)
        isPipette = false,          // Пипетка (false)
        isDrawingRect = false,      // Рисования квадрата (false)
        isDrawingOval = false,      // Рисования овала (false)
        isDrawingStar = false,      // Рисования звезды (false)
        isFigureBeingDrawn = false, // Предпросмотр фигур (false)
        isPouring = false;          // Заливка цветом (false)
    //   Размер кисти
    uint16_t brushSize = 5;
    //   Дорисовывания
    sf::Vector2i prevMousePos(-1, -1); // Последняя позиция мыши
    //   Предпросмотр фигур
    sf::Vector2f figuresStart(-1, -1), figuresEnd(-1, -1); // Первая и последняя позиции курсора для рисования фигур
    //      Объект для предпросмотра рисования фигур
    sf::RectangleShape previewFigures;
    //      Настройки объекта
    previewFigures.setFillColor(sf::Color::Transparent); // Цвет фона
    previewFigures.setOutlineThickness(2);               // Размер контура
    previewFigures.setOutlineColor(sf::Color::Green);    // Цвет контура
    //   Какой цвет настраивается
    bool draggingR{false}, draggingG{false}, draggingB{false}, draggingA{false};

    // Элементы интерфейса
    // Текстуры для кнопок
    sf::Texture buttonBrushTexture, buttonEraserTexture, buttonPipetteTexture, buttonPlusTexture, buttonMinusTexture, buttonDrawRectTexture, buttonDrawOvalTexture, buttonDrawStarTexture, buttonPouringTexture;
    // Загрузка текстур кнопок
    if (!buttonBrushTexture.loadFromFile("/usr/share/zrge/images/brush.png") ||
        !buttonEraserTexture.loadFromFile("/usr/share/zrge/images/eraser.png") ||
        !buttonPlusTexture.loadFromFile("/usr/share/zrge/images/plus.png") ||
        !buttonMinusTexture.loadFromFile("/usr/share/zrge/images/minus.png") ||
        !buttonPipetteTexture.loadFromFile("/usr/share/zrge/images/pipette.png") ||
        !buttonDrawRectTexture.loadFromFile("/usr/share/zrge/images/draw_rect.png") ||
        !buttonDrawOvalTexture.loadFromFile("/usr/share/zrge/images/draw_oval.png") ||
        !buttonDrawStarTexture.loadFromFile("/usr/share/zrge/images/draw_star.png") ||
        !buttonPouringTexture.loadFromFile("/usr/share/zrge/images/pouring.png"))
    {
        std::cerr << "\033[1;31mError: Failed to load icon!\033[0m" << std::endl;
        return -1;
    }

    // Кнопки
    sf::Sprite buttonBrush(buttonBrushTexture), buttonEraser(buttonEraserTexture), buttonPipette(buttonPipetteTexture),
        buttonPlus(buttonPlusTexture), buttonMinus(buttonMinusTexture), buttonDrawRect(buttonDrawRectTexture),
        buttonDrawOval(buttonDrawOvalTexture), buttonDrawStar(buttonDrawStarTexture), buttonPouring(buttonPouringTexture);
    // Слайдеры
    sf::RectangleShape sliderR, sliderG, sliderB, sliderA, colorPreview, indent;
    sliderR.setFillColor(sf::Color::Red);
    sliderG.setFillColor(sf::Color::Green);
    sliderB.setFillColor(sf::Color::Blue);
    // Разделитель
    indent.setFillColor(sf::Color(60, 60, 60));
    indent.setSize(sf::Vector2f(INDENT_X / 2, window.getSize().y));
    indent.setPosition(sf::Vector2f(img.width * img.factor, 0));
    // Показатель цвета
    colorPreview.setSize(sf::Vector2f(WIDTH_COLOR_SLIDER + INDENT_X + INDENT_X / 2, HEIGHT_COLOR_PREVIEW));
    colorPreview.setPosition(sf::Vector2f(img.width * img.factor + INDENT_X / 2, 0));
    colorPreview.setFillColor(brushColor);
    // Фон для инструментов
    sf::RectangleShape backgroundRight(sf::Vector2f(WIDTH_COLOR_SLIDER + INDENT_X + INDENT_X / 2, window.getSize().y - HEIGHT_COLOR_PREVIEW));
    backgroundRight.setPosition(sf::Vector2f(img.width * img.factor + INDENT_X / 2, HEIGHT_COLOR_PREVIEW));
    backgroundRight.setFillColor(sf::Color(120, 120, 120));

    // Переменные для расчёта FPS
    sf::Clock clock;
    int frameCount = 0;
    float elapsedTime = 0.f;

    // Цикл программы (окна)
    while (window.isOpen())
    {
        // Обработка событий
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            const auto *keyEvent = event->getIf<sf::Event::KeyPressed>(); // Работа с клавиатурой 
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

            // Закрытие окна
            if (event->is<sf::Event::Closed>() || event->is<sf::Event::KeyPressed>() && keyEvent->scancode == sf::Keyboard::Scan::Escape)
            {
                if (isSave == false)
                {
                    char answer;

                    // Запрос на сохранения изображения
                    cout << "Save image? [Y/n] ";
                    getNumberOrChar(answer);
                    if (answer == 'Y' || answer == 'y')
                    {
                        if (saveImageZPIF(canvas, img, filepath, filepath_temp) < 0)
                            return 1;
                    }
                }
                cout << "\033[1;33mExit\033[0m" << endl;
                window.close();
                return 0;
            }

            // Мышь
            //   Нажатие кнопок мыши
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                // Кнопки
                //   Кисть
                if (buttonBrush.getGlobalBounds().contains(mousePos))
                    isErasing = isDrawingRect = isDrawingOval = isDrawingStar = isPouring = false;
                //   Ластик
                else if (buttonEraser.getGlobalBounds().contains(mousePos))
                    isErasing = true;
                //   Пипетка
                else if (buttonPipette.getGlobalBounds().contains(mousePos))
                    isPipette = true;
                //   Плюс (размер кисти)
                else if (buttonPlus.getGlobalBounds().contains(mousePos))
                    brushSize = std::min(brushSize + 3, 50);
                //   Минус (размер кисти)
                else if (buttonMinus.getGlobalBounds().contains(mousePos))
                    brushSize = std::max(brushSize - 3, 1);
                //   Заливка цветом
                else if (buttonPouring.getGlobalBounds().contains(mousePos))
                    isPouring = true, isDrawingRect = isDrawingOval, isDrawingStar = isErasing = false;
                //   Рисования прямоугольников
                else if (buttonDrawRect.getGlobalBounds().contains(mousePos))
                    isDrawingRect = true, isDrawingOval, isDrawingStar = isErasing = isPouring = false;
                //   Рисования овалов
                else if (buttonDrawOval.getGlobalBounds().contains(mousePos))
                    isDrawingOval = true, isDrawingRect, isDrawingStar = isErasing = isPouring = false;
                //   Рисования звезды
                else if (buttonDrawStar.getGlobalBounds().contains(mousePos))
                    isDrawingStar = true, isDrawingRect, isDrawingOval = isErasing = isPouring = false;
                //   Заливка цветом
                else if (isPouring && !isPipette && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) && mousePos.x >= 0 && mousePos.x / img.factor < img.width && mousePos.y >= 0 && mousePos.y / img.factor < img.height)
                    fillColor(mousePos.x / img.factor, mousePos.y / img.factor, img, canvas, texture, isErasing ? sf::Color::Transparent : brushColor), isSave = false;
                // Слайдеры
                else if (sliderR.getGlobalBounds().contains(mousePos))
                {
                    img.rgba[0] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingR = true;
                    isPaint = false;
                }
                else if (sliderG.getGlobalBounds().contains(mousePos))
                {
                    img.rgba[1] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingG = true;
                    isPaint = false;
                }
                else if (sliderB.getGlobalBounds().contains(mousePos))
                {
                    img.rgba[2] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingB = true;
                    isPaint = false;
                }
                else if (sliderA.getGlobalBounds().contains(mousePos))
                {
                    img.rgba[3] = std::clamp(mousePos.x - sliderA.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingA = true;
                    isPaint = false;
                }
                // Рисование фигур
                if ((isDrawingRect || isDrawingOval || isDrawingStar) && mouseEvent->button == sf::Mouse::Button::Left && !isPipette)
                {
                    figuresStart = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), isFigureBeingDrawn = true;
                }
                // Пипетка CTRL
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && (mousePos.x / img.factor <= img.width && mousePos.y / img.factor <= img.height))
                {
                    brushColor = canvas.getPixel(sf::Vector2u(mousePos.x / img.factor, mousePos.y / img.factor));
                    colorPreview.setFillColor(brushColor);
                    img.rgba[0] = brushColor.r;
                    img.rgba[1] = brushColor.g;
                    img.rgba[2] = brushColor.b;
                    img.rgba[3] = brushColor.a;
                    isPipette = false;
                }
            }
            //   Отжатие кнопок мыши
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                const auto *mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();
                // Пипетка
                if (isPipette && (mousePos.x / img.factor <= img.width && mousePos.y / img.factor <= img.height))
                {
                    brushColor = canvas.getPixel(sf::Vector2u(mousePos.x / img.factor, mousePos.y / img.factor));
                    colorPreview.setFillColor(brushColor);
                    img.rgba[0] = brushColor.r;
                    img.rgba[1] = brushColor.g;
                    img.rgba[2] = brushColor.b;
                    img.rgba[3] = brushColor.a;
                    isPipette = false;
                }
                // Рисование фигур
                //   Прямоугольник
                else if (isDrawingRect && isFigureBeingDrawn && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                    // Нормализация координат
                    figuresStart.x /= img.factor, figuresStart.y /= img.factor, figuresEnd.x /= img.factor, figuresEnd.y /= img.factor;

                    int rectWidth = figuresEnd.x - figuresStart.x, rectHeight = figuresEnd.y - figuresStart.y;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
                        rectWidth = rectWidth < 0 && rectHeight > 0 || rectHeight < 0 && rectWidth > 0 ? -rectHeight : rectHeight;
                    if (rectWidth < 0)
                        figuresStart.x += rectWidth, rectWidth = -rectWidth;
                    if (rectHeight < 0)
                        figuresStart.y += rectHeight, rectHeight = -rectHeight;

                    for (int x = figuresStart.x; x < figuresStart.x + rectWidth; ++x)
                    {
                        for (int y = figuresStart.y; y < figuresStart.y + rectHeight; ++y)
                        {
                            if (x >= 0 && x < img.width && y >= 0 && y < img.height)
                                canvas.setPixel(sf::Vector2u(x, y), isErasing ? sf::Color::Transparent : brushColor);
                        }
                    }
                    texture.update(canvas);
                    isFigureBeingDrawn = isSave = false;
                }
                //   Овал
                else if (isDrawingOval && isFigureBeingDrawn && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                    // Нормализация координат
                    figuresStart.x /= img.factor, figuresStart.y /= img.factor, figuresEnd.x /= img.factor, figuresEnd.y /= img.factor;

                    int rectWidth = figuresEnd.x - figuresStart.x, rectHeight = figuresEnd.y - figuresStart.y;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
                        rectWidth = rectWidth < 0 && rectHeight > 0 || rectHeight < 0 && rectWidth > 0 ? -rectHeight : rectHeight;
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
                                    canvas.setPixel(sf::Vector2u(pixelX, pixelY), isErasing ? sf::Color::Transparent : brushColor);
                            }
                        }
                    }
                    texture.update(canvas);
                    isFigureBeingDrawn = isSave = false;
                }
                //   Звезда
                else if (isDrawingStar && isFigureBeingDrawn && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                    // Нормализация координат
                    figuresStart.x /= img.factor;
                    figuresStart.y /= img.factor;
                    figuresEnd.x /= img.factor;
                    figuresEnd.y /= img.factor;

                    int rectWidth = figuresEnd.x - figuresStart.x, rectHeight = figuresEnd.y - figuresStart.y;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
                        rectWidth = rectWidth < 0 && rectHeight > 0 || rectHeight < 0 && rectWidth > 0 ? -rectHeight : rectHeight;
                    if (rectWidth < 0)
                        figuresStart.x += rectWidth, rectWidth = -rectWidth;
                    if (rectHeight < 0)
                        figuresStart.y += rectHeight, rectHeight = -rectHeight;

                    int centerX = figuresStart.x + rectWidth / 2;
                    int centerY = figuresStart.y + rectHeight / 2;

                    float scaleX = rectWidth / 2.0f;
                    float scaleY = rectHeight / 2.0f;

                    // Количество вершин звезды
                    const int numPoints = 10; // 5 основных + 5 внутренних
                    sf::Vector2f points[numPoints];

                    // Вычисляем координаты вершин звезды
                    for (int i = 0; i < numPoints; ++i)
                    {
                        float angle = i * 2 * 3.14159265f / numPoints - 3.14159265f / 2; // Начинаем с верхней точки
                        float r = (i % 2 == 0) ? 1.0f : 0.4f;                            // Внешние и внутренние вершины
                        points[i] = {static_cast<float>(centerX + scaleX * r * cos(angle)), static_cast<float>(centerY + scaleY * r * sin(angle))};
                    }

                    // Рисуем звезду, заполняя её пикселями
                    for (int y = figuresStart.y; y < figuresStart.y + rectHeight; ++y)
                    {
                        for (int x = figuresStart.x; x < figuresStart.x + rectWidth; ++x)
                        {
                            int intersections = 0;
                            for (int i = 0; i < numPoints; ++i)
                            {
                                sf::Vector2f p1 = points[i];
                                sf::Vector2f p2 = points[(i + 1) % numPoints];

                                if ((p1.y > y) != (p2.y > y) &&
                                    x < (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y) + p1.x)
                                {
                                    intersections++;
                                }
                            }

                            if (intersections % 2 == 1) // Чётное — снаружи, нечётное — внутри
                            {
                                if (x >= 0 && x < img.width && y >= 0 && y < img.height)
                                    canvas.setPixel(sf::Vector2u(x, y), isErasing ? sf::Color::Transparent : brushColor);
                            }
                        }
                    }

                    texture.update(canvas);
                    isFigureBeingDrawn = isSave = false;
                }

                draggingR = draggingG = draggingB = draggingA = false;
                isPaint = true;
            }

            // Передвижения курсора мыши
            if (event->is<sf::Event::MouseMoved>())
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (draggingR)
                    img.rgba[0] = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingG)
                    img.rgba[1] = std::clamp(mousePos.x - sliderG.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingB)
                    img.rgba[2] = std::clamp(mousePos.x - sliderB.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingA)
                    img.rgba[3] = std::clamp(mousePos.x - sliderA.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
            }

            // Клавиатура
            else if (event->is<sf::Event::KeyPressed>())
            {
                // Проверка не зажата ли CTRL
                if (!keyEvent->control)
                { 
                    // Кисточка
                    if (keyEvent->scancode == sf::Keyboard::Scan::E)
                        isErasing = isDrawingOval = isDrawingRect = isDrawingStar = isPouring = false;
                    // Ластик
                    else if (keyEvent->scancode == sf::Keyboard::Scan::Q)
                        isErasing = true;
                    // Рисования прямоугольников
                    else if (keyEvent->scancode == sf::Keyboard::Scan::R)
                        isDrawingRect = true, isDrawingOval = isDrawingStar = isPouring = false;
                    // Рисования овалов
                    else if (keyEvent->scancode == sf::Keyboard::Scan::O)
                        isDrawingOval = true, isDrawingRect = isDrawingStar = isPouring = false;
                    // Рисования овалов
                    else if (keyEvent->scancode == sf::Keyboard::Scan::S)
                        isDrawingStar = true, isDrawingRect = isDrawingOval = isPouring = false;
                    // Заливка цветом
                    else if (keyEvent->scancode == sf::Keyboard::Scan::F)
                        isPouring = true, isErasing = false;
                    // Очистка
                    else if (keyEvent->scancode == sf::Keyboard::Scan::C)
                    {
                        canvas.resize(sf::Vector2u(img.width, img.height), sf::Color::White);
                        texture.update(canvas);
                        isSave = true;
                    }
                    // Увелечения и уменьшения размера кисти
                    else if (keyEvent->scancode == sf::Keyboard::Scan::Equal)
                        brushSize = std::min(brushSize + 2, 50);
                    else if (keyEvent->scancode == sf::Keyboard::Scan::Hyphen)
                        brushSize = std::max(brushSize - 2, 1);
                }

                // Сохранение
                else if (keyEvent->control && keyEvent->scancode == sf::Keyboard::Scan::S)
                {
                    // Проверка нажатия Shift
                    if (keyEvent->shift)
                    {
                        // Запрос нового названия файла
                        cout << "\033[1mEnter a new file name: \033[0m";
                        getline(std::cin, filepath);

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
                            cout << "\033[32mImage saved successfully.\033[0m" << endl;
                        else
                        {
                            cerr << "\033[1;31mFailed to save image.\033[0m" << endl;
                            return 1;
                        }
                    }
                    // Сохранение ZPIF
                    else if (img.format == "zpif")
                    {
                        if (saveImageZPIF(canvas, img, filepath, filepath_temp) < 0)
                            return 1;
                    }
                    isSave = true;
                }
            }
        }

        // Обновление цвета кисти
        brushColor = sf::Color(img.rgba[0], img.rgba[1], img.rgba[2], img.rgba[3]);
        colorPreview.setFillColor(brushColor);

        // Рисование мышью
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isPaint && !isPouring && !isPipette && !isDrawingOval && !isDrawingRect && !isDrawingStar && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Нормализация координат мыши
            mousePos.x /= img.factor;
            mousePos.y /= img.factor;

            int x = mousePos.x;
            int y = mousePos.y;

            if (x >= 0 && x < img.width && y >= 0 && y < img.height)
            {
                sf::Color colorToUse = isErasing ? sf::Color::Transparent : brushColor;

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

                                    if (pixelX >= 0 && pixelX < img.width && pixelY >= 0 && pixelY < img.height)
                                    {
                                        canvas.setPixel(sf::Vector2u(pixelX, pixelY), colorToUse);
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

                                if (pixelX >= 0 && pixelX < img.width && pixelY >= 0 && pixelY < img.height)
                                {
                                    canvas.setPixel(sf::Vector2u(pixelX, pixelY), colorToUse);
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
            int rectWidth((mousePos.x / img.factor > img.width ? (mousePos.x < 0 ? 0 : img.width * img.factor) : mousePos.x) - figuresStart.x); // Проверка на то что позиция курс. в пределах изображения (и на то что поз. больше 0)
            int rectHeight(mousePos.y - figuresStart.y);

            // Квадрат если зажата Shift
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
                rectWidth = rectWidth < 0 && rectHeight > 0 || rectHeight < 0 && rectWidth > 0 ? -rectHeight : rectHeight;

            // Отрисовка превью фигуры
            previewFigures.setPosition(figuresStart);
            previewFigures.setSize(sf::Vector2f(rectWidth, rectHeight));
        }

        // Очистка окна
        window.clear(sf::Color::White);

        // Отрисовка фонов
        window.draw(backgroundSprite);
        window.draw(backgroundRight);

        // Отрисовка элементов
        window.draw(sprite);
        //   Предпросмотр фигур
        if (isFigureBeingDrawn && figuresStart.x / img.factor < img.width && !isPipette)
            window.draw(previewFigures);
        //   Интерфеис
        window.draw(indent);
        window.draw(colorPreview);

        // Создание слайдеров
        drawSlider(window, sliderR, img.rgba[0], sf::Color::Red, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + 10));
        drawSlider(window, sliderG, img.rgba[1], sf::Color::Green, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR + 10 * 2));
        drawSlider(window, sliderB, img.rgba[2], sf::Color::Blue, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 2 + 10 * 3));
        drawSlider(window, sliderA, img.rgba[3], sf::Color::White, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 3 + 10 * 4));

        // Создание кнопок
        //   1
        drawButton(window, buttonBrushTexture, buttonBrush, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X, Y_SLIDER * 7));
        drawButton(window, buttonEraserTexture, buttonEraser, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 7));
        drawButton(window, buttonPipetteTexture, buttonPipette, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 7));
        drawButton(window, buttonPlusTexture, buttonPlus, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 3, Y_SLIDER * 7));
        drawButton(window, buttonMinusTexture, buttonMinus, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 4, Y_SLIDER * 7));
        //   2
        drawButton(window, buttonDrawRectTexture, buttonDrawRect, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X, Y_SLIDER * 8 + 40));
        drawButton(window, buttonDrawOvalTexture, buttonDrawOval, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 8 + 40));
        drawButton(window, buttonDrawStarTexture, buttonDrawStar, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 8 + 40));
        drawButton(window, buttonPouringTexture, buttonPouring, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 3, Y_SLIDER * 8 + 40));

        // Логика обновления FPS
        float deltaTime = clock.restart().asSeconds();
        frameCount++;
        elapsedTime += deltaTime;

        // Вывод FPS каждую секунду
        if (elapsedTime >= 1.0f)
        {
            uint16_t fps = frameCount / elapsedTime;

            window.setTitle("ZeR Graphics Editor " + std::to_string(fps) + " FPS");

            frameCount = 0;
            elapsedTime = 0.f;
        }

        // Отрисовка окна
        window.display();
    }

    return 0;
}
