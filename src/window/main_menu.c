#include "main_menu.h"

#include "core/dir.h"
#include "core/string.h"
#include "editor/editor.h"
#include "game/file.h"
#include "game/game.h"
#include "game/save.h"
#include "game/system.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/image_button.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/screen.h"
#include "graphics/window.h"
#include "platform/version.h"
#include "sound/music.h"
#include "window/cck_selection.h"
#include "window/city.h"
#include "window/config.h"
#include "window/file_dialog.h"
#include "window/new_career.h"
#include "window/plain_message_dialog.h"
#include "window/popup_dialog.h"

#define MAX_BUTTONS 7

static void button_click(int type, int param2);

static int focus_button_id;

static const char *latest_saved_file;

static generic_button buttons[] = {
    {192, 100, 256, 25, button_click, button_none, 1, 0},
    {192, 140, 256, 25, button_click, button_none, 2, 0},
    {192, 180, 256, 25, button_click, button_none, 3, 0},
    {192, 220, 256, 25, button_click, button_none, 4, 0},
    {192, 260, 256, 25, button_click, button_none, 5, 0},
    {192, 300, 256, 25, button_click, button_none, 6, 0},
    {192, 340, 256, 25, button_click, button_none, 7, 0},
};

static void draw_version_string(void)
{
    uint8_t version_string[100] = "v";
    const uint8_t *julius_version = string_from_ascii(JULIUS_VERSION);
    const uint8_t *julius_version_suffix = string_from_ascii(JULIUS_VERSION_SUFFIX);
    int version_length = string_length(julius_version);
    int text_y = screen_height() - 30;

    string_copy(julius_version, version_string + 1, 99);
    string_copy(julius_version_suffix, version_string + 1 + version_length, 99 - version_length);

    int text_width = text_get_width(version_string, FONT_SMALL_PLAIN);

    if (text_y <= 500 && (screen_width() - 640) / 2 < text_width + 18) {
        graphics_draw_rect(10, text_y, text_width + 14, 20, COLOR_BLACK);
        graphics_fill_rect(11, text_y + 1, text_width + 12, 18, COLOR_WHITE);
        text_draw(version_string, 18, text_y + 6, FONT_SMALL_PLAIN, COLOR_BLACK);
    } else {
        text_draw(version_string, 18, text_y + 6, FONT_SMALL_PLAIN, COLOR_WHITE);
    }
}

static void draw_background(void)
{
    graphics_clear_screen();
    graphics_in_dialog();
    image_draw(image_group(GROUP_MAIN_MENU_BACKGROUND), 0, 0);
    graphics_reset_dialog();
    draw_version_string();
}

static void draw_foreground(void)
{
    graphics_in_dialog();

    for (int i = 0; i < MAX_BUTTONS; i++) {
        large_label_draw(buttons[i].x, buttons[i].y, buttons[i].width / 16, focus_button_id == i + 1 ? 1 : 0);
    }

    lang_text_draw_centered(13, 5, 192, 106, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(30, 1, 192, 146, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(30, 2, 192, 186, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(30, 3, 192, 226, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(9, 8, 192, 266, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(2, 0, 192, 306, 256, FONT_NORMAL_GREEN);
    lang_text_draw_centered(30, 5, 192, 346, 256, FONT_NORMAL_GREEN);

    graphics_reset_dialog();
}

static void handle_autosaves(void)
{
    game_save_prepare(0);
    latest_saved_file = dir_get_last_file_with_extension("sav");
}

static void handle_mouse(const mouse *m)
{
    const mouse *m_dialog = mouse_in_dialog(m);
    generic_buttons_handle_mouse(m_dialog, 0, 0, buttons, MAX_BUTTONS, &focus_button_id);
}

static void confirm_exit(int accepted)
{
    if (accepted) {
        system_exit();
    }
}

static void button_click(int type, int param2)
{
    if (type == 1) {
        if (latest_saved_file && game_file_load_saved_game(latest_saved_file)) {
            window_city_show();
        }
    }
    else if (type == 2) {
        window_new_career_show();
    } else if (type == 3) {
        window_file_dialog_show(FILE_TYPE_SAVED_GAME, FILE_DIALOG_LOAD);
    } else if (type == 4) {
        window_cck_selection_show();
    } else if (type == 5) {
        if (!editor_is_present() || !game_init_editor()) {
            window_plain_message_dialog_show(
                "Editor not installed",
                "Your Caesar 3 installation does not contain the editor files. "
                "You can download them from:\n"
                "https://bintray.com/bvschaik/caesar3-editor"
            );
        } else {
            sound_music_play_editor();
        }
    } else if (type == 6) {
        window_config_show();
    } else if (type == 7) {
        window_popup_dialog_show(POPUP_DIALOG_QUIT, confirm_exit, 1);
    }
}

void window_main_menu_show(int restart_music)
{
    handle_autosaves();
    if (restart_music) {
        sound_music_play_intro();
    }
    window_type window = {
        WINDOW_MAIN_MENU,
        draw_background,
        draw_foreground,
        handle_mouse
    };
    window_show(&window);
}
