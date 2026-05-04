/**
 * High score entry state
 */

#include "high_score_entry_state.h"

#include <gb/gb.h>
#include <stdio.h>

#include "gbwf.h"
#include "gbwf_utilities.h"
#include "high_score_manager.h"

topLevelState_t RunState_HighScoreEntry( void ) {
	return kTitleScreenState;
}

/** Retained test code from elsewhere, kept for reference. */

/**
 * Function to display high scores.
 */
void displayHighScores( void ) {
  int i;
  char cur_score_str[32];
  HighScoreEntry_t *cur_score;

  cur_score = getAllScorePtr();

  for( i = (HS_LIST_LENGTH-1); i >= 0; i -- ) {
  	sprintf ( cur_score_str, "%s\t%d",  cur_score[i].name, cur_score[i].score );
  	puts (cur_score_str);
  }

  waitForInput();

  puts("");

  addNewHighScore( 1500, "HLC" );

  for( i = (HS_LIST_LENGTH-1); i >= 0; i -- ) {
  	sprintf ( cur_score_str, "%s\t%d",  cur_score[i].name, cur_score[i].score );
  	puts (cur_score_str);
  }

  waitForInput();

  puts("");

  addNewHighScore( 5500, "RJC" );

  for( i = (HS_LIST_LENGTH-1); i >= 0; i -- ) {
  	sprintf ( cur_score_str, "%s\t%d",  cur_score[i].name, cur_score[i].score );
  	puts (cur_score_str);
  }

  waitForInput();

  puts("");

  addNewHighScore( 15000, "JPC" );

  for( i = (HS_LIST_LENGTH-1); i >= 0; i -- ) {
  	sprintf ( cur_score_str, "%s\t%d",  cur_score[i].name, cur_score[i].score );
  	puts (cur_score_str);
  }


  while(1) vsync();
}