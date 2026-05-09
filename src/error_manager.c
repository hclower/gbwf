/**
 * Error manager source code
 * Definitions for the error globals and helper functions.
 */

#include "error_manager.h"

errorCode_t global_err_code  = kNoError;
uint16_t    global_err_data = 0;

/**
 * @brief Interface function to set the two error values
 * @details Populates the two global error calues defined in error_manager.h
 *          with the variables provided.
 * 
 * @param error_code Error code to copy into global_err_code
 * @param error_data Error data to copy into global_err_data
 */
void setErrorVars( errorCode_t error_code, uint16_t error_data ) {
  global_err_code = error_code;
  global_err_data = error_data;	
}