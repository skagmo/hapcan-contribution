#include "config.h"

#include <stdint.h>

config_t config;
status_t status;

//uint8_t config_flash_load(void){
//    volatile unsigned char value = 0x09;
//    unsigned char address = 0xE5;
//    eeprom_write(address, value);     // Writing value 0x9 to EEPROM address 0xE5        
//    value = eeprom_read (address);    // Reading the value from address 0xE5
//	
//	config_t temp;
//	uint8_t* temp_u8 = &temp;
//
//	for (j=0; j<sizeof(config_t); j++){
//		
//	}
//	if ( (flash_config->checksum == crc_ccitt((uint8_t*)APP_CONFIG_FLASH_ADDRESS+2, sizeof(config_t)-2)) &&
//	     (flash_config->config_version == CONFIG_VERSION) ){
//		memcpy(&config, (void*)APP_CONFIG_FLASH_ADDRESS, sizeof(config_t));
//		return RET_OK;
//	}
//	return RET_ERROR;
//}
//
//uint8_t config_flash_save(void){
//	unsigned int j, k;
//	uint32_t* config_32 = (uint32_t*)&config;
//
//	// Calculate CRC
//	config.checksum = crc_ccitt((uint8_t*)&config+2, sizeof(config_t)-2);
//		
//	// Erase whole page in flash
//	if (NVMErasePage((uint8_t*)APP_CONFIG_FLASH_ADDRESS)) return RET_ERROR;
//
//	for (j=0; j<sizeof(config_t); j+=4){
//		if ( NVMWriteWord( (uint32_t*)(APP_CONFIG_FLASH_ADDRESS+j), config_32[j/4]) ) return RET_ERROR;
//	}
//
//	return RET_OK;
//}
//	