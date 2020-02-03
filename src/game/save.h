#ifndef GAME_SAVE_H
#define GAME_SAVE_H

#include <stdint.h>

/**
 * @file
 * Manages quicksaves and autosaves
 */

/**
  * Quicksaves a city
  */
void game_save_quicksave(void);

/**
  * Quickloads a city
  */
void game_save_quickload(void);

/**
  * Autosaves a city
  */
void game_save_autosave(void);

/**
  * Prepares the quick/autosave for a new city
  */
void game_save_prepare(const uint8_t *name);

#endif // GAME_SAVE_H