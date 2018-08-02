#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "messages.h"
#include "stdbool.h"

void response(uint8_t status); 
bool parse_package(uint8_t *msg, uint8_t length);

#endif //__COMMUNICATION_H
