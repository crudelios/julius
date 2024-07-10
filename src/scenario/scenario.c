#include "scenario.h"

#include "campaign/campaign.h"
#include "city/resource.h"
#include "core/string.h"
#include "empire/city.h"
#include "empire/trade_route.h"
#include "game/difficulty.h"
#include "game/save_version.h"
#include "game/settings.h"
#include "scenario/data.h"
#include "scenario/request.h"

#include <string.h>

struct scenario_t scenario;

static struct {
    size_t size;
    size_t start_info;
    size_t old_filler_1;
    size_t invasions_part1;
    size_t start_funds_and_enemy_id;
    size_t map_size;
    size_t briefing;
    size_t old_filler_2;
    size_t image;
    size_t herds;
    size_t demand_changes_part1;
    size_t price_changes_part1;
    size_t gladiator_revolt;
    size_t emperor_change;
    size_t random_events;
    size_t fishing;
    size_t old_filler_3;
    size_t invasions_part2;
    size_t old_filler_4;
    size_t rome_wheat;
    size_t allowed_buildings;
    size_t win_criteria;
    size_t map_points;
    size_t invasion_points;
    size_t misc;
    size_t introduction;
    size_t custom_variables;
    size_t custom_name;
    size_t end;

    // Cache info
    savegame_version_t version;
} buffer_offsets;

int scenario_is_saved(void)
{
    return scenario.is_saved;
}

static void calculate_buffer_offsets(int scenario_version)
{
    if (buffer_offsets.version && buffer_offsets.version == scenario_version) {
        return;
    }
    memset(&buffer_offsets, 0, sizeof(buffer_offsets));
    size_t next_start_offset = 0;

    if (scenario_version > SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_offsets.size = next_start_offset;
        next_start_offset = buffer_offsets.size + sizeof(int32_t);
    }

    buffer_offsets.start_info = next_start_offset;
    next_start_offset = buffer_offsets.start_info + 14;

    if (scenario_version <= SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_offsets.old_filler_1 = next_start_offset;
        next_start_offset = buffer_offsets.old_filler_1 + 160;
    }

    if (scenario_version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_offsets.invasions_part1 = next_start_offset;
        next_start_offset = buffer_offsets.invasions_part1 + (MAX_INVASIONS * 10);
    }

    buffer_offsets.start_funds_and_enemy_id = next_start_offset;
    next_start_offset = buffer_offsets.start_funds_and_enemy_id + 14;

    buffer_offsets.map_size = next_start_offset;
    next_start_offset = buffer_offsets.map_size + 16;

    buffer_offsets.briefing = next_start_offset;
    next_start_offset = buffer_offsets.briefing + MAX_BRIEF_DESCRIPTION + MAX_BRIEFING;

    if (scenario_version <= SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_offsets.old_filler_2 = next_start_offset;
        next_start_offset = buffer_offsets.old_filler_2 + 20;
    }

    buffer_offsets.image = next_start_offset;
    next_start_offset = buffer_offsets.image + 6;

    buffer_offsets.herds = next_start_offset;
    next_start_offset = buffer_offsets.herds + (MAX_HERD_POINTS * 4);

    if (scenario_version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_offsets.demand_changes_part1 = next_start_offset;
        next_start_offset = buffer_offsets.demand_changes_part1 + (MAX_DEMAND_CHANGES * 9);

        buffer_offsets.price_changes_part1 = next_start_offset;
        next_start_offset = buffer_offsets.price_changes_part1 + (MAX_PRICE_CHANGES * 6);
    }

    buffer_offsets.gladiator_revolt = next_start_offset;
    next_start_offset = buffer_offsets.gladiator_revolt + 8;

    buffer_offsets.emperor_change = next_start_offset;
    next_start_offset = buffer_offsets.emperor_change + 8;

    buffer_offsets.random_events = next_start_offset;
    next_start_offset = buffer_offsets.random_events + 36;

    buffer_offsets.fishing = next_start_offset;
    next_start_offset = buffer_offsets.fishing + (MAX_FISH_POINTS * 4);

    if (scenario_version <= SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_offsets.old_filler_3 = next_start_offset;
        next_start_offset = buffer_offsets.old_filler_3 + 20;
    }

    if (scenario_version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_offsets.invasions_part2 = next_start_offset;
        next_start_offset = buffer_offsets.invasions_part2 + (MAX_INVASIONS * 1);
    }

    if (scenario_version <= SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_offsets.old_filler_4 = next_start_offset;
        next_start_offset = buffer_offsets.old_filler_4 + 80;
    }

    buffer_offsets.rome_wheat = next_start_offset;
    next_start_offset = buffer_offsets.rome_wheat + 4;

    if (scenario_version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_offsets.allowed_buildings = next_start_offset;
        next_start_offset = buffer_offsets.allowed_buildings + (MAX_ALLOWED_BUILDINGS * 2);
    }

    buffer_offsets.win_criteria = next_start_offset;
    next_start_offset = buffer_offsets.win_criteria + 52;

    buffer_offsets.map_points = next_start_offset;
    next_start_offset = buffer_offsets.map_points + 12;

    buffer_offsets.invasion_points = next_start_offset;
    next_start_offset = buffer_offsets.invasion_points + (MAX_INVASION_POINTS * 4);

    buffer_offsets.misc = next_start_offset;
    next_start_offset = buffer_offsets.misc + 51;

    if (scenario_version > SCENARIO_LAST_NO_CUSTOM_MESSAGES) {
        buffer_offsets.introduction = next_start_offset;
        next_start_offset = buffer_offsets.introduction + 4;
    }

    if (scenario_version > SCENARIO_LAST_NO_CUSTOM_VARIABLES && scenario_version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_offsets.custom_variables = next_start_offset;
        next_start_offset = buffer_offsets.custom_variables + (9 * MAX_CUSTOM_VARIABLES);
    }

    buffer_offsets.custom_name = next_start_offset;
    next_start_offset = buffer_offsets.custom_name +
        (scenario_version > SCENARIO_LAST_WRONG_END_OFFSET ? sizeof(scenario.empire.custom_name) : 50);

    buffer_offsets.end = next_start_offset + 1;

    buffer_offsets.version = scenario_version;
}

int scenario_get_state_buffer_size_by_savegame_version(int savegame_version)
{
    if (savegame_version <= SAVE_GAME_LAST_UNVERSIONED_SCENARIOS) {
        return 1720;
    } else if (savegame_version <= SAVE_GAME_LAST_NO_EXTENDED_REQUESTS) {
        calculate_buffer_offsets(SCENARIO_LAST_NO_EXTENDED_REQUESTS);
    } else if (savegame_version <= SAVE_GAME_LAST_NO_CUSTOM_MESSAGES) {
        calculate_buffer_offsets(SCENARIO_LAST_NO_CUSTOM_MESSAGES);
    } else if (savegame_version <= SAVE_GAME_LAST_NO_CUSTOM_VARIABLES) {
        calculate_buffer_offsets(SCENARIO_LAST_NO_CUSTOM_VARIABLES);
    } else if (savegame_version <= SAVE_GAME_LAST_WRONG_SCENARIO_END_OFFSET) {
        calculate_buffer_offsets(SCENARIO_LAST_WRONG_END_OFFSET);
    } else if (savegame_version <= SAVE_GAME_LAST_STATIC_SCENARIO_ORIGINAL_DATA) {
        calculate_buffer_offsets(SCENARIO_LAST_STATIC_ORIGINAL_DATA);
    } else {
        calculate_buffer_offsets(SCENARIO_CURRENT_VERSION);
    }
    return buffer_offsets.end;
}

int scenario_get_state_buffer_size_by_scenario_version(int scenario_version)
{
    if (scenario_version <= SCENARIO_LAST_UNVERSIONED) {
        return 1720;
    } else if (scenario_version <= SCENARIO_LAST_NO_WAGE_LIMITS) {
        return 1830;
    } else {
        calculate_buffer_offsets(scenario_version);
        return buffer_offsets.end;
    }
}

void scenario_save_state(buffer *buf)
{
    int buf_size = scenario_get_state_buffer_size_by_scenario_version(SCENARIO_CURRENT_VERSION);
    uint8_t *buf_data = malloc(buf_size);
    buffer_init(buf, buf_data, buf_size);

    // size
    buffer_write_i32(buf, buf_size);

    buffer_write_i16(buf, scenario.start_year);
    buffer_write_i16(buf, 0);
    buffer_write_i16(buf, scenario.empire.id);
    buffer_skip(buf, 8);
/***
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_i16(buf, scenario.invasions[i].year);
    }
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_i16(buf, scenario.invasions[i].type);
    }
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_i16(buf, scenario.invasions[i].amount);
    }
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_i16(buf, scenario.invasions[i].from);
    }
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_i16(buf, scenario.invasions[i].attack_type);
    }
***/
    buffer_write_i16(buf, 0);
    buffer_write_i32(buf, scenario.initial_funds);
    buffer_write_i16(buf, scenario.enemy_id);
    buffer_write_i32(buf, scenario.victory_custom_message_id);
    buffer_write_u16(buf, scenario.caesar_salary);

    buffer_write_i32(buf, scenario.map.width);
    buffer_write_i32(buf, scenario.map.height);
    buffer_write_i32(buf, scenario.map.grid_start);
    buffer_write_i32(buf, scenario.map.grid_border_size);

    buffer_write_raw(buf, scenario.brief_description, MAX_BRIEF_DESCRIPTION);
    buffer_write_raw(buf, scenario.briefing, MAX_BRIEFING);

    buffer_write_i16(buf, scenario.image_id);
    buffer_write_i16(buf, scenario.is_open_play);
    buffer_write_i16(buf, scenario.player_rank);

    for (int i = 0; i < MAX_HERD_POINTS; i++) {
        buffer_write_i16(buf, scenario.herd_points[i].x);
    }
    for (int i = 0; i < MAX_HERD_POINTS; i++) {
        buffer_write_i16(buf, scenario.herd_points[i].y);
    }
/***
    for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
        buffer_write_i16(buf, scenario.demand_changes[i].year);
    }
    for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
        buffer_write_u8(buf, scenario.demand_changes[i].month);
    }
    for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
        buffer_write_u8(buf, scenario.demand_changes[i].resource);
    }
    for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
        buffer_write_u8(buf, scenario.demand_changes[i].route_id);
    }
    for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
        buffer_write_i32(buf, scenario.demand_changes[i].amount);
    }

    for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
        buffer_write_i16(buf, scenario.price_changes[i].year);
    }
    for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
        buffer_write_u8(buf, scenario.price_changes[i].month);
    }
    for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
        buffer_write_u8(buf, scenario.price_changes[i].resource);
    }
    for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
        buffer_write_u8(buf, scenario.price_changes[i].amount);
    }
    for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
        buffer_write_u8(buf, scenario.price_changes[i].is_rise);
    }
***/
    buffer_write_i32(buf, scenario.gladiator_revolt.enabled);
    buffer_write_i32(buf, scenario.gladiator_revolt.year);
    buffer_write_i32(buf, scenario.emperor_change.enabled);
    buffer_write_i32(buf, scenario.emperor_change.year);

    buffer_write_i32(buf, scenario.random_events.sea_trade_problem);
    buffer_write_i32(buf, scenario.random_events.land_trade_problem);
    buffer_write_i32(buf, scenario.random_events.raise_wages);
    buffer_write_i32(buf, scenario.random_events.max_wages);
    buffer_write_i32(buf, scenario.random_events.lower_wages);
    buffer_write_i32(buf, scenario.random_events.min_wages);
    buffer_write_i32(buf, scenario.random_events.contaminated_water);
    buffer_write_i32(buf, scenario.random_events.iron_mine_collapse);
    buffer_write_i32(buf, scenario.random_events.clay_pit_flooded);

    for (int i = 0; i < MAX_FISH_POINTS; i++) {
        buffer_write_i16(buf, scenario.fishing_points[i].x);
    }
    for (int i = 0; i < MAX_FISH_POINTS; i++) {
        buffer_write_i16(buf, scenario.fishing_points[i].y);
    }
/***
    for (int i = 0; i < MAX_INVASIONS; i++) {
        buffer_write_u8(buf, scenario.invasions[i].month);
    }
***/
    buffer_write_i32(buf, scenario.rome_supplies_wheat);
/***
    for (int i = 0; i < MAX_ALLOWED_BUILDINGS; i++) {
        buffer_write_i16(buf, scenario.allowed_buildings[i]);
    }
***/
    buffer_write_i32(buf, scenario.win_criteria.culture.goal);
    buffer_write_i32(buf, scenario.win_criteria.prosperity.goal);
    buffer_write_i32(buf, scenario.win_criteria.peace.goal);
    buffer_write_i32(buf, scenario.win_criteria.favor.goal);
    buffer_write_u8(buf, scenario.win_criteria.culture.enabled);
    buffer_write_u8(buf, scenario.win_criteria.prosperity.enabled);
    buffer_write_u8(buf, scenario.win_criteria.peace.enabled);
    buffer_write_u8(buf, scenario.win_criteria.favor.enabled);
    buffer_write_i32(buf, scenario.win_criteria.time_limit.enabled);
    buffer_write_i32(buf, scenario.win_criteria.time_limit.years);
    buffer_write_i32(buf, scenario.win_criteria.survival_time.enabled);
    buffer_write_i32(buf, scenario.win_criteria.survival_time.years);

    buffer_write_i32(buf, scenario.earthquake.severity);
    buffer_write_i32(buf, scenario.earthquake.year);

    buffer_write_i32(buf, scenario.win_criteria.population.enabled);
    buffer_write_i32(buf, scenario.win_criteria.population.goal);

    buffer_write_i16(buf, scenario.earthquake_point.x);
    buffer_write_i16(buf, scenario.earthquake_point.y);
    buffer_write_i16(buf, scenario.entry_point.x);
    buffer_write_i16(buf, scenario.entry_point.y);
    buffer_write_i16(buf, scenario.exit_point.x);
    buffer_write_i16(buf, scenario.exit_point.y);

    for (int i = 0; i < MAX_INVASION_POINTS; i++) {
        buffer_write_i16(buf, scenario.invasion_points[i].x);
    }
    for (int i = 0; i < MAX_INVASION_POINTS; i++) {
        buffer_write_i16(buf, scenario.invasion_points[i].y);
    }

    buffer_write_i16(buf, scenario.river_entry_point.x);
    buffer_write_i16(buf, scenario.river_entry_point.y);
    buffer_write_i16(buf, scenario.river_exit_point.x);
    buffer_write_i16(buf, scenario.river_exit_point.y);

    buffer_write_i32(buf, scenario.rescue_loan);
    buffer_write_i32(buf, scenario.win_criteria.milestone25_year);
    buffer_write_i32(buf, scenario.win_criteria.milestone50_year);
    buffer_write_i32(buf, scenario.win_criteria.milestone75_year);

    buffer_write_i32(buf, scenario.native_images.hut);
    buffer_write_i32(buf, scenario.native_images.meeting);
    buffer_write_i32(buf, scenario.native_images.crops);

    buffer_write_u8(buf, scenario.climate);
    buffer_write_u8(buf, scenario.flotsam_enabled);

    buffer_write_i16(buf, 0);

    buffer_write_i32(buf, scenario.empire.is_expanded);
    buffer_write_i32(buf, scenario.empire.expansion_year);

    buffer_write_u8(buf, scenario.empire.distant_battle_roman_travel_months);
    buffer_write_u8(buf, scenario.empire.distant_battle_enemy_travel_months);
    buffer_write_u8(buf, scenario.open_play_scenario_id);

    buffer_write_i32(buf, scenario.intro_custom_message_id);
/***
    for (int i = 0; i < MAX_CUSTOM_VARIABLES; i++) {
        buffer_write_u8(buf, scenario.custom_variables[i].in_use);
        buffer_write_i32(buf, scenario.custom_variables[i].value);
        if (scenario.custom_variables[i].linked_uid) {
            buffer_write_i32(buf, scenario.custom_variables[i].linked_uid->id);
        } else {
            buffer_write_i32(buf, 0);
        }
    }
***/

    buffer_write_raw(buf, scenario.empire.custom_name, sizeof(scenario.empire.custom_name));
    buffer_write_u8(buf, 0);
}

void scenario_load_state(buffer *buf, int version)
{
    calculate_buffer_offsets(version);
    if (version > SCENARIO_LAST_NO_EXTENDED_REQUESTS) {
        buffer_read_i32(buf);
    }

    scenario.start_year = buffer_read_i16(buf);
    buffer_skip(buf, 2);
    scenario.empire.id = buffer_read_i16(buf);
    buffer_skip(buf, 8);

    scenario_request_load_state_old_version(buf, version, REQUESTS_OLD_STATE_SECTIONS_TARGET);

    if (version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].year = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].type = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].amount = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].from = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].attack_type = buffer_read_i16(buf);
        }
    }

    buffer_skip(buf, 2);
    scenario.initial_funds = buffer_read_i32(buf);
    scenario.enemy_id = buffer_read_i16(buf);
    scenario.victory_custom_message_id = buffer_read_i32(buf);
    scenario.caesar_salary = buffer_read_u16(buf);

    scenario.map.width = buffer_read_i32(buf);
    scenario.map.height = buffer_read_i32(buf);
    scenario.map.grid_start = buffer_read_i32(buf);
    scenario.map.grid_border_size = buffer_read_i32(buf);

    buffer_read_raw(buf, scenario.brief_description, MAX_BRIEF_DESCRIPTION);
    buffer_read_raw(buf, scenario.briefing, MAX_BRIEFING);

    scenario_request_load_state_old_version(buf, version, REQUESTS_OLD_STATE_SECTIONS_CAN_COMPLY);

    scenario.image_id = buffer_read_i16(buf);
    scenario.is_open_play = buffer_read_i16(buf);
    scenario.player_rank = buffer_read_i16(buf);

    for (int i = 0; i < MAX_HERD_POINTS; i++) {
        scenario.herd_points[i].x = buffer_read_i16(buf);
    }
    for (int i = 0; i < MAX_HERD_POINTS; i++) {
        scenario.herd_points[i].y = buffer_read_i16(buf);
    }

    if (version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
            scenario.demand_changes[i].year = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
            scenario.demand_changes[i].month = buffer_read_u8(buf);
        }
        for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
            scenario.demand_changes[i].resource = buffer_read_u8(buf);
        }
        for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
            scenario.demand_changes[i].route_id = buffer_read_u8(buf);
        }
        if (version <= SCENARIO_LAST_UNVERSIONED) {
            for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
                int is_rise = buffer_read_u8(buf);
                int amount = is_rise ? DEMAND_CHANGE_LEGACY_IS_RISE : DEMAND_CHANGE_LEGACY_IS_FALL;
                scenario.demand_changes[i].amount = amount;
            }
        } else {
            for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
                scenario.demand_changes[i].amount = buffer_read_i32(buf);
            }
        }

        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].year = buffer_read_i16(buf);
        }
        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].month = buffer_read_u8(buf);
        }
        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].resource = buffer_read_u8(buf);
        }
        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].amount = buffer_read_u8(buf);
        }
        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].is_rise = buffer_read_u8(buf);
        }
    }

    scenario.gladiator_revolt.enabled = buffer_read_i32(buf);
    scenario.gladiator_revolt.year = buffer_read_i32(buf);
    scenario.emperor_change.enabled = buffer_read_i32(buf);
    scenario.emperor_change.year = buffer_read_i32(buf);

    scenario.random_events.sea_trade_problem = buffer_read_i32(buf);
    scenario.random_events.land_trade_problem = buffer_read_i32(buf);
    scenario.random_events.raise_wages = buffer_read_i32(buf);
    if (version > SCENARIO_LAST_NO_WAGE_LIMITS) {
        scenario.random_events.max_wages = buffer_read_i32(buf);
    }
    if (!scenario.random_events.max_wages) {
        scenario.random_events.max_wages = 45;
    }
    scenario.random_events.lower_wages = buffer_read_i32(buf);
    if (version > SCENARIO_LAST_NO_WAGE_LIMITS) {
        scenario.random_events.min_wages = buffer_read_i32(buf);
    }
    if (!scenario.random_events.min_wages) {
        scenario.random_events.min_wages = 5;
    }
    scenario.random_events.contaminated_water = buffer_read_i32(buf);
    scenario.random_events.iron_mine_collapse = buffer_read_i32(buf);
    scenario.random_events.clay_pit_flooded = buffer_read_i32(buf);

    for (int i = 0; i < MAX_FISH_POINTS; i++) {
        scenario.fishing_points[i].x = buffer_read_i16(buf);
    }
    for (int i = 0; i < MAX_FISH_POINTS; i++) {
        scenario.fishing_points[i].y = buffer_read_i16(buf);
    }

    scenario_request_load_state_old_version(buf, version, REQUESTS_OLD_STATE_SECTIONS_FAVOR_REWARD);

    if (version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        for (int i = 0; i < MAX_INVASIONS; i++) {
            scenario.invasions[i].month = buffer_read_u8(buf);
        }
    }

    scenario_request_load_state_old_version(buf, version, REQUESTS_OLD_STATE_SECTIONS_ONGOING_INFO);

    scenario.rome_supplies_wheat = buffer_read_i32(buf);

    if (version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        for (int i = 0; i < MAX_ALLOWED_BUILDINGS; i++) {
            scenario.allowed_buildings[i] = buffer_read_i16(buf);
        }
    }

    scenario.win_criteria.culture.goal = buffer_read_i32(buf);
    scenario.win_criteria.prosperity.goal = buffer_read_i32(buf);
    scenario.win_criteria.peace.goal = buffer_read_i32(buf);
    scenario.win_criteria.favor.goal = buffer_read_i32(buf);
    scenario.win_criteria.culture.enabled = buffer_read_u8(buf);
    scenario.win_criteria.prosperity.enabled = buffer_read_u8(buf);
    scenario.win_criteria.peace.enabled = buffer_read_u8(buf);
    scenario.win_criteria.favor.enabled = buffer_read_u8(buf);
    scenario.win_criteria.time_limit.enabled = buffer_read_i32(buf);
    scenario.win_criteria.time_limit.years = buffer_read_i32(buf);
    scenario.win_criteria.survival_time.enabled = buffer_read_i32(buf);
    scenario.win_criteria.survival_time.years = buffer_read_i32(buf);

    scenario.earthquake.severity = buffer_read_i32(buf);
    scenario.earthquake.year = buffer_read_i32(buf);

    scenario.win_criteria.population.enabled = buffer_read_i32(buf);
    scenario.win_criteria.population.goal = buffer_read_i32(buf);

    scenario.earthquake_point.x = buffer_read_i16(buf);
    scenario.earthquake_point.y = buffer_read_i16(buf);
    scenario.entry_point.x = buffer_read_i16(buf);
    scenario.entry_point.y = buffer_read_i16(buf);
    scenario.exit_point.x = buffer_read_i16(buf);
    scenario.exit_point.y = buffer_read_i16(buf);

    for (int i = 0; i < MAX_INVASION_POINTS; i++) {
        scenario.invasion_points[i].x = buffer_read_i16(buf);
    }
    for (int i = 0; i < MAX_INVASION_POINTS; i++) {
        scenario.invasion_points[i].y = buffer_read_i16(buf);
    }

    scenario.river_entry_point.x = buffer_read_i16(buf);
    scenario.river_entry_point.y = buffer_read_i16(buf);
    scenario.river_exit_point.x = buffer_read_i16(buf);
    scenario.river_exit_point.y = buffer_read_i16(buf);

    scenario.rescue_loan = buffer_read_i32(buf);
    scenario.win_criteria.milestone25_year = buffer_read_i32(buf);
    scenario.win_criteria.milestone50_year = buffer_read_i32(buf);
    scenario.win_criteria.milestone75_year = buffer_read_i32(buf);

    scenario.native_images.hut = buffer_read_i32(buf);
    scenario.native_images.meeting = buffer_read_i32(buf);
    scenario.native_images.crops = buffer_read_i32(buf);

    scenario.climate = buffer_read_u8(buf);
    scenario.flotsam_enabled = buffer_read_u8(buf);

    buffer_skip(buf, 2);

    scenario.empire.is_expanded = buffer_read_i32(buf);
    scenario.empire.expansion_year = buffer_read_i32(buf);

    scenario.empire.distant_battle_roman_travel_months = buffer_read_u8(buf);
    scenario.empire.distant_battle_enemy_travel_months = buffer_read_u8(buf);
    scenario.open_play_scenario_id = buffer_read_u8(buf);

    scenario.intro_custom_message_id = 0;
    if (version > SCENARIO_LAST_NO_CUSTOM_MESSAGES) {
        scenario.intro_custom_message_id = buffer_read_i32(buf);
    }

    if (version > SCENARIO_LAST_NO_CUSTOM_VARIABLES && version <= SCENARIO_LAST_STATIC_ORIGINAL_DATA) {
        buffer_set(buf, buffer_offsets.custom_variables);
        for (int i = 0; i < MAX_CUSTOM_VARIABLES; i++) {
            scenario.custom_variables[i].id = i;
            scenario.custom_variables[i].in_use = buffer_read_u8(buf);
            scenario.custom_variables[i].value = buffer_read_i32(buf);
            int uid_text_id = buffer_read_i32(buf);
            if (uid_text_id) {
                scenario.custom_variables[i].linked_uid = message_media_text_blob_get_entry(uid_text_id);
            } else {
                scenario.custom_variables[i].linked_uid = 0;
            }
        }
    } else {
        scenario_custom_variable_clear();
    }

    buffer_set(buf, buffer_offsets.custom_name);
    if (version > SCENARIO_LAST_UNVERSIONED) {
        buffer_read_raw(buf, scenario.empire.custom_name, sizeof(scenario.empire.custom_name));
    }
    buffer_skip(buf, 1);

    // We can only remap resources at the end of the scenario load as the remapping relies on the allowed building list
    // being loaded, otherwise on some edge cases changes to meat may affect fish instead
    if (resource_mapping_get_version() < RESOURCE_CURRENT_VERSION) {
        for (int i = 0; i < MAX_REQUESTS; i++) {
            scenario.requests[i].resource = resource_remap(scenario.requests[i].resource);
        }
        for (int i = 0; i < MAX_DEMAND_CHANGES; i++) {
            scenario.demand_changes[i].resource = resource_remap(scenario.demand_changes[i].resource);
        }
        for (int i = 0; i < MAX_PRICE_CHANGES; i++) {
            scenario.price_changes[i].resource = resource_remap(scenario.price_changes[i].resource);
        }
    }
    scenario.is_saved = 1;
}

void scenario_description_from_buffer(buffer *buf, uint8_t *description, int version)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.briefing);
    buffer_read_raw(buf, description, MAX_BRIEF_DESCRIPTION);
}

int scenario_climate_from_buffer(buffer *buf, int version)
{
    calculate_buffer_offsets(version);
    if (version <= SCENARIO_LAST_UNVERSIONED) {
        buffer_set(buf, 1704);
    } else if (version <= SCENARIO_LAST_NO_WAGE_LIMITS) {
        buffer_set(buf, 1764);
    } else {
        buffer_set(buf, buffer_offsets.misc + 36);
    }
    return buffer_read_u8(buf);
}

int scenario_invasions_from_buffer(buffer *buf, int version)
{
    int num_invasions = 0;

    if (version > SCENARIO_LAST_STATIC_ORIGINAL_DATA) {

    } else {
        calculate_buffer_offsets(version);
        buffer_set(buf, buffer_offsets.invasions_part1 + (MAX_INVASIONS * 2));
        for (int i = 0; i < MAX_INVASIONS; i++) {
            if (buffer_read_i16(buf)) {
                num_invasions++;
            }
        }
    }
    return num_invasions;
}

int scenario_image_id_from_buffer(buffer *buf, int version)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.image);
    return buffer_read_i16(buf);
}

int scenario_rank_from_buffer(buffer *buf, int version)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.image + 4);
    return buffer_read_i16(buf);
}

void scenario_open_play_info_from_buffer(buffer *buf, int version, int *is_open_play, int *open_play_id)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.image + 2);
    *is_open_play = buffer_read_i16(buf);

    if (version <= SCENARIO_LAST_UNVERSIONED) {
        buffer_set(buf, 1718);
    } else if (version <= SCENARIO_LAST_NO_WAGE_LIMITS) {
        buffer_set(buf, 1778);
    } else {
        buffer_set(buf, buffer_offsets.misc + 50);
    }
    *open_play_id = buffer_read_u8(buf);
}

int scenario_start_year_from_buffer(buffer *buf, int version)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.start_info);
    return buffer_read_i16(buf);
}

void scenario_objectives_from_buffer(buffer *buf, int version, scenario_win_criteria *win_criteria)
{
    calculate_buffer_offsets(version);
    if (version <= SCENARIO_LAST_UNVERSIONED) {
        buffer_set(buf, 1572);
    } else if (version <= SCENARIO_LAST_NO_WAGE_LIMITS) {
        buffer_set(buf, 1632);
    } else {
        buffer_set(buf, buffer_offsets.win_criteria);
    }
    win_criteria->culture.goal = buffer_read_i32(buf);
    win_criteria->prosperity.goal = buffer_read_i32(buf);
    win_criteria->peace.goal = buffer_read_i32(buf);
    win_criteria->favor.goal = buffer_read_i32(buf);
    win_criteria->culture.enabled = buffer_read_u8(buf);
    win_criteria->prosperity.enabled = buffer_read_u8(buf);
    win_criteria->peace.enabled = buffer_read_u8(buf);
    win_criteria->favor.enabled = buffer_read_u8(buf);
    win_criteria->time_limit.enabled = buffer_read_i32(buf);
    win_criteria->time_limit.years = buffer_read_i32(buf);
    win_criteria->survival_time.enabled = buffer_read_i32(buf);
    win_criteria->survival_time.years = buffer_read_i32(buf);
    buffer_skip(buf, 8);
    win_criteria->population.enabled = buffer_read_i32(buf);
    win_criteria->population.goal = buffer_read_i32(buf);
}

void scenario_map_data_from_buffer(buffer *buf, int *width, int *height, int *grid_start, int *grid_border_size, int version)
{
    calculate_buffer_offsets(version);
    buffer_set(buf, buffer_offsets.map_size);
    *width = buffer_read_i32(buf);
    *height = buffer_read_i32(buf);
    *grid_start = buffer_read_i32(buf);
    *grid_border_size = buffer_read_i32(buf);
}

void scenario_settings_init(void)
{
    scenario.campaign.mission = 0;
    scenario.campaign.rank = 0;
    scenario.campaign.player_name[0] = 0;
    scenario.settings.is_custom = 0;
    scenario.settings.starting_favor = difficulty_starting_favor();
    scenario.settings.starting_personal_savings = 0;
}

void scenario_settings_init_mission(void)
{
    scenario.settings.starting_favor = difficulty_starting_favor();
    scenario.settings.starting_personal_savings =
        setting_personal_savings_for_mission(scenario.campaign.rank);
}

void scenario_settings_init_favor(void)
{
    scenario.settings.starting_favor = difficulty_starting_favor();
}

void scenario_unlock_all_buildings(void)
{
    for (int i = 0; i < MAX_ALLOWED_BUILDINGS; i++) {
        scenario.allowed_buildings[i] = 1;
    }
}


void scenario_settings_save_state(buffer *part1, buffer *part2, buffer *part3,
    buffer *player_name, buffer *scenario_name, buffer *campaign_name)
{
    buffer_write_i32(part1, scenario.campaign.mission);

    buffer_write_i32(part2, scenario.settings.starting_favor);
    buffer_write_i32(part2, scenario.settings.starting_personal_savings);
    buffer_write_i32(part2, scenario.campaign.rank);

    buffer_write_i32(part3, scenario.settings.is_custom);

    for (int i = 0; i < MAX_PLAYER_NAME; i++) {
        buffer_write_u8(player_name, 0);
    }
    buffer_write_raw(player_name, scenario.settings.player_name, MAX_PLAYER_NAME);
    buffer_write_raw(scenario_name, scenario.scenario_name, MAX_SCENARIO_NAME);

    int campaign_name_length = (int) strlen(campaign_get_name()) + 1;
    int buf_size = (int) sizeof(int32_t) + campaign_name_length;
    uint8_t *buf_data = malloc(buf_size);

    buffer_init(campaign_name, buf_data, buf_size);
    buffer_write_i32(campaign_name, campaign_name_length);
    buffer_write_raw(campaign_name, campaign_get_name(), campaign_name_length);
}

void scenario_settings_load_state(
    buffer *part1, buffer *part2, buffer *part3, buffer *player_name, buffer *scenario_name, buffer *campaign_name)
{
    scenario.campaign.mission = buffer_read_i32(part1);

    scenario.settings.starting_favor = buffer_read_i32(part2);
    scenario.settings.starting_personal_savings = buffer_read_i32(part2);
    scenario.campaign.rank = buffer_read_i32(part2);

    scenario.settings.is_custom = buffer_read_i32(part3);

    buffer_skip(player_name, MAX_PLAYER_NAME);
    buffer_read_raw(player_name, scenario.settings.player_name, MAX_PLAYER_NAME);
    buffer_read_raw(scenario_name, scenario.scenario_name, MAX_SCENARIO_NAME);

    if (campaign_name && !campaign_is_active()) {
        int campaign_name_length = buffer_read_i32(campaign_name);
        char name[FILE_NAME_MAX];
        buffer_read_raw(campaign_name, name, campaign_name_length);
        campaign_load(name);
    }
}

custom_variable_t *scenario_custom_variable_create(const uint8_t *uid, int initial_value)
{
    int id = -1;
    for (int i = 1; i < MAX_CUSTOM_VARIABLES; i++) {
        if (scenario.custom_variables[i].in_use == 0) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        return 0;
    }

    scenario.custom_variables[id].id = id;
    scenario.custom_variables[id].in_use = 1;
    scenario.custom_variables[id].value = initial_value;
    if (scenario.custom_variables[id].linked_uid) {
        message_media_text_blob_mark_entry_as_unused(scenario.custom_variables[id].linked_uid);
        scenario.custom_variables[id].linked_uid = 0;
    }
    scenario.custom_variables[id].linked_uid = message_media_text_blob_add(uid);

    return &scenario.custom_variables[id];
}

void scenario_custom_variable_rename(int id, const uint8_t *name)
{
    if (scenario.custom_variables[id].linked_uid) {
        message_media_text_blob_mark_entry_as_unused(scenario.custom_variables[id].linked_uid);
        scenario.custom_variables[id].linked_uid = 0;
        scenario.custom_variables[id].in_use = 0;
    }
    if (*name) {
        scenario.custom_variables[id].linked_uid = message_media_text_blob_add(name);
        scenario.custom_variables[id].in_use = 1;
    }
}

void scenario_delete_all_custom_variables(void)
{
    for (int i = 0; i < MAX_CUSTOM_VARIABLES; i++) {
        scenario.custom_variables[i].id = i;
        scenario.custom_variables[i].in_use = 0;
        scenario.custom_variables[i].value = 0;
        if (scenario.custom_variables[i].linked_uid) {
            message_media_text_blob_mark_entry_as_unused(scenario.custom_variables[i].linked_uid);
            scenario.custom_variables[i].linked_uid = 0;
        }
    }
}

custom_variable_t *scenario_get_custom_variable(int id)
{
    if (id < 1 || id >= MAX_CUSTOM_VARIABLES) {
        return 0;
    }
    return &scenario.custom_variables[id];
}

int scenario_get_custom_variable_id_by_uid(const uint8_t *variable_uid)
{
    for (int i = 1; i < MAX_CUSTOM_VARIABLES; i++) {
        if (scenario.custom_variables[i].linked_uid) {
            const uint8_t *current_uid = scenario.custom_variables[i].linked_uid->text;
            if (string_equals(current_uid, variable_uid)) {
                return i;
            }
        }
    }
    return 0;
}

const uint8_t *scenario_get_custom_variable_name(int id)
{
    if (id < 1 || id >= MAX_CUSTOM_VARIABLES) {
        return 0;
    }
    if (scenario.custom_variables[id].linked_uid) {
        return scenario.custom_variables[id].linked_uid->text;
    }
    return 0;
}

int scenario_get_custom_variable_value(int id)
{
    return scenario.custom_variables[id].value;
}

void scenario_set_custom_variable_value(int id, int new_value)
{
    scenario.custom_variables[id].value = new_value;
}

int scenario_custom_variable_relink_text_blob(int text_id, text_blob_string_t *new_text_link)
{
    for (int i = 1; i < MAX_CUSTOM_VARIABLES; i++) {
        if (scenario.custom_variables[i].linked_uid &&
            scenario.custom_variables[i].linked_uid->id == text_id) {
            scenario.custom_variables[i].linked_uid = new_text_link;
            return 1;
        }
    }
    return 0;
}
