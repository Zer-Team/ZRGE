/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
*/

#pragma once

#include <string>
#include <algorithm>
#include <clocale>

using std::string;

struct Locale {
    string zrge,
           error_open_file,
           error_load_img,
           error_size,
           error_size_factor_canvas,
           texture,
           texture_bg,
           image,
           canvas,
           save_img,
           exit,
           new_name_file,
           img_save_ok,
           img_save_no,
           error_resize_texture,
           error_no_supp_file,
           warning_big_img,
           welcome,
           version,
           help,
           enter_file_path,
           enter_canvas_factor,
           create_file,
           enter_img_w,
           enter_img_h,
           error_path_len,
           error_temp_file_create,
           write_in_temp_file,
           write_data,
           remove_temp_file;
};

extern Locale en_locale, // Английский
              fr_locale, // Француский
              de_locale, // Немецкий
              es_locale, // Испанский
              ru_locale; // Русский

extern Locale *locale;   // Текущая локаль

// Установка языка l = локаль (есть auto)
void set_lang(string l);