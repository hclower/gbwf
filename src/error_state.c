/**
 * Error state
 */

#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "error_state.h"
#include "error_manager.h"
#include "gbwf.h"

topLevelState_t RunState_Error( void ) {
  char errorinfo[16];
  puts ("Fatal error:");
  sprintf(errorinfo, " code 0x%x\n", global_err_code);
  puts(errorinfo);
  sprintf(errorinfo, " data   %d\n", global_err_data);
  puts(errorinfo);
  sprintf(errorinfo, "      0x%x\n", global_err_data);
  puts(errorinfo);

  return kErrorState;
}