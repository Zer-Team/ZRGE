#!/bin/sh

cd '/home/admin/Рабочий стол/ZRGE2/'

readonly path='/home/admin/test.zpif'

clear

g++ -c src/compress.cpp -o build/bin/compress.o
echo "1/7"
g++ -c src/main.cpp -o build/bin/main.o
echo "2/7"
g++ -c src/load_image.cpp -o build/bin/load_image.o
echo "3/7"
g++ -c src/parser.cpp -o build/bin/parser.o
echo "4/7"
g++ -c src/render.cpp -o build/bin/render.o
echo "5/7"
g++ -c src/save_image.cpp -o build/bin/save_image.o
echo "6/7"
g++ -c src/draw.cpp -o build/bin/draw.o
echo "7/7"

# Финальная сборка
g++ -g build/bin/load_image.o build/bin/draw.o build/bin/render.o build/bin/parser.o build/bin/compress.o build/bin/save_image.o build/bin/main.o -o build/bin/zrge -lsfml-graphics -lsfml-window -lsfml-system

# Проверка наличия файла main
if [ -f ./build/bin/zrge ]; then
    ./build/bin/zrge
fi