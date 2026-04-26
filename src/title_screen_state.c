/**
 * title_screen_state.c - internal functions for handling the title screen.
 */

#include <gb/gb.h>

#include "title_screen_state.h"
#include "gbwf.h"

/**
 * Top-level function to run the title screen state.
 */
UINT8 RunState_TitleScreen( UINT8 start_at_hiscores ) {
	return GAME_STATE;
}