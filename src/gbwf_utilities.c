/**
 * GBWF Utilities
 */

#include "gbwf_utilities.h"
#include <gb/gb.h>

/**
 * Waits unitl the A or B buttons are pressed.
 */
void waitForInput( void ) {
  uint8_t joypad_state;
  
  while( 1 )
  {
  	joypad_state = joypad();

  	if( ( joypad_state & J_A ) ||
  	    ( joypad_state & J_B )    )
  		break;
    vsync();
  }
}