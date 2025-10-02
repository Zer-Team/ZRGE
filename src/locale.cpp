/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#include "../include/locale.hpp"
#include <algorithm>
#include <clocale>

// Английский
Locale en_locale = {
    .zrge = "ZeR Graphics Editor",
    .error_open_file = "Error opening file.",
    .error_load_img = "Error: Failed to load image!",
    .error_size = "ERROR SIZE: ",
    .error_size_factor_canvas = "Error: Image zoom factor cannot be less than or equal to zero.",
    .texture = "Texture :",
    .texture_bg = "Background texture: ",
    .image = "Image: ",
    .canvas = "Canvas: ",
    .save_img = "Save image?",
    .exit = "Exit",
    .new_name_file = "Enter a new file name",
    .img_save_ok = "Image saved successfully.",
    .img_save_no = "Failed to save image.",
    .error_resize_texture = "Error: Failed to resize texture!",
    .error_no_supp_file = "Error: The file format is not supported or the file is damaged. CHECK THE FILE!",
    .warning_big_img = "Warning: The number of pixels exceeds the possible.",
    .welcome = "Welcome to",
    .version = "Version",
    .help = "\033[1;33mUsage: zrge [file path]\033[0m\n\
Available keys in the program:\n\
\033[1m Q\033[0m: Eraser\n\
\033[1m E\033[0m: Brush\n\
\033[1m +\033[0m: Increase brush size\n\
\033[1m -\033[0m: Decrease brush size\n\
\033[1m R\033[0m: Drawing rectangles\n\
\033[1m O\033[0m: Drawing ovals\n\
\033[1m S\033[0m: Drawing stars\n\
\033[1m F\033[0m: Fill with color\n\
\033[1m C\033[0m: Clear canvas\n\
\033[1m CTRL\033[0m + \033[1mS\033[0m: Save image\
\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m: Save the image with a new name",
    .enter_file_path = "Enter the path to the file",
    .enter_canvas_factor = "Enter canvas factor: ",
    .create_file = "File does not exist create",
    .enter_img_w = "Enter image width: ",
    .enter_img_h = "Enter image height: ",
    .error_path_len = "File name/path is too short",
    .error_temp_file_create = "Error opening/create temp file.",
    .write_in_temp_file = "Writing data to a temporary file...",
    .write_data = "Create, record and compress image...",
    .remove_temp_file = "Remove temp file...",
};
// Русский
Locale ru_locale = {
    .zrge = "Графический редактор ZeR",
    .error_open_file = "Ошибка открытия файла.",
    .error_load_img = "Ошибка: Не удалось загрузить изображение!",
    .error_size = "ОШИБКА РАЗМЕРОВ:",
    .error_size_factor_canvas = "Ошибка: коэффициент увеличения изображения не может быть меньше или равен нулю.",
    .texture = "Текстура: ",
    .texture_bg = "Текстура фона: ",
    .image = "Изображение: ",
    .canvas = "Холст: ",
    .save_img = "Сохранить изображение?",
    .exit = "Выход",
    .new_name_file = "Введите новое имя файла",
    .img_save_ok = "Изображение успешно сохранено.",
    .img_save_no = "Не удалось сохранить изображение.",
    .error_resize_texture = "Ошибка: Не удалось изменить размер текстуры холста!",
    .error_no_supp_file = "Ошибка: Формат файла не поддерживается или файл повреждён. ПРОВЕРЬТЕ ФАЙЛ!",
    .warning_big_img = "Предупреждение: Количество пикселей превышает размер изображения.",
    .welcome = "Добро пожаловать в",
    .version = "Версия",
    .help = "\033[1;33mИспользование: zrge [путь к файлу]\033[0m\n\
Горячие клавиши:\n\
\033[1m Q\033[0m: Ластик\n\
\033[1m E\033[0m: Кисть\n\
\033[1m +\033[0m: Увеличение размера кисти\n\
\033[1m -\033[0m: Уменьшение размера кисти\n\
\033[1m R\033[0m: Рисование прямоугольника\n\
\033[1m O\033[0m: Рисование овала\n\
\033[1m S\033[0m: Рисование звезды\n\
\033[1m F\033[0m: Заливка цветом\n\
\033[1m C\033[0m: Очистка холста\n\
\033[1m CTRL\033[0m + \033[1mS\033[0m: Сохранить изображение\n\
\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m: Сохранить изображение с новым именем",
    .enter_file_path = "Введите путь к файлу",
    .enter_canvas_factor = "Введите степень увеличения холста: ",
    .create_file = "Файл не существует, создать?",
    .enter_img_w = "Введите ширину изображения: ",
    .enter_img_h = "Введите высоту изображения: ",
    .error_path_len = "Имя или путь файла слишком короткое",
    .error_temp_file_create = "Ошибка открытия или создания временного файла",
    .write_in_temp_file = "Запись данных во временный файл...",
    .write_data = "Создание, запись и сжатие изображения...",
    .remove_temp_file = "Удаление временного файла...",
};
// Французский
Locale fr_locale = {
    .zrge = "Éditeur Graphique ZeR",
    .error_open_file = "Erreur lors de l'ouverture du fichier.",
    .error_load_img = "Erreur : Impossible de charger l'image !",
    .error_size = "ERREUR DE TAILLE : ",
    .error_size_factor_canvas = "Erreur : le facteur de zoom de l’image ne peut pas être inférieur ou égal à zéro.",
    .texture = "Texture : ",
    .texture_bg = "Texture de fond : ",
    .image = "Image : ",
    .canvas = "Toile : ",
    .save_img = "Enregistrer l'image ?",
    .exit = "Quitter",
    .new_name_file = "Entrez un nouveau nom de fichier",
    .img_save_ok = "Image enregistrée avec succès.",
    .img_save_no = "Impossible d'enregistrer l'image.",
    .error_resize_texture = "Erreur : Impossible de redimensionner la texture !",
    .error_no_supp_file = "Erreur : Le format du fichier n'est pas pris en charge ou le fichier est endommagé. VÉRIFIEZ LE FICHIER !",
    .warning_big_img = "Attention : Le nombre de pixels dépasse la taille possible.",
    .welcome = "Bienvenue dans",
    .version = "Version",
    .help = "\033[1;33mUtilisation : zrge [chemin du fichier]\033[0m\n\
Touches disponibles :\n\
\033[1m Q\033[0m : Gomme\n\
\033[1m E\033[0m : Pinceau\n\
\033[1m +\033[0m : Augmenter la taille du pinceau\n\
\033[1m -\033[0m : Diminuer la taille du pinceau\n\
\033[1m R\033[0m : Dessiner un rectangle\n\
\033[1m O\033[0m : Dessiner un ovale\n\
\033[1m S\033[0m : Dessiner une étoile\n\
\033[1m F\033[0m : Remplir avec la couleur\n\
\033[1m C\033[0m : Effacer la toile\n\
\033[1m CTRL\033[0m + \033[1mS\033[0m : Enregistrer l'image\
\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m : Enregistrer l'image avec un nouveau nom",
    .enter_file_path = "Entrez le chemin du fichier",
    .enter_canvas_factor = "Entrez le facteur de la toile : ",
    .create_file = "Le fichier n'existe pas, le créer",
    .enter_img_w = "Entrez la largeur de l'image : ",
    .enter_img_h = "Entrez la hauteur de l'image : ",
    .error_path_len = "Nom ou chemin du fichier trop court",
    .error_temp_file_create = "Erreur lors de l'ouverture ou de la création du fichier temporaire",
    .write_in_temp_file = "Écriture des données dans le fichier temporaire...",
    .write_data = "Création, enregistrement et compression de l'image...",
    .remove_temp_file = "Suppression du fichier temporaire...",
};
// Немецкий
Locale de_locale = {
    .zrge = "ZeR Grafik-Editor",
    .error_open_file = "Fehler beim Öffnen der Datei.",
    .error_load_img = "Fehler: Bild konnte nicht geladen werden!",
    .error_size = "FEHLERGRÖSSE: ",
    .error_size_factor_canvas = "Fehler: Der Bildzoomfaktor kann nicht kleiner oder gleich Null sein.",
    .texture = "Textur: ",
    .texture_bg = "Hintergrundtextur: ",
    .image = "Bild: ",
    .canvas = "Leinwand: ",
    .save_img = "Bild speichern?",
    .exit = "Beenden",
    .new_name_file = "Neuen Dateinamen eingeben",
    .img_save_ok = "Bild erfolgreich gespeichert.",
    .img_save_no = "Bild konnte nicht gespeichert werden.",
    .error_resize_texture = "Fehler: Textur konnte nicht skaliert werden!",
    .error_no_supp_file = "Fehler: Dateiformat wird nicht unterstützt oder Datei beschädigt. ÜBERPRÜFEN SIE DIE DATEI!",
    .warning_big_img = "Warnung: Die Anzahl der Pixel überschreitet die mögliche Größe.",
    .welcome = "Willkommen bei",
    .version = "Version",
    .help = "\033[1;33mVerwendung: zrge [Dateipfad]\033[0m\n\
Verfügbare Tasten:\n\
\033[1m Q\033[0m: Radiergummi\n\
\033[1m E\033[0m: Pinsel\n\
\033[1m +\033[0m: Pinselgröße vergrößern\n\
\033[1m -\033[0m: Pinselgröße verkleinern\n\
\033[1m R\033[0m: Rechteck zeichnen\n\
\033[1m O\033[0m: Oval zeichnen\n\
\033[1m S\033[0m: Stern zeichnen\n\
\033[1m F\033[0m: Mit Farbe füllen\n\
\033[1m C\033[0m: Leinwand löschen\n\
\033[1m CTRL\033[0m + \033[1mS\033[0m: Bild speichern\
\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m: Bild unter neuem Namen speichern",
    .enter_file_path = "Geben Sie den Dateipfad ein",
    .enter_canvas_factor = "Leinwandfaktor eingeben: ",
    .create_file = "Datei existiert nicht, erstellen",
    .enter_img_w = "Bildbreite eingeben: ",
    .enter_img_h = "Bildhöhe eingeben: ",
    .error_path_len = "Dateiname/Dateipfad zu kurz",
    .error_temp_file_create = "Fehler beim Öffnen/Erstellen der temporären Datei",
    .write_in_temp_file = "Schreibe Daten in temporäre Datei...",
    .write_data = "Erstelle, schreibe und komprimiere Bild...",
    .remove_temp_file = "Temporäre Datei löschen...",
};
// Испанский
Locale es_locale = {
    .zrge = "Editor Gráfico ZeR",
    .error_open_file = "Error al abrir el archivo.",
    .error_load_img = "Error: ¡No se pudo cargar la imagen!",
    .error_size = "ERROR DE TAMAÑO: ",
    .error_size_factor_canvas = "Error: El factor de zoom de la imagen no puede ser menor o igual a cero.",
    .texture = "Textura: ",
    .texture_bg = "Textura de fondo: ",
    .image = "Imagen: ",
    .canvas = "Lienzo: ",
    .save_img = "¿Guardar imagen?",
    .exit = "Salir",
    .new_name_file = "Ingrese un nuevo nombre de archivo",
    .img_save_ok = "Imagen guardada con éxito.",
    .img_save_no = "No se pudo guardar la imagen.",
    .error_resize_texture = "Error: ¡No se pudo cambiar el tamaño de la textura!",
    .error_no_supp_file = "Error: Formato de archivo no soportado o archivo dañado. ¡VERIFIQUE EL ARCHIVO!",
    .warning_big_img = "Advertencia: El número de píxeles excede el tamaño permitido.",
    .welcome = "Bienvenido a",
    .version = "Versión",
    .help = "\033[1;33mUso: zrge [ruta del archivo]\033[0m\n\
Teclas disponibles:\n\
\033[1m Q\033[0m: Borrador\n\
\033[1m E\033[0m: Pincel\n\
\033[1m +\033[0m: Aumentar tamaño del pincel\n\
\033[1m -\033[0m: Disminuir tamaño del pincel\n\
\033[1m R\033[0m: Dibujar rectángulo\n\
\033[1m O\033[0m: Dibujar óvalo\n\
\033[1m S\033[0m: Dibujar estrella\n\
\033[1m F\033[0m: Rellenar con color\n\
\033[1m C\033[0m: Limpiar lienzo\n\
\033[1m CTRL\033[0m + \033[1mS\033[0m: Guardar imagen\
\033[1m CTRL\033[0m + \033[1mSHIFT\033[0m + \033[1mS\033[0m: Guardar imagen con nuevo nombre",
    .enter_file_path = "Ingrese la ruta del archivo",
    .enter_canvas_factor = "Ingrese el factor del lienzo: ",
    .create_file = "El archivo no existe, ¿crear",
    .enter_img_w = "Ingrese el ancho de la imagen: ",
    .enter_img_h = "Ingrese la altura de la imagen: ",
    .error_path_len = "Nombre o ruta del archivo demasiado corto",
    .error_temp_file_create = "Error al abrir o crear el archivo temporal",
    .write_in_temp_file = "Escribiendo datos en archivo temporal...",
    .write_data = "Creando, escribiendo y comprimiendo la imagen...",
    .remove_temp_file = "Eliminando archivo temporal...",
};

Locale *locale = &en_locale; // Текущая локаль

// Установка языка
void set_lang(string l)
{
    // Устанавливаем локаль из переменных окружения
    std::setlocale(LC_ALL, "");
    string locale_use; // Локаль
    
    if (l == "auto")
        locale_use = std::setlocale(LC_ALL, nullptr);
    else
        locale_use = l;

    // Приводим к нижнему регистру
    std::transform(locale_use.begin(), locale_use.end(), locale_use.begin(), 
                   [](unsigned char c){ return std::tolower(c); });

    // Получаем текущую локаль
    if (locale_use.rfind("ru", 0) == 0)
        locale = &ru_locale;
    else if (locale_use.rfind("fr", 0) == 0)
        locale = &fr_locale;
    else if (locale_use.rfind("de", 0) == 0)
        locale = &de_locale;
    else if (locale_use.rfind("es", 0) == 0)
        locale = &es_locale;
    else
        locale = &en_locale;
}
