#include "bootloader.h"

BLDCConfig config;

void jump(const uint32_t program_start_addr)
{
	// make functor
	typedef void(*func_ptr)(void); 
	func_ptr program_func;
	
	__disable_irq();
	
	uint32_t reset_addr = *( uint32_t*) (program_start_addr + 4);  // RESET address
	program_func = (func_ptr)reset_addr; 
	
	__set_MSP(*(__IO uint32_t*) program_start_addr); // set main stack pointer  
	
	// go to new program
	program_func();		      	
}

void erase_flash()
{
	HAL_FLASH_Unlock();
	// errase page
	FLASH_EraseInitTypeDef errase_conf;
	errase_conf.TypeErase = FLASH_TYPEERASE_PAGES; // errase only 1 page
	errase_conf.Banks = FLASH_BANK_1;
	errase_conf.PageAddress = (uint32_t)APPLICATION_ADDR;
	errase_conf.NbPages = APPLICATION_PAGE_SIZE;
	
	uint32_t page_error;
	HAL_FLASHEx_Erase(&errase_conf, &page_error);
	HAL_FLASH_Lock();
}

void shift_interrupt_vectors(uint32_t program_start_addr)
{
	__set_PRIMASK(1);

	SCB->VTOR = program_start_addr;

	__set_PRIMASK(0);
}


void FLASH_ReadSettings(BLDCConfig *config)
{
	uint32_t *source_addr = (uint32_t *)CONFIG_PAGE_ADDR;
	uint32_t *dest_addr = (void *)config;
	
	for (uint16_t i = 0; i < SETTINGS_WORDS; ++i) {
		*dest_addr = *(__IO uint32_t*)source_addr;
		source_addr++;
		dest_addr++;
	}
}

void FLASH_WriteHexLine(uint8_t *data, uint32_t address, uint8_t length)
{
	HAL_FLASH_Unlock();
	uint32_t *source_addr = (void *)data;
	uint32_t *dest_addr = (uint32_t *)address;
	
	for (uint8_t i = 0; i < length/4; ++i) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)dest_addr, *source_addr);
		source_addr++;
		dest_addr++;
	}
	HAL_FLASH_Lock();
}

void FLASH_WriteSettings(BLDCConfig *config)
{
	// Write settings
	HAL_FLASH_Unlock();

	// errase page
	FLASH_EraseInitTypeDef errase_conf;
	errase_conf.TypeErase = FLASH_TYPEERASE_PAGES; // errase only 1 page
	errase_conf.PageAddress = (uint32_t)CONFIG_PAGE_ADDR;
	errase_conf.NbPages = 1;
	
	uint32_t page_error;
	HAL_FLASHEx_Erase(&errase_conf, &page_error);
	
	// write page
	uint32_t *source_addr = (void *)config;
	uint32_t *dest_addr = (uint32_t *)CONFIG_PAGE_ADDR;
	for (uint8_t i = 0; i < SETTINGS_WORDS; ++i) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)dest_addr, *source_addr);
		source_addr++;
		dest_addr++;
	}
	
	HAL_FLASH_Lock();
}
