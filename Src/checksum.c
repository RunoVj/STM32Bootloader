#include "checksum.h"

/* CRC16-CCITT algorithm */
bool IsChecksumm16bCorrect(uint8_t *msg, uint16_t length)
{
  int i;
	uint16_t crc = 0, crc_got = (uint16_t)((msg[length - 2] << 8) + msg[length - 1]);
	
		for(i = 0; i < length - 2; ++i){
			crc = (uint8_t)(crc >> 8) | (crc << 8);
			crc ^= msg[i];
			crc ^= (uint8_t)(crc & 0xff) >> 4;
			crc ^= (crc << 8) << 4;
			crc ^= ((crc & 0xff) << 4) << 1;
		}
	
	if(crc == crc_got ){
    return 1;
  }
	else{
    return 0;
  }
}

/* CRC16-CCITT algorithm */
void AddChecksumm16b(uint8_t *msg, uint16_t length)
{
	uint16_t crc = 0;
	int i = 0;
	
	for(i = 0; i < length - 2; ++i){
			crc = (uint8_t)(crc >> 8) | (crc << 8);
			crc ^= msg[i];
			crc ^= (uint8_t)(crc & 0xff) >> 4;
			crc ^= (crc << 8) << 4;
			crc ^= ((crc & 0xff) << 4) << 1;
		}
	
	msg[length - 2] = (uint8_t) (crc >> 8);
	msg[length - 1] = (uint8_t) crc;
}

bool IsChecksumm8bCorrect(uint8_t *msg, uint16_t length)
{
	uint8_t crcGot, crc = 0;
	int i;
	
	crcGot = msg[length-1] ;
	
	for (i = 1; i < length - 1; ++i) {
		crc ^= msg[i];
	}
	
	if (crc == crcGot) {
		return 1;
	}
	else {
		return 0;
	}
}

void AddChecksumm8b(uint8_t *msg, uint16_t length)
{
	uint8_t crc = 0;
	
	for(int i = 1; i < length - 1; i++) {
		crc ^= msg[i];
	}
	
	msg[length-1] = crc;
}

void AddChecksum16bS(uint8_t *msg, uint16_t length)
{
	uint16_t checksum = 0;
  for(uint8_t i = 0; i < length; ++i) {
    checksum += msg[i];
  }
	msg[length - 2] = (uint8_t) (checksum >> 8);
	msg[length - 1] = (uint8_t) checksum;
}

bool IsChecksum16bSCorrect(uint8_t *msg, uint16_t length)
{
	uint16_t checksum = 0, checksum_in = (uint16_t) ((msg[length - 2] << 8) + (msg[length - 1]));
  for(uint8_t i = 0; i < length-2; ++i) {
    checksum += msg[i];
  }
	if(checksum == checksum_in) {
		return true;
	}
	else {
		return false;
	}
}

void CompChecksum(uint8_t *upbyte, uint8_t *lowbyte, uint8_t *msg, uint8_t size)
{
	uint16_t checksum = 0;
	for(uint8_t i=0; i<size; i++)
		checksum += (uint16_t) msg[i];
		
	*lowbyte = (uint8_t) ((checksum & 0xFF00) >> 8);
	*upbyte = (uint8_t) (checksum & 0x00FF);
}	
