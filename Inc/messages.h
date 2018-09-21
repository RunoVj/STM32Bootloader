#ifndef __MESSAGES_H
#define __MESSAGES_H

/* STM send requests and VMA send responses */

#define REQUEST_LENGTH   12  
#define CONFIG_REQUEST_LENGTH  13
#define RESPONSE_LENGTH  5
#define PACKAGE_STATIC_SIZE 11
#define RECEIVE_TIMEOUT  5
#define REQUEST_DATA_SIZE_POS 5
#define MAX_DATA_SIZE 0x10

#define ADDRESS_OFFSET_TYPE 0x04
#define WRITE_HEX 0x00
#define END_OF_HEX 0x01

#define CONFIG_REQUEST_TYPE 0x02
#define FIRMWARE_REQUEST_TYPE 0x03

#define NORMAL_REQUEST_TYPE 0x01
#define CONFIG_REQUEST_TYPE 0x02

#include "stdint.h"

#pragma pack(push, 1)

struct Request
{
	uint8_t AA; // 0xAA doesn't include in CRC calculation
	uint8_t type; // 0x01
	uint8_t address;
	uint8_t update_base_vector; // true or false
	uint8_t position_setting; // enabling of position_setting
	uint16_t angle; // angle - 0..359;
	int8_t velocity;
	uint8_t frequency;
	int16_t outrunning_angle;
	uint8_t crc;
};

struct ConfigRequest
{
    uint8_t AA;
    uint8_t type; // 0x02
    uint8_t update_firmware; // (bool) set new address or update firmware even if old address doesn't equal BLDC address
    uint8_t forse_setting; // (bool) set new address even if old address doesn't equal BLDC address
    uint8_t old_address;
    uint8_t new_address;
    uint16_t high_threshold;
    uint16_t low_threshold;
    uint16_t average_threshold;
    uint8_t crc;
};

typedef struct 
{
		uint8_t _data_size;
		uint16_t start_address;
		uint8_t operation_type;
		uint8_t data[MAX_DATA_SIZE];
		uint8_t crc;
} IntelHEX;

struct FirmwaregRequest
{
    uint8_t AA;
    uint8_t type; // 0x03
    uint8_t address;
	  uint8_t force_update; // update even if address doesn't equal BLDC address
    uint8_t get_response; // send status
    //hex

    IntelHEX hex;

    uint8_t crc;
};

struct Response
{
    uint8_t AA;
    uint8_t type;
    uint8_t address;
    uint8_t state;
    uint8_t position_code;
    uint16_t cur_angle;
    uint16_t current;
    uint8_t crc;
};

struct FirmwareResponse
{
    uint8_t AA;
    uint8_t type; // 0x03
    uint8_t address;
    uint8_t status;
    uint8_t crc;
};


#pragma pack(pop)

#endif //__MESSAGES_H
