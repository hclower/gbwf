/**
 * High Score manager
 *
 * Generalized manager for storing/adding high scores.
 * This manages the high score list; displaying of high scores is not done here
 * for portability reaons.
 *
 **/

#ifndef _HIGH_SCORE_MANAGER_H
#define _HIGH_SCORE_MANAGER_H

#include <gb/gb.h>
#include <stdint.h>

#define HS_LIST_LENGTH 10U

struct HighScoreEntry_s {
  uint8_t  name[4];
  uint32_t score;
};

typedef struct HighScoreEntry_s HighScoreEntry_t;

void               initHighScoreList ( void );
uint8_t            isNewHighScore    ( uint32_t newScore );
void               addNewHighScore   ( uint32_t newScore, char name[4] );
HighScoreEntry_t * getHighScorePtr   ( uint32_t score_num );
HighScoreEntry_t * getAllScorePtr    ( void );

#endif