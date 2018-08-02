#include <stdint.h>
#include <stdlib.h>
#include "usart.h"

#include <string.h>
#include "communication.h"
#include "checksum.h"
#include "bootloader.h"

uint8_t msg_buf[RESPONSE_LENGTH];

uint32_t flash_address = 0;

bool parse_package(uint8_t *msg, uint8_t length)
{
	if (IsChecksumm8bCorrect(msg, length)) {
		struct FirmwaregRequest req;
		memcpy((void*)&req, (void*)msg, 9);
		memcpy((void*)&req, (void*)msg, req.hex._data_size + 9);
		if (req.type != FIRMWARE_REQUEST_TYPE) {
			return false;
		}
		switch (req.hex.operation_type) {
			case WRITE_HEX:
				flash_address = (flash_address & 0xFFFF0000) | req.hex.start_address;
				FLASH_WriteHexLine(req.hex.data, flash_address, req.hex._data_size);
			break;
			
			case ADDRESS_OFFSET_TYPE:
				flash_address = (uint32_t)(req.hex.data[0] << 24 | req.hex.data[1] << 16);
			break;
			
			case END_OF_HEX:
				HAL_FLASH_Lock();
				config.update_firmware = false;
				FLASH_WriteSettings(&config);
				jump(APPLICATION_ADDR);
			break;
		}

		if (req.get_response) {
			return true;
		}
  }
	return false;	
}

void response(uint8_t status)
{
	struct FirmwareResponse resp;
	resp.AA = 0xAA;
	resp.type = FIRMWARE_REQUEST_TYPE;
	resp.address = 0;
	resp.status = status;

	memcpy((void*)msg_buf, (void*)&resp, RESPONSE_LENGTH - 1);
	AddChecksumm8b(msg_buf, RESPONSE_LENGTH);	

  HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
	
  HAL_UART_Transmit_IT(&huart1, msg_buf, RESPONSE_LENGTH);	
}
