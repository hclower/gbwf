/**
 * title_screen_state.c - internal functions for handling the title screen.
 */

#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>

#include "title_screen_state.h"
#include "high_score_manager.h"
#include "gbwf.h"
#include "gbwf_utilities.h"

void displayHighScores( void );


/**
 * Top-level function to run the title screen state.
 */
uint8_t RunState_TitleScreen( uint8_t start_at_hiscores ) {
	start_at_hiscores = 0;
	displayHighScores();
	return GAME_STATE;
}

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