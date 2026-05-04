/**
 * Gloabl GBWF constants & defines
 */

#ifndef _GBWF_H
#define _GBWF_H

#include <stdint.h>
#include "error_manager.h"

enum topLevelState {
  kSplashScreenState   = 0,
  kTitleScreenState    = 1,
  kGameState           = 2,
  kGameOverState       = 3, 
  kHighScoreEntryState = 4,
  kOptionState         = 5,
  kErrorState          = 6
};

typedef enum topLevelState topLevelState_t;

#define OPTIONS_STATE 0U
#define GAME_STATE    1U

extern uint32_t score;

#endif