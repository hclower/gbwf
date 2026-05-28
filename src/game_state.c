/**
 * Game state C code
 */

#include "game_state.h"

#include <stdint.h>
#include <stdbool.h>

#include "gbwf.h"
#include "high_score_manager.h"
#include "parallax_bg.h"
#include "../rsrc/cat_tile.h"

enum internalGameState {
  kStandardPlay
};

/* Local function declarations */
void newLevelIntro( void );
void getPlayerMoveInput( const uint8_t joypad_state );
void initGameState ( void );
bool mainGameStateLoop ( void );

typedef enum internalGameState internalGameState_t;

uint32_t score;
uint32_t lives;
uint32_t level;

subpixel_t player_x;
subpixel_t player_y;

// Constants
//  - precalculated parallax scroll info

// File-scope globals:
//  - enemy position
//  - enemy stucture info/tilemap
//  - player position
//  - pbullet array
//  - ebullet array

/**
 * @brief Top-level main game state function
 * @details Top-level function for the main game state. Handles init internally.
 * 
 * @return Returns the new state- either 'game over' or 'high score entry.'
 */
topLevelState_t RunState_Game( void ) {
  topLevelState_t return_value;
  bool enemy_alive = true;

  initGameState();

  while ( 1 ) {
    // Update level & generate new enemy

    // Display new level intro
    newLevelIntro();

    enemy_alive = mainGameStateLoop();

    // If the enemy has died...
    if ( false == enemy_alive ) {
    	// Clear the array of enemy bullets
    	//  - Work in the 'disappearing bullet' animation here?

    	// Run the enemy death visual
    } else {
    	// fade out?

    	// Break the parent loop
    	break;
    }
  }

 
  if( isNewHighScore( score ) ) 
  	return_value = kHighScoreEntryState;
  else return_value = kGameOverState;

  // Go to either Game Over or High Score Entry states
  return return_value;
}

/**
 * @brief Initializes the global variables used in the main game state.
 * @details Initializes the global score, level and lives variables.
 * New enemy generation occurs at the beginning of the next level.
 */
void initGameState( void ) {
  score = 0;
  lives = 3;
  level = 1;

  parallaxInit();

  player_x.pix = 50;
  player_y.pix = 50;
  player_x.subpix = 0;
  player_y.subpix = 0;
  set_sprite_data( 0, 2, cat_tile_tiles );
  set_sprite_tile( 0, 0 );
  set_sprite_tile( 1, 1 );
  move_sprite( 0, player_x.pix, player_y.pix );
  move_sprite( 1, player_x.pix-8, player_y.pix );

  SHOW_BKG;
  SPRITES_8x8;
  SHOW_SPRITES;
}


/**
 * @brief Main game state internal loop. Runs until complete.
 * @details Runs the stage's internal loop until the stage is over.
 *          Returns true or false based on whether the enemy is still alive
 *          at the end of the loop.
 * 
 * @return TRUE if the enemy is still alive (this is game over)
 *         FALSE otherwise (this is a new level)
 */
bool mainGameStateLoop( void ) {
  bool    enemy_alive = 1;
  uint8_t joypad_state;

  // Main state loop
  while( 1 ) {
    // Wait for vsync
    vsync();

    // vsync visual upates.
    // - Sprite positions
    // - Parallax scroll flip/flop & first line
    //   - (this should be a func we can call from elsewhere, we will need to
    //      do it from multiple substates)
    // - Window posiiton
    // - update window scrolling BG tiles
    //   - Either this or the sprite positions are the longest updates, but
    //     we can afford to do this last becaues we have a few lines before
    //     they're used, even if we overrun vsync a little.
    // After this, we should be free to update everything...

    move_sprite( 0, player_x.pix, player_y.pix );
    move_sprite( 1, player_x.pix-8, player_y.pix );
    parallaxUpdate(player_y.pix);

    // Update player position
    //   - (this should be a function, will need during level intro)
    // add new pbullets
    joypad_state = joypad();
    getPlayerMoveInput( joypad_state );


    // Next window position
    // Figure out next set of bg tiles to use for window bg
    // add new ebullets

    // eBullet position & collision detection
    //  - For each bullet, update current position
    //  - Preload new sprite data into an array we can just memcpy() into
    //    sprite memory during vsync
    //  - Enemy bullet vs player collision detection
    // This will likely be the most work-intensive part.

    // publlet poisition & collision detection

    // If part of the enemy is damaged...
    //  - Manage explosion animations
    //  - Start regenerating enemy tilemap
    //    - This should be done last and can be spread across several
    //      frames while the old-pieces-exploding effect runs
  }

  return enemy_alive;
}

/**
 * @brief Runs the new level intro.
 * @details Executes the new level intro. This will continue to run the
 *          background scroll while the intro is executing.
 *          
 *          It is assumed this is run after the next stage enemy has been
 *          generated (but possibly before it is rendered?)
 *          Note that we will need the enemy name for this but there's no 
 *          onscreen bullets and therefore not a lot going on, so potentially 
 *          we could get a name and run generation in the background.
 *          
 *          The player ship is still onscreen and controllable but the player
 *          is prevented from firing.
 *          The idea is that the level number will slide onto the screen
 *          from the top-left at around 1/4 of the way down the screen going
 *          left-to-right, while the enemy name (possibly 2 lines) slides on
 *          right-to-left at around 3/4 of the way down the screen, then they 
 *          both accelerate offscreen before the enemy appears.
 *          I'll use a bubble font for this made of sprites; assuming the player
 *          ship is 2 sprites wide this leaves 8 sprites before I bump against
 *          the 10 sprite/line limit, so lines of text will be capped at 8
 *          characters.
 *          
 *          Depending on how slanty the font ends up being, each line may be
 *          one sprite or two sprites stacked on top of each ther (eg one sprite
 *          is the top half and one sprite is the bottom half of a letter)
 *          
 *          Hypothetically since the enemy will be drawn on the window, it could
 *          slide on while the fonts slide off.
 *          Text could slowly scroll while 'stopped' for a more dynamic visual.
 *          
 *          Midframe OAM DMAs look like they should be possible but may be
 *          glitchy/tricky to get right. Will need to experiment.
 *          
 *          Start/stop/velocity can be algorithmically determined based on
 *          a set of constants.
 */
void newLevelIntro( void ){
  // Initialize new level intro
  //
  // Text Display
  // - Set initial velocity (shared by all words)
  // - While counter < counter_max
  //   - Apply velocity to top text
  //     - If anything scrolls too far offscreen move it to an illegal Y value
  //   - Start DMA of top text sprite info
  //     - If letters are two sprites tall bottom will need to be DMA'd over
  //       midway through each frame
  //   - Apply negative velocity to bottom text
  //     - If anything scrolls too far offscreen move it to an illegal Y value
  //   - Apply Ax to velocity
  //     - If this changes sign, set to min velocity
  //   - Counter checks:
  //     - If counter = pause_start, set ax to 0 and dx to desired value
  //     - If counter = pause_end, set ax to target value
}


/**
 * @brief Initializes everything for the new level into
 * @details Does the following:
 * - Load font
 * - Generate sprite arrays for text
 *   - This will require separate arrats for upper and lower text!
 * - Set global variable that makes BG scroll break midscreen.
 */
void newLevelIntroInit( void ) {
  // - Load font

  // - Generate sprite arrays for text
  //      - This will require separate arrats for upper and lower text!

  // - Set global variable that makes BG scroll break midscreen?
}


/**
 * @brief Reads input and updates the player's position.
 * @details Reads input and updates the player's position.
 *          Applies screen border limits internally.
 */
void getPlayerMoveInput( const uint8_t joypad_state ) {
  if ( joypad_state & J_LEFT )  {
    if( player_x.pix != 8 ) {
      player_x.pix --;
    }
  }

  if ( joypad_state & J_UP )  {
    if( player_y.pix != 16 ) {
      player_y.pix --;
    }
  }

  if ( joypad_state & J_RIGHT ) { 
    if( player_x.pix != 160 ) {
      player_x.pix ++;
    }
  }

  if ( joypad_state & J_DOWN ) {
    if( player_y.pix != 144 ) {
      player_y.pix ++;
    }
  }
}

/* eof */