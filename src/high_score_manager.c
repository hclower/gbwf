/**
 * High Score manager
 *
 * Generalized manager for storing/adding high scores.
 * This manages the high score list; displaying of high scores is not done here
 * for portability reaons.
 * 
 * General note: high_score_list[0] is the lowest score, 
 * high_score_list[HS_LIST_LENGTH] is the highest.
 **/

#include "high_score_manager.h"
#include <stdint.h>

HighScoreEntry_t high_score_list[HS_LIST_LENGTH];

/**
 * Initialize the high score list. Will preload with sample scores.
 * Eventually this should probably go into one of the battery-backed RAM banks.
 * 
 * @args   none
 * @return none
 */
void initHighScoreList( void )
{
  uint32_t i;

  for( i = 0; i < HS_LIST_LENGTH; i ++ ) {
    high_score_list[i].name[0] = 'A'+i;
    high_score_list[i].name[1] = 'A'+i;
    high_score_list[i].name[2] = 'A'+i;
    high_score_list[i].name[3] = '\0';
    high_score_list[i].score   = ((i+1)*1000);
  }

  return;
}

/**
 *  Tool to check if the given score should be added to the list.
 * 
 *  @args   uint32_t The newest score
 *  @return TRUE  if newScore is greater than the lowest socre on the list.
 *          FALSE otherwise.
 **/
uint8_t isNewHighScore( UINT32 newScore )
{
  if( newScore > high_score_list[0].score )
    return 0;
  else return 1;
}

/**
 * Add the provided new high score to the list in the correct place, and bump
 * all other scores down. The bottom score will be removed from the list.
 * 
 * If the score passed in is lower than the lowest score, this function
 * silently does nothing.
 * 
 * @args  
 */
void addNewHighScore ( uint32_t new_score, char new_name[3] ) {
  int i;

  if ( new_score > high_score_list[0].score ) {
    for ( i = 0; i < HS_LIST_LENGTH; i ++ ) {
      if (i != 0 ) {
        high_score_list[i-1].name[0] = high_score_list[i].name[0];
        high_score_list[i-1].name[1] = high_score_list[i].name[1];
        high_score_list[i-1].name[2] = high_score_list[i].name[2];
        high_score_list[i-1].score   = high_score_list[i].score;
      }

      if ( (i != (HS_LIST_LENGTH-1) ) &&
           (new_score < high_score_list[i+1].score) )
        break;
    }

    // Inelegant and gross!
    if ( i == HS_LIST_LENGTH ) i --;

    high_score_list[i].name[0] = new_name[0];
    high_score_list[i].name[1] = new_name[1];
    high_score_list[i].name[2] = new_name[2];
    high_score_list[i].score   = new_score;
  }

  return;
}

/**
 *  Returns a pointer to the high score number provided.
 * 
 * @arg scoreNum The score number to get (0 is the lowest on the list)
 * @return A pointer to the score, or null of the score is out of range.
 */
HighScoreEntry_t * getHighScorePtr ( uint32_t score_num ) {
  if (score_num < HS_LIST_LENGTH ) {
    return &(high_score_list[score_num] );
  } 

  return (HighScoreEntry_t *) NULL;
}


/**
 *  Returns a pointer to the high score list.
 * 
 * @return A pointer to the high score list.
 */
HighScoreEntry_t * getAllScorePtr    ( void ) {
  return high_score_list;
}