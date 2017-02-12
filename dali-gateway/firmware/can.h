#ifndef CAN_H
#define	CAN_H

#include <stdint.h>

// CMSGSID data type
typedef struct{
	unsigned SID:11;
	unsigned :21;
}txcmsgsid;

typedef struct{
	unsigned SID:11;
	unsigned FILHIT:5;
	unsigned CMSGTS:16;
}rxcmsgsid;

typedef struct{
	unsigned DLC:4;
	unsigned RB0:1;
	unsigned :3;
	unsigned RB1:1;
	unsigned RTR:1;
	unsigned EID:18;
	unsigned IDE:1;
	unsigned SRR:1;
	unsigned :2;
}cmsgeid;

typedef struct{
	unsigned Byte0:8;
	unsigned Byte1:8;
	unsigned Byte2:8;
	unsigned Byte3:8;
}cmsgdata0;

typedef struct{
	unsigned Byte4:8;
	unsigned Byte5:8;
	unsigned Byte6:8;
	unsigned Byte7:8;
}cmsgdata1;

// Main data structures
typedef union uCANTxMessageBuffer {
	struct{
		txcmsgsid CMSGSID;
		cmsgeid CMSGEID;
		cmsgdata0 CMSGDATA0;
		cmsgdata1 CMSGDATA1;
	};
	int messageWord[4];
}CANTxMessageBuffer;

typedef union uCANRxMessageBuffer {
	struct{
		rxcmsgsid CMSGSID;
		cmsgeid CMSGEID;
		cmsgdata0 CMSGDATA0;
		cmsgdata1 CMSGDATA1;
	};
	int messageWord[4];
}CANRxMessageBuffer;

void can_init(void);
void can_try_rx(void);
CANTxMessageBuffer* can_tx_prepare_buffer(void);
void can_tx_finish_buffer(void);

#endif