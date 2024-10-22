#include "scenario_event_details.h"

#include "assets/assets.h"
#include "core/log.h"
#include "core/string.h"
#include "editor/editor.h"
#include "graphics/button.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/grid_box.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "input/input.h"
#include "scenario/scenario_event.h"
#include "scenario/scenario_events_controller.h"
#include "scenario/scenario_events_parameter_data.h"
#include "window/editor/map.h"
#include "window/editor/scenario_action_edit.h"
#include "window/editor/scenario_condition_edit.h"
#include "window/numeric_input.h"

#define BUTTON_LEFT_PADDING 32
#define BUTTON_WIDTH 608
#define SHORT_BUTTON_LEFT_PADDING 128
#define SHORT_BUTTON_WIDTH 480
#define EVENT_REPEAT_Y_OFFSET 96
#define DETAILS_Y_OFFSET 192
#define DETAILS_ROW_HEIGHT 32
#define MAX_VISIBLE_ROWS 10
#define MAX_TEXT_LENGTH 75

enum {
    SCENARIO_EVENT_DETAILS_SET_MAX_REPEATS = 0,
    SCENARIO_EVENT_DETAILS_SET_REPEAT_MIN,
    SCENARIO_EVENT_DETAILS_SET_REPEAT_MAX,
};

static void button_amount(const generic_button *button);
static void button_add_new_condition(const generic_button *button);
static void button_add_new_action(const generic_button *button);
static void button_delete_event(const generic_button *button);
static void button_ok(const generic_button *button);

static void draw_condition_button(const grid_box_item *item);
static void draw_action_button(const grid_box_item *item);
static void click_condition_button(unsigned int index, unsigned int mouse_x, unsigned int mouse_y);
static void click_action_button(unsigned int index, unsigned int mouse_x, unsigned int mouse_y);
static void handle_condition_tooltip(const grid_box_item *item, tooltip_context *c);
static void handle_action_tooltip(const grid_box_item *item, tooltip_context *c);

#define NUM_BUTTONS (sizeof(buttons) / sizeof(generic_button))

static generic_button buttons[] = {
    {24, 404, 288, 25, button_add_new_condition},
    {328, 404, 288, 25, button_add_new_action},
    {16, 439, 200, 25, button_delete_event},
    {524, 439, 100, 25, button_ok},
};

typedef struct {
    int group_id;
    scenario_condition_t *condition;
} condition_list_item;

static grid_box_type conditions_grid_box = {
    .x = 16,
    .y = 228,
    .width = 18 * BLOCK_SIZE,
    .height = 10 * BLOCK_SIZE,
    .num_columns = 1,
    .item_height = 30,
    .item_margin.horizontal = 10,
    .item_margin.vertical = 4,
    .extend_to_hidden_scrollbar = 1,
    .draw_item = draw_condition_button,
    .on_click = click_condition_button,
    .handle_tooltip = handle_condition_tooltip
};

static grid_box_type actions_grid_box = {
    .x = 320,
    .y = 228,
    .width = 18 * BLOCK_SIZE,
    .height = 10 * BLOCK_SIZE,
    .num_columns = 1,
    .item_height = 30,
    .item_margin.horizontal = 10,
    .item_margin.vertical = 4,
    .extend_to_hidden_scrollbar = 1,
    .draw_item = draw_action_button,
    .on_click = click_action_button,
  // .handle_tooltip = handle_action_tooltip
};

static struct {
    unsigned int focus_button_id;
    scenario_event_t *event;
    struct {
        condition_list_item *list;
        unsigned int active;
        uint8_t *selected;
        unsigned int available;
    } conditions;
    struct {
        scenario_action_t **list;
        unsigned int active;
        uint8_t *selected;
        unsigned int available;
    } actions;
    grid_box_type *focused_grid_box;
} data;

static unsigned int count_maximum_needed_list_items(void)
{
    unsigned int total_items;
    scenario_condition_group_t *group;
    array_foreach(data.event->condition_groups, group) {
        if (group->conditions.size > 0) {
            total_items += group->conditions.size + 1;
        }
    }
    return total_items;
}

static void update_visible_conditions_and_actions(void)
{
    unsigned int max_needed_items = count_maximum_needed_list_items();
    if (max_needed_items > data.conditions.available) {
        free(data.conditions.list);
        free(data.conditions.selected);
        data.conditions.list = calloc(max_needed_items, sizeof(condition_list_item));
        data.conditions.selected = calloc(max_needed_items, sizeof(uint8_t));

        if (!data.conditions.list) {
            log_error("Unable to create conditions list - out of memory. The game will probably crash.", 0, 0);
            data.conditions.available = 0;
        } else {
            data.conditions.available = max_needed_items;
        }
    }
    data.conditions.active = 0;
    if (data.conditions.available) {
        scenario_condition_group_t *group;
        scenario_condition_t *condition;
        for (unsigned int i = 1; i < data.event->condition_groups.size; i++) {
            group = array_item(data.event->condition_groups, i);
            if (group->conditions.size > 0) {
                data.conditions.list[data.conditions.active].group_id = i;
                data.conditions.active++;
                array_foreach(group->conditions, condition) {
                    if (condition->type != CONDITION_TYPE_UNDEFINED) {
                        data.conditions.list[data.conditions.active].group_id = i;
                        data.conditions.list[data.conditions.active].condition = condition;
                        data.conditions.active++;
                    }
                }
            }
        }
        group = array_item(data.event->condition_groups, 0);
        if (data.conditions.active && group->conditions.size > 0) {
            data.conditions.list[data.conditions.active].group_id = 0;
            data.conditions.active++;
        }
        array_foreach(group->conditions, condition) {
            if (condition->type != CONDITION_TYPE_UNDEFINED) {
                data.conditions.list[data.conditions.active].group_id = 0;
                data.conditions.list[data.conditions.active].condition = condition;
                data.conditions.active++;
            }
        }
    }
    grid_box_update_total_items(&conditions_grid_box, data.conditions.active);

    if (data.event->actions.size > data.actions.available) {
        free(data.actions.list);
        free(data.actions.selected);
        data.actions.list = calloc(data.event->actions.size, sizeof(scenario_action_t *));
        data.actions.selected = calloc(data.event->actions.size, sizeof(uint8_t));

        if (!data.actions.list) {
            log_error("Unable to create actions list - out of memory. The game will probably crash.", 0, 0);
            data.actions.available = 0;
        } else {
            data.actions.available = data.event->actions.size;
        }
    }
    data.actions.active = 0;
    if (data.actions.available) {
        scenario_action_t *action;
        array_foreach(data.event->actions, action) {
            if (action->type != ACTION_TYPE_UNDEFINED) {
                data.actions.list[data.actions.active] = action;
                data.actions.active++;
            }
        }
    }
    grid_box_update_total_items(&actions_grid_box, data.actions.active);
}

static void select_no_conditions(void)
{
    for (unsigned int i = 0; i < data.conditions.active; i++) {
        data.conditions.selected[i] = 0;
    }
}

static void select_no_actions(void)
{
    for (unsigned int i = 0; i < data.actions.active; i++) {
        data.actions.selected[i] = 0;
    }
}

static void select_all_conditions(void)
{
    for (unsigned int i = 0; i < data.conditions.active; i++) {
        data.conditions.selected[i] = 1;
    }
}

static void select_all_actions(void)
{
    for (unsigned int i = 0; i < data.actions.active; i++) {
        data.actions.selected[i] = 1;
    }
}

static void init(int event_id)
{
    data.event = scenario_event_get(event_id);
    grid_box_init(&conditions_grid_box, count_maximum_needed_list_items());
    grid_box_init(&actions_grid_box, data.event->actions.size);
    select_no_conditions();
    select_no_actions();
}

static int color_from_state(event_state state)
{
    if (!editor_is_active()) {
        if (state == EVENT_STATE_ACTIVE) {
            return COLOR_MASK_GREEN;
        } else if (state == EVENT_STATE_PAUSED) {
            return COLOR_MASK_RED;
        } else if (state == EVENT_STATE_DISABLED) {
            return COLOR_MASK_GREY;
        }
    }
    return COLOR_MASK_NONE;
}

static void draw_background(void)
{
    update_visible_conditions_and_actions();

    window_editor_map_draw_all();

    graphics_in_dialog();

    outer_panel_draw(0, 0, 40, 30);

    // Title and ID
    text_draw_centered(translation_for(TR_EDITOR_SCENARIO_EVENTS_TITLE), 0, 16, 640, FONT_LARGE_BLACK, 0);
    text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_ID),
        data.event->id, "", 16, 24, FONT_NORMAL_PLAIN, COLOR_BLACK);

    // Refresh lists
    grid_box_request_refresh(&conditions_grid_box);
    grid_box_request_refresh(&actions_grid_box);

    // Add condition/action buttons
    lang_text_draw_centered(CUSTOM_TRANSLATION, TR_EDITOR_SCENARIO_CONDITION_ADD,
        buttons[0].x, buttons[0].y + 6, buttons[0].width, FONT_NORMAL_BLACK);

    lang_text_draw_centered(CUSTOM_TRANSLATION, TR_EDITOR_SCENARIO_ACTION_ADD,
        buttons[1].x, buttons[1].y + 6, buttons[1].width, FONT_NORMAL_BLACK);
    
    // Bottom buttons
    lang_text_draw_centered_colored(CUSTOM_TRANSLATION, TR_EDITOR_DELETE, buttons[2].x, buttons[2].y + 6,
        buttons[2].width, FONT_NORMAL_PLAIN, COLOR_RED);
    lang_text_draw_centered(18, 3, buttons[3].x, buttons[3].y + 6, buttons[3].width, FONT_NORMAL_BLACK);

    graphics_reset_dialog();
}

static void draw_condition_button(const grid_box_item *item)
{
    const condition_list_item *list_item = &data.conditions.list[item->index];
    if (!list_item->condition) {
        text_draw(string_from_ascii(list_item->group_id ? "Group" : "No group"), item->x + 4, item->y + 7,
            FONT_NORMAL_BLACK, 0);
        return;
    }
    int selection_button_y_offset = (item->height - 20) / 2;
    button_border_draw(item->x, item->y + selection_button_y_offset, 20, 20,
        item->is_focused && item->mouse.x < 20);

    if (data.conditions.selected && data.conditions.selected[item->index]) {
        int checkmark_id = assets_lookup_image_id(ASSET_UI_SELECTION_CHECKMARK);
        const image *img = image_get(checkmark_id);
        image_draw(checkmark_id, item->x + (20 - img->original.width) / 2,
            item->y + selection_button_y_offset + (20 - img->original.height) / 2, COLOR_MASK_NONE, SCALE_NONE);
    }

    button_border_draw(item->x + 24, item->y, item->width - 24, item->height,
        item->is_focused && item->mouse.x >= 24);

    const scenario_condition_t *condition = data.conditions.list[item->index].condition;
    uint8_t text[MAX_TEXT_LENGTH];
    scenario_events_parameter_data_get_display_string_for_condition(condition, text, MAX_TEXT_LENGTH);
    if (text_get_width(text, FONT_NORMAL_BLACK) > item->width - item->height - 12) {
        text_draw_ellipsized(text, item->x + 28, item->y + 7, item->width - 32, FONT_NORMAL_BLACK, 0);
    } else {
        text_draw_centered(text, item->x + 28, item->y + 7, item->width - 32, FONT_NORMAL_BLACK, 0);
    }
}

static void draw_action_button(const grid_box_item *item)
{
    button_border_draw(item->x, item->y, item->width, item->height, item->is_focused);
    const scenario_action_t *action = data.actions.list[item->index];
    uint8_t text[MAX_TEXT_LENGTH];
    scenario_events_parameter_data_get_display_string_for_action(action, text, MAX_TEXT_LENGTH);
    if (text_get_width(text, FONT_NORMAL_BLACK) > item->width - 8) {
        text_draw_ellipsized(text, item->x + 4, item->y + 7, item->width - 8, FONT_NORMAL_BLACK, 0);
    } else {
        text_draw_centered(text, item->x, item->y + 7, item->width, FONT_NORMAL_BLACK, 0);
    }
}

static void draw_foreground(void)
{
    graphics_in_dialog();
    
    if (!editor_is_active()) {
        text_draw_centered(translation_for(TR_EDITOR_SCENARIO_EVENT_STATE_UNDEFINED + data.event->state),
            420, 40, 80, FONT_NORMAL_GREEN, color_from_state(data.event->state));
        text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_EXECUTION_COUNT),
            data.event->execution_count, "", 40, 72, FONT_NORMAL_PLAIN, COLOR_BLACK);
        text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_MONTHS_UNTIL_ACTIVE),
            data.event->months_until_active, "", 336, 72, FONT_NORMAL_PLAIN, COLOR_BLACK);
    }

    for (int i = 0; i < NUM_BUTTONS; i++) {
        button_border_draw(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height,
            data.focus_button_id == i + 1);
    }
/****
    int y_offset = EVENT_REPEAT_Y_OFFSET;
    if (scenario_event_can_repeat(data.event) == 0) {
        text_draw_centered(translation_for(TR_EDITOR_SCENARIO_EVENT_DOES_NOT_REPEAT), 32, y_offset + 8,
            SHORT_BUTTON_WIDTH, FONT_NORMAL_GREEN, COLOR_MASK_NONE);
    } else if (data.event->max_number_of_repeats > 0) {
        text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_MAX_NUM_REPEATS),
            data.event->max_number_of_repeats, "", SHORT_BUTTON_LEFT_PADDING + 16, y_offset + 8,
            FONT_NORMAL_GREEN, COLOR_MASK_NONE);
    } else {
        text_draw_centered(translation_for(TR_EDITOR_SCENARIO_EVENT_MAX_NUM_REPEATS), 32, y_offset + 8,
            SHORT_BUTTON_WIDTH, FONT_NORMAL_GREEN, COLOR_MASK_NONE);
        text_draw_centered(translation_for(TR_EDITOR_SCENARIO_EVENT_REPEATS_FOREVER), 240, y_offset + 8,
            SHORT_BUTTON_WIDTH, FONT_NORMAL_GREEN, COLOR_MASK_NONE);
    }

    y_offset += DETAILS_ROW_HEIGHT;
    text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_REPEAT_MIN_MONTHS),
        data.event->repeat_months_min, "", SHORT_BUTTON_LEFT_PADDING + 16, y_offset + 8,
        FONT_NORMAL_GREEN, COLOR_MASK_NONE);

    y_offset += DETAILS_ROW_HEIGHT;
    text_draw_label_and_number(translation_for(TR_EDITOR_SCENARIO_EVENT_REPEAT_MAX_MONTHS),
        data.event->repeat_months_max, "", SHORT_BUTTON_LEFT_PADDING + 16, y_offset + 8,
        FONT_NORMAL_GREEN, COLOR_MASK_NONE);
****/
    grid_box_draw(&conditions_grid_box);
    grid_box_draw(&actions_grid_box);

    graphics_reset_dialog();
}

static void get_focused_grid_box(const mouse *m)
{
    if (!data.focused_grid_box) {
        data.focused_grid_box = &conditions_grid_box;
    }
    if (m->x >= conditions_grid_box.x && m->x < conditions_grid_box.x + conditions_grid_box.width &&
        m->y >= conditions_grid_box.y && m->y < conditions_grid_box.y + conditions_grid_box.height) {
        data.focused_grid_box = &conditions_grid_box;
    } else if (m->x >= actions_grid_box.x && m->x < actions_grid_box.x + actions_grid_box.width &&
        m->y >= actions_grid_box.y && m->y < actions_grid_box.y + actions_grid_box.height) {
        data.focused_grid_box = &actions_grid_box;
    }
}

static void handle_input(const mouse *m, const hotkeys *h)
{
    const mouse *m_dialog = mouse_in_dialog(m);
    get_focused_grid_box(m_dialog);
    if (generic_buttons_handle_mouse(m_dialog, 0, 0, buttons, NUM_BUTTONS, &data.focus_button_id) ||
        grid_box_handle_input(data.focused_grid_box, m_dialog, 1)) {
        return;
    }
    if (input_go_back_requested(m, h)) {
        window_go_back();
    }
}

static void set_amount_max_repeats(int value)
{
    data.event->max_number_of_repeats = value;
}

static void set_amount_repeat_min(int value)
{
    data.event->repeat_months_min = value;
}

static void set_amount_repeat_max(int value)
{
    data.event->repeat_months_max = value;
}

static void button_add_new_condition(const generic_button *button)
{
    condition_types type = CONDITION_TYPE_TIME_PASSED;
    scenario_condition_t *condition = scenario_event_condition_create(array_item(data.event->condition_groups, 0), type);
    condition->parameter1 = 1;
    select_no_conditions();
    window_request_refresh();
}

static void button_add_new_action(const generic_button *button)
{
    action_types type = ACTION_TYPE_ADJUST_FAVOR;
    scenario_event_action_create(data.event, type);
    select_no_actions();
    window_request_refresh();
}

static void button_delete_event(const generic_button *button)
{
    scenario_event_delete(data.event);
    window_go_back();
}

static void button_ok(const generic_button *button)
{
    window_go_back();
}

static void click_condition_button(unsigned int index, unsigned int mouse_x, unsigned int mouse_y)
{
    const condition_list_item *list_item = &data.conditions.list[index];
    if (!list_item->condition) {
        return;
    }

    if (list_item->condition->type != CONDITION_TYPE_UNDEFINED) {
        if (mouse_x < 20) {
            if (data.conditions.selected) {
                data.conditions.selected[index] ^= 1;
                window_request_refresh();
            }
        } else {
            window_editor_scenario_condition_edit_show(list_item->condition);
        }
    }
}

static void click_action_button(unsigned int index, unsigned int mouse_x, unsigned int mouse_y)
{
    if (data.actions.list[index]->type != ACTION_TYPE_UNDEFINED) {
        window_editor_scenario_action_edit_show(data.actions.list[index]);
    }
}

static void button_amount(const generic_button *button)
{
    int param1 = button->parameter1;
    if (param1 == SCENARIO_EVENT_DETAILS_SET_MAX_REPEATS) {
        window_numeric_input_show(0, 0, button, 3, 100000, set_amount_max_repeats);
    } else if (param1 == SCENARIO_EVENT_DETAILS_SET_REPEAT_MIN) {
        window_numeric_input_show(0, 0, button, 3, 1000, set_amount_repeat_min);
    } else if (param1 == SCENARIO_EVENT_DETAILS_SET_REPEAT_MAX) {
        window_numeric_input_show(0, 0, button, 3, 1000, set_amount_repeat_max);
    }
}

static void handle_condition_tooltip(const grid_box_item *item, tooltip_context *c)
{
    if (item->mouse.x < 24) {
        return;
    }
    const condition_list_item *list_item = &data.conditions.list[item->index];
    if (!list_item->condition) {
        return;
    }
    static uint8_t text[MAX_TEXT_LENGTH * 2];
    scenario_events_parameter_data_get_display_string_for_condition(list_item->condition, text, MAX_TEXT_LENGTH * 2);
    if (text_get_width(text, FONT_NORMAL_BLACK) > item->width - item->height - 12) {
        c->precomposed_text = text;
        c->type = TOOLTIP_BUTTON;
    }
}

static void get_tooltip(tooltip_context *c)
{
    grid_box_handle_tooltip(data.focused_grid_box, c);
}

void window_editor_scenario_event_details_show(int event_id)
{
    window_type window = {
        WINDOW_EDITOR_SCENARIO_EVENT_DETAILS,
        draw_background,
        draw_foreground,
        handle_input,
        get_tooltip
    };
    init(event_id);
    window_show(&window);
}
