/**
 * Game state C code
 */

#include "game_state.h"

topLevelState_t RunState_Game( void ) {

	// Go to either Game Over or High Score Entry states
	return kGameOverState;
}
