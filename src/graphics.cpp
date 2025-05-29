#include "../include/graphics.hpp"

// Рисования звезды
void drawStar(sf::Image &canvas, sf::Texture &texture, const Image &img, sf::Vector2f &figuresStart, sf::Vector2f &figuresEnd, sf::Color &color)
{
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
                    canvas.setPixel(sf::Vector2u(x, y), color);
            }
        }
    }

    texture.update(canvas);
}

// Рисования овала
void drawOval(sf::Image &canvas, sf::Texture &texture, const Image &img, sf::Vector2f &figuresStart, sf::Vector2f &figuresEnd, sf::Color &color)
{
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
                    canvas.setPixel(sf::Vector2u(pixelX, pixelY), color);
            }
        }
    }
    texture.update(canvas);
}

// Рисования квадрата
void drawRect(sf::Image &canvas, sf::Texture &texture, Image &img, sf::Vector2f &figuresStart, sf::Vector2f &figuresEnd, sf::Color &color)
{
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
                canvas.setPixel(sf::Vector2u(x, y), color);
        }
    }
    texture.update(canvas);
}

// Функция для заливки цветом
void fillColor(const int &x, const int &y, const Image &img, sf::Image &canvas, sf::Texture &texture, const sf::Color newColor)
{
    sf::Color targetColor = canvas.getPixel(sf::Vector2u(x, y));
    if (targetColor == newColor)
        return; // Если цвет совпадает, ничего не делаем

    std::stack<sf::Vector2i> stack;
    stack.push(sf::Vector2i(x, y));

    while (!stack.empty())
    {
        sf::Vector2i pos = stack.top();
        stack.pop();

        // Проверка границ и совпадение цвета
        if (pos.x < 0 || pos.x >= img.width || pos.y < 0 || pos.y >= img.height ||
            canvas.getPixel(static_cast<sf::Vector2u>(pos)) != targetColor)
        {
            continue;
        }

        // Новый цвет
        canvas.setPixel(static_cast<sf::Vector2u>(pos), newColor);

        // Добавление соседнего пиксели в стек
        if (pos.x + 1 < img.width)
            stack.push({pos.x + 1, pos.y});
        if (pos.x - 1 >= 0)
            stack.push({pos.x - 1, pos.y});
        if (pos.y + 1 < img.height)
            stack.push({pos.x, pos.y + 1});
        if (pos.y - 1 >= 0)
            stack.push({pos.x, pos.y - 1});
    }

    texture.update(canvas);
}

// Главная функция всей графики
int render(Image &img, sf::Image &canvas, sf::Texture &texture, std::string &filepath, std::string &filepath_temp)
{
    // Получаем размеры экрана:
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Создания окна
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(img.width * img.factor + WIDTH_COLOR_SLIDER + INDENT_X * 2, img.height * img.factor > Y_SLIDER * 12 ? img.height * img.factor : Y_SLIDER * 12)), "ZeR Graphics Editor", sf::Style::Close);

    // Установка позиции окна
    window.setPosition(sf::Vector2i(((desktop.size.x - window.getSize().x) / 2), ((desktop.size.y - window.getSize().y) / 2)));

    // Загрузка курсоров
    sf::Cursor arrowCursor(sf::Cursor::Type::Arrow), handCursor(sf::Cursor::Type::Hand), crossCursor(sf::Cursor::Type::Cross);

    // Создание индикатора
    sf::Sprite sprite(texture);

    // Загрузка иконки и фона
    sf::Image icon;
    sf::Texture backgroundTexture;
    if (!icon.loadFromFile("/usr/share/zrge/images/icon.png") || !backgroundTexture.loadFromFile("/usr/share/zrge/images/textures/background.jpg"))
    {
        std::cerr << "\033[1;31mError: Failed to load image!\033[0m" << std::endl;
        return 1;
    }

    // Установка иконки окна
    window.setIcon(icon);

    // Фон холста
    sf::Sprite backgroundSprite{backgroundTexture};

    // Проверка размеров
    if (backgroundTexture.getSize().x == 0 || backgroundTexture.getSize().y == 0 || texture.getSize().y == 0 || texture.getSize().x == 0 || img.factor <= 0)
    {
        cerr << "\x1b[1;31mERROR SIZE\n"
             << "Factor: " << img.factor << '\n'
             << "Texture: " << texture.getSize().x << "x" << texture.getSize().y << '\n'
             << "BG texture: " << backgroundTexture.getSize().x << "x" << backgroundTexture.getSize().y << '\n'
             << "Image: " << img.width << "x" << img.height << '\n'
             << "Canvas: " << canvas.getSize().x << "x" << canvas.getSize().y << "\x1b[0m" << endl;
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
    sf::Color color = sf::Color::Black; // Цвет кисти
    // Режим рисования
    ModeDraw drawing{ModeDraw::BRUSH};
    //   Для проверок
    bool isSave = true,             // Сохранено (true)
        isFigureBeingDrawn = false, // Предпросмотр фигур (false)
        isPipette = false,          // Пипетка (false)
        isPaint = true;             // Рисование (true)
    //   Размер кисти
    uint16_t brushSize = 5;
    //   Дорисовывания
    sf::Vector2i prevMousePos(-1, -1); // Последняя позиция мыши
    //   Предпросмотр фигур
    sf::Vector2f figuresStart(-1, -1), figuresEnd(-1, -1); // Первая и последняя позиции курсора для рисования фигур
    //      Объект для предпросмотра рисования фигур
    sf::RectangleShape previewFigures;
    //      Параметры объекта
    previewFigures.setFillColor(sf::Color::Transparent); // Цвет фона
    previewFigures.setOutlineThickness(2);               // Размер контура
    previewFigures.setOutlineColor(sf::Color::Green);    // Цвет контура
    //   Какой цвет настраивается
    bool draggingR{false}, draggingG{false}, draggingB{false}, draggingA{false};

    // Элементы интерфейса
    //   Текстура для кнопок
    sf::Texture buttonAtlas;
    if (!buttonAtlas.loadFromFile("/usr/share/zrge/images/textures/buttons.png"))
    {
        std::cerr << "\033[1;31mError: Failed to load button atlas!\033[0m" << std::endl;
        return -1;
    }

    //   Кнопки
    sf::Sprite buttonBrush(buttonAtlas), buttonEraser(buttonAtlas), buttonPipette(buttonAtlas),
        buttonPlus(buttonAtlas), buttonMinus(buttonAtlas), buttonDrawRect(buttonAtlas),
        buttonDrawOval(buttonAtlas), buttonDrawStar(buttonAtlas), buttonPouring(buttonAtlas);

    //   Слайдеры
    sf::RectangleShape sliderR, sliderG, sliderB, sliderA,
        colorPreview, // Предпросмотр цвета
        indent;       // Разделитель
    sliderR.setFillColor(sf::Color::Red);
    sliderG.setFillColor(sf::Color::Green);
    sliderB.setFillColor(sf::Color::Blue);
    //    Параметры разделителя
    indent.setFillColor(sf::Color(60, 60, 60));
    indent.setSize(sf::Vector2f(INDENT_X / 2, window.getSize().y));
    indent.setPosition(sf::Vector2f(img.width * img.factor, 0));
    //   Показатель цвета
    colorPreview.setSize(sf::Vector2f(WIDTH_COLOR_SLIDER + INDENT_X * 1.5, HEIGHT_COLOR_PREVIEW));
    colorPreview.setPosition(sf::Vector2f(img.width * img.factor + INDENT_X / 2, 0));
    colorPreview.setFillColor(color);
    //   Фон для инструментов
    sf::RectangleShape backgroundRight(sf::Vector2f(WIDTH_COLOR_SLIDER + INDENT_X * 1.5, window.getSize().y - HEIGHT_COLOR_PREVIEW));
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

            std::vector<const sf::Sprite *> buttons = {
                &buttonBrush, &buttonDrawOval, &buttonDrawRect, &buttonDrawStar,
                &buttonEraser, &buttonMinus, &buttonPipette, &buttonPlus, &buttonPouring};

            std::vector<const sf::RectangleShape *> sliders = {
                &sliderA, &sliderB, &sliderG, &sliderR};

            bool isHandCursor = false; // Наведён ли курсор на слайдер или кнопку

            for (const auto *button : buttons)
            {
                if (button->getGlobalBounds().contains(mousePos))
                {
                    isHandCursor = true;
                    break;
                }
            }

            if (!isHandCursor)
            {
                for (const auto *slider : sliders)
                {
                    if (slider->getGlobalBounds().contains(mousePos))
                    {
                        isHandCursor = true;
                        break;
                    }
                }
            }

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
                        // Сохранение PNG и JPG
                        if (img.format == ImageFormat::PNG || img.format == ImageFormat::JPEG)
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
                        else if (img.format == ImageFormat::ZPIF)
                        {
                            if (saveImageZPIF(canvas, img, filepath, filepath_temp) < 0)
                                return 1;
                        }
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
                    drawing = ModeDraw::BRUSH;
                //   Ластик
                else if (buttonEraser.getGlobalBounds().contains(mousePos))
                    drawing = ModeDraw::ERASING;
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
                    drawing = ModeDraw::POURING;
                //   Рисования прямоугольников
                else if (buttonDrawRect.getGlobalBounds().contains(mousePos))
                    drawing = ModeDraw::DRAWING_RECT;
                //   Рисования овалов
                else if (buttonDrawOval.getGlobalBounds().contains(mousePos))
                    drawing = ModeDraw::DRAWING_OVAL;
                //   Рисования звезды
                else if (buttonDrawStar.getGlobalBounds().contains(mousePos))
                    drawing = ModeDraw::DRAWING_STAR;
                //   Заливка цветом
                else if (drawing == ModeDraw::POURING && !isPipette && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) && mousePos.x >= 0 && mousePos.x / img.factor < img.width && mousePos.y >= 0 && mousePos.y / img.factor < img.height)
                    fillColor(mousePos.x / img.factor, mousePos.y / img.factor, img, canvas, texture, color), isSave = false;
                // Слайдеры
                else if (sliderR.getGlobalBounds().contains(mousePos))
                {
                    color.r = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingR = true;
                    isPaint = false;
                }
                else if (sliderG.getGlobalBounds().contains(mousePos))
                {
                    color.g = std::clamp(mousePos.x - sliderG.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingG = true;
                    isPaint = false;
                }
                else if (sliderB.getGlobalBounds().contains(mousePos))
                {
                    color.b = std::clamp(mousePos.x - sliderB.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingB = true;
                    isPaint = false;
                }
                else if (sliderA.getGlobalBounds().contains(mousePos))
                {
                    color.a = std::clamp(mousePos.x - sliderA.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                    draggingA = true;
                    isPaint = false;
                }
                // Рисование фигур
                if ((drawing == ModeDraw::DRAWING_RECT || drawing == ModeDraw::DRAWING_OVAL || drawing == ModeDraw::DRAWING_STAR) &&
                    mouseEvent->button == sf::Mouse::Button::Left && !isPipette)
                {
                    figuresStart = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    isFigureBeingDrawn = true;
                }
                // Пипетка CTRL
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && (mousePos.x / img.factor <= img.width && mousePos.y / img.factor <= img.height))
                {
                    color = canvas.getPixel(sf::Vector2u(mousePos.x / img.factor, mousePos.y / img.factor));
                    colorPreview.setFillColor(color);
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
                    color = canvas.getPixel(sf::Vector2u(mousePos.x / img.factor, mousePos.y / img.factor));
                    colorPreview.setFillColor(color);
                    isPipette = false;
                }
                // Рисование фигур
                //   Прямоугольник
                else if (drawing == ModeDraw::DRAWING_RECT && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    drawRect(canvas, texture, img, figuresStart, figuresEnd, color);
                    isFigureBeingDrawn = isSave = false;
                }
                //   Овал
                else if (drawing == ModeDraw::DRAWING_OVAL && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    drawOval(canvas, texture, img, figuresStart, figuresEnd, color);
                    isFigureBeingDrawn = isSave = false;
                }
                //   Звезда
                else if (drawing == ModeDraw::DRAWING_STAR && mouseEvent->button == sf::Mouse::Button::Left && figuresStart.x / img.factor < img.width)
                {
                    figuresEnd = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    drawStar(canvas, texture, img, figuresStart, figuresEnd, color);
                    isFigureBeingDrawn = isSave = false;
                }

                draggingR = draggingG = draggingB = draggingA = false;
                isPaint = true;
            }

            // Передвижения курсора мыши
            if (event->is<sf::Event::MouseMoved>())
            {
                if (draggingR)
                    color.r = std::clamp(mousePos.x - sliderR.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingG)
                    color.g = std::clamp(mousePos.x - sliderG.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingB)
                    color.b = std::clamp(mousePos.x - sliderB.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);
                else if (draggingA)
                    color.a = std::clamp(mousePos.x - sliderA.getPosition().x, 0.0f, (float)WIDTH_COLOR_SLIDER);

                // Изменения курсора
                if (isHandCursor)
                    window.setMouseCursor(handCursor);
                else if (sprite.getGlobalBounds().contains(mousePos))
                    window.setMouseCursor(crossCursor);
                else
                    window.setMouseCursor(arrowCursor);
            }

            // Клавиатура
            else if (event->is<sf::Event::KeyPressed>())
            {
                // Проверка не зажата ли CTRL
                if (!keyEvent->control)
                {
                    // Кисточка
                    if (keyEvent->scancode == sf::Keyboard::Scan::E)
                        drawing = ModeDraw::BRUSH;
                    // Ластик
                    else if (keyEvent->scancode == sf::Keyboard::Scan::Q)
                        drawing = ModeDraw::ERASING;
                    // Рисования прямоугольников
                    else if (keyEvent->scancode == sf::Keyboard::Scan::R)
                        drawing = ModeDraw::DRAWING_RECT;
                    // Рисования овалов
                    else if (keyEvent->scancode == sf::Keyboard::Scan::O)
                        drawing = ModeDraw::DRAWING_OVAL;
                    // Рисования звезды
                    else if (keyEvent->scancode == sf::Keyboard::Scan::S)
                        drawing = ModeDraw::DRAWING_STAR;
                    // Заливка цветом
                    else if (keyEvent->scancode == sf::Keyboard::Scan::F)
                        drawing = ModeDraw::POURING;
                    // Очистка
                    else if (keyEvent->scancode == sf::Keyboard::Scan::C)
                    {
                        canvas.resize(sf::Vector2u(img.width, img.height), sf::Color::White);
                        texture.update(canvas);
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
                            img.format = ImageFormat::JPEG;
                        else if (filepath.substr(filepath.length() - 4) == ".jpg" || filepath.substr(filepath.length() - 5) == ".jpeg")
                            img.format = ImageFormat::PNG;
                        else if (filepath.substr(filepath.length() - 5) == ".zpif")
                            img.format = ImageFormat::ZPIF;
                    }

                    // Сохранение PNG и JPG
                    if (img.format == ImageFormat::PNG || img.format == ImageFormat::JPEG)
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
                    else if (img.format == ImageFormat::ZPIF)
                    {
                        if (saveImageZPIF(canvas, img, filepath, filepath_temp) < 0)
                            return 1;
                    }
                    isSave = true;
                }
            }
        }

        colorPreview.setFillColor(color);

        // Рисование мышью
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isPaint && !isPipette && (drawing == ModeDraw::BRUSH || drawing == ModeDraw::ERASING) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Нормализация координат мыши
            mousePos.x /= img.factor;
            mousePos.y /= img.factor;

            if (mousePos.x >= 0 && mousePos.x < img.width && mousePos.y >= 0 && mousePos.y < img.height)
            {
                sf::Color colorToUse = drawing == ModeDraw::ERASING ? sf::Color::Transparent : color;

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
                                int pixelX = mousePos.x + i;
                                int pixelY = mousePos.y + j;

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
        drawSlider(window, sliderR, color.r, sf::Color::Red, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + 10));
        drawSlider(window, sliderG, color.g, sf::Color::Green, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR + 10 * 2));
        drawSlider(window, sliderB, color.b, sf::Color::Blue, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 2 + 10 * 3));
        drawSlider(window, sliderA, color.a, sf::Color::White, sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X / 4, HEIGHT_COLOR_PREVIEW + HEIGHT_COLOR_INDICATOR * 3 + 10 * 4));

        // Создание кнопок
        //   1
        drawButton(window, buttonBrush, sf::IntRect({0, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X, Y_SLIDER * 7));
        drawButton(window, buttonEraser, sf::IntRect({40, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 7));
        drawButton(window, buttonPipette, sf::IntRect({80, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 7));
        drawButton(window, buttonPlus, sf::IntRect({120, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 3, Y_SLIDER * 7));
        drawButton(window, buttonMinus, sf::IntRect({160, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 4, Y_SLIDER * 7));
        //   2
        drawButton(window, buttonDrawRect, sf::IntRect({200, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X, Y_SLIDER * 8 + 40));
        drawButton(window, buttonDrawOval, sf::IntRect({240, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + 26 + 20, Y_SLIDER * 8 + 40));
        drawButton(window, buttonDrawStar, sf::IntRect({280, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 2, Y_SLIDER * 8 + 40));
        drawButton(window, buttonPouring, sf::IntRect({320, 0}, {40, 40}), sf::Vector2f((img.width * img.factor) + INDENT_X + INDENT_X + (26 + 20) * 3, Y_SLIDER * 8 + 40));

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
