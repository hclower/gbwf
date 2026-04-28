/**
 * Error state
 */

#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "error_state.h"

void RunState_Error( uint8_t error_code, uint16_t data ) {
  char errorinfo[16];

  sprintf(errorinfo, "code %d\n", error_code);
  puts(errorinfo);
  sprintf(errorinfo, "data %d\n", data);
  puts(errorinfo);

  return;
}