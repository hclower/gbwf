/**
 * title_screen_state.c - internal functions for handling the title screen.
 */

#include <gb/gb.h>
#include <stdint.h>

#include <stdio.h>

#include "title_screen_state.h"
#include "high_score_manager.h"
#include "gbwf.h"
#include "gbwf_utilities.h"


/**
 * Top-level function to run the title screen state.
 */
topLevelState_t RunState_TitleScreen( uint8_t start_at_hiscores ) {
	start_at_hiscores = 0;

	return kGameState;
}