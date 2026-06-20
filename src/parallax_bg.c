/**
 * Functions related to managing the parallax-scrolling BG in the game state.
 * To run this:
 *  - Call parallax_init()
 *  - Call parallax_update() during VSYNC every frame
 *
 * This manages the window display and hide as well, and redrawing the 
 * background.
 * 
 * Note that parallax_init() will install an ISR function which does the 
 * actual movement. This will not work if that is uninstalled.
 */

#include "parallax_bg.h"

#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "../rsrc/parallax_bg_tiles.h"
#include "../rsrc/win_bg_test.h"

#define INVALID_WIN_X    ((uint8_t) 170)
#define WINDOW_ON_COUNT  ((uint8_t) 2)
#define WINDOW_OFF_COUNT ((uint8_t) 3)

void parallaxDrawBgTiles( void );
void parallaxLoadBgTiles( void );
void parallaxInitBg( void );
void parallaxDrawWinTiles( void );
void parallaxLoadWinTiles( void );
void parallaxInitWin( void );
void parallaxIsr( void );
void parallaxUpdateWinTiles( void );

/* Big tables of predefined data. Pulled from parallax_spreadsheet.ods. */
const uint8_t parallax_int_lines[17][5] = { {  7, 11,  95, 111, 135},
                                            {  8, 13,  97, 112, 135},
                                            {  9, 14,  98, 113, 135},
                                            { 10, 16, 100, 114, 135},
                                            { 11, 17, 101, 115, 135},
                                            { 12, 19, 103, 116, 135},
                                            { 13, 20, 104, 117, 135},
                                            { 14, 22, 106, 118, 135},
                                            { 15, 23, 107, 119, 135},
                                            { 16, 25, 109, 120, 135},
                                            { 17, 26, 110, 121, 135},
                                            { 18, 28, 112, 122, 135},
                                            { 19, 29, 113, 123, 135},
                                            { 20, 31, 115, 124, 135},
                                            { 21, 32, 116, 125, 135},
                                            { 22, 34, 118, 126, 135},
                                            { 23, 35, 119, 127, 135}  };

const uint8_t parallax_y_offsets[17][6] = { {17, 28, 29, 32, 32, 32},
{16, 26, 27, 30, 31, 32},
{15, 25, 26, 29, 30, 32},
{14, 23, 24, 27, 29, 32},
{13, 22, 23, 26, 28, 32},
{12, 20, 21, 24, 27, 32},
{11, 19, 20, 23, 26, 32},
{10, 17, 18, 21, 25, 32},
{9, 16, 17, 20, 24, 32},
{8, 14, 15, 18, 23, 32},
{7, 13, 14, 17, 22, 32},
{6, 11, 12, 15, 21, 32},
{5, 10, 11, 14, 20, 32},
{4, 8, 9, 12, 19, 32},
{3, 7, 8, 11, 18, 32},
{2, 5, 6, 9, 17, 32},
{1, 4, 5, 8, 16, 32} };

/* Constant tile info */
const uint8_t bg_tiles_start    = 1;
const uint8_t bg_tile_count     = 9;
const uint8_t bg_tile_t_dark    = 0;
const uint8_t bg_tile_t_light   = 1;
const uint8_t bg_tile_center_ul = 2;
const uint8_t bg_tile_center_ur = 3;
const uint8_t bg_tile_center_bl = 4;
const uint8_t bg_tile_center_br = 5;
const uint8_t bg_tile_b_light   = 6;
const uint8_t bg_tile_b_dark    = 7;
const uint8_t bg_tile_rows      = 22;
const uint8_t bg_tile_array[23] = { 0, 0, 0, 1, 1, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 6, 6, 7, 7, 7, 8 };

const uint8_t win_bg_tiles_start = 10;
const uint8_t win_bg_tile_count  = 4;  // Only four in tile space; will be recycled.

/* 'live' data for each frame. */
uint8_t parallax_count          = 0;
uint8_t parallax_curframe_count = 0;
uint8_t parallax_cur_slice      = 0;
uint8_t parallax_x_offset[]     = {    0,    0,    0,    0,    0,   0 };
uint8_t parallax_rate[]         = {    0,    1,    2,    1,    0,   0 };
uint8_t parallax_rate_mask[]    = { 0x00, 0x01, 0x03, 0x01, 0x00 };

uint8_t show_window             = 0;
uint8_t window_x                = 100;
uint8_t window_y                = 50;
/**
 * @brief Initializes parallax scrolling.
 * @details Initializes the parallax BG functions.
 *  - Sets initial values
 *  - Loads background tiles and draws BG
 *  - Installs ISR
 * 
 */
void parallaxInit( void ) {
  parallaxInitBg();
  parallaxInitWin();

  /* Set up interrupts for the parallaxing effect */
  CRITICAL {
    STAT_REG = STATF_LYC;
    LYC_REG  = 0; 

    add_LCD(parallaxIsr);
    add_LCD(nowait_int_handler);
  }
  set_interrupts(IE_REG | LCD_IFLAG);
}

/**
 * @brief Updates the parallax scrolling variables during VBLANK.
 * @details Updates the parallax scrolling variables; call this during
 *          VBLANK at the beginning of every frame.
 *    - Calculates the new Y slice
 *    - Sets the first X and Y offsets.
 *    - Sets the Y interrupt reg.
 */
void parallaxUpdate( const uint8_t y ) {
  // Update slice
  parallax_cur_slice = ((y-16) >> 3);
  parallax_curframe_count = 1;
  parallax_count = (parallax_count + 1) & 0x1F;

  // If memory is a concern, here are two other approaches that may work here:
  // 1. Changing the way rate works such that we're determining X offset by setting it to
  //    parallax_count >> rate, where we've removed the limits on count.
  // 2. Making parallax_x_offset a UINT16 and using subpixel animation.
  //    (I think that would be about the same memory, we'get rid of rate_mask but double the
  //     size of x_offset, but it would be slightly faster)
  if ( ( parallax_count & parallax_rate_mask[0] ) == parallax_rate[0] ) parallax_x_offset[0] ++;
  if ( ( parallax_count & parallax_rate_mask[1] ) == parallax_rate[1] ) parallax_x_offset[1] ++;
  if ( ( parallax_count & parallax_rate_mask[2] ) == parallax_rate[2] ) parallax_x_offset[2] ++;
  if ( ( parallax_count & parallax_rate_mask[3] ) == parallax_rate[3] ) parallax_x_offset[3] ++;
  if ( ( parallax_count & parallax_rate_mask[4] ) == parallax_rate[4] ) parallax_x_offset[4] ++;

  // Set initial register values
  LYC_REG = parallax_int_lines [parallax_cur_slice][0];
  SCX_REG = parallax_x_offset                      [0];
  SCY_REG = parallax_y_offsets [parallax_cur_slice][0];

  if ( show_window ) {
    //if ( parallax_count == WINDOW_ON_COUNT ) {
      WY_REG = window_y;
      WX_REG = window_x;
    //} else {
    //  WX_REG = INVALID_WIN_X;
    //}
  }

  parallaxUpdateWinTiles();
  WY_REG = window_y;

  return;
}

/**
 * @brief ISR use by parallax functions.
 * @details Updates SCX and SCY for this slice and sets LYC for the next.
 * 
 */
void parallaxIsr( void ) NONBANKED {
  // Busy wait so we don't change the interrupt regs during the interrupt
  while (STAT_REG & STATF_BUSY) {}

  SCY_REG = parallax_y_offsets [parallax_cur_slice][parallax_curframe_count];
  SCX_REG = parallax_x_offset                      [parallax_curframe_count];


  if( parallax_curframe_count == 2 )
  {
     if( show_window )
     {
          WX_REG = window_x;
          WY_REG = window_y;
          SHOW_WIN;
     }     
  }
  if( parallax_curframe_count == 3 )
  {
     WX_REG = 1;
     HIDE_WIN;
  }


  // Set X offset reg for the current strip.
  LYC_REG = parallax_int_lines[parallax_cur_slice][parallax_curframe_count];

  // Update curframe count
  if( 5 != parallax_curframe_count )
  {
     parallax_curframe_count ++;
  }	
}

/**
 * @brief Initializes the background
 * @details Loads and draws the background tiles.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 */
void parallaxInitBg( void ) {
  parallaxLoadBgTiles();
  parallaxDrawBgTiles();	
}

/**
 * @brief Loads the BG tiles
 * @details Loads the BG tiles to use.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 * 
 * @todo Look up how inline works, consider defining this as inline for speed.
 */
void parallaxLoadBgTiles( void ) {
    set_bkg_data( bg_tiles_start, bg_tile_count, parallax_bg_tiles_tiles );	
}

/**
 * @brief Draws the BG tiles
 * @details Draws the lines of BG tiles.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 * 
 * @todo Look up how inline works, consider defining this as inline for speed.
 */
void parallaxDrawBgTiles( void ) {
  uint8_t i;
  uint8_t j;
  uint8_t bg_line[32];

  for( i = 0; i < bg_tile_rows; i ++ ) {
    if( bg_tile_array[i] == bg_tile_center_ul ||
        bg_tile_array[i] == bg_tile_center_bl ) {

      for( j = 0; j < 32; j ++ ) {
        bg_line[j] = bg_tiles_start + bg_tile_array[i] + (j & 0x01);
      }
    }
    else { 
        memset(bg_line, bg_tiles_start + bg_tile_array[i], 32 );
    }
    set_bkg_tiles( 0,  i, 32, 1, bg_line );
  }
}


/**
 * @brief Initializes the window
 * @details Loads and draws the background tiles used on the window.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 *          
 *          I'm not too concerned because it's only executed once, but if time
 *          is a concern it'd probably be a bit faster to piggyback drawing the
 *          window with paralladDrawBgTiles().
 */
void parallaxInitWin( void ) {
  parallaxLoadWinTiles();
  parallaxDrawWinTiles();

  show_window = 1; 
}


/**
 * @brief Loads the BG tiles
 * @details Loads the BG tiles to use.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 * 
 * @todo Look up how inline works, consider defining this as inline for speed.
 */
void parallaxLoadWinTiles( void ) {
    // This loads the test tiles, which are disticnt from the noral BG.
    set_bkg_data( win_bg_tiles_start, 
                  2,
                  win_bg_test_tiles );
    set_bkg_data( win_bg_tiles_start + 2, 
                  2,
                  &(win_bg_test_tiles[48]) );
                  // Real tiles
                  //&(parallax_bg_tiles_tiles[16*bg_tile_center_ul]) );
}

/**
 * @brief Draws the BG tiles
 * @details Draws the lines of BG tiles.
 *          Currently uses constants defined elsewhere in this file; it may be
 *          desirable to move these to a separate header at some point.
 * 
 * @todo Look up how inline works, consider defining this as inline for speed.
 */
void parallaxDrawWinTiles( void ) {
  uint8_t i;
  uint8_t j;
  uint8_t bg_line[32];

  for( i = 0; i < bg_tile_rows; i ++ ) {
      for( j = 0; j < 32; j ++ ) {
        bg_line[j] = win_bg_tiles_start + (i & 0x01) + ((j & 0x01)<<1);
      }
 
    set_win_tiles( 0,  i, 32, 1, bg_line );
  }
}

/**
 * @brief Redraws the tiles used for the background in tile lookup space.
 * @details Updates the four tiles used to draw the window background so that 
 *          they line up with the normal BG tiles. Uses precalculated BG tiles.
 *          This doesn't strictly need to be called during VBLANK but should 
 *          complete before the Window layer starts drawing.
 */
void parallaxUpdateWinTiles( void ) {
  uint8_t new_win_bg_tiles[64];
  uint8_t y_offset;
  uint8_t x_offset;
  uint8_t y_tile_target;
  uint8_t x_tile_target;

  // Figure out target Y offset based on window_y.
  // This is the offset within the tile, 
  y_offset = window_y & 0x07;

  // Figure out target X offset based on window_x
  x_offset = window_x & 0x07;
  //x_offset = 0;

  // Do the copies (unroll loops for speed- from earlier experiments, doing a 
  // for() loop here adds up surprisingly quickly)

  // Generailzed formula for finding a byte:
  //   (((x offset * total y tiles * 8) + y offset) * 2) + Tile Offset + Byte
  // For a 2x2 gid, Tile Offset will be 0, 16, 48, and 64 for tiles 1, 2, 3, & 4
  new_win_bg_tiles[ 0] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  0];
  new_win_bg_tiles[ 1] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  1];
  new_win_bg_tiles[ 2] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  2];
  new_win_bg_tiles[ 3] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  3];
  new_win_bg_tiles[ 4] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  4];
  new_win_bg_tiles[ 5] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  5];
  new_win_bg_tiles[ 6] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  6];
  new_win_bg_tiles[ 7] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  7];
  new_win_bg_tiles[ 8] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  8];
  new_win_bg_tiles[ 9] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 +  9];
  new_win_bg_tiles[10] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 10];
  new_win_bg_tiles[11] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 11];
  new_win_bg_tiles[12] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 12];
  new_win_bg_tiles[13] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 13];
  new_win_bg_tiles[14] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 14];
  new_win_bg_tiles[15] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) +  0 + 15];

  new_win_bg_tiles[16] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  0];
  new_win_bg_tiles[17] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  1];
  new_win_bg_tiles[18] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  2];
  new_win_bg_tiles[19] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  3];
  new_win_bg_tiles[20] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  4];
  new_win_bg_tiles[21] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  5];
  new_win_bg_tiles[22] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  6];
  new_win_bg_tiles[23] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  7];
  new_win_bg_tiles[24] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  8];
  new_win_bg_tiles[25] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 +  9];
  new_win_bg_tiles[26] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 10];
  new_win_bg_tiles[27] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 11];
  new_win_bg_tiles[28] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 12];
  new_win_bg_tiles[29] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 13];
  new_win_bg_tiles[30] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 14];
  new_win_bg_tiles[31] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 16 + 15];

  new_win_bg_tiles[32] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  0];
  new_win_bg_tiles[33] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  1];
  new_win_bg_tiles[34] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  2];
  new_win_bg_tiles[35] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  3];
  new_win_bg_tiles[36] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  4];
  new_win_bg_tiles[37] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  5];
  new_win_bg_tiles[38] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  6];
  new_win_bg_tiles[39] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  7];
  new_win_bg_tiles[40] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  8];
  new_win_bg_tiles[41] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 +  9];
  new_win_bg_tiles[42] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 10];
  new_win_bg_tiles[43] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 11];
  new_win_bg_tiles[44] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 12];
  new_win_bg_tiles[45] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 13];
  new_win_bg_tiles[46] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 14];
  new_win_bg_tiles[47] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 48 + 15];

  new_win_bg_tiles[48] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  0];
  new_win_bg_tiles[49] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  1];
  new_win_bg_tiles[50] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  2];
  new_win_bg_tiles[51] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  3];
  new_win_bg_tiles[52] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  4];
  new_win_bg_tiles[53] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  5];
  new_win_bg_tiles[54] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  6];
  new_win_bg_tiles[55] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  7];
  new_win_bg_tiles[56] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  8];
  new_win_bg_tiles[57] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 +  9];
  new_win_bg_tiles[58] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 10];
  new_win_bg_tiles[59] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 11];
  new_win_bg_tiles[60] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 12];
  new_win_bg_tiles[61] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 13];
  new_win_bg_tiles[62] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 14];
  new_win_bg_tiles[63] = win_bg_test_tiles[ (((x_offset * 6 * 8 ) + y_offset)*2) + 64 + 15];

  // Write local new tiles buffer into tile memory
  x_tile_target = (((window_x & 0xf) >> 3) << 1);
  y_tile_target =  ((window_y & 0xf) >> 3);

  set_bkg_data( win_bg_tiles_start +      y_tile_target  +      x_tile_target,
                1,
                &(new_win_bg_tiles[0]) );

  set_bkg_data( win_bg_tiles_start + (1 - y_tile_target) +      x_tile_target,
                1,
                &(new_win_bg_tiles[16]) );

  set_bkg_data( win_bg_tiles_start +      y_tile_target  + (2 - x_tile_target), 
                1,
                &(new_win_bg_tiles[32]) );

  set_bkg_data( win_bg_tiles_start + (1 - y_tile_target) + (2 - x_tile_target), 
                1,
                &(new_win_bg_tiles[48]) );

//      set_bkg_data( win_bg_tiles_start, 
//                    1,
//                    &(new_win_bg_tiles[0]) );
}

/* eof */