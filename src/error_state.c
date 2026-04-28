/**
 * Error state
 */

#include <gb/gb.h>
#include <stdio.h>
#include "error_state.h"

void RunState_Error( UINT8 error_code, UINT16 data ) {
  char errorinfo[16];

  sprintf(errorinfo, "code %d\n", error_code);
  puts(errorinfo);
  sprintf(errorinfo, "data %d\n", data);
  puts(errorinfo);

  return;
}