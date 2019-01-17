#include "game/system.h"
#include "input/cursor.h"
#include "platform/dpi.h"

#include "SDL.h"

static SDL_Cursor *cursors[CURSOR_MAX];
static SDL_Surface *cursor_surfaces[CURSOR_MAX];
static int current_cursor_id = CURSOR_ARROW;

static const SDL_Color mouse_colors[] = {
    { 0x00, 0x00, 0x00, 0x00 }, /* Transparent */
    { 0x00, 0x00, 0x00, 0xFF }, /* Black */
    { 0x3F, 0x3F, 0x3F, 0xFF }, /* Dark gray */
    { 0x73, 0x73, 0x73, 0xFF }, /* Medium gray */
    { 0xB3, 0xB3, 0xB3, 0xFF }, /* Light gray */
    { 0xFF, 0xFF, 0xFF, 0xFF }  /* White */
};

static SDL_Surface *generate_cursor_surface(const char *data, int width, int height)
{
    SDL_Surface * cursor_surface = SDL_CreateRGBSurfaceFrom((void *)data, width, height, 8, sizeof(Uint8) * width, 0, 0, 0, 0);

    SDL_LockSurface(cursor_surface);
    SDL_SetPaletteColors(cursor_surface->format->palette, mouse_colors, ' ', 1);
    SDL_SetPaletteColors(cursor_surface->format->palette, &mouse_colors[1], '#', 5);
    SDL_UnlockSurface(cursor_surface);

    return cursor_surface;
}

void system_init_cursors(void)
{
    double dpi_scale = platform_dpi_get_scale();
    for (int i = 0; i < CURSOR_MAX; i++) {
        const cursor * c = input_cursor_data(i, dpi_scale);
        cursor_surfaces[i] = generate_cursor_surface(c->data, c->width, c->height);
        cursors[i] = SDL_CreateColorCursor(cursor_surfaces[i], c->hotspot_x, c->hotspot_y);
    }
    system_set_cursor(current_cursor_id);
}

void system_set_cursor(int cursor_id)
{
    current_cursor_id = cursor_id;
    SDL_SetCursor(cursors[cursor_id]);
}

void system_free_cursors(void)
{
    for (int i = 0; i < CURSOR_MAX; i++) {
        SDL_FreeCursor(cursors[i]);
        SDL_FreeSurface(cursor_surfaces[i]);
    }
}
