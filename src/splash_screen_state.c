/**
 * splash_screen_state.c: Internal functions for the splash screen state.
 */

#include "splash_screen_state.h"
#include "gbwf.h"

 /**
  * Top-level function for running the splash screen.
  */
 topLevelState_t RunState_SplashScreen( void ) {
   return kTitleScreenState;
 }