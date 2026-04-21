#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "Export.h"
//#include "sprites.h"
#include "face_sprite.h"

// Constants
#define LEFT_TILE_X       ((UINT8)  5)
#define LEFT_TILE_Y       ((UINT8) 12)
#define UP_TILE_X         ((UINT8)  6)
#define UP_TILE_Y         ((UINT8) 11)
#define RIGHT_TILE_X      ((UINT8)  7)
#define RIGHT_TILE_Y      ((UINT8) 12)
#define DOWN_TILE_X       ((UINT8)  6)
#define DOWN_TILE_Y       ((UINT8) 13)
#define A_TILE_X          ((UINT8) 13)
#define A_TILE_Y          ((UINT8) 12)
#define B_TILE_X          ((UINT8) 15)
#define B_TILE_Y          ((UINT8) 12)
#define SL_TILE_X         ((UINT8)  9)
#define SL_TILE_Y         ((UINT8) 13)
#define SL_LABEL_TILE_X   ((UINT8)  9)
#define SL_LABEL_TILE_Y   ((UINT8) 14)
#define ST_TILE_X         ((UINT8) 11)
#define ST_TILE_Y         ((UINT8) 13)
#define ST_LABEL_TILE_X   ((UINT8) 11)
#define ST_LABEL_TILE_Y   ((UINT8) 14)


// Tile IDs
#define TILE_SMILEY          ((UINT8)  1)
#define TILE_LEFT_BTN        ((UINT8)  2)
#define TILE_UP_BTN          ((UINT8)  3)
#define TILE_RIGHT_BTN       ((UINT8)  4)
#define TILE_DOWN_BTN        ((UINT8)  5)
#define TILE_SS_BTN          ((UINT8)  6)
#define TILE_A_BTN           ((UINT8)  7)
#define TILE_B_BTN           ((UINT8)  8)
#define TILE_SL_LABEL        ((UINT8)  9)
#define TILE_ST_LABEL        ((UINT8) 10)
#define TILE_LEFT_BTN_DOWN   ((UINT8) 11)
#define TILE_UP_BTN_DOWN     ((UINT8) 12)
#define TILE_RIGHT_BTN_DOWN  ((UINT8) 13)
#define TILE_DOWN_BTN_DOWN   ((UINT8) 14)
#define TILE_SS_BTN_DOWN     ((UINT8) 15)
#define TILE_A_BTN_DOWN      ((UINT8) 16)
#define TILE_B_BTN_DOWN      ((UINT8) 17)
#define TILE_SMILE_TL        ((UINT8) 18)
#define TILE_SMILE_TR        ((UINT8) 19)
#define TILE_SMILE_BL        ((UINT8) 20)
#define TILE_SMILE_BR        ((UINT8) 21)

int faceX;
int faceY;

void initHelloWorld( void );

void main(void)
{
    uint8_t joypadState;
    puts("");
    puts("Hello world");

    // Init portion
    initHelloWorld();

    // Loop forever
    while(1) {


		// Game main loop processing goes here
        joypadState = joypad();

        if ( joypadState & J_LEFT ) 
        {
             set_bkg_tile_xy(  LEFT_TILE_X,  LEFT_TILE_Y,    TILE_LEFT_BTN_DOWN );
             if (faceX != 16) faceX --;
        }
        else set_bkg_tile_xy(  LEFT_TILE_X,  LEFT_TILE_Y,    TILE_LEFT_BTN );

        if ( joypadState & J_UP ) 
        {
             set_bkg_tile_xy(    UP_TILE_X,    UP_TILE_Y,    TILE_UP_BTN_DOWN );
             if (faceY != 16) faceY --;
        }
        else set_bkg_tile_xy(    UP_TILE_X,    UP_TILE_Y,    TILE_UP_BTN );

        if ( joypadState & J_RIGHT ) 
        { 
             set_bkg_tile_xy( RIGHT_TILE_X, RIGHT_TILE_Y,    TILE_RIGHT_BTN_DOWN );
             if (faceX != 150 ) faceX ++;
        }
        else set_bkg_tile_xy( RIGHT_TILE_X, RIGHT_TILE_Y,    TILE_RIGHT_BTN );    

        if ( joypadState & J_DOWN ) 
        {
             set_bkg_tile_xy(  DOWN_TILE_X,  DOWN_TILE_Y,    TILE_DOWN_BTN_DOWN );
             if (faceY != 154) faceY ++;
        }
        else set_bkg_tile_xy(  DOWN_TILE_X,  DOWN_TILE_Y,    TILE_DOWN_BTN );

        if ( joypadState & J_START ) 
             set_bkg_tile_xy(  ST_TILE_X,  ST_TILE_Y,    TILE_SS_BTN_DOWN );
        else set_bkg_tile_xy(  ST_TILE_X,  ST_TILE_Y,    TILE_SS_BTN );

        if ( joypadState & J_SELECT) 
             set_bkg_tile_xy(  SL_TILE_X,  SL_TILE_Y,    TILE_SS_BTN_DOWN );
        else set_bkg_tile_xy(  SL_TILE_X,  SL_TILE_Y,    TILE_SS_BTN );

        if ( joypadState & J_A ) 
             set_bkg_tile_xy(  A_TILE_X,  A_TILE_Y,    TILE_A_BTN_DOWN );
        else set_bkg_tile_xy(  A_TILE_X,  A_TILE_Y,    TILE_A_BTN );

        if ( joypadState & J_B ) 
             set_bkg_tile_xy(  B_TILE_X,  B_TILE_Y,    TILE_B_BTN_DOWN );
        else set_bkg_tile_xy(  B_TILE_X,  B_TILE_Y,    TILE_B_BTN );

        move_sprite( 0, faceX,     faceY );
        move_sprite( 1, faceX + 8, faceY );
		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}

void initHelloWorld ( void )
{
    // Set tile data
    set_bkg_data(1, 21, TileLabel);

    // Set tile values
    set_bkg_tile_xy(     LEFT_TILE_X,     LEFT_TILE_Y,  TILE_LEFT_BTN );
    set_bkg_tile_xy(       UP_TILE_X,       UP_TILE_Y,    TILE_UP_BTN );    
    set_bkg_tile_xy(    RIGHT_TILE_X,    RIGHT_TILE_Y, TILE_RIGHT_BTN );
    set_bkg_tile_xy(     DOWN_TILE_X,     DOWN_TILE_Y,  TILE_DOWN_BTN );
    set_bkg_tile_xy(       SL_TILE_X,       SL_TILE_Y,    TILE_SS_BTN );
    set_bkg_tile_xy(       ST_TILE_X,       ST_TILE_Y,    TILE_SS_BTN );
    set_bkg_tile_xy( SL_LABEL_TILE_X, SL_LABEL_TILE_Y,  TILE_SL_LABEL );
    set_bkg_tile_xy( ST_LABEL_TILE_X, ST_LABEL_TILE_Y,  TILE_ST_LABEL );
    set_bkg_tile_xy(        A_TILE_X,        A_TILE_Y,     TILE_A_BTN );
    set_bkg_tile_xy(        B_TILE_X,        B_TILE_Y,     TILE_B_BTN );

//    set_bkg_tile_xy( 5, 5, TILE_SMILE_TL );
//    set_bkg_tile_xy( 6, 5, TILE_SMILE_TR );
//    set_bkg_tile_xy( 5, 6, TILE_SMILE_BL );
//    set_bkg_tile_xy( 6, 6, TILE_SMILE_BR );


    // Set sprite data
    faceX = 50;
    faceY = 50;


    SHOW_SPRITES;
    SPRITES_8x16;
//    set_sprite_data( 0, 4, SpriteLabel );
    set_sprite_data( 0, 4, face_sprite_tiles );

    set_sprite_tile( 0, 0 );
    set_sprite_tile( 1, 2 );

    move_sprite( 0, faceX,     faceY );
    move_sprite( 1, faceX + 8, faceY );
}