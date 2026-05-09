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

#include "error_manager.h"

void main(void)
{
  int displayHighScoresFirst = 1;
  topLevelState_t cur_state = kSplashScreenState;

  // Initialize high scores
  initHighScoreList();

  setErrorVars( kTestError, 0x1234 );
  cur_state = kErrorState;

  while( 1 )
  {
    switch ( cur_state )
    {
      case kSplashScreenState:
        // Splash screen- run once and then exit
        cur_state = RunState_SplashScreen();
        break;

      case kTitleScreenState:
        // Title screen - cycles between title screen, high scores, and attract
        cur_state = RunState_TitleScreen( displayHighScoresFirst );

      case kOptionState:
        cur_state = RunState_Options();
        break;

      case kGameState:
        cur_state = RunState_Game();
        break;

      case kHighScoreEntryState:
        cur_state = RunState_HighScoreEntry();

      case kGameOverState:
        cur_state = RunState_GameOver();
        break;

      default:
        cur_state = RunState_Error();
        
        // Do not continue execution after a fatal error.
        while(1);

        break;
      }
    }
}