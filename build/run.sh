#!/bin/sh

# Очистка
clear

# Переход в нужную директорию
cd /home/admin/Desktop/ZRGE\ 2/

# Проверка есть ли bin
if [ -f "build/bin" ]; 
then
    rm -r build/bin
    
    mkdir build/bin
fi

# Компиляция
#   Сжаатие
g++ -c src/compress.cpp -o build/bin/compress.o
echo "1/7"
#   Главный (стартовый)
g++ -c src/main.cpp -o build/bin/main.o
echo "2/7"
#   Загрузка изображенияф
g++ -c src/load_image.cpp -o build/bin/load_image.o
echo "3/7"
#   Парсер парамотров
g++ -c src/parser.cpp -o build/bin/parser.o
echo "4/7"
#   Графика
g++ -c src/graphics.cpp -o build/bin/graphics.o
echo "5/7"
#   Сахронения изображения
g++ -c src/save_image.cpp -o build/bin/save_image.o
echo "6/7"
#   Отрисовка
g++ -c src/draw.cpp -o build/bin/draw.o
echo "7/7"

# Финальная сборка
g++ -g build/bin/load_image.o build/bin/draw.o build/bin/graphics.o build/bin/parser.o build/bin/compress.o build/bin/save_image.o build/bin/main.o -o build/bin/zrge -lsfml-graphics -lsfml-window -lsfml-system

# Проверка наличия файла main
if [ -f ./build/bin/zrge ]; then
    ./build/bin/zrge --path=test.zpif
fi