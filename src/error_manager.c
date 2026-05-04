/**
 * Error manager source code
 * Definitions for the error globals and helper functions.
 */

#include "error_manager.h"

errorCode_t global_err_code  = kNoError;
uint16_t    global_err_data = 0;

/**
 * Sets the two error variables
 *
 * 
 */
void setErrorVars( errorCode_t error_code, uint16_t error_data ) {
  global_err_code = error_code;
  global_err_data = error_data;	
}