/**
 * Gloabl GBWF constants & defines
 */

#ifndef _GBWF_H
#define _GBWF_H

#include <stdint.h>

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

union subpixel_u{
  uint16_t u16;
  struct {
    uint8_t pix;
    uint8_t subpix;
  };
};

typedef union subpixel_u subpixel_t;

#define OPTIONS_STATE 0U
#define GAME_STATE    1U

extern uint32_t score;

#endif