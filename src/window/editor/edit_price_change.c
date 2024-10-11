#include "edit_price_change.h"

#include "game/resource.h"
#include "graphics/button.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "input/input.h"
#include "scenario/editor.h"
#include "scenario/price_change.h"
#include "scenario/property.h"
#include "window/editor/map.h"
#include "window/editor/price_changes.h"
#include "window/numeric_input.h"
#include "window/select_list.h"

static void button_year(const generic_button *button);
static void button_resource(const generic_button *button);
static void button_toggle_rise(const generic_button *button);
static void button_amount(const generic_button *button);
static void button_delete(const generic_button *button);
static void button_save(const generic_button *button);

#define NUM_BUTTONS (sizeof(buttons) / sizeof(generic_button))

static generic_button buttons[] = {
    {30, 152, 60, 25, button_year},
    {240, 152, 120, 25, button_resource},
    {100, 192, 200, 25, button_toggle_rise},
    {350, 192, 100, 25, button_amount},
    {30, 230, 250, 25, button_delete},
    {320, 230, 100, 25, button_save}
};

static struct {
    price_change_t price_change;
    unsigned int focus_button_id;
    resource_type available_resources[RESOURCE_MAX];
} data;

static void init(int id)
{
    const price_change_t *price_change = scenario_price_change_get(id);
    data.price_change = *price_change;
}

static void draw_background(void)
{
    window_editor_map_draw_all();

    graphics_in_dialog();

    outer_panel_draw(0, 100, 38, 11);
    lang_text_draw(44, 95, 20, 114, FONT_LARGE_BLACK);

    text_draw_number_centered_prefix(data.price_change.year, '+', 30, 158, 60, FONT_NORMAL_BLACK);
    lang_text_draw_year(scenario_property_start_year() + data.price_change.year, 100, 158, FONT_NORMAL_BLACK);

    text_draw_centered(resource_get_data(data.price_change.resource)->text, 240, 158, 120, FONT_NORMAL_BLACK,
        COLOR_MASK_NONE);

    lang_text_draw_centered(44, data.price_change.is_rise ? 104 : 103, 100, 198, 200, FONT_NORMAL_BLACK);

    text_draw_number_centered(data.price_change.amount, 350, 198, 100, FONT_NORMAL_BLACK);

    lang_text_draw_centered(44, 105, 30, 236, 250, FONT_NORMAL_BLACK);

    lang_text_draw_centered(18, 3, 320, 236, 100, FONT_NORMAL_BLACK);

    graphics_reset_dialog();
}

static void draw_foreground(void)
{
    graphics_in_dialog();

    for (size_t i = 0; i < NUM_BUTTONS; i++) {
        int focus = data.focus_button_id == i + 1;
        button_border_draw(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, focus);
    }

    graphics_reset_dialog();
}

static void handle_input(const mouse *m, const hotkeys *h)
{
    if (generic_buttons_handle_mouse(mouse_in_dialog(m), 0, 0, buttons, 6, &data.focus_button_id)) {
        return;
    }
    if (input_go_back_requested(m, h)) {
        button_save(0);
    }
}

static void set_year(int value)
{
    data.price_change.year = value;
}

static void button_year(const generic_button *button)
{
    window_numeric_input_show(screen_dialog_offset_x() + 100, screen_dialog_offset_y() + 50, 3, 999, set_year);
}

static void set_resource(int value)
{
    data.price_change.resource = data.available_resources[value];
}

static void button_resource(const generic_button *button)
{
    static const uint8_t *resource_texts[RESOURCE_MAX];
    static int total_resources = 0;
    if (!total_resources) {
        for (resource_type resource = RESOURCE_NONE; resource < RESOURCE_MAX; resource++) {
            if (!resource_is_storable(resource)) {
                continue;
            }
            resource_texts[total_resources] = resource_get_data(resource)->text;
            data.available_resources[total_resources] = resource;
            total_resources++;
        }
    }
    window_select_list_show_text(screen_dialog_offset_x() + 25, screen_dialog_offset_y() + 40,
        resource_texts, total_resources, set_resource);
}

static void button_toggle_rise(const generic_button *button)
{
    data.price_change.is_rise = !data.price_change.is_rise;
    window_request_refresh();
}

static void set_amount(int value)
{
    data.price_change.amount = value;
}

static void button_amount(const generic_button *button)
{
    window_numeric_input_show(screen_dialog_offset_x() + 460, screen_dialog_offset_y() + 50, 2, 99, set_amount);
}

static void button_delete(const generic_button *button)
{
    scenario_price_change_delete(data.price_change.id);
    scenario_editor_set_as_unsaved();
    window_editor_price_changes_show();
}

static void button_save(const generic_button *button)
{
    scenario_price_change_update(&data.price_change);
    scenario_editor_set_as_unsaved();
    window_editor_price_changes_show();
}

void window_editor_edit_price_change_show(int id)
{
    window_type window = {
        WINDOW_EDITOR_EDIT_PRICE_CHANGE,
        draw_background,
        draw_foreground,
        handle_input
    };
    init(id);
    window_show(&window);
}
