/**
 * High Score manager
 *
 * Generalized manager for storing/adding high scores.
 * This manages the high score list; displaying of high scores is not done here
 * for portability reaons.
 **/

#include "high_score_manager.h"

/**
 *  Tool to check if the given score should be added to the list.
 *  @args INT16 The newest score
 *  @return TRUE  if newScore is greater than the lowest socre on the list.
 *          FALSE otherwise.
 **/
int isNewHighScore( UINT32 newScore )
{
  // TO DO implement this function, currently just returns 0.
  return 0;
}

int addNewHighScore ( UINT32 newScore, char name[3] ) {
   return 0;
}


int getHighScorePtr ( UINT32 scoreNum,  HighScoreEntry_t *scorePtr ) {
  return 0;
}


int getAllScorePtr  ( HighScoreEntry_t *scoreList[] ) {
  return 0;
}