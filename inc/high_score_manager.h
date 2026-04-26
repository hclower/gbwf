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

#define HS_LIST_LENGTH 10U

struct HighScoreEntry_s {
  UINT8  name[3];
  UINT32 score;
};

typedef struct HighScoreEntry_s HighScoreEntry_t;

int isNewHighScore  ( UINT32 newScore );
int addNewHighScore ( UINT32 newScore, char name[3] );
int getHighScorePtr ( UINT32 scoreNum,  HighScoreEntry_t *scorePtr );
int getAllScorePtr  ( HighScoreEntry_t *scoreList[] );

#endif