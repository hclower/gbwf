/**
 * Headers for parallax_bg code. See parallax_bg.c comment for more info.
 */

#ifndef _PARALLAX_BG_H
#define _PARALLAX_BG_H

#include <stdint.h>

void parallaxInit( void );
void parallaxUpdate( const uint8_t y );

// Window location variables
extern uint8_t window_x;
extern uint8_t window_y;

#endif