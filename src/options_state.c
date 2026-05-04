/**
 * Options_State.c - internal functions for handling the options menu.
 */

#include <gb/gb.h>
#include "options_state.h"

/**
 * Top-level function to run the options state.
 */
topLevelState_t RunState_Options( void ) {
	return kTitleScreenState;
}