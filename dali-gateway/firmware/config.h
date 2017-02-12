#ifndef CONFIG_H
#define	CONFIG_H

#include <stdint.h>

#define INT16_INV 0x7fff
#define UINT16_INV 0xffff
#define INT32_INV 0x7fffffff

typedef struct{
	// Config version and checksum. Only used for saving/loading.
    uint16_t checksum;
	uint16_t config_version;
	
}config_t;

typedef struct{

}status_t;

extern config_t config;
extern status_t status;

void config_default_load(void);

#endif
