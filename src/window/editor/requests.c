#include "requests.h"

#include "core/image_group_editor.h"
#include "game/resource.h"
#include "graphics/button.h"
#include "graphics/graphics.h"
#include "graphics/grid_box.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "input/input.h"
#include "scenario/data.h"
#include "scenario/editor.h"
#include "scenario/property.h"
#include "scenario/request.h"
#include "window/editor/attributes.h"
#include "window/editor/edit_request.h"
#include "window/editor/map.h"

static void button_request(unsigned int id, unsigned int mouse_x, unsigned int mouse_y);
static void draw_request_button(const grid_box_item *item);

static struct {
    const scenario_request **requests;
    unsigned int total_requests;
} data;

static grid_box_type request_buttons = {
    .x = 20,
    .y = 40,
    .width = 38 * BLOCK_SIZE,
    .height = 19 * BLOCK_SIZE,
    .num_columns = 2,
    .item_height = 30,
    .item_margin.horizontal = 10,
    .item_margin.vertical = 5,
    .extend_to_hidden_scrollbar = 1,
    .on_click = button_request,
    .draw_item = draw_request_button
};

static void sort_list(void)
{
    for (unsigned int i = 0; i < data.total_requests; i++) {
        for (unsigned int j = data.total_requests - 1; j > 0; j--) {
            const scenario_request *current = data.requests[j];
            const scenario_request *prev = data.requests[j - 1];
            if (current->resource && (!prev->resource || prev->year > current->year)) {
                const scenario_request *tmp = data.requests[j];
                data.requests[j] = data.requests[j - 1];
                data.requests[j - 1] = tmp;
            }
        }
    }
}

static void update_request_list(void)
{
    int current_requests = scenario_request_count_total();
    if (current_requests != data.total_requests) {
        free(data.requests);
        data.requests = 0;
        grid_box_update_total_items(&request_buttons, data.total_requests);
        if (current_requests) {
            data.requests = malloc(current_requests * sizeof(scenario_request *));
            if (!data.requests) {
                data.total_requests = 0;
                return;
            }
            for (unsigned int i = 0; i < current_requests; i++) {
                data.requests[i] = scenario_request_get(i);
            }
            data.total_requests = current_requests;
        }
    }
    sort_list();
}

static void draw_background(void)
{
    window_editor_map_draw_all();

    graphics_in_dialog();

    outer_panel_draw(0, 0, 40, 30);
    lang_text_draw(44, 14, 20, 12, FONT_LARGE_BLACK);
    lang_text_draw_centered(13, 3, 0, 456, 640, FONT_NORMAL_BLACK);
    lang_text_draw_multiline(152, 1, 32, 376, 576, FONT_NORMAL_BLACK);

    graphics_reset_dialog();

    update_request_list();

    grid_box_request_refresh(&request_buttons);
}

static void draw_request_button(const grid_box_item *item)
{
    button_border_draw(item->x, item->y, item->width, item->height, item->is_focused);
    const scenario_request *request = data.requests[item->index];
    if (request->resource) {
        if (*request->description) {
            text_draw_centered(request->description, item->x, item->y + 7, item->width, FONT_NORMAL_BLACK, 0);
        } else {
            text_draw_number(request->year, '+', " ", item->x + 10, item->y + 7, FONT_NORMAL_BLACK, 0);
            lang_text_draw_year(scenario_property_start_year() + request->year, item->x + 65, item->y + 7,
                FONT_NORMAL_BLACK);
            int width = text_draw_number(request->amount, '@', " ", item->x + 165, item->y + 7, FONT_NORMAL_BLACK, 0);
            image_draw(resource_get_data(request->resource)->image.editor.icon, item->x + 165 + width, item->y + 2,
                COLOR_MASK_NONE, SCALE_NONE);
        }
    } else {
        lang_text_draw_centered(44, 23, item->x, item->y + 7, item->width, FONT_NORMAL_BLACK);
    }
}

static void draw_foreground(void)
{
    graphics_in_dialog();

    if (data.total_requests) {
        grid_box_draw(&request_buttons);
    } else {
        text_draw("No requests", 20, 40, FONT_NORMAL_BLACK, 0);
    }

    graphics_reset_dialog();
}

static void handle_input(const mouse *m, const hotkeys *h)
{
    if (grid_box_handle_input(&request_buttons, mouse_in_dialog(m), 1)) {
        return;
    }
    if (input_go_back_requested(m, h)) {
        window_editor_attributes_show();
    }
}

static void button_request(unsigned int id, unsigned int mouse_x, unsigned int mouse_y)
{
    window_editor_edit_request_show(data.requests[id]->id);
}

void window_editor_requests_show(void)
{
    window_type window = {
        WINDOW_EDITOR_REQUESTS,
        draw_background,
        draw_foreground,
        handle_input
    };
    grid_box_init(&request_buttons, scenario_request_count_total());
    window_show(&window);
}
