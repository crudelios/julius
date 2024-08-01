#include "request.h"

#include "building/granary.h"
#include "building/warehouse.h"
#include "city/finance.h"
#include "city/message.h"
#include "city/population.h"
#include "city/ratings.h"
#include "city/resource.h"
#include "core/array.h"
#include "core/log.h"
#include "core/random.h"
#include "game/resource.h"
#include "game/save_version.h"
#include "game/time.h"
#include "game/tutorial.h"
#include "scenario/data.h"

#define REQUESTS_ARRAY_SIZE_STEP 16
#define MAX_ORIGINAL_REQUESTS 20

#define REQUESTS_STRUCT_SIZE_CURRENT ((7 * sizeof(int16_t)) + (6 * sizeof(uint8_t)))

static array(scenario_request) requests;

static void new_request(scenario_request *request, int index)
{
    request->id = index;
    request->deadline_years = REQUESTS_DEFAULT_DEADLINE_YEARS;
    request->favor = REQUESTS_DEFAULT_FAVOUR;
    request->extension_months_to_comply = REQUESTS_DEFAULT_MONTHS_TO_COMPLY;
    request->extension_disfavor = REQUESTS_DEFAULT_EXTENSION_DISFAVOUR;
    request->ignored_disfavor = REQUESTS_DEFAULT_IGNORED_DISFAVOUR;
}

static int request_in_use(const scenario_request *request)
{
    return request->resource != RESOURCE_NONE;
}

static void make_request_visible_and_send_message(scenario_request *request)
{
    request->visible = 1;
    if (city_resource_count(request->resource) >= request->amount) {
        request->can_comply_dialog_shown = 1;
    }
    if (request->resource == RESOURCE_DENARII) {
        city_message_post(1, MESSAGE_CAESAR_REQUESTS_MONEY, request->id, 0);
    } else if (request->resource == RESOURCE_TROOPS) {
        city_message_post(1, MESSAGE_CAESAR_REQUESTS_ARMY, request->id, 0);
    } else {
        city_message_post(1, MESSAGE_CAESAR_REQUESTS_GOODS, request->id, 0);
    }
}

void scenario_request_clear_all(void)
{
    array_init(requests, REQUESTS_ARRAY_SIZE_STEP, new_request, request_in_use);
}

void scenario_request_init(void)
{
    scenario_request *request;
    array_foreach(requests, request) {
        random_generate_next();
        if (request->resource != RESOURCE_NONE) {
            request->month = (random_byte() & 7) + 2;
            request->months_to_comply = 12 * request->deadline_years;
        }
    }
}

int scenario_request_new(void)
{
    scenario_request *request;
    array_new_item(requests, 0, request);
    return request ? request->id : -1;
}

static void process_request(scenario_request *request)
{
    if (!request->resource || request->state > REQUEST_STATE_DISPATCHED_LATE) {
        return;
    }
    int state = request->state;
    if (state == REQUEST_STATE_DISPATCHED || state == REQUEST_STATE_DISPATCHED_LATE) {
        --request->months_to_comply;
        if (request->months_to_comply <= 0) {
            if (state == REQUEST_STATE_DISPATCHED) {
                city_message_post(1, MESSAGE_REQUEST_RECEIVED, request->id, 0);
                city_ratings_change_favor(request->favor);
            } else {
                city_message_post(1, MESSAGE_REQUEST_RECEIVED_LATE, request->id, 0);
                city_ratings_change_favor(request->favor / 2);
            }
            request->state = REQUEST_STATE_RECEIVED;
            request->visible = 0;
        }
        return;
    }

    // normal or overdue
    if (request->visible) {
        --request->months_to_comply;
        if (state == REQUEST_STATE_NORMAL) {
            if (request->months_to_comply == 12) {
                // reminder
                city_message_post(1, MESSAGE_REQUEST_REMINDER, request->id, 0);
            } else if (request->months_to_comply <= 0) {
                city_message_post(1, MESSAGE_REQUEST_REFUSED, request->id, 0);
                request->state = REQUEST_STATE_OVERDUE;
                request->months_to_comply = request->extension_months_to_comply;
                city_ratings_reduce_favor_missed_request(request->extension_disfavor);
            }
        } else if (state == REQUEST_STATE_OVERDUE) {
            if (request->months_to_comply <= 0) {
                city_message_post(1, MESSAGE_REQUEST_REFUSED_OVERDUE, request->id, 0);
                request->state = REQUEST_STATE_IGNORED;
                request->visible = 0;
                city_ratings_reduce_favor_missed_request(request->ignored_disfavor);
            }
        }
        if (!request->can_comply_dialog_shown) {
            resource_type resource = request->resource;
            int resource_amount = city_resource_count(resource);
            if (resource_is_food(resource)) {
                resource_amount += city_resource_count_food_on_granaries(resource) / RESOURCE_ONE_LOAD;
            }
            if (resource_amount >= request->amount) {
                request->can_comply_dialog_shown = 1;
                city_message_post(1, MESSAGE_REQUEST_CAN_COMPLY, request->id, 0);
            }
        }
        return;
    }

    // request is not visible
    int year = scenario.start_year;
    if (!tutorial_adjust_request_year(&year)) {
        return;
    }
    if (game_time_year() == year + request->year &&
        game_time_month() == request->month) {
        make_request_visible_and_send_message(request);
    }
}

void scenario_request_process(void)
{
    array_foreach_callback(requests, process_request);
}

void scenario_request_dispatch(int id)
{
    scenario_request *request = array_item(requests, id);
    if (request->state == REQUEST_STATE_NORMAL) {
        request->state = REQUEST_STATE_DISPATCHED;
    } else {
        request->state = REQUEST_STATE_DISPATCHED_LATE;
    }
    request->months_to_comply = (random_byte() & 3) + 1;
    request->visible = 0;
    int amount = request->amount;
    if (request->resource == RESOURCE_DENARII) {
        city_finance_process_sundry(amount);
    } else if (request->resource == RESOURCE_TROOPS) {
        city_population_remove_for_troop_request(amount);
        building_warehouses_remove_resource(RESOURCE_WEAPONS, amount);
    } else {
        int amount_left = building_warehouses_send_resources_to_rome(request->resource, amount);
        if (amount_left > 0 && resource_is_food(request->resource)) {
            building_granaries_send_resources_to_rome(request->resource, amount_left * RESOURCE_ONE_LOAD);
        }
    }
}

const scenario_request *scenario_request_get(int id)
{
    return array_item(requests, id);
}

void scenario_request_update(int id, const scenario_request *request)
{
    scenario_request *base_request = array_item(requests, id);
    *base_request = *request;
    base_request->id = id;
    array_trim(requests);
    scenario.is_saved = 0;
}

void scenario_request_delete(int id)
{
    scenario_request *request = array_item(requests, id);
    request->resource = RESOURCE_NONE;
    array_trim(requests);
    scenario.is_saved = 0;
}

void scenario_request_remap_resource(void)
{
    scenario_request *request;
    array_foreach(requests, request) {
        request->resource = resource_remap(request->resource);
    }
}

int scenario_request_count_total(void)
{
    return requests.size;
}

int scenario_request_count_visible(void)
{
    int count = 0;
    const scenario_request *request;
    array_foreach(requests, request) {
        if (request->resource && request->visible) {
            count++;
        }
    }
    return count;
}

int scenario_request_foreach_visible(int start_index, void (*callback)(int index, const scenario_request *request))
{
    int index = start_index;
    const scenario_request *request;
    array_foreach(requests, request) {
        if (request->resource && request->visible) {
            callback(index, scenario_request_get(request->id));
            index++;
        }
    }
    return index;
}

const scenario_request *scenario_request_get_visible(int index)
{
    const scenario_request *request;
    array_foreach(requests, request)
    {
        if (request->resource && request->visible && request->state <= 1) {
            if (index == 0) {
                return scenario_request_get(request->id);
            }
            index--;
        }
    }
    return 0;
}

int scenario_request_is_ongoing(int id)
{
    if (id < 0 || id >= requests.size) {
        return 0;
    }

    const scenario_request *request = array_item(requests, id);

    if (!request->resource) {
        return 0;
    }
    
    if (request->visible
        && (request->state == REQUEST_STATE_NORMAL
            || request->state == REQUEST_STATE_OVERDUE)
        ) {
        return 1;
    }

    if (!request->visible
        && (request->state == REQUEST_STATE_DISPATCHED
            || request->state == REQUEST_STATE_DISPATCHED_LATE)
        ) {
        return 1;
    }

    return 0;
}

int scenario_request_force_start(int id)
{
    if (id < 0 || id >= requests.size) {
        return 0;
    }
    
    scenario_request *request = array_item(requests, id);

    if (!request->resource) {
        return 0;
    }

    if (scenario_request_is_ongoing(id)) {
        return 0;
    }

    request->state = REQUEST_STATE_NORMAL;
    request->months_to_comply = 12 * request->deadline_years;
    request->year = game_time_year();
    request->month = game_time_month();
    request->can_comply_dialog_shown = 0;
    
    make_request_visible_and_send_message(request);

    return 1;
}

static void request_save(buffer *list, const scenario_request *request)
{
    buffer_write_i16(list, request->year);
    buffer_write_i16(list, request->resource);
    buffer_write_i16(list, request->amount);
    buffer_write_i16(list, request->deadline_years);

    buffer_write_u8(list, request->can_comply_dialog_shown);

    buffer_write_u8(list, request->favor);
    buffer_write_u8(list, request->month);
    buffer_write_u8(list, request->state);
    buffer_write_u8(list, request->visible);
    buffer_write_u8(list, request->months_to_comply);

    buffer_write_i16(list, request->extension_months_to_comply);
    buffer_write_i16(list, request->extension_disfavor);
    buffer_write_i16(list, request->ignored_disfavor);
}

void scenario_request_save_state(buffer *list)
{
    int32_t struct_size = REQUESTS_STRUCT_SIZE_CURRENT;
    buffer_init_dynamic_piece(list,
        SCENARIO_CURRENT_VERSION,
        requests.size,
        struct_size);

    const scenario_request *request;
    array_foreach(requests, request) {
        request_save(list, request);
    }
}

static void request_load(buffer *list, scenario_request *request)
{
    request->year = buffer_read_i16(list);
    request->resource = resource_remap(buffer_read_i16(list));
    request->amount = buffer_read_i16(list);
    request->deadline_years = buffer_read_i16(list);
    request->can_comply_dialog_shown = buffer_read_u8(list);
    request->favor = buffer_read_u8(list);
    request->month = buffer_read_u8(list);
    request->state = buffer_read_u8(list);
    request->visible = buffer_read_u8(list);
    request->months_to_comply = buffer_read_u8(list);
    request->extension_months_to_comply = buffer_read_i16(list);
    request->extension_disfavor = buffer_read_i16(list);
    request->ignored_disfavor = buffer_read_i16(list);
}

void scenario_request_load_state(buffer *list)
{
    int buffer_size, version, array_size, struct_size;

    buffer_load_dynamic_piece_header_data(list,
        &buffer_size,
        &version,
        &array_size,
        &struct_size);

    if (!array_init(requests, REQUESTS_ARRAY_SIZE_STEP, new_request, request_in_use) ||
        !array_expand(requests, array_size)) {
        log_error("Error creating requests array. The game will probably crash.", 0, 0);
    }

    for (int i = 0; i < array_size; i++) {
        scenario_request *request = array_advance(requests);
        request_load(list, request);
    }

    array_trim(requests);
}

void scenario_request_load_state_old_version(buffer *list, int state_version, requests_old_state_sections section)
{
    // Old savegames had request data split out into multiple chunks,
    // and saved as multiple arrays of variables, rather than an array of struct approach.
    // So here we need to load in a similar section / varaible array manner when dealing with old versions.
    if (state_version <= SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        scenario_request *request;
        if (section == REQUESTS_OLD_STATE_SECTIONS_TARGET) {
            if (!array_init(requests, REQUESTS_ARRAY_SIZE_STEP, new_request, request_in_use) ||
                !array_expand(requests, MAX_ORIGINAL_REQUESTS)) {
                log_error("Error creating requests array. The game will probably crash.", 0, 0);
            }
            array_foreach(requests, request) {
                request->year = buffer_read_i16(list);
            }
            array_foreach(requests, request) {
                request->resource = buffer_read_i16(list);
            }
            array_foreach(requests, request) {
                request->amount = buffer_read_i16(list);
            }
            array_foreach(requests, request) {
                request->deadline_years = buffer_read_i16(list);
            }
            array_trim(requests);
        } else if (section == REQUESTS_OLD_STATE_SECTIONS_CAN_COMPLY) {
            array_foreach(requests, request) {
                request->can_comply_dialog_shown = buffer_read_u8(list);
            }
        } else if (section == REQUESTS_OLD_STATE_SECTIONS_FAVOR_REWARD) {
            array_foreach(requests, request) {
                request->favor = buffer_read_u8(list);
            }
        } else if (section == REQUESTS_OLD_STATE_SECTIONS_ONGOING_INFO) {
            array_foreach(requests, request) {
                request->month = buffer_read_u8(list);
            }
            array_foreach(requests, request) {
                request->state = buffer_read_u8(list);
            }
            array_foreach(requests, request) {
                request->visible = buffer_read_u8(list);
            }
            array_foreach(requests, request) {
                request->months_to_comply = buffer_read_u8(list);
            }
            // Setup any default values we need for values that didn't exist in old versions.
            array_foreach(requests, request) {
                request->extension_months_to_comply = REQUESTS_DEFAULT_MONTHS_TO_COMPLY;
                request->extension_disfavor = REQUESTS_DEFAULT_EXTENSION_DISFAVOUR;
                request->ignored_disfavor = REQUESTS_DEFAULT_IGNORED_DISFAVOUR;
            }
        }
    }
}
