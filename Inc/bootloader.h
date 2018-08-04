#ifndef __bootloader_H
#define __bootloader_H

#include "stdint.h"
#include "stdbool.h"
#include <string.h>
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "communication.h"

#define CONFIG_PAGE_NUMB 31
#define CONFIG_PAGE_ADDR 0x08007C00
#define BOOTLOADER_ADDR  0x08000000
#define APPLICATION_ADDR 0x08002800  // 10 kB for bootloader
#define APPLICATION_PAGE_SIZE 21
#define FLASH_END_ADDR   0x08008000
#define SETTINGS_WORDS sizeof(BLDCConfig)/4
	
#define MAX_BASE_VECTORS_NUMB 8

#pragma pack(push, 1)

typedef struct { 
	uint8_t address;
	uint8_t update_firmware;
	uint16_t base_vectors[MAX_BASE_VECTORS_NUMB]; // 8*2 byte
	uint16_t nothing2;
	
} BLDCConfig;

extern BLDCConfig config;


void FLASH_ReadSettings(BLDCConfig *config);
void FLASH_WriteSettings(BLDCConfig *config);
void FLASH_WriteHexLine(uint8_t *data, uint32_t address, uint8_t length);

void erase_flash(void);
	
void jump(const uint32_t program_start_addr);
void shift_interrupt_vectors(uint32_t program_start_addr);

#endif // __bootloader_H
