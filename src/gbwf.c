#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "gbwf.h"

#include "splash_screen_state.h"
#include "title_screen_state.h"
#include "options_state.h"
#include "game_state.h"
#include "high_score_entry_state.h"
#include "game_over_state.h"
#include "error_state.h"
#include "high_score_manager.h"

#include "error_codes.h"

uint32_t score;

void main(void)
{
  int displayHighScoresFirst = 1;
  int return_value = 0;

  // Splash screen- run once and then exit
  RunState_SplashScreen();

  // Initialize high scores
  initHighScoreList();

  while( 1 )
  {
    // Title screen - cycles between title screen, high scores, and attract
    return_value = RunState_TitleScreen( displayHighScoresFirst );

    switch (return_value)
    {
      case OPTIONS_STATE:
        RunState_Options();
        break;
      case GAME_STATE:
        RunState_Game();

        if ( isNewHighScore (score) )
           RunState_HighScoreEntry();

        RunState_GameOver();
        break;
      default:
        RunState_Error( kBadState, return_value );
        break;
      }
    }
}