#include "edit_request.h"

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
#include "scenario/property.h"
#include "scenario/request.h"
#include "window/editor/map.h"
#include "window/editor/requests.h"
#include "window/numeric_input.h"
#include "window/select_list.h"

static void button_year(int param1, int param2);
static void button_amount(int param1, int param2);
static void button_resource(int param1, int param2);
static void button_deadline_years(int param1, int param2);
static void button_favor(int param1, int param2);
static void button_extension_months(int param1, int param2);
static void button_extension_disfavor(int param1, int param2);
static void button_ignored_disfavor(int param1, int param2);
static void button_delete(int param1, int param2);
static void button_save(int param1, int param2);

static struct {
    scenario_request request;
    unsigned int focus_button_id;
    resource_type avaialble_resources[RESOURCE_MAX];
} data;

#define NUM_BUTTONS (sizeof(buttons) / sizeof(generic_button))

static generic_button buttons[] = {
    {30, 186, 60, 25, button_year, button_none},
    {330, 186, 80, 25, button_amount, button_none},
    {430, 186, 100, 25, button_resource, button_none},
    {70, 224, 140, 25, button_deadline_years, button_none},
    {400, 224, 80, 25, button_favor, button_none},
    {400, 264, 80, 25, button_extension_months, button_none},
    {400, 304, 80, 25, button_extension_disfavor, button_none},
    {400, 344, 80, 25, button_ignored_disfavor, button_none},
    {110, 384, 250, 25, button_delete, button_none},
    {400, 384, 100, 25, button_save, button_none}
};

static void init(int id)
{
    const scenario_request *request = scenario_request_get(id);
    data.request = *request;
}

static void draw_background(void)
{
    window_editor_map_draw_all();

    graphics_in_dialog();

    outer_panel_draw(0, 100, 38, 20);
    lang_text_draw_centered(44, 21, 14, 114, 580, FONT_LARGE_BLACK);

    int width = text_get_number_width(data.request.id, 0, "", FONT_NORMAL_BLACK);
    width += lang_text_get_width(CUSTOM_TRANSLATION, TR_EDITOR_SCENARIO_EVENT_ID, FONT_NORMAL_BLACK);
    int base_x = (580 - width) / 2 + 14;

    width = lang_text_draw(CUSTOM_TRANSLATION, TR_EDITOR_SCENARIO_EVENT_ID, base_x, 154, FONT_NORMAL_BLACK);
    text_draw_number(data.request.id, 0, "", base_x + width, 154, FONT_NORMAL_BLACK, 0);

    text_draw_number_centered_prefix(data.request.year, '+', 30, 192, 60, FONT_NORMAL_BLACK);
    lang_text_draw_year(scenario_property_start_year() + data.request.year, 110, 192, FONT_NORMAL_BLACK);

    lang_text_draw(44, 72, 250, 192, FONT_NORMAL_BLACK);
    text_draw_number_centered(data.request.amount, 330, 192, 80, FONT_NORMAL_BLACK);

    text_draw_centered(resource_get_data(data.request.resource)->text, 430, 192, 100, FONT_NORMAL_BLACK,
        COLOR_MASK_NONE);

    lang_text_draw(44, 24, 40, 230, FONT_NORMAL_BLACK);

    lang_text_draw_amount(8, 8, data.request.deadline_years, 80, 230, FONT_NORMAL_BLACK);

    lang_text_draw(44, 73, 300, 230, FONT_NORMAL_BLACK);

    text_draw_number_centered_prefix(data.request.favor, '+', 400, 230, 80, FONT_NORMAL_BLACK);

    lang_text_draw(CUSTOM_TRANSLATION, TR_EDITOR_FAVOUR_EXTENSION_MONTHS, 70, 270, FONT_NORMAL_BLACK);
    text_draw_number_centered_prefix(data.request.extension_months_to_comply, '+', 400, 270, 80, FONT_NORMAL_BLACK);

    lang_text_draw(CUSTOM_TRANSLATION, TR_EDITOR_FAVOUR_DISFAVOR, 70, 310, FONT_NORMAL_BLACK);
    text_draw_number_centered_prefix(data.request.extension_disfavor, '-', 400, 310, 80, FONT_NORMAL_BLACK);

    lang_text_draw(CUSTOM_TRANSLATION, TR_EDITOR_FAVOUR_IGNORED, 70, 350, FONT_NORMAL_BLACK);
    text_draw_number_centered_prefix(data.request.ignored_disfavor, '-', 400, 350, 80, FONT_NORMAL_BLACK);
    lang_text_draw_centered(18, 3, 400, 390, 100, FONT_NORMAL_BLACK);
    lang_text_draw_centered(44, 25, 110, 390, 250, FONT_NORMAL_BLACK);

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
    if (generic_buttons_handle_mouse(mouse_in_dialog(m), 0, 0, buttons, NUM_BUTTONS, &data.focus_button_id)) {
        return;
    }
    if (input_go_back_requested(m, h)) {
        button_save(0, 0);
    }
}

static void set_year(int value)
{
    data.request.year = value;
}

static void button_year(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 100, screen_dialog_offset_y() + 50, 3, 999, set_year);
}

static void set_amount(int value)
{
    data.request.amount = value;
}

static void button_amount(int param1, int param2)
{
    int max_amount = 999;
    int max_digits = 3;
    if (data.request.resource == RESOURCE_DENARII) {
        max_amount = 30000;
        max_digits = 5;
    }
    window_numeric_input_show(
        screen_dialog_offset_x() + 190, screen_dialog_offset_y() + 50,
        max_digits, max_amount, set_amount
    );
}

static void set_resource(int value)
{
    data.request.resource = data.avaialble_resources[value];
    if (data.request.amount > 999) {
        data.request.amount = 999;
    }
}

static void button_resource(int param1, int param2)
{
    static const uint8_t *resource_texts[RESOURCE_MAX + RESOURCE_TOTAL_SPECIAL];
    static int total_resources = 0;
    if (!total_resources) {
        for (resource_type resource = RESOURCE_NONE; resource < RESOURCE_MAX + RESOURCE_TOTAL_SPECIAL; resource++) {
            if (!resource_is_storable(resource) && resource < RESOURCE_MAX) {
                continue;
            }
            resource_texts[total_resources] = resource_get_data(resource)->text;
            data.avaialble_resources[total_resources] = resource;
            total_resources++;
        }
    }
    window_select_list_show_text(screen_dialog_offset_x() + 210, screen_dialog_offset_y() + 40,
        resource_texts, total_resources, set_resource);
}

static void set_deadline_years(int value)
{
    data.request.deadline_years = value;
}

static void button_deadline_years(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 220, screen_dialog_offset_y() + 100,
        3, 999, set_deadline_years);
}

static void set_favor(int value)
{
    data.request.favor = value;
}

static void button_favor(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 260, screen_dialog_offset_y() + 100, 3, 100, set_favor);
}

static void set_extension_months(int value)
{
    data.request.extension_months_to_comply = value;
}

static void button_extension_months(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 260, screen_dialog_offset_y() + 100, 3, 120, set_extension_months);
}

static void set_extension_disfavor(int value)
{
    data.request.extension_disfavor = value;
}

static void button_extension_disfavor(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 260, screen_dialog_offset_y() + 100, 3, 100, set_extension_disfavor);
}

static void set_ignored_disfavor(int value)
{
    data.request.ignored_disfavor = value;
}

static void button_ignored_disfavor(int param1, int param2)
{
    window_numeric_input_show(screen_dialog_offset_x() + 260, screen_dialog_offset_y() + 100, 3, 100, set_ignored_disfavor);
}

static void button_delete(int param1, int param2)
{
    scenario_request_delete(data.request.id);
    scenario_editor_set_as_unsaved();
    window_go_back();
}

static void button_save(int param1, int param2)
{
    scenario_request_update(&data.request);
    scenario_editor_set_as_unsaved();
    window_go_back();
}

void window_editor_edit_request_show(int id)
{
    window_type window = {
        WINDOW_EDITOR_EDIT_REQUEST,
        draw_background,
        draw_foreground,
        handle_input
    };
    init(id);
    window_show(&window);
}
