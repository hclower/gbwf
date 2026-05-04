/**
 * Error codes for gbwf
 */

#ifndef _ERROR_MANAGER_H
#define _ERROR_MANAGER_H

#include <stdint.h>

enum errorCode_e {
	kNoError = 0,
	kBadState = 1,
	kTestError = 0x1A
};

typedef enum errorCode_e errorCode_t;

extern errorCode_t global_err_code;
extern uint16_t     global_err_data;

void setErrorVars( errorCode_t error_code, uint16_t error_data );

#endif