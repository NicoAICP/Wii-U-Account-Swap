#ifndef SUB_SCREENS_HPP
#define SUB_SCREENS_HPP


void draw_unlink_menu();
void draw_backup_menu();
void draw_overwrite_menu(const char* backup_path);
void draw_error_menu(const char* error_message);
void draw_success_menu(const char* type, bool inkay = false);


#endif