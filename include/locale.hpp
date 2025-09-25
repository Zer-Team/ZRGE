/*
    Copyright (C) 2025 Zakhar Shakhanov

    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

*/

#pragma once

#include <string>

using std::string;

struct Locale {
    string zrge;
    string error_open_file;
    string error_load_img;
    string error_size;
    string texture;
    string texture_bg;
    string image;
    string canvas;
    string save_img;
    string exit;
    string new_name_file;
    string img_save_ok;
    string img_save_no;
    string error_resize_texture;
    string error_no_supp_file;
    string warning_big_img;
    string welcome;
    string version;
    string help;
    string enter_file_path;
    string enter_canvas_factor;
    string create_file;
    string enter_img_w;
    string enter_img_h;
    string error_path_len;
    string error_temp_file_create;
    string write_in_temp_file;
    string write_data;
    string remove_temp_file;
};

extern Locale en_locale; // Английский
extern Locale fr_locale; // Француский
extern Locale de_locale; // Немецкий
extern Locale es_locale; // Испанский
extern Locale ru_locale; // Русский

extern Locale *locale;