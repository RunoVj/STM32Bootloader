#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

bool IsChecksumm16bCorrect(uint8_t *msg, uint16_t length);
void AddChecksumm16b(uint8_t *msg, uint16_t length);

bool IsChecksumm8bCorrect(uint8_t *msg, uint16_t length);
void AddChecksumm8b(uint8_t *msg, uint16_t length);

void AddChecksum16bS(uint8_t *msg, uint16_t length);
bool IsChecksum16bSCorrect(uint8_t *msg, uint16_t length);

void CompChecksum(uint8_t *upbyte, uint8_t *lowbyte, uint8_t *msg, uint8_t size);

#endif
