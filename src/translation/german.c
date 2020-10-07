#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "Patch 1.0.1.0 ist nicht installiert"},
    {TR_NO_PATCH_MESSAGE,
        "Deine Caesar 3 Installation hat den Patch 1.0.1.0 nicht installiert. "
        "Der Patch kann hier heruntergeladen werden:\n"
        URL_PATCHES "\n"
        "Fahre auf eigene Gefahr fort."},
    {TR_MISSING_FONTS_TITLE, "Schriftarten fehlen"},
    {TR_MISSING_FONTS_MESSAGE,
        "Deine Caesar 3 Installation benötigt zusätzliche Schriftarten. "
        "Du kannst Schriftarten für deine Sprache hier herunterladen:\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "Editor nicht installiert"},
    {TR_NO_EDITOR_MESSAGE,
        "Deiner Caesar 3 Installation fehlen die Dateien für den Editor. "
        "Du kannst die fehlenden Dateien hier herunterladen:\n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "Ungültiges Sprachverzeichnis"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "Das gewählte Verzeichnis enthält kein gültiges Sprachpaket. "
        "Bitte überprüfe das Protokoll auf Fehler."},
    {TR_BUTTON_OK, "OK"},
    {TR_BUTTON_CANCEL, "Abbrechen"},
    {TR_BUTTON_RESET_DEFAULTS, "Zurücksetzen"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Tastenkombinationen"},
    {TR_CONFIG_TITLE, "Julius Einstellungen"},
    {TR_CONFIG_LANGUAGE_LABEL, "Sprache:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(Standard)"},
    {TR_CONFIG_HEADER_UI_CHANGES, "Benutzeroberfläche ändern"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Spielablauf ändern"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Intro-Videos abspielen"},
    {TR_CONFIG_SIDEBAR_INFO, "Aktiviere zusätzliche Informationen im rechtsseitigen Bedienfeld"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Aktiviere sanftes Scrollen"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Beim Planieren der Landfläche das visuelle Feedback verbessern"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Erlaube alle Tempel nacheinander zu bauen"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Zeige Reichweite beim Bauen von Reservoirs und Brunnen"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Zeige Konstruktionsgröße beim Bauen"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Hebe Legion beim Berühren mit der Maus hervor"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Behebe Immigrationsfehler auf 'Sehr schwierig'"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Behebe '100 Jahre alte Geister'"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "Behebe Wechsel- und Überlebenszeit des Imperators in eigenen Missionen"},
    {TR_HOTKEY_TITLE, "Julius Tastenkombinationen einstellen"},
    {TR_HOTKEY_LABEL, "Tastenkombination"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Alternative"},
    {TR_HOTKEY_HEADER_ARROWS, "Pfeiltasten"},
    {TR_HOTKEY_HEADER_GLOBAL, "Globale Tastenkombinationen"},
    {TR_HOTKEY_HEADER_CITY, "Stadt"},
    {TR_HOTKEY_HEADER_ADVISORS, "Berater"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Spezialkarten"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "Lesezeichen für Stadtkarten"},
    {TR_HOTKEY_HEADER_EDITOR, "Editor"},
    {TR_HOTKEY_HEADER_BUILD, "Bau-Tastenkombinationen"}, // TODO: Google Translate
    {TR_HOTKEY_ARROW_UP, "Hoch"},
    {TR_HOTKEY_ARROW_DOWN, "Runter"},
    {TR_HOTKEY_ARROW_LEFT, "Links"},
    {TR_HOTKEY_ARROW_RIGHT, "Rechts"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "Vollbildanzeige an/abschalten"},
    {TR_HOTKEY_CENTER_WINDOW, "Fenster zentrieren"},
    {TR_HOTKEY_RESIZE_TO_640, "Fenstergröße: 640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "Fenstergröße: 800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "Fenstergröße: 1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "Screenshot speichern"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "Stadtweiten Screenshot sp."},
    {TR_HOTKEY_BUILD_CLONE, "Gebäude unter Cursor klonen"}, // TODO: Google Translate
    {TR_HOTKEY_LOAD_FILE, "Datei laden"},
    {TR_HOTKEY_SAVE_FILE, "Datei speichern"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "Spielgeschwindigkeit erhöhen"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "Spielgeschwindigkeit reduzieren"},
    {TR_HOTKEY_TOGGLE_PAUSE, "Pause umschalten"},
    {TR_HOTKEY_CYCLE_LEGION, "Durch Legionen wechseln"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "Karte nach links drehen"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "Karte nach rechts drehen"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "Berater für Arbeit"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "Berater für Legionen"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "Berater für den Imperator"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "Berater für Wertungen"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "Berater für Handel"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "Berater für Bevölkerung"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "Berater für Gesundheit"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "Berater für Bildung"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "Berater für Unterhaltung"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "Berater für Religion"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "Berater für Finanzen"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "Oberster Berater"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Akt. Spezialkarte umschalten"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Wasserkarte"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Feuerkarte"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Schadenskarte"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Kriminalitätskarte"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Problemkarte"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Gehe zu Lesezeichen 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Gehe zu Lesezeichen 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Gehe zu Lesezeichen 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Gehe zu Lesezeichen 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Setze Lesezeichen 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Setze Lesezeichen 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Setze Lesezeichen 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Setze Lesezeichen 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Kampfinformationen umschalten"},
    {TR_HOTKEY_EDIT_TITLE, "Neue Tastenkombination"}
};

void translation_german(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
