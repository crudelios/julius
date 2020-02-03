#include "save.h"

#include "core/file.h"
#include "game/file_io.h"

#define MAX_SAVE_FILE_NAME FILE_NAME_MAX - 15
static int autosave_slot;
static char city_name[MAX_SAVE_FILE_NAME];
static char file_name[FILE_NAME_MAX];
static int enabled;

void game_save_prepare(const uint8_t *name)
{
    if (!name) {
        enabled = 0;
    }
    autosave_slot = 0;
    snprintf(city_name, MAX_SAVE_FILE_NAME, "%s", name);
    enabled = 1;
}

void game_save_quicksave(void)
{
    if (enabled) {
        snprintf(file_name, FILE_NAME_MAX, "quicksave-%s.sav", city_name);
        game_file_io_write_saved_game(file_name);
    }
}

void game_save_autosave(void)
{
    if (enabled) {
        snprintf(file_name, FILE_NAME_MAX, "autosave-%s-%d.sav", city_name, autosave_slot);
        game_file_io_write_saved_game(file_name);
        autosave_slot = (autosave_slot + 1) % 10;
    }
}